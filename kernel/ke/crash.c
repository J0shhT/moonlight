/**
 * PROJECT:		Project Moonlight Kernel
 * FILE:		kernel/ke/crash.c
 * PURPOSE:		Crash support functions for the kernel
 * 
 * AUTHORS: 	Josh Theriault (joshriaul@gmail.com)
 * UPDATED:		1/05/2022
 */

#include <boot/i386/interrupt.h>
#include <ke/crash.h>

#include <arch/i386/intrinsic.h>
#include <ke/exception.h>
#include <kd/debug.h>
#include <drivers/video.h>

#include <string.h>
#include <stdarg.h>

#define MAX_CRASH_MESSAGE_SIZE 64
#define MAX_CRASH_MESSAGE_COUNT 128
static SIZE KiCrashMessageCount = 0;
static SIZE KiCrashMessageIds[MAX_CRASH_MESSAGE_COUNT] = { 0 };
static CHAR KiCrashMessages[MAX_CRASH_MESSAGE_COUNT * MAX_CRASH_MESSAGE_SIZE] = { 0 };

STR KiGetCrashMessageEntry(SIZE index)
{
	if (index >= MAX_CRASH_MESSAGE_COUNT)
		return NULL;
	return &KiCrashMessages[index * MAX_CRASH_MESSAGE_SIZE];
}

KSTATUS KeRegisterCrashMessage(SIZE messageId, CSTR messageStr)
{
	if (KiCrashMessageCount >= MAX_CRASH_MESSAGE_COUNT || messageStr == NULL)
		return STATUS_ERROR;
	// TODO: Validate parameters (message is a proper string, id is not in use already, etc...)

	KiCrashMessageIds[KiCrashMessageCount] = messageId; // Set error code ID for finding later
	strncpy(KiGetCrashMessageEntry(KiCrashMessageCount), messageStr, MAX_CRASH_MESSAGE_SIZE - 1); // Copy crash message into KiCrashMessages
	++KiCrashMessageCount;

	return STATUS_SUCCESS;
}

#define KiRegisterExceptionMessage(e, msg) KeRegisterCrashMessage(KiExceptionCodeToCrashCode(e), (msg))

void KiInitializeSystemCrashMessages(void)
{
	if (KiCrashMessageCount != 0) return; // Already initialized?

	// KeRegisterCrashMessage(0, "UNKNOWN_CRASH_REASON");

	//* System Exception Messages
	{
		KiRegisterExceptionMessage(EXCEPTION_RESERVED, 					"RESERVED_EXCEPTION");
		KiRegisterExceptionMessage(EXCEPTION_DIVIDE_BY_ZERO, 			"DIVIDE_BY_ZERO");
		KiRegisterExceptionMessage(EXCEPTION_DEBUG_STEP, 				"DEBUG_SINGLE_STEP");
		KiRegisterExceptionMessage(EXCEPTION_NMI, 						"UNHANDLED_NMI");
		KiRegisterExceptionMessage(EXCEPTION_DEBUG_BREAK, 				"DEBUG_BREAKPOINT");
		KiRegisterExceptionMessage(EXCEPTION_INTEGER_OVERFLOW, 			"INTEGER_OVERFLOW");
		KiRegisterExceptionMessage(EXCEPTION_BOUNDS_EXCEEDED, 			"BOUNDS_EXCEEDED");
		KiRegisterExceptionMessage(EXCEPTION_ILLEGAL_INSTRUCTION, 		"ILLEGAL_INSTRUCTION");
		KiRegisterExceptionMessage(EXCEPTION_DEVICE_UNAVAILABLE, 		"DEVICE_UNAVAILABLE");
		KiRegisterExceptionMessage(EXCEPTION_UNHANDLED_FAULT, 			"UNHANDLED_FAULT");
		KiRegisterExceptionMessage(EXCEPTION_DEVICE_SEGMENT_OVERRUN, 	"DEVICE_SEGMENT_OVERRUN");
		KiRegisterExceptionMessage(EXCEPTION_INVALID_TSS, 				"INVALID_TSS");
		KiRegisterExceptionMessage(EXCEPTION_SEGMENT_NOT_PRESET, 		"SEGMENT_NOT_PRESENT");
		KiRegisterExceptionMessage(EXCEPTION_STACK_FAULT, 				"STACK_FAULT");
		KiRegisterExceptionMessage(EXCEPTION_ACCESS_VIOLATION, 			"ACCESS_VIOLATION");
		KiRegisterExceptionMessage(EXCEPTION_PAGE_FAULT, 				"PAGE_FAULT");
		KiRegisterExceptionMessage(EXCEPTION_FPU, 						"FPU_EXCEPTION");
		KiRegisterExceptionMessage(EXCEPTION_ALIGNMENT_CHECK, 			"ALIGNMENT_CHECK");
		KiRegisterExceptionMessage(EXCEPTION_MACHINE_CHECK, 			"MACHINE_CHECK");
		KiRegisterExceptionMessage(EXCEPTION_SIMD, 						"SIMD_EXCEPTION");
		KiRegisterExceptionMessage(EXCEPTION_VIRTUALIZATION, 			"VIRTUALIZATION_EXCEPTION");
		KiRegisterExceptionMessage(EXCEPTION_CONTROL_PROTECTION, 		"CONTROL_PROTECTION");
		KiRegisterExceptionMessage(EXCEPTION_HYPERVISOR, 				"HYPERVISOR_EXCEPTION");
		KiRegisterExceptionMessage(EXCEPTION_VMM_COMMUNICATION, 		"VMM_COMMUNICATION_FAULT");
		KiRegisterExceptionMessage(EXCEPTION_SECURITY, 					"SECURITY_EXCEPTION");
	}

	//* Kernel Exception Messages
	{
		KeRegisterCrashMessage(STATUS_ASSERTION_FAILED, 				"ASSERTION_FAILED");
		KeRegisterCrashMessage(STATUS_PORT_UNAVAILABLE,					"PORT_UNAVAILABLE");
		KeRegisterCrashMessage(0xDEAD, 									"MANUALLY_INITIATED_CRASH");
	}
}

KSTATUS KiGetCrashMessage(SIZE messageId, STR buffer)
{
	if (buffer == NULL)
		return STATUS_ERROR;

	SIZE index = MAX_CRASH_MESSAGE_COUNT;
	for (SIZE i = 0; i < index; ++i)
	{
		if (KiCrashMessageIds[i] == messageId)
		{
			index = i;
			break;
		}
	}

	if (index >= MAX_CRASH_MESSAGE_COUNT)
		return STATUS_ERROR;
	
	strncpy(buffer, KiGetCrashMessageEntry(index), MAX_CRASH_MESSAGE_SIZE - 1); // Copy crash message to buffer
	buffer[MAX_CRASH_MESSAGE_SIZE - 1] = '\0';
	return STATUS_SUCCESS;
}

_No_Return void KiSystemCrash(DWORD err, KCRASH_EXTRAS *extras, KTRAP_FRAME *ctx)
{
	__cli(); // Disable interrupts

	CHAR buffer[MAX_CRASH_MESSAGE_SIZE] = { 0 };
	KSTATUS status = KiGetCrashMessage(err, &buffer[0]);

	CSTR message = &buffer[0];
	if (KERROR(status))
		message = "UNKNOWN_CRASH_REASON";

	// Log the crash
	if (extras != NULL)
	{
		if (err == STATUS_ASSERTION_FAILED)
			KLOGF_FATAL("*** Assertion Failed: %s (file %s line %d)\n", (CSTR)(extras->arg1), (CSTR)(extras->arg2), (int)(extras->arg3));
		else
			KLOGF_FATAL("*** System Crash (0x%x, 0x%x, 0x%x, 0x%x)\n", err, extras->arg1, extras->arg2, extras->arg3);
	}
	else
	{
		KLOGF_FATAL("*** System Crash (0x%x)\n", err);
	}
	
	// Setup the crash screen
	VidResetDisplay();
	VidFillRect(VID_COLOR_RED, 0, 0, VidGetWidth(), VidGetHeight());
	VidSetBackColor(VID_COLOR_TRANSPARENT);
	VidSetTextColor(VID_COLOR_WHITE);
	KdPrintLine("\n A fatal system error has occured!");
	KdPrintLine(" The computer has been stopped to prevent damage. Restart to continue.\n");

	// Display the error message and any extras
	if (extras != NULL && err != STATUS_ASSERTION_FAILED)
	{
		KdPrintf(" %s (0x%x, 0x%x, 0x%x, 0x%x)\n\n ", message, err, extras->arg1, extras->arg2, extras->arg3);
	}
	else
	{
		KdPrintf(" %s (0x%x)\n\n ", message, err);
	}

	if (err == STATUS_ASSERTION_FAILED)
	{
		KdTitle("TECHNICAL INFORMATION");
		KdPrintf("\n %s, %s:%d\n", (CSTR)(extras->arg1), (CSTR)(extras->arg2), (int)(extras->arg3));
	}
	else if (ctx != NULL) // Display the trap frame if available
	{
		KdTitle("TECHNICAL INFORMATION");
		KdPrintf("\n EIP=0x%08x\n\n EAX=0x%08x EBX=0x%08x ECX=0x%08x\n EDX=0x%08x ESI=0x%08x EDI=0x%08x\n ESP=0x%08x EBP=0x%08x\n\n",
			ctx->eip,
			ctx->eax, ctx->ebx, ctx->ecx,
			ctx->edx, ctx->esi, ctx->edi,
			ctx->esp, ctx->ebp);

		KdPrintf(" CS=0x%02x DS=0x%02x ES=0x%02x FS=0x%02x GS=0x%02x\n",
			ctx->cs & 0xFFFF, ctx->ds & 0xFFFF, ctx->es & 0xFFFF, ctx->fs & 0xFFFF, ctx->gs & 0xFFFF);

		KdPrintf(" EFLAGS=0x%08x ERROR=0x%02x\n\n",
			ctx->eflags, ctx->error);
		
		DWORD *eip0, *eip1, *eip2, *eip3;
		eip0 = (DWORD*)ctx->eip, eip1 = (DWORD*)(ctx->eip+4), eip2 = (DWORD*)(ctx->eip+8), eip3 = (DWORD*)(ctx->eip+12);

		KdPrintf(" 0x%08x: %02x %02x %02x %02x\n"
			" 0x%08x: %02x %02x %02x %02x\n"
			" 0x%08x: %02x %02x %02x %02x\n"
			" 0x%08x: %02x %02x %02x %02x\n",
			(ctx->eip+0), (*eip0) & 0xFF, (*eip0 >> 8) & 0xFF, (*eip0 >> 16) & 0xFF, (*eip0 >> 24) & 0xFF,
			(ctx->eip+4), (*eip1) & 0xFF, (*eip1 >> 8) & 0xFF, (*eip1 >> 16) & 0xFF, (*eip1 >> 24) & 0xFF,
			(ctx->eip+8), (*eip2) & 0xFF, (*eip2 >> 8) & 0xFF, (*eip2 >> 16) & 0xFF, (*eip2 >> 24) & 0xFF,
			(ctx->eip+12), (*eip3) & 0xFF, (*eip3 >> 8) & 0xFF, (*eip3 >> 16) & 0xFF, (*eip3 >> 24) & 0xFF);
	}
	
	// Finally, hang the system
	KLOG_INFO("Halting system\n");
	__hang_processor();
}

_No_Return void KeSystemCrash(DWORD err)
{
	KiSystemCrash(err, NULL, NULL);
}

_No_Return void KeSystemCrashVa(DWORD err, KTRAP_FRAME *ctx, SIZE argc, ...)
{
	KCRASH_EXTRAS extras;
	KeInitializeCrashExtras(&extras);

	if (argc > MAX_CRASH_EXTRAS_COUNT)
		argc = MAX_CRASH_EXTRAS_COUNT;
	
	// Copy arguments to extras struct
	va_list args;
	va_start(args, argc);
	for(SIZE i = 0; i < argc; ++i)
		extras.args[i] = va_arg(args, DWORD_PTR);
	va_end(args);

	KiSystemCrash(err, &extras, ctx);
}

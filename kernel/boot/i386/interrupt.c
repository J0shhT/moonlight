#include <boot/i386/interrupt.h>
#include <arch/i386/intrinsic.h>
#include <boot/i386/pic.h>

#include <ke/crash.h>
#include <ke/exception.h>
#include <kd/debug.h>

#include <assert.h>

void KiHandleException(KTRAP_FRAME *ctx)
{
	KEXCEPTION_CODE exception = ctx->ivec;
	KLOGF_FATAL("System exception 0x%x has occured\n", exception);

	if (exception >= 31 || exception == 15 || (exception > 21 && exception < 28))
	{
		// Exception is undefined or reserved by Intel
		KeSystemCrashCtxEx(EXCEPTION_RESERVED, ctx, ctx->ivec); // arg1 is the exception code (interrupt vector)
	}
	else
	{
		// Exception is valid
		DWORD code = KiExceptionCodeToCrashCode(exception);
		switch (exception)
		{
			default:
				KeSystemCrashCtx(code, ctx);
				break;
		}
	}

	// Should never reach here!
	assert(false); //TODO: replace with something better?
	while (1) {}
}

static volatile KSERVICE_ROUTINE KiIrqRoutines[16] = { NULL };

void KiSetInterruptRoutine(DWORD irq, KSERVICE_ROUTINE handler)
{
	__IRQ_SAVE();
	assert(irq < (sizeof(KiIrqRoutines) / sizeof(KSERVICE_ROUTINE))); //TODO: replace with a kernel assert
	KiIrqRoutines[irq] = handler;
	KLOGF_INFO("IRQ %u installed with service routine 0x%08x\n", irq, (DWORD_PTR)(handler));
	__IRQ_RESTORE();
}

void KiHandleInterrupt(KTRAP_FRAME *ctx)
{
	PicSignalEnd(ctx->ivec);

	volatile DWORD irqn = ctx->ivec; // IRQ number
	ctx->ivec += 32; // Vector number = IRQ number + 32

	if (irqn != 0)
	{
		KLOGF_DEBUG("KiHandleInterrupt: IRQ %u (vector %u)\n", irqn, ctx->ivec);
	}
	
	// Check for custom IRQ handler
	KSERVICE_ROUTINE handler = (KSERVICE_ROUTINE)KiIrqRoutines[irqn];
	if (handler != NULL)
	{
		(*handler)(ctx); // Execute custom handler
	}
}

/**
 * PROJECT:		Project Moonlight Kernel
 * FILE:		kernel/boot/kinit.c
 * PURPOSE:		Initialization for the kernel during very early system startup
 * 
 * AUTHORS: 	Josh Theriault (joshriaul@gmail.com)
 * UPDATED:		1/05/2022
 */

#include <boot/i386/gdt.h>
#include <boot/i386/idt.h>
#include <drivers/video.h>
#include <drivers/i386/serial.h>

#include <rtl/string.h>
#include <kd/debug.h>
#include <ke/crash.h>

#include <assert.h>

#include <boot/i386/multiboot.h>
#define MB_HAS_INFO(flag) ((mb->flags & (flag)) != 0)

static inline const char *KiGetMemoryMapTypeString(DWORD type)
{
	static const char *types[] = { "RESERVED", "AVAILABLE", "RECLAIMABLE", "DEFECTIVE" };
	switch (type)
	{
		case MULTIBOOT_MEMORY_AVAILABLE:
			return types[1];
		case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
			return types[2];
		case MULTIBOOT_MEMORY_BADRAM:
			return types[3];
		default:
			return types[0];
	}
}

static inline void KiPrintMemoryMap(multiboot_memory_map_t *mmapBase, int length)
{
	//KdTitle("BASIC MEMORY MAP");
	multiboot_memory_map_t *mmap = mmapBase;
	while (length > 0)
	{
		KLOGF_INFO("multiboot_memory_map: 0x%08x to 0x%08x is %s\n",
			mmap->addr_low, mmap->addr_low + mmap->len_low - 1, KiGetMemoryMapTypeString(mmap->type));

		length -= (mmap->size + 4);
		mmap = (multiboot_memory_map_t*)((DWORD_PTR)mmap + (mmap->size + 4));
	}
}

static inline void KiPrintMultibootInfo(multiboot_info_t *mb)
{
	if (MB_HAS_INFO(MULTIBOOT_INFO_MEMORY))
	{
		KLOGF_INFO("multiboot_info: %u KB of low address memory is available\n", mb->mem_lower);
		KLOGF_INFO("multiboot_info: %u KB of high address memory is available\n", mb->mem_upper);
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_BOOTDEV))
	{
		KLOGF_INFO("multiboot_info: Boot device is 0x%08x\n", mb->boot_device);
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_CMDLINE))
	{
		CSTR cmdline = (CSTR)(mb->cmdline);
		if (RtlStringLength(cmdline) > 0)
		{
			KLOGF_INFO("multiboot_info: Command line is \"%s\"\n", cmdline);
		}
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_MODS))
	{
		KLOG_INFO("multiboot_info: Boot modules are available\n");
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_AOUT_SYMS))
	{
		KLOG_INFO("multiboot_info: Raw symbols are available\n");
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_ELF_SHDR))
	{
		KLOG_INFO("multiboot_info: ELF sections are available\n");
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_MEM_MAP))
	{
		KiPrintMemoryMap((multiboot_memory_map_t*)(mb->mmap_addr), mb->mmap_length);
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_DRIVE_INFO))
	{
		KLOG_INFO("multiboot_info: Drives are available\n");
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_CONFIG_TABLE))
	{
		KLOG_INFO("multiboot_info: Configuration table is available\n");
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_BOOT_LOADER_NAME))
	{
		KLOGF_INFO("multiboot_info: Boot loader is \"%s\"\n", (CSTR)(mb->boot_loader_name));
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_APM_TABLE))
	{
		KLOG_INFO("multiboot_info: APM table is available\n");
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_VBE_INFO))
	{
		KLOG_INFO("multiboot_info: VBE table is available\n");
	}

	if (MB_HAS_INFO(MULTIBOOT_INFO_FRAMEBUFFER_INFO))
	{
		KLOG_INFO("multiboot_info: Framebuffer is available\n");
	}

	//KdPrintLine();
}

/**
 * This is called during very early system startup!
 * The current state of the system is as follows:
 * - The stack is initialized for 16KiB.
 * - Interrupts are disabled.
 * - Global C/C++ constructors have NOT been called yet.
 */
void kernel_init(DWORD mbMagic, DWORD_PTR mbInfoAddr)
{
	SeInitializePort(SE_PORT_COM1);
	KiInitializeSystemCrashMessages();

	KiLoadGlobalDescriptorTable();
	KiLoadInterruptDescriptorTable();
	
	VidInitialize();
	VidFillBack(VID_COLOR_BLUE, 3, 10, 50, 6);

	assert(mbMagic == MULTIBOOT_BOOTLOADER_MAGIC);
	multiboot_info_t *mb = (multiboot_info_t*)mbInfoAddr;
	KiPrintMultibootInfo(mb);
}
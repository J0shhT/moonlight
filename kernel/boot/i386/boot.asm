; Multiboot specification constants
%include "boot/i386/multiboot.inc"
MB_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MB_MAGIC	equ MULTIBOOT_HEADER_MAGIC
MB_CHECKSUM	equ -(MB_MAGIC + MB_FLAGS)

; Multiboot Header
section .multiboot
align 4
	dd MB_MAGIC
	dd MB_FLAGS
	dd MB_CHECKSUM

; Allocate stack space
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

%include "arch/i386/platform.inc"

; System entry point
section .text
global _start:function (_start.end - _start)
_start:
		cli ; Disable interrupts
		cld ; Clear direction flag
		mov esp, stack_top ; Setup the stack
		xor ebp, ebp ; Set NULL frame pointer

		; Prepare function parameters for kernel_init()
		push ebx ; Parameter 2: Multiboot information struct
		push eax ; Parameter 1: Multiboot magic value

		; Initialize kernel
		extern kernel_init
		call kernel_init

		; Cleanup function parameters from kernel_init()
		pop eax
		pop ebx

		; Call C/C++ global constructors
		extern _init
		call _init

		; Transfer control to the kernel
		extern kernel_main
		call kernel_main

	.loop:
		hlt
		jmp .loop

		; Call C/C++ global deconstructors
		; extern _fini
		; call _fini

		call __hang_processor ; Wait forever
.end:

global __hang_processor:function (__hang_processor.end - __hang_processor)
__hang_processor:
		save_caller

	.loop:
		cli ; Disable interrupts
		hlt ; Halt the CPU (wait for next interrupt)
		jmp .loop ; Jump back if awaken (from a NMI)

		restore_caller
		ret
.end:

; global __divide_by_zero:function
; __divide_by_zero:
;	save_caller
;
; 	mov edx, 0
; 	mov eax, 1
; 	mov ecx, 0
; 	div ecx
;
;	restore_caller
; 	ret
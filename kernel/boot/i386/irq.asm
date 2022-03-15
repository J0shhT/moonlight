section .text

%macro define_irq 1
global __irq%1
__irq%1:
	;cli ; disable interrupts
	push 0 ; push dummy error code
	push %1 ; push IRQ number
	jmp __irq ; jump to common code
%endmacro

define_irq 0
define_irq 1
define_irq 2
define_irq 3
define_irq 4
define_irq 5
define_irq 6
define_irq 7
define_irq 8
define_irq 9
define_irq 10
define_irq 11
define_irq 12
define_irq 13
define_irq 14
define_irq 15

__irq:
	pushad ; save processor state

	; save old data segments
	push ds
	push es
	push fs
	push gs

	; switch to kernel data segments
	mov eax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov eax, esp ; save pointer to ProcessorState (for param 1)
	push eax ; Push parameter 1 - ProcessorState *cpu

	cld ; Clear direction flag (required by System V ABI)

	; Transfer control to handler
	extern KiHandleInterrupt
	call KiHandleInterrupt

	pop eax ; Pop parameter 1

	; restore old data segments
	pop gs
	pop fs
	pop es
	pop ds

	popad ; restore processor state

	pop eax ; pop IRQ number
	pop eax ; pop error code
	;sti ; enable interrupts
	iret ; interrupt return

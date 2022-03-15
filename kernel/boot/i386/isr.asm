section .text

%macro interrupt_isr 1
global __isr%1
__isr%1:
	;cli ; disable interrupts
	push 0 ; push dummy error code
	push %1 ; push vector number
	jmp __isr ; jump to common code
%endmacro

%macro exception_isr 1
global __isr%1
__isr%1:
	;cli ; disable interrupts
	push %1 ; push vector number
	jmp __isr ; jump to common code
%endmacro

interrupt_isr 0 	; #DE Divide-by-zero Error
interrupt_isr 1 	; #DB Debug
interrupt_isr 2 	; Non-maskable Interrupt
interrupt_isr 3 	; #BP Breakpoint
interrupt_isr 4 	; #OF Overflow
interrupt_isr 5 	; #BR Bound Range Exceeded
interrupt_isr 6 	; #UD Invalid Opcode
interrupt_isr 7 	; #NM Device Not Available
exception_isr 8 	; #DF Double Fault
interrupt_isr 9 	; Coprocessor Segment Overrun
exception_isr 10 	; #TS Invalid TSS
exception_isr 11 	; #NP Segment Not Present
exception_isr 12 	; #SS Stack-Segment Fault
exception_isr 13 	; #GP General Protection Fault
exception_isr 14 	; #PF Page Fault
interrupt_isr 15	; Intel Reserved
interrupt_isr 16	; #MF x87 Floating-Point Exception
exception_isr 17 	; #AC Alignment Check
interrupt_isr 18 	; #MC Machine Check
interrupt_isr 19 	; #XM SIMD Floating-Point Exception
interrupt_isr 20 	; #VE Virtualization Exception
exception_isr 21 	; #CP Control Protection Exception
interrupt_isr 22	; Intel Reserved
interrupt_isr 23	; Intel Reserved
interrupt_isr 24	; Intel Reserved
interrupt_isr 25	; Intel Reserved
interrupt_isr 26	; Intel Reserved
interrupt_isr 27	; Intel Reserved
interrupt_isr 28	; #HV Hypervisor Injection Exception
exception_isr 29	; #VC VMM Communication Exception
exception_isr 30	; #SX Security Exception
interrupt_isr 31	; Intel Reserved

__isr:
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
	extern KiHandleException
	call KiHandleException

	pop eax ; Pop parameter 1

	; restore old data segments
	pop gs
	pop fs
	pop es
	pop ds

	popad ; restore processor state

	pop eax ; pop vector number
	pop eax ; pop error code
	;sti ; enable interrupts
	iret ; interrupt return

section .text

global __lgdt ; void __lgdt(DWORD_PTR gdt);
__lgdt:
	mov eax, [esp+4] ; eax = DWORD_PTR gdt
	lgdt [eax] ; Load the new GDT (pointed to by eax)
	; Update segment selectors
	mov ax, 0x10 ; 0x10 = New data selector (0000000000010 0 00)
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	; Long jump to flush CPU prefetch cache
	jmp 0x08:.flush ; 0x08 = New code selector (0000000000001 0 00)
.flush:
	ret
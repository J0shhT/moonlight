section .text

global __lidt ; void __lidt(DWORD_PTR idt);
__lidt:
	mov eax, [esp+4] ; eax = DWORD_PTR idt
	lidt [eax] ; Load the new IDT (pointed to by eax)
	ret

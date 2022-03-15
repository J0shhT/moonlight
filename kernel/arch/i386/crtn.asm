section .init
	; crtend.o .init section inserted here
	pop ebp
	ret

section .fini
	; crtend.o .fini section inserted here
	pop ebp
	ret
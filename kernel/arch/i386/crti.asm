section .init
global _init:function
_init:
	push ebp
	mov ebp, esp
	; crtbegin.o .init section inserted here

section .fini
global _fini:function
_fini:
	push ebp
	mov ebp, esp
	; crtbegin.o .fini section inserted here
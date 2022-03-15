%macro save_caller 0
	push ebp
	mov ebp, esp
%endmacro

%macro restore_caller 0
	mov esp, ebp
	pop ebp
%endmacro
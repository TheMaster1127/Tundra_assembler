format ELF64 executable 3
entry _start
segment readable writeable
print_buffer   rb 21
minus_sign     db '-'
nl   db 10
ASM_STR_TEMP_PRINT_1 db "Hello World", 10
ASM_STR_TEMP_PRINT_1_len = $-ASM_STR_TEMP_PRINT_1
segment readable executable

print_number:
	push rbp
	mov rbp, rsp
	push rbx
	push r12
	push rdi
	cmp rsi, 0
	je .skip_sign_print
	mov rax, 1
	mov rdi, 1
	mov rsi, minus_sign
	mov rdx, 1
	syscall
	.skip_sign_print:
	pop rax
	lea r12, [print_buffer + 19]
	mov byte [r12], 0
	dec r12
	mov rbx, 10
	.conversion_loop:
	xor rdx, rdx
	div rbx
	add rdx, '0'
	mov [r12], dl
	dec r12
	cmp rax, 0
	jne .conversion_loop
	inc r12
	mov rsi, r12
	lea rdx, [print_buffer + 20]
	sub rdx, rsi
	mov rax, 1
	mov rdi, 1
	syscall
	mov rax, 1
	mov rdi, 1
	mov rsi, nl
	mov rdx, 1
	syscall
	pop r12
	pop rbx
	pop rbp
ret

print_str:
	push rbp
	mov rbp, rsp
	mov rax, 1
	mov rdi, 1
	syscall
	pop rbp
ret

_start:
	push rbp
	mov rbp, rsp
	and rsp, -16

	push r12
	push r13
	xor r13, r13
	mov r12, 5
	.loop1_0:
	cmp r12, 0
	je .loop1_end0
	mov rsi, ASM_STR_TEMP_PRINT_1
	mov rdx, ASM_STR_TEMP_PRINT_1_len
	call print_str
	mov rdi, r13
	mov rsi, 0
	call print_number
	.cloop1_end0:
	inc r13
	dec r12
	jmp .loop1_0
	.loop1_end0:
	pop r13
	pop r12
	
	mov rsp, rbp
	pop rbp
	mov rax, 60
	xor rdi, rdi
	syscall

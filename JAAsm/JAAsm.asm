.data
align 16
shuffle_mask db 1, 2, 3, 2, 3, 4, 3, 5, 5, 0, 0, 0, 0, 0, 0, 0 

.code
applyFilter PROC
    ; Save non-volatile registers
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    push r14
    push r15
    
    ; Setup stack frame
    push rbp
    mov rbp, rsp
    ; Single stack allocation for window buffer (16-byte aligned)
    sub rsp, 16         ; Allocate 32 bytes to maintain alignment
    
    ; Save parameters in registers
    mov r12, rcx        ; input pointer
    mov r13, rdx        ; output pointer
    mov r14, r8         ; width
    mov r15, r9         ; height
    mov r9, r14         ; save true width
    
    ; Trim edges
    sub r14, 1
    sub r15, 1
    
    ; y start = 1
    mov ebx, 1
    ; output index
    xor r10, r10
outer_loop:
    cmp ebx, r15d
    jge done
    mov esi, 1          ; x start = 1
inner_loop:
    cmp esi, r14d
    jge end_inner_loop

    ; Calculate offset
    mov eax, ebx
    mov edx, r14d
    inc edx
    imul eax, edx
    add eax, esi
    mov r8d, eax

    ; Load pixel data
    mov eax, r8d
    sub eax, r9d
    dec eax
    lea rcx, [r12+rax]
    movdqu xmm0, [rcx]
    
    mov eax, r8d
    dec eax
    lea rcx, [r12+rax]
    movdqu xmm1, [rcx]
    
    mov eax, r8d
    add eax, r9d
    dec eax
    lea rcx, [r12+rax]
    movdqu xmm2, [rcx]

    ; Store into window buffer on stack
    lea rcx, [rbp-16]   ; Use stack-allocated buffer
    
    ; Store top row
    pextrb byte ptr [rcx], xmm0, 0
    pextrb byte ptr [rcx + 1], xmm0, 1
    pextrb byte ptr [rcx + 2], xmm0, 2
    
    ; Store middle row
    pextrb byte ptr [rcx + 3], xmm1, 0
    pextrb byte ptr [rcx + 4], xmm1, 1
    pextrb byte ptr [rcx + 5], xmm1, 2
    
    ; Store bottom row
    pextrb byte ptr [rcx + 6], xmm2, 0
    pextrb byte ptr [rcx + 7], xmm2, 1
    pextrb byte ptr [rcx + 8], xmm2, 2

    ; Sort window
    push rbx
    push rsi
    lea r11, [rbp-16]   ; Use same buffer for sorting
    mov rbx, 0          ; i = 0
outer_sort:
    cmp rbx, 8
    jge sort_done
    mov rsi, 0
inner_sort:
    cmp rsi, 8
    jge next_outer
    movzx rax, byte ptr [r11+rsi]
    movzx rcx, byte ptr [r11+rsi+1]
    cmp rax, rcx
    jbe no_swap
    mov byte ptr [r11+rsi], cl
    mov byte ptr [r11+rsi+1], al
no_swap:
    inc rsi
    jmp inner_sort
next_outer:
    inc rbx
    jmp outer_sort
sort_done:
    movzx rax, byte ptr [r11+4]
    mov byte ptr[r13 + r10], al
    pop rsi
    pop rbx

    inc r10
    inc esi
    jmp inner_loop
end_inner_loop:
    inc ebx
    jmp outer_loop
    
done:
    ; Cleanup stack frame properly
    mov rsp, rbp               ; Restores rbp and rsp
    pop rbp

    ; Restore non-volatile registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    ret
applyFilter ENDP
END





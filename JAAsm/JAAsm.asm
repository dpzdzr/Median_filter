; Median Filter Implementation in x64 Assembly
; This procedure applies a 3x3 median filter to 
; a single RGB channel of the image fragment
; Input parameters:
; rcx - input image pointer
; rdx - output image pointer
; r8  - image width
; r9  - image height

.code
applyFilter PROC
    ; Preserve non-volatile registers
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    push r14
    push r15
    
    ; Setup stack frame
    push rbp            ; save old base pointer
    mov rbp, rsp        ; establish new base pointer

    ; Stack allocation for window buffer (16-byte aligned)
    sub rsp, 16         ; allocate 16 bytes to maintain alignment
    
    ; Save parameters in registers
    mov r12, rcx        ; input pointer
    mov r13, rdx        ; output pointer
    mov r14, r8         ; width
    mov r15, r9         ; height
    mov r9, r14         ; save true width
    
    ; Trim edges
    sub r14, 1          ; trim one column (width)
    sub r15, 1          ; trim one row (height)
    
    mov ebx, 1          ; initilize row counter (starts at 1, so skips edge on the left)
    xor r10, r10        ; output buffer index (starts at 0)

outer_loop:
    cmp ebx, r15d       ; check if all rows (excluding edges) are processed
    jge done            ; exit if done
    mov esi, 1          ; initilize column counter (starts at 1, skips the edges)
inner_loop:
    cmp esi, r14d       ; check if all columns in the row are processed
    jge end_inner_loop  ; exit inner loop if done

    ; Calculate the current pixel offset in the 1D array representation
    mov eax, ebx        ; eax = current row
    mov edx, r14d       ; edx = trimmed width
    inc edx             ; edx = actual witdh
    imul eax, edx
    ;imul eax, r9d
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




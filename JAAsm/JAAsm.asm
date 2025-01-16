; Median Filter Implementation in x64 Assembly
; This procedure applies a 3x3 median filter to 
; a single RGB channel of the image fragment
; Input parameters:
; rcx - input image pointer
; rdx - output image pointer
; r8  - image width
; r9  - image height

.code
compare_swap MACRO offset1, offset2
    LOCAL no_swap
    movzx rax, byte ptr[r11+offset1]
    movzx rcx, byte ptr[r11+offset2]
    cmp al, cl
    jbe no_swap
    mov byte ptr[r11 + offset1], cl
    mov byte ptr [r11 + offset2], al
no_swap:
ENDM

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

outer_loop:
    cmp ebx, r15d       ; check if all rows (excluding edges) are processed
    jge done            ; exit if done
    mov esi, 1          ; initilize column counter (starts at 1, skips the edges)

    ; Calculate the first pixel offset in the 1D array representation
    mov eax, ebx
    mov edx, r9d
    imul eax, edx
    mov r8d, eax

inner_loop:
    cmp esi, r14d       ; check if all columns in the row are processed
    jge end_inner_loop  ; exit inner loop if done

    inc r8d              ; increment the input pixel offset

    ; Load the 3x3 window around the current pixel

    ; Top row
    mov eax, r8d        ; copy current pixel position to eax
    sub eax, r9d        ; move to the row above
    dec eax             ; move one column left
    lea rcx, [r12+rax]  ; addres of the top-left pixel
    movdqu xmm0, [rcx]  ; load data into xmm0
    
    ; Middle row
    mov eax, r8d        ; copy current pixel position to eax        
    dec eax             ; move one column left
    lea rcx, [r12+rax]  ; address of the middle-left pixel
    movdqu xmm1, [rcx]  ; load data into xmm1
    
    ; Bottom row
    mov eax, r8d        ; copy current pixel position to eax
    add eax, r9d        ; move to the row below
    dec eax             ; move one column left
    lea rcx, [r12+rax]  ; address of the bottom-left pixel
    movdqu xmm2, [rcx]  ; load data into xmm2

    ; Store the 3x3 window into a stack-allocated buffer
    lea rcx, [rbp-16]   ; address of the window buffer on the stack
    
    ; Extract bytes from xmm registers and store them into the buffer
    ; Top row
    pextrb byte ptr [rcx], xmm0, 0
    pextrb byte ptr [rcx + 1], xmm0, 1
    pextrb byte ptr [rcx + 2], xmm0, 2
    
    ; Middle row
    pextrb byte ptr [rcx + 3], xmm1, 0
    pextrb byte ptr [rcx + 4], xmm1, 1
    pextrb byte ptr [rcx + 5], xmm1, 2
    
    ; Bottom row
    pextrb byte ptr [rcx + 6], xmm2, 0
    pextrb byte ptr [rcx + 7], xmm2, 1
    pextrb byte ptr [rcx + 8], xmm2, 2

    ; Sort window
    ; Save rbx and rsi for the sorting routine
    lea r11, [rbp-16]   ; address of the window buffer for sorting
    
        ; Pierwsza warstwa
    compare_swap 0, 3
    compare_swap 1, 7
    compare_swap 2, 5
    compare_swap 4, 8

    ; Druga warstwa
    compare_swap 0, 7
    compare_swap 2, 4
    compare_swap 3, 8
    compare_swap 5, 6

    ; Trzecia warstwa
    compare_swap 0, 2
    compare_swap 1, 3
    compare_swap 4, 5
    compare_swap 7, 8

    ; Czwarta warstwa
    compare_swap 1, 4
    compare_swap 3, 6
    compare_swap 5, 7

    ; Pi¹ta warstwa
    compare_swap 0, 1
    compare_swap 2, 4
    compare_swap 3, 5
    compare_swap 6, 8

    ; Szósta warstwa
    compare_swap 2, 3
    compare_swap 4, 5
    compare_swap 6, 7

    ; Siódma warstwa
    compare_swap 1, 2
    compare_swap 3, 4
    compare_swap 5, 6

    movzx rax, byte ptr [r11+4]
    mov byte ptr[r13], al         ; write median to the output buffer
        
    inc r13             ; increment the output buffer index
    inc esi             ; increment column counter

    jmp inner_loop

end_inner_loop:
    inc ebx             ; increment row counter
    jmp outer_loop
    
done:
    ; Restore stack frame 
    mov rsp, rbp               
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
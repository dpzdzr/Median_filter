; Median Filter Implementation in x64 Assembly
; This procedure applies a 3x3 median filter to 
; a single RGB channel of the image fragment
; Input parameters:
; rcx - input image pointer
; rdx - output image pointer
; r8  - image width
; r9  - image height

.code
; Macro for sorting network
; compares two given bytes and swaps them if needed
compare_swap MACRO reg1, reg2
    LOCAL no_swap
    mov rax, reg1
    mov rcx, reg2
    cmp al, cl
    jbe no_swap
    mov reg1, rcx
    mov reg2, rax
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

    mov rbx, 1          ; initilize row counter (starts at 1, so skips edge on the left)
outer_loop:
    cmp rbx, r15        ; check if all rows (excluding edges) are processed
    jge done            ; exit if done
    mov esi, 1          ; initilize column counter (starts at 1, skips the edges)

    mov rdi, r12        ; move current row pointer for inner looping

inner_loop:
    cmp esi, r14d       ; check if all columns in the row are processed
    jge end_inner_loop  ; exit inner loop if done

    ; Load the 3x3 window around the current pixel
    ; Top row
    movdqu xmm0, [rdi]  ; load data into xmm0
    
    ; Middle row
    movdqu xmm1, [rdi+r9]  ; load data into xmm1
    
    ; Bottom row
    movdqu xmm2, [rdi+2*r9]  ; load data into xmm2

    ; Store the 3x3 window into a stack-allocated buffer
    lea rcx, [rbp-16]   ; address of the window buffer on the stack
    
    ; Extract bytes from xmm registers and store them into the buffer
    push rbx
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Top row
    ;pextrb byte ptr rcx, xmm0, 0
    ;pextrb byte ptr [rcx + 1], xmm0, 1
   ; pextrb byte ptr [rcx + 2], xmm0, 2
    
    pextrb rbx, xmm0, 0
    pextrb r8, xmm0, 1
    pextrb r9, xmm0, 2

    ; Middle row
    pextrb r10, xmm1, 0
    pextrb r11, xmm1, 1
    pextrb r12, xmm1, 2
    
    ; Bottom row
    pextrb r13, xmm2, 0
    pextrb r14, xmm2, 1
    pextrb r15, xmm2, 2

    ; Sort window
    ; Save rbx and rsi for the sorting routine
    ;lea r11, [rbp-16]   ; address of the window buffer for sorting
    
    ; First layer
    compare_swap rbx, r10     ; 0, 3
    compare_swap r8, r15      ; 1, 7
    compare_swap r9, r12      ; 2, 5
    compare_swap r11, r13     ; 4, 8

    ; Second layer
    compare_swap rbx, r15     ; 0, 7
    compare_swap r9, r11      ; 2, 4
    compare_swap r10, r13     ; 3, 8
    compare_swap r12, r14     ; 5, 6

    ; Third layer
    compare_swap rbx, r9      ; 0, 2
    compare_swap r8, r10      ; 1, 3
    compare_swap r11, r12     ; 4, 5
    compare_swap r15, r13     ; 7, 8

    ; Fourth layer
    compare_swap r8, r11      ; 1, 4
    compare_swap r10, r14     ; 3, 6
    compare_swap r12, r15     ; 5, 7

    ; Fifth layer
    compare_swap rbx, r8      ; 0, 1
    compare_swap r9, r11      ; 2, 4
    compare_swap r10, r12     ; 3, 5
    compare_swap r14, r13     ; 6, 8

    ; Sixth layer
    compare_swap r9, r10      ; 2, 3
    compare_swap r11, r12     ; 4, 5
    compare_swap r14, r15     ; 6, 7

    ; Seventh layer
    compare_swap r8, r9       ; 1, 2
    compare_swap r10, r11     ; 3, 4
    compare_swap r12, r14     ; 5, 6

    mov rax, r11              ; save median in rax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbx

    mov byte ptr[r13], al    ; write median to the output buffer
            
    inc r13             ; increment the output buffer index
    inc esi             ; increment column counter
    inc rdi             ; increment current pixel in row pointer

    jmp inner_loop

end_inner_loop:
    inc rbx             ; increment row counter
    add r12, r9         ; move to the next row
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
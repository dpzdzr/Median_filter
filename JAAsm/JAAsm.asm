; x64 parameter passing:
; rcx - first parameter  (input pointer)  [uint8_t* input]
; rdx - second parameter (output pointer) [uint8_t* output]
; r8d - third parameter  (width)
; r9d - fourth parameter (height)
.data
align 16
shuffle_mask db 1, 2, 3, 2, 3, 4, 3, 5, 5, 0, 0, 0, 0, 0, 0, 0 
align 16
window db 9 dup(0)


.code
applyFilter PROC
    ; Zachowaj rejestry nieulotne (non-volatile)
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    push r14
    push r15
    
    ; Zapisz parametry w rejestrach "w³asnych"
    mov r12, rcx        ; input pointer
    mov r13, rdx        ; output pointer
    mov r14, r8         ; width
    mov r15, r9         ; height

    ; Zapamiêtujemy PRAWDZIW¥ szerokoœæ w rsi (potrzebna do offsetu)
    mov r9, r14

    ; Odcinamy po 1 pikselu na brzegach, by skipowaæ pierwsz¹ i ostatni¹ kolumnê/wiersz
    sub r14, 1
    sub r15, 1
    
    ; Alokacja na stosie (nieu¿ywana tutaj do filtra, ale zostaje)
    sub rsp, 16

    ; y start = 1
    mov ebx, 1
    ; output index
    xor r10, r10

outer_loop:
    cmp ebx, r15d
    jge done       ; jeœli ebx >= (height - 1), wyjdŸ

    ; x start = 1
    mov esi, 1

inner_loop:
    cmp esi, r14d
    jge end_inner_loop  ; jeœli esi >= (width - 1), przejdŸ do kolejnego wiersza

    ; Oblicz przesuniêcie: offset = y * (prawdziwa_szerokosc) + x
    ; rsi zawiera oryginalne 'width'
    mov eax, ebx
    imul eax, r9d
    add eax, esi
    mov r8d, eax
    ; Wczytaj bajt z wejœcia i zapisz do wyjœcia
    ;movzx  ecx, byte ptr [r12+rax]
    ;mov    byte ptr [r13+r10], cl

    ;;;;;;;;
    mov eax, r8d
    sub eax, r14d
    dec eax
    lea rcx, [r12+rax]
    movdqu xmm0, [rcx]

    mov eax, r8d
    dec eax
    lea rcx, [r12+rax]
    movdqu xmm1, [rcx]

    mov eax, r8d
    add eax, r14d
    dec eax
    lea rcx, [r12+rax]
    movdqu xmm2, [rcx]

    ;pshufb xmm0, xmmword ptr shuffle_mask
    ;pshufb xmm1, xmmword ptr shuffle_mask
    ;pshufb xmm2, xmmword ptr shuffle_mask

    lea rcx, window

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

 ; Sortowanie tablicy window (bubble sort)

    push r11
    lea r11, window
    movzx  ecx, byte ptr [r11+4]
    mov    byte ptr [r13+r10], cl
    pop r11
    ;;;;;;;;
    inc r10
    inc esi
    jmp inner_loop

end_inner_loop:
    inc ebx
    jmp outer_loop
    
done:
    add rsp, 16         ; przywróæ stos
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
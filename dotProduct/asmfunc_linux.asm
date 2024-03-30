; assembly part using x86-64
; DIFFERENCES b/w LINUX AND WINDOWS:
; ----PARAMETER CALLING----
; LINUX
;           foo(
; parameters:   int a, int b, int c, int d, int e, int f)
; registers:       RDI,  RSI,   RDX,   RCX,    R8,    R9
;
; WINDOWS
;           foo(
; parameters:   int a, int b, int c, int d)
; registers:       RCX,  RDX,    R8,    R9
;
; SIMILARITIES
; *  RAX <- integer return values
; * xmm0 <- float return values

section .data
msg db "Hello World",13,10,0
empty db 0.0

section .text
bits 64
default rel ; to handle address relocation

global asmDotProduct
asmDotProduct:
    ; initialization
	push rdi   ; [RSP + (8 * 3)] address of float A[]
	push rsi   ; [RSP + (8 * 2)] address of float B[]
    push rdx   ; [RSP + 8      ] size n
    sub rsp, 8 ; 16-byte alignment, segfault otherwise

    movss xmm0, [empty]
    movss xmm1, [empty]
    movss xmm2, [empty]

    ; no need to subtract rsp more since
    ; it's not calling any c function.

    ; for (i = 0; i < n; i++)
    mov rdx, [rsp + 8] ; rdx is n
    xor rcx, rcx       ; rcx is i

    mov rax, [rsp + (8 * 3)]
    mov rbx, [rsp + (8 * 2)]
L1:
    movss xmm1, [rax] ; xmm1 = A
    movss xmm2, [rbx] ; xmm2 = B
    ; product = product + (A * B)
    mulss xmm1, xmm2
    addss xmm0, xmm1

    inc rcx ; i++
    ;   i    n
    ;   ---  ---
    cmp rcx, rdx
    jl nextFloat ; (i < n)
    jmp finish

nextFloat:
    ; sizeof(float) = 4
    add rax, 4 ; next index of float array
    add rbx, 4
    jmp L1

finish:
    add rsp, 8 * 4
    xor rax, rax
    xor rbx, rbx
    ret

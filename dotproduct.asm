section .data
msg db "HI BABY GIRL\n", 0

section .text
extern printf
bits 64
default rel

global hello

hello:
    sub rsp, 8
    mov rcx, msg
    call printf
    add rsp, 8
    ret

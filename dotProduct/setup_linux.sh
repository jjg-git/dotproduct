#!/usr/bin/bash
nasm -felf64 asmfunc_linux.asm -o x64_linux/Debug/asmfunc_linux.o
gcc *.c x64_linux/Debug/*.o -m64 -no-pie -o x64_linux/Debug/dotProduct

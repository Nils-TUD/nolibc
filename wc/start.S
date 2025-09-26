.global _start

_start:
    call main

    mov %eax, %ebx
    mov $0xFC, %eax
    int $0x80

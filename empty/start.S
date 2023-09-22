#include <sys/syscall.h>

.extern main
.global _start

_start:
    call main

    mov %eax, %ebx
    mov $SYS_exit_group, %eax
    int $0x80

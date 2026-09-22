#include <syscall.h>

.global _start

_start:
    call main
    mov %rax, %rdi
    mov $SYS_exit_group, %rax
    syscall

.extern main
.extern exit

.global _start
_start:
    call main
    push %eax
    call exit

    # just to be sure
1:  jmp 1b

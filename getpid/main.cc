#include <sys/syscall.h>
#include <stdio.h>

int main() {
    int v = SYS_getpid;
    asm volatile (
        "int $0x80"
        : "+a"(v)
    );

    printf("Hello World: %d\n", v);
    return 0;
}

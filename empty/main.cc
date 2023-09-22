#include <sys/syscall.h>

/* write a program */
int main() {
    return 0;
}

static int syscall1(int no, int arg1) {
    int v = no;
    asm volatile (
        "int $0x80"
        : "+a"(v)
        : "b"(arg1)
    );
    return v;
}

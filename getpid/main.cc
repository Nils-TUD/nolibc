#include <syscall.h>

static int getpid() {
    unsigned long v = SYS_getpid;
    asm volatile (
        "syscall"
        : "+a"(v)
        :: "rcx", "r11", "memory"
    );
    return v;
}

static long write(int fd, const char *buffer, unsigned long count) {
    unsigned long v = SYS_write;
    asm volatile (
        "syscall"
        : "+a"(v)
        : "D"(fd), "S"(buffer), "d"(count)
        : "rcx", "r11", "memory"
    );
    return v;
}

int main() {
    getpid();

    char s[] = "Hello World!\n";
    write(1, s, sizeof(s) - 1);

    return 0;
}

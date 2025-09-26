#include <sys/syscall.h>

extern "C" void _start() {
    int v = SYS_getpid;
    asm volatile(
        "int $0x80"
        : "+a"(v)
    );

    v = SYS_write;
    asm volatile(
        "int $0x80"
        : "+a"(v)
        : "b"(1),
          "c"("Hello World\n"),
          "d"(12)
    );

    v = SYS_exit;
    asm volatile(
        "int $0x80"
        : "+a"(v)
        : "b"(0)
    );
}

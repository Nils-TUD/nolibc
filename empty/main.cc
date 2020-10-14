#include <syscall.h>

static unsigned syscall0(unsigned no) {
    asm volatile (
        "int $0x80"
        : "+a"(no)
    );
    return no;
}

static unsigned syscall1(unsigned no, unsigned arg1) {
    asm volatile (
        "int $0x80"
        : "+a"(no)
        : "b"(arg1)
    );
    return no;
}

static unsigned getpid() {
    return syscall0(__NR_getpid);
}

static void exit(int result) {
    syscall1(__NR_exit, result);
}

int main() {
	return getpid();
}

extern "C" void _start() {
    int result = main();
    exit(result);
    __builtin_unreachable();
}

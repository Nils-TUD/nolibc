#include <syscall.h>

typedef unsigned long mword;

static mword syscall0(mword no) {
    asm volatile (
        "int $0x80"
        : "+a"(no)
    );
    return no;
}

static mword syscall1(mword no, mword arg1) {
    asm volatile (
        "int $0x80"
        : "+a"(no)
        : "b"(arg1)
    );
    return no;
}

static mword syscall3(mword no, mword arg1, mword arg2, mword arg3) {
    asm volatile (
        "int $0x80"
        : "+a"(no)
        : "b"(arg1), "c"(arg2), "d"(arg3)
    );
    return no;
}

static int getpid() {
    return syscall0(__NR_getpid);
}

extern "C" __attribute__((noreturn)) void exit(int result) {
    syscall1(__NR_exit, result);
    __builtin_unreachable();
}

static mword write(int fd, const void *data, mword size) {
    return syscall3(__NR_write, fd, reinterpret_cast<mword>(data), size);
}

static mword read(int fd, void *data, mword size) {
    return syscall3(__NR_read, fd, reinterpret_cast<mword>(data), size);
}

static void printn(int fd, int n, unsigned base) {
    if(base > 16)
        exit(1);
    if(n >= base)
        printn(fd, n / base, base);
    char c = "0123456789abcdef"[n % base];
    write(fd, &c, 1);
}

int main() {
    int lines = 0;
    char c;
    while(read(0, &c, 1) > 0) {
        if(c == '\n')
            lines += 1;
    }
    write(1, "Lines: ", 7);
    printn(1, lines, 10);
    write(1, "\n", 1);
    return 0;
}

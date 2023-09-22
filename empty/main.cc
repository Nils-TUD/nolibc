#include <sys/syscall.h>

static int syscall3(int no, unsigned arg1, unsigned arg2, unsigned arg3) {
    int v = no;
    asm volatile (
        "int $0x80"
        : "+a"(v)
        : "b"(arg1), "c"(arg2), "d"(arg3)
    );
    return v;
}

unsigned strlen(const char *s) {
    unsigned n = 0;
    while(*s++)
        n++;
    return n;
}

void puts(const char *s) {
    syscall3(SYS_write, 1, (unsigned)s, strlen(s));
}

void putchar(char c) {
    syscall3(SYS_write, 1, (unsigned)&c, 1);
}

void printu(unsigned n) {
    if(n >= 10)
        printu(n / 10);
    putchar('0' + (n % 10));
}

int main() {
    unsigned lines = 0;

    char c;
    while(1) {
        int res = syscall3(SYS_read, 0, (unsigned)&c, 1);
        if(res == 0)
            break;
        if(c == '\n')
            lines++;
    }

    puts("Lines: ");
    printu(lines);
    putchar('\n');
    return 0;
}

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

static long read(int fd, char *buffer, unsigned long count) {
    unsigned long v = SYS_read;
    asm volatile (
        "syscall"
        : "+a"(v)
        : "D"(fd), "S"(buffer), "d"(count)
        : "rcx", "r11", "memory"
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

static int count_lines(int fd) {
    int count = 0;
    while(true) {
        char c;
        long res = read(fd, &c, 1);
        if(res <= 0)
            break;
        if(c == '\n')
            count++;
    }
    return count;
}

static void printn(int num) {
    if(num >= 10) {
        printn(num / 10);
    }
    char c = '0' + (num % 10);
    write(1, &c, 1);
}

int main() {
    getpid();

    char s[] = "Hello World!\n";
    write(1, s, sizeof(s) - 1);

    int lines = count_lines(0);
    printn(lines);
    write(1, "\n", 1);

    return 0;
}

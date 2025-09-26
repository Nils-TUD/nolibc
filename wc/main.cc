#include <sys/syscall.h>

static int read(int fd, char *buf, unsigned int len) {
    int v = SYS_read;
    asm volatile(
        "int $0x80"
        : "+a"(v)
        : "b"(fd),
          "c"(buf),
          "d"(len)
    );
    return v;
}

static int write(int fd, const char *buf, unsigned int len) {
    int v = SYS_write;
    asm volatile(
        "int $0x80"
        : "+a"(v)
        : "b"(fd),
          "c"(buf),
          "d"(len)
    );
    return v;
}

static void putn(int n) {
    if(n >= 10) {
        putn(n / 10);
    }
    char digit = '0' + (n % 10);
    write(1, &digit, 1);
}

int main() {
    int lines = 0;
    char c;
    while(read(0, &c, 1) > 0) {
        if(c == '\n')
            lines++;
    }

    putn(lines);
    write(1, "\n", 1);
    return 1;
}

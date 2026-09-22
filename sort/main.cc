#include <sys/mman.h>
#include <syscall.h>
#include <cstddef>

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

static void *my_mmap(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags,
                     unsigned long fd, unsigned long off) {
    unsigned long v = SYS_mmap;
    register unsigned long r10 asm("r10") = flags;
    register unsigned long r8 asm("r8") = fd;
    register unsigned long r9 asm("r9") = off;
    asm volatile (
        "syscall"
        : "+a"(v)
        : "D"(addr), "S"(len), "d"(prot), "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory"
    );
    return (void*)v;
}

static void *malloc(unsigned long size) {
    static char *cur = NULL;
    static unsigned long cur_off = 0;
    static unsigned long cur_size = 0;
    if(cur == NULL || size > cur_size - cur_off) {
        size = (size + 4096 - 1) & ~(4096 - 1);
        cur = (char*)my_mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        cur_off = 0;
        cur_size = size;
    }
    cur_off += size;
    return cur + (cur_off - size);
}

static void printn(int num) {
    if(num >= 10) {
        printn(num / 10);
    }
    char c = '0' + (num % 10);
    write(1, &c, 1);
}

int main() {
    void *p1 = malloc(10);
    void *p2 = malloc(20);
    char *p3 = (char*)malloc(20000);
    *p3 = '4';
    return 0;
}

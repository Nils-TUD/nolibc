#include <sys/mman.h>
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

static mword syscall6(mword no, mword arg1, mword arg2, mword arg3,
                      mword arg4, mword arg5, mword arg6) {
    asm volatile (
        "push %%ebp\n"
        "mov %6, %%ebp\n"
        "int $0x80\n"
        "pop %%ebp\n"
        : "+a"(no)
        : "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4), "D"(arg5), "m"(arg6)
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

static void *my_mmap(void *addr, mword length, int prot, int flags, int fd, mword offset) {
    mword res = syscall6(__NR_mmap2, reinterpret_cast<mword>(addr), length, prot, flags, fd, offset);
    return reinterpret_cast<void*>(res);
}

int main() {
    void *addr = my_mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(addr == MAP_FAILED)
        return 1;
    return 0;
}

#include <sys/mman.h>
#include <syscall.h>

typedef unsigned int mword;

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

extern "C" __attribute__((noreturn)) void exit(int result) {
    syscall1(__NR_exit, result);
    __builtin_unreachable();
}

static long write(int fd, const void *data, mword size) {
    return syscall3(__NR_write, fd, reinterpret_cast<mword>(data), size);
}

static long read(int fd, void *data, mword size) {
    return syscall3(__NR_read, fd, reinterpret_cast<mword>(data), size);
}

static void *mmap_anon(mword length) {
    mword res = syscall6(__NR_mmap2, 0, length, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    return reinterpret_cast<void*>(res);
}

// Allocate a huge chunk of memory and then give small slices of it
// back to the application.
void *operator new(mword s) {
    static char *alloc = 0;

    if (alloc == 0)
        // First time we call this. Allocate memory from the system. We
        // allocate 512 MB. The system will only back this with physical
        // memory when we actually use it.
        alloc = reinterpret_cast<char *>(mmap_anon(512 * 1024 * 1024));

    // Remember the current allocation pointer.
    void *p = alloc;

    // We align the size of the allocation to 16-bytes. This is required
    // for some data structures. Won't hurt if you don't do it in this
    // example.
    s = (s + 0xF) & ~0xF;

    // We move our allocation pointer further. It now points after the
    // last used memory block.
    alloc += s;

    return p;
}

void *operator new[](mword s) {
    return operator new(s);
}

void memcpy(void *dest, const void *src, mword len) {
    char *cd = (char *)dest;
    char *cs = (char *)src;
    while (len--)
        *(cd++) = *(cs++);
}

// Read a line from stdin and return a pointer to it. Memory is
// allocated on the heap and the caller has to free it (or not...).
char *read_line(bool *eof) {
    const mword max_line_length = 128;
    char buf[max_line_length];
    char *cur = buf;
    long bytes;

    while (
         // We don't exceed the buffer length (two bytes need to be
         // reserved for the newline and zero byte) ...
         cur < &buf[max_line_length - 2]
         // ... and have read a single character
         && (0 < (bytes = read(0, cur, 1)))
         // ... and that character wasn't a newline.
         && (*cur != '\n'))
        cur++;

    // Remember whether we hit EOF.
    *eof = (bytes == 0);

    // Now our buffer is either full or has a newline. Deal with either case.
    cur[0] = '\n';
    cur[1] = 0;
    cur   += 2;

    // cur now points to the end of a zero-terminated string.

    // The line is now in our local buffer. Allocate a heap buffer and
    // copy it there. cur - buf is the length of the string including
    // the zero byte.
    char *heap_buf = new char[cur - buf];
    memcpy(heap_buf, buf, cur - buf);
    return heap_buf;
}

mword strlen(const char *str) {
    mword len = 0;
    for (const char *s = str; *s != 0; s++)
        len++;
    return len;
}

int main() {
    // We will spectacularly fail, if we read more lines.
    const mword max_lines = 4096;

    /* An array of character pointers. */
    char *lines[max_lines];

    /* Points to one element after the last line. */
    char **cur_line = lines;

    do {
        bool eof;
        *cur_line = read_line(&eof);
        if (eof)
            /* Empty line (not even a newline). Time to sort. */
            break;
        cur_line++;
    }
    while (1);

    mword total_lines = cur_line - lines;

    // Sort lines according to length with Bubble Sort.
    bool swapped;
    do {
        swapped = false;

        for (mword i = 1; i < total_lines; i++) {
            if (strlen(lines[i - 1]) > strlen(lines[i])) {
                char *tmp = lines[i - 1];
                lines[i - 1] = lines[i];
                lines[i]   = tmp;
                swapped = true;
            }
        }
    }
    while (swapped);

    for (char **p = lines; p < cur_line; p++)
        write(1, *p, strlen(*p));

    return 0;
}

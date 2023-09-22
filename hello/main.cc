#include <stdio.h>

int main() {
    int a = 4;
    int b = 5;
    asm volatile (
        "add %1, %0"
        : "+r"(a)
        : "r"(b)
    );

    printf("Hello World: %d\n", a);
    return 0;
}

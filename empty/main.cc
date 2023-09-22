/* write a program */
int main() {
    return 0;
}

extern "C" void _start() {
    main();
    while(1)
        ;
}

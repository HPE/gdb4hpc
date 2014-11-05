_Cilk_shared void attach_gdb() {
    volatile int loop = 1;
    while (loop) {
        volatile int bp = 1;
    }
}

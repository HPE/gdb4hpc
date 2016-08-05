_Cilk_shared void myo_attach_gdb() {
    volatile int loop = 1;
    while (loop) {
        volatile int bp = 1;    /* endless_loop_breakpoint */
    }
}

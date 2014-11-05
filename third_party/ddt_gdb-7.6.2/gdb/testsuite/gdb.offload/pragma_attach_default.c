void pragma_attach_gdb() {
    volatile int loop = 1;
    #pragma offload target (mic)
    while (loop) {
        volatile int bp = 1;    /* endless_loop_breakpoint */
    }
}

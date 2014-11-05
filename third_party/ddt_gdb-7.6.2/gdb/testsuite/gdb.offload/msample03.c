extern _Cilk_shared myo_attach_gdb();
_Cilk_shared int offload_count = 1;

_Cilk_shared loop()
{
    // Read the value to get ownership
    volatile int local_count = offload_count;

    volatile int loop = 1;
    while (loop) {
        volatile int bp = 1;    /* loop_breakpoint */
    }
}

int main()
{
    _Cilk_offload myo_attach_gdb();

    volatile int loop_forever = 1;
    while (loop_forever)
    {
        _Cilk_offload loop();
        offload_count++;
    }
    return 0;
}

extern _Cilk_shared myo_attach_gdb();

_Cilk_shared int global = 42;

_Cilk_shared target() {
    volatile int bp = 0;
    global++;
    bp = global;        /* force_read_global */
}


int main(int argc, char* argv[]) {
    _Cilk_offload myo_attach_gdb();

    volatile int dummy = 0;

    // dummy assignments are used to force a Shared variable read
    dummy = global;
    global++;

    _Cilk_offload target();
    dummy = global;
    global++;

    _Cilk_offload target();
    global++;

    _Cilk_offload target();
    global++;

    dummy = 1;

    return 0;
}

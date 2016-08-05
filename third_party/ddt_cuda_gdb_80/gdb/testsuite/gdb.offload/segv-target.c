_Cilk_shared float foo;

extern _Cilk_shared myo_attach_gdb();

_Cilk_shared target() {
    foo = 42;               /* bp-before-shared-write */
    volatile int bp = 0;    /* bp-before-after-write */
}

_Cilk_shared force_segv() {
    *(char *)0 = 0;         /* bp-force-segv */
    volatile int bp = 0;    /* bp-after-segv */
}

int main(int argc, char* argv[]) {
    _Cilk_offload myo_attach_gdb();
    _Cilk_offload target();
    _Cilk_offload force_segv();
    return 0;
}

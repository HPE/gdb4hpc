_Cilk_shared int shared_var;

_Cilk_shared init(int val) {
    shared_var = val;
}

int main(int argc, char *argv[]) {
    // Initialize on target to not get ownership yet
    _Cilk_offload init(1);

    // Read the value on host: no ownership, ownership on target
    volatile int old_val = shared_var;

    volatile int bp1 = 1;       /* bp1 */

    // Write the value on host: ownership on host
    shared_var = 1;

    volatile int bp2 = 1;       /* bp2 */

    // Initialize on target again to remove ownership from host
    _Cilk_offload init(1311);

    volatile int bp3 = 1;       /* bp3 */

    return 0;
}

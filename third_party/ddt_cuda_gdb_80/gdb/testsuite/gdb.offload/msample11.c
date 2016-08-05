extern _Cilk_shared myo_attach_gdb();

_Cilk_shared int global_A;
_Cilk_shared int global_B;

_Cilk_shared foo()
{
    volatile int super_dummy_A = 1;     /* BP1 */
    volatile int super_dummy_B = 2;     /* BP2 */

    super_dummy_A = global_A;
    super_dummy_B = global_B;           /* BP3 */

    global_A++;                         /* BP4 */
    global_B += 2;                      /* BP5 */

    super_dummy_A = global_A;
    super_dummy_B = global_B;
    super_dummy_B = 1;                  /* BP6 */
}

int main(int argc, char* argv[])
{
    _Cilk_offload myo_attach_gdb();

    volatile int dummy_A = 0; /* just for BP */
    volatile int dummy_B = 0;

    global_A = 40;
    global_B = 3;

    /* dummy assignments are used to force a Shared variable read */
    dummy_A = global_A;
    dummy_B = global_B;
    global_A += 2;
    global_B += 3;

    _Cilk_offload foo();
    dummy_A = global_A;
    dummy_B = global_B;
    global_A += 2;
    global_B += 3;

    _Cilk_offload foo();
    global_A += 2;
    global_B += 3;

    _Cilk_offload foo();
    global_A += 2;
    global_B += 3;

    dummy_A = 1;

    return 0;
}


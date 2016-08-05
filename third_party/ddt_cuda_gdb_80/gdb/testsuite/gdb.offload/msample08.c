#include <stdio.h>

extern _Cilk_shared myo_attach_gdb();

_Cilk_shared float my_shared;

_Cilk_shared foo(float val)
{
    volatile float old_val = my_shared; /* force reading the current value on MIC. */
    my_shared += val;                   /* here my_shared MUST be evaluated correctly. */
    old_val = val;                      /* BP1 */
}

int main()
{
    _Cilk_offload myo_attach_gdb();

    my_shared = 1.0f;
    float my_copy = 0.0f;
    int tot_loops = 100;
    int i = 0;

    for(i=0; i<tot_loops; i++) {
        my_copy = my_shared;            /* force the proper value on host, */
        my_copy += 1.0f;                /* using two lines to avoid bp on shared function call. */
        _Cilk_offload foo(2.0f);        /* here my_shared MUST be evaluated correctly, */
        my_shared += 1.0f;              /* using two lines for easier bp handling. */
    }

    return 0;
}


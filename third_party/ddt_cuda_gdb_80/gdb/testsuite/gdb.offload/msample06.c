#include <stdio.h>

extern _Cilk_shared myo_attach_gdb();

#define SIZE 4097

_Cilk_shared float in1[SIZE];
_Cilk_shared float in2[SIZE];
_Cilk_shared float res[SIZE];

_Cilk_shared populate(_Cilk_shared float* data, int len, float step)
{
    volatile float superdummy;
    int i;

    for(i=0; i<len; i++) {
        superdummy = data[i];   /* force reading the current value. */
        data[i] = i * step;
        superdummy = data[i];   /* BP1 */
    }
    superdummy = 0.0f;          /* BP2 */
}

_Cilk_shared compute_sum()
{
    volatile float superdummy;
    int i;

    for(i=0; i<SIZE; i++) {
        superdummy = in1[i];
        res[i] = in1[i] + in2[i];       /* BP4 */
        superdummy = in2[i];
    }
    superdummy = 0.0f;          /* BP3 */
}

int main()
{
    _Cilk_offload myo_attach_gdb();
    populate(in1, SIZE, 1.0f);                  /* first time populate on the host and execute on MIC. */
    populate(in2, SIZE, 2.0f);                  /* use only float that are guarateed to have an exact */
    populate(res, SIZE, 0.0f);                  /* representation, we need it for == comparisons. */
    _Cilk_offload compute_sum();

    populate(in1, SIZE, 0.0f);                  /* clean the arrays on host. */
    populate(in2, SIZE, 0.0f);
    populate(res, SIZE, 0.0f);

    _Cilk_offload populate(in1, SIZE, 1.0f);    /* second time populate and execute on MIC. */
    _Cilk_offload populate(in2, SIZE, 2.0f);
    _Cilk_offload populate(res, SIZE, 0.0f);
    _Cilk_offload compute_sum();

    return 0;
}


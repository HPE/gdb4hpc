#include <stdlib.h>
#include <omp.h>

extern _Cilk_shared myo_attach_gdb();

_Cilk_shared int total = 0;

_Cilk_shared void msample04()
{
    int num_threads = 128;
    volatile int loop = 1;

    volatile int bp = 0;
    while (loop)
    {
        total = 42; // don't start from 0,
                    // better avoid nasty
                    // surprises with
                    // _Cilk_shared vars

        omp_set_num_threads(num_threads);
        bp = 1;         /* before_omp_loop */
        int i;
        #pragma omp parallel for
        for(i=0; i<1048576; i++)
        {
            bp = 2;     /* omp_loop_bp1 */
            total++;
            bp = 3;     /* omp_loop_bp2 */
        }
    }

    bp = 0;
}

int main()
{
    _Cilk_offload myo_attach_gdb();
    _Cilk_offload msample04();
    return 0;
}

#include <stdio.h>
#include <unistd.h>

extern _Cilk_shared myo_attach_gdb();

#define SIZE 16384

_Cilk_shared int arr[SIZE];

_Cilk_shared void delay_trick()
{
    sleep(2);
}

_Cilk_shared void init_array()
{
    volatile int use_delay_trick = 0;
    int i;
    _Cilk_for (i=0; i<SIZE; i++)
    {
        arr[i] = i;             /* before_delay_trick */
        if(use_delay_trick) {   /* if_use_delay_trick */
            delay_trick();
        }
    }
    use_delay_trick = -1; // just for bp
    use_delay_trick = 0;        /* delay_trick_set_0 */
}

int main()
{
    _Cilk_offload myo_attach_gdb();
    _Cilk_offload init_array();

    return 0;
}


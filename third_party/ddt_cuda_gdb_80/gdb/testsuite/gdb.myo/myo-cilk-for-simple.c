#include <stdio.h>

extern _Cilk_shared attach_gdb();

#define SIZE 16384

_Cilk_shared int arr1[SIZE];
_Cilk_shared int arr2[SIZE];
_Cilk_shared int arr3[SIZE];

_Cilk_shared void init_array()
{
  char buf[8];
  volatile int num_threads = 2;
  sprintf(buf, "%d", num_threads);

  volatile int superdummy = 0;
  int i;

  __cilkrts_set_param("nworkers", buf);

  _Cilk_for (i=0; i<SIZE; i++) {
    arr1[i] = i;
    arr2[i] = 1;
    arr3[i] = 2;
  }
  superdummy = -1; // just for bp
  superdummy = 0;
}

int main()
{
  _Cilk_offload attach_gdb();

  _Cilk_offload init_array();

  return 0;
}


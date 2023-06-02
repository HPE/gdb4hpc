#include <stdio.h>
#include <shmem.h>

int main (void)
{
  shmem_init();
  int me = shmem_my_pe();
  int npes = shmem_n_pes();

  int i, local[4];

  int *vec1 = (int *)shmem_malloc(sizeof(int) * 4);
  if(vec1 == NULL)
  {
    fprintf(stdout, "shmeme_malloc returned NULL, exiting...");
    shmem_finalize();
    return 1;
  }

  for (i=0; i<4; ++i) {
     local[i] = 4*me + i;
  }

  shmem_int_put(vec1, local, 4, (me+1)%npes);
  //dummy code                                                                                  
  shmem_quiet();
  //clean up                                                                                    
  shmem_free(vec1);
  //only one call to free, nothing extra                                                        
  shmem_finalize();

  return 0;
}


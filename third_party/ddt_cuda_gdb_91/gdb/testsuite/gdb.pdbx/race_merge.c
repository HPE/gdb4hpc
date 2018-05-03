// A big access is split into several smaller accesses for logging.
//
// Memory accesses are split into pointer-sized chunks.  Race reports on more
// than one chunk are merged if the chunks belong to the same memory object.
//
// We expect a single sharing detection.
//

#include <pthread.h>

struct shared_s
{
  /* We need the structure to be bigger than 8 bytes.  */
  int arr[8];
};
static struct shared_s shared;
static pthread_barrier_t barrier;

static void*
write (void *arg)
{
  struct shared_s init, *var;

  var = arg;
  *var = init; /* bp.write */

  /* Keep threads alive to guarantee we see the data race.  */
  pthread_barrier_wait (&barrier);

  return NULL;
}

static void*
read (void *arg)
{
  struct shared_s init, *var;

  var = arg;
  init = *var; /* bp.read */

  /* Keep threads alive to guarantee we see the data race.  */
  pthread_barrier_wait (&barrier);

  return NULL;
}

static void
test (struct shared_s *var)
{
  pthread_t thread[2];

  pthread_barrier_init (&barrier, NULL, 2);

  pthread_create (&thread[0], NULL, read, var);  /* bp.thread */
  pthread_create (&thread[1], NULL, write, var);

  pthread_join (thread[0], NULL);
  pthread_join (thread[1], NULL);
}

int
main(void) {
  test (&shared);

  return 0;
}

#include <new>


extern "C" _Cilk_shared void myo_attach_gdb();

class _Cilk_shared CELO4
{
  public:
    int    fieldi;
    double fieldd;
    float  fieldf;
    char   fieldc;
    short  fields;
    static int no_objs_count;
    bool   fieldummy;

    CELO4() {
      fieldi = 5;
      fieldd = 8888.0;
      fieldf = 9.0;
      fieldc = 'A';
      fields = 127;
      no_objs_count++;
      fieldummy = true;
    }

    ~CELO4() {
      fieldi = -45;
      fieldd = 29.0;
      fieldf = 42.0;
      fieldc = 'B';
      fields = 13;
      no_objs_count--;
      fieldummy = false;
    }
};

int CELO4::no_objs_count = 0;

_Cilk_shared CELO4 c4;
CELO4 _Cilk_shared *c4ptr;

_Cilk_shared int localclass()
{
  volatile int superdummy = 1;
  CELO4 c4_local;
  superdummy = 2;
  return c4_local.no_objs_count;
}

_Cilk_shared int checkpoint()
{
  volatile int checkdummy = 1;
  return checkdummy;
}

int main()
{
  _Cilk_offload myo_attach_gdb();

  int num_objs = -1;

  c4ptr = new( _Offload_shared_malloc(sizeof(CELO4))) _Cilk_shared CELO4;

  num_objs = _Cilk_offload localclass();

    _Offload_shared_free(c4ptr);

  _Cilk_offload checkpoint();

  return 0;
}


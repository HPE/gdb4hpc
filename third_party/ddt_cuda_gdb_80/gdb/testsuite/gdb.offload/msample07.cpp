#include <new>

extern "C" _Cilk_shared void myo_attach_gdb();

class _Cilk_shared MSAMPLE07
{
    public:
        int    fieldi;
        double fieldd;
        float  fieldf;
        char   fieldc;
        short  fields;
        static int no_objs_count;
        bool   fielddummy;

    MSAMPLE07() {
        fieldi = 5;
        fieldd = 8888.3;
        fieldf = 9.3;
        fieldc = 'A';
        fields = 127;
        no_objs_count++;
        fielddummy = true;      /* fielddummy_breakpoint_constructor */
    }

    ~MSAMPLE07() {
        fieldi = -45;
        fieldd = 29.3;
        fieldf = 42.3;
        fieldc = 'B';
        fields = 13;
        no_objs_count--;
        fielddummy = false;     /* fielddummy_breakpoint_destructor */
    }
};

int MSAMPLE07::no_objs_count = 0;

_Cilk_shared MSAMPLE07 m7;
MSAMPLE07 _Cilk_shared *m7ptr;

_Cilk_shared int localclass()
{                               /* localclass_entry */
    volatile int superdummy = 1;
    MSAMPLE07 m7_local;         /* MSAMPLE07_local_create */
    superdummy = 2;
    return m7_local.no_objs_count;  /* localclass_end */
}

_Cilk_shared int checkpoint()
{
    volatile int checkdummy = 1;    /* checkpoint_breakpoint */
    return checkdummy;
}


int main(int argc, char* argv[]) {
    _Cilk_offload myo_attach_gdb();

    int num_objs = -1;
    m7ptr = new( _Offload_shared_malloc(sizeof(MSAMPLE07))) _Cilk_shared MSAMPLE07;

    num_objs = _Cilk_offload localclass();

    _Offload_shared_free(m7ptr);

    _Cilk_offload checkpoint();

    return 0;
}

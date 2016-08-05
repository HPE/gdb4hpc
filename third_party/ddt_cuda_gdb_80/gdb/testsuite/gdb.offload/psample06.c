extern void pragma_attach_gdb();

typedef double T;

#define SIZE 1000

#pragma offload_attribute(push, target(mic))
static T in1_02[SIZE];
static T in2_02[SIZE];
static T res_02[SIZE];
#pragma offload_attribute(pop)

static void populate(T* a, int s);

void psample06()
{
    populate(in1_02, SIZE);
    populate(in2_02, SIZE);

    #pragma offload target(mic) in(in1_02, in2_02) out(res_02)
    {
        int i;
        for (i=0; i<SIZE; i++)
        {
            res_02[i] = in1_02[i] + in2_02[i];  /* inside_offload_loop */
        }
    }

    int success = 0;

    if (res_02[0] == 0 && res_02[SIZE-1] == 2*(SIZE-1))
    {
        success = 1;
    }
}

static void populate(T* a, int s)
{
    int i;

    for (i=0; i<s; i++)
    {
        a[i] = i;
    }
}

int main()
{
    pragma_attach_gdb();
    psample06();
    return 0;
}

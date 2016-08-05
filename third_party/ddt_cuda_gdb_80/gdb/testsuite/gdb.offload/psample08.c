extern void pragma_attach_gdb();

typedef double T;
#define SIZE 1000

static T in1_04[SIZE];
static T in2_04[SIZE];
static __attribute((target(mic))) T res_04[SIZE];
static void populate(T* a, int s);

void psample08()
{
    T* p1, *p2;
    int s;
    populate(in1_04, SIZE);
    populate(in2_04, SIZE);

    p1 = in1_04;
    p2 = in2_04;
    s = SIZE;

    #pragma offload target(mic) in(p1, p2:length(s)) out(res_04)
    {
        int i;
        for (i=0; i<s; i++)
        {
            res_04[i] = p1[i] + p2[i];  /* calculation_in_offload_loop */
        }
    }

    int success = 0;
    if (res_04[0] == 0 && res_04[SIZE-1] == 2*(SIZE-1))
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
    psample08();
    return 0;
}

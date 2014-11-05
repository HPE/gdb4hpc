extern void pragma_attach_gdb();

typedef double T;

#define SIZE 1000

#pragma offload_attribute(push, target(mic))
static T in1_02[SIZE];
static T in2_02[SIZE];
static T res_02[SIZE];
static void populate_02(T* a, int s);
#pragma offload_attribute(pop)

void psample05()
{
    // Have i outside of the offload region
    int i;
    populate_02(in1_02, SIZE);
    populate_02(in2_02, SIZE);

    #pragma offload target(mic)
    {
        for (i=0; i<SIZE; i++)
        {
            res_02[i] = in1_02[i] + in2_02[i];  /* inside_offload_loop_1 */
        }
    }

    int success = 0;
    
    if (res_02[0] == 0 && res_02[SIZE-1] == 2*(SIZE-1))
    {
        success = 1;
    }
    
    // Have j inside the offload region
    populate_02(in1_02, SIZE);
    populate_02(in2_02, SIZE);

    #pragma offload target(mic)
    {
        int j;
        for (j=0; j<SIZE; j++)
        {
            res_02[j] = in1_02[j] + in2_02[j];  /* inside_offload_loop_2 */
        }
    }

    success = 0;
    
    if (res_02[0] == 0 && res_02[SIZE-1] == 2*(SIZE-1))
    {
        success = 1;
    }    
}

static void populate_02(T* a, int s)
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
    psample05();
    return 0;
}

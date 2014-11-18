extern void pragma_attach_gdb();

#define SIZE 1000

__attribute((target(mic))) int array1[SIZE];
__attribute((target(mic))) int send_array(int* p, int s);
__attribute((target(mic))) void compute(int* out, int size);

void psample07()
{
    int in_data[16] = { 1, 2, 3, 4, 5, 6, 7, 8,
                        9, 10, 11, 12, 13, 14, 15, 16 };
    int out_data[16];
    int array_sent = 0;
    int success = 0;

    #pragma offload target(mic : 0)
    {
        array_sent = send_array(in_data, 16);
    }

    if (array_sent)
    {
        success = 1;        /* check_if_offloaded */
    }

    #pragma offload target(mic : 0) out(out_data)
    {
        compute(out_data, 16);
    }

    success = 0;

    if (out_data[0] == 2 && out_data[15] == 32)
    {
        success = 1;
    }
}

__attribute((target(mic))) int send_array(int* p, int s)
{
    int retval;
    int i;

    for (i=0; i<s; i++)
    {
        array1[i] = p[i];
    }

#ifdef __MIC__
    retval = 1;
#else
    retval = 0;
#endif

    return retval;
}

__attribute((target(mic))) void compute(int* out, int size)
{
    int i;

    for (i=0; i<size; i++)
    {
        out[i] = array1[i]*2;       /* second_offload_inside_for_loop */
    }
}


int main()
{
    pragma_attach_gdb();
    psample07();
    return 0;
}

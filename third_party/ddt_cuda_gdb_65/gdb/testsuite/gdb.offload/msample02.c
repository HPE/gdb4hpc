extern _Cilk_shared myo_attach_gdb();

_Cilk_shared int global;

_Cilk_shared target(float val)
{
    // force reading the current value on MIC.
    volatile float old_val = global;

    // here global MUST be evaluated correctly.
    global += val;       /* write_shared */

    old_val = val;
}


int main(int argc, char* argv[])
{
    _Cilk_offload myo_attach_gdb();
    global = 1.0f;
    float copy = 0.0f;
    int tot_loops = 100;
    int i = 0;

    for(i=0; i<tot_loops; i++)
    {
        // force the proper value on host
        copy = global;
        // using two lines to avoid bp on shared function call.
        copy += 1.0f;
        // here global MUST be evaluated correctly,
        _Cilk_offload target(2.0f);
        // using two lines for easier bp handling.
        global += 1.0f;
    }

    return 0;
}

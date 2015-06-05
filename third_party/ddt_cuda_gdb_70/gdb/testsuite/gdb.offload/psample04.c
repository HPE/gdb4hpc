extern void pragma_attach_gdb();

float __attribute((target(mic))) psub_f(float val_f)
{
    float f = val_f;

    if((f - 1.0) > 0.0) {
        return f + psub_f(f - 1.0);
    } else {
        return f;       /* recursive_f_return */
    }
}

double __attribute((target(mic))) psub_d(double val_d)
{
    double d = val_d;

    if((d - 1.0) > 0.0) {
        return d + psub_d(d - 1.0);
    } else {
        return d;       /* recursive_d_return */
    }
}

void psample04()
{
    float num_it_f = 100.0;
    double num_it_d = 100.0;
    volatile int dummy = 0;

    float host_result_f;
    double host_result_d;

    #pragma offload target(mic)
    {
        dummy = 1;              /* dummy_bp_1 */
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                host_result_f = psub_f(num_it_f);   /* psub_f_call */
            }
            #pragma omp section
            {
                host_result_f = psub_d(num_it_d);   /* psub_d_call */
            }
        }
        dummy = 0;              /* dummy_bp_2 */
    }
}

int main()
{
    pragma_attach_gdb();
    psample04();
    return 0;
}

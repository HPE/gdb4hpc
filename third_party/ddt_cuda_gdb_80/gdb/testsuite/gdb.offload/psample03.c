extern void pragma_attach_gdb();

__attribute((target(mic))) int psample03(int* a)
{
    *a = 2;
    return 0;           /* breakpoint_at_return */
}

int main()
{
    pragma_attach_gdb();
    #pragma offload target(mic)
    {
        int val = 1;
        psample03(&val);    /* function_call */
        val = 0;            /* after_psample03 */
        val += 1;           /* last_user_offload_statement */
    }
    return 0;
}

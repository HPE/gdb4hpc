#include <wchar.h>

extern "C" void pragma_attach_gdb();

void psample02()
{
    #pragma offload target(mic)
    {
        int boolSize = sizeof(bool);
        int charSize = sizeof(char);
        int shortSize = sizeof(short);
        int intSize = sizeof(int);
        int longSize = sizeof(long);
        int longlongSize = sizeof(long long);
        int floatSize = sizeof(float);
        int doubleSize = sizeof(double);
        int longdoubleSize = sizeof(long double);
        int intptrSize = sizeof(int*);
        int wchar_tSize = sizeof(wchar_t);

        int dummy_for_breakpoint = 0;       /* dummy_for_breakpoint */
    }
}

int main()
{
    pragma_attach_gdb();
    psample02();
    return 0;
}

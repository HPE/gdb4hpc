#include <stdio.h>

extern _Cilk_shared myo_attach_gdb();

#define VSC 0x70
#define VSI 0x70000000
#define VSL 0x7000000000000000l
#define VSLL 0x7000000000000000ll
#define VF 10.0
#define VD 10101010.0
#define VLD 1010101010101010.0

typedef struct s {
    char        sc;
    int         si;
    long        sl;
    long long   sll;
    float       f;
    double      d;
    long double ld;
} celo_struct;

_Cilk_shared celo_struct SCI;
_Cilk_shared celo_struct* _Cilk_shared sci_ptr;

_Cilk_shared int init(void) {
    SCI.sc   = VSC;                     /* BP3 */
    SCI.si   = VSI;
    SCI.sl   = VSL;
    SCI.sll  = VSLL;
    SCI.f    = VF;
    SCI.d    = VD;
    SCI.ld   = VLD;
    return 0;                           /* BP2 */
}

_Cilk_shared int check() {
    int res = 0;
    celo_struct* ahah = sci_ptr;
    if (sci_ptr->sc != VSC)   res++;
    if (sci_ptr->si != VSI)   res++;
    if (sci_ptr->sl != VSL)   res++;
    if (sci_ptr->sll != VSLL) res++;
    if (sci_ptr->f != VF)     res++;
    if (sci_ptr->d != VD)     res++;
    if (sci_ptr->ld != VLD)   res++;    /* BP1 */
    return res;
}

_Cilk_shared void result(int val)
{
    volatile int superdummy = val;
    superdummy = val;
}

int main(void) {

    _Cilk_offload myo_attach_gdb();

    int res;

    sci_ptr = &SCI;

    /* perform tests initializing values on the host */
    init();

    res = _Cilk_offload check();
    _Cilk_offload result(res);
    //printf("Expected value 0  actual value %d\n", res);

    res = _Cilk_offload check();
    _Cilk_offload result(res);
    printf("Expected value 0  actual value %d\n", res);


    /* perform tests initializing values on MIC */
    _Cilk_offload init();

    res = _Cilk_offload check();
    _Cilk_offload result(res);
    //printf("Expected value 0  actual value %d\n", res);

    _Cilk_offload init();

    res = _Cilk_offload check();
    _Cilk_offload result(res);
    //printf("Expected value 0  actual value %d\n", res);

    return 0;
}


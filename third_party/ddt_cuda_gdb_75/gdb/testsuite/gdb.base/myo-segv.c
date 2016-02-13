#include <signal.h>
#define FORCE_SEGV 1
#define RAISE_SEGV 2

_Cilk_shared int foo;

void myo_segv() {
    /* Writing to a shared variable will cause a special SEGV,
       which should be catched by the MIC enabled GDB and
       not reported to the user. */
    foo = 1;            /* breakpoint1 */
}

void force_segv() {
    /* Force a "normal" SEGV, which should be reported to the user. */
    *(char *)0 = 0;     /* breakpoint3-1 */
}

void raise_segv() {
    /* Force a "normal" SEGV, using the raise command. */
    raise(SIGSEGV);     /* breakpoint3-2 */
}

int main(int argc, char *argv[]) {
    volatile int localDummy = 0;
    volatile int segv_switch = FORCE_SEGV;

    myo_segv();

    /* dummy */
    localDummy = 1;     /* breakpoint2 */

    switch (segv_switch) {
        case FORCE_SEGV:
            force_segv();
            break;
        case RAISE_SEGV:
            raise_segv();
            break;
    }

    /* dummy */
    localDummy = 2;     /* breakpoint4 */

    return 0;
}

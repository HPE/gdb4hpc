#include <math.h>
#include <stdio.h>
#if defined (__INTEL_COMPILER)
#define REGN "zmm"
#else
#define REGN "xmm"
#endif
int main(int argc, char *argv[]) {
    asm("mov $0x0101, %eax\n\
         kmov %eax,%k1");
    asm("mov $0x0202, %eax\n\
         kmov %eax,%k2");
    asm("mov $0x0303, %eax\n\
         kmov %eax,%k3");
    asm("mov $0x0404, %eax\n\
         kmov %eax,%k4");
    asm("mov $0x0505, %eax\n\
         kmov %eax,%k5");
    asm("mov $0x0606, %eax\n\
         kmov %eax,%k6");
    asm("mov $0x0707, %eax\n\
         kmov %eax,%k7");

    float f[32] = { 1.001, 1.111, 1.222, 1.333, 1.444, 1.555, 1.666, 1.777, 1.888, 1.999,
                    2.001, 2.111, 2.222, 2.333, 2.444, 2.555, 2.666, 2.777, 2.888, 2.999,
                    3.001, 3.111, 3.222, 3.333, 3.444, 3.555, 3.666, 3.777, 3.888, 3.999,
                    4.001, 4.111 };

    // Fill v16_float register values
    asm ("vbroadcastss 0(%0), %%zmm0\n\t"   /* breakpoint2 */
         "vbroadcastss 4(%0), %%zmm1\n\t"
         "vbroadcastss 8(%0), %%zmm2\n\t"
         "vbroadcastss 12(%0), %%zmm3\n\t"
         "vbroadcastss 16(%0), %%zmm4\n\t"
         "vbroadcastss 20(%0), %%zmm5\n\t"
         "vbroadcastss 24(%0), %%zmm6\n\t"
         "vbroadcastss 28(%0), %%zmm7\n\t"
         "vbroadcastss 32(%0), %%zmm8\n\t"
         "vbroadcastss 36(%0), %%zmm9\n\t"
         "vbroadcastss 40(%0), %%zmm10\n\t"
         "vbroadcastss 44(%0), %%zmm11\n\t"
         "vbroadcastss 48(%0), %%zmm12\n\t"
         "vbroadcastss 52(%0), %%zmm13\n\t"
         "vbroadcastss 56(%0), %%zmm14\n\t"
         "vbroadcastss 60(%0), %%zmm15\n\t"
         "vbroadcastss 64(%0), %%zmm16\n\t"
         "vbroadcastss 68(%0), %%zmm17\n\t"
         "vbroadcastss 72(%0), %%zmm18\n\t"
         "vbroadcastss 76(%0), %%zmm19\n\t"
         "vbroadcastss 80(%0), %%zmm20\n\t"
         "vbroadcastss 84(%0), %%zmm21\n\t"
         "vbroadcastss 88(%0), %%zmm22\n\t"
         "vbroadcastss 92(%0), %%zmm23\n\t"
         "vbroadcastss 96(%0), %%zmm24\n\t"
         "vbroadcastss 100(%0), %%zmm25\n\t"
         "vbroadcastss 104(%0), %%zmm26\n\t"
         "vbroadcastss 108(%0), %%zmm27\n\t"
         "vbroadcastss 112(%0), %%zmm28\n\t"
         "vbroadcastss 116(%0), %%zmm29\n\t"
         "vbroadcastss 120(%0), %%zmm30\n\t"
         "vbroadcastss 124(%0), %%zmm31\n\t"
        :
        : "r" (f)
        : REGN"0", REGN"1", REGN"2", REGN"3", REGN"4", REGN"5", REGN"6", REGN"7",
          REGN"8", REGN"9", REGN"10", REGN"11", REGN"12", REGN"13", REGN"14", REGN"15",
          REGN"16", REGN"17", REGN"18", REGN"19", REGN"20", REGN"21", REGN"22", REGN"23",
          REGN"24", REGN"25", REGN"26", REGN"27", REGN"28", REGN"29", REGN"30", REGN"31",
          "rdx"
    );

    double d[32] = { 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,  /* breakpoint3 */
                    2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9,
                    3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9,
                    4.0, 4.1 };

    // Fill v8_double register values
    asm ("vbroadcastsd 0(%0), %%zmm0\n\t"
         "vbroadcastsd 8(%0), %%zmm1\n\t"
         "vbroadcastsd 16(%0), %%zmm2\n\t"
         "vbroadcastsd 24(%0), %%zmm3\n\t"
         "vbroadcastsd 32(%0), %%zmm4\n\t"
         "vbroadcastsd 40(%0), %%zmm5\n\t"
         "vbroadcastsd 48(%0), %%zmm6\n\t"
         "vbroadcastsd 56(%0), %%zmm7\n\t"
         "vbroadcastsd 64(%0), %%zmm8\n\t"
         "vbroadcastsd 72(%0), %%zmm9\n\t"
         "vbroadcastsd 80(%0), %%zmm10\n\t"
         "vbroadcastsd 88(%0), %%zmm11\n\t"
         "vbroadcastsd 96(%0), %%zmm12\n\t"
         "vbroadcastsd 104(%0), %%zmm13\n\t"
         "vbroadcastsd 112(%0), %%zmm14\n\t"
         "vbroadcastsd 120(%0), %%zmm15\n\t"
         "vbroadcastsd 128(%0), %%zmm16\n\t"
         "vbroadcastsd 136(%0), %%zmm17\n\t"
         "vbroadcastsd 144(%0), %%zmm18\n\t"
         "vbroadcastsd 152(%0), %%zmm19\n\t"
         "vbroadcastsd 160(%0), %%zmm20\n\t"
         "vbroadcastsd 168(%0), %%zmm21\n\t"
         "vbroadcastsd 176(%0), %%zmm22\n\t"
         "vbroadcastsd 184(%0), %%zmm23\n\t"
         "vbroadcastsd 192(%0), %%zmm24\n\t"
         "vbroadcastsd 200(%0), %%zmm25\n\t"
         "vbroadcastsd 208(%0), %%zmm26\n\t"
         "vbroadcastsd 216(%0), %%zmm27\n\t"
         "vbroadcastsd 224(%0), %%zmm28\n\t"
         "vbroadcastsd 232(%0), %%zmm29\n\t"
         "vbroadcastsd 240(%0), %%zmm30\n\t"
         "vbroadcastsd 248(%0), %%zmm31\n\t"
        :
        : "r" (d)
        : REGN"0", REGN"1", REGN"2", REGN"3", REGN"4", REGN"5", REGN"6", REGN"7",
          REGN"8", REGN"9", REGN"10", REGN"11", REGN"12", REGN"13", REGN"14", REGN"15",
          REGN"16", REGN"17", REGN"18", REGN"19", REGN"20", REGN"21", REGN"22", REGN"23",
          REGN"24", REGN"25", REGN"26", REGN"27", REGN"28", REGN"29", REGN"30", REGN"31"
    );

    char c[32] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,       /* breakpoint4 */
                    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

    // Fill v64_int8 register values
    asm ("vbroadcasti32x4 0(%0), %%zmm0\n\t"
         "vbroadcasti32x4 16(%0), %%zmm1\n\t"
         "vbroadcasti32x4 0(%0), %%zmm2\n\t"
         "vbroadcasti32x4 16(%0), %%zmm3\n\t"
         "vbroadcasti32x4 0(%0), %%zmm4\n\t"
         "vbroadcasti32x4 16(%0), %%zmm5\n\t"
         "vbroadcasti32x4 0(%0), %%zmm6\n\t"
         "vbroadcasti32x4 16(%0), %%zmm7\n\t"
         "vbroadcasti32x4 0(%0), %%zmm8\n\t"
         "vbroadcasti32x4 16(%0), %%zmm9\n\t"
         "vbroadcasti32x4 0(%0), %%zmm10\n\t"
         "vbroadcasti32x4 16(%0), %%zmm11\n\t"
         "vbroadcasti32x4 0(%0), %%zmm12\n\t"
         "vbroadcasti32x4 16(%0), %%zmm13\n\t"
         "vbroadcasti32x4 0(%0), %%zmm14\n\t"
         "vbroadcasti32x4 16(%0), %%zmm15\n\t"
         "vbroadcasti32x4 0(%0), %%zmm16\n\t"
         "vbroadcasti32x4 16(%0), %%zmm17\n\t"
         "vbroadcasti32x4 0(%0), %%zmm18\n\t"
         "vbroadcasti32x4 16(%0), %%zmm19\n\t"
         "vbroadcasti32x4 0(%0), %%zmm20\n\t"
         "vbroadcasti32x4 16(%0), %%zmm21\n\t"
         "vbroadcasti32x4 0(%0), %%zmm22\n\t"
         "vbroadcasti32x4 16(%0), %%zmm23\n\t"
         "vbroadcasti32x4 0(%0), %%zmm24\n\t"
         "vbroadcasti32x4 16(%0), %%zmm25\n\t"
         "vbroadcasti32x4 0(%0), %%zmm26\n\t"
         "vbroadcasti32x4 16(%0), %%zmm27\n\t"
         "vbroadcasti32x4 0(%0), %%zmm28\n\t"
         "vbroadcasti32x4 16(%0), %%zmm29\n\t"
         "vbroadcasti32x4 0(%0), %%zmm30\n\t"
         "vbroadcasti32x4 16(%0), %%zmm31\n\t"
        :
        : "r" (c)
        : REGN"0", REGN"1", REGN"2", REGN"3", REGN"4", REGN"5", REGN"6", REGN"7",
          REGN"8", REGN"9", REGN"10", REGN"11", REGN"12", REGN"13", REGN"14", REGN"15",
          REGN"16", REGN"17", REGN"18", REGN"19", REGN"20", REGN"21", REGN"22", REGN"23",
          REGN"24", REGN"25", REGN"26", REGN"27", REGN"28", REGN"29", REGN"30", REGN"31"
    );


    short s[32] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,      /* breakpoint5 */
                    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

    // Fill v32_int16 register values
    asm ("vbroadcasti32x4 0(%0), %%zmm0\n\t"
         "vbroadcasti32x4 16(%0), %%zmm1\n\t"
         "vbroadcasti32x4 32(%0), %%zmm2\n\t"
         "vbroadcasti32x4 48(%0), %%zmm3\n\t"
         "vbroadcasti32x4 0(%0), %%zmm4\n\t"
         "vbroadcasti32x4 16(%0), %%zmm5\n\t"
         "vbroadcasti32x4 32(%0), %%zmm6\n\t"
         "vbroadcasti32x4 48(%0), %%zmm7\n\t"
         "vbroadcasti32x4 0(%0), %%zmm8\n\t"
         "vbroadcasti32x4 16(%0), %%zmm9\n\t"
         "vbroadcasti32x4 32(%0), %%zmm10\n\t"
         "vbroadcasti32x4 48(%0), %%zmm11\n\t"
         "vbroadcasti32x4 0(%0), %%zmm12\n\t"
         "vbroadcasti32x4 16(%0), %%zmm13\n\t"
         "vbroadcasti32x4 32(%0), %%zmm14\n\t"
         "vbroadcasti32x4 48(%0), %%zmm15\n\t"
         "vbroadcasti32x4 0(%0), %%zmm16\n\t"
         "vbroadcasti32x4 16(%0), %%zmm17\n\t"
         "vbroadcasti32x4 32(%0), %%zmm18\n\t"
         "vbroadcasti32x4 48(%0), %%zmm19\n\t"
         "vbroadcasti32x4 0(%0), %%zmm20\n\t"
         "vbroadcasti32x4 16(%0), %%zmm21\n\t"
         "vbroadcasti32x4 32(%0), %%zmm22\n\t"
         "vbroadcasti32x4 48(%0), %%zmm23\n\t"
         "vbroadcasti32x4 0(%0), %%zmm24\n\t"
         "vbroadcasti32x4 16(%0), %%zmm25\n\t"
         "vbroadcasti32x4 32(%0), %%zmm26\n\t"
         "vbroadcasti32x4 48(%0), %%zmm27\n\t"
         "vbroadcasti32x4 0(%0), %%zmm28\n\t"
         "vbroadcasti32x4 16(%0), %%zmm29\n\t"
         "vbroadcasti32x4 32(%0), %%zmm30\n\t"
         "vbroadcasti32x4 48(%0), %%zmm31\n\t"
        :
        : "r" (s)
        : REGN"0", REGN"1", REGN"2", REGN"3", REGN"4", REGN"5", REGN"6", REGN"7",
          REGN"8", REGN"9", REGN"10", REGN"11", REGN"12", REGN"13", REGN"14", REGN"15",
          REGN"16", REGN"17", REGN"18", REGN"19", REGN"20", REGN"21", REGN"22", REGN"23",
          REGN"24", REGN"25", REGN"26", REGN"27", REGN"28", REGN"29", REGN"30", REGN"31"
    );

    // Set v16_int32
    int i[16] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};   /* breakpoint6 */

    asm ("vbroadcasti32x4 0(%0), %%zmm0\n\t"
         "vbroadcasti32x4 16(%0), %%zmm1\n\t"
         "vbroadcasti32x4 32(%0), %%zmm2\n\t"
         "vbroadcasti32x4 0(%0), %%zmm3\n\t"
         "vbroadcasti32x4 16(%0), %%zmm4\n\t"
         "vbroadcasti32x4 32(%0), %%zmm5\n\t"
         "vbroadcasti32x4 0(%0), %%zmm6\n\t"
         "vbroadcasti32x4 16(%0), %%zmm7\n\t"
         "vbroadcasti32x4 32(%0), %%zmm8\n\t"
         "vbroadcasti32x4 0(%0), %%zmm9\n\t"
         "vbroadcasti32x4 16(%0), %%zmm10\n\t"
         "vbroadcasti32x4 32(%0), %%zmm11\n\t"
         "vbroadcasti32x4 0(%0), %%zmm12\n\t"
         "vbroadcasti32x4 16(%0), %%zmm13\n\t"
         "vbroadcasti32x4 32(%0), %%zmm14\n\t"
         "vbroadcasti32x4 16(%0), %%zmm15\n\t"
         "vbroadcasti32x4 32(%0), %%zmm16\n\t"
         "vbroadcasti32x4 0(%0), %%zmm17\n\t"
         "vbroadcasti32x4 16(%0), %%zmm18\n\t"
         "vbroadcasti32x4 32(%0), %%zmm19\n\t"
         "vbroadcasti32x4 0(%0), %%zmm20\n\t"
         "vbroadcasti32x4 16(%0), %%zmm21\n\t"
         "vbroadcasti32x4 32(%0), %%zmm22\n\t"
         "vbroadcasti32x4 0(%0), %%zmm23\n\t"
         "vbroadcasti32x4 16(%0), %%zmm24\n\t"
         "vbroadcasti32x4 32(%0), %%zmm25\n\t"
         "vbroadcasti32x4 0(%0), %%zmm26\n\t"
         "vbroadcasti32x4 16(%0), %%zmm27\n\t"
         "vbroadcasti32x4 32(%0), %%zmm28\n\t"
         "vbroadcasti32x4 0(%0), %%zmm29\n\t"
         "vbroadcasti32x4 16(%0), %%zmm30\n\t"
         "vbroadcasti32x4 32(%0), %%zmm31\n\t"
        :
        : "r" (i)
        : REGN"0", REGN"1", REGN"2", REGN"3", REGN"4", REGN"5", REGN"6", REGN"7",
          REGN"8", REGN"9", REGN"10", REGN"11", REGN"12", REGN"13", REGN"14", REGN"15",
          REGN"16", REGN"17", REGN"18", REGN"19", REGN"20", REGN"21", REGN"22", REGN"23",
          REGN"24", REGN"25", REGN"26", REGN"27", REGN"28", REGN"29", REGN"30", REGN"31"
    );

    long long ll[16] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};  /* breakpoint7 */

    // Fill v8_int64 register values
    asm ("vbroadcasti32x4 0(%0), %%zmm0\n\t"
         "vbroadcasti32x4 16(%0), %%zmm1\n\t"
         "vbroadcasti32x4 32(%0), %%zmm2\n\t"
         "vbroadcasti32x4 48(%0), %%zmm3\n\t"
         "vbroadcasti32x4 64(%0), %%zmm4\n\t"
         "vbroadcasti32x4 80(%0), %%zmm5\n\t"
         "vbroadcasti32x4 96(%0), %%zmm6\n\t"
         "vbroadcasti32x4 112(%0), %%zmm7\n\t"
         "vbroadcasti32x4 0(%0), %%zmm8\n\t"
         "vbroadcasti32x4 16(%0), %%zmm9\n\t"
         "vbroadcasti32x4 32(%0), %%zmm10\n\t"
         "vbroadcasti32x4 48(%0), %%zmm11\n\t"
         "vbroadcasti32x4 64(%0), %%zmm12\n\t"
         "vbroadcasti32x4 80(%0), %%zmm13\n\t"
         "vbroadcasti32x4 96(%0), %%zmm14\n\t"
         "vbroadcasti32x4 112(%0), %%zmm15\n\t"
         "vbroadcasti32x4 0(%0), %%zmm16\n\t"
         "vbroadcasti32x4 16(%0), %%zmm17\n\t"
         "vbroadcasti32x4 32(%0), %%zmm18\n\t"
         "vbroadcasti32x4 48(%0), %%zmm19\n\t"
         "vbroadcasti32x4 64(%0), %%zmm20\n\t"
         "vbroadcasti32x4 80(%0), %%zmm21\n\t"
         "vbroadcasti32x4 96(%0), %%zmm22\n\t"
         "vbroadcasti32x4 112(%0), %%zmm23\n\t"
         "vbroadcasti32x4 0(%0), %%zmm24\n\t"
         "vbroadcasti32x4 16(%0), %%zmm25\n\t"
         "vbroadcasti32x4 32(%0), %%zmm26\n\t"
         "vbroadcasti32x4 48(%0), %%zmm27\n\t"
         "vbroadcasti32x4 64(%0), %%zmm28\n\t"
         "vbroadcasti32x4 80(%0), %%zmm29\n\t"
         "vbroadcasti32x4 96(%0), %%zmm30\n\t"
         "vbroadcasti32x4 112(%0), %%zmm31\n\t"
        :
        : "r" (ll)
        : REGN"0", REGN"1", REGN"2", REGN"3", REGN"4", REGN"5", REGN"6", REGN"7",
          REGN"8", REGN"9", REGN"10", REGN"11", REGN"12", REGN"13", REGN"14", REGN"15",
          REGN"16", REGN"17", REGN"18", REGN"19", REGN"20", REGN"21", REGN"22", REGN"23",
          REGN"24", REGN"25", REGN"26", REGN"27", REGN"28", REGN"29", REGN"30", REGN"31"
    );

    return 0;   /* breakpoint8 */
}

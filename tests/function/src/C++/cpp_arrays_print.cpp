#include "mpi.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    // arrays
    char one_d[] = "Hello world!\n";
    unsigned char one_d_u[] = "Numbers"; // will be printed as decimal numbers
    signed char one_d_s[] = "Hello world! (signed)\t\n"; // treated the same as char
    char two_d[4][16] = {"Hello", "World!\n", "Empty ->", ""};
    unsigned char two_d_u[4][4] = {"abc", "123", "xyz", "098"};
    int intarray[] = {1, 2, 3, 4, 5};
    int intarray2d[2][2] = {{1, 2}, {3, 4}};
    int intarray2d2[3][2] = {{1, 2}, {3, 4}, {5, 6}};
    float floatarray[] = {1.1, 2.2, 3.3, 4.4, 5.5};

    // pointers
    char *cstring = "hello \n world!";
    char *cstring2 = "hola mundo!";
    char *cstring3 = "hallo wereld!";
    char *cptr = cstring;
    const char *const_cstring = "const hello world!";
    char *empty = "";
    char *cstringarray[] = {cstring, cstring2, cstring3};
    int inte = 23;
    int *intp = &inte;
    int inte2 = 24;
    int *intp2 = &inte2;
    int *intparray[] = {intp, intp2};

    double array3d[10][12][20];
    for (int i=0; i<10; ++i) {
        for (int j=0; j<12; ++j) {
            for (int k = 0; k<20; ++k) {
                array3d[i][j][k] = i + 0.1 * j + 0.01 * k;
            }
        }
    }
    
    MPI_Finalize();
    return 0;
}

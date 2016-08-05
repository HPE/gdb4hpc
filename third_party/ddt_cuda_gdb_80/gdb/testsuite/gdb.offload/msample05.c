extern _Cilk_shared myo_attach_gdb();

_Cilk_shared param_int(int val) {
    volatile int local_val = val;   /* int1 */
    local_val++;
    val = local_val;
}

_Cilk_shared param_double(double val) {
    volatile double local_val = val;   /* double1 */
    local_val++;
    val = local_val;
}

_Cilk_shared param_float(float val) {
    volatile float local_val = val;   /* float1 */
    local_val++;
    val = local_val;
}


int main(int argc, char* argv[]) {
    _Cilk_offload myo_attach_gdb();

    volatile int intp = 42;
    volatile double doublep = 42.42;
    volatile float floatp = 42.424242f;

    _Cilk_offload param_int(intp);
    _Cilk_offload param_double(doublep);
    _Cilk_offload param_float(floatp);

    return 0;
}

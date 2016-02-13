extern _Cilk_shared myo_attach_gdb();

int main() {
    _Cilk_offload myo_attach_gdb();
    return 0;
}

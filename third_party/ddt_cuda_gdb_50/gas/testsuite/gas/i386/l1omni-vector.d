#as: -march=l1om
#objdump: -dw
#name: L1OMni Vector ops

.*: +file format .*

Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	d6 00 08 9e c2          	vmaxabsps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 9e c2          	vmaxabsps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 9e c2          	vmaxabsps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 9e c2          	vmaxabsps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 9e c2          	vmaxabsps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 9e c2          	vmaxabsps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 9e c2          	vmaxabsps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 9e c2          	vmaxabsps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 9e c2          	vmaxabsps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 9e c2          	vmaxabsps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 9e c7          	vmaxabsps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 9e c0          	vmaxabsps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 9e c1          	vmaxabsps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 9e c7          	vmaxabsps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 9e c0          	vmaxabsps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 9e c1          	vmaxabsps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 9e c7          	vmaxabsps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 9e c7          	vmaxabsps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 9e cf          	vmaxabsps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 9e ff          	vmaxabsps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 9e ff          	vmaxabsps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 9e ff          	vmaxabsps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 9e ff          	vmaxabsps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 9e ff          	vmaxabsps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 9e 00          	vmaxabsps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 9e 00          	vmaxabsps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 9e 00          	vmaxabsps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 9e 00          	vmaxabsps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 9e 00          	vmaxabsps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 9e 00          	vmaxabsps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 9e 00          	vmaxabsps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 9e 00          	vmaxabsps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 9e 00          	vmaxabsps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 9e 00          	vmaxabsps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 9e 00          	vmaxabsps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 9e 84 08 04 00 00 00 	vmaxabsps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 9e 44 08 01    	vmaxabsps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 9e 44 08 ff    	vmaxabsps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 43 c2          	vaddsetcpi %v2,%k1,%v0
[ 	]*[a-f0-9]+:	d6 00 07 43 c2          	vaddsetcpi %v2,%k0,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 17 43 8c 08 04 00 00 00 	vaddsetcpi 0x4\(%r8,%r9,1\)\{nt\},%k2,%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 1f 43 54 08 01    	vaddsetcpi 0x40\(%r8,%r9,1\)\{nt\},%k3,%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 9d c2          	vcvtps2srgb8 %v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 9d c2          	vcvtps2srgb8 %v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 9d c2          	vcvtps2srgb8 %v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 9d c2          	vcvtps2srgb8 %v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9d c2          	vcvtps2srgb8 %v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 9d ff          	vcvtps2srgb8 %v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 00          	vcvtps2srgb8 \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 9d 00          	vcvtps2srgb8 \(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 9d 07          	vcvtps2srgb8 \(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9d 00          	vcvtps2srgb8 \(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 9d 00          	vcvtps2srgb8 \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9d 00          	vcvtps2srgb8 \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9d 00          	vcvtps2srgb8 \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 83 07 9d 8c 08 04 00 00 00 	vcvtps2srgb8 0x4\(%r8,%r9,1\)\{nt\},%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 07 9d 54 08 01    	vcvtps2srgb8 0x40\(%r8,%r9,1\)\{nt\},%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 07 9d c2          	vcvtps2srgb8 %v2,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 f0 c2 00 00    	vfixupps \$0x0,%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 f0 c2 00 00    	vfixupps \$0x0,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 f0 c2 00 00    	vfixupps \$0x0,%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f0 c2 00 00    	vfixupps \$0x0,%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f0 c2 00 00    	vfixupps \$0x0,%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 f0 ff 00 00    	vfixupps \$0x0,%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 01 f0 00 00 00    	vfixupps \$0x0,\(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 f0 00 00 00    	vfixupps \$0x0,\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 00    	vfixupps \$0x0,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 00    	vfixupps \$0x0,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 01 00    	vfixupps \$0x1,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 07 00    	vfixupps \$0x7,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 7f 00    	vfixupps \$0x7f,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 01    	vfixupps \$0x100,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 40    	vfixupps \$0x4000,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 ff ff    	vfixupps \$0xffff,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 f0 00 00 00    	vfixupps \$0x10000,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 10 00          	vloadd \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 10 00          	vloadd \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 10 00          	vloadd \(%rax\)\{1to16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 10 00          	vloadd \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 10 00          	vloadd \(%rax\)\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 19 10 00          	vloadd \(%rax\)\{1to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 19 10 00          	vloadd \(%rax\)\{4to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 01 10 00          	vloadd \(%rax\)\{nt\}\{4to16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 10 00          	vloadd \(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 19 10 00          	vloadd \(%rax\)\{nt\}\{1to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 19 10 00          	vloadd \(%rax\)\{nt\}\{1to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 14 00          	vstored %v0,\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 14 00          	vstored %v0,\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 20 01 14 00          	vstored %v0\{dcba\},\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 14 00          	vstored %v0\{sint8\},\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 14 00          	vstored %v0\{dcba\}\{sint8\},\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 14 00          	vstored %v0\{dcba\}\{sint8\},\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 01 14 00          	vstored %v0\{dcba\},\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 29 14 00          	vstored %v0\{sint8\},\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 90 29 14 00          	vstored %v0\{a\}\{sint8\},\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 90 29 14 00          	vstored %v0\{a\}\{sint8\},\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 90 29 14 00          	vstored %v0\{a\}\{sint8\},\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 14 40 04       	vstored %v0\{float16rz\},0x80\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 09 45 c2          	vadcpi %v2\{aaaa\},%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 09 45 c2          	vadcpi %v2\{bbbb\},%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 09 45 c2          	vadcpi %v2\{cccc\},%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 3f 45 ff          	vadcpi %v31\{cccc\},%k7,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 09 45 c2          	vadcpi %v2,%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 45 00          	vadcpi \(%rax\),%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 09 45 00          	vadcpi \(%r8\),%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 09 45 07          	vadcpi \(%r15\),%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 09 45 00          	vadcpi \(%rax\)\{sint8\},%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 09 45 00          	vadcpi \(%rax\)\{nt\},%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 45 00          	vadcpi \(%rax\)\{nt\}\{sint8\},%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 45 00          	vadcpi \(%rax\)\{nt\}\{sint8\},%k1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 08 41 c2          	vaddpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 41 c2          	vaddpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 41 c2          	vaddpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 41 c2          	vaddpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 41 c2          	vaddpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 41 c2          	vaddpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 41 c2          	vaddpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 41 c2          	vaddpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 41 c2          	vaddpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 41 c2          	vaddpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 41 c7          	vaddpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 41 c0          	vaddpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 41 c1          	vaddpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 41 c7          	vaddpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 41 c0          	vaddpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 41 c1          	vaddpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 41 c7          	vaddpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 41 c7          	vaddpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 41 cf          	vaddpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 41 ff          	vaddpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 41 ff          	vaddpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 41 ff          	vaddpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 41 ff          	vaddpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 41 ff          	vaddpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 41 00          	vaddpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 41 00          	vaddpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 41 00          	vaddpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 41 00          	vaddpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 41 00          	vaddpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 41 00          	vaddpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 41 00          	vaddpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 41 00          	vaddpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 41 00          	vaddpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 41 00          	vaddpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 41 00          	vaddpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 41 84 08 04 00 00 00 	vaddpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 41 44 08 01    	vaddpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 41 44 08 ff    	vaddpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 9b c2          	vcvtinsps2f11 \$0x0,\{rn\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 9b c2          	vcvtinsps2f11 \$0x0,\{rn\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 9b c2          	vcvtinsps2f11 \$0x0,\{rn\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 9b c2          	vcvtinsps2f11 \$0x0,\{rn\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9b c2          	vcvtinsps2f11 \$0x0,\{rn\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 9b ff          	vcvtinsps2f11 \$0x0,\{rn\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 9b 00          	vcvtinsps2f11 \$0x0,\{rn\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 9b 00          	vcvtinsps2f11 \$0x0,\{rn\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 9b 07          	vcvtinsps2f11 \$0x0,\{rn\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9b 00          	vcvtinsps2f11 \$0x0,\{rn\},\(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 9b 00          	vcvtinsps2f11 \$0x0,\{rn\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9b 00          	vcvtinsps2f11 \$0x0,\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 c1 9b 00          	vcvtinsps2f11 \$0x0,\{rz\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9b 00          	vcvtinsps2f11 \$0x0,\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 9b 00          	vcvtinsps2f11 \$0x1,\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 9b 00          	vcvtinsps2f11 \$0x2,\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 9b 80 16 00 00 00 	vcvtinsps2f11 \$0x2,\{rn\},0x16\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 9b 84 98 16 00 00 00 	vcvtinsps2f11 \$0x2,\{rn\},0x16\(%rax,%rbx,4\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 95 c2          	vcvtpi2ps \{.0b\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 95 c2          	vcvtpi2ps \{.0b\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 95 c2          	vcvtpi2ps \{.0b\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 95 c2          	vcvtpi2ps \{.0b\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 95 c2          	vcvtpi2ps \{.0b\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 95 ff          	vcvtpi2ps \{.0b\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 95 00          	vcvtpi2ps \{.0b\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 95 00          	vcvtpi2ps \{.0b\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 95 07          	vcvtpi2ps \{.0b\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 95 00          	vcvtpi2ps \{.0b\},\(%rax\)\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 95 00          	vcvtpi2ps \{.0b\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 95 00          	vcvtpi2ps \{.0b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 95 00          	vcvtpi2ps \{.4b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 95 00          	vcvtpi2ps \{.5b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 19 95 00          	vcvtpi2ps \{.8b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 21 95 00          	vcvtpi2ps \{.16b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 29 95 00          	vcvtpi2ps \{.24b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 31 95 00          	vcvtpi2ps \{.31b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 39 95 00          	vcvtpi2ps \{.32b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 95 40 01       	vcvtpi2ps \{.0b\},0x10\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 95 80 08 00 00 00 	vcvtpi2ps \{.0b\},0x8\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 95 80 04 00 00 00 	vcvtpi2ps \{.0b\},0x4\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 95 40 01       	vcvtpi2ps \{.0b\},0x10\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 95 44 98 01    	vcvtpi2ps \{.0b\},0x10\(%rax,%rbx,4\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 91 c2          	vcvtps2pi \{.0b\},\{rn\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 91 c2          	vcvtps2pi \{.0b\},\{rn\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 91 c2          	vcvtps2pi \{.0b\},\{rn\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 91 c2          	vcvtps2pi \{.0b\},\{rn\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 91 c2          	vcvtps2pi \{.0b\},\{rn\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 91 ff          	vcvtps2pi \{.0b\},\{rn\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 91 00          	vcvtps2pi \{.0b\},\{rn\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 91 00          	vcvtps2pi \{.0b\},\{rn\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 91 07          	vcvtps2pi \{.0b\},\{rn\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 91 00          	vcvtps2pi \{.0b\},\{rn\},\(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 91 00          	vcvtps2pi \{.0b\},\{rn\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 91 00          	vcvtps2pi \{.0b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 91 00          	vcvtps2pi \{.4b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 91 00          	vcvtps2pi \{.5b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 19 91 00          	vcvtps2pi \{.8b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 21 91 00          	vcvtps2pi \{.16b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 29 91 00          	vcvtps2pi \{.24b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 31 91 00          	vcvtps2pi \{.31b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 39 91 00          	vcvtps2pi \{.32b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 79 91 00          	vcvtps2pi \{.32b\},\{rd\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 b9 91 00          	vcvtps2pi \{.32b\},\{ru\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 f9 91 00          	vcvtps2pi \{.32b\},\{rz\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 91 40 01       	vcvtps2pi \{.0b\},\{rn\},0x10\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 91 80 08 00 00 00 	vcvtps2pi \{.0b\},\{rn\},0x8\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 91 80 04 00 00 00 	vcvtps2pi \{.0b\},\{rn\},0x4\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 91 40 01       	vcvtps2pi \{.0b\},\{rn\},0x10\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 91 44 98 01    	vcvtps2pi \{.0b\},\{rn\},0x10\(%rax,%rbx,4\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 00 00    	vinsertfieldpi \$0x0,\$0x0,\$0x0,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 00 08    	vinsertfieldpi \$0x1,\$0x0,\$0x0,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 40 00    	vinsertfieldpi \$0x0,\$0x1,\$0x0,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 01 00    	vinsertfieldpi \$0x0,\$0x0,\$0x1,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 01 08    	vinsertfieldpi \$0x1,\$0x0,\$0x1,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 41 08    	vinsertfieldpi \$0x1,\$0x1,\$0x1,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 c1 79    	vinsertfieldpi \$0xf,\$0x7,\$0x1,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 88 18    	vinsertfieldpi \$0x3,\$0x2,\$0x8,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 d0 98    	vinsertfieldpi \$0x13,\$0x3,\$0x10,%v2,%v1,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 0f f2 c2 d0 98    	vinsertfieldpi \$0x13,\$0x3,\$0x10,%v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f f2 40 01 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x10\(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f f2 80 01 00 00 00 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x1\(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f f2 44 98 01 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x10\(%rax,%rbx,4\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f f2 84 98 01 00 00 00 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x1\(%rax,%rbx,4\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 00 00    	vrotatefieldpi \$0x0,\$0x0,\$0x0,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 00 08    	vrotatefieldpi \$0x1,\$0x0,\$0x0,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 40 00    	vrotatefieldpi \$0x0,\$0x1,\$0x0,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 01 00    	vrotatefieldpi \$0x0,\$0x0,\$0x1,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 01 08    	vrotatefieldpi \$0x1,\$0x0,\$0x1,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 41 08    	vrotatefieldpi \$0x1,\$0x1,\$0x1,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 c1 79    	vrotatefieldpi \$0xf,\$0x7,\$0x1,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 88 18    	vrotatefieldpi \$0x3,\$0x2,\$0x8,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 d0 98    	vrotatefieldpi \$0x13,\$0x3,\$0x10,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 07 f3 c2 d0 98    	vrotatefieldpi \$0x13,\$0x3,\$0x10,%v2\{cccc\},%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 07 f3 40 01 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x10\(%rax\)\{nt\}\{sint8\},%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 07 f3 80 01 00 00 00 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x1\(%rax\)\{nt\}\{sint8\},%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 07 f3 44 98 01 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x10\(%rax,%rbx,4\)\{nt\}\{sint8\},%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 07 f3 84 98 01 00 00 00 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x1\(%rax,%rbx,4\)\{nt\}\{sint8\},%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 21 c9          	vcmppi \{eq\},%v1\{aaaa\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 21 c9          	vcmppi \{eq\},%v1\{bbbb\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 f9          	vcmppi \{eq\},%v1,%v0,%k7\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 07 21 c9          	vcmppi \{eq\},%v1,%v0,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 f9 21 c9          	vcmppi \{eq\},%v1,%v31,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 03 01 21 cf          	vcmppi \{eq\},%v31,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 21 c9          	vcmppi \{lt\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 21 c9          	vcmppi \{le\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 25 c9          	vcmppi \{neq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 25 c9          	vcmppi \{nlt\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 25 c9          	vcmppi \{nle\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 21 08          	vcmppi \{lt\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 21 08          	vcmppi \{le\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 25 08          	vcmppi \{neq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 25 08          	vcmppi \{nlt\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 25 08          	vcmppi \{nle\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 21 08          	vcmppi \{eq\},\(%rax\)\{nt\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 21 08          	vcmppi \{eq\},\(%rax\)\{sint8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 21 08          	vcmppi \{eq\},\(%rax\)\{nt\}\{sint8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 21 08          	vcmppi \{eq\},\(%rax\)\{nt\}\{sint8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 22 c9          	vcmppu \{eq\},%v1\{aaaa\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 22 c9          	vcmppu \{eq\},%v1\{bbbb\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 f9          	vcmppu \{eq\},%v1,%v0,%k7\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 07 22 c9          	vcmppu \{eq\},%v1,%v0,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 f9 22 c9          	vcmppu \{eq\},%v1,%v31,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 03 01 22 cf          	vcmppu \{eq\},%v31,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 22 c9          	vcmppu \{lt\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 22 c9          	vcmppu \{le\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 26 c9          	vcmppu \{neq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 26 c9          	vcmppu \{nlt\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 26 c9          	vcmppu \{nle\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 22 08          	vcmppu \{lt\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 22 08          	vcmppu \{le\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 26 08          	vcmppu \{neq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 26 08          	vcmppu \{nlt\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 26 08          	vcmppu \{nle\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 22 08          	vcmppu \{eq\},\(%rax\)\{nt\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 22 08          	vcmppu \{eq\},\(%rax\)\{sint8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 22 08          	vcmppu \{eq\},\(%rax\)\{nt\}\{sint8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 22 08          	vcmppu \{eq\},\(%rax\)\{nt\}\{sint8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 20 c9          	vcmpps \{eq\},%v1\{aaaa\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 20 c9          	vcmpps \{eq\},%v1\{bbbb\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 f9          	vcmpps \{eq\},%v1,%v0,%k7\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 07 20 c9          	vcmpps \{eq\},%v1,%v0,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 f9 20 c9          	vcmpps \{eq\},%v1,%v31,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 03 01 20 cf          	vcmpps \{eq\},%v31,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps \{eq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 20 c9          	vcmpps \{lt\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 20 c9          	vcmpps \{le\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 24 c9          	vcmpps \{neq\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 24 c9          	vcmpps \{nlt\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 24 c9          	vcmpps \{nle\},%v1,%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps \{eq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 20 08          	vcmpps \{lt\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 20 08          	vcmpps \{le\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 24 08          	vcmpps \{neq\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 24 08          	vcmpps \{nlt\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 08 01 24 08          	vcmpps \{nle\},\(%rax\),%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 20 08          	vcmpps \{eq\},\(%rax\)\{nt\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 20 08          	vcmpps \{eq\},\(%rax\)\{unorm8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 20 08          	vcmpps \{eq\},\(%rax\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 20 08          	vcmpps \{eq\},\(%rax\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 dc 01 20 08          	vcmpps \{unord\},\(%rax\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 dc 01 24 08          	vcmpps \{ord\},\(%rax\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 9a c2          	vcvtinsps2u10 \$0x0,%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 9a c2          	vcvtinsps2u10 \$0x0,%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 9a c2          	vcvtinsps2u10 \$0x0,%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 9a c2          	vcvtinsps2u10 \$0x0,%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9a c2          	vcvtinsps2u10 \$0x0,%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 9a ff          	vcvtinsps2u10 \$0x0,%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 9a 00          	vcvtinsps2u10 \$0x0,\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 9a 07          	vcvtinsps2u10 \$0x0,\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 \$0x0,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 9a 00          	vcvtinsps2u10 \$0x1,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 9a 00          	vcvtinsps2u10 \$0x2,\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 9a 40 01       	vcvtinsps2u10 \$0x2,0x10\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 9a 80 16 00 00 00 	vcvtinsps2u10 \$0x2,0x16\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 9a 84 98 16 00 00 00 	vcvtinsps2u10 \$0x2,0x16\(%rax,%rbx,4\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 92 c2          	vcvtps2pu \{.0b\},\{rn\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 92 c2          	vcvtps2pu \{.0b\},\{rn\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 92 c2          	vcvtps2pu \{.0b\},\{rn\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 92 c2          	vcvtps2pu \{.0b\},\{rn\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 92 c2          	vcvtps2pu \{.0b\},\{rn\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 92 ff          	vcvtps2pu \{.0b\},\{rn\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 92 00          	vcvtps2pu \{.0b\},\{rn\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 92 00          	vcvtps2pu \{.0b\},\{rn\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 92 07          	vcvtps2pu \{.0b\},\{rn\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 92 00          	vcvtps2pu \{.0b\},\{rn\},\(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 92 00          	vcvtps2pu \{.0b\},\{rn\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 92 00          	vcvtps2pu \{.0b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 92 00          	vcvtps2pu \{.4b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 92 00          	vcvtps2pu \{.5b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 19 92 00          	vcvtps2pu \{.8b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 21 92 00          	vcvtps2pu \{.16b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 29 92 00          	vcvtps2pu \{.24b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 31 92 00          	vcvtps2pu \{.31b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 39 92 00          	vcvtps2pu \{.32b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 79 92 00          	vcvtps2pu \{.32b\},\{rd\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 b9 92 00          	vcvtps2pu \{.32b\},\{ru\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 f9 92 00          	vcvtps2pu \{.32b\},\{rz\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 92 40 01       	vcvtps2pu \{.0b\},\{rn\},0x10\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 92 80 08 00 00 00 	vcvtps2pu \{.0b\},\{rn\},0x8\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 92 80 04 00 00 00 	vcvtps2pu \{.0b\},\{rn\},0x4\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 92 40 01       	vcvtps2pu \{.0b\},\{rn\},0x10\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 92 44 98 01    	vcvtps2pu \{.0b\},\{rn\},0x10\(%rax,%rbx,4\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 96 c2          	vcvtpu2ps \{.0b\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 96 c2          	vcvtpu2ps \{.0b\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 96 c2          	vcvtpu2ps \{.0b\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 96 c2          	vcvtpu2ps \{.0b\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 96 c2          	vcvtpu2ps \{.0b\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 96 ff          	vcvtpu2ps \{.0b\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 96 00          	vcvtpu2ps \{.0b\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 96 00          	vcvtpu2ps \{.0b\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 96 07          	vcvtpu2ps \{.0b\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 96 00          	vcvtpu2ps \{.0b\},\(%rax\)\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 96 00          	vcvtpu2ps \{.0b\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 96 00          	vcvtpu2ps \{.0b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 96 00          	vcvtpu2ps \{.4b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 96 00          	vcvtpu2ps \{.5b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 19 96 00          	vcvtpu2ps \{.8b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 21 96 00          	vcvtpu2ps \{.16b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 29 96 00          	vcvtpu2ps \{.24b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 31 96 00          	vcvtpu2ps \{.31b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 39 96 00          	vcvtpu2ps \{.32b\},\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 96 40 01       	vcvtpu2ps \{.0b\},0x10\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 96 80 08 00 00 00 	vcvtpu2ps \{.0b\},0x8\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 96 80 04 00 00 00 	vcvtpu2ps \{.0b\},0x4\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 96 40 01       	vcvtpu2ps \{.0b\},0x10\(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 96 44 98 01    	vcvtpu2ps \{.0b\},0x10\(%rax,%rbx,4\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 12 00          	vloadunpackld \(%rax\),%v0
[ 	]*[a-f0-9]+:	d6 00 00 12 38          	vloadunpackld \(%rax\),%v7
[ 	]*[a-f0-9]+:	d6 04 00 12 00          	vloadunpackld \(%rax\),%v8
[ 	]*[a-f0-9]+:	d6 04 00 12 38          	vloadunpackld \(%rax\),%v15
[ 	]*[a-f0-9]+:	d6 0c 00 12 38          	vloadunpackld \(%rax\),%v31
[ 	]*[a-f0-9]+:	d6 01 00 12 00          	vloadunpackld \(%r8\),%v0
[ 	]*[a-f0-9]+:	d6 01 00 12 07          	vloadunpackld \(%r15\),%v0
[ 	]*[a-f0-9]+:	d6 0d 00 12 3f          	vloadunpackld \(%r15\),%v31
[ 	]*[a-f0-9]+:	d6 80 00 12 00          	vloadunpackld \(%rax\)\{nt\},%v0
[ 	]*[a-f0-9]+:	d6 00 01 12 00          	vloadunpackld \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 12 00          	vloadunpackld \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 13 00          	vloadunpackhd \(%rax\),%v0
[ 	]*[a-f0-9]+:	d6 80 00 13 00          	vloadunpackhd \(%rax\)\{nt\},%v0
[ 	]*[a-f0-9]+:	d6 00 01 13 00          	vloadunpackhd \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 13 00          	vloadunpackhd \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 16 00          	vpackstoreld %v0,\(%rax\)
[ 	]*[a-f0-9]+:	d6 80 00 16 00          	vpackstoreld %v0,\(%rax\)\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 16 00          	vpackstoreld %v0,\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 16 00          	vpackstoreld %v0,\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 01 00 16 00          	vpackstoreld %v0,\(%r8\)
[ 	]*[a-f0-9]+:	d6 81 00 16 07          	vpackstoreld %v0,\(%r15\)\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 16 00          	vpackstoreld %v0,\(%rax\)\{%k7\}
[ 	]*[a-f0-9]+:	d6 84 01 16 38          	vpackstoreld %v15,\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 01 16 40 20       	vpackstoreld %v0,0x80\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 17 00          	vpackstorehd %v0,\(%rax\)
[ 	]*[a-f0-9]+:	d6 80 00 17 00          	vpackstorehd %v0,\(%rax\)\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 17 00          	vpackstorehd %v0,\(%rax\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 17 00          	vpackstorehd %v0,\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 01 00 17 00          	vpackstorehd %v0,\(%r8\)
[ 	]*[a-f0-9]+:	d6 81 00 17 07          	vpackstorehd %v0,\(%r15\)\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 17 00          	vpackstorehd %v0,\(%rax\)\{%k7\}
[ 	]*[a-f0-9]+:	d6 84 01 17 38          	vpackstorehd %v15,\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 01 17 40 20       	vpackstorehd %v0,0x80\(%rax\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 98 c2          	vroundps \{.0b\},\{rn\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 98 c2          	vroundps \{.0b\},\{rn\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 98 c2          	vroundps \{.0b\},\{rn\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 98 c2          	vroundps \{.0b\},\{rn\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 98 c2          	vroundps \{.0b\},\{rn\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 98 ff          	vroundps \{.0b\},\{rn\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 98 00          	vroundps \{.0b\},\{rn\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 98 00          	vroundps \{.0b\},\{rn\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 98 07          	vroundps \{.0b\},\{rn\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 98 00          	vroundps \{.0b\},\{rn\},\(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 98 00          	vroundps \{.0b\},\{rn\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 98 00          	vroundps \{.0b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 09 98 00          	vroundps \{.4b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 11 98 00          	vroundps \{.5b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 19 98 00          	vroundps \{.8b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 21 98 00          	vroundps \{.16b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 29 98 00          	vroundps \{.24b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 31 98 00          	vroundps \{.31b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 39 98 00          	vroundps \{.32b\},\{rn\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 79 98 00          	vroundps \{.32b\},\{rd\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 b9 98 00          	vroundps \{.32b\},\{ru\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 f9 98 00          	vroundps \{.32b\},\{rz\},\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 98 40 01       	vroundps \{.0b\},\{rn\},0x10\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 98 80 08 00 00 00 	vroundps \{.0b\},\{rn\},0x8\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 98 80 04 00 00 00 	vroundps \{.0b\},\{rn\},0x4\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 98 40 01       	vroundps \{.0b\},\{rn\},0x10\(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 98 44 98 01    	vroundps \{.0b\},\{rn\},0x10\(%rax,%rbx,4\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 00 30 c9          	vtestpi %v1,%v0,%k1
[ 	]*[a-f0-9]+:	d6 30 00 30 c9          	vtestpi %v1\{aaaa\},%v0,%k1
[ 	]*[a-f0-9]+:	d6 40 00 30 c9          	vtestpi %v1\{bbbb\},%v0,%k1
[ 	]*[a-f0-9]+:	d6 00 00 30 f9          	vtestpi %v1,%v0,%k7
[ 	]*[a-f0-9]+:	d6 00 07 30 c9          	vtestpi %v1,%v0,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 f8 30 c9          	vtestpi %v1,%v31,%k1
[ 	]*[a-f0-9]+:	d6 03 00 30 cf          	vtestpi %v31,%v0,%k1
[ 	]*[a-f0-9]+:	d6 00 00 30 08          	vtestpi \(%rax\),%v0,%k1
[ 	]*[a-f0-9]+:	d6 00 00 30 38          	vtestpi \(%rax\),%v0,%k7
[ 	]*[a-f0-9]+:	d6 00 07 30 08          	vtestpi \(%rax\),%v0,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 f8 30 08          	vtestpi \(%rax\),%v31,%k1
[ 	]*[a-f0-9]+:	d6 01 00 30 0f          	vtestpi \(%r15\),%v0,%k1
[ 	]*[a-f0-9]+:	d6 80 00 30 08          	vtestpi \(%rax\)\{nt\},%v0,%k1
[ 	]*[a-f0-9]+:	d6 50 00 30 08          	vtestpi \(%rax\)\{sint8\},%v0,%k1
[ 	]*[a-f0-9]+:	d6 d0 00 30 08          	vtestpi \(%rax\)\{nt\}\{sint8\},%v0,%k1
[ 	]*[a-f0-9]+:	d6 d0 00 30 08          	vtestpi \(%rax\)\{nt\}\{sint8\},%v0,%k1
[ 	]*[a-f0-9]+:	d6 83 0f 30 8c 08 04 00 00 00 	vtestpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 30 4c 08 01    	vtestpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 30 4c 08 ff    	vtestpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%k1\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 00    	vshuf128x32 \{aaaa\},\{0000\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 0f 07 f1 ff 00 00    	vshuf128x32 \{aaaa\},\{0000\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 00 00 00    	vshuf128x32 \{aaaa\},\{0000\},\(%rax\),%v0
[ 	]*[a-f0-9]+:	d6 01 00 f1 00 00 00    	vshuf128x32 \{aaaa\},\{0000\},\(%r8\),%v0
[ 	]*[a-f0-9]+:	d6 80 00 f1 00 00 00    	vshuf128x32 \{aaaa\},\{0000\},\(%rax\)\{nt\},%v0
[ 	]*[a-f0-9]+:	d6 01 00 f1 07 00 00    	vshuf128x32 \{aaaa\},\{0000\},\(%r15\),%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 03    	vshuf128x32 \{aaad\},\{0000\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 0f    	vshuf128x32 \{aadd\},\{0000\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 3f    	vshuf128x32 \{addd\},\{0000\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 ff    	vshuf128x32 \{dddd\},\{0000\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 02 ff    	vshuf128x32 \{dddd\},\{0002\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 22 ff    	vshuf128x32 \{dddd\},\{0202\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 ff ff    	vshuf128x32 \{dddd\},\{3333\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 ff 1b    	vshuf128x32 \{abcd\},\{3333\},%v2,%v0
[ 	]*[a-f0-9]+:	d6 00 00 b0 c2          	vexp2lutps %v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 b0 c2          	vexp2lutps %v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b0 c2          	vexp2lutps %v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 c2          	vexp2lutps %v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 c2          	vexp2lutps %v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 b0 ff          	vexp2lutps %v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 00          	vexp2lutps \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b0 00          	vexp2lutps \(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b0 07          	vexp2lutps \(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 00          	vexp2lutps \(%rax\)\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b0 00          	vexp2lutps \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b0 00          	vexp2lutps \(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b0 00          	vexp2lutps \(%rax\)\{nt\}\{sint8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 83 07 b0 8c 08 04 00 00 00 	vexp2lutps 0x4\(%r8,%r9,1\)\{nt\},%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 07 b0 54 08 01    	vexp2lutps 0x40\(%r8,%r9,1\)\{nt\},%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 07 b0 c2          	vexp2lutps %v2,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 b4 c2          	vgetexpps %v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 b4 c2          	vgetexpps %v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b4 c2          	vgetexpps %v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 c2          	vgetexpps %v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 c2          	vgetexpps %v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 b4 ff          	vgetexpps %v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 00          	vgetexpps \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b4 00          	vgetexpps \(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b4 07          	vgetexpps \(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 00          	vgetexpps \(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b4 00          	vgetexpps \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b4 00          	vgetexpps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b4 00          	vgetexpps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 83 07 b4 8c 08 04 00 00 00 	vgetexpps 0x4\(%r8,%r9,1\)\{nt\},%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 07 b4 54 08 01    	vgetexpps 0x40\(%r8,%r9,1\)\{nt\},%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 07 b4 c2          	vgetexpps %v2,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 c0 c2          	vrcpresps %v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 c0 c2          	vrcpresps %v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 c0 c2          	vrcpresps %v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 c2          	vrcpresps %v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 c2          	vrcpresps %v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 c0 ff          	vrcpresps %v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 00          	vrcpresps \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 c0 00          	vrcpresps \(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 c0 07          	vrcpresps \(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 00          	vrcpresps \(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 c0 00          	vrcpresps \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 c0 00          	vrcpresps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 c0 00          	vrcpresps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 83 07 c0 8c 08 04 00 00 00 	vrcpresps 0x4\(%r8,%r9,1\)\{nt\},%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 07 c0 54 08 01    	vrcpresps 0x40\(%r8,%r9,1\)\{nt\},%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 07 c0 c2          	vrcpresps %v2,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 b2 c2          	vrsqrtlutps %v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 b2 c2          	vrsqrtlutps %v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b2 c2          	vrsqrtlutps %v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 c2          	vrsqrtlutps %v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 c2          	vrsqrtlutps %v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 b2 ff          	vrsqrtlutps %v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 00          	vrsqrtlutps \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b2 00          	vrsqrtlutps \(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b2 07          	vrsqrtlutps \(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 00          	vrsqrtlutps \(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b2 00          	vrsqrtlutps \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b2 00          	vrsqrtlutps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b2 00          	vrsqrtlutps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 83 07 b2 8c 08 04 00 00 00 	vrsqrtlutps 0x4\(%r8,%r9,1\)\{nt\},%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 07 b2 54 08 01    	vrsqrtlutps 0x40\(%r8,%r9,1\)\{nt\},%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 07 b2 c2          	vrsqrtlutps %v2,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 55 c2          	vsbbpi %v2,%k1,%v0
[ 	]*[a-f0-9]+:	d6 00 07 55 c2          	vsbbpi %v2,%k0,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 17 55 8c 08 04 00 00 00 	vsbbpi 0x4\(%r8,%r9,1\)\{nt\},%k2,%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 1f 55 54 08 01    	vsbbpi 0x40\(%r8,%r9,1\)\{nt\},%k3,%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 5d c2          	vsbbrpi %v2,%k1,%v0
[ 	]*[a-f0-9]+:	d6 00 07 5d c2          	vsbbrpi %v2,%k0,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 17 5d 8c 08 04 00 00 00 	vsbbrpi 0x4\(%r8,%r9,1\)\{nt\},%k2,%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 1f 5d 54 08 01    	vsbbrpi 0x40\(%r8,%r9,1\)\{nt\},%k3,%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 5b c2          	vsubrsetbpi %v2,%k1,%v0
[ 	]*[a-f0-9]+:	d6 00 07 5b c2          	vsubrsetbpi %v2,%k0,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 17 5b 8c 08 04 00 00 00 	vsubrsetbpi 0x4\(%r8,%r9,1\)\{nt\},%k2,%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 1f 5b 54 08 01    	vsubrsetbpi 0x40\(%r8,%r9,1\)\{nt\},%k3,%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 53 c2          	vsubsetbpi %v2,%k1,%v0
[ 	]*[a-f0-9]+:	d6 00 07 53 c2          	vsubsetbpi %v2,%k0,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 17 53 8c 08 04 00 00 00 	vsubsetbpi 0x4\(%r8,%r9,1\)\{nt\},%k2,%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 1f 53 54 08 01    	vsubsetbpi 0x40\(%r8,%r9,1\)\{nt\},%k3,%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 00 b1 c2          	vlog2lutps %v2,%v0
[ 	]*[a-f0-9]+:	d6 00 01 b1 c2          	vlog2lutps %v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b1 c2          	vlog2lutps %v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 c2          	vlog2lutps %v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 c2          	vlog2lutps %v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 5f 07 b1 ff          	vlog2lutps %v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 00          	vlog2lutps \(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b1 00          	vlog2lutps \(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 01 01 b1 07          	vlog2lutps \(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 00          	vlog2lutps \(%rax\)\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b1 00          	vlog2lutps \(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b1 00          	vlog2lutps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 d0 01 b1 00          	vlog2lutps \(%rax\)\{nt\}\{unorm8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 83 07 b1 8c 08 04 00 00 00 	vlog2lutps 0x4\(%r8,%r9,1\)\{nt\},%v1\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 07 b1 54 08 01    	vlog2lutps 0x40\(%r8,%r9,1\)\{nt\},%v2\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 07 b1 c2          	vlog2lutps %v2,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 08 10 21 08          	vcmppi \{le\},\(%rax\),%v2,%k1
[ 	]*[a-f0-9]+:	d6 08 12 21 08          	vcmppi \{le\},\(%rax\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 08 18 20 08          	vcmpps \{le\},\(%rax\),%v3,%k1
[ 	]*[a-f0-9]+:	d6 08 48 22 08          	vcmppu \{le\},\(%rax\),%v9,%k1
[ 	]*[a-f0-9]+:	d6 08 10 21 c9          	vcmppi \{le\},%v1,%v2,%k1
[ 	]*[a-f0-9]+:	d6 08 12 21 ca          	vcmppi \{le\},%v2,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 08 18 20 cb          	vcmpps \{le\},%v3,%v3,%k1
[ 	]*[a-f0-9]+:	d6 08 48 22 cc          	vcmppu \{le\},%v4,%v9,%k1
[ 	]*[a-f0-9]+:	d6 00 00 10 04 05 40 00 00 00 	vloadd 0x40\(,%rax,1\),%v0
[ 	]*[a-f0-9]+:	d6 00 00 10 04 05 c0 ff ff ff 	vloadd -0x40\(,%rax,1\),%v0
[ 	]*[a-f0-9]+:	d6 00 08 48 c2          	vaddnps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 48 c2          	vaddnps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 48 c2          	vaddnps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 48 c2          	vaddnps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 48 c2          	vaddnps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 48 c2          	vaddnps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 48 c2          	vaddnps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 48 c2          	vaddnps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 48 c2          	vaddnps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 48 c2          	vaddnps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 48 c7          	vaddnps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 48 c0          	vaddnps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 48 c1          	vaddnps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 48 c7          	vaddnps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 48 c0          	vaddnps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 48 c1          	vaddnps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 48 c7          	vaddnps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 48 c7          	vaddnps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 48 cf          	vaddnps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 48 ff          	vaddnps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 48 ff          	vaddnps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 48 ff          	vaddnps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 48 ff          	vaddnps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 48 ff          	vaddnps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 48 00          	vaddnps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 48 00          	vaddnps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 48 00          	vaddnps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 48 00          	vaddnps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 48 00          	vaddnps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 48 00          	vaddnps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 48 00          	vaddnps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 48 00          	vaddnps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 48 00          	vaddnps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 48 00          	vaddnps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 48 00          	vaddnps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 48 84 08 04 00 00 00 	vaddnps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 48 44 08 01    	vaddnps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 48 44 08 ff    	vaddnps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 00 08 48 c2       	vaddnpd %v2,%v1,%v0
[ 	]*[a-f0-9]+:	66 d6 00 08 48 c2       	vaddnpd %v2,%v1,%v0
[ 	]*[a-f0-9]+:	66 d6 00 0f 48 c2       	vaddnpd %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 10 0f 48 c2       	vaddnpd %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 20 0f 48 c2       	vaddnpd %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 30 0f 48 c2       	vaddnpd %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 40 0f 48 c2       	vaddnpd %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 50 0f 48 c2       	vaddnpd %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 60 0f 48 c2       	vaddnpd %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 70 0f 48 c2       	vaddnpd %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 70 0f 48 c7       	vaddnpd %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 71 0f 48 c0       	vaddnpd %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 71 0f 48 c1       	vaddnpd %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 71 0f 48 c7       	vaddnpd %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 72 0f 48 c0       	vaddnpd %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 72 0f 48 c1       	vaddnpd %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 73 0f 48 c7       	vaddnpd %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 77 0f 48 c7       	vaddnpd %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 77 0f 48 cf       	vaddnpd %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 77 0f 48 ff       	vaddnpd %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 77 47 48 ff       	vaddnpd %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 77 4f 48 ff       	vaddnpd %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 77 7f 48 ff       	vaddnpd %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 7f ff 48 ff       	vaddnpd %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 00 0f 48 00       	vaddnpd \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 00 0f 48 00       	vaddnpd \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 10 0f 48 00       	vaddnpd \(%rax\)\{1to8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 20 0f 48 00       	vaddnpd \(%rax\)\{4to8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 80 0f 48 00       	vaddnpd \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 80 0f 48 00       	vaddnpd \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 83 0f 48 84 08 04 00 00 00 	vaddnpd 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 83 0f 48 44 08 01 	vaddnpd 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 83 0f 48 44 08 ff 	vaddnpd -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 90 19 10 40 01       	vloadd 0x1\(%rax\)\{nt\}\{1to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 41 10 80 01 00 00 00 	vloadd 0x1\(%rax\)\{nt\}\{1to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 41 10 40 01       	vloadd 0x2\(%rax\)\{nt\}\{1to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 10 80 01 00 00 00 	vloadd 0x1\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 19 10 80 01 00 00 00 	vloadd 0x1\(%rax\)\{nt\}\{4to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 01 00 00 00 	vloadd 0x1\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 02 00 00 00 	vloadd 0x2\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 03 00 00 00 	vloadd 0x3\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 04 00 00 00 	vloadd 0x4\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 05 00 00 00 	vloadd 0x5\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 06 00 00 00 	vloadd 0x6\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 07 00 00 00 	vloadd 0x7\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 40 01       	vloadd 0x8\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 41 10 80 09 00 00 00 	vloadd 0x9\(%rax\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 10 40 01       	vloadd 0x10\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 10 80 0f 00 00 00 	vloadd 0xf\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 10 80 01 00 00 00 	vloadd 0x1\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 10 80 01 00 00 00 	vloadd 0x1\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 10 80 20 00 00 00 	vloadd 0x20\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 10 40 01       	vloadd 0x40\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 13 40 7f       	vloadunpackhd 0x7f\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 13 40 20       	vloadunpackhd 0x80\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 13 80 80 00 00 00 	vloadunpackhd 0x80\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 12 40 7f       	vloadunpackld 0x7f\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 12 40 20       	vloadunpackld 0x80\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 12 80 80 00 00 00 	vloadunpackld 0x80\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 13 40 10    	vloadunpackhq 0x80\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 12 40 10    	vloadunpacklq 0x80\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 19 10 00          	vloadd \(%rax\)\{nt\}\{1to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 19 10 40 08       	vloadd 0x80\(%rax\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 19 10 40 7e       	vloadd 0x7e\(%rax\)\{nt\}\{1to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 19 10 40 20       	vloadd 0x80\(%rax\)\{nt\}\{4to16\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 28 14 00          	vstored %v0\{a\}\{sint8\},\(%rax\)
[ 	]*[a-f0-9]+:	d6 00 28 14 40 08       	vstored %v0\{sint8\},0x80\(%rax\)
[ 	]*[a-f0-9]+:	d6 10 28 14 40 7e       	vstored %v0\{a\}\{sint8\},0x7e\(%rax\)
[ 	]*[a-f0-9]+:	d6 20 28 14 40 20       	vstored %v0\{dcba\}\{sint8\},0x80\(%rax\)
[ 	]*[a-f0-9]+:	d6 00 28 17 40 7f       	vpackstorehd %v0\{sint8\},0x7f\(%rax\)
[ 	]*[a-f0-9]+:	d6 00 28 17 80 80 00 00 00 	vpackstorehd %v0\{sint8\},0x80\(%rax\)
[ 	]*[a-f0-9]+:	d6 80 00 17 40 20       	vpackstorehd %v0,0x80\(%rax\)\{nt\}
[ 	]*[a-f0-9]+:	d6 00 28 16 40 7f       	vpackstoreld %v0\{sint8\},0x7f\(%rax\)
[ 	]*[a-f0-9]+:	d6 00 28 16 80 80 00 00 00 	vpackstoreld %v0\{sint8\},0x80\(%rax\)
[ 	]*[a-f0-9]+:	d6 80 00 16 40 20       	vpackstoreld %v0,0x80\(%rax\)\{nt\}
[ 	]*[a-f0-9]+:	66 d6 80 00 17 40 10    	vpackstorehq %v0,0x80\(%rax\)\{nt\}
[ 	]*[a-f0-9]+:	66 d6 80 00 16 40 10    	vpackstorelq %v0,0x80\(%rax\)\{nt\}
[ 	]*[a-f0-9]+:	66 d6 5a 08 96 9c b1 33 22 11 00 	vcvtpu2pd \$0x1,0x112233\(%rcx,%r14,4\)\{sint8\},%v19
[ 	]*[a-f0-9]+:	d6 5a 08 96 9c b1 33 22 11 00 	vcvtpu2ps \{.4b\},0x112233\(%rcx,%r14,4\)\{sint8\},%v19
[ 	]*[a-f0-9]+:	66 d6 08 06 94 d2       	vcvtps2pd \$0x0,%v2,%v18\{%k6\}
[ 	]*[a-f0-9]+:	d6 08 24 13 9c 4a 22 11 00 00 	vloadunpackhd 0x1122\(%rdx,%rcx,2\)\{uint8\},%v19\{%k4\}
[ 	]*[a-f0-9]+:	d6 84 80 12 9c 08 22 11 00 00 	vloadunpackld 0x1122\(%rax,%rcx,1\)\{nt\}\{unorm10A\},%v11
[ 	]*[a-f0-9]+:	d6 85 4a 17 b4 aa 11 00 00 00 	vpackstorehd %v14\{sint16\},0x11\(%r10,%rbp,4\)\{%k2\}\{nt\}
[ 	]*[a-f0-9]+:	d6 08 2e 16 7c 3a 11    	vpackstoreld %v23\{sint8\},0x11\(%rdx,%rdi,1\)\{%k6\}
[ 	]*[a-f0-9]+:	62 0b 84 44 11 00 00 00 	vprefetch1 \$0x0,0x11\(%r12,%r8,2\)
[ 	]*[a-f0-9]+:	62 db 44 3d 00          	vprefetch2 \$0x0,0x0\(%rbp,%rdi,1\)
[ 	]*[a-f0-9]+:	d6 00 01 18 04 98       	vgatherd \(%rax,%v3,4\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 01 18 04 f0       	vgatherd \(%rax,%v22,8\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 b0 19 18 44 30 7f    	vgatherd 0x7f\(%rax,%v30,1\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 18 44 50 20    	vgatherd 0x80\(%rax,%v10,2\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 19 18 84 88 80 00 00 00 	vgatherd 0x80\(%rax,%v9,4\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 1d 00 00 00 00 	vgatherd 0x0\(,%v11,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 07 19 04 98       	vgatherpfd \(%rax,%v3,4\)\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 06 19 04 e0       	vgatherpfd \(%rax,%v4,8\)\{%k6\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 1d 19 44 28 7f    	vgatherpfd 0x7f\(%rax,%v5,1\)\{%k5\}\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 80 04 19 44 70 20    	vgatherpfd 0x80\(%rax,%v6,2\)\{%k4\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 1b 19 84 b8 80 00 00 00 	vgatherpfd 0x80\(%rax,%v7,4\)\{%k3\}\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 07 19 04 9d 00 00 00 00 	vgatherpfd 0x0\(,%v3,4\)\{%k7\}
[ 	]*[a-f0-9]+:	d6 04 01 1c 1c 0d 80 00 00 00 	vscatterd %v11,0x80\(,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 1c 1c 0d 00 00 00 00 	vscatterd %v11,0x0\(,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 1c 1c 08       	vscatterd %v11,\(%rax,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 84 01 1c 14 50       	vscatterd %v10,\(%rax,%v2,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 04 01 1c 0c 98       	vscatterd %v9,\(%rax,%v3,4\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 29 1c 04 d8       	vscatterd %v8\{sint8\},\(%rax,%v3,8\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 1c 3c 20       	vscatterd %v7\{sint8\},\(%rax,%v4,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 1c 34 68       	vscatterd %v6\{sint8\},\(%rax,%v5,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 1c 2c b0       	vscatterd %v5,\(%rax,%v6,4\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 29 1c 24 f8       	vscatterd %v4\{sint8\},\(%rax,%v7,8\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 90 29 1c 1c 00       	vscatterd %v3\{sint8\},\(%rax,%v8,1\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 90 29 1c 14 48       	vscatterd %v2\{sint8\},\(%rax,%v9,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 90 29 1c 0c 90       	vscatterd %v1\{sint8\},\(%rax,%v10,4\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 11 1c 44 d8 40    	vscatterd %v0\{float16rz\},0x80\(%rax,%v11,8\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 b0       	vscatterpfd \(%rax,%v6,4\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 80       	vscatterpfd \(%rax,%v0,4\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 08       	vscatterpfd \(%rax,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 50       	vscatterpfd \(%rax,%v2,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 98       	vscatterpfd \(%rax,%v3,4\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 44 98 20    	vscatterpfd 0x80\(%rax,%v3,4\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 0d 00 00 00 00 	vscatterpfd 0x0\(,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 0d 80 00 00 00 	vscatterpfd 0x80\(,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 d8       	vscatterpfd \(%rax,%v3,8\)\{%k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 20       	vscatterpfd \(%rax,%v4,1\)\{%k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 68       	vscatterpfd \(%rax,%v5,2\)\{%k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 80 29 1d 04 f8       	vscatterpfd \(%rax,%v7,8\)\{%k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 00       	vscatterpfd \(%rax,%v8,1\)\{%k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 48       	vscatterpfd \(%rax,%v9,2\)\{%k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 90       	vscatterpfd \(%rax,%v10,4\)\{%k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 11 1d 44 d8 40    	vscatterpfd 0x80\(%rax,%v11,8\)\{%k1\}\{float16rz\}
[ 	]*[a-f0-9]+:	66 d6 5a 08 95 9c b1 33 22 11 00 	vcvtpi2pd \$0x1,0x112233\(%rcx,%r14,4\)\{sint8\},%v19
[ 	]*[a-f0-9]+:	66 d6 4a 08 94 9c b1 33 22 11 00 	vcvtps2pd \$0x1,0x112233\(%rcx,%r14,4\)\{uint8\},%v19
[ 	]*[a-f0-9]+:	66 d6 5a 08 96 9c b1 33 22 11 00 	vcvtpu2pd \$0x1,0x112233\(%rcx,%r14,4\)\{sint8\},%v19
[ 	]*[a-f0-9]+:	66 d6 00 08 91 c2       	vcvtpd2pi \$0x1,\{rn\},%v2,%v0
[ 	]*[a-f0-9]+:	66 d6 00 09 91 c2       	vcvtpd2pi \$0x1,\{rn\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 91 c2       	vcvtpd2pi \$0x1,\{rn\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 91 c2       	vcvtpd2pi \$0x1,\{rn\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 91 c2       	vcvtpd2pi \$0x1,\{rn\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 5f 0f 91 ff       	vcvtpd2pi \$0x1,\{rn\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 00       	vcvtpd2pi \$0x1,\{rn\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 01 09 91 00       	vcvtpd2pi \$0x1,\{rn\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 01 09 91 07       	vcvtpd2pi \$0x1,\{rn\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 91 00       	vcvtpd2pi \$0x1,\{rn\},\(%rax\)\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 91 00       	vcvtpd2pi \$0x1,\{rn\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 91 00       	vcvtpd2pi \$0x1,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 91 00       	vcvtpd2pi \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 91 00       	vcvtpd2pi \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 91 00       	vcvtpd2pi \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 91 00       	vcvtpd2pi \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 91 00       	vcvtpd2pi \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 91 00       	vcvtpd2pi \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 91 00       	vcvtpd2pi \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 41 91 00       	vcvtpd2pi \$0x0,\{rd\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 81 91 00       	vcvtpd2pi \$0x0,\{ru\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 c1 91 00       	vcvtpd2pi \$0x0,\{rz\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 91 40 02    	vcvtpd2pi \$0x1,\{rn\},0x10\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 91 40 01    	vcvtpd2pi \$0x1,\{rn\},0x8\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 91 80 04 00 00 00 	vcvtpd2pi \$0x1,\{rn\},0x4\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 91 40 02    	vcvtpd2pi \$0x1,\{rn\},0x10\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 91 44 98 02 	vcvtpd2pi \$0x1,\{rn\},0x10\(%rax,%rbx,4\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 08 92 c2       	vcvtpd2pu \$0x1,\{rn\},%v2,%v0
[ 	]*[a-f0-9]+:	66 d6 00 09 92 c2       	vcvtpd2pu \$0x1,\{rn\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 92 c2       	vcvtpd2pu \$0x1,\{rn\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 92 c2       	vcvtpd2pu \$0x1,\{rn\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 92 c2       	vcvtpd2pu \$0x1,\{rn\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 5f 0f 92 ff       	vcvtpd2pu \$0x1,\{rn\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 00       	vcvtpd2pu \$0x1,\{rn\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 01 09 92 00       	vcvtpd2pu \$0x1,\{rn\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 01 09 92 07       	vcvtpd2pu \$0x1,\{rn\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 92 00       	vcvtpd2pu \$0x1,\{rn\},\(%rax\)\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 92 00       	vcvtpd2pu \$0x1,\{rn\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 92 00       	vcvtpd2pu \$0x1,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 92 00       	vcvtpd2pu \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 92 00       	vcvtpd2pu \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 92 00       	vcvtpd2pu \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 92 00       	vcvtpd2pu \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 92 00       	vcvtpd2pu \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 92 00       	vcvtpd2pu \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 92 00       	vcvtpd2pu \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 41 92 00       	vcvtpd2pu \$0x0,\{rd\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 81 92 00       	vcvtpd2pu \$0x0,\{ru\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 c1 92 00       	vcvtpd2pu \$0x0,\{rz\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 92 40 02    	vcvtpd2pu \$0x1,\{rn\},0x10\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 92 40 01    	vcvtpd2pu \$0x1,\{rn\},0x8\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 92 80 04 00 00 00 	vcvtpd2pu \$0x1,\{rn\},0x4\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 92 40 02    	vcvtpd2pu \$0x1,\{rn\},0x10\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 92 44 98 02 	vcvtpd2pu \$0x1,\{rn\},0x10\(%rax,%rbx,4\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 08 90 c2       	vcvtpd2ps \$0x1,\{rn\},%v2,%v0
[ 	]*[a-f0-9]+:	66 d6 00 09 90 c2       	vcvtpd2ps \$0x1,\{rn\},%v2,%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 90 c2       	vcvtpd2ps \$0x1,\{rn\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 90 c2       	vcvtpd2ps \$0x1,\{rn\},%v2\{bbbb\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 90 c2       	vcvtpd2ps \$0x1,\{rn\},%v2\{cccc\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 5f 0f 90 ff       	vcvtpd2ps \$0x1,\{rn\},%v31\{cccc\},%v31\{%k7\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 00       	vcvtpd2ps \$0x1,\{rn\},\(%rax\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 01 09 90 00       	vcvtpd2ps \$0x1,\{rn\},\(%r8\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 01 09 90 07       	vcvtpd2ps \$0x1,\{rn\},\(%r15\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 90 00       	vcvtpd2ps \$0x1,\{rn\},\(%rax\)\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 90 00       	vcvtpd2ps \$0x1,\{rn\},\(%rax\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 90 00       	vcvtpd2ps \$0x1,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 90 00       	vcvtpd2ps \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 90 00       	vcvtpd2ps \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 90 00       	vcvtpd2ps \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 90 00       	vcvtpd2ps \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 90 00       	vcvtpd2ps \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 90 00       	vcvtpd2ps \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 01 90 00       	vcvtpd2ps \$0x0,\{rn\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 41 90 00       	vcvtpd2ps \$0x0,\{rd\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 81 90 00       	vcvtpd2ps \$0x0,\{ru\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 c1 90 00       	vcvtpd2ps \$0x0,\{rz\},\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 90 40 02    	vcvtpd2ps \$0x1,\{rn\},0x10\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 90 40 01    	vcvtpd2ps \$0x1,\{rn\},0x8\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 90 80 04 00 00 00 	vcvtpd2ps \$0x1,\{rn\},0x4\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 90 40 02    	vcvtpd2ps \$0x1,\{rn\},0x10\(%rax\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 90 09 90 44 98 02 	vcvtpd2ps \$0x1,\{rn\},0x10\(%rax,%rbx,4\)\{nt\}\{1to8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 0c 08 24 ca       	vcmppd \{ord\},%v2,%v1,%k1
[ 	]*[a-f0-9]+:	d6 ad a8 10 9c fa 22 11 00 00 	vloadd 0x1122\(%r10,%rdi,8\)\{nt\}\{4to16\}\{float11B\},%v27
#pass

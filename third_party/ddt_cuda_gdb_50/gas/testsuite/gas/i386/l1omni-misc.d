#as: -march=l1om
#objdump: -dw
#name: L1OMni misc ops

.*: +file format .*


Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	62 cd c8                	vkand  %k1,%k0
[ 	]*[a-f0-9]+:	62 cd f9                	vkand  %k7,%k1
[ 	]*[a-f0-9]+:	62 dd c8                	vkandn %k1,%k0
[ 	]*[a-f0-9]+:	62 dd f9                	vkandn %k7,%k1
[ 	]*[a-f0-9]+:	62 ed c8                	vkandnr %k1,%k0
[ 	]*[a-f0-9]+:	62 ed f9                	vkandnr %k7,%k1
[ 	]*[a-f0-9]+:	62 fb c8                	vkmovlhb %k1,%k0
[ 	]*[a-f0-9]+:	62 fb f9                	vkmovlhb %k7,%k1
[ 	]*[a-f0-9]+:	62 fd c8                	vknot  %k1,%k0
[ 	]*[a-f0-9]+:	62 fd f9                	vknot  %k7,%k1
[ 	]*[a-f0-9]+:	62 ce c8                	vkor   %k1,%k0
[ 	]*[a-f0-9]+:	62 ce f9                	vkor   %k7,%k1
[ 	]*[a-f0-9]+:	62 de c8                	vkortest %k1,%k0
[ 	]*[a-f0-9]+:	62 de f9                	vkortest %k7,%k1
[ 	]*[a-f0-9]+:	62 ef c8                	vkswapb %k1,%k0
[ 	]*[a-f0-9]+:	62 ef f9                	vkswapb %k7,%k1
[ 	]*[a-f0-9]+:	62 fe c8                	vkxnor %k1,%k0
[ 	]*[a-f0-9]+:	62 fe f9                	vkxnor %k7,%k1
[ 	]*[a-f0-9]+:	62 ee c8                	vkxor  %k1,%k0
[ 	]*[a-f0-9]+:	62 ee f9                	vkxor  %k7,%k1
[ 	]*[a-f0-9]+:	66 62 f5 d8             	bitinterleave21 %bx,%ax
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 %eax,%ebx
[ 	]*[a-f0-9]+:	62 a5 c5                	bitinterleave21 %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f2 d8             	bsff   %bx,%ax
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a2 c5                	bsff   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f0 d8             	bsfi   %bx,%ax
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a0 c5                	bsfi   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f3 d8             	bsrf   %bx,%ax
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a3 c5                	bsrf   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f1 d8             	bsri   %bx,%ax
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a1 c5                	bsri   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f8 d8             	countbits %bx,%ax
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits %eax,%ebx
[ 	]*[a-f0-9]+:	62 a8 c5                	countbits %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f9 d8             	quadmask16 %bx,%ax
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 %eax,%ebx
[ 	]*[a-f0-9]+:	62 a9 c5                	quadmask16 %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f5 c3             	bitinterleave21 %ax,%bx
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 %eax,%ebx
[ 	]*[a-f0-9]+:	62 a5 c5                	bitinterleave21 %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f2 c3             	bsff   %ax,%bx
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a2 c5                	bsff   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f0 c3             	bsfi   %ax,%bx
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a0 c5                	bsfi   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f3 c3             	bsrf   %ax,%bx
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a3 c5                	bsrf   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f1 c3             	bsri   %ax,%bx
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   %eax,%ebx
[ 	]*[a-f0-9]+:	62 a1 c5                	bsri   %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f8 c3             	countbits %ax,%bx
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits %eax,%ebx
[ 	]*[a-f0-9]+:	62 a8 c5                	countbits %rax,%r13
[ 	]*[a-f0-9]+:	66 62 f9 c3             	quadmask16 %ax,%bx
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 %eax,%ebx
[ 	]*[a-f0-9]+:	62 a9 c5                	quadmask16 %rax,%r13
[ 	]*[a-f0-9]+:	d6 00 08 40 c2          	vaddps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 40 c2          	vaddps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 40 c2          	vaddps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 40 c2          	vaddps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 40 c2          	vaddps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 40 c2          	vaddps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 40 c2          	vaddps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 40 c2          	vaddps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 40 c2          	vaddps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 40 c2          	vaddps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 40 c7          	vaddps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 40 c0          	vaddps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 40 c1          	vaddps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 40 c7          	vaddps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 40 c0          	vaddps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 40 c1          	vaddps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 40 c7          	vaddps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 40 c7          	vaddps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 40 cf          	vaddps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 40 ff          	vaddps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 40 ff          	vaddps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 40 ff          	vaddps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 40 ff          	vaddps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 40 ff          	vaddps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 40 00          	vaddps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 40 00          	vaddps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 40 00          	vaddps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 40 00          	vaddps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 40 00          	vaddps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 40 00          	vaddps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 40 00          	vaddps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 40 00          	vaddps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 40 00          	vaddps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 40 00          	vaddps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 40 00          	vaddps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 40 84 08 04 00 00 00 	vaddps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 40 44 08 01    	vaddps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 40 44 08 ff    	vaddps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
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
[ 	]*[a-f0-9]+:	d6 00 08 86 c2          	vclampzps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 86 c2          	vclampzps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 86 c2          	vclampzps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 86 c2          	vclampzps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 86 c2          	vclampzps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 86 c2          	vclampzps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 86 c2          	vclampzps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 86 c2          	vclampzps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 86 c2          	vclampzps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 86 c2          	vclampzps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 86 c7          	vclampzps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 86 c0          	vclampzps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 86 c1          	vclampzps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 86 c7          	vclampzps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 86 c0          	vclampzps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 86 c1          	vclampzps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 86 c7          	vclampzps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 86 c7          	vclampzps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 86 cf          	vclampzps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 86 ff          	vclampzps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 86 ff          	vclampzps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 86 ff          	vclampzps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 86 ff          	vclampzps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 86 ff          	vclampzps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 86 00          	vclampzps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 86 00          	vclampzps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 86 00          	vclampzps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 86 00          	vclampzps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 86 00          	vclampzps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 86 00          	vclampzps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 86 00          	vclampzps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 86 00          	vclampzps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 86 00          	vclampzps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 86 00          	vclampzps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 86 00          	vclampzps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 86 84 08 04 00 00 00 	vclampzps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 86 44 08 01    	vclampzps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 86 44 08 ff    	vclampzps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 64 c2          	vmadd132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 64 c2          	vmadd132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 64 c2          	vmadd132ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 64 c2          	vmadd132ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 64 c2          	vmadd132ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 64 c2          	vmadd132ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 64 c2          	vmadd132ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 64 c2          	vmadd132ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 64 c2          	vmadd132ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 64 c2          	vmadd132ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 64 c7          	vmadd132ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 64 c0          	vmadd132ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 64 c1          	vmadd132ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 64 c7          	vmadd132ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 64 c0          	vmadd132ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 64 c1          	vmadd132ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 64 c7          	vmadd132ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 64 c7          	vmadd132ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 64 cf          	vmadd132ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 64 ff          	vmadd132ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 64 ff          	vmadd132ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 64 ff          	vmadd132ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 64 ff          	vmadd132ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 64 ff          	vmadd132ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 64 00          	vmadd132ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 64 00          	vmadd132ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 64 00          	vmadd132ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 64 00          	vmadd132ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 64 00          	vmadd132ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 64 00          	vmadd132ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 64 00          	vmadd132ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 64 00          	vmadd132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 64 00          	vmadd132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 64 00          	vmadd132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 64 00          	vmadd132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 64 84 08 04 00 00 00 	vmadd132ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 64 44 08 01    	vmadd132ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 64 44 08 ff    	vmadd132ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 62 c2          	vmadd213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 62 c2          	vmadd213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 62 c2          	vmadd213ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 62 c2          	vmadd213ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 62 c2          	vmadd213ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 62 c2          	vmadd213ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 62 c2          	vmadd213ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 62 c2          	vmadd213ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 62 c2          	vmadd213ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 62 c2          	vmadd213ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 62 c7          	vmadd213ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 62 c0          	vmadd213ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 62 c1          	vmadd213ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 62 c7          	vmadd213ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 62 c0          	vmadd213ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 62 c1          	vmadd213ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 62 c7          	vmadd213ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 62 c7          	vmadd213ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 62 cf          	vmadd213ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 62 ff          	vmadd213ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 62 ff          	vmadd213ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 62 ff          	vmadd213ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 62 ff          	vmadd213ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 62 ff          	vmadd213ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 62 00          	vmadd213ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 62 00          	vmadd213ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 62 00          	vmadd213ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 62 00          	vmadd213ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 62 00          	vmadd213ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 62 00          	vmadd213ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 62 00          	vmadd213ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 62 00          	vmadd213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 62 00          	vmadd213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 62 00          	vmadd213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 62 00          	vmadd213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 62 84 08 04 00 00 00 	vmadd213ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 62 44 08 01    	vmadd213ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 62 44 08 ff    	vmadd213ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 60 c2          	vmadd231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 60 c2          	vmadd231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 60 c2          	vmadd231ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 60 c2          	vmadd231ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 60 c2          	vmadd231ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 60 c2          	vmadd231ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 60 c2          	vmadd231ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 60 c2          	vmadd231ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 60 c2          	vmadd231ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 60 c2          	vmadd231ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 60 c7          	vmadd231ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 60 c0          	vmadd231ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 60 c1          	vmadd231ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 60 c7          	vmadd231ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 60 c0          	vmadd231ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 60 c1          	vmadd231ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 60 c7          	vmadd231ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 60 c7          	vmadd231ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 60 cf          	vmadd231ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 60 ff          	vmadd231ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 60 ff          	vmadd231ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 60 ff          	vmadd231ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 60 ff          	vmadd231ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 60 ff          	vmadd231ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 60 00          	vmadd231ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 60 00          	vmadd231ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 60 00          	vmadd231ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 60 00          	vmadd231ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 60 00          	vmadd231ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 60 00          	vmadd231ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 60 00          	vmadd231ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 60 00          	vmadd231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 60 00          	vmadd231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 60 00          	vmadd231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 60 00          	vmadd231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 60 84 08 04 00 00 00 	vmadd231ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 60 44 08 01    	vmadd231ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 60 44 08 ff    	vmadd231ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 46 c2          	vaddsetsps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 46 c2          	vaddsetsps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 46 c2          	vaddsetsps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 46 c2          	vaddsetsps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 46 c2          	vaddsetsps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 46 c2          	vaddsetsps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 46 c2          	vaddsetsps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 46 c2          	vaddsetsps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 46 c7          	vaddsetsps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 46 c0          	vaddsetsps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 46 c1          	vaddsetsps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 46 c7          	vaddsetsps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 46 c0          	vaddsetsps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 46 c1          	vaddsetsps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 46 c7          	vaddsetsps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 46 c7          	vaddsetsps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 46 cf          	vaddsetsps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 46 ff          	vaddsetsps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 46 ff          	vaddsetsps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 46 ff          	vaddsetsps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 46 ff          	vaddsetsps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 46 ff          	vaddsetsps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 46 00          	vaddsetsps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 46 00          	vaddsetsps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 46 00          	vaddsetsps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 46 00          	vaddsetsps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 46 00          	vaddsetsps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 46 00          	vaddsetsps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 46 00          	vaddsetsps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 46 00          	vaddsetsps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 46 00          	vaddsetsps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 46 00          	vaddsetsps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 46 00          	vaddsetsps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 46 84 08 04 00 00 00 	vaddsetsps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 46 44 08 01    	vaddsetsps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 46 44 08 ff    	vaddsetsps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 66 c2          	vmadd233ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 66 c2          	vmadd233ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 66 c2          	vmadd233ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 66 c2          	vmadd233ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 66 c2          	vmadd233ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 66 c2          	vmadd233ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 66 c2          	vmadd233ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 66 c2          	vmadd233ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 66 c2          	vmadd233ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 66 c2          	vmadd233ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 66 c7          	vmadd233ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 66 c0          	vmadd233ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 66 c1          	vmadd233ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 66 c7          	vmadd233ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 66 c0          	vmadd233ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 66 c1          	vmadd233ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 66 c7          	vmadd233ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 66 c7          	vmadd233ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 66 cf          	vmadd233ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 66 ff          	vmadd233ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 66 ff          	vmadd233ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 66 ff          	vmadd233ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 66 ff          	vmadd233ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 66 ff          	vmadd233ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 66 00          	vmadd233ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 66 00          	vmadd233ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 66 00          	vmadd233ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 66 00          	vmadd233ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 66 00          	vmadd233ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 66 00          	vmadd233ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 66 00          	vmadd233ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 66 00          	vmadd233ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 66 00          	vmadd233ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 66 00          	vmadd233ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 66 00          	vmadd233ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 66 84 08 04 00 00 00 	vmadd233ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 66 44 08 01    	vmadd233ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 66 44 08 ff    	vmadd233ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 74 c2          	vmaddn132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 74 c2          	vmaddn132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 74 c2          	vmaddn132ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 74 c2          	vmaddn132ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 74 c2          	vmaddn132ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 74 c2          	vmaddn132ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 74 c2          	vmaddn132ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 74 c2          	vmaddn132ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 74 c2          	vmaddn132ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 74 c2          	vmaddn132ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 74 c7          	vmaddn132ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 74 c0          	vmaddn132ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 74 c1          	vmaddn132ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 74 c7          	vmaddn132ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 74 c0          	vmaddn132ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 74 c1          	vmaddn132ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 74 c7          	vmaddn132ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 74 c7          	vmaddn132ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 74 cf          	vmaddn132ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 74 ff          	vmaddn132ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 74 ff          	vmaddn132ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 74 ff          	vmaddn132ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 74 ff          	vmaddn132ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 74 ff          	vmaddn132ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 74 00          	vmaddn132ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 74 00          	vmaddn132ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 74 00          	vmaddn132ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 74 00          	vmaddn132ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 74 00          	vmaddn132ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 74 00          	vmaddn132ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 74 00          	vmaddn132ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 74 00          	vmaddn132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 74 00          	vmaddn132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 74 00          	vmaddn132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 74 00          	vmaddn132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 74 84 08 04 00 00 00 	vmaddn132ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 74 44 08 01    	vmaddn132ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 74 44 08 ff    	vmaddn132ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 72 c2          	vmaddn213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 72 c2          	vmaddn213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 72 c2          	vmaddn213ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 72 c2          	vmaddn213ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 72 c2          	vmaddn213ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 72 c2          	vmaddn213ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 72 c2          	vmaddn213ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 72 c2          	vmaddn213ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 72 c2          	vmaddn213ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 72 c2          	vmaddn213ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 72 c7          	vmaddn213ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 72 c0          	vmaddn213ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 72 c1          	vmaddn213ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 72 c7          	vmaddn213ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 72 c0          	vmaddn213ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 72 c1          	vmaddn213ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 72 c7          	vmaddn213ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 72 c7          	vmaddn213ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 72 cf          	vmaddn213ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 72 ff          	vmaddn213ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 72 ff          	vmaddn213ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 72 ff          	vmaddn213ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 72 ff          	vmaddn213ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 72 ff          	vmaddn213ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 72 00          	vmaddn213ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 72 00          	vmaddn213ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 72 00          	vmaddn213ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 72 00          	vmaddn213ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 72 00          	vmaddn213ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 72 00          	vmaddn213ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 72 00          	vmaddn213ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 72 00          	vmaddn213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 72 00          	vmaddn213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 72 00          	vmaddn213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 72 00          	vmaddn213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 72 84 08 04 00 00 00 	vmaddn213ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 72 44 08 01    	vmaddn213ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 72 44 08 ff    	vmaddn213ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 70 c2          	vmaddn231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 70 c2          	vmaddn231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 70 c2          	vmaddn231ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 70 c2          	vmaddn231ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 70 c2          	vmaddn231ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 70 c2          	vmaddn231ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 70 c2          	vmaddn231ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 70 c2          	vmaddn231ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 70 c2          	vmaddn231ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 70 c2          	vmaddn231ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 70 c7          	vmaddn231ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 70 c0          	vmaddn231ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 70 c1          	vmaddn231ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 70 c7          	vmaddn231ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 70 c0          	vmaddn231ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 70 c1          	vmaddn231ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 70 c7          	vmaddn231ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 70 c7          	vmaddn231ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 70 cf          	vmaddn231ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 70 ff          	vmaddn231ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 70 ff          	vmaddn231ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 70 ff          	vmaddn231ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 70 ff          	vmaddn231ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 70 ff          	vmaddn231ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 70 00          	vmaddn231ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 70 00          	vmaddn231ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 70 00          	vmaddn231ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 70 00          	vmaddn231ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 70 00          	vmaddn231ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 70 00          	vmaddn231ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 70 00          	vmaddn231ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 70 00          	vmaddn231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 70 00          	vmaddn231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 70 00          	vmaddn231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 70 00          	vmaddn231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 70 84 08 04 00 00 00 	vmaddn231ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 70 44 08 01    	vmaddn231ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 70 44 08 ff    	vmaddn231ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 88 c2          	vmaxps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 88 c2          	vmaxps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 88 c2          	vmaxps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 88 c2          	vmaxps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 88 c2          	vmaxps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 88 c2          	vmaxps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 88 c2          	vmaxps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 88 c2          	vmaxps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 88 c2          	vmaxps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 88 c2          	vmaxps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 88 c7          	vmaxps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 88 c0          	vmaxps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 88 c1          	vmaxps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 88 c7          	vmaxps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 88 c0          	vmaxps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 88 c1          	vmaxps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 88 c7          	vmaxps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 88 c7          	vmaxps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 88 cf          	vmaxps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 88 ff          	vmaxps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 88 ff          	vmaxps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 88 ff          	vmaxps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 88 ff          	vmaxps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 88 ff          	vmaxps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 88 00          	vmaxps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 88 00          	vmaxps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 88 00          	vmaxps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 88 00          	vmaxps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 88 00          	vmaxps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 88 00          	vmaxps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 88 00          	vmaxps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 88 00          	vmaxps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 88 00          	vmaxps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 88 00          	vmaxps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 88 00          	vmaxps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 88 84 08 04 00 00 00 	vmaxps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 88 44 08 01    	vmaxps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 88 44 08 ff    	vmaxps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 8c c2          	vminps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 8c c2          	vminps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 8c c2          	vminps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8c c2          	vminps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8c c2          	vminps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 8c c2          	vminps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8c c2          	vminps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8c c2          	vminps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8c c2          	vminps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8c c2          	vminps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8c c7          	vminps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8c c0          	vminps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8c c1          	vminps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8c c7          	vminps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8c c0          	vminps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8c c1          	vminps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 8c c7          	vminps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8c c7          	vminps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8c cf          	vminps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8c ff          	vminps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 8c ff          	vminps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 8c ff          	vminps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 8c ff          	vminps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 8c ff          	vminps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 8c 00          	vminps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8c 00          	vminps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8c 00          	vminps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8c 00          	vminps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8c 00          	vminps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8c 00          	vminps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8c 00          	vminps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8c 00          	vminps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8c 00          	vminps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8c 00          	vminps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8c 00          	vminps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8c 84 08 04 00 00 00 	vminps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8c 44 08 01    	vminps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8c 44 08 ff    	vminps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 6c c2          	vmsub132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 6c c2          	vmsub132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 6c c2          	vmsub132ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 6c c2          	vmsub132ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 6c c2          	vmsub132ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 6c c2          	vmsub132ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 6c c2          	vmsub132ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 6c c2          	vmsub132ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 6c c2          	vmsub132ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 6c c2          	vmsub132ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 6c c7          	vmsub132ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 6c c0          	vmsub132ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 6c c1          	vmsub132ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 6c c7          	vmsub132ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 6c c0          	vmsub132ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 6c c1          	vmsub132ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 6c c7          	vmsub132ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 6c c7          	vmsub132ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 6c cf          	vmsub132ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 6c ff          	vmsub132ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 6c ff          	vmsub132ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 6c ff          	vmsub132ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 6c ff          	vmsub132ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 6c ff          	vmsub132ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 6c 00          	vmsub132ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 6c 00          	vmsub132ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 6c 00          	vmsub132ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 6c 00          	vmsub132ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 6c 00          	vmsub132ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 6c 00          	vmsub132ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 6c 00          	vmsub132ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 6c 00          	vmsub132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 6c 00          	vmsub132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6c 00          	vmsub132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6c 00          	vmsub132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 6c 84 08 04 00 00 00 	vmsub132ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 6c 44 08 01    	vmsub132ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 6c 44 08 ff    	vmsub132ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 6a c2          	vmsub213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 6a c2          	vmsub213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 6a c2          	vmsub213ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 6a c2          	vmsub213ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 6a c2          	vmsub213ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 6a c2          	vmsub213ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 6a c2          	vmsub213ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 6a c2          	vmsub213ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 6a c2          	vmsub213ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 6a c2          	vmsub213ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 6a c7          	vmsub213ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 6a c0          	vmsub213ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 6a c1          	vmsub213ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 6a c7          	vmsub213ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 6a c0          	vmsub213ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 6a c1          	vmsub213ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 6a c7          	vmsub213ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 6a c7          	vmsub213ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 6a cf          	vmsub213ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 6a ff          	vmsub213ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 6a ff          	vmsub213ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 6a ff          	vmsub213ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 6a ff          	vmsub213ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 6a ff          	vmsub213ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 6a 00          	vmsub213ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 6a 00          	vmsub213ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 6a 00          	vmsub213ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 6a 00          	vmsub213ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 6a 00          	vmsub213ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 6a 00          	vmsub213ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 6a 00          	vmsub213ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 6a 00          	vmsub213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 6a 00          	vmsub213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6a 00          	vmsub213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6a 00          	vmsub213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 6a 84 08 04 00 00 00 	vmsub213ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 6a 44 08 01    	vmsub213ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 6a 44 08 ff    	vmsub213ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 68 c2          	vmsub231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 68 c2          	vmsub231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 68 c2          	vmsub231ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 68 c2          	vmsub231ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 68 c2          	vmsub231ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 68 c2          	vmsub231ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 68 c2          	vmsub231ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 68 c2          	vmsub231ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 68 c2          	vmsub231ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 68 c2          	vmsub231ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 68 c7          	vmsub231ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 68 c0          	vmsub231ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 68 c1          	vmsub231ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 68 c7          	vmsub231ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 68 c0          	vmsub231ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 68 c1          	vmsub231ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 68 c7          	vmsub231ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 68 c7          	vmsub231ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 68 cf          	vmsub231ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 68 ff          	vmsub231ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 68 ff          	vmsub231ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 68 ff          	vmsub231ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 68 ff          	vmsub231ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 68 ff          	vmsub231ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 68 00          	vmsub231ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 68 00          	vmsub231ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 68 00          	vmsub231ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 68 00          	vmsub231ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 68 00          	vmsub231ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 68 00          	vmsub231ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 68 00          	vmsub231ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 68 00          	vmsub231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 68 00          	vmsub231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 68 00          	vmsub231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 68 00          	vmsub231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 68 84 08 04 00 00 00 	vmsub231ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 68 44 08 01    	vmsub231ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 68 44 08 ff    	vmsub231ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 7c c2          	vmsubr132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 7c c2          	vmsubr132ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 7c c2          	vmsubr132ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 7c c2          	vmsubr132ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 7c c2          	vmsubr132ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 7c c2          	vmsubr132ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 7c c2          	vmsubr132ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 7c c2          	vmsubr132ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 7c c2          	vmsubr132ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7c c2          	vmsubr132ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7c c7          	vmsubr132ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7c c0          	vmsubr132ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7c c1          	vmsubr132ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7c c7          	vmsubr132ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 7c c0          	vmsubr132ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 7c c1          	vmsubr132ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 7c c7          	vmsubr132ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7c c7          	vmsubr132ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7c cf          	vmsubr132ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7c ff          	vmsubr132ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 7c ff          	vmsubr132ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 7c ff          	vmsubr132ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 7c ff          	vmsubr132ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 7c ff          	vmsubr132ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 7c 00          	vmsubr132ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 7c 00          	vmsubr132ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 7c 00          	vmsubr132ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 7c 00          	vmsubr132ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7c 00          	vmsubr132ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 7c 00          	vmsubr132ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 7c 00          	vmsubr132ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 7c 00          	vmsubr132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 7c 00          	vmsubr132ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7c 00          	vmsubr132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7c 00          	vmsubr132ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7c 84 08 04 00 00 00 	vmsubr132ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7c 44 08 01    	vmsubr132ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7c 44 08 ff    	vmsubr132ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 7a c2          	vmsubr213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 7a c2          	vmsubr213ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 7a c2          	vmsubr213ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 7a c2          	vmsubr213ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 7a c2          	vmsubr213ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 7a c2          	vmsubr213ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 7a c2          	vmsubr213ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 7a c2          	vmsubr213ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 7a c2          	vmsubr213ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7a c2          	vmsubr213ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7a c7          	vmsubr213ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7a c0          	vmsubr213ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7a c1          	vmsubr213ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7a c7          	vmsubr213ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 7a c0          	vmsubr213ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 7a c1          	vmsubr213ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 7a c7          	vmsubr213ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7a c7          	vmsubr213ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7a cf          	vmsubr213ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7a ff          	vmsubr213ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 7a ff          	vmsubr213ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 7a ff          	vmsubr213ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 7a ff          	vmsubr213ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 7a ff          	vmsubr213ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 7a 00          	vmsubr213ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 7a 00          	vmsubr213ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 7a 00          	vmsubr213ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 7a 00          	vmsubr213ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7a 00          	vmsubr213ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 7a 00          	vmsubr213ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 7a 00          	vmsubr213ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 7a 00          	vmsubr213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 7a 00          	vmsubr213ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7a 00          	vmsubr213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7a 00          	vmsubr213ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7a 84 08 04 00 00 00 	vmsubr213ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7a 44 08 01    	vmsubr213ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7a 44 08 ff    	vmsubr213ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 78 c2          	vmsubr231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 78 c2          	vmsubr231ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 78 c2          	vmsubr231ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 78 c2          	vmsubr231ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 78 c2          	vmsubr231ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 78 c2          	vmsubr231ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 78 c2          	vmsubr231ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 78 c2          	vmsubr231ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 78 c2          	vmsubr231ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 78 c2          	vmsubr231ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 78 c7          	vmsubr231ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 78 c0          	vmsubr231ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 78 c1          	vmsubr231ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 78 c7          	vmsubr231ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 78 c0          	vmsubr231ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 78 c1          	vmsubr231ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 78 c7          	vmsubr231ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 78 c7          	vmsubr231ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 78 cf          	vmsubr231ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 78 ff          	vmsubr231ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 78 ff          	vmsubr231ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 78 ff          	vmsubr231ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 78 ff          	vmsubr231ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 78 ff          	vmsubr231ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 78 00          	vmsubr231ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 78 00          	vmsubr231ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 78 00          	vmsubr231ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 78 00          	vmsubr231ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 78 00          	vmsubr231ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 78 00          	vmsubr231ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 78 00          	vmsubr231ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 78 00          	vmsubr231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 78 00          	vmsubr231ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 78 00          	vmsubr231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 78 00          	vmsubr231ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 78 84 08 04 00 00 00 	vmsubr231ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 78 44 08 01    	vmsubr231ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 78 44 08 ff    	vmsubr231ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 7e c2          	vmsubr23c1ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 7e c2          	vmsubr23c1ps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 7e c2          	vmsubr23c1ps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 7e c2          	vmsubr23c1ps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 7e c2          	vmsubr23c1ps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 7e c2          	vmsubr23c1ps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 7e c2          	vmsubr23c1ps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 7e c2          	vmsubr23c1ps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 7e c2          	vmsubr23c1ps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7e c2          	vmsubr23c1ps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7e c7          	vmsubr23c1ps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7e c0          	vmsubr23c1ps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7e c1          	vmsubr23c1ps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 7e c7          	vmsubr23c1ps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 7e c0          	vmsubr23c1ps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 7e c1          	vmsubr23c1ps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 7e c7          	vmsubr23c1ps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7e c7          	vmsubr23c1ps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7e cf          	vmsubr23c1ps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 7e ff          	vmsubr23c1ps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 7e ff          	vmsubr23c1ps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 7e ff          	vmsubr23c1ps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 7e ff          	vmsubr23c1ps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 7e ff          	vmsubr23c1ps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 7e 00          	vmsubr23c1ps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 7e 00          	vmsubr23c1ps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 7e 00          	vmsubr23c1ps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 7e 00          	vmsubr23c1ps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 7e 00          	vmsubr23c1ps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 7e 00          	vmsubr23c1ps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 7e 00          	vmsubr23c1ps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 7e 00          	vmsubr23c1ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 7e 00          	vmsubr23c1ps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7e 00          	vmsubr23c1ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7e 00          	vmsubr23c1ps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7e 84 08 04 00 00 00 	vmsubr23c1ps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7e 44 08 01    	vmsubr23c1ps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 7e 44 08 ff    	vmsubr23c1ps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 80 c2          	vmulps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 80 c2          	vmulps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 80 c2          	vmulps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 80 c2          	vmulps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 80 c2          	vmulps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 80 c2          	vmulps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 80 c2          	vmulps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 80 c2          	vmulps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 80 c2          	vmulps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 80 c2          	vmulps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 80 c7          	vmulps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 80 c0          	vmulps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 80 c1          	vmulps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 80 c7          	vmulps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 80 c0          	vmulps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 80 c1          	vmulps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 80 c7          	vmulps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 80 c7          	vmulps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 80 cf          	vmulps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 80 ff          	vmulps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 80 ff          	vmulps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 80 ff          	vmulps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 80 ff          	vmulps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 80 ff          	vmulps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 80 00          	vmulps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 80 00          	vmulps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 80 00          	vmulps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 80 00          	vmulps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 80 00          	vmulps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 80 00          	vmulps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 80 00          	vmulps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 80 00          	vmulps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 80 00          	vmulps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 80 00          	vmulps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 80 00          	vmulps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 80 84 08 04 00 00 00 	vmulps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 80 44 08 01    	vmulps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 80 44 08 ff    	vmulps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 c2 c2          	vrcprefineps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 c2 c2          	vrcprefineps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f c2 c2          	vrcprefineps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f c2 c2          	vrcprefineps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f c2 c2          	vrcprefineps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f c2 c2          	vrcprefineps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f c2 c2          	vrcprefineps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f c2 c2          	vrcprefineps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f c2 c2          	vrcprefineps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f c2 c2          	vrcprefineps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f c2 c7          	vrcprefineps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f c2 c0          	vrcprefineps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f c2 c1          	vrcprefineps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f c2 c7          	vrcprefineps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f c2 c0          	vrcprefineps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f c2 c1          	vrcprefineps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f c2 c7          	vrcprefineps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f c2 c7          	vrcprefineps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f c2 cf          	vrcprefineps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f c2 ff          	vrcprefineps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 c2 ff          	vrcprefineps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f c2 ff          	vrcprefineps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f c2 ff          	vrcprefineps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff c2 ff          	vrcprefineps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f c2 00          	vrcprefineps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f c2 00          	vrcprefineps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f c2 00          	vrcprefineps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f c2 00          	vrcprefineps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f c2 00          	vrcprefineps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f c2 00          	vrcprefineps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f c2 00          	vrcprefineps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f c2 00          	vrcprefineps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f c2 00          	vrcprefineps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f c2 00          	vrcprefineps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f c2 00          	vrcprefineps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f c2 84 08 04 00 00 00 	vrcprefineps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f c2 44 08 01    	vrcprefineps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f c2 44 08 ff    	vrcprefineps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 50 c2          	vsubps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 50 c2          	vsubps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 50 c2          	vsubps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 50 c2          	vsubps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 50 c2          	vsubps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 50 c2          	vsubps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 50 c2          	vsubps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 50 c2          	vsubps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 50 c2          	vsubps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 50 c2          	vsubps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 50 c7          	vsubps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 50 c0          	vsubps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 50 c1          	vsubps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 50 c7          	vsubps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 50 c0          	vsubps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 50 c1          	vsubps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 50 c7          	vsubps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 50 c7          	vsubps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 50 cf          	vsubps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 50 ff          	vsubps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 50 ff          	vsubps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 50 ff          	vsubps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 50 ff          	vsubps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 50 ff          	vsubps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 50 00          	vsubps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 50 00          	vsubps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 50 00          	vsubps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 50 00          	vsubps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 50 00          	vsubps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 50 00          	vsubps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 50 00          	vsubps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 50 00          	vsubps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 50 00          	vsubps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 50 00          	vsubps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 50 00          	vsubps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 50 84 08 04 00 00 00 	vsubps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 50 44 08 01    	vsubps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 50 44 08 ff    	vsubps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 58 c2          	vsubrps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 58 c2          	vsubrps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 58 c2          	vsubrps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 58 c2          	vsubrps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 58 c2          	vsubrps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 58 c2          	vsubrps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 58 c2          	vsubrps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 58 c2          	vsubrps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 58 c2          	vsubrps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 58 c2          	vsubrps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 58 c7          	vsubrps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 58 c0          	vsubrps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 58 c1          	vsubrps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 58 c7          	vsubrps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 58 c0          	vsubrps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 58 c1          	vsubrps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 58 c7          	vsubrps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 58 c7          	vsubrps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 58 cf          	vsubrps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 58 ff          	vsubrps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 58 ff          	vsubrps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 58 ff          	vsubrps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 58 ff          	vsubrps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 58 ff          	vsubrps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 58 00          	vsubrps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 58 00          	vsubrps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 58 00          	vsubrps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 58 00          	vsubrps \(%rax\)\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 58 00          	vsubrps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 58 00          	vsubrps \(%rax\)\{float16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 58 00          	vsubrps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 58 00          	vsubrps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 58 00          	vsubrps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 58 00          	vsubrps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 58 00          	vsubrps \(%rax\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 58 84 08 04 00 00 00 	vsubrps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 58 44 08 01    	vsubrps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 58 44 08 ff    	vsubrps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a5 c2          	vandpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a5 c2          	vandpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a5 c2          	vandpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a5 c2          	vandpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a5 c2          	vandpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a5 c2          	vandpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a5 c2          	vandpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a5 c2          	vandpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a5 c2          	vandpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a5 c2          	vandpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a5 c7          	vandpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a5 c0          	vandpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a5 c1          	vandpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a5 c7          	vandpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a5 c0          	vandpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a5 c1          	vandpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a5 c7          	vandpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a5 c7          	vandpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a5 cf          	vandpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a5 ff          	vandpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a5 ff          	vandpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a5 ff          	vandpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a5 ff          	vandpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a5 ff          	vandpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a5 00          	vandpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a5 00          	vandpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a5 00          	vandpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a5 00          	vandpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a5 00          	vandpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a5 00          	vandpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a5 00          	vandpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a5 00          	vandpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a5 00          	vandpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a5 00          	vandpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a5 00          	vandpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a5 84 08 04 00 00 00 	vandpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a5 44 08 01    	vandpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a5 44 08 ff    	vandpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a4 c2          	vandnpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a4 c2          	vandnpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a4 c2          	vandnpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a4 c2          	vandnpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a4 c2          	vandnpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a4 c2          	vandnpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a4 c2          	vandnpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a4 c2          	vandnpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a4 c2          	vandnpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a4 c2          	vandnpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a4 c7          	vandnpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a4 c0          	vandnpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a4 c1          	vandnpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a4 c7          	vandnpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a4 c0          	vandnpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a4 c1          	vandnpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a4 c7          	vandnpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a4 c7          	vandnpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a4 cf          	vandnpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a4 ff          	vandnpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a4 ff          	vandnpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a4 ff          	vandnpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a4 ff          	vandnpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a4 ff          	vandnpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a4 00          	vandnpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a4 00          	vandnpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a4 00          	vandnpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a4 00          	vandnpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a4 00          	vandnpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a4 00          	vandnpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a4 00          	vandnpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a4 00          	vandnpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a4 00          	vandnpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a4 00          	vandnpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a4 00          	vandnpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a4 84 08 04 00 00 00 	vandnpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a4 44 08 01    	vandnpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a4 44 08 ff    	vandnpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a8 c2          	vbitinterleave11pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a8 c2          	vbitinterleave11pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a8 c2          	vbitinterleave11pi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a8 c2          	vbitinterleave11pi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a8 c2          	vbitinterleave11pi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a8 c2          	vbitinterleave11pi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a8 c2          	vbitinterleave11pi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a8 c2          	vbitinterleave11pi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a8 c2          	vbitinterleave11pi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a8 c2          	vbitinterleave11pi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a8 c7          	vbitinterleave11pi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a8 c0          	vbitinterleave11pi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a8 c1          	vbitinterleave11pi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a8 c7          	vbitinterleave11pi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a8 c0          	vbitinterleave11pi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a8 c1          	vbitinterleave11pi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a8 c7          	vbitinterleave11pi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a8 c7          	vbitinterleave11pi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a8 cf          	vbitinterleave11pi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a8 ff          	vbitinterleave11pi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a8 ff          	vbitinterleave11pi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a8 ff          	vbitinterleave11pi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a8 ff          	vbitinterleave11pi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a8 ff          	vbitinterleave11pi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a8 00          	vbitinterleave11pi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a8 00          	vbitinterleave11pi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a8 00          	vbitinterleave11pi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a8 00          	vbitinterleave11pi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a8 00          	vbitinterleave11pi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a8 00          	vbitinterleave11pi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a8 00          	vbitinterleave11pi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a8 00          	vbitinterleave11pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a8 00          	vbitinterleave11pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a8 00          	vbitinterleave11pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a8 00          	vbitinterleave11pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a8 84 08 04 00 00 00 	vbitinterleave11pi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a8 44 08 01    	vbitinterleave11pi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a8 44 08 ff    	vbitinterleave11pi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a9 c2          	vbitinterleave21pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a9 c2          	vbitinterleave21pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a9 c2          	vbitinterleave21pi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a9 c2          	vbitinterleave21pi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a9 c2          	vbitinterleave21pi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a9 c2          	vbitinterleave21pi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a9 c2          	vbitinterleave21pi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a9 c2          	vbitinterleave21pi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a9 c2          	vbitinterleave21pi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a9 c2          	vbitinterleave21pi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a9 c7          	vbitinterleave21pi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a9 c0          	vbitinterleave21pi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a9 c1          	vbitinterleave21pi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a9 c7          	vbitinterleave21pi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a9 c0          	vbitinterleave21pi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a9 c1          	vbitinterleave21pi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a9 c7          	vbitinterleave21pi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a9 c7          	vbitinterleave21pi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a9 cf          	vbitinterleave21pi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a9 ff          	vbitinterleave21pi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a9 ff          	vbitinterleave21pi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a9 ff          	vbitinterleave21pi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a9 ff          	vbitinterleave21pi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a9 ff          	vbitinterleave21pi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a9 00          	vbitinterleave21pi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a9 00          	vbitinterleave21pi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a9 00          	vbitinterleave21pi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a9 00          	vbitinterleave21pi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a9 00          	vbitinterleave21pi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a9 00          	vbitinterleave21pi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a9 00          	vbitinterleave21pi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a9 00          	vbitinterleave21pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a9 00          	vbitinterleave21pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a9 00          	vbitinterleave21pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a9 00          	vbitinterleave21pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a9 84 08 04 00 00 00 	vbitinterleave21pi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a9 44 08 01    	vbitinterleave21pi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a9 44 08 ff    	vbitinterleave21pi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 87 c2          	vclampzpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 87 c2          	vclampzpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 87 c2          	vclampzpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 87 c2          	vclampzpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 87 c2          	vclampzpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 87 c2          	vclampzpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 87 c2          	vclampzpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 87 c2          	vclampzpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 87 c2          	vclampzpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 87 c2          	vclampzpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 87 c7          	vclampzpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 87 c0          	vclampzpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 87 c1          	vclampzpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 87 c7          	vclampzpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 87 c0          	vclampzpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 87 c1          	vclampzpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 87 c7          	vclampzpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 87 c7          	vclampzpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 87 cf          	vclampzpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 87 ff          	vclampzpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 87 ff          	vclampzpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 87 ff          	vclampzpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 87 ff          	vclampzpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 87 ff          	vclampzpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 87 00          	vclampzpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 87 00          	vclampzpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 87 00          	vclampzpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 87 00          	vclampzpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 87 00          	vclampzpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 87 00          	vclampzpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 87 00          	vclampzpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 87 00          	vclampzpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 87 00          	vclampzpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 87 00          	vclampzpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 87 00          	vclampzpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 87 84 08 04 00 00 00 	vclampzpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 87 44 08 01    	vclampzpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 87 44 08 ff    	vclampzpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 61 c2          	vmadd231pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 61 c2          	vmadd231pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 61 c2          	vmadd231pi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 61 c2          	vmadd231pi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 61 c2          	vmadd231pi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 61 c2          	vmadd231pi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 61 c2          	vmadd231pi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 61 c2          	vmadd231pi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 61 c2          	vmadd231pi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 61 c2          	vmadd231pi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 61 c7          	vmadd231pi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 61 c0          	vmadd231pi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 61 c1          	vmadd231pi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 61 c7          	vmadd231pi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 61 c0          	vmadd231pi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 61 c1          	vmadd231pi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 61 c7          	vmadd231pi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 61 c7          	vmadd231pi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 61 cf          	vmadd231pi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 61 ff          	vmadd231pi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 61 ff          	vmadd231pi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 61 ff          	vmadd231pi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 61 ff          	vmadd231pi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 61 ff          	vmadd231pi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 61 00          	vmadd231pi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 61 00          	vmadd231pi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 61 00          	vmadd231pi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 61 00          	vmadd231pi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 61 00          	vmadd231pi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 61 00          	vmadd231pi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 61 00          	vmadd231pi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 61 00          	vmadd231pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 61 00          	vmadd231pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 61 00          	vmadd231pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 61 00          	vmadd231pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 61 84 08 04 00 00 00 	vmadd231pi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 61 44 08 01    	vmadd231pi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 61 44 08 ff    	vmadd231pi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 47 c2          	vaddsetspi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 47 c2          	vaddsetspi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 47 c2          	vaddsetspi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 47 c2          	vaddsetspi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 47 c2          	vaddsetspi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 47 c2          	vaddsetspi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 47 c2          	vaddsetspi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 47 c2          	vaddsetspi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 47 c7          	vaddsetspi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 47 c0          	vaddsetspi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 47 c1          	vaddsetspi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 47 c7          	vaddsetspi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 47 c0          	vaddsetspi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 47 c1          	vaddsetspi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 47 c7          	vaddsetspi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 47 c7          	vaddsetspi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 47 cf          	vaddsetspi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 47 ff          	vaddsetspi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 47 ff          	vaddsetspi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 47 ff          	vaddsetspi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 47 ff          	vaddsetspi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 47 ff          	vaddsetspi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 47 00          	vaddsetspi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 47 00          	vaddsetspi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 47 00          	vaddsetspi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 47 00          	vaddsetspi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 47 00          	vaddsetspi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 47 00          	vaddsetspi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 47 00          	vaddsetspi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 47 00          	vaddsetspi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 47 00          	vaddsetspi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 47 00          	vaddsetspi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 47 00          	vaddsetspi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 47 84 08 04 00 00 00 	vaddsetspi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 47 44 08 01    	vaddsetspi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 47 44 08 ff    	vaddsetspi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 67 c2          	vmadd233pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 67 c2          	vmadd233pi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 67 c2          	vmadd233pi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 67 c2          	vmadd233pi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 67 c2          	vmadd233pi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 67 c2          	vmadd233pi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 67 c2          	vmadd233pi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 67 c2          	vmadd233pi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 67 c2          	vmadd233pi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 67 c2          	vmadd233pi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 67 c7          	vmadd233pi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 67 c0          	vmadd233pi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 67 c1          	vmadd233pi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 67 c7          	vmadd233pi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 67 c0          	vmadd233pi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 67 c1          	vmadd233pi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 67 c7          	vmadd233pi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 67 c7          	vmadd233pi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 67 cf          	vmadd233pi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 67 ff          	vmadd233pi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 67 ff          	vmadd233pi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 67 ff          	vmadd233pi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 67 ff          	vmadd233pi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 67 ff          	vmadd233pi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 67 00          	vmadd233pi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 67 00          	vmadd233pi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 67 00          	vmadd233pi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 67 00          	vmadd233pi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 67 00          	vmadd233pi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 67 00          	vmadd233pi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 67 00          	vmadd233pi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 67 00          	vmadd233pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 67 00          	vmadd233pi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 67 00          	vmadd233pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 67 00          	vmadd233pi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 67 84 08 04 00 00 00 	vmadd233pi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 67 44 08 01    	vmadd233pi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 67 44 08 ff    	vmadd233pi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 89 c2          	vmaxpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 89 c2          	vmaxpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 89 c2          	vmaxpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 89 c2          	vmaxpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 89 c2          	vmaxpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 89 c2          	vmaxpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 89 c2          	vmaxpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 89 c2          	vmaxpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 89 c2          	vmaxpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 89 c2          	vmaxpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 89 c7          	vmaxpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 89 c0          	vmaxpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 89 c1          	vmaxpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 89 c7          	vmaxpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 89 c0          	vmaxpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 89 c1          	vmaxpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 89 c7          	vmaxpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 89 c7          	vmaxpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 89 cf          	vmaxpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 89 ff          	vmaxpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 89 ff          	vmaxpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 89 ff          	vmaxpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 89 ff          	vmaxpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 89 ff          	vmaxpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 89 00          	vmaxpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 89 00          	vmaxpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 89 00          	vmaxpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 89 00          	vmaxpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 89 00          	vmaxpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 89 00          	vmaxpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 89 00          	vmaxpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 89 00          	vmaxpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 89 00          	vmaxpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 89 00          	vmaxpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 89 00          	vmaxpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 89 84 08 04 00 00 00 	vmaxpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 89 44 08 01    	vmaxpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 89 44 08 ff    	vmaxpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 8a c2          	vmaxpu %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 8a c2          	vmaxpu %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 8a c2          	vmaxpu %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8a c2          	vmaxpu %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8a c2          	vmaxpu %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 8a c2          	vmaxpu %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8a c2          	vmaxpu %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8a c2          	vmaxpu %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8a c2          	vmaxpu %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8a c2          	vmaxpu %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8a c7          	vmaxpu %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8a c0          	vmaxpu %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8a c1          	vmaxpu %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8a c7          	vmaxpu %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8a c0          	vmaxpu %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8a c1          	vmaxpu %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 8a c7          	vmaxpu %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8a c7          	vmaxpu %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8a cf          	vmaxpu %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8a ff          	vmaxpu %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 8a ff          	vmaxpu %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 8a ff          	vmaxpu %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 8a ff          	vmaxpu %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 8a ff          	vmaxpu %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 8a 00          	vmaxpu \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8a 00          	vmaxpu \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8a 00          	vmaxpu \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8a 00          	vmaxpu \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8a 00          	vmaxpu \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8a 00          	vmaxpu \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8a 00          	vmaxpu \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8a 00          	vmaxpu \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8a 00          	vmaxpu \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8a 00          	vmaxpu \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8a 00          	vmaxpu \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8a 84 08 04 00 00 00 	vmaxpu 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8a 44 08 01    	vmaxpu 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8a 44 08 ff    	vmaxpu -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 8d c2          	vminpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 8d c2          	vminpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 8d c2          	vminpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8d c2          	vminpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8d c2          	vminpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 8d c2          	vminpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8d c2          	vminpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8d c2          	vminpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8d c2          	vminpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8d c2          	vminpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8d c7          	vminpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8d c0          	vminpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8d c1          	vminpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8d c7          	vminpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8d c0          	vminpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8d c1          	vminpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 8d c7          	vminpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8d c7          	vminpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8d cf          	vminpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8d ff          	vminpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 8d ff          	vminpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 8d ff          	vminpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 8d ff          	vminpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 8d ff          	vminpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 8d 00          	vminpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8d 00          	vminpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8d 00          	vminpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8d 00          	vminpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8d 00          	vminpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8d 00          	vminpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8d 00          	vminpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8d 00          	vminpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8d 00          	vminpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8d 00          	vminpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8d 00          	vminpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8d 84 08 04 00 00 00 	vminpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8d 44 08 01    	vminpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8d 44 08 ff    	vminpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 8e c2          	vminpu %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 8e c2          	vminpu %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 8e c2          	vminpu %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8e c2          	vminpu %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8e c2          	vminpu %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 8e c2          	vminpu %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8e c2          	vminpu %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8e c2          	vminpu %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8e c2          	vminpu %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8e c2          	vminpu %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8e c7          	vminpu %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8e c0          	vminpu %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8e c1          	vminpu %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 8e c7          	vminpu %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8e c0          	vminpu %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 8e c1          	vminpu %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 8e c7          	vminpu %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8e c7          	vminpu %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8e cf          	vminpu %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 8e ff          	vminpu %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 8e ff          	vminpu %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 8e ff          	vminpu %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 8e ff          	vminpu %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 8e ff          	vminpu %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 8e 00          	vminpu \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 8e 00          	vminpu \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 8e 00          	vminpu \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 8e 00          	vminpu \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 8e 00          	vminpu \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 8e 00          	vminpu \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 8e 00          	vminpu \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8e 00          	vminpu \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 8e 00          	vminpu \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8e 00          	vminpu \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8e 00          	vminpu \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8e 84 08 04 00 00 00 	vminpu 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8e 44 08 01    	vminpu 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 8e 44 08 ff    	vminpu -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 81 c2          	vmulhpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 81 c2          	vmulhpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 81 c2          	vmulhpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 81 c2          	vmulhpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 81 c2          	vmulhpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 81 c2          	vmulhpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 81 c2          	vmulhpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 81 c2          	vmulhpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 81 c2          	vmulhpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 81 c2          	vmulhpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 81 c7          	vmulhpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 81 c0          	vmulhpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 81 c1          	vmulhpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 81 c7          	vmulhpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 81 c0          	vmulhpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 81 c1          	vmulhpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 81 c7          	vmulhpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 81 c7          	vmulhpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 81 cf          	vmulhpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 81 ff          	vmulhpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 81 ff          	vmulhpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 81 ff          	vmulhpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 81 ff          	vmulhpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 81 ff          	vmulhpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 81 00          	vmulhpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 81 00          	vmulhpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 81 00          	vmulhpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 81 00          	vmulhpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 81 00          	vmulhpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 81 00          	vmulhpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 81 00          	vmulhpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 81 00          	vmulhpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 81 00          	vmulhpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 81 00          	vmulhpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 81 00          	vmulhpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 81 84 08 04 00 00 00 	vmulhpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 81 44 08 01    	vmulhpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 81 44 08 ff    	vmulhpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 82 c2          	vmulhpu %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 82 c2          	vmulhpu %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 82 c2          	vmulhpu %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 82 c2          	vmulhpu %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 82 c2          	vmulhpu %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 82 c2          	vmulhpu %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 82 c2          	vmulhpu %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 82 c2          	vmulhpu %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 82 c2          	vmulhpu %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 82 c2          	vmulhpu %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 82 c7          	vmulhpu %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 82 c0          	vmulhpu %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 82 c1          	vmulhpu %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 82 c7          	vmulhpu %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 82 c0          	vmulhpu %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 82 c1          	vmulhpu %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 82 c7          	vmulhpu %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 82 c7          	vmulhpu %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 82 cf          	vmulhpu %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 82 ff          	vmulhpu %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 82 ff          	vmulhpu %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 82 ff          	vmulhpu %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 82 ff          	vmulhpu %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 82 ff          	vmulhpu %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 82 00          	vmulhpu \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 82 00          	vmulhpu \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 82 00          	vmulhpu \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 82 00          	vmulhpu \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 82 00          	vmulhpu \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 82 00          	vmulhpu \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 82 00          	vmulhpu \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 82 00          	vmulhpu \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 82 00          	vmulhpu \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 82 00          	vmulhpu \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 82 00          	vmulhpu \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 82 84 08 04 00 00 00 	vmulhpu 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 82 44 08 01    	vmulhpu 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 82 44 08 ff    	vmulhpu -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 83 c2          	vmullpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 83 c2          	vmullpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 83 c2          	vmullpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 83 c2          	vmullpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 83 c2          	vmullpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 83 c2          	vmullpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 83 c2          	vmullpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 83 c2          	vmullpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 83 c2          	vmullpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 83 c2          	vmullpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 83 c7          	vmullpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 83 c0          	vmullpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 83 c1          	vmullpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 83 c7          	vmullpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 83 c0          	vmullpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 83 c1          	vmullpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 83 c7          	vmullpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 83 c7          	vmullpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 83 cf          	vmullpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 83 ff          	vmullpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 83 ff          	vmullpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 83 ff          	vmullpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 83 ff          	vmullpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 83 ff          	vmullpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 83 00          	vmullpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 83 00          	vmullpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 83 00          	vmullpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 83 00          	vmullpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 83 00          	vmullpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 83 00          	vmullpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 83 00          	vmullpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 83 00          	vmullpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 83 00          	vmullpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 83 00          	vmullpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 83 00          	vmullpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 83 84 08 04 00 00 00 	vmullpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 83 44 08 01    	vmullpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 83 44 08 ff    	vmullpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a6 c2          	vorpi  %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a6 c2          	vorpi  %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a6 c2          	vorpi  %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a6 c2          	vorpi  %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a6 c2          	vorpi  %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a6 c2          	vorpi  %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a6 c2          	vorpi  %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a6 c2          	vorpi  %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a6 c2          	vorpi  %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a6 c2          	vorpi  %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a6 c7          	vorpi  %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a6 c0          	vorpi  %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a6 c1          	vorpi  %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a6 c7          	vorpi  %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a6 c0          	vorpi  %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a6 c1          	vorpi  %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a6 c7          	vorpi  %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a6 c7          	vorpi  %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a6 cf          	vorpi  %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a6 ff          	vorpi  %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a6 ff          	vorpi  %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a6 ff          	vorpi  %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a6 ff          	vorpi  %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a6 ff          	vorpi  %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a6 00          	vorpi  \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a6 00          	vorpi  \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a6 00          	vorpi  \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a6 00          	vorpi  \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a6 00          	vorpi  \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a6 00          	vorpi  \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a6 00          	vorpi  \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a6 00          	vorpi  \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a6 00          	vorpi  \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a6 00          	vorpi  \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a6 00          	vorpi  \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a6 84 08 04 00 00 00 	vorpi  0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a6 44 08 01    	vorpi  0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a6 44 08 ff    	vorpi  -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a0 c2          	vsllpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a0 c2          	vsllpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a0 c2          	vsllpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a0 c2          	vsllpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a0 c2          	vsllpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a0 c2          	vsllpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a0 c2          	vsllpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a0 c2          	vsllpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a0 c2          	vsllpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a0 c2          	vsllpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a0 c7          	vsllpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a0 c0          	vsllpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a0 c1          	vsllpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a0 c7          	vsllpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a0 c0          	vsllpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a0 c1          	vsllpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a0 c7          	vsllpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a0 c7          	vsllpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a0 cf          	vsllpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a0 ff          	vsllpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a0 ff          	vsllpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a0 ff          	vsllpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a0 ff          	vsllpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a0 ff          	vsllpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a0 00          	vsllpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a0 00          	vsllpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a0 00          	vsllpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a0 00          	vsllpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a0 00          	vsllpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a0 00          	vsllpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a0 00          	vsllpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a0 00          	vsllpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a0 00          	vsllpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a0 00          	vsllpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a0 00          	vsllpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a0 84 08 04 00 00 00 	vsllpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a0 44 08 01    	vsllpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a0 44 08 ff    	vsllpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a1 c2          	vsrapi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a1 c2          	vsrapi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a1 c2          	vsrapi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a1 c2          	vsrapi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a1 c2          	vsrapi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a1 c2          	vsrapi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a1 c2          	vsrapi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a1 c2          	vsrapi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a1 c2          	vsrapi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a1 c2          	vsrapi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a1 c7          	vsrapi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a1 c0          	vsrapi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a1 c1          	vsrapi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a1 c7          	vsrapi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a1 c0          	vsrapi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a1 c1          	vsrapi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a1 c7          	vsrapi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a1 c7          	vsrapi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a1 cf          	vsrapi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a1 ff          	vsrapi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a1 ff          	vsrapi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a1 ff          	vsrapi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a1 ff          	vsrapi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a1 ff          	vsrapi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a1 00          	vsrapi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a1 00          	vsrapi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a1 00          	vsrapi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a1 00          	vsrapi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a1 00          	vsrapi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a1 00          	vsrapi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a1 00          	vsrapi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a1 00          	vsrapi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a1 00          	vsrapi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a1 00          	vsrapi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a1 00          	vsrapi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a1 84 08 04 00 00 00 	vsrapi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a1 44 08 01    	vsrapi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a1 44 08 ff    	vsrapi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a2 c2          	vsrlpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a2 c2          	vsrlpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a2 c2          	vsrlpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a2 c2          	vsrlpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a2 c2          	vsrlpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a2 c2          	vsrlpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a2 c2          	vsrlpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a2 c2          	vsrlpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a2 c2          	vsrlpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a2 c2          	vsrlpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a2 c7          	vsrlpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a2 c0          	vsrlpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a2 c1          	vsrlpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a2 c7          	vsrlpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a2 c0          	vsrlpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a2 c1          	vsrlpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a2 c7          	vsrlpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a2 c7          	vsrlpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a2 cf          	vsrlpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a2 ff          	vsrlpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a2 ff          	vsrlpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a2 ff          	vsrlpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a2 ff          	vsrlpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a2 ff          	vsrlpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a2 00          	vsrlpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a2 00          	vsrlpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a2 00          	vsrlpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a2 00          	vsrlpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a2 00          	vsrlpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a2 00          	vsrlpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a2 00          	vsrlpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a2 00          	vsrlpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a2 00          	vsrlpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a2 00          	vsrlpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a2 00          	vsrlpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a2 84 08 04 00 00 00 	vsrlpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a2 44 08 01    	vsrlpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a2 44 08 ff    	vsrlpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 51 c2          	vsubpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 51 c2          	vsubpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 51 c2          	vsubpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 51 c2          	vsubpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 51 c2          	vsubpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 51 c2          	vsubpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 51 c2          	vsubpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 51 c2          	vsubpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 51 c2          	vsubpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 51 c2          	vsubpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 51 c7          	vsubpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 51 c0          	vsubpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 51 c1          	vsubpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 51 c7          	vsubpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 51 c0          	vsubpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 51 c1          	vsubpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 51 c7          	vsubpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 51 c7          	vsubpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 51 cf          	vsubpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 51 ff          	vsubpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 51 ff          	vsubpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 51 ff          	vsubpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 51 ff          	vsubpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 51 ff          	vsubpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 51 00          	vsubpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 51 00          	vsubpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 51 00          	vsubpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 51 00          	vsubpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 51 00          	vsubpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 51 00          	vsubpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 51 00          	vsubpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 51 00          	vsubpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 51 00          	vsubpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 51 00          	vsubpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 51 00          	vsubpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 51 84 08 04 00 00 00 	vsubpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 51 44 08 01    	vsubpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 51 44 08 ff    	vsubpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 59 c2          	vsubrpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 59 c2          	vsubrpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f 59 c2          	vsubrpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 59 c2          	vsubrpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 59 c2          	vsubrpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f 59 c2          	vsubrpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 59 c2          	vsubrpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 59 c2          	vsubrpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 59 c2          	vsubrpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 59 c2          	vsubrpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 59 c7          	vsubrpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 59 c0          	vsubrpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 59 c1          	vsubrpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f 59 c7          	vsubrpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 59 c0          	vsubrpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f 59 c1          	vsubrpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f 59 c7          	vsubrpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 59 c7          	vsubrpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 59 cf          	vsubrpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f 59 ff          	vsubrpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 59 ff          	vsubrpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f 59 ff          	vsubrpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f 59 ff          	vsubrpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff 59 ff          	vsubrpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f 59 00          	vsubrpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f 59 00          	vsubrpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f 59 00          	vsubrpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f 59 00          	vsubrpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f 59 00          	vsubrpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f 59 00          	vsubrpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f 59 00          	vsubrpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 59 00          	vsubrpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f 59 00          	vsubrpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 59 00          	vsubrpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f 59 00          	vsubrpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 59 84 08 04 00 00 00 	vsubrpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 59 44 08 01    	vsubrpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f 59 44 08 ff    	vsubrpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 a7 c2          	vxorpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 a7 c2          	vxorpi %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f a7 c2          	vxorpi %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a7 c2          	vxorpi %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a7 c2          	vxorpi %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f a7 c2          	vxorpi %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a7 c2          	vxorpi %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a7 c2          	vxorpi %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a7 c2          	vxorpi %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a7 c2          	vxorpi %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a7 c7          	vxorpi %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a7 c0          	vxorpi %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a7 c1          	vxorpi %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f a7 c7          	vxorpi %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a7 c0          	vxorpi %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f a7 c1          	vxorpi %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f a7 c7          	vxorpi %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a7 c7          	vxorpi %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a7 cf          	vxorpi %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f a7 ff          	vxorpi %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 a7 ff          	vxorpi %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f a7 ff          	vxorpi %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f a7 ff          	vxorpi %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff a7 ff          	vxorpi %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f a7 00          	vxorpi \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f a7 00          	vxorpi \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f a7 00          	vxorpi \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f a7 00          	vxorpi \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f a7 00          	vxorpi \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f a7 00          	vxorpi \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f a7 00          	vxorpi \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a7 00          	vxorpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f a7 00          	vxorpi \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a7 00          	vxorpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f a7 00          	vxorpi \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a7 84 08 04 00 00 00 	vxorpi 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a7 44 08 01    	vxorpi 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f a7 44 08 ff    	vxorpi -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 08 b8 c2          	vscaleps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 08 b8 c2          	vscaleps %v2,%v1,%v0
[ 	]*[a-f0-9]+:	d6 00 0f b8 c2          	vscaleps %v2,%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f b8 c2          	vscaleps %v2\{cdab\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f b8 c2          	vscaleps %v2\{badc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 30 0f b8 c2          	vscaleps %v2\{aaaa\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f b8 c2          	vscaleps %v2\{bbbb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f b8 c2          	vscaleps %v2\{cccc\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f b8 c2          	vscaleps %v2\{dddd\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f b8 c2          	vscaleps %v2\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f b8 c7          	vscaleps %v7\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f b8 c0          	vscaleps %v8\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f b8 c1          	vscaleps %v9\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 71 0f b8 c7          	vscaleps %v15\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f b8 c0          	vscaleps %v16\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 72 0f b8 c1          	vscaleps %v17\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 73 0f b8 c7          	vscaleps %v31\{dacb\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f b8 c7          	vscaleps %v31\{dacb\},%v1,%v8\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f b8 cf          	vscaleps %v31\{dacb\},%v1,%v9\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 0f b8 ff          	vscaleps %v31\{dacb\},%v1,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 47 b8 ff          	vscaleps %v31\{dacb\},%v8,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 4f b8 ff          	vscaleps %v31\{dacb\},%v9,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 77 7f b8 ff          	vscaleps %v31\{dacb\},%v15,%v15\{%k7\}
[ 	]*[a-f0-9]+:	d6 7f ff b8 ff          	vscaleps %v31\{dacb\},%v31,%v31\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 0f b8 00          	vscaleps \(%rax\),%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 10 0f b8 00          	vscaleps \(%rax\)\{1to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 20 0f b8 00          	vscaleps \(%rax\)\{4to16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 50 0f b8 00          	vscaleps \(%rax\)\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 70 0f b8 00          	vscaleps \(%rax\)\{sint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 60 0f b8 00          	vscaleps \(%rax\)\{uint16\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 40 0f b8 00          	vscaleps \(%rax\)\{uint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f b8 00          	vscaleps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 0f b8 00          	vscaleps \(%rax\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f b8 00          	vscaleps \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 d0 0f b8 00          	vscaleps \(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f b8 84 08 04 00 00 00 	vscaleps 0x4\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f b8 44 08 01    	vscaleps 0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 83 0f b8 44 08 ff    	vscaleps -0x40\(%r8,%r9,1\)\{nt\},%v1,%v0\{%k7\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 04       	vloadd 0x40\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 10       	vloadd 0x100\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 01       	vloadd 0x40\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 04       	vloadd 0x100\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 01       	vloadd 0x40\(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 04       	vloadd 0x100\(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 10       	vloadd 0x40\(%rcx\)\{4to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 40       	vloadd 0x100\(%rcx\)\{4to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 04       	vloadd 0x40\(%rcx\)\{4to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 10       	vloadd 0x100\(%rcx\)\{4to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 04       	vloadd 0x40\(%rcx\)\{4to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 10       	vloadd 0x100\(%rcx\)\{4to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 13 49 40       	vloadunpackhd 0x40\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 13 89 00 01 00 00 	vloadunpackhd 0x100\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 20       	vloadunpackhd 0x40\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 40       	vloadunpackhd 0x80\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 10       	vloadunpackhd 0x40\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 40       	vloadunpackhd 0x100\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 10       	vloadunpackhd 0x40\(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 40       	vloadunpackhd 0x100\(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 62 ec d8             	vkmov  %bx,%k0
[ 	]*[a-f0-9]+:	62 ec d8                	vkmov  %ebx,%k0
[ 	]*[a-f0-9]+:	66 62 ec c1             	vkmov  %ax,%k1
[ 	]*[a-f0-9]+:	62 ec c1                	vkmov  %eax,%k1
[ 	]*[a-f0-9]+:	66 62 ec c2             	vkmov  %ax,%k2
[ 	]*[a-f0-9]+:	62 ec c2                	vkmov  %eax,%k2
[ 	]*[a-f0-9]+:	66 62 ec c3             	vkmov  %ax,%k3
[ 	]*[a-f0-9]+:	62 ec c3                	vkmov  %eax,%k3
[ 	]*[a-f0-9]+:	66 62 f4 c0             	bitinterleave11 %ax,%ax
[ 	]*[a-f0-9]+:	62 f4 c0                	bitinterleave11 %eax,%eax
[ 	]*[a-f0-9]+:	62 e4 c0                	bitinterleave11 %rax,%rax
[ 	]*[a-f0-9]+:	67 62 cb 02             	vprefetch1 \$0x0,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 cb 0a             	vprefetch1 \$0x1,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 cb 3a             	vprefetch1 \$0x7,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 db 02             	vprefetch2 \$0x0,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 db 0a             	vprefetch2 \$0x1,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 db 1a             	vprefetch2 \$0x3,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 cb 42 01          	vprefetch1 \$0x0,0x40\(%edx\)
[ 	]*[a-f0-9]+:	67 62 eb 02             	clevict1 \(%edx\)
[ 	]*[a-f0-9]+:	66 62 df c0             	delay  %ax
[ 	]*[a-f0-9]+:	62 df c0                	delay  %eax
[ 	]*[a-f0-9]+:	62 cf c0                	delay  %rax
[ 	]*[a-f0-9]+:	66 62 9f c4             	delay  %r12w
[ 	]*[a-f0-9]+:	62 9f c4                	delay  %r12d
[ 	]*[a-f0-9]+:	62 8f c4                	delay  %r12
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 00 00 	insertfield \$0x0,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff 00 00 	insertfield \$0x0,\$0x0,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 ff 00 	insertfield \$0x0,\$0xff,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff ff 00 	insertfield \$0x0,\$0xff,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 00 ff 	insertfield \$0xff,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff ff ff 	insertfield \$0xff,\$0xff,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	d6 00 00 f1 04 25 00 00 f0 ca d4 0f 	vshuf128x32 \{aadd\},\{3110\},0xffffffffcaf00000,%v0
[ 	]*[a-f0-9]+:	2e d6 00 00 f1 04 25 00 00 f0 ca d4 0f 	vshuf128x32 \{aadd\},\{3110\},%cs:0xffffffffcaf00000,%v0
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
[ 	]*[a-f0-9]+:	d6 00 01 21 cf          	vcmppi \{eq\},%v7,%v0,%k1\{%k1\}
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
[ 	]*[a-f0-9]+:	d6 00 01 22 cf          	vcmppu \{eq\},%v7,%v0,%k1\{%k1\}
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
[ 	]*[a-f0-9]+:	d6 00 01 20 cf          	vcmpps \{eq\},%v7,%v0,%k1\{%k1\}
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
[ 	]*[a-f0-9]+:	d6 00 00 10 40 01       	vloadd 0x40\(%rax\),%v0
[ 	]*[a-f0-9]+:	d6 00 00 10 40 ff       	vloadd -0x40\(%rax\),%v0
[ 	]*[a-f0-9]+:	66 62 df c8             	spflt  %ax
[ 	]*[a-f0-9]+:	62 df c8                	spflt  %eax
[ 	]*[a-f0-9]+:	62 cf c8                	spflt  %rax
[ 	]*[a-f0-9]+:	66 62 9f cc             	spflt  %r12w
[ 	]*[a-f0-9]+:	62 9f cc                	spflt  %r12d
[ 	]*[a-f0-9]+:	62 8f cc                	spflt  %r12
[ 	]*[a-f0-9]+:	d6 00 01 18 04 98       	vgatherd \(%rax,%v3,4\),%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 a0 01 18 04 f0       	vgatherd \(%rax,%v22,8\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 b0 19 18 44 30 7f    	vgatherd 0x7f\(%rax,%v30,1\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 18 44 50 20    	vgatherd 0x80\(%rax,%v10,2\)\{nt\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 19 18 84 88 80 00 00 00 	vgatherd 0x80\(%rax,%v9,4\)\{nt\}\{srgb8\},%v0\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 07 19 04 98       	vgatherpfd \(%rax,%v3,4\)\{%k7\}
[ 	]*[a-f0-9]+:	d6 80 07 19 04 d8       	vgatherpfd \(%rax,%v3,8\)\{%k7\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 1d 19 44 28 7f    	vgatherpfd 0x7f\(%rax,%v5,1\)\{%k5\}\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 80 04 19 44 70 20    	vgatherpfd 0x80\(%rax,%v6,2\)\{%k4\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 1b 19 84 b8 80 00 00 00 	vgatherpfd 0x80\(%rax,%v7,4\)\{%k3\}\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 04 01 1c 1c 0d 80 00 00 00 	vscatterd %v11,0x80\(,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 84 01 1c 1c 10       	vscatterd %v11,\(%rax,%v2,1\)\{%k1\}\{nt\}
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
[ 	]*[a-f0-9]+:	d6 10 11 1c 04 1d 80 00 00 00 	vscatterd %v0\{float16rz\},0x80\(,%v11,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 b0       	vscatterpfd \(%rax,%v6,4\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 80       	vscatterpfd \(%rax,%v0,4\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 08       	vscatterpfd \(%rax,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 50       	vscatterpfd \(%rax,%v2,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 98       	vscatterpfd \(%rax,%v3,4\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 44 98 20    	vscatterpfd 0x80\(%rax,%v3,4\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 0d 80 00 00 00 	vscatterpfd 0x80\(,%v1,1\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 d8       	vscatterpfd \(%rax,%v3,8\)\{%k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 20       	vscatterpfd \(%rax,%v4,1\)\{%k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 68       	vscatterpfd \(%rax,%v5,2\)\{%k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 f8       	vscatterpfd \(%rax,%v7,8\)\{%k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 00       	vscatterpfd \(%rax,%v8,1\)\{%k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 48       	vscatterpfd \(%rax,%v9,2\)\{%k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 90       	vscatterpfd \(%rax,%v10,4\)\{%k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 11 1d 44 d8 40    	vscatterpfd 0x80\(%rax,%v11,8\)\{%k1\}\{nt\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 04       	vloadd 0x40\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 10       	vloadd 0x100\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 01       	vloadd 0x40\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 04       	vloadd 0x100\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 01       	vloadd 0x40\(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 04       	vloadd 0x100\(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 10       	vloadd 0x40\(%rcx\)\{4to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 40       	vloadd 0x100\(%rcx\)\{4to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 04       	vloadd 0x40\(%rcx\)\{4to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 10       	vloadd 0x100\(%rcx\)\{4to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 04       	vloadd 0x40\(%rcx\)\{4to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 10       	vloadd 0x100\(%rcx\)\{4to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 13 49 40       	vloadunpackhd 0x40\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 13 89 00 01 00 00 	vloadunpackhd 0x100\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 20       	vloadunpackhd 0x40\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 40       	vloadunpackhd 0x80\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 10       	vloadunpackhd 0x40\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 40       	vloadunpackhd 0x100\(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 10       	vloadunpackhd 0x40\(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 40       	vloadunpackhd 0x100\(%rcx\)\{float11A\},%v1\{%k1\}
#pass

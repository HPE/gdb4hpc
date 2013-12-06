#as: -march=l1om
#objdump: -dw -Mintel
#name: L1OMni misc ops (Intel mode)
#source: l1omni-misc.s

.*: +file format .*


Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	62 cd c8                	vkand  k0,k1
[ 	]*[a-f0-9]+:	62 cd f9                	vkand  k1,k7
[ 	]*[a-f0-9]+:	62 dd c8                	vkandn k0,k1
[ 	]*[a-f0-9]+:	62 dd f9                	vkandn k1,k7
[ 	]*[a-f0-9]+:	62 ed c8                	vkandnr k0,k1
[ 	]*[a-f0-9]+:	62 ed f9                	vkandnr k1,k7
[ 	]*[a-f0-9]+:	62 fb c8                	vkmovlhb k0,k1
[ 	]*[a-f0-9]+:	62 fb f9                	vkmovlhb k1,k7
[ 	]*[a-f0-9]+:	62 fd c8                	vknot  k0,k1
[ 	]*[a-f0-9]+:	62 fd f9                	vknot  k1,k7
[ 	]*[a-f0-9]+:	62 ce c8                	vkor   k0,k1
[ 	]*[a-f0-9]+:	62 ce f9                	vkor   k1,k7
[ 	]*[a-f0-9]+:	62 de c8                	vkortest k0,k1
[ 	]*[a-f0-9]+:	62 de f9                	vkortest k1,k7
[ 	]*[a-f0-9]+:	62 ef c8                	vkswapb k0,k1
[ 	]*[a-f0-9]+:	62 ef f9                	vkswapb k1,k7
[ 	]*[a-f0-9]+:	62 fe c8                	vkxnor k0,k1
[ 	]*[a-f0-9]+:	62 fe f9                	vkxnor k1,k7
[ 	]*[a-f0-9]+:	62 ee c8                	vkxor  k0,k1
[ 	]*[a-f0-9]+:	62 ee f9                	vkxor  k1,k7
[ 	]*[a-f0-9]+:	66 62 f5 d8             	bitinterleave21 ax,bx
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 ebx,eax
[ 	]*[a-f0-9]+:	62 a5 c5                	bitinterleave21 r13,rax
[ 	]*[a-f0-9]+:	66 62 f2 d8             	bsff   ax,bx
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   ebx,eax
[ 	]*[a-f0-9]+:	62 a2 c5                	bsff   r13,rax
[ 	]*[a-f0-9]+:	66 62 f0 d8             	bsfi   ax,bx
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   ebx,eax
[ 	]*[a-f0-9]+:	62 a0 c5                	bsfi   r13,rax
[ 	]*[a-f0-9]+:	66 62 f3 d8             	bsrf   ax,bx
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   ebx,eax
[ 	]*[a-f0-9]+:	62 a3 c5                	bsrf   r13,rax
[ 	]*[a-f0-9]+:	66 62 f1 d8             	bsri   ax,bx
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   ebx,eax
[ 	]*[a-f0-9]+:	62 a1 c5                	bsri   r13,rax
[ 	]*[a-f0-9]+:	66 62 f8 d8             	countbits ax,bx
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits ebx,eax
[ 	]*[a-f0-9]+:	62 a8 c5                	countbits r13,rax
[ 	]*[a-f0-9]+:	66 62 f9 d8             	quadmask16 ax,bx
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 ebx,eax
[ 	]*[a-f0-9]+:	62 a9 c5                	quadmask16 r13,rax
[ 	]*[a-f0-9]+:	66 62 f5 c3             	bitinterleave21 bx,ax
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 ebx,eax
[ 	]*[a-f0-9]+:	62 a5 c5                	bitinterleave21 r13,rax
[ 	]*[a-f0-9]+:	66 62 f2 c3             	bsff   bx,ax
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   ebx,eax
[ 	]*[a-f0-9]+:	62 a2 c5                	bsff   r13,rax
[ 	]*[a-f0-9]+:	66 62 f0 c3             	bsfi   bx,ax
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   ebx,eax
[ 	]*[a-f0-9]+:	62 a0 c5                	bsfi   r13,rax
[ 	]*[a-f0-9]+:	66 62 f3 c3             	bsrf   bx,ax
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   ebx,eax
[ 	]*[a-f0-9]+:	62 a3 c5                	bsrf   r13,rax
[ 	]*[a-f0-9]+:	66 62 f1 c3             	bsri   bx,ax
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   ebx,eax
[ 	]*[a-f0-9]+:	62 a1 c5                	bsri   r13,rax
[ 	]*[a-f0-9]+:	66 62 f8 c3             	countbits bx,ax
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits ebx,eax
[ 	]*[a-f0-9]+:	62 a8 c5                	countbits r13,rax
[ 	]*[a-f0-9]+:	66 62 f9 c3             	quadmask16 bx,ax
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 ebx,eax
[ 	]*[a-f0-9]+:	62 a9 c5                	quadmask16 r13,rax
[ 	]*[a-f0-9]+:	d6 00 08 40 c2          	vaddps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 40 c2          	vaddps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 40 c2          	vaddps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 40 c2          	vaddps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 40 c2          	vaddps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 40 c2          	vaddps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 40 c2          	vaddps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 40 c2          	vaddps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 40 c2          	vaddps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 40 c2          	vaddps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 40 c7          	vaddps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 40 c0          	vaddps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 40 c1          	vaddps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 40 c7          	vaddps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 40 c0          	vaddps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 40 c1          	vaddps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 40 c7          	vaddps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 40 c7          	vaddps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 40 cf          	vaddps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 40 ff          	vaddps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 40 ff          	vaddps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 40 ff          	vaddps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 40 ff          	vaddps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 40 ff          	vaddps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 40 00          	vaddps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 40 84 08 04 00 00 00 	vaddps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 40 44 08 01    	vaddps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 40 44 08 ff    	vaddps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 48 c2          	vaddnps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 48 c2          	vaddnps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 48 c2          	vaddnps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 48 c2          	vaddnps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 48 c2          	vaddnps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 48 c2          	vaddnps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 48 c2          	vaddnps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 48 c2          	vaddnps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 48 c2          	vaddnps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 48 c2          	vaddnps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 48 c7          	vaddnps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 48 c0          	vaddnps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 48 c1          	vaddnps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 48 c7          	vaddnps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 48 c0          	vaddnps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 48 c1          	vaddnps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 48 c7          	vaddnps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 48 c7          	vaddnps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 48 cf          	vaddnps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 48 ff          	vaddnps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 48 ff          	vaddnps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 48 ff          	vaddnps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 48 ff          	vaddnps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 48 ff          	vaddnps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 48 00          	vaddnps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 48 84 08 04 00 00 00 	vaddnps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 48 44 08 01    	vaddnps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 48 44 08 ff    	vaddnps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 86 c2          	vclampzps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 86 c2          	vclampzps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 86 c2          	vclampzps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 86 c2          	vclampzps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 86 c2          	vclampzps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 86 c2          	vclampzps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 86 c2          	vclampzps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 86 c2          	vclampzps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 86 c2          	vclampzps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 86 c2          	vclampzps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 86 c7          	vclampzps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 86 c0          	vclampzps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 86 c1          	vclampzps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 86 c7          	vclampzps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 86 c0          	vclampzps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 86 c1          	vclampzps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 86 c7          	vclampzps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 86 c7          	vclampzps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 86 cf          	vclampzps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 86 ff          	vclampzps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 86 ff          	vclampzps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 86 ff          	vclampzps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 86 ff          	vclampzps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 86 ff          	vclampzps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 86 00          	vclampzps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 86 84 08 04 00 00 00 	vclampzps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 86 44 08 01    	vclampzps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 86 44 08 ff    	vclampzps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 64 c2          	vmadd132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 64 c2          	vmadd132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 64 c2          	vmadd132ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 64 c7          	vmadd132ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 64 c0          	vmadd132ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 64 c1          	vmadd132ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 64 c7          	vmadd132ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 64 c0          	vmadd132ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 64 c1          	vmadd132ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 64 c7          	vmadd132ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 64 c7          	vmadd132ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 64 cf          	vmadd132ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 64 ff          	vmadd132ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 64 ff          	vmadd132ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 64 ff          	vmadd132ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 64 ff          	vmadd132ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 64 ff          	vmadd132ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 64 00          	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 64 84 08 04 00 00 00 	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 64 44 08 01    	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 64 44 08 ff    	vmadd132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 62 c2          	vmadd213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 62 c2          	vmadd213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 62 c2          	vmadd213ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 62 c7          	vmadd213ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 62 c0          	vmadd213ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 62 c1          	vmadd213ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 62 c7          	vmadd213ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 62 c0          	vmadd213ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 62 c1          	vmadd213ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 62 c7          	vmadd213ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 62 c7          	vmadd213ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 62 cf          	vmadd213ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 62 ff          	vmadd213ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 62 ff          	vmadd213ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 62 ff          	vmadd213ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 62 ff          	vmadd213ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 62 ff          	vmadd213ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 62 00          	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 62 84 08 04 00 00 00 	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 62 44 08 01    	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 62 44 08 ff    	vmadd213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 60 c2          	vmadd231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 60 c2          	vmadd231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 60 c2          	vmadd231ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 60 c7          	vmadd231ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 60 c0          	vmadd231ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 60 c1          	vmadd231ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 60 c7          	vmadd231ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 60 c0          	vmadd231ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 60 c1          	vmadd231ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 60 c7          	vmadd231ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 60 c7          	vmadd231ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 60 cf          	vmadd231ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 60 ff          	vmadd231ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 60 ff          	vmadd231ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 60 ff          	vmadd231ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 60 ff          	vmadd231ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 60 ff          	vmadd231ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 60 00          	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 60 84 08 04 00 00 00 	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 60 44 08 01    	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 60 44 08 ff    	vmadd231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 46 c2          	vaddsetsps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 46 c7          	vaddsetsps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 46 c0          	vaddsetsps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 46 c1          	vaddsetsps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 46 c7          	vaddsetsps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 46 c0          	vaddsetsps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 46 c1          	vaddsetsps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 46 c7          	vaddsetsps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 46 c7          	vaddsetsps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 46 cf          	vaddsetsps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 46 ff          	vaddsetsps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 46 ff          	vaddsetsps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 46 ff          	vaddsetsps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 46 ff          	vaddsetsps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 46 ff          	vaddsetsps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 46 00          	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 46 84 08 04 00 00 00 	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 46 44 08 01    	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 46 44 08 ff    	vaddsetsps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 66 c2          	vmadd233ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 66 c2          	vmadd233ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 66 c2          	vmadd233ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 66 c7          	vmadd233ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 66 c0          	vmadd233ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 66 c1          	vmadd233ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 66 c7          	vmadd233ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 66 c0          	vmadd233ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 66 c1          	vmadd233ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 66 c7          	vmadd233ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 66 c7          	vmadd233ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 66 cf          	vmadd233ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 66 ff          	vmadd233ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 66 ff          	vmadd233ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 66 ff          	vmadd233ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 66 ff          	vmadd233ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 66 ff          	vmadd233ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 66 00          	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 66 84 08 04 00 00 00 	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 66 44 08 01    	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 66 44 08 ff    	vmadd233ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 74 c2          	vmaddn132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 74 c2          	vmaddn132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 74 c2          	vmaddn132ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 74 c7          	vmaddn132ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 74 c0          	vmaddn132ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 74 c1          	vmaddn132ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 74 c7          	vmaddn132ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 74 c0          	vmaddn132ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 74 c1          	vmaddn132ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 74 c7          	vmaddn132ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 74 c7          	vmaddn132ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 74 cf          	vmaddn132ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 74 ff          	vmaddn132ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 74 ff          	vmaddn132ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 74 ff          	vmaddn132ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 74 ff          	vmaddn132ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 74 ff          	vmaddn132ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 74 00          	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 74 84 08 04 00 00 00 	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 74 44 08 01    	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 74 44 08 ff    	vmaddn132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 72 c2          	vmaddn213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 72 c2          	vmaddn213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 72 c2          	vmaddn213ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 72 c7          	vmaddn213ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 72 c0          	vmaddn213ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 72 c1          	vmaddn213ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 72 c7          	vmaddn213ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 72 c0          	vmaddn213ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 72 c1          	vmaddn213ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 72 c7          	vmaddn213ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 72 c7          	vmaddn213ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 72 cf          	vmaddn213ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 72 ff          	vmaddn213ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 72 ff          	vmaddn213ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 72 ff          	vmaddn213ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 72 ff          	vmaddn213ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 72 ff          	vmaddn213ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 72 00          	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 72 84 08 04 00 00 00 	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 72 44 08 01    	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 72 44 08 ff    	vmaddn213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 70 c2          	vmaddn231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 70 c2          	vmaddn231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 70 c2          	vmaddn231ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 70 c7          	vmaddn231ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 70 c0          	vmaddn231ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 70 c1          	vmaddn231ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 70 c7          	vmaddn231ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 70 c0          	vmaddn231ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 70 c1          	vmaddn231ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 70 c7          	vmaddn231ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 70 c7          	vmaddn231ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 70 cf          	vmaddn231ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 70 ff          	vmaddn231ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 70 ff          	vmaddn231ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 70 ff          	vmaddn231ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 70 ff          	vmaddn231ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 70 ff          	vmaddn231ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 70 00          	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 70 84 08 04 00 00 00 	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 70 44 08 01    	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 70 44 08 ff    	vmaddn231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 88 c2          	vmaxps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 88 c2          	vmaxps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 88 c2          	vmaxps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 88 c2          	vmaxps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 88 c2          	vmaxps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 88 c2          	vmaxps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 88 c2          	vmaxps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 88 c2          	vmaxps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 88 c2          	vmaxps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 88 c2          	vmaxps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 88 c7          	vmaxps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 88 c0          	vmaxps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 88 c1          	vmaxps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 88 c7          	vmaxps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 88 c0          	vmaxps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 88 c1          	vmaxps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 88 c7          	vmaxps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 88 c7          	vmaxps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 88 cf          	vmaxps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 88 ff          	vmaxps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 88 ff          	vmaxps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 88 ff          	vmaxps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 88 ff          	vmaxps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 88 ff          	vmaxps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 88 00          	vmaxps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 88 84 08 04 00 00 00 	vmaxps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 88 44 08 01    	vmaxps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 88 44 08 ff    	vmaxps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 8c c2          	vminps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 8c c2          	vminps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 8c c2          	vminps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 8c c2          	vminps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 8c c2          	vminps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 8c c2          	vminps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 8c c2          	vminps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 8c c2          	vminps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 8c c2          	vminps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 8c c2          	vminps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 8c c7          	vminps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8c c0          	vminps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8c c1          	vminps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8c c7          	vminps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8c c0          	vminps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8c c1          	vminps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 8c c7          	vminps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8c c7          	vminps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8c cf          	vminps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8c ff          	vminps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 8c ff          	vminps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 8c ff          	vminps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 8c ff          	vminps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 8c ff          	vminps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8c 00          	vminps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 8c 84 08 04 00 00 00 	vminps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8c 44 08 01    	vminps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8c 44 08 ff    	vminps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 6c c2          	vmsub132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 6c c2          	vmsub132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 6c c2          	vmsub132ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 6c c7          	vmsub132ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 6c c0          	vmsub132ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 6c c1          	vmsub132ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 6c c7          	vmsub132ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 6c c0          	vmsub132ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 6c c1          	vmsub132ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 6c c7          	vmsub132ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 6c c7          	vmsub132ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 6c cf          	vmsub132ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 6c ff          	vmsub132ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 6c ff          	vmsub132ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 6c ff          	vmsub132ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 6c ff          	vmsub132ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 6c ff          	vmsub132ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6c 00          	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 6c 84 08 04 00 00 00 	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 6c 44 08 01    	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 6c 44 08 ff    	vmsub132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 6a c2          	vmsub213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 6a c2          	vmsub213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 6a c2          	vmsub213ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 6a c7          	vmsub213ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 6a c0          	vmsub213ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 6a c1          	vmsub213ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 6a c7          	vmsub213ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 6a c0          	vmsub213ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 6a c1          	vmsub213ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 6a c7          	vmsub213ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 6a c7          	vmsub213ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 6a cf          	vmsub213ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 6a ff          	vmsub213ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 6a ff          	vmsub213ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 6a ff          	vmsub213ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 6a ff          	vmsub213ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 6a ff          	vmsub213ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 6a 00          	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 6a 84 08 04 00 00 00 	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 6a 44 08 01    	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 6a 44 08 ff    	vmsub213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 68 c2          	vmsub231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 68 c2          	vmsub231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 68 c2          	vmsub231ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 68 c7          	vmsub231ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 68 c0          	vmsub231ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 68 c1          	vmsub231ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 68 c7          	vmsub231ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 68 c0          	vmsub231ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 68 c1          	vmsub231ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 68 c7          	vmsub231ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 68 c7          	vmsub231ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 68 cf          	vmsub231ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 68 ff          	vmsub231ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 68 ff          	vmsub231ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 68 ff          	vmsub231ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 68 ff          	vmsub231ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 68 ff          	vmsub231ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 68 00          	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 68 84 08 04 00 00 00 	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 68 44 08 01    	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 68 44 08 ff    	vmsub231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 7c c2          	vmsubr132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 7c c2          	vmsubr132ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 7c c2          	vmsubr132ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 7c c7          	vmsubr132ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7c c0          	vmsubr132ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7c c1          	vmsubr132ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7c c7          	vmsubr132ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 7c c0          	vmsubr132ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 7c c1          	vmsubr132ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 7c c7          	vmsubr132ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7c c7          	vmsubr132ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7c cf          	vmsubr132ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7c ff          	vmsubr132ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 7c ff          	vmsubr132ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 7c ff          	vmsubr132ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 7c ff          	vmsubr132ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 7c ff          	vmsubr132ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7c 00          	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 7c 84 08 04 00 00 00 	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 7c 44 08 01    	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 7c 44 08 ff    	vmsubr132ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 7a c2          	vmsubr213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 7a c2          	vmsubr213ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 7a c2          	vmsubr213ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 7a c7          	vmsubr213ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7a c0          	vmsubr213ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7a c1          	vmsubr213ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7a c7          	vmsubr213ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 7a c0          	vmsubr213ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 7a c1          	vmsubr213ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 7a c7          	vmsubr213ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7a c7          	vmsubr213ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7a cf          	vmsubr213ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7a ff          	vmsubr213ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 7a ff          	vmsubr213ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 7a ff          	vmsubr213ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 7a ff          	vmsubr213ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 7a ff          	vmsubr213ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7a 00          	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 7a 84 08 04 00 00 00 	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 7a 44 08 01    	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 7a 44 08 ff    	vmsubr213ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 78 c2          	vmsubr231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 78 c2          	vmsubr231ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 78 c2          	vmsubr231ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 78 c7          	vmsubr231ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 78 c0          	vmsubr231ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 78 c1          	vmsubr231ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 78 c7          	vmsubr231ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 78 c0          	vmsubr231ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 78 c1          	vmsubr231ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 78 c7          	vmsubr231ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 78 c7          	vmsubr231ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 78 cf          	vmsubr231ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 78 ff          	vmsubr231ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 78 ff          	vmsubr231ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 78 ff          	vmsubr231ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 78 ff          	vmsubr231ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 78 ff          	vmsubr231ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 78 00          	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 78 84 08 04 00 00 00 	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 78 44 08 01    	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 78 44 08 ff    	vmsubr231ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 7e c2          	vmsubr23c1ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 7e c2          	vmsubr23c1ps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 7e c2          	vmsubr23c1ps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 7e c7          	vmsubr23c1ps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7e c0          	vmsubr23c1ps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7e c1          	vmsubr23c1ps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 7e c7          	vmsubr23c1ps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 7e c0          	vmsubr23c1ps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 7e c1          	vmsubr23c1ps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 7e c7          	vmsubr23c1ps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7e c7          	vmsubr23c1ps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7e cf          	vmsubr23c1ps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 7e ff          	vmsubr23c1ps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 7e ff          	vmsubr23c1ps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 7e ff          	vmsubr23c1ps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 7e ff          	vmsubr23c1ps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 7e ff          	vmsubr23c1ps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 7e 00          	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 7e 84 08 04 00 00 00 	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 7e 44 08 01    	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 7e 44 08 ff    	vmsubr23c1ps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 80 c2          	vmulps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 80 c2          	vmulps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 80 c2          	vmulps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 80 c2          	vmulps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 80 c2          	vmulps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 80 c2          	vmulps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 80 c2          	vmulps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 80 c2          	vmulps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 80 c2          	vmulps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 80 c2          	vmulps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 80 c7          	vmulps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 80 c0          	vmulps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 80 c1          	vmulps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 80 c7          	vmulps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 80 c0          	vmulps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 80 c1          	vmulps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 80 c7          	vmulps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 80 c7          	vmulps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 80 cf          	vmulps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 80 ff          	vmulps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 80 ff          	vmulps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 80 ff          	vmulps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 80 ff          	vmulps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 80 ff          	vmulps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 80 00          	vmulps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 80 84 08 04 00 00 00 	vmulps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 80 44 08 01    	vmulps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 80 44 08 ff    	vmulps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 c2 c2          	vrcprefineps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 c2 c2          	vrcprefineps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f c2 c2          	vrcprefineps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f c2 c7          	vrcprefineps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f c2 c0          	vrcprefineps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f c2 c1          	vrcprefineps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f c2 c7          	vrcprefineps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f c2 c0          	vrcprefineps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f c2 c1          	vrcprefineps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f c2 c7          	vrcprefineps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f c2 c7          	vrcprefineps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f c2 cf          	vrcprefineps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f c2 ff          	vrcprefineps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 c2 ff          	vrcprefineps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f c2 ff          	vrcprefineps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f c2 ff          	vrcprefineps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff c2 ff          	vrcprefineps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f c2 00          	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f c2 84 08 04 00 00 00 	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f c2 44 08 01    	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f c2 44 08 ff    	vrcprefineps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 50 c2          	vsubps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 50 c2          	vsubps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 50 c2          	vsubps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 50 c2          	vsubps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 50 c2          	vsubps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 50 c2          	vsubps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 50 c2          	vsubps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 50 c2          	vsubps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 50 c2          	vsubps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 50 c2          	vsubps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 50 c7          	vsubps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 50 c0          	vsubps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 50 c1          	vsubps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 50 c7          	vsubps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 50 c0          	vsubps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 50 c1          	vsubps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 50 c7          	vsubps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 50 c7          	vsubps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 50 cf          	vsubps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 50 ff          	vsubps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 50 ff          	vsubps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 50 ff          	vsubps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 50 ff          	vsubps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 50 ff          	vsubps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 50 00          	vsubps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 50 84 08 04 00 00 00 	vsubps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 50 44 08 01    	vsubps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 50 44 08 ff    	vsubps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 58 c2          	vsubrps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 58 c2          	vsubrps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 58 c2          	vsubrps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 58 c2          	vsubrps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 58 c2          	vsubrps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 58 c2          	vsubrps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 58 c2          	vsubrps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 58 c2          	vsubrps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 58 c2          	vsubrps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 58 c2          	vsubrps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 58 c7          	vsubrps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 58 c0          	vsubrps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 58 c1          	vsubrps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 58 c7          	vsubrps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 58 c0          	vsubrps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 58 c1          	vsubrps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 58 c7          	vsubrps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 58 c7          	vsubrps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 58 cf          	vsubrps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 58 ff          	vsubrps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 58 ff          	vsubrps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 58 ff          	vsubrps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 58 ff          	vsubrps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 58 ff          	vsubrps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 58 00          	vsubrps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 58 84 08 04 00 00 00 	vsubrps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 58 44 08 01    	vsubrps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 58 44 08 ff    	vsubrps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a5 c2          	vandpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a5 c2          	vandpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a5 c2          	vandpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a5 c2          	vandpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a5 c2          	vandpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a5 c2          	vandpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a5 c2          	vandpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a5 c2          	vandpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a5 c2          	vandpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a5 c2          	vandpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a5 c7          	vandpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a5 c0          	vandpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a5 c1          	vandpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a5 c7          	vandpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a5 c0          	vandpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a5 c1          	vandpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a5 c7          	vandpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a5 c7          	vandpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a5 cf          	vandpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a5 ff          	vandpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a5 ff          	vandpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a5 ff          	vandpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a5 ff          	vandpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a5 ff          	vandpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a5 00          	vandpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a5 84 08 04 00 00 00 	vandpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a5 44 08 01    	vandpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a5 44 08 ff    	vandpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a4 c2          	vandnpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a4 c2          	vandnpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a4 c2          	vandnpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a4 c2          	vandnpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a4 c2          	vandnpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a4 c2          	vandnpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a4 c2          	vandnpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a4 c2          	vandnpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a4 c2          	vandnpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a4 c2          	vandnpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a4 c7          	vandnpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a4 c0          	vandnpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a4 c1          	vandnpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a4 c7          	vandnpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a4 c0          	vandnpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a4 c1          	vandnpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a4 c7          	vandnpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a4 c7          	vandnpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a4 cf          	vandnpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a4 ff          	vandnpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a4 ff          	vandnpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a4 ff          	vandnpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a4 ff          	vandnpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a4 ff          	vandnpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a4 00          	vandnpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a4 84 08 04 00 00 00 	vandnpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a4 44 08 01    	vandnpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a4 44 08 ff    	vandnpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a8 c2          	vbitinterleave11pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a8 c2          	vbitinterleave11pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a8 c2          	vbitinterleave11pi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a8 c7          	vbitinterleave11pi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a8 c0          	vbitinterleave11pi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a8 c1          	vbitinterleave11pi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a8 c7          	vbitinterleave11pi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a8 c0          	vbitinterleave11pi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a8 c1          	vbitinterleave11pi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a8 c7          	vbitinterleave11pi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a8 c7          	vbitinterleave11pi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a8 cf          	vbitinterleave11pi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a8 ff          	vbitinterleave11pi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a8 ff          	vbitinterleave11pi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a8 ff          	vbitinterleave11pi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a8 ff          	vbitinterleave11pi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a8 ff          	vbitinterleave11pi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a8 00          	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a8 84 08 04 00 00 00 	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a8 44 08 01    	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a8 44 08 ff    	vbitinterleave11pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a9 c2          	vbitinterleave21pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a9 c2          	vbitinterleave21pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a9 c2          	vbitinterleave21pi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a9 c7          	vbitinterleave21pi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a9 c0          	vbitinterleave21pi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a9 c1          	vbitinterleave21pi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a9 c7          	vbitinterleave21pi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a9 c0          	vbitinterleave21pi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a9 c1          	vbitinterleave21pi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a9 c7          	vbitinterleave21pi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a9 c7          	vbitinterleave21pi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a9 cf          	vbitinterleave21pi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a9 ff          	vbitinterleave21pi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a9 ff          	vbitinterleave21pi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a9 ff          	vbitinterleave21pi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a9 ff          	vbitinterleave21pi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a9 ff          	vbitinterleave21pi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a9 00          	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a9 84 08 04 00 00 00 	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a9 44 08 01    	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a9 44 08 ff    	vbitinterleave21pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 87 c2          	vclampzpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 87 c2          	vclampzpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 87 c2          	vclampzpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 87 c2          	vclampzpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 87 c2          	vclampzpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 87 c2          	vclampzpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 87 c2          	vclampzpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 87 c2          	vclampzpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 87 c2          	vclampzpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 87 c2          	vclampzpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 87 c7          	vclampzpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 87 c0          	vclampzpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 87 c1          	vclampzpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 87 c7          	vclampzpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 87 c0          	vclampzpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 87 c1          	vclampzpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 87 c7          	vclampzpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 87 c7          	vclampzpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 87 cf          	vclampzpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 87 ff          	vclampzpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 87 ff          	vclampzpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 87 ff          	vclampzpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 87 ff          	vclampzpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 87 ff          	vclampzpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 87 00          	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 87 84 08 04 00 00 00 	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 87 44 08 01    	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 87 44 08 ff    	vclampzpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 61 c2          	vmadd231pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 61 c2          	vmadd231pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 61 c2          	vmadd231pi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 61 c7          	vmadd231pi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 61 c0          	vmadd231pi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 61 c1          	vmadd231pi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 61 c7          	vmadd231pi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 61 c0          	vmadd231pi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 61 c1          	vmadd231pi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 61 c7          	vmadd231pi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 61 c7          	vmadd231pi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 61 cf          	vmadd231pi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 61 ff          	vmadd231pi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 61 ff          	vmadd231pi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 61 ff          	vmadd231pi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 61 ff          	vmadd231pi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 61 ff          	vmadd231pi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 61 00          	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 61 84 08 04 00 00 00 	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 61 44 08 01    	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 61 44 08 ff    	vmadd231pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 47 c2          	vaddsetspi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 47 c7          	vaddsetspi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 47 c0          	vaddsetspi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 47 c1          	vaddsetspi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 47 c7          	vaddsetspi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 47 c0          	vaddsetspi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 47 c1          	vaddsetspi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 47 c7          	vaddsetspi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 47 c7          	vaddsetspi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 47 cf          	vaddsetspi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 47 ff          	vaddsetspi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 47 ff          	vaddsetspi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 47 ff          	vaddsetspi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 47 ff          	vaddsetspi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 47 ff          	vaddsetspi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 47 00          	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 47 84 08 04 00 00 00 	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 47 44 08 01    	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 47 44 08 ff    	vaddsetspi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 67 c2          	vmadd233pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 67 c2          	vmadd233pi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 67 c2          	vmadd233pi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 67 c7          	vmadd233pi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 67 c0          	vmadd233pi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 67 c1          	vmadd233pi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 67 c7          	vmadd233pi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 67 c0          	vmadd233pi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 67 c1          	vmadd233pi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 67 c7          	vmadd233pi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 67 c7          	vmadd233pi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 67 cf          	vmadd233pi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 67 ff          	vmadd233pi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 67 ff          	vmadd233pi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 67 ff          	vmadd233pi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 67 ff          	vmadd233pi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 67 ff          	vmadd233pi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 67 00          	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 67 84 08 04 00 00 00 	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 67 44 08 01    	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 67 44 08 ff    	vmadd233pi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 89 c2          	vmaxpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 89 c2          	vmaxpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 89 c2          	vmaxpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 89 c2          	vmaxpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 89 c2          	vmaxpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 89 c2          	vmaxpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 89 c2          	vmaxpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 89 c2          	vmaxpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 89 c2          	vmaxpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 89 c2          	vmaxpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 89 c7          	vmaxpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 89 c0          	vmaxpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 89 c1          	vmaxpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 89 c7          	vmaxpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 89 c0          	vmaxpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 89 c1          	vmaxpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 89 c7          	vmaxpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 89 c7          	vmaxpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 89 cf          	vmaxpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 89 ff          	vmaxpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 89 ff          	vmaxpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 89 ff          	vmaxpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 89 ff          	vmaxpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 89 ff          	vmaxpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 89 00          	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 89 84 08 04 00 00 00 	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 89 44 08 01    	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 89 44 08 ff    	vmaxpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 8a c2          	vmaxpu v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 8a c2          	vmaxpu v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 8a c2          	vmaxpu v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 8a c2          	vmaxpu v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 8a c2          	vmaxpu v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 8a c2          	vmaxpu v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 8a c2          	vmaxpu v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 8a c2          	vmaxpu v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 8a c2          	vmaxpu v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 8a c2          	vmaxpu v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 8a c7          	vmaxpu v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8a c0          	vmaxpu v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8a c1          	vmaxpu v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8a c7          	vmaxpu v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8a c0          	vmaxpu v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8a c1          	vmaxpu v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 8a c7          	vmaxpu v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8a c7          	vmaxpu v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8a cf          	vmaxpu v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8a ff          	vmaxpu v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 8a ff          	vmaxpu v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 8a ff          	vmaxpu v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 8a ff          	vmaxpu v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 8a ff          	vmaxpu v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8a 00          	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 8a 84 08 04 00 00 00 	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8a 44 08 01    	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8a 44 08 ff    	vmaxpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 8d c2          	vminpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 8d c2          	vminpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 8d c2          	vminpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 8d c2          	vminpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 8d c2          	vminpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 8d c2          	vminpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 8d c2          	vminpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 8d c2          	vminpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 8d c2          	vminpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 8d c2          	vminpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 8d c7          	vminpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8d c0          	vminpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8d c1          	vminpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8d c7          	vminpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8d c0          	vminpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8d c1          	vminpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 8d c7          	vminpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8d c7          	vminpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8d cf          	vminpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8d ff          	vminpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 8d ff          	vminpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 8d ff          	vminpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 8d ff          	vminpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 8d ff          	vminpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8d 00          	vminpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 8d 84 08 04 00 00 00 	vminpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8d 44 08 01    	vminpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8d 44 08 ff    	vminpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 8e c2          	vminpu v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 8e c2          	vminpu v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 8e c2          	vminpu v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 8e c2          	vminpu v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 8e c2          	vminpu v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 8e c2          	vminpu v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 8e c2          	vminpu v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 8e c2          	vminpu v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 8e c2          	vminpu v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 8e c2          	vminpu v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 8e c7          	vminpu v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8e c0          	vminpu v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8e c1          	vminpu v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 8e c7          	vminpu v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8e c0          	vminpu v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 8e c1          	vminpu v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 8e c7          	vminpu v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8e c7          	vminpu v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8e cf          	vminpu v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 8e ff          	vminpu v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 8e ff          	vminpu v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 8e ff          	vminpu v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 8e ff          	vminpu v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 8e ff          	vminpu v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 8e 00          	vminpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 8e 84 08 04 00 00 00 	vminpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8e 44 08 01    	vminpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 8e 44 08 ff    	vminpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 81 c2          	vmulhpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 81 c2          	vmulhpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 81 c2          	vmulhpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 81 c2          	vmulhpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 81 c2          	vmulhpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 81 c2          	vmulhpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 81 c2          	vmulhpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 81 c2          	vmulhpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 81 c2          	vmulhpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 81 c2          	vmulhpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 81 c7          	vmulhpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 81 c0          	vmulhpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 81 c1          	vmulhpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 81 c7          	vmulhpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 81 c0          	vmulhpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 81 c1          	vmulhpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 81 c7          	vmulhpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 81 c7          	vmulhpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 81 cf          	vmulhpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 81 ff          	vmulhpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 81 ff          	vmulhpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 81 ff          	vmulhpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 81 ff          	vmulhpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 81 ff          	vmulhpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 81 00          	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 81 84 08 04 00 00 00 	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 81 44 08 01    	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 81 44 08 ff    	vmulhpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 82 c2          	vmulhpu v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 82 c2          	vmulhpu v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 82 c2          	vmulhpu v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 82 c2          	vmulhpu v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 82 c2          	vmulhpu v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 82 c2          	vmulhpu v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 82 c2          	vmulhpu v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 82 c2          	vmulhpu v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 82 c2          	vmulhpu v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 82 c2          	vmulhpu v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 82 c7          	vmulhpu v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 82 c0          	vmulhpu v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 82 c1          	vmulhpu v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 82 c7          	vmulhpu v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 82 c0          	vmulhpu v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 82 c1          	vmulhpu v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 82 c7          	vmulhpu v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 82 c7          	vmulhpu v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 82 cf          	vmulhpu v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 82 ff          	vmulhpu v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 82 ff          	vmulhpu v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 82 ff          	vmulhpu v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 82 ff          	vmulhpu v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 82 ff          	vmulhpu v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 82 00          	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 82 84 08 04 00 00 00 	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 82 44 08 01    	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 82 44 08 ff    	vmulhpu v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 83 c2          	vmullpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 83 c2          	vmullpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 83 c2          	vmullpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 83 c2          	vmullpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 83 c2          	vmullpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 83 c2          	vmullpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 83 c2          	vmullpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 83 c2          	vmullpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 83 c2          	vmullpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 83 c2          	vmullpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 83 c7          	vmullpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 83 c0          	vmullpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 83 c1          	vmullpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 83 c7          	vmullpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 83 c0          	vmullpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 83 c1          	vmullpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 83 c7          	vmullpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 83 c7          	vmullpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 83 cf          	vmullpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 83 ff          	vmullpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 83 ff          	vmullpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 83 ff          	vmullpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 83 ff          	vmullpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 83 ff          	vmullpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 83 00          	vmullpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 83 84 08 04 00 00 00 	vmullpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 83 44 08 01    	vmullpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 83 44 08 ff    	vmullpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a6 c2          	vorpi  v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a6 c2          	vorpi  v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a6 c2          	vorpi  v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a6 c2          	vorpi  v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a6 c2          	vorpi  v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a6 c2          	vorpi  v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a6 c2          	vorpi  v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a6 c2          	vorpi  v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a6 c2          	vorpi  v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a6 c2          	vorpi  v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a6 c7          	vorpi  v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a6 c0          	vorpi  v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a6 c1          	vorpi  v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a6 c7          	vorpi  v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a6 c0          	vorpi  v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a6 c1          	vorpi  v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a6 c7          	vorpi  v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a6 c7          	vorpi  v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a6 cf          	vorpi  v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a6 ff          	vorpi  v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a6 ff          	vorpi  v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a6 ff          	vorpi  v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a6 ff          	vorpi  v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a6 ff          	vorpi  v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a6 00          	vorpi  v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a6 84 08 04 00 00 00 	vorpi  v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a6 44 08 01    	vorpi  v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a6 44 08 ff    	vorpi  v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a0 c2          	vsllpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a0 c2          	vsllpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a0 c2          	vsllpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a0 c2          	vsllpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a0 c2          	vsllpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a0 c2          	vsllpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a0 c2          	vsllpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a0 c2          	vsllpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a0 c2          	vsllpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a0 c2          	vsllpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a0 c7          	vsllpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a0 c0          	vsllpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a0 c1          	vsllpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a0 c7          	vsllpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a0 c0          	vsllpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a0 c1          	vsllpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a0 c7          	vsllpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a0 c7          	vsllpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a0 cf          	vsllpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a0 ff          	vsllpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a0 ff          	vsllpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a0 ff          	vsllpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a0 ff          	vsllpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a0 ff          	vsllpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a0 00          	vsllpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a0 84 08 04 00 00 00 	vsllpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a0 44 08 01    	vsllpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a0 44 08 ff    	vsllpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a1 c2          	vsrapi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a1 c2          	vsrapi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a1 c2          	vsrapi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a1 c2          	vsrapi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a1 c2          	vsrapi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a1 c2          	vsrapi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a1 c2          	vsrapi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a1 c2          	vsrapi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a1 c2          	vsrapi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a1 c2          	vsrapi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a1 c7          	vsrapi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a1 c0          	vsrapi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a1 c1          	vsrapi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a1 c7          	vsrapi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a1 c0          	vsrapi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a1 c1          	vsrapi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a1 c7          	vsrapi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a1 c7          	vsrapi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a1 cf          	vsrapi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a1 ff          	vsrapi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a1 ff          	vsrapi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a1 ff          	vsrapi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a1 ff          	vsrapi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a1 ff          	vsrapi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a1 00          	vsrapi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a1 84 08 04 00 00 00 	vsrapi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a1 44 08 01    	vsrapi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a1 44 08 ff    	vsrapi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a2 c2          	vsrlpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a2 c2          	vsrlpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a2 c2          	vsrlpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a2 c2          	vsrlpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a2 c2          	vsrlpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a2 c2          	vsrlpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a2 c2          	vsrlpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a2 c2          	vsrlpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a2 c2          	vsrlpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a2 c2          	vsrlpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a2 c7          	vsrlpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a2 c0          	vsrlpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a2 c1          	vsrlpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a2 c7          	vsrlpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a2 c0          	vsrlpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a2 c1          	vsrlpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a2 c7          	vsrlpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a2 c7          	vsrlpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a2 cf          	vsrlpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a2 ff          	vsrlpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a2 ff          	vsrlpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a2 ff          	vsrlpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a2 ff          	vsrlpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a2 ff          	vsrlpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a2 00          	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a2 84 08 04 00 00 00 	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a2 44 08 01    	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a2 44 08 ff    	vsrlpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 51 c2          	vsubpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 51 c2          	vsubpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 51 c2          	vsubpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 51 c2          	vsubpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 51 c2          	vsubpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 51 c2          	vsubpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 51 c2          	vsubpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 51 c2          	vsubpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 51 c2          	vsubpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 51 c2          	vsubpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 51 c7          	vsubpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 51 c0          	vsubpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 51 c1          	vsubpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 51 c7          	vsubpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 51 c0          	vsubpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 51 c1          	vsubpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 51 c7          	vsubpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 51 c7          	vsubpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 51 cf          	vsubpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 51 ff          	vsubpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 51 ff          	vsubpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 51 ff          	vsubpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 51 ff          	vsubpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 51 ff          	vsubpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 51 00          	vsubpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 51 84 08 04 00 00 00 	vsubpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 51 44 08 01    	vsubpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 51 44 08 ff    	vsubpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 59 c2          	vsubrpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 59 c2          	vsubrpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 59 c2          	vsubrpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 59 c2          	vsubrpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 59 c2          	vsubrpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 59 c2          	vsubrpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 59 c2          	vsubrpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 59 c2          	vsubrpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 59 c2          	vsubrpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 59 c2          	vsubrpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 59 c7          	vsubrpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 59 c0          	vsubrpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 59 c1          	vsubrpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 59 c7          	vsubrpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 59 c0          	vsubrpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 59 c1          	vsubrpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 59 c7          	vsubrpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 59 c7          	vsubrpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 59 cf          	vsubrpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 59 ff          	vsubrpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 59 ff          	vsubrpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 59 ff          	vsubrpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 59 ff          	vsubrpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 59 ff          	vsubrpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 59 00          	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 59 84 08 04 00 00 00 	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 59 44 08 01    	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 59 44 08 ff    	vsubrpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 a7 c2          	vxorpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 a7 c2          	vxorpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f a7 c2          	vxorpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f a7 c2          	vxorpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f a7 c2          	vxorpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f a7 c2          	vxorpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f a7 c2          	vxorpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f a7 c2          	vxorpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f a7 c2          	vxorpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f a7 c2          	vxorpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f a7 c7          	vxorpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a7 c0          	vxorpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a7 c1          	vxorpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f a7 c7          	vxorpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a7 c0          	vxorpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f a7 c1          	vxorpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f a7 c7          	vxorpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a7 c7          	vxorpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a7 cf          	vxorpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f a7 ff          	vxorpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 a7 ff          	vxorpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f a7 ff          	vxorpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f a7 ff          	vxorpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff a7 ff          	vxorpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f a7 00          	vxorpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f a7 84 08 04 00 00 00 	vxorpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a7 44 08 01    	vxorpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f a7 44 08 ff    	vxorpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 b8 c2          	vscaleps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 b8 c2          	vscaleps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f b8 c2          	vscaleps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f b8 c2          	vscaleps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f b8 c2          	vscaleps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f b8 c2          	vscaleps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f b8 c2          	vscaleps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f b8 c2          	vscaleps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f b8 c2          	vscaleps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f b8 c2          	vscaleps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f b8 c7          	vscaleps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f b8 c0          	vscaleps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f b8 c1          	vscaleps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f b8 c7          	vscaleps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f b8 c0          	vscaleps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f b8 c1          	vscaleps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f b8 c7          	vscaleps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f b8 c7          	vscaleps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f b8 cf          	vscaleps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f b8 ff          	vscaleps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 b8 ff          	vscaleps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f b8 ff          	vscaleps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f b8 ff          	vscaleps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff b8 ff          	vscaleps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f b8 00          	vscaleps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f b8 84 08 04 00 00 00 	vscaleps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f b8 44 08 01    	vscaleps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f b8 44 08 ff    	vscaleps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 04       	vloadd v1\{k1\},\[rcx\+0x40\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 10       	vloadd v1\{k1\},\[rcx\+0x100\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 01       	vloadd v1\{k1\},\[rcx\+0x40\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 04       	vloadd v1\{k1\},\[rcx\+0x100\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 01       	vloadd v1\{k1\},\[rcx\+0x40\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 04       	vloadd v1\{k1\},\[rcx\+0x100\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 10       	vloadd v1\{k1\},\[rcx\+0x40\]\{4to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 40       	vloadd v1\{k1\},\[rcx\+0x100\]\{4to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 04       	vloadd v1\{k1\},\[rcx\+0x40\]\{4to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 10       	vloadd v1\{k1\},\[rcx\+0x100\]\{4to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 04       	vloadd v1\{k1\},\[rcx\+0x40\]\{4to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 10       	vloadd v1\{k1\},\[rcx\+0x100\]\{4to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 29 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 13 89 00 01 00 00 	vloadunpackhd v1\{k1\},\[rcx\+0x100\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 20       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 10       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x100\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 10       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x100\]\{float11A\}
[ 	]*[a-f0-9]+:	66 62 ec d8             	vkmov  k0,bx
[ 	]*[a-f0-9]+:	62 ec d8                	vkmov  k0,ebx
[ 	]*[a-f0-9]+:	66 62 ec c1             	vkmov  k1,ax
[ 	]*[a-f0-9]+:	62 ec c1                	vkmov  k1,eax
[ 	]*[a-f0-9]+:	66 62 ec c2             	vkmov  k2,ax
[ 	]*[a-f0-9]+:	62 ec c2                	vkmov  k2,eax
[ 	]*[a-f0-9]+:	66 62 ec c3             	vkmov  k3,ax
[ 	]*[a-f0-9]+:	62 ec c3                	vkmov  k3,eax
[ 	]*[a-f0-9]+:	66 62 f4 c0             	bitinterleave11 ax,ax
[ 	]*[a-f0-9]+:	62 f4 c0                	bitinterleave11 eax,eax
[ 	]*[a-f0-9]+:	62 e4 c0                	bitinterleave11 rax,rax
[ 	]*[a-f0-9]+:	67 62 cb 02             	vprefetch1 \[edx\],0x0
[ 	]*[a-f0-9]+:	67 62 cb 0a             	vprefetch1 \[edx\],0x1
[ 	]*[a-f0-9]+:	67 62 cb 3a             	vprefetch1 \[edx\],0x7
[ 	]*[a-f0-9]+:	67 62 db 02             	vprefetch2 \[edx\],0x0
[ 	]*[a-f0-9]+:	67 62 db 0a             	vprefetch2 \[edx\],0x1
[ 	]*[a-f0-9]+:	67 62 db 1a             	vprefetch2 \[edx\],0x3
[ 	]*[a-f0-9]+:	67 62 cb 42 01          	vprefetch1 \[edx\+0x40\],0x0
[ 	]*[a-f0-9]+:	67 62 eb 02             	clevict1 BYTE PTR \[edx\]
[ 	]*[a-f0-9]+:	66 62 df c0             	delay  ax
[ 	]*[a-f0-9]+:	62 df c0                	delay  eax
[ 	]*[a-f0-9]+:	62 cf c0                	delay  rax
[ 	]*[a-f0-9]+:	66 62 9f c4             	delay  r12w
[ 	]*[a-f0-9]+:	62 9f c4                	delay  r12d
[ 	]*[a-f0-9]+:	62 8f c4                	delay  r12
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 00 00 	insertfield ax,ax,0x0,0x0,0x0
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff 00 00 	insertfield ax,ax,0xffff,0x0,0x0
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 ff 00 	insertfield ax,ax,0x0,0xff,0x0
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff ff 00 	insertfield ax,ax,0xffff,0xff,0x0
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 00 ff 	insertfield ax,ax,0x0,0x0,0xff
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff ff ff 	insertfield ax,ax,0xffff,0xff,0xff
[ 	]*[a-f0-9]+:	d6 00 00 f1 04 25 00 00 f0 ca d4 0f 	vshuf128x32 v0,ds:0xffffffffcaf00000,\{3110\},\{aadd\}
[ 	]*[a-f0-9]+:	2e d6 00 00 f1 04 25 00 00 f0 ca d4 0f 	vshuf128x32 v0,cs:0xffffffffcaf00000,\{3110\},\{aadd\}
[ 	]*[a-f0-9]+:	d6 00 08 9e c2          	vmaxabsps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 9e c2          	vmaxabsps v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 9e c2          	vmaxabsps v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 9e c7          	vmaxabsps v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 9e c0          	vmaxabsps v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 9e c1          	vmaxabsps v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 9e c7          	vmaxabsps v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 9e c0          	vmaxabsps v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 9e c1          	vmaxabsps v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 9e c7          	vmaxabsps v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 9e c7          	vmaxabsps v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 9e cf          	vmaxabsps v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 9e ff          	vmaxabsps v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 9e ff          	vmaxabsps v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 9e ff          	vmaxabsps v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 9e ff          	vmaxabsps v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 9e ff          	vmaxabsps v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 70 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{float16\}
[ 	]*[a-f0-9]+:	d6 40 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 9e 00          	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 0f 9e 84 08 04 00 00 00 	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 9e 44 08 01    	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 9e 44 08 ff    	vmaxabsps v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 43 c2          	vaddsetcpi v0,k1,v2
[ 	]*[a-f0-9]+:	d6 00 07 43 c2          	vaddsetcpi v0\{k7\},k0,v2
[ 	]*[a-f0-9]+:	d6 83 17 43 8c 08 04 00 00 00 	vaddsetcpi v1\{k7\},k2,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 1f 43 54 08 01    	vaddsetcpi v2\{k7\},k3,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 9d c2          	vcvtps2srgb8 v0,v2
[ 	]*[a-f0-9]+:	d6 00 01 9d c2          	vcvtps2srgb8 v0\{k1\},v2
[ 	]*[a-f0-9]+:	d6 30 01 9d c2          	vcvtps2srgb8 v0\{k1\},v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 9d c2          	vcvtps2srgb8 v0\{k1\},v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 9d c2          	vcvtps2srgb8 v0\{k1\},v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 5f 07 9d ff          	vcvtps2srgb8 v31\{k7\},v31\{cccc\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 00          	vcvtps2srgb8 v0\{k1\},ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 01 9d 00          	vcvtps2srgb8 v0\{k1\},ZMMWORD PTR \[r8\]
[ 	]*[a-f0-9]+:	d6 01 01 9d 07          	vcvtps2srgb8 v0\{k1\},ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 50 01 9d 00          	vcvtps2srgb8 v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 80 01 9d 00          	vcvtps2srgb8 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 01 9d 00          	vcvtps2srgb8 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 01 9d 00          	vcvtps2srgb8 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 07 9d 8c 08 04 00 00 00 	vcvtps2srgb8 v1\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 07 9d 54 08 01    	vcvtps2srgb8 v2\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 9d c2          	vcvtps2srgb8 v0\{k7\},v2
[ 	]*[a-f0-9]+:	d6 00 00 f0 c2 00 00    	vfixupps v0,v2,0x0
[ 	]*[a-f0-9]+:	d6 00 01 f0 c2 00 00    	vfixupps v0\{k1\},v2,0x0
[ 	]*[a-f0-9]+:	d6 30 01 f0 c2 00 00    	vfixupps v0\{k1\},v2\{aaaa\},0x0
[ 	]*[a-f0-9]+:	d6 40 01 f0 c2 00 00    	vfixupps v0\{k1\},v2\{bbbb\},0x0
[ 	]*[a-f0-9]+:	d6 50 01 f0 c2 00 00    	vfixupps v0\{k1\},v2\{cccc\},0x0
[ 	]*[a-f0-9]+:	d6 5f 07 f0 ff 00 00    	vfixupps v31\{k7\},v31\{cccc\},0x0
[ 	]*[a-f0-9]+:	d6 50 01 f0 00 00 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 80 01 f0 00 00 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 01 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x1
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 07 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x7
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 7f 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x7f
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 01    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x100
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 00 40    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x4000
[ 	]*[a-f0-9]+:	d6 d0 01 f0 00 ff ff    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0xffff
[ 	]*[a-f0-9]+:	d6 d0 09 f0 00 00 00    	vfixupps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x10000
[ 	]*[a-f0-9]+:	d6 00 01 10 00          	vloadd v0\{k1\},\[rax\]
[ 	]*[a-f0-9]+:	d6 00 01 10 00          	vloadd v0\{k1\},\[rax\]
[ 	]*[a-f0-9]+:	d6 10 01 10 00          	vloadd v0\{k1\},\[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 80 01 10 00          	vloadd v0\{k1\},\[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 19 10 00          	vloadd v0\{k1\},\[rax\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 19 10 00          	vloadd v0\{k1\},\[rax\]\{1to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 20 19 10 00          	vloadd v0\{k1\},\[rax\]\{4to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 a0 01 10 00          	vloadd v0\{k1\},\[rax\]\{nt\}\{4to16\}
[ 	]*[a-f0-9]+:	d6 80 19 10 00          	vloadd v0\{k1\},\[rax\]\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 90 19 10 00          	vloadd v0\{k1\},\[rax\]\{nt\}\{1to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 90 19 10 00          	vloadd v0\{k1\},\[rax\]\{nt\}\{1to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 01 14 00          	vstored \[rax\]\{k1\},v0
[ 	]*[a-f0-9]+:	d6 80 01 14 00          	vstored \[rax\]\{k1\}\{nt\},v0
[ 	]*[a-f0-9]+:	d6 20 01 14 00          	vstored \[rax\]\{k1\},v0\{dcba\}
[ 	]*[a-f0-9]+:	d6 00 29 14 00          	vstored \[rax\]\{k1\},v0\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 29 14 00          	vstored \[rax\]\{k1\},v0\{dcba\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 29 14 00          	vstored \[rax\]\{k1\},v0\{dcba\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 a0 01 14 00          	vstored \[rax\]\{k1\}\{nt\},v0\{dcba\}
[ 	]*[a-f0-9]+:	d6 80 29 14 00          	vstored \[rax\]\{k1\}\{nt\},v0\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 14 00          	vstored \[rax\]\{k1\}\{nt\},v0\{a\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 14 00          	vstored \[rax\]\{k1\}\{nt\},v0\{a\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 14 00          	vstored \[rax\]\{k1\}\{nt\},v0\{a\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 11 14 40 04       	vstored \[rax\+0x80\]\{k1\},v0\{float16rz\}
[ 	]*[a-f0-9]+:	d6 30 09 45 c2          	vadcpi v0\{k1\},k1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 09 45 c2          	vadcpi v0\{k1\},k1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 09 45 c2          	vadcpi v0\{k1\},k1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 5f 3f 45 ff          	vadcpi v31\{k7\},k7,v31\{cccc\}
[ 	]*[a-f0-9]+:	d6 00 09 45 c2          	vadcpi v0\{k1\},k1,v2
[ 	]*[a-f0-9]+:	d6 00 09 45 00          	vadcpi v0\{k1\},k1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 09 45 00          	vadcpi v0\{k1\},k1,ZMMWORD PTR \[r8\]
[ 	]*[a-f0-9]+:	d6 01 09 45 07          	vadcpi v0\{k1\},k1,ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 50 09 45 00          	vadcpi v0\{k1\},k1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 80 09 45 00          	vadcpi v0\{k1\},k1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 09 45 00          	vadcpi v0\{k1\},k1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 09 45 00          	vadcpi v0\{k1\},k1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 08 41 c2          	vaddpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 08 41 c2          	vaddpi v0,v1,v2
[ 	]*[a-f0-9]+:	d6 00 0f 41 c2          	vaddpi v0\{k7\},v1,v2
[ 	]*[a-f0-9]+:	d6 10 0f 41 c2          	vaddpi v0\{k7\},v1,v2\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 0f 41 c2          	vaddpi v0\{k7\},v1,v2\{badc\}
[ 	]*[a-f0-9]+:	d6 30 0f 41 c2          	vaddpi v0\{k7\},v1,v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 0f 41 c2          	vaddpi v0\{k7\},v1,v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 0f 41 c2          	vaddpi v0\{k7\},v1,v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 0f 41 c2          	vaddpi v0\{k7\},v1,v2\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 0f 41 c2          	vaddpi v0\{k7\},v1,v2\{dacb\}
[ 	]*[a-f0-9]+:	d6 70 0f 41 c7          	vaddpi v0\{k7\},v1,v7\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 41 c0          	vaddpi v0\{k7\},v1,v8\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 41 c1          	vaddpi v0\{k7\},v1,v9\{dacb\}
[ 	]*[a-f0-9]+:	d6 71 0f 41 c7          	vaddpi v0\{k7\},v1,v15\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 41 c0          	vaddpi v0\{k7\},v1,v16\{dacb\}
[ 	]*[a-f0-9]+:	d6 72 0f 41 c1          	vaddpi v0\{k7\},v1,v17\{dacb\}
[ 	]*[a-f0-9]+:	d6 73 0f 41 c7          	vaddpi v0\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 41 c7          	vaddpi v8\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 41 cf          	vaddpi v9\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 0f 41 ff          	vaddpi v15\{k7\},v1,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 47 41 ff          	vaddpi v15\{k7\},v8,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 4f 41 ff          	vaddpi v15\{k7\},v9,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 77 7f 41 ff          	vaddpi v15\{k7\},v15,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 7f ff 41 ff          	vaddpi v31\{k7\},v31,v31\{dacb\}
[ 	]*[a-f0-9]+:	d6 00 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 10 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 50 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 70 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 60 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 40 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 80 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 80 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 0f 41 00          	vaddpi v0\{k7\},v1,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 41 84 08 04 00 00 00 	vaddpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 41 44 08 01    	vaddpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 41 44 08 ff    	vaddpi v0\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 9b c2          	vcvtinsps2f11 v0,v2,rn,0x0
[ 	]*[a-f0-9]+:	d6 00 01 9b c2          	vcvtinsps2f11 v0\{k1\},v2,rn,0x0
[ 	]*[a-f0-9]+:	d6 30 01 9b c2          	vcvtinsps2f11 v0\{k1\},v2\{aaaa\},rn,0x0
[ 	]*[a-f0-9]+:	d6 40 01 9b c2          	vcvtinsps2f11 v0\{k1\},v2\{bbbb\},rn,0x0
[ 	]*[a-f0-9]+:	d6 50 01 9b c2          	vcvtinsps2f11 v0\{k1\},v2\{cccc\},rn,0x0
[ 	]*[a-f0-9]+:	d6 5f 07 9b ff          	vcvtinsps2f11 v31\{k7\},v31\{cccc\},rn,0x0
[ 	]*[a-f0-9]+:	d6 00 01 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\],rn,0x0
[ 	]*[a-f0-9]+:	d6 01 01 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[r8\],rn,0x0
[ 	]*[a-f0-9]+:	d6 01 01 9b 07          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[r15\],rn,0x0
[ 	]*[a-f0-9]+:	d6 50 01 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\},rn,0x0
[ 	]*[a-f0-9]+:	d6 80 01 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},rn,0x0
[ 	]*[a-f0-9]+:	d6 d0 01 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,0x0
[ 	]*[a-f0-9]+:	d6 d0 c1 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rz,0x0
[ 	]*[a-f0-9]+:	d6 d0 01 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,0x0
[ 	]*[a-f0-9]+:	d6 d0 09 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,0x1
[ 	]*[a-f0-9]+:	d6 d0 11 9b 00          	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,0x2
[ 	]*[a-f0-9]+:	d6 d0 11 9b 80 16 00 00 00 	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\+0x16\]\{nt\}\{unorm8\},rn,0x2
[ 	]*[a-f0-9]+:	d6 d0 11 9b 84 98 16 00 00 00 	vcvtinsps2f11 v0\{k1\},ZMMWORD PTR \[rax\+rbx\*4\+0x16\]\{nt\}\{unorm8\},rn,0x2
[ 	]*[a-f0-9]+:	d6 00 00 95 c2          	vcvtpi2ps v0,v2,.0b
[ 	]*[a-f0-9]+:	d6 00 01 95 c2          	vcvtpi2ps v0\{k1\},v2,.0b
[ 	]*[a-f0-9]+:	d6 30 01 95 c2          	vcvtpi2ps v0\{k1\},v2\{aaaa\},.0b
[ 	]*[a-f0-9]+:	d6 40 01 95 c2          	vcvtpi2ps v0\{k1\},v2\{bbbb\},.0b
[ 	]*[a-f0-9]+:	d6 50 01 95 c2          	vcvtpi2ps v0\{k1\},v2\{cccc\},.0b
[ 	]*[a-f0-9]+:	d6 5f 07 95 ff          	vcvtpi2ps v31\{k7\},v31\{cccc\},.0b
[ 	]*[a-f0-9]+:	d6 00 01 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\],.0b
[ 	]*[a-f0-9]+:	d6 01 01 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[r8\],.0b
[ 	]*[a-f0-9]+:	d6 01 01 95 07          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[r15\],.0b
[ 	]*[a-f0-9]+:	d6 50 01 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 80 01 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 09 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.4b
[ 	]*[a-f0-9]+:	d6 d0 11 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.5b
[ 	]*[a-f0-9]+:	d6 d0 19 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.8b
[ 	]*[a-f0-9]+:	d6 d0 21 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.16b
[ 	]*[a-f0-9]+:	d6 d0 29 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.24b
[ 	]*[a-f0-9]+:	d6 d0 31 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.31b
[ 	]*[a-f0-9]+:	d6 d0 39 95 00          	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.32b
[ 	]*[a-f0-9]+:	d6 d0 01 95 40 01       	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 95 80 08 00 00 00 	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\+0x8\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 95 80 04 00 00 00 	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\+0x4\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 95 40 01       	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 95 44 98 01    	vcvtpi2ps v0\{k1\},ZMMWORD PTR \[rax\+rbx\*4\+0x10\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 00 00 91 c2          	vcvtps2pi v0,v2,rn,.0b
[ 	]*[a-f0-9]+:	d6 00 01 91 c2          	vcvtps2pi v0\{k1\},v2,rn,.0b
[ 	]*[a-f0-9]+:	d6 30 01 91 c2          	vcvtps2pi v0\{k1\},v2\{aaaa\},rn,.0b
[ 	]*[a-f0-9]+:	d6 40 01 91 c2          	vcvtps2pi v0\{k1\},v2\{bbbb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 50 01 91 c2          	vcvtps2pi v0\{k1\},v2\{cccc\},rn,.0b
[ 	]*[a-f0-9]+:	d6 5f 07 91 ff          	vcvtps2pi v31\{k7\},v31\{cccc\},rn,.0b
[ 	]*[a-f0-9]+:	d6 00 01 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\],rn,.0b
[ 	]*[a-f0-9]+:	d6 01 01 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[r8\],rn,.0b
[ 	]*[a-f0-9]+:	d6 01 01 91 07          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[r15\],rn,.0b
[ 	]*[a-f0-9]+:	d6 50 01 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 80 01 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 09 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.4b
[ 	]*[a-f0-9]+:	d6 d0 11 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.5b
[ 	]*[a-f0-9]+:	d6 d0 19 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.8b
[ 	]*[a-f0-9]+:	d6 d0 21 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 d0 29 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.24b
[ 	]*[a-f0-9]+:	d6 d0 31 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.31b
[ 	]*[a-f0-9]+:	d6 d0 39 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.32b
[ 	]*[a-f0-9]+:	d6 d0 79 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rd,.32b
[ 	]*[a-f0-9]+:	d6 d0 b9 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},ru,.32b
[ 	]*[a-f0-9]+:	d6 d0 f9 91 00          	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rz,.32b
[ 	]*[a-f0-9]+:	d6 d0 01 91 40 01       	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 91 80 08 00 00 00 	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\+0x8\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 91 80 04 00 00 00 	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\+0x4\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 91 40 01       	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 91 44 98 01    	vcvtps2pi v0\{k1\},ZMMWORD PTR \[rax\+rbx\*4\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 00 00    	vinsertfieldpi v0\{k1\},v1,v2,0x0,0x0,0x0
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 00 08    	vinsertfieldpi v0\{k1\},v1,v2,0x0,0x0,0x1
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 40 00    	vinsertfieldpi v0\{k1\},v1,v2,0x0,0x1,0x0
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 01 00    	vinsertfieldpi v0\{k1\},v1,v2,0x1,0x0,0x0
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 01 08    	vinsertfieldpi v0\{k1\},v1,v2,0x1,0x0,0x1
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 41 08    	vinsertfieldpi v0\{k1\},v1,v2,0x1,0x1,0x1
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 c1 79    	vinsertfieldpi v0\{k1\},v1,v2,0x1,0x7,0xf
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 88 18    	vinsertfieldpi v0\{k1\},v1,v2,0x8,0x2,0x3
[ 	]*[a-f0-9]+:	d6 00 09 f2 c2 d0 98    	vinsertfieldpi v0\{k1\},v1,v2,0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 50 0f f2 c2 d0 98    	vinsertfieldpi v0\{k7\},v1,v2\{cccc\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 0f f2 40 01 d0 98 	vinsertfieldpi v0\{k7\},v1,ZMMWORD PTR \[rax\+0x10\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 0f f2 80 01 00 00 00 d0 98 	vinsertfieldpi v0\{k7\},v1,ZMMWORD PTR \[rax\+0x1\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 0f f2 44 98 01 d0 98 	vinsertfieldpi v0\{k7\},v1,ZMMWORD PTR \[rax\+rbx\*4\+0x10\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 0f f2 84 98 01 00 00 00 d0 98 	vinsertfieldpi v0\{k7\},v1,ZMMWORD PTR \[rax\+rbx\*4\+0x1\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 00 00    	vrotatefieldpi v0\{k1\},v2,0x0,0x0,0x0
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 00 08    	vrotatefieldpi v0\{k1\},v2,0x0,0x0,0x1
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 40 00    	vrotatefieldpi v0\{k1\},v2,0x0,0x1,0x0
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 01 00    	vrotatefieldpi v0\{k1\},v2,0x1,0x0,0x0
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 01 08    	vrotatefieldpi v0\{k1\},v2,0x1,0x0,0x1
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 41 08    	vrotatefieldpi v0\{k1\},v2,0x1,0x1,0x1
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 c1 79    	vrotatefieldpi v0\{k1\},v2,0x1,0x7,0xf
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 88 18    	vrotatefieldpi v0\{k1\},v2,0x8,0x2,0x3
[ 	]*[a-f0-9]+:	d6 00 01 f3 c2 d0 98    	vrotatefieldpi v0\{k1\},v2,0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 50 07 f3 c2 d0 98    	vrotatefieldpi v0\{k7\},v2\{cccc\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 07 f3 40 01 d0 98 	vrotatefieldpi v0\{k7\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 07 f3 80 01 00 00 00 d0 98 	vrotatefieldpi v0\{k7\},ZMMWORD PTR \[rax\+0x1\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 07 f3 44 98 01 d0 98 	vrotatefieldpi v0\{k7\},ZMMWORD PTR \[rax\+rbx\*4\+0x10\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 d0 07 f3 84 98 01 00 00 00 d0 98 	vrotatefieldpi v0\{k7\},ZMMWORD PTR \[rax\+rbx\*4\+0x1\]\{nt\}\{sint8\},0x10,0x3,0x13
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 30 01 21 c9          	vcmppi k1\{k1\},v0,v1\{aaaa\},eq
[ 	]*[a-f0-9]+:	d6 40 01 21 c9          	vcmppi k1\{k1\},v0,v1\{bbbb\},eq
[ 	]*[a-f0-9]+:	d6 00 01 21 f9          	vcmppi k7\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 07 21 c9          	vcmppi k1\{k7\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 f9 21 c9          	vcmppi k1\{k1\},v31,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 21 cf          	vcmppi k1\{k1\},v0,v7,eq
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 21 c9          	vcmppi k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 04 01 21 c9          	vcmppi k1\{k1\},v0,v1,lt
[ 	]*[a-f0-9]+:	d6 08 01 21 c9          	vcmppi k1\{k1\},v0,v1,le
[ 	]*[a-f0-9]+:	d6 00 01 25 c9          	vcmppi k1\{k1\},v0,v1,neq
[ 	]*[a-f0-9]+:	d6 04 01 25 c9          	vcmppi k1\{k1\},v0,v1,nlt
[ 	]*[a-f0-9]+:	d6 08 01 25 c9          	vcmppi k1\{k1\},v0,v1,nle
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 04 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],lt
[ 	]*[a-f0-9]+:	d6 08 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],le
[ 	]*[a-f0-9]+:	d6 00 01 25 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],neq
[ 	]*[a-f0-9]+:	d6 04 01 25 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],nlt
[ 	]*[a-f0-9]+:	d6 08 01 25 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\],nle
[ 	]*[a-f0-9]+:	d6 80 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 50 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\]\{sint8\},eq
[ 	]*[a-f0-9]+:	d6 d0 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{sint8\},eq
[ 	]*[a-f0-9]+:	d6 d0 01 21 08          	vcmppi k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{sint8\},eq
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 30 01 22 c9          	vcmppu k1\{k1\},v0,v1\{aaaa\},eq
[ 	]*[a-f0-9]+:	d6 40 01 22 c9          	vcmppu k1\{k1\},v0,v1\{bbbb\},eq
[ 	]*[a-f0-9]+:	d6 00 01 22 f9          	vcmppu k7\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 07 22 c9          	vcmppu k1\{k7\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 f9 22 c9          	vcmppu k1\{k1\},v31,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 22 cf          	vcmppu k1\{k1\},v0,v7,eq
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 22 c9          	vcmppu k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 04 01 22 c9          	vcmppu k1\{k1\},v0,v1,lt
[ 	]*[a-f0-9]+:	d6 08 01 22 c9          	vcmppu k1\{k1\},v0,v1,le
[ 	]*[a-f0-9]+:	d6 00 01 26 c9          	vcmppu k1\{k1\},v0,v1,neq
[ 	]*[a-f0-9]+:	d6 04 01 26 c9          	vcmppu k1\{k1\},v0,v1,nlt
[ 	]*[a-f0-9]+:	d6 08 01 26 c9          	vcmppu k1\{k1\},v0,v1,nle
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 04 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],lt
[ 	]*[a-f0-9]+:	d6 08 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],le
[ 	]*[a-f0-9]+:	d6 00 01 26 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],neq
[ 	]*[a-f0-9]+:	d6 04 01 26 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],nlt
[ 	]*[a-f0-9]+:	d6 08 01 26 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\],nle
[ 	]*[a-f0-9]+:	d6 80 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 50 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\]\{sint8\},eq
[ 	]*[a-f0-9]+:	d6 d0 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{sint8\},eq
[ 	]*[a-f0-9]+:	d6 d0 01 22 08          	vcmppu k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{sint8\},eq
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 30 01 20 c9          	vcmpps k1\{k1\},v0,v1\{aaaa\},eq
[ 	]*[a-f0-9]+:	d6 40 01 20 c9          	vcmpps k1\{k1\},v0,v1\{bbbb\},eq
[ 	]*[a-f0-9]+:	d6 00 01 20 f9          	vcmpps k7\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 07 20 c9          	vcmpps k1\{k7\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 f9 20 c9          	vcmpps k1\{k1\},v31,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 20 cf          	vcmpps k1\{k1\},v0,v7,eq
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 00 01 20 c9          	vcmpps k1\{k1\},v0,v1,eq
[ 	]*[a-f0-9]+:	d6 04 01 20 c9          	vcmpps k1\{k1\},v0,v1,lt
[ 	]*[a-f0-9]+:	d6 08 01 20 c9          	vcmpps k1\{k1\},v0,v1,le
[ 	]*[a-f0-9]+:	d6 00 01 24 c9          	vcmpps k1\{k1\},v0,v1,neq
[ 	]*[a-f0-9]+:	d6 04 01 24 c9          	vcmpps k1\{k1\},v0,v1,nlt
[ 	]*[a-f0-9]+:	d6 08 01 24 c9          	vcmpps k1\{k1\},v0,v1,nle
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 00 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],eq
[ 	]*[a-f0-9]+:	d6 04 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],lt
[ 	]*[a-f0-9]+:	d6 08 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],le
[ 	]*[a-f0-9]+:	d6 00 01 24 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],neq
[ 	]*[a-f0-9]+:	d6 04 01 24 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],nlt
[ 	]*[a-f0-9]+:	d6 08 01 24 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\],nle
[ 	]*[a-f0-9]+:	d6 80 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 50 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\]\{unorm8\},eq
[ 	]*[a-f0-9]+:	d6 d0 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},eq
[ 	]*[a-f0-9]+:	d6 d0 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},eq
[ 	]*[a-f0-9]+:	d6 dc 01 20 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},unord
[ 	]*[a-f0-9]+:	d6 dc 01 24 08          	vcmpps k1\{k1\},v0,ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},ord
[ 	]*[a-f0-9]+:	d6 00 00 9a c2          	vcvtinsps2u10 v0,v2,0x0
[ 	]*[a-f0-9]+:	d6 00 01 9a c2          	vcvtinsps2u10 v0\{k1\},v2,0x0
[ 	]*[a-f0-9]+:	d6 30 01 9a c2          	vcvtinsps2u10 v0\{k1\},v2\{aaaa\},0x0
[ 	]*[a-f0-9]+:	d6 40 01 9a c2          	vcvtinsps2u10 v0\{k1\},v2\{bbbb\},0x0
[ 	]*[a-f0-9]+:	d6 50 01 9a c2          	vcvtinsps2u10 v0\{k1\},v2\{cccc\},0x0
[ 	]*[a-f0-9]+:	d6 5f 07 9a ff          	vcvtinsps2u10 v31\{k7\},v31\{cccc\},0x0
[ 	]*[a-f0-9]+:	d6 00 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\],0x0
[ 	]*[a-f0-9]+:	d6 01 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[r8\],0x0
[ 	]*[a-f0-9]+:	d6 01 01 9a 07          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[r15\],0x0
[ 	]*[a-f0-9]+:	d6 50 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 80 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 d0 01 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x0
[ 	]*[a-f0-9]+:	d6 d0 09 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x1
[ 	]*[a-f0-9]+:	d6 d0 11 9a 00          	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 d0 11 9a 40 01       	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 d0 11 9a 80 16 00 00 00 	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\+0x16\]\{nt\}\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 d0 11 9a 84 98 16 00 00 00 	vcvtinsps2u10 v0\{k1\},ZMMWORD PTR \[rax\+rbx\*4\+0x16\]\{nt\}\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 00 00 92 c2          	vcvtps2pu v0,v2,rn,.0b
[ 	]*[a-f0-9]+:	d6 00 01 92 c2          	vcvtps2pu v0\{k1\},v2,rn,.0b
[ 	]*[a-f0-9]+:	d6 30 01 92 c2          	vcvtps2pu v0\{k1\},v2\{aaaa\},rn,.0b
[ 	]*[a-f0-9]+:	d6 40 01 92 c2          	vcvtps2pu v0\{k1\},v2\{bbbb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 50 01 92 c2          	vcvtps2pu v0\{k1\},v2\{cccc\},rn,.0b
[ 	]*[a-f0-9]+:	d6 5f 07 92 ff          	vcvtps2pu v31\{k7\},v31\{cccc\},rn,.0b
[ 	]*[a-f0-9]+:	d6 00 01 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\],rn,.0b
[ 	]*[a-f0-9]+:	d6 01 01 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[r8\],rn,.0b
[ 	]*[a-f0-9]+:	d6 01 01 92 07          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[r15\],rn,.0b
[ 	]*[a-f0-9]+:	d6 50 01 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 80 01 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 09 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.4b
[ 	]*[a-f0-9]+:	d6 d0 11 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.5b
[ 	]*[a-f0-9]+:	d6 d0 19 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.8b
[ 	]*[a-f0-9]+:	d6 d0 21 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 d0 29 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.24b
[ 	]*[a-f0-9]+:	d6 d0 31 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.31b
[ 	]*[a-f0-9]+:	d6 d0 39 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.32b
[ 	]*[a-f0-9]+:	d6 d0 79 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rd,.32b
[ 	]*[a-f0-9]+:	d6 d0 b9 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},ru,.32b
[ 	]*[a-f0-9]+:	d6 d0 f9 92 00          	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rz,.32b
[ 	]*[a-f0-9]+:	d6 d0 01 92 40 01       	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 92 80 08 00 00 00 	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\+0x8\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 92 80 04 00 00 00 	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\+0x4\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 92 40 01       	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 92 44 98 01    	vcvtps2pu v0\{k1\},ZMMWORD PTR \[rax\+rbx\*4\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 00 00 96 c2          	vcvtpu2ps v0,v2,.0b
[ 	]*[a-f0-9]+:	d6 00 01 96 c2          	vcvtpu2ps v0\{k1\},v2,.0b
[ 	]*[a-f0-9]+:	d6 30 01 96 c2          	vcvtpu2ps v0\{k1\},v2\{aaaa\},.0b
[ 	]*[a-f0-9]+:	d6 40 01 96 c2          	vcvtpu2ps v0\{k1\},v2\{bbbb\},.0b
[ 	]*[a-f0-9]+:	d6 50 01 96 c2          	vcvtpu2ps v0\{k1\},v2\{cccc\},.0b
[ 	]*[a-f0-9]+:	d6 5f 07 96 ff          	vcvtpu2ps v31\{k7\},v31\{cccc\},.0b
[ 	]*[a-f0-9]+:	d6 00 01 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\],.0b
[ 	]*[a-f0-9]+:	d6 01 01 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[r8\],.0b
[ 	]*[a-f0-9]+:	d6 01 01 96 07          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[r15\],.0b
[ 	]*[a-f0-9]+:	d6 50 01 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 80 01 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 09 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.4b
[ 	]*[a-f0-9]+:	d6 d0 11 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.5b
[ 	]*[a-f0-9]+:	d6 d0 19 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.8b
[ 	]*[a-f0-9]+:	d6 d0 21 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.16b
[ 	]*[a-f0-9]+:	d6 d0 29 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.24b
[ 	]*[a-f0-9]+:	d6 d0 31 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.31b
[ 	]*[a-f0-9]+:	d6 d0 39 96 00          	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\},.32b
[ 	]*[a-f0-9]+:	d6 d0 01 96 40 01       	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 96 80 08 00 00 00 	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\+0x8\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 96 80 04 00 00 00 	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\+0x4\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 96 40 01       	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 d0 01 96 44 98 01    	vcvtpu2ps v0\{k1\},ZMMWORD PTR \[rax\+rbx\*4\+0x10\]\{nt\}\{sint8\},.0b
[ 	]*[a-f0-9]+:	d6 00 00 12 00          	vloadunpackld v0,\[rax\]
[ 	]*[a-f0-9]+:	d6 00 00 12 38          	vloadunpackld v7,\[rax\]
[ 	]*[a-f0-9]+:	d6 04 00 12 00          	vloadunpackld v8,\[rax\]
[ 	]*[a-f0-9]+:	d6 04 00 12 38          	vloadunpackld v15,\[rax\]
[ 	]*[a-f0-9]+:	d6 0c 00 12 38          	vloadunpackld v31,\[rax\]
[ 	]*[a-f0-9]+:	d6 01 00 12 00          	vloadunpackld v0,\[r8\]
[ 	]*[a-f0-9]+:	d6 01 00 12 07          	vloadunpackld v0,\[r15\]
[ 	]*[a-f0-9]+:	d6 0d 00 12 3f          	vloadunpackld v31,\[r15\]
[ 	]*[a-f0-9]+:	d6 80 00 12 00          	vloadunpackld v0,\[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 12 00          	vloadunpackld v0\{k1\},\[rax\]
[ 	]*[a-f0-9]+:	d6 80 01 12 00          	vloadunpackld v0\{k1\},\[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 13 00          	vloadunpackhd v0,\[rax\]
[ 	]*[a-f0-9]+:	d6 80 00 13 00          	vloadunpackhd v0,\[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 13 00          	vloadunpackhd v0\{k1\},\[rax\]
[ 	]*[a-f0-9]+:	d6 80 01 13 00          	vloadunpackhd v0\{k1\},\[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 16 00          	vpackstoreld \[rax\],v0
[ 	]*[a-f0-9]+:	d6 80 00 16 00          	vpackstoreld \[rax\]\{nt\},v0
[ 	]*[a-f0-9]+:	d6 00 01 16 00          	vpackstoreld \[rax\]\{k1\},v0
[ 	]*[a-f0-9]+:	d6 80 01 16 00          	vpackstoreld \[rax\]\{k1\}\{nt\},v0
[ 	]*[a-f0-9]+:	d6 01 00 16 00          	vpackstoreld \[r8\],v0
[ 	]*[a-f0-9]+:	d6 81 00 16 07          	vpackstoreld \[r15\]\{nt\},v0
[ 	]*[a-f0-9]+:	d6 00 07 16 00          	vpackstoreld \[rax\]\{k7\},v0
[ 	]*[a-f0-9]+:	d6 84 01 16 38          	vpackstoreld \[rax\]\{k1\}\{nt\},v15
[ 	]*[a-f0-9]+:	d6 80 01 16 40 20       	vpackstoreld \[rax\+0x80\]\{k1\}\{nt\},v0
[ 	]*[a-f0-9]+:	d6 00 00 17 00          	vpackstorehd \[rax\],v0
[ 	]*[a-f0-9]+:	d6 80 00 17 00          	vpackstorehd \[rax\]\{nt\},v0
[ 	]*[a-f0-9]+:	d6 00 01 17 00          	vpackstorehd \[rax\]\{k1\},v0
[ 	]*[a-f0-9]+:	d6 80 01 17 00          	vpackstorehd \[rax\]\{k1\}\{nt\},v0
[ 	]*[a-f0-9]+:	d6 01 00 17 00          	vpackstorehd \[r8\],v0
[ 	]*[a-f0-9]+:	d6 81 00 17 07          	vpackstorehd \[r15\]\{nt\},v0
[ 	]*[a-f0-9]+:	d6 00 07 17 00          	vpackstorehd \[rax\]\{k7\},v0
[ 	]*[a-f0-9]+:	d6 84 01 17 38          	vpackstorehd \[rax\]\{k1\}\{nt\},v15
[ 	]*[a-f0-9]+:	d6 80 01 17 40 20       	vpackstorehd \[rax\+0x80\]\{k1\}\{nt\},v0
[ 	]*[a-f0-9]+:	d6 00 00 98 c2          	vroundps v0,v2,rn,.0b
[ 	]*[a-f0-9]+:	d6 00 01 98 c2          	vroundps v0\{k1\},v2,rn,.0b
[ 	]*[a-f0-9]+:	d6 30 01 98 c2          	vroundps v0\{k1\},v2\{aaaa\},rn,.0b
[ 	]*[a-f0-9]+:	d6 40 01 98 c2          	vroundps v0\{k1\},v2\{bbbb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 50 01 98 c2          	vroundps v0\{k1\},v2\{cccc\},rn,.0b
[ 	]*[a-f0-9]+:	d6 5f 07 98 ff          	vroundps v31\{k7\},v31\{cccc\},rn,.0b
[ 	]*[a-f0-9]+:	d6 00 01 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\],rn,.0b
[ 	]*[a-f0-9]+:	d6 01 01 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[r8\],rn,.0b
[ 	]*[a-f0-9]+:	d6 01 01 98 07          	vroundps v0\{k1\},ZMMWORD PTR \[r15\],rn,.0b
[ 	]*[a-f0-9]+:	d6 50 01 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 80 01 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 09 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.4b
[ 	]*[a-f0-9]+:	d6 d0 11 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.5b
[ 	]*[a-f0-9]+:	d6 d0 19 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.8b
[ 	]*[a-f0-9]+:	d6 d0 21 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 d0 29 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.24b
[ 	]*[a-f0-9]+:	d6 d0 31 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.31b
[ 	]*[a-f0-9]+:	d6 d0 39 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rn,.32b
[ 	]*[a-f0-9]+:	d6 d0 79 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rd,.32b
[ 	]*[a-f0-9]+:	d6 d0 b9 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},ru,.32b
[ 	]*[a-f0-9]+:	d6 d0 f9 98 00          	vroundps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\},rz,.32b
[ 	]*[a-f0-9]+:	d6 d0 01 98 40 01       	vroundps v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 98 80 08 00 00 00 	vroundps v0\{k1\},ZMMWORD PTR \[rax\+0x8\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 98 80 04 00 00 00 	vroundps v0\{k1\},ZMMWORD PTR \[rax\+0x4\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 98 40 01       	vroundps v0\{k1\},ZMMWORD PTR \[rax\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 d0 01 98 44 98 01    	vroundps v0\{k1\},ZMMWORD PTR \[rax\+rbx\*4\+0x10\]\{nt\}\{unorm8\},rn,.0b
[ 	]*[a-f0-9]+:	d6 00 00 30 c9          	vtestpi k1,v0,v1
[ 	]*[a-f0-9]+:	d6 30 00 30 c9          	vtestpi k1,v0,v1\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 00 30 c9          	vtestpi k1,v0,v1\{bbbb\}
[ 	]*[a-f0-9]+:	d6 00 00 30 f9          	vtestpi k7,v0,v1
[ 	]*[a-f0-9]+:	d6 00 07 30 c9          	vtestpi k1\{k7\},v0,v1
[ 	]*[a-f0-9]+:	d6 00 f8 30 c9          	vtestpi k1,v31,v1
[ 	]*[a-f0-9]+:	d6 03 00 30 cf          	vtestpi k1,v0,v31
[ 	]*[a-f0-9]+:	d6 00 00 30 08          	vtestpi k1,v0,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 00 00 30 38          	vtestpi k7,v0,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 00 07 30 08          	vtestpi k1\{k7\},v0,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 00 f8 30 08          	vtestpi k1,v31,ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 00 30 0f          	vtestpi k1,v0,ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 80 00 30 08          	vtestpi k1,v0,ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 50 00 30 08          	vtestpi k1,v0,ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 00 30 08          	vtestpi k1,v0,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 00 30 08          	vtestpi k1,v0,ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 0f 30 8c 08 04 00 00 00 	vtestpi k1\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 30 4c 08 01    	vtestpi k1\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 0f 30 4c 08 ff    	vtestpi k1\{k7\},v1,ZMMWORD PTR \[r8\+r9\*1-0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 00    	vshuf128x32 v0,v2,\{0000\},\{aaaa\}
[ 	]*[a-f0-9]+:	d6 0f 07 f1 ff 00 00    	vshuf128x32 v31\{k7\},v31,\{0000\},\{aaaa\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 00 00 00    	vshuf128x32 v0,\[rax\],\{0000\},\{aaaa\}
[ 	]*[a-f0-9]+:	d6 01 00 f1 00 00 00    	vshuf128x32 v0,\[r8\],\{0000\},\{aaaa\}
[ 	]*[a-f0-9]+:	d6 80 00 f1 00 00 00    	vshuf128x32 v0,\[rax\]\{nt\},\{0000\},\{aaaa\}
[ 	]*[a-f0-9]+:	d6 01 00 f1 07 00 00    	vshuf128x32 v0,\[r15\],\{0000\},\{aaaa\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 03    	vshuf128x32 v0,v2,\{0000\},\{aaad\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 0f    	vshuf128x32 v0,v2,\{0000\},\{aadd\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 3f    	vshuf128x32 v0,v2,\{0000\},\{addd\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 00 ff    	vshuf128x32 v0,v2,\{0000\},\{dddd\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 02 ff    	vshuf128x32 v0,v2,\{0002\},\{dddd\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 22 ff    	vshuf128x32 v0,v2,\{0202\},\{dddd\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 ff ff    	vshuf128x32 v0,v2,\{3333\},\{dddd\}
[ 	]*[a-f0-9]+:	d6 00 00 f1 c2 ff 1b    	vshuf128x32 v0,v2,\{3333\},\{abcd\}
[ 	]*[a-f0-9]+:	d6 00 00 b0 c2          	vexp2lutps v0,v2
[ 	]*[a-f0-9]+:	d6 00 01 b0 c2          	vexp2lutps v0\{k1\},v2
[ 	]*[a-f0-9]+:	d6 30 01 b0 c2          	vexp2lutps v0\{k1\},v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 c2          	vexp2lutps v0\{k1\},v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 c2          	vexp2lutps v0\{k1\},v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 5f 07 b0 ff          	vexp2lutps v31\{k7\},v31\{cccc\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 00          	vexp2lutps v0\{k1\},ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 01 b0 00          	vexp2lutps v0\{k1\},ZMMWORD PTR \[r8\]
[ 	]*[a-f0-9]+:	d6 01 01 b0 07          	vexp2lutps v0\{k1\},ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 50 01 b0 00          	vexp2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 80 01 b0 00          	vexp2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 01 b0 00          	vexp2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 d0 01 b0 00          	vexp2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 83 07 b0 8c 08 04 00 00 00 	vexp2lutps v1\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 07 b0 54 08 01    	vexp2lutps v2\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 b0 c2          	vexp2lutps v0\{k7\},v2
[ 	]*[a-f0-9]+:	d6 00 00 b4 c2          	vgetexpps v0,v2
[ 	]*[a-f0-9]+:	d6 00 01 b4 c2          	vgetexpps v0\{k1\},v2
[ 	]*[a-f0-9]+:	d6 30 01 b4 c2          	vgetexpps v0\{k1\},v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 c2          	vgetexpps v0\{k1\},v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 c2          	vgetexpps v0\{k1\},v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 5f 07 b4 ff          	vgetexpps v31\{k7\},v31\{cccc\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 00          	vgetexpps v0\{k1\},ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 01 b4 00          	vgetexpps v0\{k1\},ZMMWORD PTR \[r8\]
[ 	]*[a-f0-9]+:	d6 01 01 b4 07          	vgetexpps v0\{k1\},ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 50 01 b4 00          	vgetexpps v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 80 01 b4 00          	vgetexpps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 01 b4 00          	vgetexpps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 01 b4 00          	vgetexpps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 07 b4 8c 08 04 00 00 00 	vgetexpps v1\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 07 b4 54 08 01    	vgetexpps v2\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 b4 c2          	vgetexpps v0\{k7\},v2
[ 	]*[a-f0-9]+:	d6 00 00 c0 c2          	vrcpresps v0,v2
[ 	]*[a-f0-9]+:	d6 00 01 c0 c2          	vrcpresps v0\{k1\},v2
[ 	]*[a-f0-9]+:	d6 30 01 c0 c2          	vrcpresps v0\{k1\},v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 c2          	vrcpresps v0\{k1\},v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 c2          	vrcpresps v0\{k1\},v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 5f 07 c0 ff          	vrcpresps v31\{k7\},v31\{cccc\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 00          	vrcpresps v0\{k1\},ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 01 c0 00          	vrcpresps v0\{k1\},ZMMWORD PTR \[r8\]
[ 	]*[a-f0-9]+:	d6 01 01 c0 07          	vrcpresps v0\{k1\},ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 50 01 c0 00          	vrcpresps v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 80 01 c0 00          	vrcpresps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 01 c0 00          	vrcpresps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 01 c0 00          	vrcpresps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 07 c0 8c 08 04 00 00 00 	vrcpresps v1\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 07 c0 54 08 01    	vrcpresps v2\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 c0 c2          	vrcpresps v0\{k7\},v2
[ 	]*[a-f0-9]+:	d6 00 00 b2 c2          	vrsqrtlutps v0,v2
[ 	]*[a-f0-9]+:	d6 00 01 b2 c2          	vrsqrtlutps v0\{k1\},v2
[ 	]*[a-f0-9]+:	d6 30 01 b2 c2          	vrsqrtlutps v0\{k1\},v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 c2          	vrsqrtlutps v0\{k1\},v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 c2          	vrsqrtlutps v0\{k1\},v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 5f 07 b2 ff          	vrsqrtlutps v31\{k7\},v31\{cccc\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 00          	vrsqrtlutps v0\{k1\},ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 01 b2 00          	vrsqrtlutps v0\{k1\},ZMMWORD PTR \[r8\]
[ 	]*[a-f0-9]+:	d6 01 01 b2 07          	vrsqrtlutps v0\{k1\},ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 50 01 b2 00          	vrsqrtlutps v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 80 01 b2 00          	vrsqrtlutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 01 b2 00          	vrsqrtlutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 01 b2 00          	vrsqrtlutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 07 b2 8c 08 04 00 00 00 	vrsqrtlutps v1\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 07 b2 54 08 01    	vrsqrtlutps v2\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 b2 c2          	vrsqrtlutps v0\{k7\},v2
[ 	]*[a-f0-9]+:	d6 00 08 55 c2          	vsbbpi v0,k1,v2
[ 	]*[a-f0-9]+:	d6 00 07 55 c2          	vsbbpi v0\{k7\},k0,v2
[ 	]*[a-f0-9]+:	d6 83 17 55 8c 08 04 00 00 00 	vsbbpi v1\{k7\},k2,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 1f 55 54 08 01    	vsbbpi v2\{k7\},k3,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 5d c2          	vsbbrpi v0,k1,v2
[ 	]*[a-f0-9]+:	d6 00 07 5d c2          	vsbbrpi v0\{k7\},k0,v2
[ 	]*[a-f0-9]+:	d6 83 17 5d 8c 08 04 00 00 00 	vsbbrpi v1\{k7\},k2,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 1f 5d 54 08 01    	vsbbrpi v2\{k7\},k3,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 5b c2          	vsubrsetbpi v0,k1,v2
[ 	]*[a-f0-9]+:	d6 00 07 5b c2          	vsubrsetbpi v0\{k7\},k0,v2
[ 	]*[a-f0-9]+:	d6 83 17 5b 8c 08 04 00 00 00 	vsubrsetbpi v1\{k7\},k2,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 1f 5b 54 08 01    	vsubrsetbpi v2\{k7\},k3,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 08 53 c2          	vsubsetbpi v0,k1,v2
[ 	]*[a-f0-9]+:	d6 00 07 53 c2          	vsubsetbpi v0\{k7\},k0,v2
[ 	]*[a-f0-9]+:	d6 83 17 53 8c 08 04 00 00 00 	vsubsetbpi v1\{k7\},k2,ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 1f 53 54 08 01    	vsubsetbpi v2\{k7\},k3,ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 00 b1 c2          	vlog2lutps v0,v2
[ 	]*[a-f0-9]+:	d6 00 01 b1 c2          	vlog2lutps v0\{k1\},v2
[ 	]*[a-f0-9]+:	d6 30 01 b1 c2          	vlog2lutps v0\{k1\},v2\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 c2          	vlog2lutps v0\{k1\},v2\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 c2          	vlog2lutps v0\{k1\},v2\{cccc\}
[ 	]*[a-f0-9]+:	d6 5f 07 b1 ff          	vlog2lutps v31\{k7\},v31\{cccc\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 00          	vlog2lutps v0\{k1\},ZMMWORD PTR \[rax\]
[ 	]*[a-f0-9]+:	d6 01 01 b1 00          	vlog2lutps v0\{k1\},ZMMWORD PTR \[r8\]
[ 	]*[a-f0-9]+:	d6 01 01 b1 07          	vlog2lutps v0\{k1\},ZMMWORD PTR \[r15\]
[ 	]*[a-f0-9]+:	d6 50 01 b1 00          	vlog2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 80 01 b1 00          	vlog2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}
[ 	]*[a-f0-9]+:	d6 d0 01 b1 00          	vlog2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 d0 01 b1 00          	vlog2lutps v0\{k1\},ZMMWORD PTR \[rax\]\{nt\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 83 07 b1 8c 08 04 00 00 00 	vlog2lutps v1\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x4\]\{nt\}
[ 	]*[a-f0-9]+:	d6 83 07 b1 54 08 01    	vlog2lutps v2\{k7\},ZMMWORD PTR \[r8\+r9\*1\+0x40\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 07 b1 c2          	vlog2lutps v0\{k7\},v2
[ 	]*[a-f0-9]+:	d6 08 10 21 08          	vcmppi k1,v2,ZMMWORD PTR \[rax\],le
[ 	]*[a-f0-9]+:	d6 08 12 21 08          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rax\],le
[ 	]*[a-f0-9]+:	d6 08 18 20 08          	vcmpps k1,v3,ZMMWORD PTR \[rax\],le
[ 	]*[a-f0-9]+:	d6 08 48 22 08          	vcmppu k1,v9,ZMMWORD PTR \[rax\],le
[ 	]*[a-f0-9]+:	d6 08 10 21 c9          	vcmppi k1,v2,v1,le
[ 	]*[a-f0-9]+:	d6 08 12 21 ca          	vcmppi k1\{k2\},v2,v2,le
[ 	]*[a-f0-9]+:	d6 08 18 20 cb          	vcmpps k1,v3,v3,le
[ 	]*[a-f0-9]+:	d6 08 48 22 cc          	vcmppu k1,v9,v4,le
[ 	]*[a-f0-9]+:	d6 00 00 10 40 01       	vloadd v0,\[rax\+0x40\]
[ 	]*[a-f0-9]+:	d6 00 00 10 40 ff       	vloadd v0,\[rax-0x40\]
[ 	]*[a-f0-9]+:	66 62 df c8             	spflt  ax
[ 	]*[a-f0-9]+:	62 df c8                	spflt  eax
[ 	]*[a-f0-9]+:	62 cf c8                	spflt  rax
[ 	]*[a-f0-9]+:	66 62 9f cc             	spflt  r12w
[ 	]*[a-f0-9]+:	62 9f cc                	spflt  r12d
[ 	]*[a-f0-9]+:	62 8f cc                	spflt  r12
[ 	]*[a-f0-9]+:	d6 00 01 18 04 98       	vgatherd v0\{k1\},\[rax\+v3\*4\]
[ 	]*[a-f0-9]+:	d6 a0 01 18 04 f0       	vgatherd v0\{k1\},\[rax\+v22\*8\]\{nt\}
[ 	]*[a-f0-9]+:	d6 b0 19 18 44 30 7f    	vgatherd v0\{k1\},\[rax\+v30\*1\+0x7f\]\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 90 01 18 44 50 20    	vgatherd v0\{k1\},\[rax\+v10\*2\+0x80\]\{nt\}
[ 	]*[a-f0-9]+:	d6 90 19 18 84 88 80 00 00 00 	vgatherd v0\{k1\},\[rax\+v9\*4\+0x80\]\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 07 19 04 98       	vgatherpfd \[rax\+v3\*4\]\{k7\}
[ 	]*[a-f0-9]+:	d6 80 07 19 04 d8       	vgatherpfd \[rax\+v3\*8\]\{k7\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 1d 19 44 28 7f    	vgatherpfd \[rax\+v5\*1\+0x7f\]\{k5\}\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 80 04 19 44 70 20    	vgatherpfd \[rax\+v6\*2\+0x80\]\{k4\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 1b 19 84 b8 80 00 00 00 	vgatherpfd \[rax\+v7\*4\+0x80\]\{k3\}\{nt\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 04 01 1c 1c 0d 80 00 00 00 	vscatterd \[v1\*1\+0x80\]\{k1\},v11
[ 	]*[a-f0-9]+:	d6 84 01 1c 1c 10       	vscatterd \[rax\+v2\*1\]\{k1\}\{nt\},v11
[ 	]*[a-f0-9]+:	d6 84 01 1c 14 50       	vscatterd \[rax\+v2\*2\]\{k1\}\{nt\},v10
[ 	]*[a-f0-9]+:	d6 04 01 1c 0c 98       	vscatterd \[rax\+v3\*4\]\{k1\},v9
[ 	]*[a-f0-9]+:	d6 04 29 1c 04 d8       	vscatterd \[rax\+v3\*8\]\{k1\},v8\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1c 3c 20       	vscatterd \[rax\+v4\*1\]\{k1\},v7\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1c 34 68       	vscatterd \[rax\+v5\*2\]\{k1\},v6\{sint8\}
[ 	]*[a-f0-9]+:	d6 80 01 1c 2c b0       	vscatterd \[rax\+v6\*4\]\{k1\}\{nt\},v5
[ 	]*[a-f0-9]+:	d6 80 29 1c 24 f8       	vscatterd \[rax\+v7\*8\]\{k1\}\{nt\},v4\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1c 1c 00       	vscatterd \[rax\+v8\*1\]\{k1\}\{nt\},v3\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1c 14 48       	vscatterd \[rax\+v9\*2\]\{k1\}\{nt\},v2\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1c 0c 90       	vscatterd \[rax\+v10\*4\]\{k1\}\{nt\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 11 1c 04 1d 80 00 00 00 	vscatterd \[v11\*1\+0x80\]\{k1\},v0\{float16rz\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 b0       	vscatterpfd \[rax\+v6\*4\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 80       	vscatterpfd \[rax\+v0\*4\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 08       	vscatterpfd \[rax\+v1\*1\]\{k1\}
[ 	]*[a-f0-9]+:	d6 80 01 1d 04 50       	vscatterpfd \[rax\+v2\*2\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 98       	vscatterpfd \[rax\+v3\*4\]\{k1\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 44 98 20    	vscatterpfd \[rax\+v3\*4\+0x80\]\{k1\}
[ 	]*[a-f0-9]+:	d6 00 01 1d 04 0d 80 00 00 00 	vscatterpfd \[v1\*1\+0x80\]\{k1\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 d8       	vscatterpfd \[rax\+v3\*8\]\{k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 20       	vscatterpfd \[rax\+v4\*1\]\{k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 68       	vscatterpfd \[rax\+v5\*2\]\{k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 1d 04 f8       	vscatterpfd \[rax\+v7\*8\]\{k1\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 00       	vscatterpfd \[rax\+v8\*1\]\{k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 48       	vscatterpfd \[rax\+v9\*2\]\{k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 29 1d 04 90       	vscatterpfd \[rax\+v10\*4\]\{k1\}\{nt\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 90 11 1d 44 d8 40    	vscatterpfd \[rax\+v11\*8\+0x80\]\{k1\}\{nt\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 04       	vloadd v1\{k1\},\[rcx\+0x40\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 10 49 10       	vloadd v1\{k1\},\[rcx\+0x100\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 01       	vloadd v1\{k1\},\[rcx\+0x40\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 99 10 49 04       	vloadd v1\{k1\},\[rcx\+0x100\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 01       	vloadd v1\{k1\},\[rcx\+0x40\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 49 04       	vloadd v1\{k1\},\[rcx\+0x100\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 10       	vloadd v1\{k1\},\[rcx\+0x40\]\{4to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 29 10 49 40       	vloadd v1\{k1\},\[rcx\+0x100\]\{4to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 04       	vloadd v1\{k1\},\[rcx\+0x40\]\{4to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 20 99 10 49 10       	vloadd v1\{k1\},\[rcx\+0x100\]\{4to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 04       	vloadd v1\{k1\},\[rcx\+0x40\]\{4to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 49 10       	vloadd v1\{k1\},\[rcx\+0x100\]\{4to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 29 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 13 89 00 01 00 00 	vloadunpackhd v1\{k1\},\[rcx\+0x100\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 20       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 49 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 10       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 99 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x100\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 10       	vloadunpackhd v1\{k1\},\[rcx\+0x40\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 49 40       	vloadunpackhd v1\{k1\},\[rcx\+0x100\]\{float11A\}
#pass

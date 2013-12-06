#as: -march=l1om
#objdump: -dw
#name: L1OMni Mixed L1OMni and non-L1OMni insns

.*: +file format .*

Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	4c 89 64 24 f8          	mov    %r12,-0x8\(%rsp\)
[ 	]*[a-f0-9]+:	62 ee d8                	vkxor  %k3,%k0
[ 	]*[a-f0-9]+:	48 89 5c 24 e8          	mov    %rbx,-0x18\(%rsp\)
[ 	]*[a-f0-9]+:	62 cd cd                	vkand  %k1,%k5
[ 	]*[a-f0-9]+:	48 83 3d 00 00 00 00 00 	cmpq   \$0x0,0x0\(%rip\)        # 18 <_start\+0x18>
[ 	]*[a-f0-9]+:	62 dc d0                	vkmov  %k2,%eax
[ 	]*[a-f0-9]+:	48 8b 1c 24             	mov    \(%rsp\),%rbx
[ 	]*[a-f0-9]+:	62 dc fb                	vkmov  %k7,%ebx
[ 	]*[a-f0-9]+:	4c 8b 64 24 10          	mov    0x10\(%rsp\),%r12
[ 	]*[a-f0-9]+:	62 dc e4                	vkmov  %k4,%esp
[ 	]*[a-f0-9]+:	c3                      	retq   
[ 	]*[a-f0-9]+:	62 ec fe                	vkmov  %edi,%k6
[ 	]*[a-f0-9]+:	48 89 c3                	mov    %rax,%rbx
[ 	]*[a-f0-9]+:	66 62 cc c2             	vkmov  %r8w,%k2
[ 	]*[a-f0-9]+:	48 89 ee                	mov    %rbp,%rsi
[ 	]*[a-f0-9]+:	66 62 cc d4             	vkmov  %r10w,%k4
[ 	]*[a-f0-9]+:	48 89 c7                	mov    %rax,%rdi
[ 	]*[a-f0-9]+:	66 62 f4 d9             	bitinterleave11 %bx,%cx
[ 	]*[a-f0-9]+:	48 89 df                	mov    %rbx,%rdi
[ 	]*[a-f0-9]+:	62 a5 d7                	bitinterleave21 %rdx,%r15
[ 	]*[a-f0-9]+:	48 89 58 38             	mov    %rbx,0x38\(%rax\)
[ 	]*[a-f0-9]+:	62 f5 cd                	bitinterleave21 %ecx,%ebp
[ 	]*[a-f0-9]+:	48 8b 6c 24 08          	mov    0x8\(%rsp\),%rbp
[ 	]*[a-f0-9]+:	45 0f bd c5             	bsr    %r13d,%r8d
[ 	]*[a-f0-9]+:	48 83 c4 18             	add    \$0x18,%rsp
[ 	]*[a-f0-9]+:	48 62 d0 de             	rex.W bsfi %r11d,%esi
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	66 62 f1 d9             	bsri   %bx,%cx
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	62 6b 44 ea c0          	clevict1 -0x40\(%rdx,%r13,8\)
[ 	]*[a-f0-9]+:	be 00 00 00 00          	mov    \$0x0,%esi
[ 	]*[a-f0-9]+:	62 e8 c4                	countbits %rax,%rsp
[ 	]*[a-f0-9]+:	ba 05 00 00 00          	mov    \$0x5,%edx
[ 	]*[a-f0-9]+:	62 df c0                	delay  %eax
[ 	]*[a-f0-9]+:	48 89 de                	mov    %rbx,%rsi
[ 	]*[a-f0-9]+:	66 62 f9 c1             	quadmask16 %ax,%cx
[ 	]*[a-f0-9]+:	5b                      	pop    %rbx
[ 	]*[a-f0-9]+:	62 d9 c8                	quadmask16 %r9d,%eax
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	62 e9 c3                	quadmask16 %rax,%rbx
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	66 62 b7 d7 ff 5f 03 07 	rotatefield \$0x7,\$0x3,\$0x5fff,%dx,%r15w
[ 	]*[a-f0-9]+:	be 00 00 00 00          	mov    \$0x0,%esi
[ 	]*[a-f0-9]+:	62 f7 d7 0c 00 00 01    	rotatefield \$0x1,\$0x0,\$0xc,%edx,%edi
[ 	]*[a-f0-9]+:	ba 05 00 00 00          	mov    \$0x5,%edx
[ 	]*[a-f0-9]+:	67 62 cb a8 13 00 00 00 	vprefetch1 \$0x5,0x13\(%eax\)
[ 	]*[a-f0-9]+:	48 89 de                	mov    %rbx,%rsi
[ 	]*[a-f0-9]+:	62 cb 9c 59 a0 ff ff ff 	vprefetch1 \$0x3,-0x60\(%rcx,%rbx,2\)
[ 	]*[a-f0-9]+:	5b                      	pop    %rbx
[ 	]*[a-f0-9]+:	d6 00 08 9e d8          	vmaxabsps %v0,%v1,%v3
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	d6 00 08 45 ec          	vadcpi %v4,%k1,%v5
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	d6 00 32 48 b8 ff 00 00 00 	vaddnps 0xff\(%rax\),%v6,%v7\{%k2\}
[ 	]*[a-f0-9]+:	48 89 fd                	mov    %rdi,%rbp
[ 	]*[a-f0-9]+:	d6 54 4b 41 10          	vaddpi \(%rax\)\{sint8\},%v9,%v10\{%k3\}
[ 	]*[a-f0-9]+:	48 83 ec 08             	sub    \$0x8,%rsp
[ 	]*[a-f0-9]+:	d6 84 5c 40 a4 13 f4 ff ff ff 	vaddps -0xc\(%rbx,%rdx,1\)\{nt\},%v11,%v12\{%k4\}
[ 	]*[a-f0-9]+:	48 8b 7b 08             	mov    0x8\(%rbx\),%rdi
[ 	]*[a-f0-9]+:	d6 a6 28 43 2c 00       	vaddsetcpi \(%rax,%r8,1\)\{nt\}\{4to16\},%k5,%v13
[ 	]*[a-f0-9]+:	85 c0                   	test   %eax,%eax
[ 	]*[a-f0-9]+:	d6 09 7e a4 c6          	vandnpi %v14,%v15,%v16\{%k6\}
[ 	]*[a-f0-9]+:	48 85 db                	test   %rbx,%rbx
[ 	]*[a-f0-9]+:	67 d6 fa 88 a5 94 c8 67 45 23 01 	vandpi 0x1234567\(%eax,%r9d,8\)\{nt\}\{sint16\},%v17,%v18
[ 	]*[a-f0-9]+:	48 89 c3                	mov    %rax,%rbx
[ 	]*[a-f0-9]+:	d6 59 9f 86 24 4b       	vclampzps \(%r11,%rcx,2\)\{unorm8\},%v19,%v20\{%k7\}
[ 	]*[a-f0-9]+:	48 89 ef                	mov    %rbp,%rdi
[ 	]*[a-f0-9]+:	d6 80 c2 25 be f3 ff ff ff 	vcmppi \{neq\},-0xd\(%rsi\)\{nt\},%v24,%k7\{%k2\}
[ 	]*[a-f0-9]+:	48 83 c4 08             	add    \$0x8,%rsp
[ 	]*[a-f0-9]+:	67 d6 9f c9 20 b4 10 02 00 00 00 	vcmpps \{unord\},0x2\(%r8d,%r10d,1\)\{nt\}\{1to16\},%v25,%k6\{%k1\}
[ 	]*[a-f0-9]+:	5d                      	pop    %rbp
[ 	]*[a-f0-9]+:	d6 8c 10 9b 94 7e 10 20 30 40 	vcvtinsps2f11 \$0x2,\{rn\},0x40302010\(%rsi,%rdi,2\)\{nt\},%v26
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	d6 d4 21 95 ac f8 40 30 20 10 	vcvtpi2ps \{.16b\},0x10203040\(%rax,%rdi,8\)\{nt\}\{sint8\},%v13\{%k1\}
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	d6 dc 2b 91 9c f8 40 30 20 10 	vcvtps2pi \{.24b\},\{rn\},0x10203040\(%rax,%rdi,8\)\{nt\}\{unorm8\},%v27\{%k3\}
[ 	]*[a-f0-9]+:	48 8b 3d 00 00 00 00    	mov    0x0\(%rip\),%rdi        # 157 <_start\+0x157>
[ 	]*[a-f0-9]+:	67 d6 8c 05 9d a7 20 10 00 00 	vcvtps2srgb8 0x1020\(%edi\)\{nt\},%v28\{%k5\}
[ 	]*[a-f0-9]+:	48 83 c4 08             	add    \$0x8,%rsp
[ 	]*[a-f0-9]+:	d6 04 f8 f0 a8 f9 ff ff ff ff ff 	vfixupps \$0x1fffff,-0x7\(%rax\),%v13
[ 	]*[a-f0-9]+:	66 90                   	xchg   %ax,%ax
[ 	]*[a-f0-9]+:	d6 2c 01 10 28          	vloadd \(%rax\)\{4to16\},%v29\{%k1\}
[ 	]*[a-f0-9]+:	48 89 fd                	mov    %rdi,%rbp
[ 	]*[a-f0-9]+:	d6 84 49 10 ac f8 40 30 00 00 	vloadd 0x3040\(%rax,%rdi,8\)\{nt\}\{sint16\},%v13\{%k1\}
[ 	]*[a-f0-9]+:	48 83 ec 18             	sub    \$0x18,%rsp
[ 	]*[a-f0-9]+:	67 d6 8c 03 f3 b4 c8 fb ff ff ff 0f 10 	vrotatefieldpi \$0x2,\$0x0,\$0xf,-0x5\(%eax,%ecx,8\)\{nt\},%v30\{%k3\}
[ 	]*[a-f0-9]+:	85 c0                   	test   %eax,%eax
[ 	]*[a-f0-9]+:	d6 84 11 14 8c f8 40 30 20 10 	vstored %v9\{float16rz\},0x10203040\(%rax,%rdi,8\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	48 83 fa 00             	cmp    \$0x0,%rdx
[ 	]*[a-f0-9]+:	d6 88 71 14 8f 20 10 00 00 	vstored %v17\{uint16i\},0x1020\(%rdi\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	48 8d 74 24 14          	lea    0x14\(%rsp\),%rsi
[ 	]*[a-f0-9]+:	d6 88 58 14 a8 f4 00 00 00 	vstored %v21\{snorm16\},0xf4\(%rax\)\{nt\}
#pass

#as: -march=k1om
#objdump: -drwMintel
#name: LRB2 branch (Intel disassembly)
#source: k1omni-branch.s

.*: +file format .*

Disassembly of section .text:

0+ <.text>:
[ 	]*[a-f0-9]+:	b8 00 c4 e0 40       	mov    eax,0x40e0c400
[ 	]*[a-f0-9]+:	0f 85 00 00 00 00    	jne    0xb	7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c0 85 00 00 00 00 	jknzd  k7,0x12	e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 48 75 e9       	jknzd  k6,0x0
[ 	]*[a-f0-9]+:	c4 e0 78 75 e4       	jknzd  k0,0x0
[ 	]*[a-f0-9]+:	c5 f0 85 00 00 00 00 	jknzd  k1,0x23	1f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 e8 84 00 00 00 00 	jkzd   k2,0x2a	26: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 60 74 d1       	jkzd   k3,0x0
[ 	]*[a-f0-9]+:	c4 e0 58 74 cc       	jkzd   k4,0x0
[ 	]*[a-f0-9]+:	c5 d0 84 00 00 00 00 	jkzd   k5,0x3b	37: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c8 85 89 90 90 00 	jknzd  k6,0x9090cb
[ 	]*[a-f0-9]+:	c5 c8 84 89 90 90 00 	jkzd   k6,0x9090d2
[ 	]*[a-f0-9]+:	b8 00 c4 e0 40       	mov    eax,0x40e0c400
[ 	]*[a-f0-9]+:	0f 85 00 00 00 00    	jne    0x54	50: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c0 85 00 00 00 00 	jknzd  k7,0x5b	57: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 48 75 a0       	jknzd  k6,0x0
[ 	]*[a-f0-9]+:	c4 e0 78 75 9b       	jknzd  k0,0x0
[ 	]*[a-f0-9]+:	c5 f0 85 00 00 00 00 	jknzd  k1,0x6c	68: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 e8 84 00 00 00 00 	jkzd   k2,0x73	6f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 60 74 88       	jkzd   k3,0x0
[ 	]*[a-f0-9]+:	c4 e0 58 74 83       	jkzd   k4,0x0
[ 	]*[a-f0-9]+:	c5 d0 84 00 00 00 00 	jkzd   k5,0x84	80: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c8 85 89 90 90 00 	jknzd  k6,0x909114
[ 	]*[a-f0-9]+:	c5 c8 84 89 90 90 00 	jkzd   k6,0x90911b
[ 	]*[a-f0-9]+:	90                   	nop
#pass

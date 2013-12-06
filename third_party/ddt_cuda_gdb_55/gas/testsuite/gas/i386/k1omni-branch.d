#as: -march=k1om
#objdump: -drw
#name: LRB2 branch

.*: +file format .*

Disassembly of section .text:

0+ <.text>:
[ 	]*[a-f0-9]+:	b8 00 c4 e0 40       	mov    \$0x40e0c400,%eax
[ 	]*[a-f0-9]+:	0f 85 00 00 00 00    	jne    0xb	7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c0 85 00 00 00 00 	jknzd  0x12,%k7	e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 48 75 e9       	jknzd  0x0,%k6
[ 	]*[a-f0-9]+:	c4 e0 78 75 e4       	jknzd  0x0,%k0
[ 	]*[a-f0-9]+:	c5 f0 85 00 00 00 00 	jknzd  0x23,%k1	1f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 e8 84 00 00 00 00 	jkzd   0x2a,%k2	26: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 60 74 d1       	jkzd   0x0,%k3
[ 	]*[a-f0-9]+:	c4 e0 58 74 cc       	jkzd   0x0,%k4
[ 	]*[a-f0-9]+:	c5 d0 84 00 00 00 00 	jkzd   0x3b,%k5	37: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c8 85 89 90 90 00 	jknzd  0x9090cb,%k6
[ 	]*[a-f0-9]+:	c5 c8 84 89 90 90 00 	jkzd   0x9090d2,%k6
[ 	]*[a-f0-9]+:	b8 00 c4 e0 40       	mov    \$0x40e0c400,%eax
[ 	]*[a-f0-9]+:	0f 85 00 00 00 00    	jne    0x54	50: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c0 85 00 00 00 00 	jknzd  0x5b,%k7	57: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 48 75 a0       	jknzd  0x0,%k6
[ 	]*[a-f0-9]+:	c4 e0 78 75 9b       	jknzd  0x0,%k0
[ 	]*[a-f0-9]+:	c5 f0 85 00 00 00 00 	jknzd  0x6c,%k1	68: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 e8 84 00 00 00 00 	jkzd   0x73,%k2	6f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c4 e0 60 74 88       	jkzd   0x0,%k3
[ 	]*[a-f0-9]+:	c4 e0 58 74 83       	jkzd   0x0,%k4
[ 	]*[a-f0-9]+:	c5 d0 84 00 00 00 00 	jkzd   0x84,%k5	80: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	c5 c8 85 89 90 90 00 	jknzd  0x909114,%k6
[ 	]*[a-f0-9]+:	c5 c8 84 89 90 90 00 	jkzd   0x90911b,%k6
[ 	]*[a-f0-9]+:	90                   	nop
#pass

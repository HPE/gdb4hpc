#as: -march=l1om
#objdump: -drw
#name: L1OMni rip addressing

.*: +file format .*

Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	d6 00 09 45 05 00 00 00 00 	vadcpi 0x0\(%rip\),%k1,%v0\{%k1\}        # 9 <_start\+0x9>	5: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 09 45 05 00 00 00 00 	vadcpi 0x0\(%rip\)\{nt\},%k1,%v0\{%k1\}        # 12 <_start\+0x12>	e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 45 05 00 00 00 00 	vadcpi 0x0\(%rip\)\{nt\}\{sint8\},%k1,%v0\{%k1\}        # 1b <_start\+0x1b>	17: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 09 45 05 00 00 00 00 	vadcpi 0x0\(%rip\)\{sint8\},%k1,%v0\{%k1\}        # 24 <_start\+0x24>	20: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 45 05 00 00 00 00 	vadcpi 0x0\(%rip\)\{nt\}\{sint8\},%k1,%v0\{%k1\}        # 2d <_start\+0x2d>	29: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 10 0f 48 05 00 00 00 00 	vaddnpd 0x0\(%rip\)\{1to8\},%v1,%v0\{%k7\}        # 37 <_start\+0x37>	33: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 20 0f 48 05 00 00 00 00 	vaddnpd 0x0\(%rip\)\{4to8\},%v1,%v0\{%k7\}        # 41 <_start\+0x41>	3d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 00 0f 48 05 00 00 00 00 	vaddnpd 0x0\(%rip\),%v1,%v0\{%k7\}        # 4b <_start\+0x4b>	47: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 0f 48 05 00 00 00 00 	vaddnpd 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 55 <_start\+0x55>	51: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 0f 48 05 00 00 00 00 	vaddnpd 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 5f <_start\+0x5f>	5b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 0f 48 05 00 00 00 00 	vaddnpd 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 69 <_start\+0x69>	65: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 00 0f 48 05 00 00 00 00 	vaddnpd 0x0\(%rip\),%v1,%v0\{%k7\}        # 73 <_start\+0x73>	6f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 10 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{1to16\},%v1,%v0\{%k7\}        # 7c <_start\+0x7c>	78: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{4to16\},%v1,%v0\{%k7\}        # 85 <_start\+0x85>	81: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 60 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{float16\},%v1,%v0\{%k7\}        # 8e <_start\+0x8e>	8a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}        # 97 <_start\+0x97>	93: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # a0 <_start\+0xa0>	9c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # a9 <_start\+0xa9>	a5: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # b2 <_start\+0xb2>	ae: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 70 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{sint16\},%v1,%v0\{%k7\}        # bb <_start\+0xbb>	b7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 40 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{uint8\},%v1,%v0\{%k7\}        # c4 <_start\+0xc4>	c0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}        # cd <_start\+0xcd>	c9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\)\{unorm8\},%v1,%v0\{%k7\}        # d6 <_start\+0xd6>	d2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 0f 48 05 00 00 00 00 	vaddnps 0x0\(%rip\),%v1,%v0\{%k7\}        # df <_start\+0xdf>	db: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 10 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{1to16\},%v1,%v0\{%k7\}        # e8 <_start\+0xe8>	e4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{4to16\},%v1,%v0\{%k7\}        # f1 <_start\+0xf1>	ed: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{nt\}\{sint8\},%v1,%v0\{%k7\}        # fa <_start\+0xfa>	f6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 103 <_start\+0x103>	ff: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 10c <_start\+0x10c>	108: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 115 <_start\+0x115>	111: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 70 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{sint16\},%v1,%v0\{%k7\}        # 11e <_start\+0x11e>	11a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{nt\}\{sint8\},%v1,%v0\{%k7\}        # 127 <_start\+0x127>	123: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{sint8\},%v1,%v0\{%k7\}        # 130 <_start\+0x130>	12c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 60 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{uint16\},%v1,%v0\{%k7\}        # 139 <_start\+0x139>	135: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 40 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\)\{uint8\},%v1,%v0\{%k7\}        # 142 <_start\+0x142>	13e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 0f 41 05 00 00 00 00 	vaddpi 0x0\(%rip\),%v1,%v0\{%k7\}        # 14b <_start\+0x14b>	147: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 17 43 0d 00 00 00 00 	vaddsetcpi 0x0\(%rip\)\{nt\},%k2,%v1\{%k7\}        # 154 <_start\+0x154>	150: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 1f 43 15 00 00 00 00 	vaddsetcpi 0x0\(%rip\)\{nt\},%k3,%v2\{%k7\}        # 15d <_start\+0x15d>	159: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 21 0d 00 00 00 00 	vcmppi \{eq\},0x0\(%rip\)\{nt\}\{sint8\},%v0,%k1\{%k1\}        # 166 <_start\+0x166>	162: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 21 0d 00 00 00 00 	vcmppi \{eq\},0x0\(%rip\)\{nt\},%v0,%k1\{%k1\}        # 16f <_start\+0x16f>	16b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 21 0d 00 00 00 00 	vcmppi \{eq\},0x0\(%rip\)\{nt\}\{sint8\},%v0,%k1\{%k1\}        # 178 <_start\+0x178>	174: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 21 0d 00 00 00 00 	vcmppi \{eq\},0x0\(%rip\)\{sint8\},%v0,%k1\{%k1\}        # 181 <_start\+0x181>	17d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 21 0d 00 00 00 00 	vcmppi \{eq\},0x0\(%rip\),%v0,%k1\{%k1\}        # 18a <_start\+0x18a>	186: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 01 21 0d 00 00 00 00 	vcmppi \{le\},0x0\(%rip\),%v0,%k1\{%k1\}        # 193 <_start\+0x193>	18f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 10 21 0d 00 00 00 00 	vcmppi \{le\},0x0\(%rip\),%v2,%k1        # 19c <_start\+0x19c>	198: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 12 21 0d 00 00 00 00 	vcmppi \{le\},0x0\(%rip\),%v2,%k1\{%k2\}        # 1a5 <_start\+0x1a5>	1a1: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 01 21 0d 00 00 00 00 	vcmppi \{lt\},0x0\(%rip\),%v0,%k1\{%k1\}        # 1ae <_start\+0x1ae>	1aa: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 25 0d 00 00 00 00 	vcmppi \{neq\},0x0\(%rip\),%v0,%k1\{%k1\}        # 1b7 <_start\+0x1b7>	1b3: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 01 25 0d 00 00 00 00 	vcmppi \{nle\},0x0\(%rip\),%v0,%k1\{%k1\}        # 1c0 <_start\+0x1c0>	1bc: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 01 25 0d 00 00 00 00 	vcmppi \{nlt\},0x0\(%rip\),%v0,%k1\{%k1\}        # 1c9 <_start\+0x1c9>	1c5: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 20 0d 00 00 00 00 	vcmpps \{eq\},0x0\(%rip\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}        # 1d2 <_start\+0x1d2>	1ce: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 20 0d 00 00 00 00 	vcmpps \{eq\},0x0\(%rip\)\{nt\},%v0,%k1\{%k1\}        # 1db <_start\+0x1db>	1d7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 20 0d 00 00 00 00 	vcmpps \{eq\},0x0\(%rip\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}        # 1e4 <_start\+0x1e4>	1e0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 20 0d 00 00 00 00 	vcmpps \{eq\},0x0\(%rip\)\{unorm8\},%v0,%k1\{%k1\}        # 1ed <_start\+0x1ed>	1e9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 20 0d 00 00 00 00 	vcmpps \{eq\},0x0\(%rip\),%v0,%k1\{%k1\}        # 1f6 <_start\+0x1f6>	1f2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 01 20 0d 00 00 00 00 	vcmpps \{le\},0x0\(%rip\),%v0,%k1\{%k1\}        # 1ff <_start\+0x1ff>	1fb: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 18 20 0d 00 00 00 00 	vcmpps \{le\},0x0\(%rip\),%v3,%k1        # 208 <_start\+0x208>	204: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 01 20 0d 00 00 00 00 	vcmpps \{lt\},0x0\(%rip\),%v0,%k1\{%k1\}        # 211 <_start\+0x211>	20d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 24 0d 00 00 00 00 	vcmpps \{neq\},0x0\(%rip\),%v0,%k1\{%k1\}        # 21a <_start\+0x21a>	216: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 01 24 0d 00 00 00 00 	vcmpps \{nle\},0x0\(%rip\),%v0,%k1\{%k1\}        # 223 <_start\+0x223>	21f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 01 24 0d 00 00 00 00 	vcmpps \{nlt\},0x0\(%rip\),%v0,%k1\{%k1\}        # 22c <_start\+0x22c>	228: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 dc 01 24 0d 00 00 00 00 	vcmpps \{ord\},0x0\(%rip\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}        # 235 <_start\+0x235>	231: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 dc 01 20 0d 00 00 00 00 	vcmpps \{unord\},0x0\(%rip\)\{nt\}\{unorm8\},%v0,%k1\{%k1\}        # 23e <_start\+0x23e>	23a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 22 0d 00 00 00 00 	vcmppu \{eq\},0x0\(%rip\)\{nt\}\{sint8\},%v0,%k1\{%k1\}        # 247 <_start\+0x247>	243: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 22 0d 00 00 00 00 	vcmppu \{eq\},0x0\(%rip\)\{nt\},%v0,%k1\{%k1\}        # 250 <_start\+0x250>	24c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 22 0d 00 00 00 00 	vcmppu \{eq\},0x0\(%rip\)\{nt\}\{sint8\},%v0,%k1\{%k1\}        # 259 <_start\+0x259>	255: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 22 0d 00 00 00 00 	vcmppu \{eq\},0x0\(%rip\)\{sint8\},%v0,%k1\{%k1\}        # 262 <_start\+0x262>	25e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 22 0d 00 00 00 00 	vcmppu \{eq\},0x0\(%rip\),%v0,%k1\{%k1\}        # 26b <_start\+0x26b>	267: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 01 22 0d 00 00 00 00 	vcmppu \{le\},0x0\(%rip\),%v0,%k1\{%k1\}        # 274 <_start\+0x274>	270: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 48 22 0d 00 00 00 00 	vcmppu \{le\},0x0\(%rip\),%v9,%k1        # 27d <_start\+0x27d>	279: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 01 22 0d 00 00 00 00 	vcmppu \{lt\},0x0\(%rip\),%v0,%k1\{%k1\}        # 286 <_start\+0x286>	282: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 26 0d 00 00 00 00 	vcmppu \{neq\},0x0\(%rip\),%v0,%k1\{%k1\}        # 28f <_start\+0x28f>	28b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 01 26 0d 00 00 00 00 	vcmppu \{nle\},0x0\(%rip\),%v0,%k1\{%k1\}        # 298 <_start\+0x298>	294: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 01 26 0d 00 00 00 00 	vcmppu \{nlt\},0x0\(%rip\),%v0,%k1\{%k1\}        # 2a1 <_start\+0x2a1>	29d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x0,\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 2aa <_start\+0x2aa>	2a6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x0,\{rn\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 2b3 <_start\+0x2b3>	2af: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x0,\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 2bc <_start\+0x2bc>	2b8: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x0,\{rn\},0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 2c5 <_start\+0x2c5>	2c1: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x0,\{rn\},0x0\(%rip\),%v0\{%k1\}        # 2ce <_start\+0x2ce>	2ca: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 c1 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x0,\{rz\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 2d7 <_start\+0x2d7>	2d3: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x1,\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 2e0 <_start\+0x2e0>	2dc: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 11 9b 05 00 00 00 00 	vcvtinsps2f11 \$0x2,\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 2e9 <_start\+0x2e9>	2e5: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 9a 05 00 00 00 00 	vcvtinsps2u10 \$0x0,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 2f2 <_start\+0x2f2>	2ee: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 9a 05 00 00 00 00 	vcvtinsps2u10 \$0x0,0x0\(%rip\)\{nt\},%v0\{%k1\}        # 2fb <_start\+0x2fb>	2f7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 9a 05 00 00 00 00 	vcvtinsps2u10 \$0x0,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 304 <_start\+0x304>	300: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 9a 05 00 00 00 00 	vcvtinsps2u10 \$0x0,0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 30d <_start\+0x30d>	309: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 9a 05 00 00 00 00 	vcvtinsps2u10 \$0x0,0x0\(%rip\),%v0\{%k1\}        # 316 <_start\+0x316>	312: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 9a 05 00 00 00 00 	vcvtinsps2u10 \$0x1,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 31f <_start\+0x31f>	31b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 11 9a 05 00 00 00 00 	vcvtinsps2u10 \$0x2,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 328 <_start\+0x328>	324: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 41 91 05 00 00 00 00 	vcvtpd2pi \$0x0,\{rd\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 332 <_start\+0x332>	32e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 01 91 05 00 00 00 00 	vcvtpd2pi \$0x0,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 33c <_start\+0x33c>	338: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 81 91 05 00 00 00 00 	vcvtpd2pi \$0x0,\{ru\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 346 <_start\+0x346>	342: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 c1 91 05 00 00 00 00 	vcvtpd2pi \$0x0,\{rz\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 350 <_start\+0x350>	34c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 09 91 05 00 00 00 00 	vcvtpd2pi \$0x1,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 35a <_start\+0x35a>	356: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 10 09 91 05 00 00 00 00 	vcvtpd2pi \$0x1,\{rn\},0x0\(%rip\)\{1to8\},%v0\{%k1\}        # 364 <_start\+0x364>	360: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 09 91 05 00 00 00 00 	vcvtpd2pi \$0x1,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 36e <_start\+0x36e>	36a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 09 91 05 00 00 00 00 	vcvtpd2pi \$0x1,\{rn\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 378 <_start\+0x378>	374: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 00 09 91 05 00 00 00 00 	vcvtpd2pi \$0x1,\{rn\},0x0\(%rip\),%v0\{%k1\}        # 382 <_start\+0x382>	37e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 41 90 05 00 00 00 00 	vcvtpd2ps \$0x0,\{rd\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 38c <_start\+0x38c>	388: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 01 90 05 00 00 00 00 	vcvtpd2ps \$0x0,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 396 <_start\+0x396>	392: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 81 90 05 00 00 00 00 	vcvtpd2ps \$0x0,\{ru\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 3a0 <_start\+0x3a0>	39c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 c1 90 05 00 00 00 00 	vcvtpd2ps \$0x0,\{rz\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 3aa <_start\+0x3aa>	3a6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 09 90 05 00 00 00 00 	vcvtpd2ps \$0x1,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 3b4 <_start\+0x3b4>	3b0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 10 09 90 05 00 00 00 00 	vcvtpd2ps \$0x1,\{rn\},0x0\(%rip\)\{1to8\},%v0\{%k1\}        # 3be <_start\+0x3be>	3ba: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 09 90 05 00 00 00 00 	vcvtpd2ps \$0x1,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 3c8 <_start\+0x3c8>	3c4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 09 90 05 00 00 00 00 	vcvtpd2ps \$0x1,\{rn\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 3d2 <_start\+0x3d2>	3ce: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 00 09 90 05 00 00 00 00 	vcvtpd2ps \$0x1,\{rn\},0x0\(%rip\),%v0\{%k1\}        # 3dc <_start\+0x3dc>	3d8: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 41 92 05 00 00 00 00 	vcvtpd2pu \$0x0,\{rd\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 3e6 <_start\+0x3e6>	3e2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 01 92 05 00 00 00 00 	vcvtpd2pu \$0x0,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 3f0 <_start\+0x3f0>	3ec: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 81 92 05 00 00 00 00 	vcvtpd2pu \$0x0,\{ru\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 3fa <_start\+0x3fa>	3f6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 c1 92 05 00 00 00 00 	vcvtpd2pu \$0x0,\{rz\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 404 <_start\+0x404>	400: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 09 92 05 00 00 00 00 	vcvtpd2pu \$0x1,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 40e <_start\+0x40e>	40a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 10 09 92 05 00 00 00 00 	vcvtpd2pu \$0x1,\{rn\},0x0\(%rip\)\{1to8\},%v0\{%k1\}        # 418 <_start\+0x418>	414: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 90 09 92 05 00 00 00 00 	vcvtpd2pu \$0x1,\{rn\},0x0\(%rip\)\{nt\}\{1to8\},%v0\{%k1\}        # 422 <_start\+0x422>	41e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 09 92 05 00 00 00 00 	vcvtpd2pu \$0x1,\{rn\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 42c <_start\+0x42c>	428: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 00 09 92 05 00 00 00 00 	vcvtpd2pu \$0x1,\{rn\},0x0\(%rip\),%v0\{%k1\}        # 436 <_start\+0x436>	432: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 58 08 95 1d 00 00 00 00 	vcvtpi2pd \$0x1,0x0\(%rip\)\{sint8\},%v19        # 440 <_start\+0x440>	43c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 95 05 00 00 00 00 	vcvtpi2ps \{.0b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 449 <_start\+0x449>	445: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 95 05 00 00 00 00 	vcvtpi2ps \{.0b\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 452 <_start\+0x452>	44e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 95 05 00 00 00 00 	vcvtpi2ps \{.0b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 45b <_start\+0x45b>	457: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 95 05 00 00 00 00 	vcvtpi2ps \{.0b\},0x0\(%rip\)\{sint8\},%v0\{%k1\}        # 464 <_start\+0x464>	460: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 95 05 00 00 00 00 	vcvtpi2ps \{.0b\},0x0\(%rip\),%v0\{%k1\}        # 46d <_start\+0x46d>	469: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 21 95 05 00 00 00 00 	vcvtpi2ps \{.16b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 476 <_start\+0x476>	472: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 29 95 05 00 00 00 00 	vcvtpi2ps \{.24b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 47f <_start\+0x47f>	47b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 31 95 05 00 00 00 00 	vcvtpi2ps \{.31b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 488 <_start\+0x488>	484: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 39 95 05 00 00 00 00 	vcvtpi2ps \{.32b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 491 <_start\+0x491>	48d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 95 05 00 00 00 00 	vcvtpi2ps \{.4b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 49a <_start\+0x49a>	496: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 11 95 05 00 00 00 00 	vcvtpi2ps \{.5b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 4a3 <_start\+0x4a3>	49f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 19 95 05 00 00 00 00 	vcvtpi2ps \{.8b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 4ac <_start\+0x4ac>	4a8: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 48 08 94 1d 00 00 00 00 	vcvtps2pd \$0x1,0x0\(%rip\)\{uint8\},%v19        # 4b6 <_start\+0x4b6>	4b2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 91 05 00 00 00 00 	vcvtps2pi \{.0b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 4bf <_start\+0x4bf>	4bb: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 91 05 00 00 00 00 	vcvtps2pi \{.0b\},\{rn\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 4c8 <_start\+0x4c8>	4c4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 91 05 00 00 00 00 	vcvtps2pi \{.0b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 4d1 <_start\+0x4d1>	4cd: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 91 05 00 00 00 00 	vcvtps2pi \{.0b\},\{rn\},0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 4da <_start\+0x4da>	4d6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 91 05 00 00 00 00 	vcvtps2pi \{.0b\},\{rn\},0x0\(%rip\),%v0\{%k1\}        # 4e3 <_start\+0x4e3>	4df: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 21 91 05 00 00 00 00 	vcvtps2pi \{.16b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 4ec <_start\+0x4ec>	4e8: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 29 91 05 00 00 00 00 	vcvtps2pi \{.24b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 4f5 <_start\+0x4f5>	4f1: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 31 91 05 00 00 00 00 	vcvtps2pi \{.31b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 4fe <_start\+0x4fe>	4fa: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 79 91 05 00 00 00 00 	vcvtps2pi \{.32b\},\{rd\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 507 <_start\+0x507>	503: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 39 91 05 00 00 00 00 	vcvtps2pi \{.32b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 510 <_start\+0x510>	50c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 b9 91 05 00 00 00 00 	vcvtps2pi \{.32b\},\{ru\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 519 <_start\+0x519>	515: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 f9 91 05 00 00 00 00 	vcvtps2pi \{.32b\},\{rz\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 522 <_start\+0x522>	51e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 91 05 00 00 00 00 	vcvtps2pi \{.4b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 52b <_start\+0x52b>	527: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 11 91 05 00 00 00 00 	vcvtps2pi \{.5b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 534 <_start\+0x534>	530: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 19 91 05 00 00 00 00 	vcvtps2pi \{.8b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 53d <_start\+0x53d>	539: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 92 05 00 00 00 00 	vcvtps2pu \{.0b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 546 <_start\+0x546>	542: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 92 05 00 00 00 00 	vcvtps2pu \{.0b\},\{rn\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 54f <_start\+0x54f>	54b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 92 05 00 00 00 00 	vcvtps2pu \{.0b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 558 <_start\+0x558>	554: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 92 05 00 00 00 00 	vcvtps2pu \{.0b\},\{rn\},0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 561 <_start\+0x561>	55d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 92 05 00 00 00 00 	vcvtps2pu \{.0b\},\{rn\},0x0\(%rip\),%v0\{%k1\}        # 56a <_start\+0x56a>	566: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 21 92 05 00 00 00 00 	vcvtps2pu \{.16b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 573 <_start\+0x573>	56f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 29 92 05 00 00 00 00 	vcvtps2pu \{.24b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 57c <_start\+0x57c>	578: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 31 92 05 00 00 00 00 	vcvtps2pu \{.31b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 585 <_start\+0x585>	581: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 79 92 05 00 00 00 00 	vcvtps2pu \{.32b\},\{rd\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 58e <_start\+0x58e>	58a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 39 92 05 00 00 00 00 	vcvtps2pu \{.32b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 597 <_start\+0x597>	593: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 b9 92 05 00 00 00 00 	vcvtps2pu \{.32b\},\{ru\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 5a0 <_start\+0x5a0>	59c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 f9 92 05 00 00 00 00 	vcvtps2pu \{.32b\},\{rz\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 5a9 <_start\+0x5a9>	5a5: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 92 05 00 00 00 00 	vcvtps2pu \{.4b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 5b2 <_start\+0x5b2>	5ae: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 11 92 05 00 00 00 00 	vcvtps2pu \{.5b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 5bb <_start\+0x5bb>	5b7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 19 92 05 00 00 00 00 	vcvtps2pu \{.8b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 5c4 <_start\+0x5c4>	5c0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 9d 05 00 00 00 00 	vcvtps2srgb8 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 5cd <_start\+0x5cd>	5c9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 9d 05 00 00 00 00 	vcvtps2srgb8 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 5d6 <_start\+0x5d6>	5d2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 9d 0d 00 00 00 00 	vcvtps2srgb8 0x0\(%rip\)\{nt\},%v1\{%k7\}        # 5df <_start\+0x5df>	5db: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 9d 15 00 00 00 00 	vcvtps2srgb8 0x0\(%rip\)\{nt\},%v2\{%k7\}        # 5e8 <_start\+0x5e8>	5e4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 9d 05 00 00 00 00 	vcvtps2srgb8 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 5f1 <_start\+0x5f1>	5ed: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 9d 05 00 00 00 00 	vcvtps2srgb8 0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 5fa <_start\+0x5fa>	5f6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 9d 05 00 00 00 00 	vcvtps2srgb8 0x0\(%rip\),%v0\{%k1\}        # 603 <_start\+0x603>	5ff: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 58 08 96 1d 00 00 00 00 	vcvtpu2pd \$0x1,0x0\(%rip\)\{sint8\},%v19        # 60d <_start\+0x60d>	609: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 96 05 00 00 00 00 	vcvtpu2ps \{.0b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 616 <_start\+0x616>	612: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 96 05 00 00 00 00 	vcvtpu2ps \{.0b\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # 61f <_start\+0x61f>	61b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 96 05 00 00 00 00 	vcvtpu2ps \{.0b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 628 <_start\+0x628>	624: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 96 05 00 00 00 00 	vcvtpu2ps \{.0b\},0x0\(%rip\)\{sint8\},%v0\{%k1\}        # 631 <_start\+0x631>	62d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 96 05 00 00 00 00 	vcvtpu2ps \{.0b\},0x0\(%rip\),%v0\{%k1\}        # 63a <_start\+0x63a>	636: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 21 96 05 00 00 00 00 	vcvtpu2ps \{.16b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 643 <_start\+0x643>	63f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 58 08 96 1d 00 00 00 00 	vcvtpu2ps \{.4b\},0x0\(%rip\)\{sint8\},%v19        # 64c <_start\+0x64c>	648: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 29 96 05 00 00 00 00 	vcvtpu2ps \{.24b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 655 <_start\+0x655>	651: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 31 96 05 00 00 00 00 	vcvtpu2ps \{.31b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 65e <_start\+0x65e>	65a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 39 96 05 00 00 00 00 	vcvtpu2ps \{.32b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 667 <_start\+0x667>	663: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 96 05 00 00 00 00 	vcvtpu2ps \{.4b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 670 <_start\+0x670>	66c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 11 96 05 00 00 00 00 	vcvtpu2ps \{.5b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 679 <_start\+0x679>	675: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 19 96 05 00 00 00 00 	vcvtpu2ps \{.8b\},0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 682 <_start\+0x682>	67e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 b0 05 00 00 00 00 	vexp2lutps 0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 68b <_start\+0x68b>	687: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 b0 05 00 00 00 00 	vexp2lutps 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 694 <_start\+0x694>	690: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b0 0d 00 00 00 00 	vexp2lutps 0x0\(%rip\)\{nt\},%v1\{%k7\}        # 69d <_start\+0x69d>	699: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b0 15 00 00 00 00 	vexp2lutps 0x0\(%rip\)\{nt\},%v2\{%k7\}        # 6a6 <_start\+0x6a6>	6a2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 b0 05 00 00 00 00 	vexp2lutps 0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k1\}        # 6af <_start\+0x6af>	6ab: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 b0 05 00 00 00 00 	vexp2lutps 0x0\(%rip\)\{sint8\},%v0\{%k1\}        # 6b8 <_start\+0x6b8>	6b4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 b0 05 00 00 00 00 	vexp2lutps 0x0\(%rip\),%v0\{%k1\}        # 6c1 <_start\+0x6c1>	6bd: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 00 00 	vfixupps \$0x0,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 6cc <_start\+0x6cc>	6c6: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 80 01 f0 05 00 00 00 00 00 00 	vfixupps \$0x0,0x0\(%rip\)\{nt\},%v0\{%k1\}        # 6d7 <_start\+0x6d7>	6d1: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 00 00 	vfixupps \$0x0,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 6e2 <_start\+0x6e2>	6dc: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 50 01 f0 05 00 00 00 00 00 00 	vfixupps \$0x0,0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 6ed <_start\+0x6ed>	6e7: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 7f 00 	vfixupps \$0x7f,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 6f8 <_start\+0x6f8>	6f2: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 00 40 	vfixupps \$0x4000,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 703 <_start\+0x703>	6fd: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 01 00 	vfixupps \$0x1,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 70e <_start\+0x70e>	708: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 00 01 	vfixupps \$0x100,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 719 <_start\+0x719>	713: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 ff ff 	vfixupps \$0xffff,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 724 <_start\+0x724>	71e: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 09 f0 05 00 00 00 00 00 00 	vfixupps \$0x10000,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 72f <_start\+0x72f>	729: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 07 00 	vfixupps \$0x7,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 73a <_start\+0x73a>	734: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 b4 05 00 00 00 00 	vgetexpps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 743 <_start\+0x743>	73f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 b4 05 00 00 00 00 	vgetexpps 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 74c <_start\+0x74c>	748: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b4 0d 00 00 00 00 	vgetexpps 0x0\(%rip\)\{nt\},%v1\{%k7\}        # 755 <_start\+0x755>	751: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b4 15 00 00 00 00 	vgetexpps 0x0\(%rip\)\{nt\},%v2\{%k7\}        # 75e <_start\+0x75e>	75a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 b4 05 00 00 00 00 	vgetexpps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 767 <_start\+0x767>	763: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 b4 05 00 00 00 00 	vgetexpps 0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 770 <_start\+0x770>	76c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 b4 05 00 00 00 00 	vgetexpps 0x0\(%rip\),%v0\{%k1\}        # 779 <_start\+0x779>	775: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 0f f2 05 00 00 00 00 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x0\(%rip\)\{nt\}\{sint8\},%v1,%v0\{%k7\}        # 784 <_start\+0x784>	77e: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 00 01 10 05 00 00 00 00 	vloadd 0x0\(%rip\),%v0\{%k1\}        # 78d <_start\+0x78d>	789: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 10 01 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{1to16\},%v0\{%k1\}        # 796 <_start\+0x796>	792: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 a0 01 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{4to16\},%v0\{%k1\}        # 79f <_start\+0x79f>	79b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{4to16\}\{srgb8\},%v0\{%k1\}        # 7a8 <_start\+0x7a8>	7a4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 ac a8 10 1d 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{4to16\}\{float11B\},%v27        # 7b1 <_start\+0x7b1>	7ad: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{srgb8\},%v0\{%k1\}        # 7ba <_start\+0x7ba>	7b6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 90 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{1to16\}\{srgb8\},%v0\{%k1\}        # 7c3 <_start\+0x7c3>	7bf: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 90 41 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{1to16\}\{uint16\},%v0\{%k1\}        # 7cc <_start\+0x7cc>	7c8: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 a0 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{4to16\}\{srgb8\},%v0\{%k1\}        # 7d5 <_start\+0x7d5>	7d1: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 a0 41 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{4to16\}\{uint16\},%v0\{%k1\}        # 7de <_start\+0x7de>	7da: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{srgb8\},%v0\{%k1\}        # 7e7 <_start\+0x7e7>	7e3: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 7f0 <_start\+0x7f0>	7ec: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 10 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{1to16\}\{srgb8\},%v0\{%k1\}        # 7f9 <_start\+0x7f9>	7f5: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 90 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{1to16\}\{srgb8\},%v0\{%k1\}        # 802 <_start\+0x802>	7fe: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 a0 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{4to16\}\{srgb8\},%v0\{%k1\}        # 80b <_start\+0x80b>	807: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{nt\}\{srgb8\},%v0\{%k1\}        # 814 <_start\+0x814>	810: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 19 10 05 00 00 00 00 	vloadd 0x0\(%rip\)\{srgb8\},%v0\{%k1\}        # 81d <_start\+0x81d>	819: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 10 05 00 00 00 00 	vloadd 0x0\(%rip\),%v0        # 826 <_start\+0x826>	822: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 10 05 00 00 00 00 	vloadd 0x0\(%rip\),%v0\{%k1\}        # 82f <_start\+0x82f>	82b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 00 13 05 00 00 00 00 	vloadunpackhd 0x0\(%rip\)\{nt\},%v0        # 838 <_start\+0x838>	834: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 13 05 00 00 00 00 	vloadunpackhd 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 841 <_start\+0x841>	83d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 13 05 00 00 00 00 	vloadunpackhd 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 84a <_start\+0x84a>	846: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 19 13 05 00 00 00 00 	vloadunpackhd 0x0\(%rip\)\{nt\}\{srgb8\},%v0\{%k1\}        # 853 <_start\+0x853>	84f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 24 13 1d 00 00 00 00 	vloadunpackhd 0x0\(%rip\)\{uint8\},%v19\{%k4\}        # 85c <_start\+0x85c>	858: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 13 05 00 00 00 00 	vloadunpackhd 0x0\(%rip\),%v0        # 865 <_start\+0x865>	861: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 13 05 00 00 00 00 	vloadunpackhd 0x0\(%rip\),%v0\{%k1\}        # 86e <_start\+0x86e>	86a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 01 13 05 00 00 00 00 	vloadunpackhq 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 878 <_start\+0x878>	874: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 84 80 12 1d 00 00 00 00 	vloadunpackld 0x0\(%rip\)\{nt\}\{unorm10A\},%v11        # 881 <_start\+0x881>	87d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 00 12 05 00 00 00 00 	vloadunpackld 0x0\(%rip\)\{nt\},%v0        # 88a <_start\+0x88a>	886: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 12 05 00 00 00 00 	vloadunpackld 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 893 <_start\+0x893>	88f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 12 05 00 00 00 00 	vloadunpackld 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 89c <_start\+0x89c>	898: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 19 12 05 00 00 00 00 	vloadunpackld 0x0\(%rip\)\{nt\}\{srgb8\},%v0\{%k1\}        # 8a5 <_start\+0x8a5>	8a1: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 12 05 00 00 00 00 	vloadunpackld 0x0\(%rip\),%v0        # 8ae <_start\+0x8ae>	8aa: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 12 05 00 00 00 00 	vloadunpackld 0x0\(%rip\),%v0\{%k1\}        # 8b7 <_start\+0x8b7>	8b3: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 00 12 3d 00 00 00 00 	vloadunpackld 0x0\(%rip\),%v15        # 8c0 <_start\+0x8c0>	8bc: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 0c 00 12 3d 00 00 00 00 	vloadunpackld 0x0\(%rip\),%v31        # 8c9 <_start\+0x8c9>	8c5: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 12 3d 00 00 00 00 	vloadunpackld 0x0\(%rip\),%v7        # 8d2 <_start\+0x8d2>	8ce: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 04 00 12 05 00 00 00 00 	vloadunpackld 0x0\(%rip\),%v8        # 8db <_start\+0x8db>	8d7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 01 12 05 00 00 00 00 	vloadunpacklq 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 8e5 <_start\+0x8e5>	8e1: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 b1 05 00 00 00 00 	vlog2lutps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 8ee <_start\+0x8ee>	8ea: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 b1 05 00 00 00 00 	vlog2lutps 0x0\(%rip\)\{nt\},%v0\{%k1\}        # 8f7 <_start\+0x8f7>	8f3: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b1 0d 00 00 00 00 	vlog2lutps 0x0\(%rip\)\{nt\},%v1\{%k7\}        # 900 <_start\+0x900>	8fc: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b1 15 00 00 00 00 	vlog2lutps 0x0\(%rip\)\{nt\},%v2\{%k7\}        # 909 <_start\+0x909>	905: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 b1 05 00 00 00 00 	vlog2lutps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 912 <_start\+0x912>	90e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 b1 05 00 00 00 00 	vlog2lutps 0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # 91b <_start\+0x91b>	917: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 b1 05 00 00 00 00 	vlog2lutps 0x0\(%rip\),%v0\{%k1\}        # 924 <_start\+0x924>	920: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 10 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{1to16\},%v1,%v0\{%k7\}        # 92d <_start\+0x92d>	929: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{4to16\},%v1,%v0\{%k7\}        # 936 <_start\+0x936>	932: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 60 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{float16\},%v1,%v0\{%k7\}        # 93f <_start\+0x93f>	93b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}        # 948 <_start\+0x948>	944: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 951 <_start\+0x951>	94d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 95a <_start\+0x95a>	956: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{nt\},%v1,%v0\{%k7\}        # 963 <_start\+0x963>	95f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 70 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{sint16\},%v1,%v0\{%k7\}        # 96c <_start\+0x96c>	968: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 40 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{uint8\},%v1,%v0\{%k7\}        # 975 <_start\+0x975>	971: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{nt\}\{unorm8\},%v1,%v0\{%k7\}        # 97e <_start\+0x97e>	97a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\)\{unorm8\},%v1,%v0\{%k7\}        # 987 <_start\+0x987>	983: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 0f 9e 05 00 00 00 00 	vmaxabsps 0x0\(%rip\),%v1,%v0\{%k7\}        # 990 <_start\+0x990>	98c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 17 05 00 00 00 00 	vpackstorehd %v0,0x0\(%rip\)        # 999 <_start\+0x999>	995: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 17 05 00 00 00 00 	vpackstorehd %v0,0x0\(%rip\)\{%k1\}        # 9a2 <_start\+0x9a2>	99e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 17 05 00 00 00 00 	vpackstorehd %v0,0x0\(%rip\)\{%k1\}\{nt\}        # 9ab <_start\+0x9ab>	9a7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 17 05 00 00 00 00 	vpackstorehd %v0,0x0\(%rip\)\{%k1\}\{nt\}        # 9b4 <_start\+0x9b4>	9b0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 07 17 05 00 00 00 00 	vpackstorehd %v0,0x0\(%rip\)\{%k7\}        # 9bd <_start\+0x9bd>	9b9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 00 17 05 00 00 00 00 	vpackstorehd %v0,0x0\(%rip\)\{nt\}        # 9c6 <_start\+0x9c6>	9c2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 28 17 05 00 00 00 00 	vpackstorehd %v0\{sint8\},0x0\(%rip\)        # 9cf <_start\+0x9cf>	9cb: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 84 4a 17 35 00 00 00 00 	vpackstorehd %v14\{sint16\},0x0\(%rip\)\{%k2\}\{nt\}        # 9d8 <_start\+0x9d8>	9d4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 84 01 17 3d 00 00 00 00 	vpackstorehd %v15,0x0\(%rip\)\{%k1\}\{nt\}        # 9e1 <_start\+0x9e1>	9dd: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 00 17 05 00 00 00 00 	vpackstorehq %v0,0x0\(%rip\)\{nt\}        # 9eb <_start\+0x9eb>	9e7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 16 05 00 00 00 00 	vpackstoreld %v0,0x0\(%rip\)        # 9f4 <_start\+0x9f4>	9f0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 16 05 00 00 00 00 	vpackstoreld %v0,0x0\(%rip\)\{%k1\}        # 9fd <_start\+0x9fd>	9f9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 16 05 00 00 00 00 	vpackstoreld %v0,0x0\(%rip\)\{%k1\}\{nt\}        # a06 <_start\+0xa06>	a02: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 16 05 00 00 00 00 	vpackstoreld %v0,0x0\(%rip\)\{%k1\}\{nt\}        # a0f <_start\+0xa0f>	a0b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 07 16 05 00 00 00 00 	vpackstoreld %v0,0x0\(%rip\)\{%k7\}        # a18 <_start\+0xa18>	a14: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 00 16 05 00 00 00 00 	vpackstoreld %v0,0x0\(%rip\)\{nt\}        # a21 <_start\+0xa21>	a1d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 28 16 05 00 00 00 00 	vpackstoreld %v0\{sint8\},0x0\(%rip\)        # a2a <_start\+0xa2a>	a26: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 84 01 16 3d 00 00 00 00 	vpackstoreld %v15,0x0\(%rip\)\{%k1\}\{nt\}        # a33 <_start\+0xa33>	a2f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 08 2e 16 3d 00 00 00 00 	vpackstoreld %v23\{sint8\},0x0\(%rip\)\{%k6\}        # a3c <_start\+0xa3c>	a38: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	66 d6 80 00 16 05 00 00 00 00 	vpackstorelq %v0,0x0\(%rip\)\{nt\}        # a46 <_start\+0xa46>	a42: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 cb 05 00 00 00 00    	vprefetch1 \$0x0,0x0\(%rip\)        # a4d <_start\+0xa4d>	a49: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 db 05 00 00 00 00    	vprefetch2 \$0x0,0x0\(%rip\)        # a54 <_start\+0xa54>	a50: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 c0 05 00 00 00 00 	vrcpresps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # a5d <_start\+0xa5d>	a59: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 c0 05 00 00 00 00 	vrcpresps 0x0\(%rip\)\{nt\},%v0\{%k1\}        # a66 <_start\+0xa66>	a62: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 c0 0d 00 00 00 00 	vrcpresps 0x0\(%rip\)\{nt\},%v1\{%k7\}        # a6f <_start\+0xa6f>	a6b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 c0 15 00 00 00 00 	vrcpresps 0x0\(%rip\)\{nt\},%v2\{%k7\}        # a78 <_start\+0xa78>	a74: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 c0 05 00 00 00 00 	vrcpresps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # a81 <_start\+0xa81>	a7d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 c0 05 00 00 00 00 	vrcpresps 0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # a8a <_start\+0xa8a>	a86: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 c0 05 00 00 00 00 	vrcpresps 0x0\(%rip\),%v0\{%k1\}        # a93 <_start\+0xa93>	a8f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 07 f3 05 00 00 00 00 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k7\}        # a9e <_start\+0xa9e>	a98: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 07 f3 05 00 00 00 00 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k7\}        # aa9 <_start\+0xaa9>	aa3: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 01 98 05 00 00 00 00 	vroundps \{.0b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # ab2 <_start\+0xab2>	aae: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 98 05 00 00 00 00 	vroundps \{.0b\},\{rn\},0x0\(%rip\)\{nt\},%v0\{%k1\}        # abb <_start\+0xabb>	ab7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 98 05 00 00 00 00 	vroundps \{.0b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # ac4 <_start\+0xac4>	ac0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 98 05 00 00 00 00 	vroundps \{.0b\},\{rn\},0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # acd <_start\+0xacd>	ac9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 98 05 00 00 00 00 	vroundps \{.0b\},\{rn\},0x0\(%rip\),%v0\{%k1\}        # ad6 <_start\+0xad6>	ad2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 21 98 05 00 00 00 00 	vroundps \{.16b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # adf <_start\+0xadf>	adb: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 29 98 05 00 00 00 00 	vroundps \{.24b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # ae8 <_start\+0xae8>	ae4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 31 98 05 00 00 00 00 	vroundps \{.31b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # af1 <_start\+0xaf1>	aed: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 79 98 05 00 00 00 00 	vroundps \{.32b\},\{rd\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # afa <_start\+0xafa>	af6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 39 98 05 00 00 00 00 	vroundps \{.32b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b03 <_start\+0xb03>	aff: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 b9 98 05 00 00 00 00 	vroundps \{.32b\},\{ru\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b0c <_start\+0xb0c>	b08: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 f9 98 05 00 00 00 00 	vroundps \{.32b\},\{rz\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b15 <_start\+0xb15>	b11: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 09 98 05 00 00 00 00 	vroundps \{.4b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b1e <_start\+0xb1e>	b1a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 11 98 05 00 00 00 00 	vroundps \{.5b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b27 <_start\+0xb27>	b23: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 19 98 05 00 00 00 00 	vroundps \{.8b\},\{rn\},0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b30 <_start\+0xb30>	b2c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 b2 05 00 00 00 00 	vrsqrtlutps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b39 <_start\+0xb39>	b35: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 b2 05 00 00 00 00 	vrsqrtlutps 0x0\(%rip\)\{nt\},%v0\{%k1\}        # b42 <_start\+0xb42>	b3e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b2 0d 00 00 00 00 	vrsqrtlutps 0x0\(%rip\)\{nt\},%v1\{%k7\}        # b4b <_start\+0xb4b>	b47: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 07 b2 15 00 00 00 00 	vrsqrtlutps 0x0\(%rip\)\{nt\},%v2\{%k7\}        # b54 <_start\+0xb54>	b50: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 01 b2 05 00 00 00 00 	vrsqrtlutps 0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # b5d <_start\+0xb5d>	b59: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 01 b2 05 00 00 00 00 	vrsqrtlutps 0x0\(%rip\)\{unorm8\},%v0\{%k1\}        # b66 <_start\+0xb66>	b62: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 b2 05 00 00 00 00 	vrsqrtlutps 0x0\(%rip\),%v0\{%k1\}        # b6f <_start\+0xb6f>	b6b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 17 55 0d 00 00 00 00 	vsbbpi 0x0\(%rip\)\{nt\},%k2,%v1\{%k7\}        # b78 <_start\+0xb78>	b74: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 1f 55 15 00 00 00 00 	vsbbpi 0x0\(%rip\)\{nt\},%k3,%v2\{%k7\}        # b81 <_start\+0xb81>	b7d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 17 5d 0d 00 00 00 00 	vsbbrpi 0x0\(%rip\)\{nt\},%k2,%v1\{%k7\}        # b8a <_start\+0xb8a>	b86: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 1f 5d 15 00 00 00 00 	vsbbrpi 0x0\(%rip\)\{nt\},%k3,%v2\{%k7\}        # b93 <_start\+0xb93>	b8f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 00 f1 05 00 00 00 00 00 00 	vshuf128x32 \{aaaa\},\{0000\},0x0\(%rip\)\{nt\},%v0        # b9e <_start\+0xb9e>	b98: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 00 00 f1 05 00 00 00 00 00 00 	vshuf128x32 \{aaaa\},\{0000\},0x0\(%rip\),%v0        # ba9 <_start\+0xba9>	ba3: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 90 29 14 05 00 00 00 00 	vstored %v0\{a\}\{sint8\},0x0\(%rip\)\{%k1\}\{nt\}        # bb2 <_start\+0xbb2>	bae: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 01 14 05 00 00 00 00 	vstored %v0\{dcba\},0x0\(%rip\)\{%k1\}        # bbb <_start\+0xbbb>	bb7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 a0 01 14 05 00 00 00 00 	vstored %v0\{dcba\},0x0\(%rip\)\{%k1\}\{nt\}        # bc4 <_start\+0xbc4>	bc0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 29 14 05 00 00 00 00 	vstored %v0\{dcba\}\{sint8\},0x0\(%rip\)\{%k1\}        # bcd <_start\+0xbcd>	bc9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 11 14 05 00 00 00 00 	vstored %v0\{float16rz\},0x0\(%rip\)\{%k1\}        # bd6 <_start\+0xbd6>	bd2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 01 14 05 00 00 00 00 	vstored %v0,0x0\(%rip\)\{%k1\}        # bdf <_start\+0xbdf>	bdb: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 01 14 05 00 00 00 00 	vstored %v0,0x0\(%rip\)\{%k1\}\{nt\}        # be8 <_start\+0xbe8>	be4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 10 28 14 05 00 00 00 00 	vstored %v0\{a\}\{sint8\},0x0\(%rip\)        # bf1 <_start\+0xbf1>	bed: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 90 29 14 05 00 00 00 00 	vstored %v0\{a\}\{sint8\},0x0\(%rip\)\{%k1\}\{nt\}        # bfa <_start\+0xbfa>	bf6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 28 14 05 00 00 00 00 	vstored %v0\{dcba\}\{sint8\},0x0\(%rip\)        # c03 <_start\+0xc03>	bff: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 20 29 14 05 00 00 00 00 	vstored %v0\{dcba\}\{sint8\},0x0\(%rip\)\{%k1\}        # c0c <_start\+0xc0c>	c08: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 28 14 05 00 00 00 00 	vstored %v0\{sint8\},0x0\(%rip\)        # c15 <_start\+0xc15>	c11: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 29 14 05 00 00 00 00 	vstored %v0\{sint8\},0x0\(%rip\)\{%k1\}        # c1e <_start\+0xc1e>	c1a: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 29 14 05 00 00 00 00 	vstored %v0\{sint8\},0x0\(%rip\)\{%k1\}\{nt\}        # c27 <_start\+0xc27>	c23: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 17 5b 0d 00 00 00 00 	vsubrsetbpi 0x0\(%rip\)\{nt\},%k2,%v1\{%k7\}        # c30 <_start\+0xc30>	c2c: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 1f 5b 15 00 00 00 00 	vsubrsetbpi 0x0\(%rip\)\{nt\},%k3,%v2\{%k7\}        # c39 <_start\+0xc39>	c35: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 17 53 0d 00 00 00 00 	vsubsetbpi 0x0\(%rip\)\{nt\},%k2,%v1\{%k7\}        # c42 <_start\+0xc42>	c3e: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 1f 53 15 00 00 00 00 	vsubsetbpi 0x0\(%rip\)\{nt\},%k3,%v2\{%k7\}        # c4b <_start\+0xc4b>	c47: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 00 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\)\{nt\}\{sint8\},%v0,%k1        # c54 <_start\+0xc54>	c50: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 00 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\)\{nt\},%v0,%k1        # c5d <_start\+0xc5d>	c59: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 80 0f 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\)\{nt\},%v1,%k1\{%k7\}        # c66 <_start\+0xc66>	c62: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 d0 00 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\)\{nt\}\{sint8\},%v0,%k1        # c6f <_start\+0xc6f>	c6b: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 50 00 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\)\{sint8\},%v0,%k1        # c78 <_start\+0xc78>	c74: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\),%v0,%k1        # c81 <_start\+0xc81>	c7d: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 07 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\),%v0,%k1\{%k7\}        # c8a <_start\+0xc8a>	c86: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 00 30 3d 00 00 00 00 	vtestpi 0x0\(%rip\),%v0,%k7        # c93 <_start\+0xc93>	c8f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	d6 00 f8 30 0d 00 00 00 00 	vtestpi 0x0\(%rip\),%v31,%k1        # c9c <_start\+0xc9c>	c98: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 eb 05 00 00 00 00    	clevict1 0x0\(%rip\)        # ca3 <_start\+0xca3>	c9f: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 eb 15 00 00 00 00    	ldvxcsr 0x0\(%rip\)        # caa <_start\+0xcaa>	ca6: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 eb 15 00 00 00 00    	ldvxcsr 0x0\(%rip\)        # cb1 <_start\+0xcb1>	cad: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 eb 1d 00 00 00 00    	stvxcsr 0x0\(%rip\)        # cb8 <_start\+0xcb8>	cb4: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 eb 1d 00 00 00 00    	stvxcsr 0x0\(%rip\)        # cbf <_start\+0xcbf>	cbb: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 cb 05 00 00 00 00    	vprefetch1 \$0x0,0x0\(%rip\)        # cc6 <_start\+0xcc6>	cc2: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 cb 0d 00 00 00 00    	vprefetch1 \$0x1,0x0\(%rip\)        # ccd <_start\+0xccd>	cc9: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 cb 3d 00 00 00 00    	vprefetch1 \$0x7,0x0\(%rip\)        # cd4 <_start\+0xcd4>	cd0: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 db 05 00 00 00 00    	vprefetch2 \$0x0,0x0\(%rip\)        # cdb <_start\+0xcdb>	cd7: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 db 0d 00 00 00 00    	vprefetch2 \$0x1,0x0\(%rip\)        # ce2 <_start\+0xce2>	cde: R_X86_64_PC32	foo\+0xfffffffffffffffc
[ 	]*[a-f0-9]+:	62 db 1d 00 00 00 00    	vprefetch2 \$0x3,0x0\(%rip\)        # ce9 <_start\+0xce9>	ce5: R_X86_64_PC32	foo\+0xfffffffffffffffc
#pass

#as: -march=l1om
#objdump: -drw
#name: L1OMni relocs

.*: +file format .*

Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	d6 04 f8 f0 a8 00 00 00 00 ff ff 	vfixupps \$0x1fffff,0x0\(%rax\),%v13	5: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 06 f8 f0 ac 60 00 00 00 00 ff ff 	vfixupps \$0x1fffff,0x0\(%rax,%r12,2\),%v13	11: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 d0 01 f0 05 00 00 00 00 00 00 	vfixupps \$0x0,0x0\(%rip\)\{nt\}\{unorm8\},%v0\{%k1\}        # 22 <_start\+0x22>	1c: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 0f f2 80 00 00 00 00 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x0\(%rax\)\{nt\}\{sint8\},%v1,%v0\{%k7\}	27: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 d0 0f f2 84 98 00 00 00 00 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x0\(%rax,%rbx,4\)\{nt\}\{sint8\},%v1,%v0\{%k7\}	33: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 d0 0f f2 05 00 00 00 00 d0 98 	vinsertfieldpi \$0x13,\$0x3,\$0x10,0x0\(%rip\)\{nt\}\{sint8\},%v1,%v0\{%k7\}        # 44 <_start\+0x44>	3e: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 d0 07 f3 80 00 00 00 00 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x0\(%rax\)\{nt\}\{sint8\},%v0\{%k7\}	49: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 d0 07 f3 84 98 00 00 00 00 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x0\(%rax,%rbx,4\)\{nt\}\{sint8\},%v0\{%k7\}	55: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 d0 07 f3 05 00 00 00 00 d0 98 	vrotatefieldpi \$0x13,\$0x3,\$0x10,0x0\(%rip\)\{nt\}\{sint8\},%v0\{%k7\}        # 66 <_start\+0x66>	60: R_X86_64_PC32	foo\+0xfffffffffffffffa
[ 	]*[a-f0-9]+:	d6 01 00 f1 80 00 00 00 00 00 00 	vshuf128x32 \{aaaa\},\{0000\},0x0\(%r8\),%v0	6b: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 03 00 f1 84 f0 00 00 00 00 00 00 	vshuf128x32 \{aaaa\},\{0000\},0x0\(%r8,%r14,8\),%v0	77: R_X86_64_32S	foo
[ 	]*[a-f0-9]+:	d6 00 00 f1 05 00 00 00 00 00 00 	vshuf128x32 \{aaaa\},\{0000\},0x0\(%rip\),%v0        # 88 <_start\+0x88>	82: R_X86_64_PC32	foo\+0xfffffffffffffffa
#pass

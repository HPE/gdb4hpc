#as: -march=l1om
#objdump: -drw
#name: L1OMni Scalar ops

.*: +file format .*

Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	66 62 f4 c0             	bitinterleave11 %ax,%ax
[ 	]*[a-f0-9]+:	66 62 f4 d8             	bitinterleave11 %bx,%ax
[ 	]*[a-f0-9]+:	66 62 b4 d8             	bitinterleave11 %bx,%r8w
[ 	]*[a-f0-9]+:	62 f4 c0                	bitinterleave11 %eax,%eax
[ 	]*[a-f0-9]+:	62 f4 d8                	bitinterleave11 %ebx,%eax
[ 	]*[a-f0-9]+:	62 b4 d8                	bitinterleave11 %ebx,%r8d
[ 	]*[a-f0-9]+:	62 a4 d8                	bitinterleave11 %rbx,%r8
[ 	]*[a-f0-9]+:	62 f4 c0                	bitinterleave11 %eax,%eax
[ 	]*[a-f0-9]+:	62 e4 c0                	bitinterleave11 %rax,%rax
[ 	]*[a-f0-9]+:	62 e4 d8                	bitinterleave11 %rbx,%rax
[ 	]*[a-f0-9]+:	66 62 f4 c0             	bitinterleave11 %ax,%ax
[ 	]*[a-f0-9]+:	62 f4 c0                	bitinterleave11 %eax,%eax
[ 	]*[a-f0-9]+:	62 e4 c0                	bitinterleave11 %rax,%rax
[ 	]*[a-f0-9]+:	62 cb 02                	vprefetch1 \$0x0,\(%rdx\)
[ 	]*[a-f0-9]+:	62 cb 0a                	vprefetch1 \$0x1,\(%rdx\)
[ 	]*[a-f0-9]+:	62 cb 3a                	vprefetch1 \$0x7,\(%rdx\)
[ 	]*[a-f0-9]+:	62 cb 4a 01             	vprefetch1 \$0x1,0x40\(%rdx\)
[ 	]*[a-f0-9]+:	62 cb 8a 04 00 00 00    	vprefetch1 \$0x1,0x4\(%rdx\)
[ 	]*[a-f0-9]+:	62 cb 4a ff             	vprefetch1 \$0x1,-0x40\(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 02                	clevict1 \(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 02                	clevict1 \(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 02                	clevict1 \(%rdx\)
[ 	]*[a-f0-9]+:	62 ab 00                	clevict1 \(%r8\)
[ 	]*[a-f0-9]+:	62 eb 42 40             	clevict1 0x40\(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 42 04             	clevict1 0x4\(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 42 c0             	clevict1 -0x40\(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 0a                	clevict2 \(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 0a                	clevict2 \(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 0a                	clevict2 \(%rdx\)
[ 	]*[a-f0-9]+:	62 ab 08                	clevict2 \(%r8\)
[ 	]*[a-f0-9]+:	62 eb 4a 40             	clevict2 0x40\(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 4a 04             	clevict2 0x4\(%rdx\)
[ 	]*[a-f0-9]+:	62 eb 4a c0             	clevict2 -0x40\(%rdx\)
[ 	]*[a-f0-9]+:	62 9f c1                	delay  %r9d
[ 	]*[a-f0-9]+:	66 62 9f c1             	delay  %r9w
[ 	]*[a-f0-9]+:	62 df c3                	delay  %ebx
[ 	]*[a-f0-9]+:	66 62 df c3             	delay  %bx
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 00 00 	insertfield \$0x0,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff 00 00 	insertfield \$0x0,\$0x0,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 ff 00 	insertfield \$0x0,\$0xff,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff ff 00 	insertfield \$0x0,\$0xff,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 00 ff 	insertfield \$0xff,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f6 c0 ff ff ff ff 	insertfield \$0xff,\$0xff,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	62 f6 c0 00 00 00 00    	insertfield \$0x0,\$0x0,\$0x0,%eax,%eax
[ 	]*[a-f0-9]+:	62 e6 c0 00 00 00 00    	insertfield \$0x0,\$0x0,\$0x0,%rax,%rax
[ 	]*[a-f0-9]+:	66 62 f6 c0 00 00 00 00 	insertfield \$0x0,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	62 f6 c0 00 00 00 00    	insertfield \$0x0,\$0x0,\$0x0,%eax,%eax
[ 	]*[a-f0-9]+:	62 e6 c0 00 00 00 00    	insertfield \$0x0,\$0x0,\$0x0,%rax,%rax
[ 	]*[a-f0-9]+:	67 62 db 02             	vprefetch2 \$0x0,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 db 0a             	vprefetch2 \$0x1,\(%edx\)
[ 	]*[a-f0-9]+:	67 62 db 1a             	vprefetch2 \$0x3,\(%edx\)
[ 	]*[a-f0-9]+:	62 db 02                	vprefetch2 \$0x0,\(%rdx\)
[ 	]*[a-f0-9]+:	62 db 0a                	vprefetch2 \$0x1,\(%rdx\)
[ 	]*[a-f0-9]+:	62 db 1a                	vprefetch2 \$0x3,\(%rdx\)
[ 	]*[a-f0-9]+:	62 db 4a 01             	vprefetch2 \$0x1,0x40\(%rdx\)
[ 	]*[a-f0-9]+:	62 db 8a 04 00 00 00    	vprefetch2 \$0x1,0x4\(%rdx\)
[ 	]*[a-f0-9]+:	62 db 4a ff             	vprefetch2 \$0x1,-0x40\(%rdx\)
[ 	]*[a-f0-9]+:	66 62 f7 c0 00 00 00 00 	rotatefield \$0x0,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f7 c0 ff ff 00 00 	rotatefield \$0x0,\$0x0,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f7 c0 00 00 ff 00 	rotatefield \$0x0,\$0xff,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f7 c0 ff ff ff 00 	rotatefield \$0x0,\$0xff,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f7 c0 00 00 00 ff 	rotatefield \$0xff,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	66 62 f7 c0 ff ff ff ff 	rotatefield \$0xff,\$0xff,\$0xffff,%ax,%ax
[ 	]*[a-f0-9]+:	62 f7 c0 00 00 00 00    	rotatefield \$0x0,\$0x0,\$0x0,%eax,%eax
[ 	]*[a-f0-9]+:	62 e7 c0 00 00 00 00    	rotatefield \$0x0,\$0x0,\$0x0,%rax,%rax
[ 	]*[a-f0-9]+:	66 62 f7 c0 00 00 00 00 	rotatefield \$0x0,\$0x0,\$0x0,%ax,%ax
[ 	]*[a-f0-9]+:	62 f7 c0 00 00 00 00    	rotatefield \$0x0,\$0x0,\$0x0,%eax,%eax
[ 	]*[a-f0-9]+:	62 e7 c0 00 00 00 00    	rotatefield \$0x0,\$0x0,\$0x0,%rax,%rax
[ 	]*[a-f0-9]+:	62 ab 02                	clevict1 \(%r10\)
[ 	]*[a-f0-9]+:	66 62 9f c0             	delay  %r8w
[ 	]*[a-f0-9]+:	62 9f c0                	delay  %r8d
[ 	]*[a-f0-9]+:	62 8f c0                	delay  %r8
[ 	]*[a-f0-9]+:	66 62 df c0             	delay  %ax
[ 	]*[a-f0-9]+:	62 df c0                	delay  %eax
[ 	]*[a-f0-9]+:	62 cf c0                	delay  %rax
[ 	]*[a-f0-9]+:	62 eb 10                	ldvxcsr \(%rax\)
[ 	]*[a-f0-9]+:	62 eb 18                	stvxcsr \(%rax\)
[ 	]*[a-f0-9]+:	62 eb 10                	ldvxcsr \(%rax\)
[ 	]*[a-f0-9]+:	62 eb 18                	stvxcsr \(%rax\)
[ 	]*[a-f0-9]+:	66 62 9f c8             	spflt  %r8w
[ 	]*[a-f0-9]+:	62 9f c8                	spflt  %r8d
[ 	]*[a-f0-9]+:	62 8f c8                	spflt  %r8
[ 	]*[a-f0-9]+:	66 62 df c8             	spflt  %ax
[ 	]*[a-f0-9]+:	62 df c8                	spflt  %eax
[ 	]*[a-f0-9]+:	62 cf c8                	spflt  %rax
#pass

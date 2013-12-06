#as: -march=l1om
#objdump: -dw
#name: L1OMni REX ops

.*: +file format .*

Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	66 40 62 f0 c0          	rex bsfi %ax,%ax
[ 	]*[a-f0-9]+:	66 40 62 b0 c0          	rex bsfi %ax,%r8w
[ 	]*[a-f0-9]+:	66 40 62 d0 c0          	rex bsfi %r8w,%ax
[ 	]*[a-f0-9]+:	66 40 62 90 c0          	rex bsfi %r8w,%r8w
[ 	]*[a-f0-9]+:	66 48 62 f0 c0          	rex.W bsfi %ax,%ax
[ 	]*[a-f0-9]+:	66 48 62 b0 c0          	rex.W bsfi %ax,%r8w
[ 	]*[a-f0-9]+:	66 48 62 d0 c0          	rex.W bsfi %r8w,%ax
[ 	]*[a-f0-9]+:	66 48 62 90 c0          	rex.W bsfi %r8w,%r8w
[ 	]*[a-f0-9]+:	66 44 62 f0 c0          	rex.R bsfi %ax,%ax
[ 	]*[a-f0-9]+:	66 44 62 b0 c0          	rex.R bsfi %ax,%r8w
[ 	]*[a-f0-9]+:	66 44 62 d0 c0          	rex.R bsfi %r8w,%ax
[ 	]*[a-f0-9]+:	66 44 62 90 c0          	rex.R bsfi %r8w,%r8w
[ 	]*[a-f0-9]+:	66 42 62 f0 c0          	rex.X bsfi %ax,%ax
[ 	]*[a-f0-9]+:	66 42 62 b0 c0          	rex.X bsfi %ax,%r8w
[ 	]*[a-f0-9]+:	66 42 62 d0 c0          	rex.X bsfi %r8w,%ax
[ 	]*[a-f0-9]+:	66 42 62 90 c0          	rex.X bsfi %r8w,%r8w
[ 	]*[a-f0-9]+:	66 41 62 f0 c0          	rex.B bsfi %ax,%ax
[ 	]*[a-f0-9]+:	66 41 62 b0 c0          	rex.B bsfi %ax,%r8w
[ 	]*[a-f0-9]+:	66 41 62 d0 c0          	rex.B bsfi %r8w,%ax
[ 	]*[a-f0-9]+:	66 41 62 90 c0          	rex.B bsfi %r8w,%r8w
[ 	]*[a-f0-9]+:	66 4f 62 f0 c0          	rex.WRXB bsfi %ax,%ax
[ 	]*[a-f0-9]+:	66 4f 62 b0 c0          	rex.WRXB bsfi %ax,%r8w
[ 	]*[a-f0-9]+:	66 4f 62 d0 c0          	rex.WRXB bsfi %r8w,%ax
[ 	]*[a-f0-9]+:	66 4f 62 90 c0          	rex.WRXB bsfi %r8w,%r8w
[ 	]*[a-f0-9]+:	40 62 f0 c0             	rex bsfi %eax,%eax
[ 	]*[a-f0-9]+:	40 62 b0 c0             	rex bsfi %eax,%r8d
[ 	]*[a-f0-9]+:	40 62 d0 c0             	rex bsfi %r8d,%eax
[ 	]*[a-f0-9]+:	40 62 90 c0             	rex bsfi %r8d,%r8d
[ 	]*[a-f0-9]+:	48 62 f0 c0             	rex.W bsfi %eax,%eax
[ 	]*[a-f0-9]+:	48 62 b0 c0             	rex.W bsfi %eax,%r8d
[ 	]*[a-f0-9]+:	48 62 d0 c0             	rex.W bsfi %r8d,%eax
[ 	]*[a-f0-9]+:	48 62 90 c0             	rex.W bsfi %r8d,%r8d
[ 	]*[a-f0-9]+:	44 62 f0 c0             	rex.R bsfi %eax,%eax
[ 	]*[a-f0-9]+:	44 62 b0 c0             	rex.R bsfi %eax,%r8d
[ 	]*[a-f0-9]+:	44 62 d0 c0             	rex.R bsfi %r8d,%eax
[ 	]*[a-f0-9]+:	44 62 90 c0             	rex.R bsfi %r8d,%r8d
[ 	]*[a-f0-9]+:	42 62 f0 c0             	rex.X bsfi %eax,%eax
[ 	]*[a-f0-9]+:	42 62 b0 c0             	rex.X bsfi %eax,%r8d
[ 	]*[a-f0-9]+:	42 62 d0 c0             	rex.X bsfi %r8d,%eax
[ 	]*[a-f0-9]+:	42 62 90 c0             	rex.X bsfi %r8d,%r8d
[ 	]*[a-f0-9]+:	41 62 f0 c0             	rex.B bsfi %eax,%eax
[ 	]*[a-f0-9]+:	41 62 b0 c0             	rex.B bsfi %eax,%r8d
[ 	]*[a-f0-9]+:	41 62 d0 c0             	rex.B bsfi %r8d,%eax
[ 	]*[a-f0-9]+:	41 62 90 c0             	rex.B bsfi %r8d,%r8d
[ 	]*[a-f0-9]+:	4f 62 f0 c0             	rex.WRXB bsfi %eax,%eax
[ 	]*[a-f0-9]+:	4f 62 b0 c0             	rex.WRXB bsfi %eax,%r8d
[ 	]*[a-f0-9]+:	4f 62 d0 c0             	rex.WRXB bsfi %r8d,%eax
[ 	]*[a-f0-9]+:	4f 62 90 c0             	rex.WRXB bsfi %r8d,%r8d
[ 	]*[a-f0-9]+:	40 62 e0 c0             	rex bsfi %rax,%rax
[ 	]*[a-f0-9]+:	40 62 a0 c0             	rex bsfi %rax,%r8
[ 	]*[a-f0-9]+:	40 62 c0 c0             	rex bsfi %r8,%rax
[ 	]*[a-f0-9]+:	40 62 80 c0             	rex bsfi %r8,%r8
[ 	]*[a-f0-9]+:	48 62 e0 c0             	rex.W bsfi %rax,%rax
[ 	]*[a-f0-9]+:	48 62 a0 c0             	rex.W bsfi %rax,%r8
[ 	]*[a-f0-9]+:	48 62 c0 c0             	rex.W bsfi %r8,%rax
[ 	]*[a-f0-9]+:	48 62 80 c0             	rex.W bsfi %r8,%r8
[ 	]*[a-f0-9]+:	44 62 e0 c0             	rex.R bsfi %rax,%rax
[ 	]*[a-f0-9]+:	44 62 a0 c0             	rex.R bsfi %rax,%r8
[ 	]*[a-f0-9]+:	44 62 c0 c0             	rex.R bsfi %r8,%rax
[ 	]*[a-f0-9]+:	44 62 80 c0             	rex.R bsfi %r8,%r8
[ 	]*[a-f0-9]+:	42 62 e0 c0             	rex.X bsfi %rax,%rax
[ 	]*[a-f0-9]+:	42 62 a0 c0             	rex.X bsfi %rax,%r8
[ 	]*[a-f0-9]+:	42 62 c0 c0             	rex.X bsfi %r8,%rax
[ 	]*[a-f0-9]+:	42 62 80 c0             	rex.X bsfi %r8,%r8
[ 	]*[a-f0-9]+:	41 62 e0 c0             	rex.B bsfi %rax,%rax
[ 	]*[a-f0-9]+:	41 62 a0 c0             	rex.B bsfi %rax,%r8
[ 	]*[a-f0-9]+:	41 62 c0 c0             	rex.B bsfi %r8,%rax
[ 	]*[a-f0-9]+:	41 62 80 c0             	rex.B bsfi %r8,%r8
[ 	]*[a-f0-9]+:	4f 62 e0 c0             	rex.WRXB bsfi %rax,%rax
[ 	]*[a-f0-9]+:	4f 62 a0 c0             	rex.WRXB bsfi %rax,%r8
[ 	]*[a-f0-9]+:	4f 62 c0 c0             	rex.WRXB bsfi %r8,%rax
[ 	]*[a-f0-9]+:	4f 62 80 c0             	rex.WRXB bsfi %r8,%r8
[ 	]*[a-f0-9]+:	67 40 62 eb 44 80 08    	rex clevict1 0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 40 62 6b 44 80 08    	rex clevict1 0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 40 62 ab 44 80 08    	rex clevict1 0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 40 62 2b 44 80 08    	rex clevict1 0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 48 62 eb 44 80 08    	rex.W clevict1 0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 48 62 6b 44 80 08    	rex.W clevict1 0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 48 62 ab 44 80 08    	rex.W clevict1 0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 48 62 2b 44 80 08    	rex.W clevict1 0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 44 62 eb 44 80 08    	rex.R clevict1 0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 44 62 6b 44 80 08    	rex.R clevict1 0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 44 62 ab 44 80 08    	rex.R clevict1 0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 44 62 2b 44 80 08    	rex.R clevict1 0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 42 62 eb 44 80 08    	rex.X clevict1 0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 42 62 6b 44 80 08    	rex.X clevict1 0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 42 62 ab 44 80 08    	rex.X clevict1 0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 42 62 2b 44 80 08    	rex.X clevict1 0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 41 62 eb 44 80 08    	rex.B clevict1 0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 41 62 6b 44 80 08    	rex.B clevict1 0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 41 62 ab 44 80 08    	rex.B clevict1 0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 41 62 2b 44 80 08    	rex.B clevict1 0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 4f 62 eb 44 80 08    	rex.WRXB clevict1 0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 4f 62 6b 44 80 08    	rex.WRXB clevict1 0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 4f 62 ab 44 80 08    	rex.WRXB clevict1 0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 4f 62 2b 44 80 08    	rex.WRXB clevict1 0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	40 62 eb 44 80 08       	rex clevict1 0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	40 62 6b 44 80 08       	rex clevict1 0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	40 62 ab 44 80 08       	rex clevict1 0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	40 62 2b 44 80 08       	rex clevict1 0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	48 62 eb 44 80 08       	rex.W clevict1 0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	48 62 6b 44 80 08       	rex.W clevict1 0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	48 62 ab 44 80 08       	rex.W clevict1 0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	48 62 2b 44 80 08       	rex.W clevict1 0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	44 62 eb 44 80 08       	rex.R clevict1 0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	44 62 6b 44 80 08       	rex.R clevict1 0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	44 62 ab 44 80 08       	rex.R clevict1 0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	44 62 2b 44 80 08       	rex.R clevict1 0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	42 62 eb 44 80 08       	rex.X clevict1 0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	42 62 6b 44 80 08       	rex.X clevict1 0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	42 62 ab 44 80 08       	rex.X clevict1 0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	42 62 2b 44 80 08       	rex.X clevict1 0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	41 62 eb 44 80 08       	rex.B clevict1 0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	41 62 6b 44 80 08       	rex.B clevict1 0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	41 62 ab 44 80 08       	rex.B clevict1 0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	41 62 2b 44 80 08       	rex.B clevict1 0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	4f 62 eb 44 80 08       	rex.WRXB clevict1 0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	4f 62 6b 44 80 08       	rex.WRXB clevict1 0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	4f 62 ab 44 80 08       	rex.WRXB clevict1 0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	4f 62 2b 44 80 08       	rex.WRXB clevict1 0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	67 40 62 cb 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 40 62 4b 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 40 62 8b 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 40 62 0b 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 48 62 cb 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 48 62 4b 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 48 62 8b 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 48 62 0b 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 44 62 cb 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 44 62 4b 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 44 62 8b 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 44 62 0b 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 42 62 cb 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 42 62 4b 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 42 62 8b 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 42 62 0b 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 41 62 cb 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 41 62 4b 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 41 62 8b 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 41 62 0b 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	67 4f 62 cb 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%eax,%eax,4\)
[ 	]*[a-f0-9]+:	67 4f 62 4b 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%eax,%r8d,4\)
[ 	]*[a-f0-9]+:	67 4f 62 8b 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%r8d,%eax,4\)
[ 	]*[a-f0-9]+:	67 4f 62 0b 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%r8d,%r8d,4\)
[ 	]*[a-f0-9]+:	40 62 cb 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	40 62 4b 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	40 62 8b 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	40 62 0b 8c 80 08 00 00 00 	rex vprefetch1 \$0x1,0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	48 62 cb 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	48 62 4b 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	48 62 8b 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	48 62 0b 8c 80 08 00 00 00 	rex.W vprefetch1 \$0x1,0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	44 62 cb 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	44 62 4b 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	44 62 8b 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	44 62 0b 8c 80 08 00 00 00 	rex.R vprefetch1 \$0x1,0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	42 62 cb 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	42 62 4b 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	42 62 8b 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	42 62 0b 8c 80 08 00 00 00 	rex.X vprefetch1 \$0x1,0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	41 62 cb 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	41 62 4b 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	41 62 8b 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	41 62 0b 8c 80 08 00 00 00 	rex.B vprefetch1 \$0x1,0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	4f 62 cb 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%rax,%rax,4\)
[ 	]*[a-f0-9]+:	4f 62 4b 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%rax,%r8,4\)
[ 	]*[a-f0-9]+:	4f 62 8b 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%r8,%rax,4\)
[ 	]*[a-f0-9]+:	4f 62 0b 8c 80 08 00 00 00 	rex.WRXB vprefetch1 \$0x1,0x8\(%r8,%r8,4\)
[ 	]*[a-f0-9]+:	66 40 62 df c0          	rex delay %ax
[ 	]*[a-f0-9]+:	48 62 df c0             	rex.W delay %eax
[ 	]*[a-f0-9]+:	44 62 cf c0             	rex.R delay %rax
[ 	]*[a-f0-9]+:	66 42 62 9f c0          	rex.X delay %r8w
[ 	]*[a-f0-9]+:	41 62 9f c0             	rex.B delay %r8d
[ 	]*[a-f0-9]+:	41 62 8f c0             	rex.B delay %r8
[ 	]*[a-f0-9]+:	44 62 dc f0             	rex.R vkmov %k6,%eax
[ 	]*[a-f0-9]+:	44 62 9c f0             	rex.R vkmov %k6,%r8d
[ 	]*[a-f0-9]+:	40 62 ec c6             	rex vkmov %eax,%k6
[ 	]*[a-f0-9]+:	40 62 cc c6             	rex vkmov %r8d,%k6
[ 	]*[a-f0-9]+:	41 62 ec c6             	rex.B vkmov %eax,%k6
[ 	]*[a-f0-9]+:	41 62 cc c6             	rex.B vkmov %r8d,%k6
[ 	]*[a-f0-9]+:	48 62 cc c6             	rex.W vkmov %r8d,%k6
[ 	]*[a-f0-9]+:	40 62 cd f6             	rex vkand %k6,%k6
[ 	]*[a-f0-9]+:	48 62 cd f6             	rex.W vkand %k6,%k6
[ 	]*[a-f0-9]+:	44 62 cd f6             	rex.R vkand %k6,%k6
[ 	]*[a-f0-9]+:	42 62 cd f6             	rex.X vkand %k6,%k6
[ 	]*[a-f0-9]+:	41 62 cd f6             	rex.B vkand %k6,%k6
[ 	]*[a-f0-9]+:	4f 62 cd f6             	rex.WRXB vkand %k6,%k6
[ 	]*[a-f0-9]+:	40 62 fc f6             	rex vkmov %k6,%k6
[ 	]*[a-f0-9]+:	48 62 fc f6             	rex.W vkmov %k6,%k6
[ 	]*[a-f0-9]+:	44 62 fc f6             	rex.R vkmov %k6,%k6
[ 	]*[a-f0-9]+:	42 62 fc f6             	rex.X vkmov %k6,%k6
[ 	]*[a-f0-9]+:	41 62 fc f6             	rex.B vkmov %k6,%k6
[ 	]*[a-f0-9]+:	4f 62 fc f6             	rex.WRXB vkmov %k6,%k6
[ 	]*[a-f0-9]+:	67 40 d6 90 00 a7 54 80 02 	rex vxorpi 0x8\(%eax,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 40 d6 92 00 a7 54 80 02 	rex vxorpi 0x8\(%eax,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 40 d6 91 00 a7 54 80 02 	rex vxorpi 0x8\(%r8d,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 40 d6 93 00 a7 54 80 02 	rex vxorpi 0x8\(%r8d,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 48 d6 90 00 a7 54 80 02 	rex.W vxorpi 0x8\(%eax,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 48 d6 92 00 a7 54 80 02 	rex.W vxorpi 0x8\(%eax,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 48 d6 91 00 a7 54 80 02 	rex.W vxorpi 0x8\(%r8d,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 48 d6 93 00 a7 54 80 02 	rex.W vxorpi 0x8\(%r8d,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 44 d6 90 00 a7 54 80 02 	rex.R vxorpi 0x8\(%eax,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 44 d6 92 00 a7 54 80 02 	rex.R vxorpi 0x8\(%eax,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 44 d6 91 00 a7 54 80 02 	rex.R vxorpi 0x8\(%r8d,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 44 d6 93 00 a7 54 80 02 	rex.R vxorpi 0x8\(%r8d,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 42 d6 90 00 a7 54 80 02 	rex.X vxorpi 0x8\(%eax,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 42 d6 92 00 a7 54 80 02 	rex.X vxorpi 0x8\(%eax,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 42 d6 91 00 a7 54 80 02 	rex.X vxorpi 0x8\(%r8d,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 42 d6 93 00 a7 54 80 02 	rex.X vxorpi 0x8\(%r8d,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 41 d6 90 00 a7 54 80 02 	rex.B vxorpi 0x8\(%eax,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 41 d6 92 00 a7 54 80 02 	rex.B vxorpi 0x8\(%eax,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 41 d6 91 00 a7 54 80 02 	rex.B vxorpi 0x8\(%r8d,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 41 d6 93 00 a7 54 80 02 	rex.B vxorpi 0x8\(%r8d,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 4f d6 90 00 a7 54 80 02 	rex.WRXB vxorpi 0x8\(%eax,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 4f d6 92 00 a7 54 80 02 	rex.WRXB vxorpi 0x8\(%eax,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 4f d6 91 00 a7 54 80 02 	rex.WRXB vxorpi 0x8\(%r8d,%eax,4\)\{nt\}\{1to16\},%v0,%v2
[ 	]*[a-f0-9]+:	67 4f d6 93 00 a7 54 80 02 	rex.WRXB vxorpi 0x8\(%r8d,%r8d,4\)\{nt\}\{1to16\},%v0,%v2
#pass

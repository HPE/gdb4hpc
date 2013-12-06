#as: -march=l1om
#objdump: -drw
#name: L1OMni Mask ops

.*: +file format .*

Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	62 fc c8                	vkmov  %k1,%k0
[ 	]*[a-f0-9]+:	62 fc f9                	vkmov  %k7,%k1
[ 	]*[a-f0-9]+:	66 62 dc d0             	vkmov  %k2,%ax
[ 	]*[a-f0-9]+:	62 dc d0                	vkmov  %k2,%eax
[ 	]*[a-f0-9]+:	66 62 dc d3             	vkmov  %k2,%bx
[ 	]*[a-f0-9]+:	62 dc d3                	vkmov  %k2,%ebx
[ 	]*[a-f0-9]+:	66 62 dc d1             	vkmov  %k2,%cx
[ 	]*[a-f0-9]+:	62 dc d1                	vkmov  %k2,%ecx
[ 	]*[a-f0-9]+:	66 62 9c d7             	vkmov  %k2,%r15w
[ 	]*[a-f0-9]+:	62 9c d7                	vkmov  %k2,%r15d
[ 	]*[a-f0-9]+:	66 62 ec c2             	vkmov  %ax,%k2
[ 	]*[a-f0-9]+:	62 ec c2                	vkmov  %eax,%k2
[ 	]*[a-f0-9]+:	66 62 ec da             	vkmov  %bx,%k2
[ 	]*[a-f0-9]+:	62 ec da                	vkmov  %ebx,%k2
[ 	]*[a-f0-9]+:	66 62 ec ca             	vkmov  %cx,%k2
[ 	]*[a-f0-9]+:	62 ec ca                	vkmov  %ecx,%k2
[ 	]*[a-f0-9]+:	66 62 cc c2             	vkmov  %r8w,%k2
[ 	]*[a-f0-9]+:	62 cc c2                	vkmov  %r8d,%k2
[ 	]*[a-f0-9]+:	66 62 cc fa             	vkmov  %r15w,%k2
[ 	]*[a-f0-9]+:	62 cc fa                	vkmov  %r15d,%k2
#pass

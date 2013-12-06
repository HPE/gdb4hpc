#as: -mtune=l1om
#source: nops-3.s
#objdump: -drw
#name: -mtune=l1om nops 3

.*: +file format .*

Disassembly of section .text:

0+ <nop>:
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	eb 1d                	jmp    20 <nop\+0x20>
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	89 c3                	mov    %eax,%ebx
[ 	]*[a-f0-9]+:	eb 0c                	jmp    30 <nop\+0x30>
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	90                   	nop
#pass

#as:
#objdump: -dw
#name: l1om in x86-64

.*: +file format .*


Disassembly of section .text:

0+ <.text>:
[ 	]*[a-f0-9]+:	d6 00 01 10 44 24 ff 	vloadd -0x40\(%rsp\),%v0\{%k1\}
#pass

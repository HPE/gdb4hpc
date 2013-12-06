#source: x86-64-l1om.s
#as:
#objdump: -dw -Mintel
#name: l1om in x86-64 (Intel mode)

.*: +file format .*


Disassembly of section .text:

0+ <.text>:
[ 	]*[a-f0-9]+:	d6 00 01 10 44 24 ff 	vloadd v0\{k1\},\[rsp-0x40\]
#pass

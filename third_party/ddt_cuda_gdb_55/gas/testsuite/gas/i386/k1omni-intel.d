#as: -march=k1om
#objdump: -dwMintel
#name: LRB2 insns (Intel disassembly)
#source: k1omni.s

.*: +file format .*


Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	c5 f8 41 d1          	kand   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 42 d1          	kandn  k2,k1
[ 	]*[a-f0-9]+:	c5 f8 43 d1          	kandnr k2,k1
[ 	]*[a-f0-9]+:	c5 f8 48 d1          	kmerge2l1h k2,k1
[ 	]*[a-f0-9]+:	c5 f8 49 d1          	kmerge2l1l k2,k1
[ 	]*[a-f0-9]+:	c5 f8 90 d1          	kmov   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 44 d1          	knot   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 45 d1          	kor    k2,k1
[ 	]*[a-f0-9]+:	c5 f8 98 d1          	kortest k2,k1
[ 	]*[a-f0-9]+:	c5 f8 46 d1          	kxnor  k2,k1
[ 	]*[a-f0-9]+:	c5 f8 47 d1          	kxor   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 92 d0          	kmov   k2,rax
[ 	]*[a-f0-9]+:	c4 c1 78 92 d1       	kmov   k2,r9
[ 	]*[a-f0-9]+:	c5 f8 93 c1          	kmov   rax,k1
[ 	]*[a-f0-9]+:	c5 78 93 c9          	kmov   r9,k1
[ 	]*[a-f0-9]+:	c5 f0 95 c2          	kconcath rax,k1,k2
[ 	]*[a-f0-9]+:	c5 70 95 ca          	kconcath r9,k1,k2
[ 	]*[a-f0-9]+:	c5 f0 97 c2          	kconcatl rax,k1,k2
[ 	]*[a-f0-9]+:	c5 70 97 ca          	kconcatl r9,k1,k2
[ 	]*[a-f0-9]+:	c4 e3 79 3e cb 03    	kextract k1,rbx,0x3
[ 	]*[a-f0-9]+:	c4 c3 79 3e cf 03    	kextract k1,r15,0x3
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 49 04 	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 49 04 	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 49 04 	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 49 04 	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 49 04 	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 49 04 	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 49 04 	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 49 04 	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 19 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 68 99 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 19 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 49 04 	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 49 04 	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 49 04 	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 49 04 	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 49 04 	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 49 04 	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 49 04 	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 49 04 	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 49 04 	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 49 04 	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 49 04 	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 49 04 	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 49 04 	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 49 04 	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 49 04 	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 19 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 68 99 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 19 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 49 04 	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 19 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 68 99 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 19 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 49 04 	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 49 04 	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 49 04 	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 49 04 	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 49 04 	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 49 04 	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 49 08 	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 49 04 	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 49 04 	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 49 04 	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 49 04 	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 49 04 	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 49 04 	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 49 04 	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 49 04 	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 49 04 	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 49 04 	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 49 04 	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 49 04 	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 49 04 	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 49 04 	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 49 04 	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 49 04 	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 49 04 	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 49 04 	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 49 04 	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 49 04 	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 49 04 	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 49 04 	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 49 04 	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 49 04 	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 49 04 	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 49 04 	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 49 04 	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 49 04 	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 49 04 	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 49 04 	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 49 04 	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 49 04 	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 49 04 	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 49 04 	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 49 04 	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 49 04 	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 49 04 	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 49 04 	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 49 04 	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 49 04 	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 49 04 	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 49 04 	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 49 04 	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 49 04 	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 49 04 	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 49 04 	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 49 04 	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 49 04 	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 49 04 	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 49 04 	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 49 04 	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 49 04 	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 89 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 49 08 	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 49 04 	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 49 04 	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 49 04 	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 49 04 	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3,0x64,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3,0x64,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 3a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 49 04 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 49 04 	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 49 04 	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 49 04 	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 49 04 	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 49 04 	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{cdab\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{badc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{cccc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{dddd\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{dacb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{cdab\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{badc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{cccc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{dddd\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{dacb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x2
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 49 20 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 49 08 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 49 20 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 49 08 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 49 20 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 49 08 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 79 19 42 cb    	vgetexpps zmm1\{k1\},zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 cb    	vgetexpps zmm1\{k1\},zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 cb    	vgetexpps zmm1\{k1\},zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 cb    	vgetexpps zmm1\{k1\},zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 cb    	vgetexpps zmm1\{k1\},zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 cb    	vgetexpps zmm1\{k1\},zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 cb    	vgetexpps zmm1\{k1\},zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 42 cb    	vgetexpps zmm1\{k1\},zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 89 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 49 04 	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 cb    	vgetexppd zmm1\{k1\},zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 cb    	vgetexppd zmm1\{k1\},zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 f9 49 42 cb    	vgetexppd zmm1\{k1\},zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 f9 59 42 cb    	vgetexppd zmm1\{k1\},zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 f9 69 42 cb    	vgetexppd zmm1\{k1\},zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 f9 79 42 cb    	vgetexppd zmm1\{k1\},zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 f9 39 42 cb    	vgetexppd zmm1\{k1\},zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 f9 c9 42 cb    	vgetexppd zmm1\{k1\},zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 f9 a9 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 49 04 	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 f9 99 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 f9 a9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 f9 b9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 49 04 	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 49 04 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 49 04 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 52 49 04 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 26 cb 07 	vgetmantps zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 26 49 04 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 49 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 59 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 69 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 79 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 39 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb c9 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 49 04 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 49 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 59 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 69 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 79 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 39 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb c9 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 49 04 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 49 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 59 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 69 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 79 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 39 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 c9 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 49 04 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 49 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 59 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 69 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 79 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 39 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 c9 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 49 04 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 f3 07 	vpslld zmm1,zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 f3 07 	vpslld zmm1,zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 f3 07 	vpslld zmm1,zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 f3 07 	vpslld zmm1,zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 f3 07 	vpslld zmm1,zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 f3 07 	vpslld zmm1,zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 39 72 f3 07 	vpslld zmm1,zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 71 04 07 	vpslld zmm1,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 e3 07 	vpsrad zmm1,zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 e3 07 	vpsrad zmm1,zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 e3 07 	vpsrad zmm1,zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 e3 07 	vpsrad zmm1,zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 e3 07 	vpsrad zmm1,zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 e3 07 	vpsrad zmm1,zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 39 72 e3 07 	vpsrad zmm1,zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 61 04 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 d3 07 	vpsrld zmm1,zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 d3 07 	vpsrld zmm1,zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 d3 07 	vpsrld zmm1,zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 d3 07 	vpsrld zmm1,zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 d3 07 	vpsrld zmm1,zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 d3 07 	vpsrld zmm1,zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 39 72 d3 07 	vpsrld zmm1,zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 51 04 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 39 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 99 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 a9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{ru\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 b9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rz\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 49 04 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 39 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 99 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 a9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{ru\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 b9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rz\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 49 04 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f2 78 89 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 39 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 49 40 	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 39 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 49 40 	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 39 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 49 04 	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 39 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 49 10 	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 39 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 49 40 	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 89 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 39 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 49 40 	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 29 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 49 40 	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 29 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 49 40 	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 29 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 49 04 	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 29 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 49 10 	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 29 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 79 49 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 49 40 	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 19 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 49 20 	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 19 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 49 20 	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 49 02 	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 79 29 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 79 39 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 49 08 	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 89 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 79 29 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 79 39 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 79 49 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 79 59 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 79 69 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 49 20 	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d5 09    	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 49 10 	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f8 89 d1 09    	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 49 10 	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 09    	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 49 02 	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 1b 09    	vbroadcastf64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 49 04 	vbroadcastf64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 09    	vbroadcastf64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 19 09    	vbroadcastsd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 49 10 	vbroadcastsd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 09    	vbroadcastsd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 89 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 49 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 49 40 	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 49 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 49 40 	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 79 49 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 49 04 	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 49 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 49 10 	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 49 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 49 40 	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d4 09    	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 49 10 	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f8 89 d0 09    	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 49 10 	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f 09    	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 49 02 	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 5b 09    	vbroadcasti64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 49 04 	vbroadcasti64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 09    	vbroadcasti64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 59 09    	vpbroadcastq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 49 10 	vpbroadcastq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 09    	vpbroadcastq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 71 09 03 ca 08 	valignd zmm1\{k1\},zmm1,zmm2,0x8
[ 	]*[a-f0-9]+:	62 f3 71 09 03 09 08 	valignd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\],0x8
[ 	]*[a-f0-9]+:	62 f3 71 09 03 49 02 08 	valignd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\+0x80\],0x8
[ 	]*[a-f0-9]+:	62 f2 79 09 6b ca    	vcvtps2srgb8 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 6b ca    	vcvtps2srgb8 zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 09    	vcvtps2srgb8 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 6b 09    	vcvtps2srgb8 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 49 02 	vcvtps2srgb8 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 ca    	vexp223ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 c8 ca    	vexp223ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 09    	vexp223ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 c8 09    	vexp223ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 49 02 	vexp223ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 ca    	vlog2ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 c9 ca    	vlog2ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 09    	vlog2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 c9 09    	vlog2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 49 02 	vlog2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 ca ca    	vrcp23ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 ca ca    	vrcp23ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 09    	vrcp23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 ca 09    	vrcp23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 49 02 	vrcp23ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 cb ca    	vrsqrt23ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 cb ca    	vrsqrt23ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 09    	vrsqrt23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 cb 09    	vrsqrt23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 49 02 	vrsqrt23ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 71 09 36 ca    	vpermd zmm1\{k1\},zmm1,zmm2
[ 	]*[a-f0-9]+:	62 f2 71 09 36 09    	vpermd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 71 09 36 49 02 	vpermd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f3 79 09 07 ca 07 	vpermf32x4 zmm1\{k1\},zmm2,0x7
[ 	]*[a-f0-9]+:	62 f3 79 89 07 09 07 	vpermf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 07 49 02 07 	vpermf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\],0x7
[ 	]*[a-f0-9]+:	62 f1 79 09 70 ca 07 	vpshufd zmm1\{k1\},zmm2,0x7
[ 	]*[a-f0-9]+:	62 f1 79 89 70 09 07 	vpshufd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 79 09 70 49 02 07 	vpshufd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\],0x7
[ 	]*[a-f0-9]+:	62 f2 79 89 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d5 49 40 	vpackstorehps ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d1 49 40 	vpackstorelps ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 89 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 78 09 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 78 39 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 f1 78 49 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 78 59 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 78 69 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 78 29 49 04 	vmovaps ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d6 49 40 	vpackstorehgps ZMMWORD PTR \[rcx\+0x80\],zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d2 49 40 	vpackstorelgps ZMMWORD PTR \[rcx\+0x80\],zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 f2 79 49 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 6a 49 04 	vmovgps ZMMWORD PTR \[rcx\+0x80\],zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d5 09    	vpackstorehpd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d5 49 10 	vpackstorehpd ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d5 09    	vpackstorehpd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 89 d1 09    	vpackstorelpd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d1 49 10 	vpackstorelpd ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d1 09    	vpackstorelpd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 89 29 09    	vmovapd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 08 29 49 02 	vmovapd ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 09 29 09    	vmovapd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 89 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 49 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d4 49 40 	vpackstorehd ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 49 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d0 49 40 	vpackstoreld ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 89 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 79 09 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 79 49 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 79 59 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 79 69 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 78 7f 49 04 	vmovdqa32 ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d4 09    	vpackstorehq ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d4 49 10 	vpackstorehq ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d4 09    	vpackstorehq ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 89 d0 09    	vpackstorelq ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d0 49 10 	vpackstorelq ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d0 09    	vpackstorelq ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 89 7f 09    	vmovdqa64 ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 08 7f 49 02 	vmovdqa64 ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 09 7f 09    	vmovdqa64 ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 19 28 ca    	vmovapd zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 28 ca    	vmovapd zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 28 ca    	vmovapd zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 28 ca    	vmovapd zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 28 ca    	vmovapd zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 28 ca    	vmovapd zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 28 ca    	vmovapd zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 78 19 28 ca    	vmovaps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 78 29 28 ca    	vmovaps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 ca    	vmovaps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 ca    	vmovaps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 ca    	vmovaps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 ca    	vmovaps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 78 39 28 ca    	vmovaps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 79 19 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 79 29 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 79 49 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 79 39 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 19 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 19 68 ca    	vmovgps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 79 29 68 ca    	vmovgps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 ca    	vmovgps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 ca    	vmovgps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 ca    	vmovgps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 ca    	vmovgps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 79 39 68 ca    	vmovgps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 19 69 ca    	vmovpgps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 ca    	vmovpgps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 ca    	vmovpgps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 ca    	vmovpgps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 ca    	vmovpgps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 ca    	vmovpgps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 ca    	vmovpgps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 09 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 49 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 4c 52 40 	vpscatterdd ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 09 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 39 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 b2 79 49 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 4c 52 40 	vscatterdps ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 09 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 39 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 b2 79 49 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 4c 52 40 	vscatterdgps ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 4c 52 40 	vgatherpf0dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 44 52 40 	vgatherpf0hintdps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 54 52 40 	vgatherpf1dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 6c 52 40 	vscatterpf0dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 64 52 40 	vscatterpf0hintdps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 74 52 40 	vscatterpf1dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 84 55 99 00 00 00 	vgatherpf0hintdpd \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 44 52 10 	vgatherpf0hintdpd \[rdx\+zmm10\*2\+0x80\]\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdpd \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 64 52 10 	vscatterpf0hintdpd \[rdx\+zmm10\*2\+0x80\]\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 39 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float16\}
[ 	]*[a-f0-9]+:	62 b2 79 49 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 4c 52 20 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 f9 89 92 8c 55 99 00 00 00 	vgatherdpd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 4c 52 10 	vgatherdpd zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 8c 55 99 00 00 00 	vgatherdpd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 89 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 29 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 4c 52 20 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 89 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 19 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float11A\}
[ 	]*[a-f0-9]+:	62 b2 79 29 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float11B\}
[ 	]*[a-f0-9]+:	62 b2 79 39 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float10C\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 4c 52 20 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 89 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 49 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 4c 52 20 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 f9 89 90 8c 55 99 00 00 00 	vpgatherdq zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 4c 52 10 	vpgatherdq zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 8c 55 99 00 00 00 	vpgatherdq zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 f9 89 a0 8c 55 99 00 00 00 	vpscatterdq ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 4c 52 10 	vpscatterdq ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 8c 55 99 00 00 00 	vpscatterdq ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	c4 e2 7b f0 d8       	bitinterleave11 ebx,eax
[ 	]*[a-f0-9]+:	c4 42 7b f0 f9       	bitinterleave11 r15d,r9d
[ 	]*[a-f0-9]+:	c4 e2 7a f0 d8       	bitinterleave21 ebx,eax
[ 	]*[a-f0-9]+:	c4 42 7a f0 f9       	bitinterleave21 r15d,r9d
[ 	]*[a-f0-9]+:	c5 fa bd d8          	lzcnt  ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7a bd f9       	lzcnt  r15d,r9d
[ 	]*[a-f0-9]+:	c4 e2 7b f1 d8       	quadmask ebx,eax
[ 	]*[a-f0-9]+:	c4 42 7b f1 f9       	quadmask r15d,r9d
[ 	]*[a-f0-9]+:	c5 fa b8 d8          	popcnt ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7a b8 f9       	popcnt r15d,r9d
[ 	]*[a-f0-9]+:	c5 fa bc d8          	tzcnt  ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7a bc f9       	tzcnt  r15d,r9d
[ 	]*[a-f0-9]+:	c5 fb bc d8          	tzcnti ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7b bc f9       	tzcnti r15d,r9d
[ 	]*[a-f0-9]+:	c4 e2 fb f0 d8       	bitinterleave11 rbx,rax
[ 	]*[a-f0-9]+:	c4 42 fb f0 f9       	bitinterleave11 r15,r9
[ 	]*[a-f0-9]+:	c4 e2 fa f0 d8       	bitinterleave21 rbx,rax
[ 	]*[a-f0-9]+:	c4 42 fa f0 f9       	bitinterleave21 r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fa bd d8       	lzcnt  rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fa bd f9       	lzcnt  r15,r9
[ 	]*[a-f0-9]+:	c4 e2 fb f1 d8       	quadmask rbx,rax
[ 	]*[a-f0-9]+:	c4 42 fb f1 f9       	quadmask r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fa b8 d8       	popcnt rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fa b8 f9       	popcnt r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fa bc d8       	tzcnt  rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fa bc f9       	tzcnt  r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fb bc d8       	tzcnti rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fb bc f9       	tzcnti r15,r9
[ 	]*[a-f0-9]+:	c5 fb ae 39          	clevict0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7b 08 ae 79 02 	clevict0 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 fa ae 39          	clevict1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 08 ae 79 02 	clevict1 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 09          	vprefetch0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 49 02 	vprefetch0 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 11          	vprefetch1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 51 02 	vprefetch1 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 19          	vprefetch2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 59 02 	vprefetch2 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 29          	vprefetche0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 69 02 	vprefetche0 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 31          	vprefetche1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 71 02 	vprefetche1 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 39          	vprefetche2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 79 02 	vprefetche2 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 21          	vprefetchenta BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 61 02 	vprefetchenta \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 01          	vprefetchnta BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 41 02 	vprefetchnta \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 fa ae f0          	delay  eax
[ 	]*[a-f0-9]+:	c4 c1 7a ae f1       	delay  r9d
[ 	]*[a-f0-9]+:	c5 fb ae f0          	spflt  eax
[ 	]*[a-f0-9]+:	c4 c1 7b ae f1       	spflt  r9d
[ 	]*[a-f0-9]+:	c4 e1 fa ae f0       	delay  rax
[ 	]*[a-f0-9]+:	c4 c1 fa ae f1       	delay  r9
[ 	]*[a-f0-9]+:	c4 e1 fb ae f0       	spflt  rax
[ 	]*[a-f0-9]+:	c4 c1 fb ae f1       	spflt  r9
[ 	]*[a-f0-9]+:	62 81 59 11 db e6    	vpandd zmm20\{k1\},zmm20,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 01 59 11 db f6    	vpandd zmm30\{k1\},zmm20,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 01 09 11 db f6    	vpandd zmm30\{k1\},zmm30,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 81 09 11 db e6    	vpandd zmm20\{k1\},zmm30,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 81 29 19 db e6    	vpandd zmm20\{k1\},zmm10,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 59 01 72 31 07 	vpslld zmm20,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 91 59 01 72 f6 07 	vpslld zmm20,zmm30,0x7
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 78 56 34 12 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0x12345678
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 78 56 34 12 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0x12345678
[ 	]*[a-f0-9]+:	62 72 69 09 5c 55 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbp\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 65 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbp\+0x0\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 24 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rsp\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 24 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rsp\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 95 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbp\+0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a5 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbp\+0x99\]
[ 	]*[a-f0-9]+:	62 52 69 09 5c 97 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r15\+0x99\]
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a7 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r15\+0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 15 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rip\+0x99\]        # 609f <_start\+0x609f>
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 25 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rip\+0x99\]        # 60a9 <_start\+0x60a9>
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 24 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rsp\+0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 24 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rsp\+0x99\]
[ 	]*[a-f0-9]+:	62 52 69 09 5c 94 24 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r12\+0x99\]
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a4 24 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r12\+0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0xffffffffffffff67
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0xffffffffffffff67
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 65 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 65 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 23 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*1-0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 23 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*1-0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 63 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 63 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 bc 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r12\+r15\*4-0x99\]
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 bc 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r12\+r15\*4-0x99\]
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 f8 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r8\+r15\*8-0x99\]
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 f8 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r8\+r15\*8-0x99\]
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 ad 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbp\+r13\*4-0x99\]
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 ad 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbp\+r13\*4-0x99\]
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 24 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rsp\+r12\*1-0x99\]
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 24 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rsp\+r12\*1-0x99\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0b 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rbx\+zmm1\*1\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0b 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rbx\+zmm1\*1\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 13 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rbx\+zmm10\*1\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 13 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rbx\+zmm10\*1\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 94 0b 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rbx\+zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 a4 0b 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rbx\+zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 94 13 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rbx\+zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 a4 13 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rbx\+zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 94 4b 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rbx\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 a4 4b 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rbx\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 94 53 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rbx\+zmm10\*2-0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 a4 53 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rbx\+zmm10\*2-0x9\]
[ 	]*[a-f0-9]+:	62 52 79 09 92 94 8c 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[r12\+zmm1\*4\+0x99\]
[ 	]*[a-f0-9]+:	62 c2 79 09 92 a4 8c 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[r12\+zmm1\*4\+0x99\]
[ 	]*[a-f0-9]+:	62 12 79 09 92 94 94 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[r12\+zmm10\*4\+0x9\]
[ 	]*[a-f0-9]+:	62 82 79 09 92 a4 94 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[r12\+zmm10\*4\+0x9\]
[ 	]*[a-f0-9]+:	62 12 79 01 92 94 b5 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[r13\+zmm30\*4-0x99\]
[ 	]*[a-f0-9]+:	62 82 79 01 92 a4 b5 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[r13\+zmm30\*4-0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 34 12 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0x1234
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 34 12 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0x1234
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 55 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[ebp\+0x0\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 65 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[ebp\+0x0\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 24 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[esp\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 24 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[esp\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 95 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[ebp\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a5 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[ebp\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 25 99 00 00 00 	addr32 vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0x99
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 25 99 00 00 00 	addr32 vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0x99
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 65 99 00 00 00 	addr32 vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 65 99 00 00 00 	addr32 vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 20 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 20 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 60 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 60 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 98 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eax\+ebx\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 98 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eax\+ebx\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 cc 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[esp\+ecx\*8\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 cc 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[esp\+ecx\*8\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 15 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[ebp\+edx\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 15 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[ebp\+edx\*1\+0x99\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 14 08 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm1\*1\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 24 08 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm1\*1\]
[ 	]*[a-f0-9]+:	67 62 32 79 09 92 14 10 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm10\*1\]
[ 	]*[a-f0-9]+:	67 62 a2 79 09 92 24 10 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm10\*1\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 08 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm1\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 08 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm1\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 38 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm7\*1\+0x9\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 38 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm7\*1\+0x9\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 48 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 48 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 78 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm7\*2-0x9\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 78 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm7\*2-0x9\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 8d 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[ebp\+zmm1\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 8d 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[ebp\+zmm1\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 bd 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[ebp\+zmm7\*4\+0x9\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 bd 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[ebp\+zmm7\*4\+0x9\]
[ 	]*[a-f0-9]+:	c5 f8 41 d1          	kand   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 42 d1          	kandn  k2,k1
[ 	]*[a-f0-9]+:	c5 f8 43 d1          	kandnr k2,k1
[ 	]*[a-f0-9]+:	c5 f8 48 d1          	kmerge2l1h k2,k1
[ 	]*[a-f0-9]+:	c5 f8 49 d1          	kmerge2l1l k2,k1
[ 	]*[a-f0-9]+:	c5 f8 90 d1          	kmov   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 44 d1          	knot   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 45 d1          	kor    k2,k1
[ 	]*[a-f0-9]+:	c5 f8 98 d1          	kortest k2,k1
[ 	]*[a-f0-9]+:	c5 f8 46 d1          	kxnor  k2,k1
[ 	]*[a-f0-9]+:	c5 f8 47 d1          	kxor   k2,k1
[ 	]*[a-f0-9]+:	c5 f8 92 d0          	kmov   k2,rax
[ 	]*[a-f0-9]+:	c4 c1 78 92 d1       	kmov   k2,r9
[ 	]*[a-f0-9]+:	c5 f8 93 c1          	kmov   rax,k1
[ 	]*[a-f0-9]+:	c5 78 93 c9          	kmov   r9,k1
[ 	]*[a-f0-9]+:	c5 f0 95 c2          	kconcath rax,k1,k2
[ 	]*[a-f0-9]+:	c5 70 95 ca          	kconcath r9,k1,k2
[ 	]*[a-f0-9]+:	c5 f0 97 c2          	kconcatl rax,k1,k2
[ 	]*[a-f0-9]+:	c5 70 97 ca          	kconcatl r9,k1,k2
[ 	]*[a-f0-9]+:	c4 e3 79 3e cb 02    	kextract k1,rbx,0x2
[ 	]*[a-f0-9]+:	c4 c3 79 3e cf 02    	kextract k1,r15,0x2
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 49 04 	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 09    	vpadcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 49 04 	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 09    	vpaddsetcd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 49 04 	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 09    	vpsbbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 49 04 	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 09    	vpsbbrd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 49 04 	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 09    	vpsubrsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd zmm1\{k1\},k2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 49 04 	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 09    	vpsubsetbd zmm1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 49 04 	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 09    	vabsdiffps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 49 04 	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 09    	vaddnps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 19 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 68 99 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 19 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 49 04 	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 09    	vaddps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 49 04 	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 09    	vaddsetsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 49 04 	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 09    	vfmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 49 04 	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 09    	vfmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 49 04 	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 09    	vfmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 49 04 	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 09    	vfmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 49 04 	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 09    	vfmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 49 04 	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 09    	vfmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 49 04 	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 09    	vfnmadd132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 49 04 	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 09    	vfnmadd213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 49 04 	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 09    	vfnmadd231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 49 04 	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 09    	vfnmaddc1ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 49 04 	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 09    	vfnmsub132ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 49 04 	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 09    	vfnmsub213ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 49 04 	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 09    	vfnmsub231ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 19 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 68 99 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 19 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 49 04 	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 09    	vmulps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 19 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 68 99 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 19 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 49 04 	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 09    	vsubps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 49 04 	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 09    	vsubrps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ce cb    	vclampzps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 49 04 	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 09    	vclampzps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 51 cb    	vgmaxabsps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 49 04 	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 09    	vgmaxabsps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 53 cb    	vgmaxps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 49 04 	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 09    	vgmaxps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 52 cb    	vgminps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 49 04 	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 09    	vgminps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 49 08 	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 09    	vfmadd233ps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 49 04 	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 09    	vblendmps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 49 04 	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 09    	vaddnpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 49 04 	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 09    	vaddpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 49 04 	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 09    	vfmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 49 04 	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 09    	vfmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 49 04 	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 09    	vfmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 49 04 	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 09    	vfmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 49 04 	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 09    	vfmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 49 04 	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 09    	vfmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 49 04 	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 09    	vfnmadd132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 49 04 	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 09    	vfnmadd213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 49 04 	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 09    	vfnmadd231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 49 04 	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 09    	vfnmaddc1pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 49 04 	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 09    	vfnmsub132pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 49 04 	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 09    	vfnmsub213pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 49 04 	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 09    	vfnmsub231pd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 49 04 	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 09    	vmulpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 49 04 	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 09    	vsubpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 49 04 	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 09    	vsubrpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ce cb    	vclampzpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 49 04 	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 09    	vclampzpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 55 cb    	vfixupnanpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 49 04 	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 09    	vfixupnanpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 53 cb    	vgmaxpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 49 04 	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 09    	vgmaxpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 52 cb    	vgminpd zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 49 04 	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 09    	vgminpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 49 04 	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 09    	vblendmpd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 49 04 	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 09    	vfixupps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 49 04 	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 09    	vpaddd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 49 04 	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 09    	vpabsdiffd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 49 04 	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 09    	vpaddsetsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 49 04 	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 09    	vpandd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 49 04 	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 09    	vpandnd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 49 04 	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 09    	vpblendmd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 49 04 	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 09    	vpbitinterleave11d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 49 04 	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 09    	vpbitinterleave21d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 49 04 	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 09    	vpclampzd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 49 04 	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 09    	vpinsertfieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 49 04 	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 09    	vpmadd231d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 49 04 	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 09    	vpmaxsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 49 04 	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 09    	vpmaxud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 49 04 	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 09    	vpminsd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 49 04 	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 09    	vpminud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 49 04 	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 09    	vpmulhd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 49 04 	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 09    	vpmulhud zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 49 04 	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 09    	vpmulld zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 49 04 	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 09    	vpord  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 49 04 	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 09    	vprotatefieldd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 49 04 	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 09    	vpsllvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 49 04 	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 09    	vpsravd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 49 04 	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 09    	vpsrlvd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 49 04 	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 09    	vpsubd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 49 04 	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 09    	vpsubrd zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 19 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 39 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 19 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 49 04 	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 09    	vpxord zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 55 cb    	vfixupnanps zmm1\{k1\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 49 04 	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 09    	vfixupnanps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 89 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 49 08 	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 09    	vpmadd233d zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 19 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 39 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}
[ 	]*[a-f0-9]+:	62 f2 69 99 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rd\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{ru\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rz\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps zmm1\{k1\},zmm2,zmm3,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 19 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 49 04 	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 09    	vscaleps zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 00 	vcmpeqps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 00 	vcmpeqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 02 	vcmpleps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 02 	vcmpleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 01 	vcmpltps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 01 	vcmpltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 04 	vcmpneqps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 04 	vcmpneqps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 06 	vcmpnleps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 06 	vcmpnleps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 05 	vcmpnltps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 05 	vcmpnltps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 07 	vcmpordps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 07 	vcmpordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 03 	vcmpunordps k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 03 	vcmpunordps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 00 	vcmpeqpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 00 	vcmpeqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 02 	vcmplepd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 02 	vcmplepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 01 	vcmpltpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 01 	vcmpltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 04 	vcmpneqpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 04 	vcmpneqpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 06 	vcmpnlepd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 06 	vcmpnlepd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 05 	vcmpnltpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 05 	vcmpnltpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 07 	vcmpordpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 07 	vcmpordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 03 	vcmpunordpd k1\{k2\},zmm2,zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 03 	vcmpunordpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 49 04 	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 09    	vpcmpeqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 02 	vpcmpled k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 49 04 	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 09    	vpcmpltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 04 	vpcmpneqd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 05 	vpcmpnltd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd k1\{k2\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 49 04 	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 09    	vptestmd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 64 	vcmpps k1\{k2\},zmm2,zmm3,0x64,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{unorm8\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{float16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 64 	vcmpps k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 64 	vcmppd k1\{k2\},zmm2,zmm3,0x64,\{sae\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 64 	vcmppd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 64 	vpcmpd k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3,0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{cdab\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{badc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{aaaa\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{bbbb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{cccc\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{dddd\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 3a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{dacb\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud k1\{k2\},zmm2,zmm3\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{eh\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\],0x64
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{1to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{4to16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 49 04 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 09 64 	vpcmpud k1\{k2\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\},0x64
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 49 04 	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 09    	vpandq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 49 04 	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 09    	vpandnq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 49 04 	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 09    	vpblendmq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 49 04 	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 09    	vporq  zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq zmm1\{k1\},zmm2,zmm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 49 04 	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 09    	vpxorq zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{cdab\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{badc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{cccc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{dddd\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{dacb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 09 02 	vcvtinsps2f11 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3,0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{cdab\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{badc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{cccc\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{dddd\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{dacb\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 zmm1\{k1\},zmm3\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x2
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 09 02 	vcvtinsps2u10 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x2
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 49 20 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 49 08 	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 09    	vcvtdq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 49 20 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 49 08 	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 09    	vcvtps2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{badc\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd zmm1\{k1\},ymm3\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 49 20 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 49 08 	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 09    	vcvtudq2pd zmm1\{k1\},YMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 79 19 42 cb    	vgetexpps zmm1\{k1\},zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 cb    	vgetexpps zmm1\{k1\},zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 cb    	vgetexpps zmm1\{k1\},zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 cb    	vgetexpps zmm1\{k1\},zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 cb    	vgetexpps zmm1\{k1\},zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 cb    	vgetexpps zmm1\{k1\},zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 cb    	vgetexpps zmm1\{k1\},zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 42 cb    	vgetexpps zmm1\{k1\},zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 89 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 49 04 	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 09    	vgetexpps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd zmm1\{k1\},zmm3
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 cb    	vgetexppd zmm1\{k1\},zmm3\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 cb    	vgetexppd zmm1\{k1\},zmm3\{badc\}
[ 	]*[a-f0-9]+:	62 f2 f9 49 42 cb    	vgetexppd zmm1\{k1\},zmm3\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 f9 59 42 cb    	vgetexppd zmm1\{k1\},zmm3\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 f9 69 42 cb    	vgetexppd zmm1\{k1\},zmm3\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 f9 79 42 cb    	vgetexppd zmm1\{k1\},zmm3\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 f9 39 42 cb    	vgetexppd zmm1\{k1\},zmm3\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 f9 c9 42 cb    	vgetexppd zmm1\{k1\},zmm3,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 f9 a9 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 49 04 	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 09    	vgetexppd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}
[ 	]*[a-f0-9]+:	62 f1 f9 99 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rd\}
[ 	]*[a-f0-9]+:	62 f1 f9 a9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{ru\}
[ 	]*[a-f0-9]+:	62 f1 f9 b9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rz\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rn\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rd\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{ru\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps zmm1\{k1\},zmm2,\{rz\}\{sae\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 49 04 	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 09    	vcvtpd2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 cb cb 07 	vcvtfxpntps2dq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 49 04 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 09 07 	vcvtfxpntps2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 ca cb 07 	vcvtfxpntps2udq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 49 04 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 09 07 	vcvtfxpntps2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 52 cb 07 	vrndfxpntps zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 52 49 04 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 52 09 07 	vrndfxpntps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 26 cb 07 	vgetmantps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 c9 26 cb 07 	vgetmantps zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 79 89 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 19 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 29 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 49 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 59 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 39 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 79 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 26 49 04 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 69 26 09 07 	vgetmantps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 49 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 59 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 69 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 79 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 39 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb c9 e6 cb 07 	vcvtfxpntpd2dq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 49 04 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 09 07 	vcvtfxpntpd2dq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 49 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 59 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 69 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 79 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 39 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb c9 ca cb 07 	vcvtfxpntpd2udq zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 49 04 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 09 07 	vcvtfxpntpd2udq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 49 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 59 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 69 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 79 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 39 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 c9 52 cb 07 	vrndfxpntpd zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 49 04 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 09 07 	vrndfxpntpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 49 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 59 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 69 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 79 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 39 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 c9 26 cb 07 	vgetmantpd zmm1\{k1\},zmm3,0x7,\{sae\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 49 04 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 09 07 	vgetmantpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 f3 07 	vpslld zmm1,zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 f3 07 	vpslld zmm1,zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 f3 07 	vpslld zmm1,zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 f3 07 	vpslld zmm1,zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 f3 07 	vpslld zmm1,zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 f3 07 	vpslld zmm1,zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 39 72 f3 07 	vpslld zmm1,zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 71 04 07 	vpslld zmm1,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 31 07 	vpslld zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 e3 07 	vpsrad zmm1,zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 e3 07 	vpsrad zmm1,zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 e3 07 	vpsrad zmm1,zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 e3 07 	vpsrad zmm1,zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 e3 07 	vpsrad zmm1,zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 e3 07 	vpsrad zmm1,zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 39 72 e3 07 	vpsrad zmm1,zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 61 04 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 21 07 	vpsrad zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld zmm1,zmm3,0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 d3 07 	vpsrld zmm1,zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 d3 07 	vpsrld zmm1,zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 d3 07 	vpsrld zmm1,zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 d3 07 	vpsrld zmm1,zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 d3 07 	vpsrld zmm1,zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 d3 07 	vpsrld zmm1,zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 39 72 d3 07 	vpsrld zmm1,zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld zmm1,zmm3\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 89 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 71 19 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 29 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 49 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 59 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 69 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 51 04 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f1 71 79 72 11 07 	vpsrld zmm1,ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 39 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 99 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 a9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{ru\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 b9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rz\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 49 04 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 09 07 	vcvtfxpntdq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{cdab\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{badc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{aaaa\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{bbbb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{cccc\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{dddd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 39 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3\{dacb\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 99 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rd\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 a9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{ru\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 b9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rz\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rn\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rd\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{ru\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps zmm1\{k1\},zmm3,\{rz\}\{sae\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 89 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f3 78 19 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 29 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 49 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 59 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 69 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 49 04 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 09 07 	vcvtfxpntudq2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x7
[ 	]*[a-f0-9]+:	62 f2 78 89 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 39 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 49 40 	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 09    	vloadunpackhps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 39 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 49 40 	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 09    	vloadunpacklps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 39 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 49 04 	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 09    	vmovaps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 39 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 49 10 	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 09    	vbroadcastf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 39 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 49 40 	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 09    	vbroadcastss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 89 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 69 09 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 69 39 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	62 f2 69 49 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 69 59 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 69 69 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 49 40 	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 09    	vbroadcastaddss zmm1\{k1\},zmm2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 29 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 49 40 	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 09    	vloadunpackhgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 29 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 49 40 	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 09    	vloadunpacklgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 29 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 49 04 	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 09    	vmovgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 29 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 49 10 	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 09    	vbroadcastgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 29 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 f2 79 49 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 49 40 	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 09    	vbroadcastgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 19 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 49 20 	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 09    	vloadunpackhpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 19 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 49 20 	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 09    	vloadunpacklpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 49 02 	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 09    	vmovpgps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 79 29 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 79 39 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 49 08 	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 09    	vbroadcastpgf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 89 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 19 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	62 f2 79 29 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	62 f2 79 39 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	62 f2 79 49 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 f2 79 59 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 f2 79 69 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 49 20 	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 09    	vbroadcastpgss zmm1\{k1\},ZMMWORD PTR \[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d5 09    	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 49 10 	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 89 d1 09    	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 49 10 	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 09    	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 49 02 	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 1b 09    	vbroadcastf64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 49 04 	vbroadcastf64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 09    	vbroadcastf64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 19 09    	vbroadcastsd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 49 10 	vbroadcastsd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 09    	vbroadcastsd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 89 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 49 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 49 40 	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 09    	vloadunpackhd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 78 49 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 49 40 	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 09    	vloadunpackld zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 79 49 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 49 04 	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 09    	vmovdqa32 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 49 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 49 10 	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 09    	vbroadcasti32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 49 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 49 40 	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 09    	vpbroadcastd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d4 09    	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 49 10 	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 89 d0 09    	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 49 10 	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f 09    	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 49 02 	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 5b 09    	vbroadcasti64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 49 04 	vbroadcasti64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 09    	vbroadcasti64x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 f9 89 59 09    	vpbroadcastq zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 49 10 	vpbroadcastq zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 09    	vpbroadcastq zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 71 09 03 ca 08 	valignd zmm1\{k1\},zmm1,zmm2,0x8
[ 	]*[a-f0-9]+:	62 f3 71 09 03 09 08 	valignd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\],0x8
[ 	]*[a-f0-9]+:	62 f3 71 09 03 49 02 08 	valignd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\+0x80\],0x8
[ 	]*[a-f0-9]+:	62 f3 71 09 03 09 08 	valignd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\],0x8
[ 	]*[a-f0-9]+:	62 f2 79 09 6b ca    	vcvtps2srgb8 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 6b ca    	vcvtps2srgb8 zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 09    	vcvtps2srgb8 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 6b 09    	vcvtps2srgb8 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 49 02 	vcvtps2srgb8 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 09    	vcvtps2srgb8 zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 ca    	vexp223ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 c8 ca    	vexp223ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 09    	vexp223ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 c8 09    	vexp223ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 49 02 	vexp223ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 09    	vexp223ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 ca    	vlog2ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 c9 ca    	vlog2ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 09    	vlog2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 c9 09    	vlog2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 49 02 	vlog2ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 09    	vlog2ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 ca ca    	vrcp23ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 ca ca    	vrcp23ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 09    	vrcp23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 ca 09    	vrcp23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 49 02 	vrcp23ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 09    	vrcp23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 09 cb ca    	vrsqrt23ps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 c9 cb ca    	vrsqrt23ps zmm1\{k1\},zmm2,\{sae\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 09    	vrsqrt23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 79 89 cb 09    	vrsqrt23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 49 02 	vrsqrt23ps zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 09    	vrsqrt23ps zmm1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 71 09 36 ca    	vpermd zmm1\{k1\},zmm1,zmm2
[ 	]*[a-f0-9]+:	62 f2 71 09 36 09    	vpermd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f2 71 09 36 49 02 	vpermd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	62 f2 71 09 36 09    	vpermd zmm1\{k1\},zmm1,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f3 79 09 07 ca 07 	vpermf32x4 zmm1\{k1\},zmm2,0x7
[ 	]*[a-f0-9]+:	62 f3 79 89 07 09 07 	vpermf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 07 49 02 07 	vpermf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\],0x7
[ 	]*[a-f0-9]+:	62 f3 79 09 07 09 07 	vpermf32x4 zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f1 79 09 70 ca 07 	vpshufd zmm1\{k1\},zmm2,0x7
[ 	]*[a-f0-9]+:	62 f1 79 89 70 09 07 	vpshufd zmm1\{k1\},ZMMWORD PTR \[rcx\]\{eh\},0x7
[ 	]*[a-f0-9]+:	62 f1 79 09 70 49 02 07 	vpshufd zmm1\{k1\},ZMMWORD PTR \[rcx\+0x80\],0x7
[ 	]*[a-f0-9]+:	62 f1 79 09 70 09 07 	vpshufd zmm1\{k1\},ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 f2 79 89 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d5 49 40 	vpackstorehps ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d5 09    	vpackstorehps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d1 49 40 	vpackstorelps ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d1 09    	vpackstorelps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 89 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 78 09 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 78 39 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 f1 78 49 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 78 59 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 78 69 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 78 29 49 04 	vmovaps ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 78 79 29 09    	vmovaps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d6 49 40 	vpackstorehgps ZMMWORD PTR \[rcx\+0x80\],zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d6 09    	vpackstorehgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d2 49 40 	vpackstorelgps ZMMWORD PTR \[rcx\+0x80\],zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d2 09    	vpackstorelgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 39 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 f2 79 49 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 6a 49 04 	vmovgps ZMMWORD PTR \[rcx\+0x80\],zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 6a 09    	vmovgps ZMMWORD PTR \[rcx\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d5 09    	vpackstorehpd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d5 49 10 	vpackstorehpd ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d5 09    	vpackstorehpd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 89 d1 09    	vpackstorelpd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d1 49 10 	vpackstorelpd ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d1 09    	vpackstorelpd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 89 29 09    	vmovapd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 08 29 49 02 	vmovapd ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 09 29 09    	vmovapd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 89 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 49 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d4 49 40 	vpackstorehd ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d4 09    	vpackstorehd ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 09 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 79 49 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d0 49 40 	vpackstoreld ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d0 09    	vpackstoreld ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 89 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 79 09 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 79 49 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 f1 79 59 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 f1 79 69 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 78 7f 49 04 	vmovdqa32 ZMMWORD PTR \[rcx\+0x80\],zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f1 79 79 7f 09    	vmovdqa32 ZMMWORD PTR \[rcx\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d4 09    	vpackstorehq ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d4 49 10 	vpackstorehq ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d4 09    	vpackstorehq ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 89 d0 09    	vpackstorelq ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 08 d0 49 10 	vpackstorelq ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f2 f9 09 d0 09    	vpackstorelq ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 89 7f 09    	vmovdqa64 ZMMWORD PTR \[rcx\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 08 7f 49 02 	vmovdqa64 ZMMWORD PTR \[rcx\+0x80\],zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 09 7f 09    	vmovdqa64 ZMMWORD PTR \[rcx\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 19 28 ca    	vmovapd zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 28 ca    	vmovapd zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 28 ca    	vmovapd zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 28 ca    	vmovapd zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 28 ca    	vmovapd zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 28 ca    	vmovapd zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 28 ca    	vmovapd zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 78 19 28 ca    	vmovaps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 78 29 28 ca    	vmovaps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 ca    	vmovaps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 ca    	vmovaps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 ca    	vmovaps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 ca    	vmovaps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 78 39 28 ca    	vmovaps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 79 19 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 79 29 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 79 49 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 79 39 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f1 f9 19 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 19 68 ca    	vmovgps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 79 29 68 ca    	vmovgps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 ca    	vmovgps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 ca    	vmovgps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 ca    	vmovgps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 ca    	vmovgps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 79 39 68 ca    	vmovgps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps zmm1\{k1\},zmm2
[ 	]*[a-f0-9]+:	62 f2 79 19 69 ca    	vmovpgps zmm1\{k1\},zmm2\{cdab\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 ca    	vmovpgps zmm1\{k1\},zmm2\{badc\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 ca    	vmovpgps zmm1\{k1\},zmm2\{aaaa\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 ca    	vmovpgps zmm1\{k1\},zmm2\{bbbb\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 ca    	vmovpgps zmm1\{k1\},zmm2\{cccc\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 ca    	vmovpgps zmm1\{k1\},zmm2\{dddd\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 ca    	vmovpgps zmm1\{k1\},zmm2\{dacb\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps zmm1\{k1\},zmm2\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 09 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 49 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 4c 52 40 	vpscatterdd ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 8c 55 99 00 00 00 	vpscatterdd ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 09 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 39 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{float16\}
[ 	]*[a-f0-9]+:	62 b2 79 49 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 4c 52 40 	vscatterdps ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 8c 55 99 00 00 00 	vscatterdps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 09 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 79 39 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{float16rz\}
[ 	]*[a-f0-9]+:	62 b2 79 49 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{unorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{snorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{unorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 4c 52 40 	vscatterdgps ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 8c 55 99 00 00 00 	vscatterdgps ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1\{snorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 4c 52 40 	vgatherpf0dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 8c 55 99 00 00 00 	vgatherpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 44 52 40 	vgatherpf0hintdps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 84 55 99 00 00 00 	vgatherpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 54 52 40 	vgatherpf1dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 94 55 99 00 00 00 	vgatherpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 6c 52 40 	vscatterpf0dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 ac 55 99 00 00 00 	vscatterpf0dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 64 52 40 	vscatterpf0hintdps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 a4 55 99 00 00 00 	vscatterpf0hintdps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{float16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint8\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{uint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 74 52 40 	vscatterpf1dps \[rdx\+zmm10\*2\+0x80\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 b4 55 99 00 00 00 	vscatterpf1dps \[rbp\+zmm10\*2\+0x99\]\{sint16\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 84 55 99 00 00 00 	vgatherpf0hintdpd \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 44 52 10 	vgatherpf0hintdpd \[rdx\+zmm10\*2\+0x80\]\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 84 55 99 00 00 00 	vgatherpf0hintdpd \[rbp\+zmm10\*2\+0x99\]\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdpd \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 64 52 10 	vscatterpf0hintdpd \[rdx\+zmm10\*2\+0x80\]\{k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 a4 55 99 00 00 00 	vscatterpf0hintdpd \[rbp\+zmm10\*2\+0x99\]\{k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 39 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float16\}
[ 	]*[a-f0-9]+:	62 b2 79 49 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 4c 52 20 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 f9 89 92 8c 55 99 00 00 00 	vgatherdpd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 4c 52 10 	vgatherdpd zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 8c 55 99 00 00 00 	vgatherdpd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 89 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 29 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{srgb8\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{snorm8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{snorm16\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 4c 52 20 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 89 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 19 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float11A\}
[ 	]*[a-f0-9]+:	62 b2 79 29 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float11B\}
[ 	]*[a-f0-9]+:	62 b2 79 39 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{float10C\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm10A\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm10B\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm10C\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{unorm2D\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 4c 52 20 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 89 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 79 49 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint8\}
[ 	]*[a-f0-9]+:	62 b2 79 59 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint8\}
[ 	]*[a-f0-9]+:	62 b2 79 69 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{uint16\}
[ 	]*[a-f0-9]+:	62 b2 79 79 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{sint16\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 4c 52 20 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 f9 89 90 8c 55 99 00 00 00 	vpgatherdq zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 4c 52 10 	vpgatherdq zmm1\{k1\},ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 8c 55 99 00 00 00 	vpgatherdq zmm1\{k1\},ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 b2 f9 89 a0 8c 55 99 00 00 00 	vpscatterdq ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{eh\}\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 4c 52 10 	vpscatterdq ZMMWORD PTR \[rdx\+zmm10\*2\+0x80\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 8c 55 99 00 00 00 	vpscatterdq ZMMWORD PTR \[rbp\+zmm10\*2\+0x99\]\{k1\},zmm1
[ 	]*[a-f0-9]+:	c4 e2 7b f0 d8       	bitinterleave11 ebx,eax
[ 	]*[a-f0-9]+:	c4 42 7b f0 d1       	bitinterleave11 r10d,r9d
[ 	]*[a-f0-9]+:	c4 e2 7a f0 d8       	bitinterleave21 ebx,eax
[ 	]*[a-f0-9]+:	c4 42 7a f0 d1       	bitinterleave21 r10d,r9d
[ 	]*[a-f0-9]+:	c5 fa bd d8          	lzcnt  ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7a bd d1       	lzcnt  r10d,r9d
[ 	]*[a-f0-9]+:	c4 e2 7b f1 d8       	quadmask ebx,eax
[ 	]*[a-f0-9]+:	c4 42 7b f1 d1       	quadmask r10d,r9d
[ 	]*[a-f0-9]+:	c5 fa b8 d8          	popcnt ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7a b8 d1       	popcnt r10d,r9d
[ 	]*[a-f0-9]+:	c5 fa bc d8          	tzcnt  ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7a bc d1       	tzcnt  r10d,r9d
[ 	]*[a-f0-9]+:	c5 fb bc d8          	tzcnti ebx,eax
[ 	]*[a-f0-9]+:	c4 41 7b bc d1       	tzcnti r10d,r9d
[ 	]*[a-f0-9]+:	c4 e2 fb f0 d8       	bitinterleave11 rbx,rax
[ 	]*[a-f0-9]+:	c4 42 fb f0 f9       	bitinterleave11 r15,r9
[ 	]*[a-f0-9]+:	c4 e2 fa f0 d8       	bitinterleave21 rbx,rax
[ 	]*[a-f0-9]+:	c4 42 fa f0 f9       	bitinterleave21 r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fa bd d8       	lzcnt  rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fa bd f9       	lzcnt  r15,r9
[ 	]*[a-f0-9]+:	c4 e2 fb f1 d8       	quadmask rbx,rax
[ 	]*[a-f0-9]+:	c4 42 fb f1 f9       	quadmask r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fa b8 d8       	popcnt rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fa b8 f9       	popcnt r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fa bc d8       	tzcnt  rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fa bc f9       	tzcnt  r15,r9
[ 	]*[a-f0-9]+:	c4 e1 fb bc d8       	tzcnti rbx,rax
[ 	]*[a-f0-9]+:	c4 41 fb bc f9       	tzcnti r15,r9
[ 	]*[a-f0-9]+:	c5 fb ae 39          	clevict0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7b 08 ae 79 02 	clevict0 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 fb ae 39          	clevict0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 fa ae 39          	clevict1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 7a 08 ae 79 02 	clevict1 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 fa ae 39          	clevict1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 09          	vprefetch0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 49 02 	vprefetch0 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 09          	vprefetch0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 11          	vprefetch1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 51 02 	vprefetch1 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 11          	vprefetch1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 19          	vprefetch2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 59 02 	vprefetch2 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 19          	vprefetch2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 29          	vprefetche0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 69 02 	vprefetche0 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 29          	vprefetche0 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 31          	vprefetche1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 71 02 	vprefetche1 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 31          	vprefetche1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 39          	vprefetche2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 79 02 	vprefetche2 \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 39          	vprefetche2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 21          	vprefetchenta BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 61 02 	vprefetchenta \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 21          	vprefetchenta BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 f8 18 01          	vprefetchnta BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 f1 78 08 18 41 02 	vprefetchnta \[rcx\+0x80\]
[ 	]*[a-f0-9]+:	c5 f8 18 01          	vprefetchnta BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	c5 fa ae f0          	delay  eax
[ 	]*[a-f0-9]+:	c4 c1 7a ae f1       	delay  r9d
[ 	]*[a-f0-9]+:	c5 fb ae f0          	spflt  eax
[ 	]*[a-f0-9]+:	c4 c1 7b ae f1       	spflt  r9d
[ 	]*[a-f0-9]+:	c4 e1 fa ae f0       	delay  rax
[ 	]*[a-f0-9]+:	c4 c1 fa ae f1       	delay  r9
[ 	]*[a-f0-9]+:	c4 e1 fb ae f0       	spflt  rax
[ 	]*[a-f0-9]+:	c4 c1 fb ae f1       	spflt  r9
[ 	]*[a-f0-9]+:	62 81 59 11 db e6    	vpandd zmm20\{k1\},zmm20,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 01 59 11 db f6    	vpandd zmm30\{k1\},zmm20,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 01 09 11 db f6    	vpandd zmm30\{k1\},zmm30,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 81 09 11 db e6    	vpandd zmm20\{k1\},zmm30,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 81 29 19 db e6    	vpandd zmm20\{k1\},zmm10,zmm30\{cdab\}
[ 	]*[a-f0-9]+:	62 f1 59 01 72 31 07 	vpslld zmm20,ZMMWORD PTR \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 91 59 01 72 f6 07 	vpslld zmm20,zmm30,0x7
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 78 56 34 12 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0x12345678
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 78 56 34 12 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0x12345678
[ 	]*[a-f0-9]+:	62 72 69 09 5c 55 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbp\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 65 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbp\+0x0\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 95 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbp\+0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a5 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbp\+0x99\]
[ 	]*[a-f0-9]+:	62 52 69 09 5c 97 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r15\+0x99\]
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a7 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r15\+0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 15 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rip\+0x99\]        # ca50 <_start\+0xca50>
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 25 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rip\+0x99\]        # ca5a <_start\+0xca5a>
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 24 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rsp\+0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 24 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rsp\+0x99\]
[ 	]*[a-f0-9]+:	62 52 69 09 5c 94 24 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r12\+0x99\]
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a4 24 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r12\+0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0xffffffffffffff67
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0xffffffffffffff67
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 65 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 65 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 23 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*1-0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 23 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*1-0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 63 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 63 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*2-0x99\]
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 bc 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r12\+r15\*4-0x99\]
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 bc 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r12\+r15\*4-0x99\]
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 f8 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[r8\+r15\*8-0x99\]
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 f8 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[r8\+r15\*8-0x99\]
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 a5 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rbp\+r12\*4-0x99\]
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 a5 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rbp\+r12\*4-0x99\]
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 2c 67 ff ff ff 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[rsp\+r13\*1-0x99\]
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 2c 67 ff ff ff 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[rsp\+r13\*1-0x99\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2\+0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 08 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rax\+zmm1\*1\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 08 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rax\+zmm1\*1\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 10 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rax\+zmm10\*1\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 10 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rax\+zmm10\*1\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 94 48 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rax\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 a4 48 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rax\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 94 50 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[rax\+zmm10\*2-0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 a4 50 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[rax\+zmm10\*2-0x9\]
[ 	]*[a-f0-9]+:	62 52 79 09 92 94 8c 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[r12\+zmm1\*4-0x99\]
[ 	]*[a-f0-9]+:	62 c2 79 09 92 a4 8c 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[r12\+zmm1\*4-0x99\]
[ 	]*[a-f0-9]+:	62 12 79 09 92 94 94 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[r12\+zmm10\*4\+0x9\]
[ 	]*[a-f0-9]+:	62 82 79 09 92 a4 94 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[r12\+zmm10\*4\+0x9\]
[ 	]*[a-f0-9]+:	62 12 79 01 92 94 b5 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[r13\+zmm30\*4\+0x99\]
[ 	]*[a-f0-9]+:	62 82 79 01 92 a4 b5 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[r13\+zmm30\*4\+0x99\]
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 34 12 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0x1234
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 34 12 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0x1234
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 55 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[ebp\+0x0\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 65 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[ebp\+0x0\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 95 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[ebp\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a5 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[ebp\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 25 99 00 00 00 	addr32 vpadcd zmm10\{k1\},k2,ZMMWORD PTR ds:0x99
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 25 99 00 00 00 	addr32 vpadcd zmm20\{k1\},k2,ZMMWORD PTR ds:0x99
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 65 99 00 00 00 	addr32 vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 65 99 00 00 00 	addr32 vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 20 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 20 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 60 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 60 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 98 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[eax\+ebx\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 98 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[eax\+ebx\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 cc 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[esp\+ecx\*8\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 cc 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[esp\+ecx\*8\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 15 99 00 00 00 	vpadcd zmm10\{k1\},k2,ZMMWORD PTR \[ebp\+edx\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 15 99 00 00 00 	vpadcd zmm20\{k1\},k2,ZMMWORD PTR \[ebp\+edx\*1\+0x99\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x0\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[zmm10\*2\+0x9\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 14 08 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm1\*1\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 24 08 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm1\*1\]
[ 	]*[a-f0-9]+:	67 62 32 79 09 92 14 10 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm10\*1\]
[ 	]*[a-f0-9]+:	67 62 a2 79 09 92 24 10 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm10\*1\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 08 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 08 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm1\*1-0x99\]
[ 	]*[a-f0-9]+:	67 62 32 79 09 92 94 10 f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	67 62 a2 79 09 92 a4 10 f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm10\*1-0x9\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 48 67 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 48 67 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm1\*2-0x99\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 78 09 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[eax\+zmm7\*2\+0x9\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 78 09 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[eax\+zmm7\*2\+0x9\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 8d 99 00 00 00 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[ebp\+zmm1\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 8d 99 00 00 00 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[ebp\+zmm1\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 bd f7 ff ff ff 	vgatherdps zmm10\{k1\},ZMMWORD PTR \[ebp\+zmm7\*4-0x9\]
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 bd f7 ff ff ff 	vgatherdps zmm20\{k1\},ZMMWORD PTR \[ebp\+zmm7\*4-0x9\]
#pass

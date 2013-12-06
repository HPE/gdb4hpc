#as: -march=k1om
#objdump: -dw
#name: LRB2 insns

.*: +file format .*


Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	90                   	nop
[ 	]*[a-f0-9]+:	c5 f8 41 d1          	kand   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 42 d1          	kandn  %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 43 d1          	kandnr %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 48 d1          	kmerge2l1h %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 49 d1          	kmerge2l1l %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 90 d1          	kmov   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 44 d1          	knot   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 45 d1          	kor    %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 98 d1          	kortest %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 46 d1          	kxnor  %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 47 d1          	kxor   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 92 d0          	kmov   %rax,%k2
[ 	]*[a-f0-9]+:	c4 c1 78 92 d1       	kmov   %r9,%k2
[ 	]*[a-f0-9]+:	c5 f8 93 c1          	kmov   %k1,%rax
[ 	]*[a-f0-9]+:	c5 78 93 c9          	kmov   %k1,%r9
[ 	]*[a-f0-9]+:	c5 f0 95 c2          	kconcath %k2,%k1,%rax
[ 	]*[a-f0-9]+:	c5 70 95 ca          	kconcath %k2,%k1,%r9
[ 	]*[a-f0-9]+:	c5 f0 97 c2          	kconcatl %k2,%k1,%rax
[ 	]*[a-f0-9]+:	c5 70 97 ca          	kconcatl %k2,%k1,%r9
[ 	]*[a-f0-9]+:	c4 e3 79 3e cb 03    	kextract \$0x3,%rbx,%k1
[ 	]*[a-f0-9]+:	c4 c3 79 3e cf 03    	kextract \$0x3,%r15,%k1
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5c cb    	vpadcd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c cb    	vpadcd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c cb    	vpadcd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c cb    	vpadcd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c cb    	vpadcd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c cb    	vpadcd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5c cb    	vpadcd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c 09    	vpadcd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5c 09    	vpadcd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c 09    	vpadcd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c 09    	vpadcd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c 09    	vpadcd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c 09    	vpadcd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 09    	vpadcd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 49 04 	vpadcd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5d cb    	vpaddsetcd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d cb    	vpaddsetcd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d cb    	vpaddsetcd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d cb    	vpaddsetcd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d cb    	vpaddsetcd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d cb    	vpaddsetcd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5d cb    	vpaddsetcd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d 09    	vpaddsetcd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5d 09    	vpaddsetcd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d 09    	vpaddsetcd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d 09    	vpaddsetcd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d 09    	vpaddsetcd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d 09    	vpaddsetcd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 09    	vpaddsetcd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 49 04 	vpaddsetcd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5e cb    	vpsbbd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e cb    	vpsbbd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e cb    	vpsbbd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e cb    	vpsbbd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e cb    	vpsbbd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e cb    	vpsbbd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5e cb    	vpsbbd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e 09    	vpsbbd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5e 09    	vpsbbd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e 09    	vpsbbd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e 09    	vpsbbd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e 09    	vpsbbd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e 09    	vpsbbd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 09    	vpsbbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 49 04 	vpsbbd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6e cb    	vpsbbrd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e cb    	vpsbbrd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e cb    	vpsbbrd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e cb    	vpsbbrd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e cb    	vpsbbrd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e cb    	vpsbbrd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6e cb    	vpsbbrd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e 09    	vpsbbrd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6e 09    	vpsbbrd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e 09    	vpsbbrd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e 09    	vpsbbrd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e 09    	vpsbbrd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e 09    	vpsbbrd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 09    	vpsbbrd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 49 04 	vpsbbrd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6f cb    	vpsubrsetbd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f cb    	vpsubrsetbd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f cb    	vpsubrsetbd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f cb    	vpsubrsetbd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f cb    	vpsubrsetbd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f cb    	vpsubrsetbd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6f cb    	vpsubrsetbd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f 09    	vpsubrsetbd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6f 09    	vpsubrsetbd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f 09    	vpsubrsetbd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f 09    	vpsubrsetbd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f 09    	vpsubrsetbd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f 09    	vpsubrsetbd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 09    	vpsubrsetbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 49 04 	vpsubrsetbd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5f cb    	vpsubsetbd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f cb    	vpsubsetbd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f cb    	vpsubsetbd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f cb    	vpsubsetbd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f cb    	vpsubsetbd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f cb    	vpsubsetbd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5f cb    	vpsubsetbd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f 09    	vpsubsetbd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5f 09    	vpsubsetbd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f 09    	vpsubsetbd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f 09    	vpsubsetbd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f 09    	vpsubsetbd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f 09    	vpsubsetbd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 09    	vpsubsetbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 49 04 	vpsubsetbd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 57 cb    	vabsdiffps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 cb    	vabsdiffps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 cb    	vabsdiffps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 cb    	vabsdiffps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 cb    	vabsdiffps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 cb    	vabsdiffps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 cb    	vabsdiffps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 cb    	vabsdiffps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 57 cb    	vabsdiffps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 57 cb    	vabsdiffps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 57 cb    	vabsdiffps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 09    	vabsdiffps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 57 09    	vabsdiffps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 09    	vabsdiffps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 09    	vabsdiffps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 09    	vabsdiffps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 09    	vabsdiffps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 09    	vabsdiffps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 09    	vabsdiffps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 49 04 	vabsdiffps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 50 cb    	vaddnps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 cb    	vaddnps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 cb    	vaddnps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 cb    	vaddnps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 cb    	vaddnps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 cb    	vaddnps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 cb    	vaddnps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 cb    	vaddnps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 50 cb    	vaddnps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 50 cb    	vaddnps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 50 cb    	vaddnps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 09    	vaddnps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 50 09    	vaddnps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 09    	vaddnps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 09    	vaddnps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 09    	vaddnps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 09    	vaddnps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 09    	vaddnps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 09    	vaddnps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 49 04 	vaddnps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 58 cb    	vaddps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 cb    	vaddps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 cb    	vaddps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 cb    	vaddps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 cb    	vaddps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 cb    	vaddps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 cb    	vaddps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 cb    	vaddps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 99 58 cb    	vaddps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 58 cb    	vaddps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 58 cb    	vaddps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 09    	vaddps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 58 09    	vaddps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 09    	vaddps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 09    	vaddps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 09    	vaddps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 09    	vaddps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 09    	vaddps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 09    	vaddps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 49 04 	vaddps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cc cb    	vaddsetsps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc cb    	vaddsetsps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc cb    	vaddsetsps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc cb    	vaddsetsps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc cb    	vaddsetsps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc cb    	vaddsetsps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc cb    	vaddsetsps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc cb    	vaddsetsps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 cc cb    	vaddsetsps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 cc cb    	vaddsetsps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 cc cb    	vaddsetsps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc 09    	vaddsetsps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cc 09    	vaddsetsps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc 09    	vaddsetsps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc 09    	vaddsetsps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc 09    	vaddsetsps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc 09    	vaddsetsps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc 09    	vaddsetsps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 09    	vaddsetsps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 49 04 	vaddsetsps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 98 cb    	vfmadd132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 cb    	vfmadd132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 cb    	vfmadd132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 cb    	vfmadd132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 cb    	vfmadd132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 cb    	vfmadd132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 cb    	vfmadd132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 cb    	vfmadd132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 98 cb    	vfmadd132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 98 cb    	vfmadd132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 98 cb    	vfmadd132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 09    	vfmadd132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 98 09    	vfmadd132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 09    	vfmadd132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 09    	vfmadd132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 09    	vfmadd132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 09    	vfmadd132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 09    	vfmadd132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 09    	vfmadd132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 49 04 	vfmadd132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 cb    	vfmadd213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 cb    	vfmadd213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 cb    	vfmadd213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 cb    	vfmadd213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 cb    	vfmadd213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 cb    	vfmadd213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 cb    	vfmadd213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 cb    	vfmadd213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a8 cb    	vfmadd213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a8 cb    	vfmadd213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a8 cb    	vfmadd213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 09    	vfmadd213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 09    	vfmadd213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 09    	vfmadd213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 09    	vfmadd213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 09    	vfmadd213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 09    	vfmadd213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 09    	vfmadd213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 09    	vfmadd213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 49 04 	vfmadd213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 cb    	vfmadd231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 cb    	vfmadd231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 cb    	vfmadd231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 cb    	vfmadd231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 cb    	vfmadd231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 cb    	vfmadd231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 cb    	vfmadd231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 cb    	vfmadd231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 b8 cb    	vfmadd231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 b8 cb    	vfmadd231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 b8 cb    	vfmadd231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 09    	vfmadd231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 09    	vfmadd231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 09    	vfmadd231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 09    	vfmadd231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 09    	vfmadd231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 09    	vfmadd231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 09    	vfmadd231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 09    	vfmadd231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 49 04 	vfmadd231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9a cb    	vfmsub132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a cb    	vfmsub132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a cb    	vfmsub132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a cb    	vfmsub132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a cb    	vfmsub132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a cb    	vfmsub132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a cb    	vfmsub132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a cb    	vfmsub132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9a cb    	vfmsub132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9a cb    	vfmsub132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9a cb    	vfmsub132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a 09    	vfmsub132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9a 09    	vfmsub132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a 09    	vfmsub132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a 09    	vfmsub132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a 09    	vfmsub132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a 09    	vfmsub132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a 09    	vfmsub132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 09    	vfmsub132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 49 04 	vfmsub132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 aa cb    	vfmsub213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa cb    	vfmsub213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa cb    	vfmsub213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa cb    	vfmsub213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa cb    	vfmsub213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa cb    	vfmsub213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa cb    	vfmsub213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa cb    	vfmsub213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 aa cb    	vfmsub213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 aa cb    	vfmsub213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 aa cb    	vfmsub213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa 09    	vfmsub213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 aa 09    	vfmsub213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa 09    	vfmsub213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa 09    	vfmsub213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa 09    	vfmsub213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa 09    	vfmsub213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa 09    	vfmsub213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 09    	vfmsub213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 49 04 	vfmsub213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ba cb    	vfmsub231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba cb    	vfmsub231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba cb    	vfmsub231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba cb    	vfmsub231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba cb    	vfmsub231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba cb    	vfmsub231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba cb    	vfmsub231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba cb    	vfmsub231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ba cb    	vfmsub231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ba cb    	vfmsub231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ba cb    	vfmsub231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba 09    	vfmsub231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ba 09    	vfmsub231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba 09    	vfmsub231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba 09    	vfmsub231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba 09    	vfmsub231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba 09    	vfmsub231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba 09    	vfmsub231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 09    	vfmsub231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 49 04 	vfmsub231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9c cb    	vfnmadd132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c cb    	vfnmadd132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c cb    	vfnmadd132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c cb    	vfnmadd132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c cb    	vfnmadd132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c cb    	vfnmadd132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c cb    	vfnmadd132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c cb    	vfnmadd132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9c cb    	vfnmadd132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9c cb    	vfnmadd132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9c cb    	vfnmadd132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c 09    	vfnmadd132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9c 09    	vfnmadd132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c 09    	vfnmadd132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c 09    	vfnmadd132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c 09    	vfnmadd132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c 09    	vfnmadd132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c 09    	vfnmadd132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 09    	vfnmadd132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 49 04 	vfnmadd132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ac cb    	vfnmadd213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac cb    	vfnmadd213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac cb    	vfnmadd213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac cb    	vfnmadd213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac cb    	vfnmadd213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac cb    	vfnmadd213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac cb    	vfnmadd213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac cb    	vfnmadd213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ac cb    	vfnmadd213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ac cb    	vfnmadd213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ac cb    	vfnmadd213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac 09    	vfnmadd213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ac 09    	vfnmadd213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac 09    	vfnmadd213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac 09    	vfnmadd213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac 09    	vfnmadd213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac 09    	vfnmadd213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac 09    	vfnmadd213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 09    	vfnmadd213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 49 04 	vfnmadd213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 bc cb    	vfnmadd231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc cb    	vfnmadd231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc cb    	vfnmadd231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc cb    	vfnmadd231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc cb    	vfnmadd231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc cb    	vfnmadd231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc cb    	vfnmadd231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc cb    	vfnmadd231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 bc cb    	vfnmadd231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 bc cb    	vfnmadd231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 bc cb    	vfnmadd231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc 09    	vfnmadd231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 bc 09    	vfnmadd231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc 09    	vfnmadd231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc 09    	vfnmadd231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc 09    	vfnmadd231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc 09    	vfnmadd231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc 09    	vfnmadd231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 09    	vfnmadd231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 49 04 	vfnmadd231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 94 cb    	vfnmaddc1ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 cb    	vfnmaddc1ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 cb    	vfnmaddc1ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 cb    	vfnmaddc1ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 cb    	vfnmaddc1ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 cb    	vfnmaddc1ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 cb    	vfnmaddc1ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 cb    	vfnmaddc1ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 94 cb    	vfnmaddc1ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 94 cb    	vfnmaddc1ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 94 cb    	vfnmaddc1ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 09    	vfnmaddc1ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 94 09    	vfnmaddc1ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 09    	vfnmaddc1ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 09    	vfnmaddc1ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 09    	vfnmaddc1ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 09    	vfnmaddc1ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 09    	vfnmaddc1ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 09    	vfnmaddc1ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 49 04 	vfnmaddc1ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9e cb    	vfnmsub132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e cb    	vfnmsub132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e cb    	vfnmsub132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e cb    	vfnmsub132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e cb    	vfnmsub132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e cb    	vfnmsub132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e cb    	vfnmsub132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e cb    	vfnmsub132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9e cb    	vfnmsub132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9e cb    	vfnmsub132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9e cb    	vfnmsub132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e 09    	vfnmsub132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9e 09    	vfnmsub132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e 09    	vfnmsub132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e 09    	vfnmsub132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e 09    	vfnmsub132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e 09    	vfnmsub132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e 09    	vfnmsub132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 09    	vfnmsub132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 49 04 	vfnmsub132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ae cb    	vfnmsub213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae cb    	vfnmsub213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae cb    	vfnmsub213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae cb    	vfnmsub213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae cb    	vfnmsub213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae cb    	vfnmsub213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae cb    	vfnmsub213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae cb    	vfnmsub213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ae cb    	vfnmsub213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ae cb    	vfnmsub213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ae cb    	vfnmsub213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae 09    	vfnmsub213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ae 09    	vfnmsub213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae 09    	vfnmsub213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae 09    	vfnmsub213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae 09    	vfnmsub213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae 09    	vfnmsub213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae 09    	vfnmsub213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 09    	vfnmsub213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 49 04 	vfnmsub213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 be cb    	vfnmsub231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be cb    	vfnmsub231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be cb    	vfnmsub231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be cb    	vfnmsub231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be cb    	vfnmsub231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be cb    	vfnmsub231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be cb    	vfnmsub231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be cb    	vfnmsub231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 be cb    	vfnmsub231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 be cb    	vfnmsub231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 be cb    	vfnmsub231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be 09    	vfnmsub231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 be 09    	vfnmsub231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be 09    	vfnmsub231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be 09    	vfnmsub231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be 09    	vfnmsub231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be 09    	vfnmsub231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be 09    	vfnmsub231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 09    	vfnmsub231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 49 04 	vfnmsub231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 59 cb    	vmulps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 cb    	vmulps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 cb    	vmulps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 cb    	vmulps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 cb    	vmulps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 cb    	vmulps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 cb    	vmulps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 cb    	vmulps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 99 59 cb    	vmulps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 59 cb    	vmulps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 59 cb    	vmulps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 09    	vmulps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 59 09    	vmulps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 09    	vmulps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 09    	vmulps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 09    	vmulps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 09    	vmulps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 09    	vmulps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 09    	vmulps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 49 04 	vmulps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 5c cb    	vsubps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c cb    	vsubps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c cb    	vsubps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c cb    	vsubps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c cb    	vsubps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c cb    	vsubps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c cb    	vsubps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c cb    	vsubps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 99 5c cb    	vsubps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 5c cb    	vsubps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 5c cb    	vsubps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c 09    	vsubps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 5c 09    	vsubps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c 09    	vsubps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c 09    	vsubps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c 09    	vsubps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c 09    	vsubps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c 09    	vsubps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 09    	vsubps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 49 04 	vsubps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6d cb    	vsubrps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d cb    	vsubrps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d cb    	vsubrps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d cb    	vsubrps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d cb    	vsubrps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d cb    	vsubrps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d cb    	vsubrps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d cb    	vsubrps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 6d cb    	vsubrps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 6d cb    	vsubrps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 6d cb    	vsubrps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d 09    	vsubrps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6d 09    	vsubrps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d 09    	vsubrps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d 09    	vsubrps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d 09    	vsubrps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d 09    	vsubrps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d 09    	vsubrps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 09    	vsubrps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 49 04 	vsubrps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ce cb    	vclampzps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce cb    	vclampzps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce cb    	vclampzps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce cb    	vclampzps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce cb    	vclampzps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce cb    	vclampzps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce cb    	vclampzps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ce cb    	vclampzps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ce 09    	vclampzps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ce 09    	vclampzps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce 09    	vclampzps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce 09    	vclampzps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce 09    	vclampzps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce 09    	vclampzps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce 09    	vclampzps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 09    	vclampzps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 49 04 	vclampzps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 51 cb    	vgmaxabsps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 cb    	vgmaxabsps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 cb    	vgmaxabsps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 cb    	vgmaxabsps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 cb    	vgmaxabsps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 cb    	vgmaxabsps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 cb    	vgmaxabsps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 51 cb    	vgmaxabsps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 51 09    	vgmaxabsps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 51 09    	vgmaxabsps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 09    	vgmaxabsps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 09    	vgmaxabsps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 09    	vgmaxabsps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 09    	vgmaxabsps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 09    	vgmaxabsps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 09    	vgmaxabsps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 49 04 	vgmaxabsps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 53 cb    	vgmaxps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 cb    	vgmaxps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 cb    	vgmaxps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 cb    	vgmaxps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 cb    	vgmaxps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 cb    	vgmaxps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 cb    	vgmaxps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 53 cb    	vgmaxps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 53 09    	vgmaxps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 53 09    	vgmaxps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 09    	vgmaxps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 09    	vgmaxps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 09    	vgmaxps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 09    	vgmaxps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 09    	vgmaxps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 09    	vgmaxps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 49 04 	vgmaxps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 52 cb    	vgminps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 cb    	vgminps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 cb    	vgminps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 cb    	vgminps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 cb    	vgminps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 cb    	vgminps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 cb    	vgminps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 52 cb    	vgminps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 52 09    	vgminps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 52 09    	vgminps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 09    	vgminps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 09    	vgminps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 09    	vgminps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 09    	vgminps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 09    	vgminps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 09    	vgminps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 49 04 	vgminps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 cb    	vfmadd233ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a4 cb    	vfmadd233ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a4 cb    	vfmadd233ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a4 cb    	vfmadd233ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 09    	vfmadd233ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 09    	vfmadd233ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 49 08 	vfmadd233ps 0x80\(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 65 cb    	vblendmps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 cb    	vblendmps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 cb    	vblendmps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 cb    	vblendmps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 cb    	vblendmps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 cb    	vblendmps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 cb    	vblendmps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 09    	vblendmps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 65 09    	vblendmps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 09    	vblendmps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 09    	vblendmps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 09    	vblendmps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 09    	vblendmps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 09    	vblendmps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 09    	vblendmps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 49 04 	vblendmps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 cb    	vaddnpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 cb    	vaddnpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 50 cb    	vaddnpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 50 cb    	vaddnpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 50 cb    	vaddnpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 50 cb    	vaddnpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 50 cb    	vaddnpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 cb    	vaddnpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 50 cb    	vaddnpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 50 cb    	vaddnpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 50 cb    	vaddnpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 09    	vaddnpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 09    	vaddnpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 09    	vaddnpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 49 04 	vaddnpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 cb    	vaddpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 cb    	vaddpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 58 cb    	vaddpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 58 cb    	vaddpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 58 cb    	vaddpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 58 cb    	vaddpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 58 cb    	vaddpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 cb    	vaddpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 58 cb    	vaddpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 58 cb    	vaddpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 58 cb    	vaddpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 09    	vaddpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 09    	vaddpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 09    	vaddpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 49 04 	vaddpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 cb    	vfmadd132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 cb    	vfmadd132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 98 cb    	vfmadd132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 98 cb    	vfmadd132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 98 cb    	vfmadd132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 98 cb    	vfmadd132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 98 cb    	vfmadd132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 cb    	vfmadd132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 98 cb    	vfmadd132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 98 cb    	vfmadd132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 98 cb    	vfmadd132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 09    	vfmadd132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 09    	vfmadd132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 09    	vfmadd132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 49 04 	vfmadd132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 cb    	vfmadd213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 cb    	vfmadd213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 a8 cb    	vfmadd213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 a8 cb    	vfmadd213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 a8 cb    	vfmadd213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 a8 cb    	vfmadd213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 a8 cb    	vfmadd213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 cb    	vfmadd213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 a8 cb    	vfmadd213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 a8 cb    	vfmadd213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 a8 cb    	vfmadd213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 09    	vfmadd213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 09    	vfmadd213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 09    	vfmadd213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 49 04 	vfmadd213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 cb    	vfmadd231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 cb    	vfmadd231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 b8 cb    	vfmadd231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 b8 cb    	vfmadd231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 b8 cb    	vfmadd231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 b8 cb    	vfmadd231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 b8 cb    	vfmadd231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 cb    	vfmadd231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 b8 cb    	vfmadd231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 b8 cb    	vfmadd231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 b8 cb    	vfmadd231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 09    	vfmadd231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 09    	vfmadd231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 09    	vfmadd231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 49 04 	vfmadd231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a cb    	vfmsub132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a cb    	vfmsub132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9a cb    	vfmsub132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9a cb    	vfmsub132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9a cb    	vfmsub132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9a cb    	vfmsub132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9a cb    	vfmsub132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a cb    	vfmsub132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9a cb    	vfmsub132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9a cb    	vfmsub132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9a cb    	vfmsub132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a 09    	vfmsub132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a 09    	vfmsub132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 09    	vfmsub132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 49 04 	vfmsub132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa cb    	vfmsub213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa cb    	vfmsub213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 aa cb    	vfmsub213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 aa cb    	vfmsub213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 aa cb    	vfmsub213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 aa cb    	vfmsub213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 aa cb    	vfmsub213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa cb    	vfmsub213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 aa cb    	vfmsub213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 aa cb    	vfmsub213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 aa cb    	vfmsub213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa 09    	vfmsub213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa 09    	vfmsub213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 09    	vfmsub213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 49 04 	vfmsub213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba cb    	vfmsub231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba cb    	vfmsub231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ba cb    	vfmsub231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ba cb    	vfmsub231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ba cb    	vfmsub231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ba cb    	vfmsub231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ba cb    	vfmsub231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba cb    	vfmsub231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ba cb    	vfmsub231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ba cb    	vfmsub231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ba cb    	vfmsub231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba 09    	vfmsub231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba 09    	vfmsub231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 09    	vfmsub231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 49 04 	vfmsub231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c cb    	vfnmadd132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c cb    	vfnmadd132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9c cb    	vfnmadd132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9c cb    	vfnmadd132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9c cb    	vfnmadd132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9c cb    	vfnmadd132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9c cb    	vfnmadd132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c cb    	vfnmadd132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9c cb    	vfnmadd132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9c cb    	vfnmadd132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9c cb    	vfnmadd132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c 09    	vfnmadd132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c 09    	vfnmadd132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 09    	vfnmadd132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 49 04 	vfnmadd132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac cb    	vfnmadd213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac cb    	vfnmadd213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ac cb    	vfnmadd213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ac cb    	vfnmadd213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ac cb    	vfnmadd213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ac cb    	vfnmadd213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ac cb    	vfnmadd213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac cb    	vfnmadd213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ac cb    	vfnmadd213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ac cb    	vfnmadd213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ac cb    	vfnmadd213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac 09    	vfnmadd213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac 09    	vfnmadd213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 09    	vfnmadd213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 49 04 	vfnmadd213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc cb    	vfnmadd231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc cb    	vfnmadd231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 bc cb    	vfnmadd231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 bc cb    	vfnmadd231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 bc cb    	vfnmadd231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 bc cb    	vfnmadd231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 bc cb    	vfnmadd231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc cb    	vfnmadd231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 bc cb    	vfnmadd231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 bc cb    	vfnmadd231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 bc cb    	vfnmadd231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc 09    	vfnmadd231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc 09    	vfnmadd231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 09    	vfnmadd231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 49 04 	vfnmadd231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 cb    	vfnmaddc1pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 cb    	vfnmaddc1pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 94 cb    	vfnmaddc1pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 94 cb    	vfnmaddc1pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 94 cb    	vfnmaddc1pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 94 cb    	vfnmaddc1pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 94 cb    	vfnmaddc1pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 cb    	vfnmaddc1pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 94 cb    	vfnmaddc1pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 94 cb    	vfnmaddc1pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 94 cb    	vfnmaddc1pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 09    	vfnmaddc1pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 09    	vfnmaddc1pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 09    	vfnmaddc1pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 49 04 	vfnmaddc1pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e cb    	vfnmsub132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e cb    	vfnmsub132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9e cb    	vfnmsub132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9e cb    	vfnmsub132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9e cb    	vfnmsub132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9e cb    	vfnmsub132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9e cb    	vfnmsub132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e cb    	vfnmsub132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9e cb    	vfnmsub132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9e cb    	vfnmsub132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9e cb    	vfnmsub132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e 09    	vfnmsub132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e 09    	vfnmsub132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 09    	vfnmsub132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 49 04 	vfnmsub132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae cb    	vfnmsub213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae cb    	vfnmsub213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ae cb    	vfnmsub213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ae cb    	vfnmsub213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ae cb    	vfnmsub213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ae cb    	vfnmsub213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ae cb    	vfnmsub213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae cb    	vfnmsub213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ae cb    	vfnmsub213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ae cb    	vfnmsub213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ae cb    	vfnmsub213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae 09    	vfnmsub213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae 09    	vfnmsub213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 09    	vfnmsub213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 49 04 	vfnmsub213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 be cb    	vfnmsub231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be cb    	vfnmsub231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 be cb    	vfnmsub231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 be cb    	vfnmsub231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 be cb    	vfnmsub231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 be cb    	vfnmsub231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 be cb    	vfnmsub231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be cb    	vfnmsub231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 be cb    	vfnmsub231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 be cb    	vfnmsub231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 be cb    	vfnmsub231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be 09    	vfnmsub231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 be 09    	vfnmsub231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 09    	vfnmsub231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 49 04 	vfnmsub231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 cb    	vmulpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 cb    	vmulpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 59 cb    	vmulpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 59 cb    	vmulpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 59 cb    	vmulpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 59 cb    	vmulpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 59 cb    	vmulpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 cb    	vmulpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 59 cb    	vmulpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 59 cb    	vmulpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 59 cb    	vmulpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 09    	vmulpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 09    	vmulpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 09    	vmulpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 49 04 	vmulpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c cb    	vsubpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c cb    	vsubpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 5c cb    	vsubpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 5c cb    	vsubpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 5c cb    	vsubpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 5c cb    	vsubpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 5c cb    	vsubpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c cb    	vsubpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 5c cb    	vsubpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 5c cb    	vsubpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 5c cb    	vsubpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c 09    	vsubpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c 09    	vsubpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 09    	vsubpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 49 04 	vsubpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d cb    	vsubrpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d cb    	vsubrpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 6d cb    	vsubrpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 6d cb    	vsubrpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 6d cb    	vsubrpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 6d cb    	vsubrpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 6d cb    	vsubrpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d cb    	vsubrpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 6d cb    	vsubrpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 6d cb    	vsubrpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 6d cb    	vsubrpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d 09    	vsubrpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d 09    	vsubrpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 09    	vsubrpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 49 04 	vsubrpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce cb    	vclampzpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce cb    	vclampzpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ce cb    	vclampzpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ce cb    	vclampzpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ce cb    	vclampzpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ce cb    	vclampzpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ce cb    	vclampzpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ce cb    	vclampzpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ce 09    	vclampzpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce 09    	vclampzpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 09    	vclampzpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 49 04 	vclampzpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 cb    	vfixupnanpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 cb    	vfixupnanpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 55 cb    	vfixupnanpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 55 cb    	vfixupnanpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 55 cb    	vfixupnanpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 55 cb    	vfixupnanpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 55 cb    	vfixupnanpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 55 cb    	vfixupnanpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 55 09    	vfixupnanpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 09    	vfixupnanpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 09    	vfixupnanpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 49 04 	vfixupnanpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 cb    	vgmaxpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 cb    	vgmaxpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 53 cb    	vgmaxpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 53 cb    	vgmaxpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 53 cb    	vgmaxpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 53 cb    	vgmaxpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 53 cb    	vgmaxpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 53 cb    	vgmaxpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 53 09    	vgmaxpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 09    	vgmaxpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 09    	vgmaxpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 49 04 	vgmaxpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 cb    	vgminpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 cb    	vgminpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 52 cb    	vgminpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 52 cb    	vgminpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 52 cb    	vgminpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 52 cb    	vgminpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 52 cb    	vgminpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 52 cb    	vgminpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 52 09    	vgminpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 09    	vgminpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 09    	vgminpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 49 04 	vgminpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 cb    	vblendmpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 cb    	vblendmpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 65 cb    	vblendmpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 65 cb    	vblendmpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 65 cb    	vblendmpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 65 cb    	vblendmpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 65 cb    	vblendmpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 09    	vblendmpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 09    	vblendmpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 09    	vblendmpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 49 04 	vblendmpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 54 cb    	vfixupps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 cb    	vfixupps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 cb    	vfixupps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 cb    	vfixupps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 cb    	vfixupps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 cb    	vfixupps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 54 cb    	vfixupps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 09    	vfixupps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 54 09    	vfixupps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 09    	vfixupps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 09    	vfixupps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 09    	vfixupps \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 09    	vfixupps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 09    	vfixupps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 49 04 	vfixupps 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fe cb    	vpaddd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe cb    	vpaddd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe cb    	vpaddd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe cb    	vpaddd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe cb    	vpaddd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe cb    	vpaddd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fe cb    	vpaddd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe 09    	vpaddd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fe 09    	vpaddd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe 09    	vpaddd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe 09    	vpaddd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe 09    	vpaddd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe 09    	vpaddd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 09    	vpaddd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 49 04 	vpaddd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 56 cb    	vpabsdiffd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 cb    	vpabsdiffd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 cb    	vpabsdiffd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 cb    	vpabsdiffd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 cb    	vpabsdiffd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 cb    	vpabsdiffd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 56 cb    	vpabsdiffd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 09    	vpabsdiffd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 56 09    	vpabsdiffd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 09    	vpabsdiffd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 09    	vpabsdiffd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 09    	vpabsdiffd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 09    	vpabsdiffd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 09    	vpabsdiffd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 49 04 	vpabsdiffd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cd cb    	vpaddsetsd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd cb    	vpaddsetsd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd cb    	vpaddsetsd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd cb    	vpaddsetsd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd cb    	vpaddsetsd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd cb    	vpaddsetsd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cd cb    	vpaddsetsd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd 09    	vpaddsetsd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cd 09    	vpaddsetsd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd 09    	vpaddsetsd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd 09    	vpaddsetsd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd 09    	vpaddsetsd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd 09    	vpaddsetsd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 09    	vpaddsetsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 49 04 	vpaddsetsd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 db cb    	vpandd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db cb    	vpandd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db cb    	vpandd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db cb    	vpandd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db cb    	vpandd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db cb    	vpandd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 db cb    	vpandd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db 09    	vpandd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 db 09    	vpandd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db 09    	vpandd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db 09    	vpandd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db 09    	vpandd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db 09    	vpandd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 09    	vpandd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 49 04 	vpandd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 df cb    	vpandnd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df cb    	vpandnd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df cb    	vpandnd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df cb    	vpandnd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df cb    	vpandnd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df cb    	vpandnd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 df cb    	vpandnd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df 09    	vpandnd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 df 09    	vpandnd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df 09    	vpandnd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df 09    	vpandnd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df 09    	vpandnd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df 09    	vpandnd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 09    	vpandnd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 49 04 	vpandnd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 64 cb    	vpblendmd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 cb    	vpblendmd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 cb    	vpblendmd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 cb    	vpblendmd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 cb    	vpblendmd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 cb    	vpblendmd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 64 cb    	vpblendmd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 09    	vpblendmd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 64 09    	vpblendmd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 09    	vpblendmd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 09    	vpblendmd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 09    	vpblendmd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 09    	vpblendmd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 09    	vpblendmd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 49 04 	vpblendmd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 70 cb    	vpbitinterleave11d %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 cb    	vpbitinterleave11d %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 cb    	vpbitinterleave11d %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 cb    	vpbitinterleave11d %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 cb    	vpbitinterleave11d %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 cb    	vpbitinterleave11d %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 70 cb    	vpbitinterleave11d %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 09    	vpbitinterleave11d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 70 09    	vpbitinterleave11d \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 09    	vpbitinterleave11d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 09    	vpbitinterleave11d \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 09    	vpbitinterleave11d \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 09    	vpbitinterleave11d \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 09    	vpbitinterleave11d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 49 04 	vpbitinterleave11d 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 71 cb    	vpbitinterleave21d %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 cb    	vpbitinterleave21d %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 cb    	vpbitinterleave21d %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 cb    	vpbitinterleave21d %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 cb    	vpbitinterleave21d %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 cb    	vpbitinterleave21d %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 71 cb    	vpbitinterleave21d %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 09    	vpbitinterleave21d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 71 09    	vpbitinterleave21d \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 09    	vpbitinterleave21d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 09    	vpbitinterleave21d \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 09    	vpbitinterleave21d \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 09    	vpbitinterleave21d \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 09    	vpbitinterleave21d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 49 04 	vpbitinterleave21d 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cf cb    	vpclampzd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf cb    	vpclampzd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf cb    	vpclampzd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf cb    	vpclampzd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf cb    	vpclampzd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf cb    	vpclampzd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cf cb    	vpclampzd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf 09    	vpclampzd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cf 09    	vpclampzd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf 09    	vpclampzd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf 09    	vpclampzd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf 09    	vpclampzd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf 09    	vpclampzd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 09    	vpclampzd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 49 04 	vpclampzd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 72 cb    	vpinsertfieldd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 cb    	vpinsertfieldd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 cb    	vpinsertfieldd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 cb    	vpinsertfieldd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 cb    	vpinsertfieldd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 cb    	vpinsertfieldd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 72 cb    	vpinsertfieldd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 09    	vpinsertfieldd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 72 09    	vpinsertfieldd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 09    	vpinsertfieldd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 09    	vpinsertfieldd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 09    	vpinsertfieldd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 09    	vpinsertfieldd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 09    	vpinsertfieldd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 49 04 	vpinsertfieldd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 cb    	vpmadd231d %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 cb    	vpmadd231d %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 cb    	vpmadd231d %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 cb    	vpmadd231d %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 cb    	vpmadd231d %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 cb    	vpmadd231d %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b5 cb    	vpmadd231d %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 09    	vpmadd231d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 09    	vpmadd231d \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 09    	vpmadd231d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 09    	vpmadd231d \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 09    	vpmadd231d \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 09    	vpmadd231d \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 09    	vpmadd231d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 49 04 	vpmadd231d 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3d cb    	vpmaxsd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d cb    	vpmaxsd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d cb    	vpmaxsd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d cb    	vpmaxsd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d cb    	vpmaxsd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d cb    	vpmaxsd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3d cb    	vpmaxsd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d 09    	vpmaxsd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3d 09    	vpmaxsd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d 09    	vpmaxsd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d 09    	vpmaxsd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d 09    	vpmaxsd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d 09    	vpmaxsd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 09    	vpmaxsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 49 04 	vpmaxsd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3f cb    	vpmaxud %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f cb    	vpmaxud %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f cb    	vpmaxud %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f cb    	vpmaxud %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f cb    	vpmaxud %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f cb    	vpmaxud %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3f cb    	vpmaxud %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f 09    	vpmaxud \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3f 09    	vpmaxud \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f 09    	vpmaxud \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f 09    	vpmaxud \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f 09    	vpmaxud \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f 09    	vpmaxud \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 09    	vpmaxud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 49 04 	vpmaxud 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 39 cb    	vpminsd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 cb    	vpminsd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 cb    	vpminsd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 cb    	vpminsd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 cb    	vpminsd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 cb    	vpminsd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 39 cb    	vpminsd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 09    	vpminsd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 39 09    	vpminsd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 09    	vpminsd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 09    	vpminsd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 09    	vpminsd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 09    	vpminsd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 09    	vpminsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 49 04 	vpminsd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3b cb    	vpminud %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b cb    	vpminud %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b cb    	vpminud %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b cb    	vpminud %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b cb    	vpminud %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b cb    	vpminud %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3b cb    	vpminud %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b 09    	vpminud \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3b 09    	vpminud \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b 09    	vpminud \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b 09    	vpminud \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b 09    	vpminud \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b 09    	vpminud \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 09    	vpminud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 49 04 	vpminud 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 87 cb    	vpmulhd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 cb    	vpmulhd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 cb    	vpmulhd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 cb    	vpmulhd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 cb    	vpmulhd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 cb    	vpmulhd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 87 cb    	vpmulhd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 09    	vpmulhd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 87 09    	vpmulhd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 09    	vpmulhd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 09    	vpmulhd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 09    	vpmulhd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 09    	vpmulhd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 09    	vpmulhd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 49 04 	vpmulhd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 86 cb    	vpmulhud %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 cb    	vpmulhud %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 cb    	vpmulhud %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 cb    	vpmulhud %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 cb    	vpmulhud %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 cb    	vpmulhud %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 86 cb    	vpmulhud %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 09    	vpmulhud \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 86 09    	vpmulhud \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 09    	vpmulhud \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 09    	vpmulhud \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 09    	vpmulhud \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 09    	vpmulhud \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 09    	vpmulhud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 49 04 	vpmulhud 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 40 cb    	vpmulld %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 cb    	vpmulld %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 cb    	vpmulld %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 cb    	vpmulld %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 cb    	vpmulld %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 cb    	vpmulld %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 40 cb    	vpmulld %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 09    	vpmulld \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 40 09    	vpmulld \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 09    	vpmulld \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 09    	vpmulld \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 09    	vpmulld \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 09    	vpmulld \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 09    	vpmulld \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 49 04 	vpmulld 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 eb cb    	vpord  %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb cb    	vpord  %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb cb    	vpord  %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb cb    	vpord  %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb cb    	vpord  %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb cb    	vpord  %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 eb cb    	vpord  %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb 09    	vpord  \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 eb 09    	vpord  \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb 09    	vpord  \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb 09    	vpord  \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb 09    	vpord  \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb 09    	vpord  \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 09    	vpord  \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 49 04 	vpord  0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 73 cb    	vprotatefieldd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 cb    	vprotatefieldd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 cb    	vprotatefieldd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 cb    	vprotatefieldd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 cb    	vprotatefieldd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 cb    	vprotatefieldd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 73 cb    	vprotatefieldd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 09    	vprotatefieldd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 73 09    	vprotatefieldd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 09    	vprotatefieldd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 09    	vprotatefieldd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 09    	vprotatefieldd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 09    	vprotatefieldd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 09    	vprotatefieldd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 49 04 	vprotatefieldd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 47 cb    	vpsllvd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 cb    	vpsllvd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 cb    	vpsllvd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 cb    	vpsllvd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 cb    	vpsllvd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 cb    	vpsllvd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 47 cb    	vpsllvd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 09    	vpsllvd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 47 09    	vpsllvd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 09    	vpsllvd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 09    	vpsllvd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 09    	vpsllvd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 09    	vpsllvd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 09    	vpsllvd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 49 04 	vpsllvd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 46 cb    	vpsravd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 cb    	vpsravd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 cb    	vpsravd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 cb    	vpsravd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 cb    	vpsravd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 cb    	vpsravd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 46 cb    	vpsravd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 09    	vpsravd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 46 09    	vpsravd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 09    	vpsravd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 09    	vpsravd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 09    	vpsravd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 09    	vpsravd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 09    	vpsravd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 49 04 	vpsravd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 45 cb    	vpsrlvd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 cb    	vpsrlvd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 cb    	vpsrlvd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 cb    	vpsrlvd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 cb    	vpsrlvd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 cb    	vpsrlvd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 45 cb    	vpsrlvd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 09    	vpsrlvd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 45 09    	vpsrlvd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 09    	vpsrlvd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 09    	vpsrlvd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 09    	vpsrlvd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 09    	vpsrlvd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 09    	vpsrlvd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 49 04 	vpsrlvd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fa cb    	vpsubd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa cb    	vpsubd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa cb    	vpsubd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa cb    	vpsubd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa cb    	vpsubd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa cb    	vpsubd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fa cb    	vpsubd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa 09    	vpsubd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fa 09    	vpsubd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa 09    	vpsubd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa 09    	vpsubd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa 09    	vpsubd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa 09    	vpsubd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 09    	vpsubd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 49 04 	vpsubd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6c cb    	vpsubrd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c cb    	vpsubrd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c cb    	vpsubrd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c cb    	vpsubrd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c cb    	vpsubrd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c cb    	vpsubrd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6c cb    	vpsubrd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c 09    	vpsubrd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6c 09    	vpsubrd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c 09    	vpsubrd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c 09    	vpsubrd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c 09    	vpsubrd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c 09    	vpsubrd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 09    	vpsubrd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 49 04 	vpsubrd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 ef cb    	vpxord %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef cb    	vpxord %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef cb    	vpxord %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef cb    	vpxord %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef cb    	vpxord %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef cb    	vpxord %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 ef cb    	vpxord %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef 09    	vpxord \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 ef 09    	vpxord \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef 09    	vpxord \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef 09    	vpxord \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef 09    	vpxord \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef 09    	vpxord \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 09    	vpxord \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 49 04 	vpxord 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 55 cb    	vfixupnanps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 cb    	vfixupnanps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 cb    	vfixupnanps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 cb    	vfixupnanps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 cb    	vfixupnanps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 cb    	vfixupnanps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 55 cb    	vfixupnanps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 55 cb    	vfixupnanps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 55 09    	vfixupnanps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 55 09    	vfixupnanps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 09    	vfixupnanps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 09    	vfixupnanps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 09    	vfixupnanps \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 09    	vfixupnanps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 09    	vfixupnanps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 49 04 	vfixupnanps 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b4 09    	vpmadd233d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 09    	vpmadd233d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 49 08 	vpmadd233d 0x80\(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 84 cb    	vscaleps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 cb    	vscaleps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 cb    	vscaleps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 cb    	vscaleps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 cb    	vscaleps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 cb    	vscaleps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 84 cb    	vscaleps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 cb    	vscaleps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 84 cb    	vscaleps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 84 cb    	vscaleps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 84 cb    	vscaleps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 09    	vscaleps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 84 09    	vscaleps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 09    	vscaleps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 09    	vscaleps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 09    	vscaleps \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 09    	vscaleps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 09    	vscaleps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 49 04 	vscaleps 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 00 	vcmpeqps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 00 	vcmpeqps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 00 	vcmpeqps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 00 	vcmpeqps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 00 	vcmpeqps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 00 	vcmpeqps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 00 	vcmpeqps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 00 	vcmpeqps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 00 	vcmpeqps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 00 	vcmpeqps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 00 	vcmpeqps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 00 	vcmpeqps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 00 	vcmpeqps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 00 	vcmpeqps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 00 	vcmpeqps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 00 	vcmpeqps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 00 	vcmpeqps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 02 	vcmpleps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 02 	vcmpleps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 02 	vcmpleps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 02 	vcmpleps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 02 	vcmpleps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 02 	vcmpleps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 02 	vcmpleps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 02 	vcmpleps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 02 	vcmpleps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 02 	vcmpleps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 02 	vcmpleps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 02 	vcmpleps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 02 	vcmpleps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 02 	vcmpleps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 02 	vcmpleps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 02 	vcmpleps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 02 	vcmpleps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 01 	vcmpltps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 01 	vcmpltps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 01 	vcmpltps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 01 	vcmpltps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 01 	vcmpltps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 01 	vcmpltps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 01 	vcmpltps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 01 	vcmpltps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 01 	vcmpltps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 01 	vcmpltps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 01 	vcmpltps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 01 	vcmpltps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 01 	vcmpltps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 01 	vcmpltps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 01 	vcmpltps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 01 	vcmpltps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 01 	vcmpltps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 04 	vcmpneqps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 04 	vcmpneqps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 04 	vcmpneqps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 04 	vcmpneqps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 04 	vcmpneqps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 04 	vcmpneqps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 04 	vcmpneqps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 04 	vcmpneqps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 04 	vcmpneqps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 04 	vcmpneqps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 04 	vcmpneqps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 04 	vcmpneqps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 04 	vcmpneqps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 04 	vcmpneqps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 04 	vcmpneqps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 04 	vcmpneqps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 04 	vcmpneqps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 06 	vcmpnleps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 06 	vcmpnleps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 06 	vcmpnleps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 06 	vcmpnleps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 06 	vcmpnleps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 06 	vcmpnleps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 06 	vcmpnleps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 06 	vcmpnleps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 06 	vcmpnleps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 06 	vcmpnleps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 06 	vcmpnleps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 06 	vcmpnleps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 06 	vcmpnleps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 06 	vcmpnleps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 06 	vcmpnleps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 06 	vcmpnleps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 06 	vcmpnleps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 05 	vcmpnltps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 05 	vcmpnltps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 05 	vcmpnltps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 05 	vcmpnltps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 05 	vcmpnltps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 05 	vcmpnltps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 05 	vcmpnltps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 05 	vcmpnltps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 05 	vcmpnltps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 05 	vcmpnltps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 05 	vcmpnltps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 05 	vcmpnltps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 05 	vcmpnltps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 05 	vcmpnltps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 05 	vcmpnltps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 05 	vcmpnltps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 05 	vcmpnltps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 07 	vcmpordps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 07 	vcmpordps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 07 	vcmpordps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 07 	vcmpordps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 07 	vcmpordps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 07 	vcmpordps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 07 	vcmpordps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 07 	vcmpordps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 07 	vcmpordps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 07 	vcmpordps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 07 	vcmpordps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 07 	vcmpordps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 07 	vcmpordps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 07 	vcmpordps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 07 	vcmpordps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 07 	vcmpordps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 07 	vcmpordps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 03 	vcmpunordps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 03 	vcmpunordps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 03 	vcmpunordps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 03 	vcmpunordps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 03 	vcmpunordps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 03 	vcmpunordps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 03 	vcmpunordps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 03 	vcmpunordps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 03 	vcmpunordps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 03 	vcmpunordps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 03 	vcmpunordps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 03 	vcmpunordps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 03 	vcmpunordps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 03 	vcmpunordps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 03 	vcmpunordps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 03 	vcmpunordps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 03 	vcmpunordps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 00 	vcmpeqpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 00 	vcmpeqpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 00 	vcmpeqpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 00 	vcmpeqpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 00 	vcmpeqpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 00 	vcmpeqpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 00 	vcmpeqpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 00 	vcmpeqpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 00 	vcmpeqpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 00 	vcmpeqpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 00 	vcmpeqpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 00 	vcmpeqpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 02 	vcmplepd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 02 	vcmplepd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 02 	vcmplepd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 02 	vcmplepd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 02 	vcmplepd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 02 	vcmplepd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 02 	vcmplepd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 02 	vcmplepd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 02 	vcmplepd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 02 	vcmplepd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 02 	vcmplepd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 02 	vcmplepd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 01 	vcmpltpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 01 	vcmpltpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 01 	vcmpltpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 01 	vcmpltpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 01 	vcmpltpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 01 	vcmpltpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 01 	vcmpltpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 01 	vcmpltpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 01 	vcmpltpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 01 	vcmpltpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 01 	vcmpltpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 01 	vcmpltpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 04 	vcmpneqpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 04 	vcmpneqpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 04 	vcmpneqpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 04 	vcmpneqpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 04 	vcmpneqpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 04 	vcmpneqpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 04 	vcmpneqpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 04 	vcmpneqpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 04 	vcmpneqpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 04 	vcmpneqpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 04 	vcmpneqpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 04 	vcmpneqpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 06 	vcmpnlepd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 06 	vcmpnlepd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 06 	vcmpnlepd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 06 	vcmpnlepd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 06 	vcmpnlepd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 06 	vcmpnlepd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 06 	vcmpnlepd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 06 	vcmpnlepd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 06 	vcmpnlepd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 06 	vcmpnlepd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 06 	vcmpnlepd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 06 	vcmpnlepd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 05 	vcmpnltpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 05 	vcmpnltpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 05 	vcmpnltpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 05 	vcmpnltpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 05 	vcmpnltpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 05 	vcmpnltpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 05 	vcmpnltpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 05 	vcmpnltpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 05 	vcmpnltpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 05 	vcmpnltpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 05 	vcmpnltpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 05 	vcmpnltpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 07 	vcmpordpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 07 	vcmpordpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 07 	vcmpordpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 07 	vcmpordpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 07 	vcmpordpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 07 	vcmpordpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 07 	vcmpordpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 07 	vcmpordpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 07 	vcmpordpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 07 	vcmpordpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 07 	vcmpordpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 07 	vcmpordpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 03 	vcmpunordpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 03 	vcmpunordpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 03 	vcmpunordpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 03 	vcmpunordpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 03 	vcmpunordpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 03 	vcmpunordpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 03 	vcmpunordpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 03 	vcmpunordpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 03 	vcmpunordpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 03 	vcmpunordpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 03 	vcmpunordpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 03 	vcmpunordpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 cb    	vpcmpeqd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 cb    	vpcmpeqd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 cb    	vpcmpeqd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 cb    	vpcmpeqd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 cb    	vpcmpeqd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 cb    	vpcmpeqd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 76 cb    	vpcmpeqd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 09    	vpcmpeqd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 09    	vpcmpeqd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 09    	vpcmpeqd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 09    	vpcmpeqd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 09    	vpcmpeqd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 09    	vpcmpeqd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 09    	vpcmpeqd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 49 04 	vpcmpeqd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 02 	vpcmpled %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 02 	vpcmpled %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 02 	vpcmpled %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 02 	vpcmpled %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 02 	vpcmpled %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 02 	vpcmpled %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 02 	vpcmpled %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 02 	vpcmpled \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 02 	vpcmpled \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 02 	vpcmpled \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 02 	vpcmpled \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 02 	vpcmpled \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 02 	vpcmpled \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 02 	vpcmpled \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 02 	vpcmpled 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 cb    	vpcmpltd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 cb    	vpcmpltd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 cb    	vpcmpltd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 cb    	vpcmpltd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 cb    	vpcmpltd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 cb    	vpcmpltd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 74 cb    	vpcmpltd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 09    	vpcmpltd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 09    	vpcmpltd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 09    	vpcmpltd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 09    	vpcmpltd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 09    	vpcmpltd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 09    	vpcmpltd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 09    	vpcmpltd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 49 04 	vpcmpltd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 04 	vpcmpneqd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 04 	vpcmpneqd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 04 	vpcmpneqd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 04 	vpcmpneqd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 04 	vpcmpneqd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 04 	vpcmpneqd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 04 	vpcmpneqd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 04 	vpcmpneqd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 04 	vpcmpneqd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 04 	vpcmpneqd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 04 	vpcmpneqd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 04 	vpcmpneqd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 04 	vpcmpneqd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 04 	vpcmpneqd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 04 	vpcmpneqd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 05 	vpcmpnltd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 05 	vpcmpnltd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 05 	vpcmpnltd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 05 	vpcmpnltd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 05 	vpcmpnltd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 05 	vpcmpnltd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 05 	vpcmpnltd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 05 	vpcmpnltd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 05 	vpcmpnltd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 05 	vpcmpnltd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 05 	vpcmpnltd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 05 	vpcmpnltd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 05 	vpcmpnltd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 05 	vpcmpnltd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 05 	vpcmpnltd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 cb    	vptestmd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 cb    	vptestmd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 cb    	vptestmd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 cb    	vptestmd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 cb    	vptestmd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 cb    	vptestmd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 27 cb    	vptestmd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 09    	vptestmd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 09    	vptestmd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 09    	vptestmd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 09    	vptestmd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 09    	vptestmd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 09    	vptestmd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 09    	vptestmd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 49 04 	vptestmd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 64 	vcmpps \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 64 	vcmpps \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 64 	vcmpps \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 64 	vcmpps \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 64 	vcmpps \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 64 	vcmpps \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 64 	vcmpps \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 64 	vcmpps \{sae\},\$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 64 	vcmpps \$0x64,0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 64 	vcmppd \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 64 	vcmppd \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 64 	vcmppd \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 64 	vcmppd \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 64 	vcmppd \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 64 	vcmppd \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 64 	vcmppd \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 64 	vcmppd \{sae\},\$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 64 	vcmppd \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 64 	vcmppd \$0x64,\(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 64 	vcmppd \$0x64,\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 64 	vcmppd \$0x64,0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 64 	vpcmpd \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 64 	vpcmpd \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 64 	vpcmpd \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 64 	vpcmpd \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 64 	vpcmpd \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 64 	vpcmpd \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 64 	vpcmpd \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 64 	vpcmpd \$0x64,0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e cb 64 	vpcmpud \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e cb 64 	vpcmpud \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e cb 64 	vpcmpud \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e cb 64 	vpcmpud \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e cb 64 	vpcmpud \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e cb 64 	vpcmpud \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1e cb 64 	vpcmpud \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 49 04 64 	vpcmpud \$0x64,0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 db cb    	vpandq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db cb    	vpandq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 db cb    	vpandq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 db cb    	vpandq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 db cb    	vpandq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 db cb    	vpandq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 db cb    	vpandq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db 09    	vpandq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 db 09    	vpandq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 09    	vpandq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 49 04 	vpandq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 df cb    	vpandnq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df cb    	vpandnq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 df cb    	vpandnq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 df cb    	vpandnq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 df cb    	vpandnq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 df cb    	vpandnq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 df cb    	vpandnq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df 09    	vpandnq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 df 09    	vpandnq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 09    	vpandnq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 49 04 	vpandnq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 cb    	vpblendmq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 cb    	vpblendmq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 64 cb    	vpblendmq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 64 cb    	vpblendmq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 64 cb    	vpblendmq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 64 cb    	vpblendmq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 64 cb    	vpblendmq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 09    	vpblendmq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 09    	vpblendmq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 09    	vpblendmq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 49 04 	vpblendmq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb cb    	vporq  %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb cb    	vporq  %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 eb cb    	vporq  %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 eb cb    	vporq  %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 eb cb    	vporq  %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 eb cb    	vporq  %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 eb cb    	vporq  %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb 09    	vporq  \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb 09    	vporq  \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 09    	vporq  \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 49 04 	vporq  0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef cb    	vpxorq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef cb    	vpxorq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 ef cb    	vpxorq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 ef cb    	vpxorq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 ef cb    	vpxorq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 ef cb    	vpxorq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 ef cb    	vpxorq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef 09    	vpxorq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef 09    	vpxorq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 09    	vpxorq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 49 04 	vpxorq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 09    	vcvtdq2pd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 09    	vcvtdq2pd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 49 20 	vcvtdq2pd 0x80\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 09    	vcvtdq2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 49 08 	vcvtdq2pd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a 09    	vcvtps2pd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 09    	vcvtps2pd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 49 20 	vcvtps2pd 0x80\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 09    	vcvtps2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 49 08 	vcvtps2pd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a 09    	vcvtudq2pd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 09    	vcvtudq2pd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 49 20 	vcvtudq2pd 0x80\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 09    	vcvtudq2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 49 08 	vcvtudq2pd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 42 cb    	vgetexpps %zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 cb    	vgetexpps %zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 cb    	vgetexpps %zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 cb    	vgetexpps %zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 cb    	vgetexpps %zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 cb    	vgetexpps %zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 cb    	vgetexpps %zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 42 cb    	vgetexpps \{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 42 09    	vgetexpps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 42 09    	vgetexpps \(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 09    	vgetexpps \(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 09    	vgetexpps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 09    	vgetexpps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 09    	vgetexpps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 09    	vgetexpps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 09    	vgetexpps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 49 04 	vgetexpps 0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 cb    	vgetexppd %zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 cb    	vgetexppd %zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 49 42 cb    	vgetexppd %zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 59 42 cb    	vgetexppd %zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 69 42 cb    	vgetexppd %zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 79 42 cb    	vgetexppd %zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 39 42 cb    	vgetexppd %zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 09    	vgetexppd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 09    	vgetexppd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 c9 42 cb    	vgetexppd \{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 a9 42 09    	vgetexppd \(%rcx\)\{eh\}\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 49 04 	vgetexppd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a ca    	vcvtpd2ps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a ca    	vcvtpd2ps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 5a ca    	vcvtpd2ps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 5a ca    	vcvtpd2ps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 5a ca    	vcvtpd2ps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 5a ca    	vcvtpd2ps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 5a ca    	vcvtpd2ps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a ca    	vcvtpd2ps \{rn\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 99 5a ca    	vcvtpd2ps \{rd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 a9 5a ca    	vcvtpd2ps \{ru\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 b9 5a ca    	vcvtpd2ps \{rz\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps \{rn\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps \{rn\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps \{rd\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps \{ru\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps \{rz\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps \{rn\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps \{rd\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps \{ru\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps \{rz\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a 09    	vcvtpd2ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a 09    	vcvtpd2ps \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 09    	vcvtpd2ps \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 49 04 	vcvtpd2ps 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 cb cb 07 	vcvtfxpntps2dq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 49 04 07 	vcvtfxpntps2dq \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 ca cb 07 	vcvtfxpntps2udq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 49 04 07 	vcvtfxpntps2udq \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 52 cb 07 	vrndfxpntps \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 52 49 04 07 	vrndfxpntps \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 26 cb 07 	vgetmantps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 26 cb 07 	vgetmantps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 26 cb 07 	vgetmantps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 26 cb 07 	vgetmantps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 26 cb 07 	vgetmantps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 26 cb 07 	vgetmantps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 26 cb 07 	vgetmantps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 26 cb 07 	vgetmantps \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 26 49 04 07 	vgetmantps \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 49 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 59 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 69 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 79 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 39 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb c9 e6 cb 07 	vcvtfxpntpd2dq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 49 04 07 	vcvtfxpntpd2dq \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 49 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 59 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 69 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 79 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 39 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb c9 ca cb 07 	vcvtfxpntpd2udq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 49 04 07 	vcvtfxpntpd2udq \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 49 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 59 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 69 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 79 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 39 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 c9 52 cb 07 	vrndfxpntpd \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 49 04 07 	vrndfxpntpd \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 cb 07 	vgetmantpd \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 cb 07 	vgetmantpd \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 49 26 cb 07 	vgetmantpd \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 59 26 cb 07 	vgetmantpd \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 69 26 cb 07 	vgetmantpd \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 79 26 cb 07 	vgetmantpd \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 39 26 cb 07 	vgetmantpd \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 c9 26 cb 07 	vgetmantpd \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 26 09 07 	vgetmantpd \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 09 07 	vgetmantpd \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 09 07 	vgetmantpd \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 49 04 07 	vgetmantpd \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 f3 07 	vpslld \$0x7,%zmm3\{cdab\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 f3 07 	vpslld \$0x7,%zmm3\{badc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 f3 07 	vpslld \$0x7,%zmm3\{aaaa\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 f3 07 	vpslld \$0x7,%zmm3\{bbbb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 f3 07 	vpslld \$0x7,%zmm3\{cccc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 f3 07 	vpslld \$0x7,%zmm3\{dddd\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 39 72 f3 07 	vpslld \$0x7,%zmm3\{dacb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 31 07 	vpslld \$0x7,\(%rcx\)\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 31 07 	vpslld \$0x7,\(%rcx\)\{1to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 31 07 	vpslld \$0x7,\(%rcx\)\{4to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 31 07 	vpslld \$0x7,\(%rcx\)\{uint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 31 07 	vpslld \$0x7,\(%rcx\)\{sint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 31 07 	vpslld \$0x7,\(%rcx\)\{uint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 31 07 	vpslld \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 71 04 07 	vpslld \$0x7,0x80\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 e3 07 	vpsrad \$0x7,%zmm3\{cdab\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 e3 07 	vpsrad \$0x7,%zmm3\{badc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 e3 07 	vpsrad \$0x7,%zmm3\{aaaa\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 e3 07 	vpsrad \$0x7,%zmm3\{bbbb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 e3 07 	vpsrad \$0x7,%zmm3\{cccc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 e3 07 	vpsrad \$0x7,%zmm3\{dddd\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 39 72 e3 07 	vpsrad \$0x7,%zmm3\{dacb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 21 07 	vpsrad \$0x7,\(%rcx\)\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 21 07 	vpsrad \$0x7,\(%rcx\)\{1to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 21 07 	vpsrad \$0x7,\(%rcx\)\{4to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 21 07 	vpsrad \$0x7,\(%rcx\)\{uint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 21 07 	vpsrad \$0x7,\(%rcx\)\{sint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 21 07 	vpsrad \$0x7,\(%rcx\)\{uint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 21 07 	vpsrad \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 61 04 07 	vpsrad \$0x7,0x80\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 d3 07 	vpsrld \$0x7,%zmm3\{cdab\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 d3 07 	vpsrld \$0x7,%zmm3\{badc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 d3 07 	vpsrld \$0x7,%zmm3\{aaaa\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 d3 07 	vpsrld \$0x7,%zmm3\{bbbb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 d3 07 	vpsrld \$0x7,%zmm3\{cccc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 d3 07 	vpsrld \$0x7,%zmm3\{dddd\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 39 72 d3 07 	vpsrld \$0x7,%zmm3\{dacb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 11 07 	vpsrld \$0x7,\(%rcx\)\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 11 07 	vpsrld \$0x7,\(%rcx\)\{1to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 11 07 	vpsrld \$0x7,\(%rcx\)\{4to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 11 07 	vpsrld \$0x7,\(%rcx\)\{uint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 11 07 	vpsrld \$0x7,\(%rcx\)\{sint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 11 07 	vpsrld \$0x7,\(%rcx\)\{uint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 11 07 	vpsrld \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 51 04 07 	vpsrld \$0x7,0x80\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 39 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 99 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rd\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 a9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{ru\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 b9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rz\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 49 04 07 	vcvtfxpntdq2ps \$0x7,0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 39 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 99 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rd\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 a9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{ru\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 b9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rz\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 49 04 07 	vcvtfxpntudq2ps \$0x7,0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d5 09    	vloadunpackhps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d5 09    	vloadunpackhps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d5 09    	vloadunpackhps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d5 09    	vloadunpackhps \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d5 09    	vloadunpackhps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 09    	vloadunpackhps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 49 40 	vloadunpackhps 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d1 09    	vloadunpacklps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d1 09    	vloadunpacklps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d1 09    	vloadunpacklps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d1 09    	vloadunpacklps \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d1 09    	vloadunpacklps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 09    	vloadunpacklps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 49 40 	vloadunpacklps 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 09    	vmovaps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 28 09    	vmovaps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 09    	vmovaps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 09    	vmovaps \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 09    	vmovaps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 09    	vmovaps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 49 04 	vmovaps 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 1a 09    	vbroadcastf32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 1a 09    	vbroadcastf32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 1a 09    	vbroadcastf32x4 \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 1a 09    	vbroadcastf32x4 \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 1a 09    	vbroadcastf32x4 \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 1a 09    	vbroadcastf32x4 \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 09    	vbroadcastf32x4 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 49 10 	vbroadcastf32x4 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 18 09    	vbroadcastss \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 18 09    	vbroadcastss \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 18 09    	vbroadcastss \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 18 09    	vbroadcastss \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 18 09    	vbroadcastss \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 18 09    	vbroadcastss \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 09    	vbroadcastss \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 49 40 	vbroadcastss 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 67 09    	vbroadcastaddss \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 67 09    	vbroadcastaddss \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 67 09    	vbroadcastaddss \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 67 09    	vbroadcastaddss \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 67 09    	vbroadcastaddss \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 67 09    	vbroadcastaddss \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 09    	vbroadcastaddss \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 49 40 	vbroadcastaddss 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d6 09    	vloadunpackhgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d6 09    	vloadunpackhgps \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d6 09    	vloadunpackhgps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d6 09    	vloadunpackhgps \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d6 09    	vloadunpackhgps \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 09    	vloadunpackhgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 49 40 	vloadunpackhgps 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d2 09    	vloadunpacklgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d2 09    	vloadunpacklgps \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d2 09    	vloadunpacklgps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d2 09    	vloadunpacklgps \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d2 09    	vloadunpacklgps \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 09    	vloadunpacklgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 49 40 	vloadunpacklgps 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 09    	vmovgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 68 09    	vmovgps \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 09    	vmovgps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 09    	vmovgps \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 09    	vmovgps \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 09    	vmovgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 49 04 	vmovgps 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4a 09    	vbroadcastgf32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4a 09    	vbroadcastgf32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 4a 09    	vbroadcastgf32x4 \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4a 09    	vbroadcastgf32x4 \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4a 09    	vbroadcastgf32x4 \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4a 09    	vbroadcastgf32x4 \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 09    	vbroadcastgf32x4 \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 49 10 	vbroadcastgf32x4 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 48 09    	vbroadcastgss \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 48 09    	vbroadcastgss \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 48 09    	vbroadcastgss \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 48 09    	vbroadcastgss \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 48 09    	vbroadcastgss \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 48 09    	vbroadcastgss \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 09    	vbroadcastgss \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 49 40 	vbroadcastgss 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d7 09    	vloadunpackhpgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 19 d7 09    	vloadunpackhpgps \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d7 09    	vloadunpackhpgps \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d7 09    	vloadunpackhpgps \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 49 20 	vloadunpackhpgps 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d3 09    	vloadunpacklpgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 19 d3 09    	vloadunpacklpgps \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d3 09    	vloadunpacklpgps \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d3 09    	vloadunpacklpgps \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 49 20 	vloadunpacklpgps 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 09    	vmovpgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 69 09    	vmovpgps \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 09    	vmovpgps \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 09    	vmovpgps \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 09    	vmovpgps \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 09    	vmovpgps \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 09    	vmovpgps \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 09    	vmovpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 49 02 	vmovpgps 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4b 09    	vbroadcastpgf32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 49 08 	vbroadcastpgf32x4 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 49 09    	vbroadcastpgss \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 49 09    	vbroadcastpgss \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 49 09    	vbroadcastpgss \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 49 09    	vbroadcastpgss \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 49 09    	vbroadcastpgss \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 49 09    	vbroadcastpgss \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 49 09    	vbroadcastpgss \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 49 09    	vbroadcastpgss \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 09    	vbroadcastpgss \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 49 20 	vbroadcastpgss 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d5 09    	vloadunpackhpd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 49 10 	vloadunpackhpd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d1 09    	vloadunpacklpd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 49 10 	vloadunpacklpd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 09    	vmovapd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 49 02 	vmovapd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 1b 09    	vbroadcastf64x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 49 04 	vbroadcastf64x4 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 09    	vbroadcastf64x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 19 09    	vbroadcastsd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 49 10 	vbroadcastsd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 09    	vbroadcastsd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d4 09    	vloadunpackhd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d4 09    	vloadunpackhd \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d4 09    	vloadunpackhd \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d4 09    	vloadunpackhd \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 09    	vloadunpackhd \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 49 40 	vloadunpackhd 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d0 09    	vloadunpackld \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d0 09    	vloadunpackld \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d0 09    	vloadunpackld \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d0 09    	vloadunpackld \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 09    	vloadunpackld \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 49 40 	vloadunpackld 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f 09    	vmovdqa32 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 49 6f 09    	vmovdqa32 \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f 09    	vmovdqa32 \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f 09    	vmovdqa32 \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 09    	vmovdqa32 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 49 04 	vmovdqa32 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 5a 09    	vbroadcasti32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 5a 09    	vbroadcasti32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 5a 09    	vbroadcasti32x4 \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 5a 09    	vbroadcasti32x4 \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 5a 09    	vbroadcasti32x4 \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 09    	vbroadcasti32x4 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 49 10 	vbroadcasti32x4 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 58 09    	vpbroadcastd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 58 09    	vpbroadcastd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 58 09    	vpbroadcastd \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 58 09    	vpbroadcastd \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 58 09    	vpbroadcastd \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 09    	vpbroadcastd \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 49 40 	vpbroadcastd 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d4 09    	vloadunpackhq \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 49 10 	vloadunpackhq 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d0 09    	vloadunpacklq \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 49 10 	vloadunpacklq 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f 09    	vmovdqa64 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 49 02 	vmovdqa64 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 5b 09    	vbroadcasti64x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 49 04 	vbroadcasti64x4 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 09    	vbroadcasti64x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 59 09    	vpbroadcastq \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 49 10 	vpbroadcastq 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 09    	vpbroadcastq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 71 09 03 ca 08 	valignd \$0x8,%zmm2,%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 71 09 03 09 08 	valignd \$0x8,\(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 71 09 03 49 02 08 	valignd \$0x8,0x80\(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b ca    	vcvtps2srgb8 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 6b ca    	vcvtps2srgb8 \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 09    	vcvtps2srgb8 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 6b 09    	vcvtps2srgb8 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 49 02 	vcvtps2srgb8 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 ca    	vexp223ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 c8 ca    	vexp223ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 09    	vexp223ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 c8 09    	vexp223ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 49 02 	vexp223ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 ca    	vlog2ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 c9 ca    	vlog2ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 09    	vlog2ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 c9 09    	vlog2ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 49 02 	vlog2ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca ca    	vrcp23ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 ca ca    	vrcp23ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 09    	vrcp23ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 ca 09    	vrcp23ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 49 02 	vrcp23ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb ca    	vrsqrt23ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 cb ca    	vrsqrt23ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 09    	vrsqrt23ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 cb 09    	vrsqrt23ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 49 02 	vrsqrt23ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 71 09 36 ca    	vpermd %zmm2,%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 71 09 36 09    	vpermd \(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 71 09 36 49 02 	vpermd 0x80\(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 07 ca 07 	vpermf32x4 \$0x7,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 07 09 07 	vpermf32x4 \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 07 49 02 07 	vpermf32x4 \$0x7,0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 70 ca 07 	vpshufd \$0x7,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 70 09 07 	vpshufd \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 70 49 02 07 	vpshufd \$0x7,0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d5 09    	vpackstorehps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d5 09    	vpackstorehps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d5 09    	vpackstorehps %zmm1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d5 09    	vpackstorehps %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d5 09    	vpackstorehps %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d5 09    	vpackstorehps %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d5 09    	vpackstorehps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d5 49 40 	vpackstorehps %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 89 d1 09    	vpackstorelps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d1 09    	vpackstorelps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d1 09    	vpackstorelps %zmm1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d1 09    	vpackstorelps %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d1 09    	vpackstorelps %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d1 09    	vpackstorelps %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d1 09    	vpackstorelps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d1 49 40 	vpackstorelps %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 89 29 09    	vmovaps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 29 09    	vmovaps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 29 09    	vmovaps %zmm1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 29 09    	vmovaps %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 29 09    	vmovaps %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 29 09    	vmovaps %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 29 09    	vmovaps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 78 29 49 04 	vmovaps %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 89 d6 09    	vpackstorehgps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d6 09    	vpackstorehgps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d6 09    	vpackstorehgps %zmm1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d6 09    	vpackstorehgps %zmm1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d6 09    	vpackstorehgps %zmm1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d6 09    	vpackstorehgps %zmm1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d6 09    	vpackstorehgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d6 49 40 	vpackstorehgps %zmm1\{snorm16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 89 d2 09    	vpackstorelgps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d2 09    	vpackstorelgps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d2 09    	vpackstorelgps %zmm1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d2 09    	vpackstorelgps %zmm1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d2 09    	vpackstorelgps %zmm1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d2 09    	vpackstorelgps %zmm1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d2 09    	vpackstorelgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d2 49 40 	vpackstorelgps %zmm1\{snorm16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 89 6a 09    	vmovgps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6a 09    	vmovgps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 6a 09    	vmovgps %zmm1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 6a 09    	vmovgps %zmm1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 6a 09    	vmovgps %zmm1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 6a 09    	vmovgps %zmm1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 6a 09    	vmovgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 6a 49 04 	vmovgps %zmm1\{snorm16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 89 d5 09    	vpackstorehpd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d5 49 10 	vpackstorehpd %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d5 09    	vpackstorehpd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d1 09    	vpackstorelpd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d1 49 10 	vpackstorelpd %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d1 09    	vpackstorelpd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 29 09    	vmovapd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 08 29 49 02 	vmovapd %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 f9 09 29 09    	vmovapd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d4 09    	vpackstorehd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d4 09    	vpackstorehd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d4 09    	vpackstorehd %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d4 09    	vpackstorehd %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d4 09    	vpackstorehd %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d4 09    	vpackstorehd %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d4 49 40 	vpackstorehd %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 89 d0 09    	vpackstoreld %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d0 09    	vpackstoreld %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d0 09    	vpackstoreld %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d0 09    	vpackstoreld %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d0 09    	vpackstoreld %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d0 09    	vpackstoreld %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d0 49 40 	vpackstoreld %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 79 89 7f 09    	vmovdqa32 %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 7f 09    	vmovdqa32 %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 49 7f 09    	vmovdqa32 %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 59 7f 09    	vmovdqa32 %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 69 7f 09    	vmovdqa32 %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 7f 09    	vmovdqa32 %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 78 7f 49 04 	vmovdqa32 %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 89 d4 09    	vpackstorehq %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d4 49 10 	vpackstorehq %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d4 09    	vpackstorehq %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d0 09    	vpackstorelq %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d0 49 10 	vpackstorelq %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d0 09    	vpackstorelq %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 7f 09    	vmovdqa64 %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 08 7f 49 02 	vmovdqa64 %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 f9 09 7f 09    	vmovdqa64 %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 28 ca    	vmovapd %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 28 ca    	vmovapd %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 28 ca    	vmovapd %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 28 ca    	vmovapd %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 28 ca    	vmovapd %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 28 ca    	vmovapd %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 28 ca    	vmovapd %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 28 ca    	vmovaps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 28 ca    	vmovaps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 ca    	vmovaps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 ca    	vmovaps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 ca    	vmovaps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 ca    	vmovaps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 28 ca    	vmovaps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 19 6f ca    	vmovdqa32 %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 29 6f ca    	vmovdqa32 %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 49 6f ca    	vmovdqa32 %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f ca    	vmovdqa32 %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f ca    	vmovdqa32 %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f ca    	vmovdqa32 %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 39 6f ca    	vmovdqa32 %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 6f ca    	vmovdqa64 %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 6f ca    	vmovdqa64 %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 6f ca    	vmovdqa64 %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 6f ca    	vmovdqa64 %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 6f ca    	vmovdqa64 %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 6f ca    	vmovdqa64 %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 6f ca    	vmovdqa64 %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 68 ca    	vmovgps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 68 ca    	vmovgps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 ca    	vmovgps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 ca    	vmovgps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 ca    	vmovgps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 ca    	vmovgps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 68 ca    	vmovgps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 69 ca    	vmovpgps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 ca    	vmovpgps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 ca    	vmovpgps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 ca    	vmovpgps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 ca    	vmovpgps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 ca    	vmovpgps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 ca    	vmovpgps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{uint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{sint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{uint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{sint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 4c 52 40 	vpscatterdd %zmm1\{sint16\},0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a2 8c 55 99 00 00 00 	vscatterdps %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 a2 8c 55 99 00 00 00 	vscatterdps %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{float16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{uint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{sint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{uint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{sint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 4c 52 40 	vscatterdps %zmm1\{sint16\},0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{float16rz\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{unorm8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{snorm8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{unorm16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{snorm16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 4c 52 40 	vscatterdgps %zmm1\{snorm16\},0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 4c 52 40 	vgatherpf0dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 44 52 40 	vgatherpf0hintdps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 54 52 40 	vgatherpf1dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 6c 52 40 	vscatterpf0dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 64 52 40 	vscatterpf0hintdps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 74 52 40 	vscatterpf1dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 84 55 99 00 00 00 	vgatherpf0hintdpd 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 44 52 10 	vgatherpf0hintdpd 0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdpd 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 64 52 10 	vscatterpf0hintdpd 0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 4c 52 20 	vgatherdps 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 92 8c 55 99 00 00 00 	vgatherdpd 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 4c 52 10 	vgatherdpd 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 8c 55 99 00 00 00 	vgatherdpd 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 29 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 4c 52 20 	vgatherdgps 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 19 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 29 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 4c 52 20 	vgatherdpgps 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 4c 52 20 	vpgatherdd 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 90 8c 55 99 00 00 00 	vpgatherdq 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 4c 52 10 	vpgatherdq 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 8c 55 99 00 00 00 	vpgatherdq 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 a0 8c 55 99 00 00 00 	vpscatterdq %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 4c 52 10 	vpscatterdq %zmm1,0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 8c 55 99 00 00 00 	vpscatterdq %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	c4 e2 7b f0 d8       	bitinterleave11 %eax,%ebx
[ 	]*[a-f0-9]+:	c4 42 7b f0 f9       	bitinterleave11 %r9d,%r15d
[ 	]*[a-f0-9]+:	c4 e2 7a f0 d8       	bitinterleave21 %eax,%ebx
[ 	]*[a-f0-9]+:	c4 42 7a f0 f9       	bitinterleave21 %r9d,%r15d
[ 	]*[a-f0-9]+:	c5 fa bd d8          	lzcnt  %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7a bd f9       	lzcnt  %r9d,%r15d
[ 	]*[a-f0-9]+:	c4 e2 7b f1 d8       	quadmask %eax,%ebx
[ 	]*[a-f0-9]+:	c4 42 7b f1 f9       	quadmask %r9d,%r15d
[ 	]*[a-f0-9]+:	c5 fa b8 d8          	popcnt %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7a b8 f9       	popcnt %r9d,%r15d
[ 	]*[a-f0-9]+:	c5 fa bc d8          	tzcnt  %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7a bc f9       	tzcnt  %r9d,%r15d
[ 	]*[a-f0-9]+:	c5 fb bc d8          	tzcnti %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7b bc f9       	tzcnti %r9d,%r15d
[ 	]*[a-f0-9]+:	c4 e2 fb f0 d8       	bitinterleave11 %rax,%rbx
[ 	]*[a-f0-9]+:	c4 42 fb f0 f9       	bitinterleave11 %r9,%r15
[ 	]*[a-f0-9]+:	c4 e2 fa f0 d8       	bitinterleave21 %rax,%rbx
[ 	]*[a-f0-9]+:	c4 42 fa f0 f9       	bitinterleave21 %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fa bd d8       	lzcnt  %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fa bd f9       	lzcnt  %r9,%r15
[ 	]*[a-f0-9]+:	c4 e2 fb f1 d8       	quadmask %rax,%rbx
[ 	]*[a-f0-9]+:	c4 42 fb f1 f9       	quadmask %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fa b8 d8       	popcnt %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fa b8 f9       	popcnt %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fa bc d8       	tzcnt  %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fa bc f9       	tzcnt  %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fb bc d8       	tzcnti %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fb bc f9       	tzcnti %r9,%r15
[ 	]*[a-f0-9]+:	c5 fb ae 39          	clevict0 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 7b 08 ae 79 02 	clevict0 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 fa ae 39          	clevict1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 7a 08 ae 79 02 	clevict1 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 09          	vprefetch0 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 49 02 	vprefetch0 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 11          	vprefetch1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 51 02 	vprefetch1 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 19          	vprefetch2 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 59 02 	vprefetch2 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 29          	vprefetche0 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 69 02 	vprefetche0 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 31          	vprefetche1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 71 02 	vprefetche1 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 39          	vprefetche2 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 79 02 	vprefetche2 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 21          	vprefetchenta \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 61 02 	vprefetchenta 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 01          	vprefetchnta \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 41 02 	vprefetchnta 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 fa ae f0          	delay  %eax
[ 	]*[a-f0-9]+:	c4 c1 7a ae f1       	delay  %r9d
[ 	]*[a-f0-9]+:	c5 fb ae f0          	spflt  %eax
[ 	]*[a-f0-9]+:	c4 c1 7b ae f1       	spflt  %r9d
[ 	]*[a-f0-9]+:	c4 e1 fa ae f0       	delay  %rax
[ 	]*[a-f0-9]+:	c4 c1 fa ae f1       	delay  %r9
[ 	]*[a-f0-9]+:	c4 e1 fb ae f0       	spflt  %rax
[ 	]*[a-f0-9]+:	c4 c1 fb ae f1       	spflt  %r9
[ 	]*[a-f0-9]+:	62 81 59 11 db e6    	vpandd %zmm30\{cdab\},%zmm20,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 01 59 11 db f6    	vpandd %zmm30\{cdab\},%zmm20,%zmm30\{%k1\}
[ 	]*[a-f0-9]+:	62 01 09 11 db f6    	vpandd %zmm30\{cdab\},%zmm30,%zmm30\{%k1\}
[ 	]*[a-f0-9]+:	62 81 09 11 db e6    	vpandd %zmm30\{cdab\},%zmm30,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 81 29 19 db e6    	vpandd %zmm30\{cdab\},%zmm10,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 59 01 72 31 07 	vpslld \$0x7,\(%rcx\),%zmm20
[ 	]*[a-f0-9]+:	62 91 59 01 72 f6 07 	vpslld \$0x7,%zmm30,%zmm20
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 78 56 34 12 	vpadcd 0x12345678,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 78 56 34 12 	vpadcd 0x12345678,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 55 00 	vpadcd 0x0\(%rbp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 65 00 	vpadcd 0x0\(%rbp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 24 	vpadcd \(%rsp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 24 	vpadcd \(%rsp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 95 99 00 00 00 	vpadcd 0x99\(%rbp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a5 99 00 00 00 	vpadcd 0x99\(%rbp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 52 69 09 5c 97 99 00 00 00 	vpadcd 0x99\(%r15\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a7 99 00 00 00 	vpadcd 0x99\(%r15\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 15 99 00 00 00 	vpadcd 0x99\(%rip\),%k2,%zmm10\{%k1\}        # 609f <_start\+0x609f>
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 25 99 00 00 00 	vpadcd 0x99\(%rip\),%k2,%zmm20\{%k1\}        # 60a9 <_start\+0x60a9>
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 24 99 00 00 00 	vpadcd 0x99\(%rsp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 24 99 00 00 00 	vpadcd 0x99\(%rsp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 52 69 09 5c 94 24 99 00 00 00 	vpadcd 0x99\(%r12\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a4 24 99 00 00 00 	vpadcd 0x99\(%r12\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 67 ff ff ff 	vpadcd 0xffffffffffffff67,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 67 ff ff ff 	vpadcd 0xffffffffffffff67,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 65 67 ff ff ff 	vpadcd -0x99\(,%riz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 65 67 ff ff ff 	vpadcd -0x99\(,%riz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 23 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 23 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 63 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 63 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 bc 67 ff ff ff 	vpadcd -0x99\(%r12,%r15,4\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 bc 67 ff ff ff 	vpadcd -0x99\(%r12,%r15,4\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 f8 67 ff ff ff 	vpadcd -0x99\(%r8,%r15,8\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 f8 67 ff ff ff 	vpadcd -0x99\(%r8,%r15,8\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 ad 67 ff ff ff 	vpadcd -0x99\(%rbp,%r13,4\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 ad 67 ff ff ff 	vpadcd -0x99\(%rbp,%r13,4\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 24 67 ff ff ff 	vpadcd -0x99\(%rsp,%r12,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 24 67 ff ff ff 	vpadcd -0x99\(%rsp,%r12,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 99 00 00 00 	vgatherdps 0x99\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 99 00 00 00 	vgatherdps 0x99\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0b 	vgatherdps \(%rbx,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0b 	vgatherdps \(%rbx,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 13 	vgatherdps \(%rbx,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 13 	vgatherdps \(%rbx,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 94 0b 67 ff ff ff 	vgatherdps -0x99\(%rbx,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 a4 0b 67 ff ff ff 	vgatherdps -0x99\(%rbx,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 94 13 f7 ff ff ff 	vgatherdps -0x9\(%rbx,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 a4 13 f7 ff ff ff 	vgatherdps -0x9\(%rbx,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 94 4b 67 ff ff ff 	vgatherdps -0x99\(%rbx,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 a4 4b 67 ff ff ff 	vgatherdps -0x99\(%rbx,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 94 53 f7 ff ff ff 	vgatherdps -0x9\(%rbx,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 a4 53 f7 ff ff ff 	vgatherdps -0x9\(%rbx,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 52 79 09 92 94 8c 99 00 00 00 	vgatherdps 0x99\(%r12,%zmm1,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 c2 79 09 92 a4 8c 99 00 00 00 	vgatherdps 0x99\(%r12,%zmm1,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 79 09 92 94 94 09 00 00 00 	vgatherdps 0x9\(%r12,%zmm10,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 79 09 92 a4 94 09 00 00 00 	vgatherdps 0x9\(%r12,%zmm10,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 79 01 92 94 b5 67 ff ff ff 	vgatherdps -0x99\(%r13,%zmm30,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 79 01 92 a4 b5 67 ff ff ff 	vgatherdps -0x99\(%r13,%zmm30,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 34 12 00 00 	vpadcd 0x1234,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 34 12 00 00 	vpadcd 0x1234,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 55 00 	vpadcd 0x0\(%ebp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 65 00 	vpadcd 0x0\(%ebp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 24 	vpadcd \(%esp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 24 	vpadcd \(%esp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 95 99 00 00 00 	vpadcd 0x99\(%ebp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a5 99 00 00 00 	vpadcd 0x99\(%ebp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 25 99 00 00 00 	addr32 vpadcd 0x99,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 25 99 00 00 00 	addr32 vpadcd 0x99,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 65 99 00 00 00 	addr32 vpadcd 0x99\(,%eiz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 65 99 00 00 00 	addr32 vpadcd 0x99\(,%eiz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 20 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 20 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 60 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 60 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 98 99 00 00 00 	vpadcd 0x99\(%eax,%ebx,4\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 98 99 00 00 00 	vpadcd 0x99\(%eax,%ebx,4\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 cc 99 00 00 00 	vpadcd 0x99\(%esp,%ecx,8\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 cc 99 00 00 00 	vpadcd 0x99\(%esp,%ecx,8\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 15 99 00 00 00 	vpadcd 0x99\(%ebp,%edx,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 15 99 00 00 00 	vpadcd 0x99\(%ebp,%edx,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 99 00 00 00 	vgatherdps 0x99\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 99 00 00 00 	vgatherdps 0x99\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 14 08 	vgatherdps \(%eax,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 24 08 	vgatherdps \(%eax,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 32 79 09 92 14 10 	vgatherdps \(%eax,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 a2 79 09 92 24 10 	vgatherdps \(%eax,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 08 99 00 00 00 	vgatherdps 0x99\(%eax,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 08 99 00 00 00 	vgatherdps 0x99\(%eax,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 38 09 00 00 00 	vgatherdps 0x9\(%eax,%zmm7,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 38 09 00 00 00 	vgatherdps 0x9\(%eax,%zmm7,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 48 67 ff ff ff 	vgatherdps -0x99\(%eax,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 48 67 ff ff ff 	vgatherdps -0x99\(%eax,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 78 f7 ff ff ff 	vgatherdps -0x9\(%eax,%zmm7,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 78 f7 ff ff ff 	vgatherdps -0x9\(%eax,%zmm7,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 8d 99 00 00 00 	vgatherdps 0x99\(%ebp,%zmm1,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 8d 99 00 00 00 	vgatherdps 0x99\(%ebp,%zmm1,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 bd 09 00 00 00 	vgatherdps 0x9\(%ebp,%zmm7,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 bd 09 00 00 00 	vgatherdps 0x9\(%ebp,%zmm7,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	c5 f8 41 d1          	kand   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 42 d1          	kandn  %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 43 d1          	kandnr %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 48 d1          	kmerge2l1h %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 49 d1          	kmerge2l1l %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 90 d1          	kmov   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 44 d1          	knot   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 45 d1          	kor    %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 98 d1          	kortest %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 46 d1          	kxnor  %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 47 d1          	kxor   %k1,%k2
[ 	]*[a-f0-9]+:	c5 f8 92 d0          	kmov   %rax,%k2
[ 	]*[a-f0-9]+:	c4 c1 78 92 d1       	kmov   %r9,%k2
[ 	]*[a-f0-9]+:	c5 f8 93 c1          	kmov   %k1,%rax
[ 	]*[a-f0-9]+:	c5 78 93 c9          	kmov   %k1,%r9
[ 	]*[a-f0-9]+:	c5 f0 95 c2          	kconcath %k2,%k1,%rax
[ 	]*[a-f0-9]+:	c5 70 95 ca          	kconcath %k2,%k1,%r9
[ 	]*[a-f0-9]+:	c5 f0 97 c2          	kconcatl %k2,%k1,%rax
[ 	]*[a-f0-9]+:	c5 70 97 ca          	kconcatl %k2,%k1,%r9
[ 	]*[a-f0-9]+:	c4 e3 79 3e cb 02    	kextract \$0x2,%rbx,%k1
[ 	]*[a-f0-9]+:	c4 c3 79 3e cf 02    	kextract \$0x2,%r15,%k1
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c cb    	vpadcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5c cb    	vpadcd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c cb    	vpadcd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c cb    	vpadcd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c cb    	vpadcd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c cb    	vpadcd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c cb    	vpadcd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5c cb    	vpadcd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c cb    	vpadcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5c 09    	vpadcd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5c 09    	vpadcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5c 09    	vpadcd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5c 09    	vpadcd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5c 09    	vpadcd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5c 09    	vpadcd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5c 09    	vpadcd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 09    	vpadcd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 49 04 	vpadcd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5c 09    	vpadcd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d cb    	vpaddsetcd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5d cb    	vpaddsetcd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d cb    	vpaddsetcd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d cb    	vpaddsetcd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d cb    	vpaddsetcd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d cb    	vpaddsetcd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d cb    	vpaddsetcd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5d cb    	vpaddsetcd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d cb    	vpaddsetcd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5d 09    	vpaddsetcd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5d 09    	vpaddsetcd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5d 09    	vpaddsetcd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5d 09    	vpaddsetcd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5d 09    	vpaddsetcd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5d 09    	vpaddsetcd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5d 09    	vpaddsetcd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 09    	vpaddsetcd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 49 04 	vpaddsetcd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5d 09    	vpaddsetcd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e cb    	vpsbbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5e cb    	vpsbbd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e cb    	vpsbbd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e cb    	vpsbbd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e cb    	vpsbbd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e cb    	vpsbbd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e cb    	vpsbbd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5e cb    	vpsbbd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e cb    	vpsbbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5e 09    	vpsbbd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5e 09    	vpsbbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5e 09    	vpsbbd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5e 09    	vpsbbd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5e 09    	vpsbbd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5e 09    	vpsbbd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5e 09    	vpsbbd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 09    	vpsbbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 49 04 	vpsbbd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5e 09    	vpsbbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e cb    	vpsbbrd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6e cb    	vpsbbrd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e cb    	vpsbbrd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e cb    	vpsbbrd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e cb    	vpsbbrd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e cb    	vpsbbrd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e cb    	vpsbbrd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6e cb    	vpsbbrd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e cb    	vpsbbrd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6e 09    	vpsbbrd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6e 09    	vpsbbrd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6e 09    	vpsbbrd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6e 09    	vpsbbrd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6e 09    	vpsbbrd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6e 09    	vpsbbrd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6e 09    	vpsbbrd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 09    	vpsbbrd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 49 04 	vpsbbrd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6e 09    	vpsbbrd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f cb    	vpsubrsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6f cb    	vpsubrsetbd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f cb    	vpsubrsetbd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f cb    	vpsubrsetbd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f cb    	vpsubrsetbd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f cb    	vpsubrsetbd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f cb    	vpsubrsetbd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6f cb    	vpsubrsetbd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f cb    	vpsubrsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6f 09    	vpsubrsetbd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6f 09    	vpsubrsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6f 09    	vpsubrsetbd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6f 09    	vpsubrsetbd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6f 09    	vpsubrsetbd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6f 09    	vpsubrsetbd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6f 09    	vpsubrsetbd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 09    	vpsubrsetbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 49 04 	vpsubrsetbd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6f 09    	vpsubrsetbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f cb    	vpsubsetbd %zmm3,%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5f cb    	vpsubsetbd %zmm3\{cdab\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f cb    	vpsubsetbd %zmm3\{badc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f cb    	vpsubsetbd %zmm3\{aaaa\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f cb    	vpsubsetbd %zmm3\{bbbb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f cb    	vpsubsetbd %zmm3\{cccc\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f cb    	vpsubsetbd %zmm3\{dddd\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 5f cb    	vpsubsetbd %zmm3\{dacb\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f cb    	vpsubsetbd %zmm3\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 5f 09    	vpsubsetbd \(%rcx\)\{eh\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 5f 09    	vpsubsetbd \(%rcx\),%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 5f 09    	vpsubsetbd \(%rcx\)\{1to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 5f 09    	vpsubsetbd \(%rcx\)\{4to16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 5f 09    	vpsubsetbd \(%rcx\)\{uint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 5f 09    	vpsubsetbd \(%rcx\)\{sint8\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 5f 09    	vpsubsetbd \(%rcx\)\{uint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 09    	vpsubsetbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 49 04 	vpsubsetbd 0x80\(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 5f 09    	vpsubsetbd \(%rcx\)\{sint16\},%k2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 cb    	vabsdiffps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 57 cb    	vabsdiffps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 cb    	vabsdiffps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 cb    	vabsdiffps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 cb    	vabsdiffps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 cb    	vabsdiffps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 cb    	vabsdiffps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 cb    	vabsdiffps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 cb    	vabsdiffps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 57 cb    	vabsdiffps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 57 cb    	vabsdiffps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 57 cb    	vabsdiffps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 57 cb    	vabsdiffps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 57 cb    	vabsdiffps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 57 cb    	vabsdiffps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 57 cb    	vabsdiffps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 57 09    	vabsdiffps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 57 09    	vabsdiffps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 57 09    	vabsdiffps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 57 09    	vabsdiffps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 57 09    	vabsdiffps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 57 09    	vabsdiffps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 57 09    	vabsdiffps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 57 09    	vabsdiffps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 09    	vabsdiffps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 49 04 	vabsdiffps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 57 09    	vabsdiffps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 cb    	vaddnps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 50 cb    	vaddnps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 cb    	vaddnps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 cb    	vaddnps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 cb    	vaddnps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 cb    	vaddnps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 cb    	vaddnps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 cb    	vaddnps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 cb    	vaddnps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 50 cb    	vaddnps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 50 cb    	vaddnps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 50 cb    	vaddnps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 50 cb    	vaddnps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 50 cb    	vaddnps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 50 cb    	vaddnps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 50 cb    	vaddnps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 50 09    	vaddnps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 50 09    	vaddnps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 50 09    	vaddnps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 50 09    	vaddnps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 50 09    	vaddnps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 50 09    	vaddnps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 50 09    	vaddnps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 50 09    	vaddnps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 09    	vaddnps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 49 04 	vaddnps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 50 09    	vaddnps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 cb    	vaddps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 58 cb    	vaddps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 cb    	vaddps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 cb    	vaddps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 cb    	vaddps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 cb    	vaddps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 cb    	vaddps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 cb    	vaddps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 cb    	vaddps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 99 58 cb    	vaddps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 58 cb    	vaddps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 58 cb    	vaddps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 58 cb    	vaddps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 58 cb    	vaddps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 58 cb    	vaddps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 58 cb    	vaddps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 58 09    	vaddps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 58 09    	vaddps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 58 09    	vaddps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 58 09    	vaddps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 58 09    	vaddps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 58 09    	vaddps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 58 09    	vaddps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 58 09    	vaddps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 09    	vaddps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 49 04 	vaddps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 58 09    	vaddps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc cb    	vaddsetsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cc cb    	vaddsetsps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc cb    	vaddsetsps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc cb    	vaddsetsps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc cb    	vaddsetsps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc cb    	vaddsetsps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc cb    	vaddsetsps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc cb    	vaddsetsps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc cb    	vaddsetsps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 cc cb    	vaddsetsps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 cc cb    	vaddsetsps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 cc cb    	vaddsetsps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 cc cb    	vaddsetsps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 cc cb    	vaddsetsps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 cc cb    	vaddsetsps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 cc cb    	vaddsetsps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cc 09    	vaddsetsps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cc 09    	vaddsetsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cc 09    	vaddsetsps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cc 09    	vaddsetsps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cc 09    	vaddsetsps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cc 09    	vaddsetsps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cc 09    	vaddsetsps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cc 09    	vaddsetsps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 09    	vaddsetsps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 49 04 	vaddsetsps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cc 09    	vaddsetsps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 cb    	vfmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 98 cb    	vfmadd132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 cb    	vfmadd132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 cb    	vfmadd132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 cb    	vfmadd132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 cb    	vfmadd132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 cb    	vfmadd132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 cb    	vfmadd132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 cb    	vfmadd132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 98 cb    	vfmadd132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 98 cb    	vfmadd132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 98 cb    	vfmadd132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 98 cb    	vfmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 98 cb    	vfmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 98 cb    	vfmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 98 cb    	vfmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 98 09    	vfmadd132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 98 09    	vfmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 98 09    	vfmadd132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 98 09    	vfmadd132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 98 09    	vfmadd132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 98 09    	vfmadd132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 98 09    	vfmadd132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 98 09    	vfmadd132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 09    	vfmadd132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 49 04 	vfmadd132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 98 09    	vfmadd132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 cb    	vfmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 cb    	vfmadd213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 cb    	vfmadd213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 cb    	vfmadd213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 cb    	vfmadd213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 cb    	vfmadd213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 cb    	vfmadd213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 cb    	vfmadd213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 cb    	vfmadd213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a8 cb    	vfmadd213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a8 cb    	vfmadd213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a8 cb    	vfmadd213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a8 cb    	vfmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a8 cb    	vfmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a8 cb    	vfmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a8 cb    	vfmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a8 09    	vfmadd213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a8 09    	vfmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 a8 09    	vfmadd213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a8 09    	vfmadd213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 a8 09    	vfmadd213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 a8 09    	vfmadd213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 a8 09    	vfmadd213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 a8 09    	vfmadd213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 09    	vfmadd213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 49 04 	vfmadd213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 a8 09    	vfmadd213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 cb    	vfmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 cb    	vfmadd231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 cb    	vfmadd231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 cb    	vfmadd231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 cb    	vfmadd231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 cb    	vfmadd231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 cb    	vfmadd231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 cb    	vfmadd231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 cb    	vfmadd231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 b8 cb    	vfmadd231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 b8 cb    	vfmadd231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 b8 cb    	vfmadd231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 b8 cb    	vfmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 b8 cb    	vfmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 b8 cb    	vfmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 b8 cb    	vfmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b8 09    	vfmadd231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b8 09    	vfmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b8 09    	vfmadd231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b8 09    	vfmadd231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b8 09    	vfmadd231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b8 09    	vfmadd231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b8 09    	vfmadd231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b8 09    	vfmadd231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 09    	vfmadd231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 49 04 	vfmadd231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b8 09    	vfmadd231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a cb    	vfmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9a cb    	vfmsub132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a cb    	vfmsub132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a cb    	vfmsub132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a cb    	vfmsub132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a cb    	vfmsub132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a cb    	vfmsub132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a cb    	vfmsub132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a cb    	vfmsub132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9a cb    	vfmsub132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9a cb    	vfmsub132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9a cb    	vfmsub132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9a cb    	vfmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9a cb    	vfmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9a cb    	vfmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9a cb    	vfmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9a 09    	vfmsub132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9a 09    	vfmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9a 09    	vfmsub132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9a 09    	vfmsub132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9a 09    	vfmsub132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9a 09    	vfmsub132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9a 09    	vfmsub132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9a 09    	vfmsub132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 09    	vfmsub132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 49 04 	vfmsub132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9a 09    	vfmsub132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa cb    	vfmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 aa cb    	vfmsub213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa cb    	vfmsub213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa cb    	vfmsub213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa cb    	vfmsub213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa cb    	vfmsub213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa cb    	vfmsub213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa cb    	vfmsub213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa cb    	vfmsub213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 aa cb    	vfmsub213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 aa cb    	vfmsub213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 aa cb    	vfmsub213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 aa cb    	vfmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 aa cb    	vfmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 aa cb    	vfmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 aa cb    	vfmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 aa 09    	vfmsub213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 aa 09    	vfmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 aa 09    	vfmsub213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 aa 09    	vfmsub213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 aa 09    	vfmsub213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 aa 09    	vfmsub213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 aa 09    	vfmsub213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 aa 09    	vfmsub213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 09    	vfmsub213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 49 04 	vfmsub213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 aa 09    	vfmsub213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba cb    	vfmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ba cb    	vfmsub231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba cb    	vfmsub231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba cb    	vfmsub231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba cb    	vfmsub231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba cb    	vfmsub231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba cb    	vfmsub231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba cb    	vfmsub231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba cb    	vfmsub231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ba cb    	vfmsub231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ba cb    	vfmsub231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ba cb    	vfmsub231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ba cb    	vfmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ba cb    	vfmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ba cb    	vfmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ba cb    	vfmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ba 09    	vfmsub231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ba 09    	vfmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ba 09    	vfmsub231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ba 09    	vfmsub231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ba 09    	vfmsub231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ba 09    	vfmsub231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ba 09    	vfmsub231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ba 09    	vfmsub231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 09    	vfmsub231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 49 04 	vfmsub231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ba 09    	vfmsub231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c cb    	vfnmadd132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9c cb    	vfnmadd132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c cb    	vfnmadd132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c cb    	vfnmadd132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c cb    	vfnmadd132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c cb    	vfnmadd132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c cb    	vfnmadd132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c cb    	vfnmadd132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c cb    	vfnmadd132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9c cb    	vfnmadd132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9c cb    	vfnmadd132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9c cb    	vfnmadd132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9c cb    	vfnmadd132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9c cb    	vfnmadd132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9c cb    	vfnmadd132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9c cb    	vfnmadd132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9c 09    	vfnmadd132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9c 09    	vfnmadd132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9c 09    	vfnmadd132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9c 09    	vfnmadd132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9c 09    	vfnmadd132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9c 09    	vfnmadd132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9c 09    	vfnmadd132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9c 09    	vfnmadd132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 09    	vfnmadd132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 49 04 	vfnmadd132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9c 09    	vfnmadd132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac cb    	vfnmadd213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ac cb    	vfnmadd213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac cb    	vfnmadd213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac cb    	vfnmadd213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac cb    	vfnmadd213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac cb    	vfnmadd213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac cb    	vfnmadd213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac cb    	vfnmadd213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac cb    	vfnmadd213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ac cb    	vfnmadd213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ac cb    	vfnmadd213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ac cb    	vfnmadd213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ac cb    	vfnmadd213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ac cb    	vfnmadd213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ac cb    	vfnmadd213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ac cb    	vfnmadd213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ac 09    	vfnmadd213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ac 09    	vfnmadd213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ac 09    	vfnmadd213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ac 09    	vfnmadd213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ac 09    	vfnmadd213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ac 09    	vfnmadd213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ac 09    	vfnmadd213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ac 09    	vfnmadd213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 09    	vfnmadd213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 49 04 	vfnmadd213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ac 09    	vfnmadd213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc cb    	vfnmadd231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 bc cb    	vfnmadd231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc cb    	vfnmadd231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc cb    	vfnmadd231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc cb    	vfnmadd231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc cb    	vfnmadd231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc cb    	vfnmadd231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc cb    	vfnmadd231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc cb    	vfnmadd231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 bc cb    	vfnmadd231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 bc cb    	vfnmadd231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 bc cb    	vfnmadd231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 bc cb    	vfnmadd231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 bc cb    	vfnmadd231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 bc cb    	vfnmadd231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 bc cb    	vfnmadd231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 bc 09    	vfnmadd231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 bc 09    	vfnmadd231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 bc 09    	vfnmadd231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 bc 09    	vfnmadd231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 bc 09    	vfnmadd231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 bc 09    	vfnmadd231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 bc 09    	vfnmadd231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 bc 09    	vfnmadd231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 09    	vfnmadd231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 49 04 	vfnmadd231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 bc 09    	vfnmadd231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 cb    	vfnmaddc1ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 94 cb    	vfnmaddc1ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 cb    	vfnmaddc1ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 cb    	vfnmaddc1ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 cb    	vfnmaddc1ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 cb    	vfnmaddc1ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 cb    	vfnmaddc1ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 cb    	vfnmaddc1ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 cb    	vfnmaddc1ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 94 cb    	vfnmaddc1ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 94 cb    	vfnmaddc1ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 94 cb    	vfnmaddc1ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 94 cb    	vfnmaddc1ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 94 cb    	vfnmaddc1ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 94 cb    	vfnmaddc1ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 94 cb    	vfnmaddc1ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 94 09    	vfnmaddc1ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 94 09    	vfnmaddc1ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 94 09    	vfnmaddc1ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 94 09    	vfnmaddc1ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 94 09    	vfnmaddc1ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 94 09    	vfnmaddc1ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 94 09    	vfnmaddc1ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 94 09    	vfnmaddc1ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 09    	vfnmaddc1ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 49 04 	vfnmaddc1ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 94 09    	vfnmaddc1ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e cb    	vfnmsub132ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9e cb    	vfnmsub132ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e cb    	vfnmsub132ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e cb    	vfnmsub132ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e cb    	vfnmsub132ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e cb    	vfnmsub132ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e cb    	vfnmsub132ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e cb    	vfnmsub132ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e cb    	vfnmsub132ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 9e cb    	vfnmsub132ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 9e cb    	vfnmsub132ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 9e cb    	vfnmsub132ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 9e cb    	vfnmsub132ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 9e cb    	vfnmsub132ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 9e cb    	vfnmsub132ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 9e cb    	vfnmsub132ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 9e 09    	vfnmsub132ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 9e 09    	vfnmsub132ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 9e 09    	vfnmsub132ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 9e 09    	vfnmsub132ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 9e 09    	vfnmsub132ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 9e 09    	vfnmsub132ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 9e 09    	vfnmsub132ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 9e 09    	vfnmsub132ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 09    	vfnmsub132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 49 04 	vfnmsub132ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 9e 09    	vfnmsub132ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae cb    	vfnmsub213ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ae cb    	vfnmsub213ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae cb    	vfnmsub213ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae cb    	vfnmsub213ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae cb    	vfnmsub213ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae cb    	vfnmsub213ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae cb    	vfnmsub213ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae cb    	vfnmsub213ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae cb    	vfnmsub213ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 ae cb    	vfnmsub213ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 ae cb    	vfnmsub213ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 ae cb    	vfnmsub213ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ae cb    	vfnmsub213ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 ae cb    	vfnmsub213ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 ae cb    	vfnmsub213ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 ae cb    	vfnmsub213ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ae 09    	vfnmsub213ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ae 09    	vfnmsub213ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ae 09    	vfnmsub213ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ae 09    	vfnmsub213ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ae 09    	vfnmsub213ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ae 09    	vfnmsub213ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ae 09    	vfnmsub213ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ae 09    	vfnmsub213ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 09    	vfnmsub213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 49 04 	vfnmsub213ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ae 09    	vfnmsub213ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be cb    	vfnmsub231ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 be cb    	vfnmsub231ps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be cb    	vfnmsub231ps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be cb    	vfnmsub231ps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be cb    	vfnmsub231ps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be cb    	vfnmsub231ps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be cb    	vfnmsub231ps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be cb    	vfnmsub231ps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be cb    	vfnmsub231ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 be cb    	vfnmsub231ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 be cb    	vfnmsub231ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 be cb    	vfnmsub231ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 be cb    	vfnmsub231ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 be cb    	vfnmsub231ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 be cb    	vfnmsub231ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 be cb    	vfnmsub231ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 be 09    	vfnmsub231ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 be 09    	vfnmsub231ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 be 09    	vfnmsub231ps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 be 09    	vfnmsub231ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 be 09    	vfnmsub231ps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 be 09    	vfnmsub231ps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 be 09    	vfnmsub231ps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 be 09    	vfnmsub231ps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 09    	vfnmsub231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 49 04 	vfnmsub231ps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 be 09    	vfnmsub231ps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 cb    	vmulps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 59 cb    	vmulps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 cb    	vmulps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 cb    	vmulps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 cb    	vmulps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 cb    	vmulps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 cb    	vmulps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 cb    	vmulps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 cb    	vmulps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 99 59 cb    	vmulps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 59 cb    	vmulps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 59 cb    	vmulps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 59 cb    	vmulps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 59 cb    	vmulps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 59 cb    	vmulps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 59 cb    	vmulps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 59 09    	vmulps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 59 09    	vmulps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 59 09    	vmulps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 59 09    	vmulps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 59 09    	vmulps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 59 09    	vmulps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 59 09    	vmulps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 59 09    	vmulps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 09    	vmulps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 49 04 	vmulps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 59 09    	vmulps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c cb    	vsubps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 5c cb    	vsubps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c cb    	vsubps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c cb    	vsubps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c cb    	vsubps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c cb    	vsubps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c cb    	vsubps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c cb    	vsubps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c cb    	vsubps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 99 5c cb    	vsubps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 a9 5c cb    	vsubps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 b9 5c cb    	vsubps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 c9 5c cb    	vsubps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 d9 5c cb    	vsubps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 e9 5c cb    	vsubps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 f9 5c cb    	vsubps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 89 5c 09    	vsubps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 09 5c 09    	vsubps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 19 5c 09    	vsubps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 29 5c 09    	vsubps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 49 5c 09    	vsubps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 59 5c 09    	vsubps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 39 5c 09    	vsubps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 79 5c 09    	vsubps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 09    	vsubps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 49 04 	vsubps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 69 5c 09    	vsubps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d cb    	vsubrps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6d cb    	vsubrps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d cb    	vsubrps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d cb    	vsubrps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d cb    	vsubrps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d cb    	vsubrps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d cb    	vsubrps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d cb    	vsubrps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d cb    	vsubrps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 6d cb    	vsubrps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 6d cb    	vsubrps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 6d cb    	vsubrps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 6d cb    	vsubrps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 6d cb    	vsubrps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 6d cb    	vsubrps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 6d cb    	vsubrps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6d 09    	vsubrps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6d 09    	vsubrps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6d 09    	vsubrps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6d 09    	vsubrps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6d 09    	vsubrps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6d 09    	vsubrps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6d 09    	vsubrps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6d 09    	vsubrps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 09    	vsubrps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 49 04 	vsubrps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6d 09    	vsubrps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce cb    	vclampzps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ce cb    	vclampzps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce cb    	vclampzps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce cb    	vclampzps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce cb    	vclampzps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce cb    	vclampzps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce cb    	vclampzps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce cb    	vclampzps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 ce cb    	vclampzps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 ce 09    	vclampzps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 ce 09    	vclampzps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 ce 09    	vclampzps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 ce 09    	vclampzps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 ce 09    	vclampzps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 ce 09    	vclampzps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 ce 09    	vclampzps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 ce 09    	vclampzps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 09    	vclampzps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 49 04 	vclampzps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 ce 09    	vclampzps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 cb    	vgmaxabsps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 51 cb    	vgmaxabsps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 cb    	vgmaxabsps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 cb    	vgmaxabsps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 cb    	vgmaxabsps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 cb    	vgmaxabsps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 cb    	vgmaxabsps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 cb    	vgmaxabsps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 51 cb    	vgmaxabsps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 51 09    	vgmaxabsps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 51 09    	vgmaxabsps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 51 09    	vgmaxabsps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 51 09    	vgmaxabsps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 51 09    	vgmaxabsps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 51 09    	vgmaxabsps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 51 09    	vgmaxabsps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 51 09    	vgmaxabsps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 09    	vgmaxabsps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 49 04 	vgmaxabsps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 51 09    	vgmaxabsps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 cb    	vgmaxps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 53 cb    	vgmaxps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 cb    	vgmaxps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 cb    	vgmaxps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 cb    	vgmaxps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 cb    	vgmaxps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 cb    	vgmaxps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 cb    	vgmaxps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 53 cb    	vgmaxps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 53 09    	vgmaxps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 53 09    	vgmaxps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 53 09    	vgmaxps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 53 09    	vgmaxps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 53 09    	vgmaxps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 53 09    	vgmaxps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 53 09    	vgmaxps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 53 09    	vgmaxps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 09    	vgmaxps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 49 04 	vgmaxps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 53 09    	vgmaxps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 cb    	vgminps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 52 cb    	vgminps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 cb    	vgminps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 cb    	vgminps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 cb    	vgminps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 cb    	vgminps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 cb    	vgminps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 cb    	vgminps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 52 cb    	vgminps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 52 09    	vgminps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 52 09    	vgminps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 52 09    	vgminps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 52 09    	vgminps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 52 09    	vgminps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 52 09    	vgminps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 52 09    	vgminps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 52 09    	vgminps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 09    	vgminps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 49 04 	vgminps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 52 09    	vgminps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 cb    	vfmadd233ps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 cb    	vfmadd233ps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 a4 cb    	vfmadd233ps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 a4 cb    	vfmadd233ps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 a4 cb    	vfmadd233ps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 a4 cb    	vfmadd233ps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 a4 cb    	vfmadd233ps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 a4 cb    	vfmadd233ps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 a4 cb    	vfmadd233ps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 a4 09    	vfmadd233ps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 a4 09    	vfmadd233ps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 09    	vfmadd233ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 49 08 	vfmadd233ps 0x80\(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 a4 09    	vfmadd233ps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 cb    	vblendmps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 65 cb    	vblendmps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 cb    	vblendmps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 cb    	vblendmps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 cb    	vblendmps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 cb    	vblendmps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 cb    	vblendmps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 cb    	vblendmps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 cb    	vblendmps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 65 09    	vblendmps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 65 09    	vblendmps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 65 09    	vblendmps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 65 09    	vblendmps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 65 09    	vblendmps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 65 09    	vblendmps \(%rcx\)\{unorm8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 65 09    	vblendmps \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 65 09    	vblendmps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 09    	vblendmps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 49 04 	vblendmps 0x80\(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 65 09    	vblendmps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 cb    	vaddnpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 cb    	vaddnpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 cb    	vaddnpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 50 cb    	vaddnpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 50 cb    	vaddnpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 50 cb    	vaddnpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 50 cb    	vaddnpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 50 cb    	vaddnpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 cb    	vaddnpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 50 cb    	vaddnpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 50 cb    	vaddnpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 50 cb    	vaddnpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 50 cb    	vaddnpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 50 cb    	vaddnpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 50 cb    	vaddnpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 50 cb    	vaddnpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 50 09    	vaddnpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 50 09    	vaddnpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 50 09    	vaddnpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 09    	vaddnpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 49 04 	vaddnpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 50 09    	vaddnpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 cb    	vaddpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 cb    	vaddpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 cb    	vaddpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 58 cb    	vaddpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 58 cb    	vaddpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 58 cb    	vaddpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 58 cb    	vaddpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 58 cb    	vaddpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 cb    	vaddpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 58 cb    	vaddpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 58 cb    	vaddpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 58 cb    	vaddpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 58 cb    	vaddpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 58 cb    	vaddpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 58 cb    	vaddpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 58 cb    	vaddpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 58 09    	vaddpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 58 09    	vaddpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 58 09    	vaddpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 09    	vaddpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 49 04 	vaddpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 58 09    	vaddpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 cb    	vfmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 cb    	vfmadd132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 cb    	vfmadd132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 98 cb    	vfmadd132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 98 cb    	vfmadd132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 98 cb    	vfmadd132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 98 cb    	vfmadd132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 98 cb    	vfmadd132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 cb    	vfmadd132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 98 cb    	vfmadd132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 98 cb    	vfmadd132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 98 cb    	vfmadd132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 98 cb    	vfmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 98 cb    	vfmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 98 cb    	vfmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 98 cb    	vfmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 98 09    	vfmadd132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 98 09    	vfmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 98 09    	vfmadd132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 09    	vfmadd132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 49 04 	vfmadd132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 98 09    	vfmadd132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 cb    	vfmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 cb    	vfmadd213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 cb    	vfmadd213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 a8 cb    	vfmadd213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 a8 cb    	vfmadd213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 a8 cb    	vfmadd213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 a8 cb    	vfmadd213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 a8 cb    	vfmadd213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 cb    	vfmadd213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 a8 cb    	vfmadd213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 a8 cb    	vfmadd213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 a8 cb    	vfmadd213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 a8 cb    	vfmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 a8 cb    	vfmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 a8 cb    	vfmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 a8 cb    	vfmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 a8 09    	vfmadd213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 a8 09    	vfmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 a8 09    	vfmadd213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 09    	vfmadd213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 49 04 	vfmadd213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 a8 09    	vfmadd213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 cb    	vfmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 cb    	vfmadd231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 cb    	vfmadd231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 b8 cb    	vfmadd231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 b8 cb    	vfmadd231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 b8 cb    	vfmadd231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 b8 cb    	vfmadd231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 b8 cb    	vfmadd231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 cb    	vfmadd231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 b8 cb    	vfmadd231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 b8 cb    	vfmadd231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 b8 cb    	vfmadd231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 b8 cb    	vfmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 b8 cb    	vfmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 b8 cb    	vfmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 b8 cb    	vfmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 b8 09    	vfmadd231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 b8 09    	vfmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 b8 09    	vfmadd231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 09    	vfmadd231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 49 04 	vfmadd231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 b8 09    	vfmadd231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a cb    	vfmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a cb    	vfmsub132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a cb    	vfmsub132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9a cb    	vfmsub132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9a cb    	vfmsub132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9a cb    	vfmsub132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9a cb    	vfmsub132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9a cb    	vfmsub132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a cb    	vfmsub132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9a cb    	vfmsub132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9a cb    	vfmsub132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9a cb    	vfmsub132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9a cb    	vfmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9a cb    	vfmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9a cb    	vfmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9a cb    	vfmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9a 09    	vfmsub132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9a 09    	vfmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9a 09    	vfmsub132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 09    	vfmsub132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 49 04 	vfmsub132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9a 09    	vfmsub132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa cb    	vfmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa cb    	vfmsub213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa cb    	vfmsub213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 aa cb    	vfmsub213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 aa cb    	vfmsub213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 aa cb    	vfmsub213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 aa cb    	vfmsub213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 aa cb    	vfmsub213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa cb    	vfmsub213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 aa cb    	vfmsub213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 aa cb    	vfmsub213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 aa cb    	vfmsub213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 aa cb    	vfmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 aa cb    	vfmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 aa cb    	vfmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 aa cb    	vfmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 aa 09    	vfmsub213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 aa 09    	vfmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 aa 09    	vfmsub213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 09    	vfmsub213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 49 04 	vfmsub213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 aa 09    	vfmsub213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba cb    	vfmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba cb    	vfmsub231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba cb    	vfmsub231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ba cb    	vfmsub231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ba cb    	vfmsub231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ba cb    	vfmsub231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ba cb    	vfmsub231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ba cb    	vfmsub231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba cb    	vfmsub231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ba cb    	vfmsub231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ba cb    	vfmsub231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ba cb    	vfmsub231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ba cb    	vfmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ba cb    	vfmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ba cb    	vfmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ba cb    	vfmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ba 09    	vfmsub231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ba 09    	vfmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ba 09    	vfmsub231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 09    	vfmsub231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 49 04 	vfmsub231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ba 09    	vfmsub231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c cb    	vfnmadd132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c cb    	vfnmadd132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c cb    	vfnmadd132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9c cb    	vfnmadd132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9c cb    	vfnmadd132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9c cb    	vfnmadd132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9c cb    	vfnmadd132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9c cb    	vfnmadd132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c cb    	vfnmadd132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9c cb    	vfnmadd132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9c cb    	vfnmadd132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9c cb    	vfnmadd132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9c cb    	vfnmadd132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9c cb    	vfnmadd132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9c cb    	vfnmadd132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9c cb    	vfnmadd132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9c 09    	vfnmadd132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9c 09    	vfnmadd132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9c 09    	vfnmadd132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 09    	vfnmadd132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 49 04 	vfnmadd132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9c 09    	vfnmadd132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac cb    	vfnmadd213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac cb    	vfnmadd213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac cb    	vfnmadd213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ac cb    	vfnmadd213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ac cb    	vfnmadd213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ac cb    	vfnmadd213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ac cb    	vfnmadd213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ac cb    	vfnmadd213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac cb    	vfnmadd213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ac cb    	vfnmadd213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ac cb    	vfnmadd213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ac cb    	vfnmadd213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ac cb    	vfnmadd213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ac cb    	vfnmadd213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ac cb    	vfnmadd213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ac cb    	vfnmadd213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ac 09    	vfnmadd213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ac 09    	vfnmadd213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ac 09    	vfnmadd213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 09    	vfnmadd213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 49 04 	vfnmadd213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ac 09    	vfnmadd213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc cb    	vfnmadd231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc cb    	vfnmadd231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc cb    	vfnmadd231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 bc cb    	vfnmadd231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 bc cb    	vfnmadd231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 bc cb    	vfnmadd231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 bc cb    	vfnmadd231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 bc cb    	vfnmadd231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc cb    	vfnmadd231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 bc cb    	vfnmadd231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 bc cb    	vfnmadd231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 bc cb    	vfnmadd231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 bc cb    	vfnmadd231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 bc cb    	vfnmadd231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 bc cb    	vfnmadd231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 bc cb    	vfnmadd231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 bc 09    	vfnmadd231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 bc 09    	vfnmadd231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 bc 09    	vfnmadd231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 09    	vfnmadd231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 49 04 	vfnmadd231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 bc 09    	vfnmadd231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 cb    	vfnmaddc1pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 cb    	vfnmaddc1pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 cb    	vfnmaddc1pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 94 cb    	vfnmaddc1pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 94 cb    	vfnmaddc1pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 94 cb    	vfnmaddc1pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 94 cb    	vfnmaddc1pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 94 cb    	vfnmaddc1pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 cb    	vfnmaddc1pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 94 cb    	vfnmaddc1pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 94 cb    	vfnmaddc1pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 94 cb    	vfnmaddc1pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 94 cb    	vfnmaddc1pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 94 cb    	vfnmaddc1pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 94 cb    	vfnmaddc1pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 94 cb    	vfnmaddc1pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 94 09    	vfnmaddc1pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 94 09    	vfnmaddc1pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 94 09    	vfnmaddc1pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 09    	vfnmaddc1pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 49 04 	vfnmaddc1pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 94 09    	vfnmaddc1pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e cb    	vfnmsub132pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e cb    	vfnmsub132pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e cb    	vfnmsub132pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 9e cb    	vfnmsub132pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 9e cb    	vfnmsub132pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 9e cb    	vfnmsub132pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 9e cb    	vfnmsub132pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 9e cb    	vfnmsub132pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e cb    	vfnmsub132pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 9e cb    	vfnmsub132pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 9e cb    	vfnmsub132pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 9e cb    	vfnmsub132pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 9e cb    	vfnmsub132pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 9e cb    	vfnmsub132pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 9e cb    	vfnmsub132pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 9e cb    	vfnmsub132pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 9e 09    	vfnmsub132pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 9e 09    	vfnmsub132pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 9e 09    	vfnmsub132pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 09    	vfnmsub132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 49 04 	vfnmsub132pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 9e 09    	vfnmsub132pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae cb    	vfnmsub213pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae cb    	vfnmsub213pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae cb    	vfnmsub213pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ae cb    	vfnmsub213pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ae cb    	vfnmsub213pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ae cb    	vfnmsub213pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ae cb    	vfnmsub213pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ae cb    	vfnmsub213pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae cb    	vfnmsub213pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 ae cb    	vfnmsub213pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 ae cb    	vfnmsub213pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 ae cb    	vfnmsub213pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ae cb    	vfnmsub213pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 ae cb    	vfnmsub213pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 ae cb    	vfnmsub213pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 ae cb    	vfnmsub213pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ae 09    	vfnmsub213pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ae 09    	vfnmsub213pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ae 09    	vfnmsub213pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 09    	vfnmsub213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 49 04 	vfnmsub213pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ae 09    	vfnmsub213pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be cb    	vfnmsub231pd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 be cb    	vfnmsub231pd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be cb    	vfnmsub231pd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 be cb    	vfnmsub231pd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 be cb    	vfnmsub231pd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 be cb    	vfnmsub231pd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 be cb    	vfnmsub231pd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 be cb    	vfnmsub231pd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be cb    	vfnmsub231pd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 be cb    	vfnmsub231pd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 be cb    	vfnmsub231pd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 be cb    	vfnmsub231pd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 be cb    	vfnmsub231pd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 be cb    	vfnmsub231pd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 be cb    	vfnmsub231pd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 be cb    	vfnmsub231pd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 be 09    	vfnmsub231pd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 be 09    	vfnmsub231pd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 be 09    	vfnmsub231pd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 09    	vfnmsub231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 49 04 	vfnmsub231pd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 be 09    	vfnmsub231pd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 cb    	vmulpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 cb    	vmulpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 cb    	vmulpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 59 cb    	vmulpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 59 cb    	vmulpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 59 cb    	vmulpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 59 cb    	vmulpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 59 cb    	vmulpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 cb    	vmulpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 59 cb    	vmulpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 59 cb    	vmulpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 59 cb    	vmulpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 59 cb    	vmulpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 59 cb    	vmulpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 59 cb    	vmulpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 59 cb    	vmulpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 59 09    	vmulpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 59 09    	vmulpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 59 09    	vmulpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 09    	vmulpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 49 04 	vmulpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 59 09    	vmulpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c cb    	vsubpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c cb    	vsubpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c cb    	vsubpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 5c cb    	vsubpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 5c cb    	vsubpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 5c cb    	vsubpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 5c cb    	vsubpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 5c cb    	vsubpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c cb    	vsubpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 99 5c cb    	vsubpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 a9 5c cb    	vsubpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 b9 5c cb    	vsubpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 c9 5c cb    	vsubpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 d9 5c cb    	vsubpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 e9 5c cb    	vsubpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 f9 5c cb    	vsubpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 5c 09    	vsubpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 5c 09    	vsubpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 5c 09    	vsubpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 09    	vsubpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 49 04 	vsubpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 5c 09    	vsubpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d cb    	vsubrpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d cb    	vsubrpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d cb    	vsubrpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 6d cb    	vsubrpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 6d cb    	vsubrpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 6d cb    	vsubrpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 6d cb    	vsubrpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 6d cb    	vsubrpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d cb    	vsubrpd \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 99 6d cb    	vsubrpd \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 a9 6d cb    	vsubrpd \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 b9 6d cb    	vsubrpd \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 6d cb    	vsubrpd \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 d9 6d cb    	vsubrpd \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 e9 6d cb    	vsubrpd \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 f9 6d cb    	vsubrpd \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 6d 09    	vsubrpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 6d 09    	vsubrpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 6d 09    	vsubrpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 09    	vsubrpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 49 04 	vsubrpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 6d 09    	vsubrpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce cb    	vclampzpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce cb    	vclampzpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce cb    	vclampzpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 ce cb    	vclampzpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 ce cb    	vclampzpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 ce cb    	vclampzpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 ce cb    	vclampzpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 ce cb    	vclampzpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 ce cb    	vclampzpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 ce 09    	vclampzpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 ce 09    	vclampzpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 ce 09    	vclampzpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 09    	vclampzpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 49 04 	vclampzpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 ce 09    	vclampzpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 cb    	vfixupnanpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 cb    	vfixupnanpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 cb    	vfixupnanpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 55 cb    	vfixupnanpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 55 cb    	vfixupnanpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 55 cb    	vfixupnanpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 55 cb    	vfixupnanpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 55 cb    	vfixupnanpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 55 cb    	vfixupnanpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 55 09    	vfixupnanpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 55 09    	vfixupnanpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 55 09    	vfixupnanpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 09    	vfixupnanpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 49 04 	vfixupnanpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 55 09    	vfixupnanpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 cb    	vgmaxpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 cb    	vgmaxpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 cb    	vgmaxpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 53 cb    	vgmaxpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 53 cb    	vgmaxpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 53 cb    	vgmaxpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 53 cb    	vgmaxpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 53 cb    	vgmaxpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 53 cb    	vgmaxpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 53 09    	vgmaxpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 53 09    	vgmaxpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 53 09    	vgmaxpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 09    	vgmaxpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 49 04 	vgmaxpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 53 09    	vgmaxpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 cb    	vgminpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 cb    	vgminpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 cb    	vgminpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 52 cb    	vgminpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 52 cb    	vgminpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 52 cb    	vgminpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 52 cb    	vgminpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 52 cb    	vgminpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 c9 52 cb    	vgminpd \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 52 09    	vgminpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 52 09    	vgminpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 52 09    	vgminpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 09    	vgminpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 49 04 	vgminpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 52 09    	vgminpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 cb    	vblendmpd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 cb    	vblendmpd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 cb    	vblendmpd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 65 cb    	vblendmpd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 65 cb    	vblendmpd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 65 cb    	vblendmpd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 65 cb    	vblendmpd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 65 cb    	vblendmpd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 cb    	vblendmpd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 65 09    	vblendmpd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 65 09    	vblendmpd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 65 09    	vblendmpd \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 09    	vblendmpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 49 04 	vblendmpd 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 65 09    	vblendmpd \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 cb    	vfixupps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 54 cb    	vfixupps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 cb    	vfixupps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 cb    	vfixupps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 cb    	vfixupps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 cb    	vfixupps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 cb    	vfixupps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 54 cb    	vfixupps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 cb    	vfixupps %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 54 09    	vfixupps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 54 09    	vfixupps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 54 09    	vfixupps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 54 09    	vfixupps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 54 09    	vfixupps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 54 09    	vfixupps \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 54 09    	vfixupps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 09    	vfixupps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 49 04 	vfixupps 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 54 09    	vfixupps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe cb    	vpaddd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fe cb    	vpaddd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe cb    	vpaddd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe cb    	vpaddd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe cb    	vpaddd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe cb    	vpaddd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe cb    	vpaddd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fe cb    	vpaddd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe cb    	vpaddd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fe 09    	vpaddd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fe 09    	vpaddd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fe 09    	vpaddd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fe 09    	vpaddd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fe 09    	vpaddd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fe 09    	vpaddd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fe 09    	vpaddd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 09    	vpaddd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 49 04 	vpaddd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fe 09    	vpaddd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 cb    	vpabsdiffd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 56 cb    	vpabsdiffd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 cb    	vpabsdiffd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 cb    	vpabsdiffd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 cb    	vpabsdiffd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 cb    	vpabsdiffd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 cb    	vpabsdiffd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 56 cb    	vpabsdiffd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 cb    	vpabsdiffd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 56 09    	vpabsdiffd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 56 09    	vpabsdiffd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 56 09    	vpabsdiffd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 56 09    	vpabsdiffd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 56 09    	vpabsdiffd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 56 09    	vpabsdiffd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 56 09    	vpabsdiffd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 09    	vpabsdiffd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 49 04 	vpabsdiffd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 56 09    	vpabsdiffd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd cb    	vpaddsetsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cd cb    	vpaddsetsd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd cb    	vpaddsetsd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd cb    	vpaddsetsd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd cb    	vpaddsetsd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd cb    	vpaddsetsd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd cb    	vpaddsetsd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cd cb    	vpaddsetsd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd cb    	vpaddsetsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cd 09    	vpaddsetsd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cd 09    	vpaddsetsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cd 09    	vpaddsetsd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cd 09    	vpaddsetsd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cd 09    	vpaddsetsd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cd 09    	vpaddsetsd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cd 09    	vpaddsetsd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 09    	vpaddsetsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 49 04 	vpaddsetsd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cd 09    	vpaddsetsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db cb    	vpandd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 db cb    	vpandd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db cb    	vpandd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db cb    	vpandd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db cb    	vpandd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db cb    	vpandd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db cb    	vpandd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 db cb    	vpandd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db cb    	vpandd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 db 09    	vpandd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 db 09    	vpandd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 db 09    	vpandd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 db 09    	vpandd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 db 09    	vpandd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 db 09    	vpandd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 db 09    	vpandd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 09    	vpandd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 49 04 	vpandd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 db 09    	vpandd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df cb    	vpandnd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 df cb    	vpandnd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df cb    	vpandnd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df cb    	vpandnd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df cb    	vpandnd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df cb    	vpandnd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df cb    	vpandnd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 df cb    	vpandnd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df cb    	vpandnd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 df 09    	vpandnd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 df 09    	vpandnd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 df 09    	vpandnd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 df 09    	vpandnd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 df 09    	vpandnd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 df 09    	vpandnd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 df 09    	vpandnd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 09    	vpandnd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 49 04 	vpandnd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 df 09    	vpandnd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 cb    	vpblendmd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 64 cb    	vpblendmd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 cb    	vpblendmd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 cb    	vpblendmd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 cb    	vpblendmd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 cb    	vpblendmd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 cb    	vpblendmd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 64 cb    	vpblendmd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 cb    	vpblendmd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 64 09    	vpblendmd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 64 09    	vpblendmd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 64 09    	vpblendmd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 64 09    	vpblendmd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 64 09    	vpblendmd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 64 09    	vpblendmd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 64 09    	vpblendmd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 09    	vpblendmd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 49 04 	vpblendmd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 64 09    	vpblendmd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 cb    	vpbitinterleave11d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 70 cb    	vpbitinterleave11d %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 cb    	vpbitinterleave11d %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 cb    	vpbitinterleave11d %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 cb    	vpbitinterleave11d %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 cb    	vpbitinterleave11d %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 cb    	vpbitinterleave11d %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 70 cb    	vpbitinterleave11d %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 cb    	vpbitinterleave11d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 70 09    	vpbitinterleave11d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 70 09    	vpbitinterleave11d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 70 09    	vpbitinterleave11d \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 70 09    	vpbitinterleave11d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 70 09    	vpbitinterleave11d \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 70 09    	vpbitinterleave11d \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 70 09    	vpbitinterleave11d \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 09    	vpbitinterleave11d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 49 04 	vpbitinterleave11d 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 70 09    	vpbitinterleave11d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 cb    	vpbitinterleave21d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 71 cb    	vpbitinterleave21d %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 cb    	vpbitinterleave21d %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 cb    	vpbitinterleave21d %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 cb    	vpbitinterleave21d %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 cb    	vpbitinterleave21d %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 cb    	vpbitinterleave21d %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 71 cb    	vpbitinterleave21d %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 cb    	vpbitinterleave21d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 71 09    	vpbitinterleave21d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 71 09    	vpbitinterleave21d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 71 09    	vpbitinterleave21d \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 71 09    	vpbitinterleave21d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 71 09    	vpbitinterleave21d \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 71 09    	vpbitinterleave21d \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 71 09    	vpbitinterleave21d \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 09    	vpbitinterleave21d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 49 04 	vpbitinterleave21d 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 71 09    	vpbitinterleave21d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf cb    	vpclampzd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cf cb    	vpclampzd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf cb    	vpclampzd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf cb    	vpclampzd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf cb    	vpclampzd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf cb    	vpclampzd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf cb    	vpclampzd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 cf cb    	vpclampzd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf cb    	vpclampzd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 cf 09    	vpclampzd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 cf 09    	vpclampzd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 cf 09    	vpclampzd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 cf 09    	vpclampzd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 cf 09    	vpclampzd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 cf 09    	vpclampzd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 cf 09    	vpclampzd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 09    	vpclampzd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 49 04 	vpclampzd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 cf 09    	vpclampzd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 cb    	vpinsertfieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 72 cb    	vpinsertfieldd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 cb    	vpinsertfieldd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 cb    	vpinsertfieldd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 cb    	vpinsertfieldd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 cb    	vpinsertfieldd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 cb    	vpinsertfieldd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 72 cb    	vpinsertfieldd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 cb    	vpinsertfieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 72 09    	vpinsertfieldd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 72 09    	vpinsertfieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 72 09    	vpinsertfieldd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 72 09    	vpinsertfieldd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 72 09    	vpinsertfieldd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 72 09    	vpinsertfieldd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 72 09    	vpinsertfieldd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 09    	vpinsertfieldd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 49 04 	vpinsertfieldd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 72 09    	vpinsertfieldd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 cb    	vpmadd231d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 cb    	vpmadd231d %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 cb    	vpmadd231d %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 cb    	vpmadd231d %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 cb    	vpmadd231d %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 cb    	vpmadd231d %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 cb    	vpmadd231d %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 b5 cb    	vpmadd231d %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 cb    	vpmadd231d %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b5 09    	vpmadd231d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b5 09    	vpmadd231d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 b5 09    	vpmadd231d \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b5 09    	vpmadd231d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 b5 09    	vpmadd231d \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 b5 09    	vpmadd231d \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 b5 09    	vpmadd231d \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 09    	vpmadd231d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 49 04 	vpmadd231d 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 b5 09    	vpmadd231d \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d cb    	vpmaxsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3d cb    	vpmaxsd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d cb    	vpmaxsd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d cb    	vpmaxsd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d cb    	vpmaxsd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d cb    	vpmaxsd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d cb    	vpmaxsd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3d cb    	vpmaxsd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d cb    	vpmaxsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3d 09    	vpmaxsd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3d 09    	vpmaxsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3d 09    	vpmaxsd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3d 09    	vpmaxsd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3d 09    	vpmaxsd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3d 09    	vpmaxsd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3d 09    	vpmaxsd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 09    	vpmaxsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 49 04 	vpmaxsd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3d 09    	vpmaxsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f cb    	vpmaxud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3f cb    	vpmaxud %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f cb    	vpmaxud %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f cb    	vpmaxud %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f cb    	vpmaxud %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f cb    	vpmaxud %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f cb    	vpmaxud %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3f cb    	vpmaxud %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f cb    	vpmaxud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3f 09    	vpmaxud \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3f 09    	vpmaxud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3f 09    	vpmaxud \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3f 09    	vpmaxud \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3f 09    	vpmaxud \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3f 09    	vpmaxud \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3f 09    	vpmaxud \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 09    	vpmaxud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 49 04 	vpmaxud 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3f 09    	vpmaxud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 cb    	vpminsd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 39 cb    	vpminsd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 cb    	vpminsd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 cb    	vpminsd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 cb    	vpminsd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 cb    	vpminsd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 cb    	vpminsd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 39 cb    	vpminsd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 cb    	vpminsd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 39 09    	vpminsd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 39 09    	vpminsd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 39 09    	vpminsd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 39 09    	vpminsd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 39 09    	vpminsd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 39 09    	vpminsd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 39 09    	vpminsd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 09    	vpminsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 49 04 	vpminsd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 39 09    	vpminsd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b cb    	vpminud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3b cb    	vpminud %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b cb    	vpminud %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b cb    	vpminud %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b cb    	vpminud %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b cb    	vpminud %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b cb    	vpminud %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 3b cb    	vpminud %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b cb    	vpminud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 3b 09    	vpminud \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 3b 09    	vpminud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 3b 09    	vpminud \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 3b 09    	vpminud \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 3b 09    	vpminud \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 3b 09    	vpminud \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 3b 09    	vpminud \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 09    	vpminud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 49 04 	vpminud 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 3b 09    	vpminud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 cb    	vpmulhd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 87 cb    	vpmulhd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 cb    	vpmulhd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 cb    	vpmulhd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 cb    	vpmulhd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 cb    	vpmulhd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 cb    	vpmulhd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 87 cb    	vpmulhd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 cb    	vpmulhd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 87 09    	vpmulhd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 87 09    	vpmulhd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 87 09    	vpmulhd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 87 09    	vpmulhd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 87 09    	vpmulhd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 87 09    	vpmulhd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 87 09    	vpmulhd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 09    	vpmulhd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 49 04 	vpmulhd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 87 09    	vpmulhd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 cb    	vpmulhud %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 86 cb    	vpmulhud %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 cb    	vpmulhud %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 cb    	vpmulhud %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 cb    	vpmulhud %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 cb    	vpmulhud %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 cb    	vpmulhud %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 86 cb    	vpmulhud %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 cb    	vpmulhud %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 86 09    	vpmulhud \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 86 09    	vpmulhud \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 86 09    	vpmulhud \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 86 09    	vpmulhud \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 86 09    	vpmulhud \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 86 09    	vpmulhud \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 86 09    	vpmulhud \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 09    	vpmulhud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 49 04 	vpmulhud 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 86 09    	vpmulhud \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 cb    	vpmulld %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 40 cb    	vpmulld %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 cb    	vpmulld %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 cb    	vpmulld %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 cb    	vpmulld %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 cb    	vpmulld %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 cb    	vpmulld %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 40 cb    	vpmulld %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 cb    	vpmulld %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 40 09    	vpmulld \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 40 09    	vpmulld \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 40 09    	vpmulld \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 40 09    	vpmulld \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 40 09    	vpmulld \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 40 09    	vpmulld \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 40 09    	vpmulld \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 09    	vpmulld \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 49 04 	vpmulld 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 40 09    	vpmulld \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb cb    	vpord  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 eb cb    	vpord  %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb cb    	vpord  %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb cb    	vpord  %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb cb    	vpord  %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb cb    	vpord  %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb cb    	vpord  %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 eb cb    	vpord  %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb cb    	vpord  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 eb 09    	vpord  \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 eb 09    	vpord  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 eb 09    	vpord  \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 eb 09    	vpord  \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 eb 09    	vpord  \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 eb 09    	vpord  \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 eb 09    	vpord  \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 09    	vpord  \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 49 04 	vpord  0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 eb 09    	vpord  \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 cb    	vprotatefieldd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 73 cb    	vprotatefieldd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 cb    	vprotatefieldd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 cb    	vprotatefieldd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 cb    	vprotatefieldd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 cb    	vprotatefieldd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 cb    	vprotatefieldd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 73 cb    	vprotatefieldd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 cb    	vprotatefieldd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 73 09    	vprotatefieldd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 73 09    	vprotatefieldd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 73 09    	vprotatefieldd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 73 09    	vprotatefieldd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 73 09    	vprotatefieldd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 73 09    	vprotatefieldd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 73 09    	vprotatefieldd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 09    	vprotatefieldd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 49 04 	vprotatefieldd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 73 09    	vprotatefieldd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 cb    	vpsllvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 47 cb    	vpsllvd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 cb    	vpsllvd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 cb    	vpsllvd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 cb    	vpsllvd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 cb    	vpsllvd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 cb    	vpsllvd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 47 cb    	vpsllvd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 cb    	vpsllvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 47 09    	vpsllvd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 47 09    	vpsllvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 47 09    	vpsllvd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 47 09    	vpsllvd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 47 09    	vpsllvd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 47 09    	vpsllvd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 47 09    	vpsllvd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 09    	vpsllvd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 49 04 	vpsllvd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 47 09    	vpsllvd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 cb    	vpsravd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 46 cb    	vpsravd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 cb    	vpsravd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 cb    	vpsravd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 cb    	vpsravd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 cb    	vpsravd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 cb    	vpsravd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 46 cb    	vpsravd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 cb    	vpsravd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 46 09    	vpsravd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 46 09    	vpsravd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 46 09    	vpsravd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 46 09    	vpsravd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 46 09    	vpsravd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 46 09    	vpsravd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 46 09    	vpsravd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 09    	vpsravd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 49 04 	vpsravd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 46 09    	vpsravd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 cb    	vpsrlvd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 45 cb    	vpsrlvd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 cb    	vpsrlvd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 cb    	vpsrlvd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 cb    	vpsrlvd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 cb    	vpsrlvd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 cb    	vpsrlvd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 45 cb    	vpsrlvd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 cb    	vpsrlvd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 45 09    	vpsrlvd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 45 09    	vpsrlvd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 45 09    	vpsrlvd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 45 09    	vpsrlvd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 45 09    	vpsrlvd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 45 09    	vpsrlvd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 45 09    	vpsrlvd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 09    	vpsrlvd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 49 04 	vpsrlvd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 45 09    	vpsrlvd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa cb    	vpsubd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fa cb    	vpsubd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa cb    	vpsubd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa cb    	vpsubd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa cb    	vpsubd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa cb    	vpsubd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa cb    	vpsubd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 fa cb    	vpsubd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa cb    	vpsubd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 fa 09    	vpsubd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 fa 09    	vpsubd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 fa 09    	vpsubd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 fa 09    	vpsubd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 fa 09    	vpsubd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 fa 09    	vpsubd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 fa 09    	vpsubd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 09    	vpsubd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 49 04 	vpsubd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 fa 09    	vpsubd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c cb    	vpsubrd %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6c cb    	vpsubrd %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c cb    	vpsubrd %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c cb    	vpsubrd %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c cb    	vpsubrd %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c cb    	vpsubrd %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c cb    	vpsubrd %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 6c cb    	vpsubrd %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c cb    	vpsubrd %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 6c 09    	vpsubrd \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 6c 09    	vpsubrd \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 6c 09    	vpsubrd \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 6c 09    	vpsubrd \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 6c 09    	vpsubrd \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 6c 09    	vpsubrd \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 6c 09    	vpsubrd \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 09    	vpsubrd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 49 04 	vpsubrd 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 6c 09    	vpsubrd \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef cb    	vpxord %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 ef cb    	vpxord %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef cb    	vpxord %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef cb    	vpxord %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef cb    	vpxord %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef cb    	vpxord %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef cb    	vpxord %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 39 ef cb    	vpxord %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef cb    	vpxord %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 89 ef 09    	vpxord \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 09 ef 09    	vpxord \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 19 ef 09    	vpxord \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 29 ef 09    	vpxord \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 49 ef 09    	vpxord \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 59 ef 09    	vpxord \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 69 ef 09    	vpxord \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 09    	vpxord \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 49 04 	vpxord 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 69 79 ef 09    	vpxord \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 cb    	vfixupnanps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 55 cb    	vfixupnanps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 cb    	vfixupnanps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 cb    	vfixupnanps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 cb    	vfixupnanps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 cb    	vfixupnanps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 cb    	vfixupnanps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 55 cb    	vfixupnanps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 55 cb    	vfixupnanps \{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 55 09    	vfixupnanps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 55 09    	vfixupnanps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 55 09    	vfixupnanps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 55 09    	vfixupnanps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 55 09    	vfixupnanps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 55 09    	vfixupnanps \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 55 09    	vfixupnanps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 09    	vfixupnanps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 49 04 	vfixupnanps 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 55 09    	vfixupnanps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 cb    	vpmadd233d %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 b4 09    	vpmadd233d \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 b4 09    	vpmadd233d \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 09    	vpmadd233d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 49 08 	vpmadd233d 0x80\(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 b4 09    	vpmadd233d \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 cb    	vscaleps %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 84 cb    	vscaleps %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 cb    	vscaleps %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 cb    	vscaleps %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 cb    	vscaleps %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 cb    	vscaleps %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 cb    	vscaleps %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 84 cb    	vscaleps %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 cb    	vscaleps \{rn\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 99 84 cb    	vscaleps \{rd\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 a9 84 cb    	vscaleps \{ru\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 b9 84 cb    	vscaleps \{rz\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 c9 84 cb    	vscaleps \{rn\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 d9 84 cb    	vscaleps \{rd\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 e9 84 cb    	vscaleps \{ru\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 f9 84 cb    	vscaleps \{rz\}\{sae\},%zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 84 09    	vscaleps \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 84 09    	vscaleps \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 19 84 09    	vscaleps \(%rcx\)\{1to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 29 84 09    	vscaleps \(%rcx\)\{4to16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 84 09    	vscaleps \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 84 09    	vscaleps \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 84 09    	vscaleps \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 09    	vscaleps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 49 04 	vscaleps 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 84 09    	vscaleps \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 00 	vcmpeqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 00 	vcmpeqps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 00 	vcmpeqps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 00 	vcmpeqps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 00 	vcmpeqps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 00 	vcmpeqps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 00 	vcmpeqps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 00 	vcmpeqps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 00 	vcmpeqps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 00 	vcmpeqps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 00 	vcmpeqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 00 	vcmpeqps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 00 	vcmpeqps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 00 	vcmpeqps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 00 	vcmpeqps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 00 	vcmpeqps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 00 	vcmpeqps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 00 	vcmpeqps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 00 	vcmpeqps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 00 	vcmpeqps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 02 	vcmpleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 02 	vcmpleps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 02 	vcmpleps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 02 	vcmpleps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 02 	vcmpleps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 02 	vcmpleps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 02 	vcmpleps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 02 	vcmpleps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 02 	vcmpleps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 02 	vcmpleps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 02 	vcmpleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 02 	vcmpleps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 02 	vcmpleps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 02 	vcmpleps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 02 	vcmpleps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 02 	vcmpleps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 02 	vcmpleps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 02 	vcmpleps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 02 	vcmpleps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 02 	vcmpleps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 01 	vcmpltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 01 	vcmpltps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 01 	vcmpltps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 01 	vcmpltps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 01 	vcmpltps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 01 	vcmpltps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 01 	vcmpltps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 01 	vcmpltps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 01 	vcmpltps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 01 	vcmpltps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 01 	vcmpltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 01 	vcmpltps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 01 	vcmpltps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 01 	vcmpltps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 01 	vcmpltps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 01 	vcmpltps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 01 	vcmpltps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 01 	vcmpltps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 01 	vcmpltps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 01 	vcmpltps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 04 	vcmpneqps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 04 	vcmpneqps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 04 	vcmpneqps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 04 	vcmpneqps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 04 	vcmpneqps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 04 	vcmpneqps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 04 	vcmpneqps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 04 	vcmpneqps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 04 	vcmpneqps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 04 	vcmpneqps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 04 	vcmpneqps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 04 	vcmpneqps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 04 	vcmpneqps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 04 	vcmpneqps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 04 	vcmpneqps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 04 	vcmpneqps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 04 	vcmpneqps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 04 	vcmpneqps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 04 	vcmpneqps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 04 	vcmpneqps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 06 	vcmpnleps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 06 	vcmpnleps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 06 	vcmpnleps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 06 	vcmpnleps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 06 	vcmpnleps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 06 	vcmpnleps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 06 	vcmpnleps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 06 	vcmpnleps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 06 	vcmpnleps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 06 	vcmpnleps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 06 	vcmpnleps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 06 	vcmpnleps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 06 	vcmpnleps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 06 	vcmpnleps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 06 	vcmpnleps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 06 	vcmpnleps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 06 	vcmpnleps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 06 	vcmpnleps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 06 	vcmpnleps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 06 	vcmpnleps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 05 	vcmpnltps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 05 	vcmpnltps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 05 	vcmpnltps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 05 	vcmpnltps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 05 	vcmpnltps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 05 	vcmpnltps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 05 	vcmpnltps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 05 	vcmpnltps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 05 	vcmpnltps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 05 	vcmpnltps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 05 	vcmpnltps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 05 	vcmpnltps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 05 	vcmpnltps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 05 	vcmpnltps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 05 	vcmpnltps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 05 	vcmpnltps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 05 	vcmpnltps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 05 	vcmpnltps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 05 	vcmpnltps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 05 	vcmpnltps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 07 	vcmpordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 07 	vcmpordps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 07 	vcmpordps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 07 	vcmpordps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 07 	vcmpordps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 07 	vcmpordps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 07 	vcmpordps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 07 	vcmpordps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 07 	vcmpordps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 07 	vcmpordps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 07 	vcmpordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 07 	vcmpordps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 07 	vcmpordps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 07 	vcmpordps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 07 	vcmpordps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 07 	vcmpordps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 07 	vcmpordps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 07 	vcmpordps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 07 	vcmpordps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 07 	vcmpordps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 03 	vcmpunordps %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 03 	vcmpunordps %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 03 	vcmpunordps %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 03 	vcmpunordps %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 03 	vcmpunordps %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 03 	vcmpunordps %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 03 	vcmpunordps %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 03 	vcmpunordps %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 03 	vcmpunordps \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 03 	vcmpunordps \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 03 	vcmpunordps \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 03 	vcmpunordps \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 03 	vcmpunordps \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 03 	vcmpunordps \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 03 	vcmpunordps \(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 03 	vcmpunordps \(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 03 	vcmpunordps \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 03 	vcmpunordps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 03 	vcmpunordps 0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 03 	vcmpunordps \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 00 	vcmpeqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 00 	vcmpeqpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 00 	vcmpeqpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 00 	vcmpeqpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 00 	vcmpeqpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 00 	vcmpeqpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 00 	vcmpeqpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 00 	vcmpeqpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 00 	vcmpeqpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 00 	vcmpeqpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 00 	vcmpeqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 00 	vcmpeqpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 00 	vcmpeqpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 00 	vcmpeqpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 00 	vcmpeqpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 02 	vcmplepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 02 	vcmplepd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 02 	vcmplepd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 02 	vcmplepd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 02 	vcmplepd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 02 	vcmplepd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 02 	vcmplepd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 02 	vcmplepd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 02 	vcmplepd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 02 	vcmplepd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 02 	vcmplepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 02 	vcmplepd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 02 	vcmplepd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 02 	vcmplepd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 02 	vcmplepd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 01 	vcmpltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 01 	vcmpltpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 01 	vcmpltpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 01 	vcmpltpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 01 	vcmpltpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 01 	vcmpltpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 01 	vcmpltpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 01 	vcmpltpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 01 	vcmpltpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 01 	vcmpltpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 01 	vcmpltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 01 	vcmpltpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 01 	vcmpltpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 01 	vcmpltpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 01 	vcmpltpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 04 	vcmpneqpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 04 	vcmpneqpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 04 	vcmpneqpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 04 	vcmpneqpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 04 	vcmpneqpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 04 	vcmpneqpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 04 	vcmpneqpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 04 	vcmpneqpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 04 	vcmpneqpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 04 	vcmpneqpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 04 	vcmpneqpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 04 	vcmpneqpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 04 	vcmpneqpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 04 	vcmpneqpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 04 	vcmpneqpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 06 	vcmpnlepd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 06 	vcmpnlepd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 06 	vcmpnlepd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 06 	vcmpnlepd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 06 	vcmpnlepd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 06 	vcmpnlepd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 06 	vcmpnlepd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 06 	vcmpnlepd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 06 	vcmpnlepd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 06 	vcmpnlepd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 06 	vcmpnlepd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 06 	vcmpnlepd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 06 	vcmpnlepd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 06 	vcmpnlepd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 06 	vcmpnlepd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 05 	vcmpnltpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 05 	vcmpnltpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 05 	vcmpnltpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 05 	vcmpnltpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 05 	vcmpnltpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 05 	vcmpnltpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 05 	vcmpnltpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 05 	vcmpnltpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 05 	vcmpnltpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 05 	vcmpnltpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 05 	vcmpnltpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 05 	vcmpnltpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 05 	vcmpnltpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 05 	vcmpnltpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 05 	vcmpnltpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 07 	vcmpordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 07 	vcmpordpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 07 	vcmpordpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 07 	vcmpordpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 07 	vcmpordpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 07 	vcmpordpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 07 	vcmpordpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 07 	vcmpordpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 07 	vcmpordpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 07 	vcmpordpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 07 	vcmpordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 07 	vcmpordpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 07 	vcmpordpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 07 	vcmpordpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 07 	vcmpordpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 03 	vcmpunordpd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 03 	vcmpunordpd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 03 	vcmpunordpd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 03 	vcmpunordpd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 03 	vcmpunordpd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 03 	vcmpunordpd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 03 	vcmpunordpd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 03 	vcmpunordpd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 03 	vcmpunordpd \{sae\},%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 03 	vcmpunordpd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 03 	vcmpunordpd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 03 	vcmpunordpd \(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 03 	vcmpunordpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 03 	vcmpunordpd 0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 03 	vcmpunordpd \(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 cb    	vpcmpeqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 cb    	vpcmpeqd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 cb    	vpcmpeqd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 cb    	vpcmpeqd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 cb    	vpcmpeqd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 cb    	vpcmpeqd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 cb    	vpcmpeqd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 76 cb    	vpcmpeqd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 cb    	vpcmpeqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 76 09    	vpcmpeqd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 76 09    	vpcmpeqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 76 09    	vpcmpeqd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 76 09    	vpcmpeqd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 76 09    	vpcmpeqd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 76 09    	vpcmpeqd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 76 09    	vpcmpeqd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 09    	vpcmpeqd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 49 04 	vpcmpeqd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 76 09    	vpcmpeqd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 02 	vpcmpled %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 02 	vpcmpled %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 02 	vpcmpled %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 02 	vpcmpled %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 02 	vpcmpled %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 02 	vpcmpled %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 02 	vpcmpled %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 02 	vpcmpled %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 02 	vpcmpled %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 02 	vpcmpled \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 02 	vpcmpled \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 02 	vpcmpled \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 02 	vpcmpled \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 02 	vpcmpled \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 02 	vpcmpled \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 02 	vpcmpled \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 02 	vpcmpled \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 02 	vpcmpled 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 02 	vpcmpled \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 cb    	vpcmpltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 cb    	vpcmpltd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 cb    	vpcmpltd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 cb    	vpcmpltd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 cb    	vpcmpltd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 cb    	vpcmpltd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 cb    	vpcmpltd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 74 cb    	vpcmpltd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 cb    	vpcmpltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 74 09    	vpcmpltd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 74 09    	vpcmpltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 74 09    	vpcmpltd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 74 09    	vpcmpltd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 74 09    	vpcmpltd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 74 09    	vpcmpltd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 74 09    	vpcmpltd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 09    	vpcmpltd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 49 04 	vpcmpltd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 74 09    	vpcmpltd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 04 	vpcmpneqd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 04 	vpcmpneqd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 04 	vpcmpneqd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 04 	vpcmpneqd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 04 	vpcmpneqd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 04 	vpcmpneqd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 04 	vpcmpneqd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 04 	vpcmpneqd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 04 	vpcmpneqd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 04 	vpcmpneqd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 04 	vpcmpneqd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 04 	vpcmpneqd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 04 	vpcmpneqd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 04 	vpcmpneqd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 04 	vpcmpneqd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 04 	vpcmpneqd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 04 	vpcmpneqd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 04 	vpcmpneqd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 04 	vpcmpneqd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 cb    	vpcmpgtd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 cb    	vpcmpgtd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 cb    	vpcmpgtd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 cb    	vpcmpgtd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 cb    	vpcmpgtd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 cb    	vpcmpgtd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 cb    	vpcmpgtd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 3a 66 cb    	vpcmpgtd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 cb    	vpcmpgtd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 8a 66 09    	vpcmpgtd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 0a 66 09    	vpcmpgtd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 1a 66 09    	vpcmpgtd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 2a 66 09    	vpcmpgtd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 4a 66 09    	vpcmpgtd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 5a 66 09    	vpcmpgtd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 6a 66 09    	vpcmpgtd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 49 04 	vpcmpgtd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 69 7a 66 09    	vpcmpgtd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 05 	vpcmpnltd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 05 	vpcmpnltd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 05 	vpcmpnltd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 05 	vpcmpnltd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 05 	vpcmpnltd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 05 	vpcmpnltd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 05 	vpcmpnltd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 05 	vpcmpnltd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 05 	vpcmpnltd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 05 	vpcmpnltd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 05 	vpcmpnltd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 05 	vpcmpnltd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 05 	vpcmpnltd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 05 	vpcmpnltd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 05 	vpcmpnltd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 05 	vpcmpnltd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 05 	vpcmpnltd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 05 	vpcmpnltd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 05 	vpcmpnltd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 cb    	vptestmd %zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 cb    	vptestmd %zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 cb    	vptestmd %zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 cb    	vptestmd %zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 cb    	vptestmd %zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 cb    	vptestmd %zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 cb    	vptestmd %zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 3a 27 cb    	vptestmd %zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 cb    	vptestmd %zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 8a 27 09    	vptestmd \(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 0a 27 09    	vptestmd \(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 1a 27 09    	vptestmd \(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 2a 27 09    	vptestmd \(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 4a 27 09    	vptestmd \(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 5a 27 09    	vptestmd \(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 6a 27 09    	vptestmd \(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 09    	vptestmd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 49 04 	vptestmd 0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f2 69 7a 27 09    	vptestmd \(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 cb 64 	vcmpps \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 cb 64 	vcmpps \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 cb 64 	vcmpps \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 cb 64 	vcmpps \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 cb 64 	vcmpps \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 cb 64 	vcmpps \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 cb 64 	vcmpps \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 cb 64 	vcmpps \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 ba c2 cb 64 	vcmpps \{sae\},\$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 8a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 0a c2 09 64 	vcmpps \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 1a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 2a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 4a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 5a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{unorm8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 3a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{float16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 7a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 49 04 64 	vcmpps \$0x64,0x80\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 68 6a c2 09 64 	vcmpps \$0x64,\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 cb 64 	vcmppd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 cb 64 	vcmppd \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 cb 64 	vcmppd \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 4a c2 cb 64 	vcmppd \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 5a c2 cb 64 	vcmppd \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 6a c2 cb 64 	vcmppd \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 7a c2 cb 64 	vcmppd \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 3a c2 cb 64 	vcmppd \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 ba c2 cb 64 	vcmppd \{sae\},\$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 8a c2 09 64 	vcmppd \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 0a c2 09 64 	vcmppd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 1a c2 09 64 	vcmppd \$0x64,\(%rcx\)\{1to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 64 	vcmppd \$0x64,\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 49 04 64 	vcmppd \$0x64,0x80\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 2a c2 09 64 	vcmppd \$0x64,\(%rcx\)\{4to8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f cb 64 	vpcmpd \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f cb 64 	vpcmpd \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f cb 64 	vpcmpd \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f cb 64 	vpcmpd \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f cb 64 	vpcmpd \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f cb 64 	vpcmpd \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f cb 64 	vpcmpd \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1f cb 64 	vpcmpd \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f cb 64 	vpcmpd \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1f 09 64 	vpcmpd \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 49 04 64 	vpcmpd \$0x64,0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1f 09 64 	vpcmpd \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e cb 64 	vpcmpud \$0x64,%zmm3,%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e cb 64 	vpcmpud \$0x64,%zmm3\{cdab\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e cb 64 	vpcmpud \$0x64,%zmm3\{badc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e cb 64 	vpcmpud \$0x64,%zmm3\{aaaa\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e cb 64 	vpcmpud \$0x64,%zmm3\{bbbb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e cb 64 	vpcmpud \$0x64,%zmm3\{cccc\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e cb 64 	vpcmpud \$0x64,%zmm3\{dddd\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 3a 1e cb 64 	vpcmpud \$0x64,%zmm3\{dacb\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e cb 64 	vpcmpud \$0x64,%zmm3\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 8a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{eh\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 0a 1e 09 64 	vpcmpud \$0x64,\(%rcx\),%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 1a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{1to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 2a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{4to16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 4a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{uint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 5a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{sint8\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 6a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{uint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 49 04 64 	vpcmpud \$0x64,0x80\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f3 69 7a 1e 09 64 	vpcmpud \$0x64,\(%rcx\)\{sint16\},%zmm2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db cb    	vpandq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 db cb    	vpandq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db cb    	vpandq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 db cb    	vpandq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 db cb    	vpandq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 db cb    	vpandq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 db cb    	vpandq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 db cb    	vpandq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db cb    	vpandq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 db 09    	vpandq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 db 09    	vpandq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 db 09    	vpandq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 09    	vpandq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 49 04 	vpandq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 db 09    	vpandq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df cb    	vpandnq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 df cb    	vpandnq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df cb    	vpandnq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 df cb    	vpandnq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 df cb    	vpandnq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 df cb    	vpandnq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 df cb    	vpandnq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 df cb    	vpandnq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df cb    	vpandnq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 df 09    	vpandnq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 df 09    	vpandnq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 df 09    	vpandnq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 09    	vpandnq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 49 04 	vpandnq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 df 09    	vpandnq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 cb    	vpblendmq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 cb    	vpblendmq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 cb    	vpblendmq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 49 64 cb    	vpblendmq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 59 64 cb    	vpblendmq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 69 64 cb    	vpblendmq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 79 64 cb    	vpblendmq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 39 64 cb    	vpblendmq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 cb    	vpblendmq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 89 64 09    	vpblendmq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 09 64 09    	vpblendmq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 19 64 09    	vpblendmq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 09    	vpblendmq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 49 04 	vpblendmq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 e9 29 64 09    	vpblendmq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb cb    	vporq  %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb cb    	vporq  %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb cb    	vporq  %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 eb cb    	vporq  %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 eb cb    	vporq  %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 eb cb    	vporq  %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 eb cb    	vporq  %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 eb cb    	vporq  %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb cb    	vporq  %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 eb 09    	vporq  \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 eb 09    	vporq  \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 eb 09    	vporq  \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 09    	vporq  \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 49 04 	vporq  0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 eb 09    	vporq  \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef cb    	vpxorq %zmm3,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef cb    	vpxorq %zmm3\{cdab\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef cb    	vpxorq %zmm3\{badc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 49 ef cb    	vpxorq %zmm3\{aaaa\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 59 ef cb    	vpxorq %zmm3\{bbbb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 69 ef cb    	vpxorq %zmm3\{cccc\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 79 ef cb    	vpxorq %zmm3\{dddd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 39 ef cb    	vpxorq %zmm3\{dacb\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef cb    	vpxorq %zmm3\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 89 ef 09    	vpxorq \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 09 ef 09    	vpxorq \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 19 ef 09    	vpxorq \(%rcx\)\{1to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 09    	vpxorq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 49 04 	vpxorq 0x80\(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 e9 29 ef 09    	vpxorq \(%rcx\)\{4to8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 cb 02 	vcvtinsps2f11 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d0 09 02 	vcvtinsps2f11 \$0x2,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 cb 02 	vcvtinsps2u10 \$0x2,%zmm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 d1 09 02 	vcvtinsps2u10 \$0x2,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 cb    	vcvtdq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 cb    	vcvtdq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 cb    	vcvtdq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 e6 cb    	vcvtdq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 e6 cb    	vcvtdq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 e6 cb    	vcvtdq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 e6 cb    	vcvtdq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 e6 cb    	vcvtdq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 cb    	vcvtdq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 e6 09    	vcvtdq2pd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 09    	vcvtdq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 e6 49 04 	vcvtdq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 09    	vcvtdq2pd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 e6 49 20 	vcvtdq2pd 0x80\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 09    	vcvtdq2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 49 08 	vcvtdq2pd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 e6 09    	vcvtdq2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a cb    	vcvtps2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a cb    	vcvtps2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a cb    	vcvtps2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 5a cb    	vcvtps2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 5a cb    	vcvtps2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 5a cb    	vcvtps2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 5a cb    	vcvtps2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 5a cb    	vcvtps2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a cb    	vcvtps2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 5a 09    	vcvtps2pd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 09    	vcvtps2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 5a 49 04 	vcvtps2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 09    	vcvtps2pd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 5a 49 20 	vcvtps2pd 0x80\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 09    	vcvtps2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 49 08 	vcvtps2pd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 5a 09    	vcvtps2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a cb    	vcvtudq2pd %ymm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a cb    	vcvtudq2pd %ymm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a cb    	vcvtudq2pd %ymm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 49 7a cb    	vcvtudq2pd %ymm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 59 7a cb    	vcvtudq2pd %ymm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 69 7a cb    	vcvtudq2pd %ymm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 79 7a cb    	vcvtudq2pd %ymm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 39 7a cb    	vcvtudq2pd %ymm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a cb    	vcvtudq2pd %ymm3\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 89 7a 09    	vcvtudq2pd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 09    	vcvtudq2pd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 09 7a 49 04 	vcvtudq2pd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 09    	vcvtudq2pd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 19 7a 49 20 	vcvtudq2pd 0x80\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 09    	vcvtudq2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 49 08 	vcvtudq2pd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 7a 29 7a 09    	vcvtudq2pd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 cb    	vgetexpps %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 42 cb    	vgetexpps %zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 cb    	vgetexpps %zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 cb    	vgetexpps %zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 cb    	vgetexpps %zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 cb    	vgetexpps %zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 cb    	vgetexpps %zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 cb    	vgetexpps %zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 42 cb    	vgetexpps \{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 42 09    	vgetexpps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 42 09    	vgetexpps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 42 09    	vgetexpps \(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 42 09    	vgetexpps \(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 42 09    	vgetexpps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 42 09    	vgetexpps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 42 09    	vgetexpps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 42 09    	vgetexpps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 09    	vgetexpps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 49 04 	vgetexpps 0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 42 09    	vgetexpps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 cb    	vgetexppd %zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 cb    	vgetexppd %zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 cb    	vgetexppd %zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 49 42 cb    	vgetexppd %zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 59 42 cb    	vgetexppd %zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 69 42 cb    	vgetexppd %zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 79 42 cb    	vgetexppd %zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 39 42 cb    	vgetexppd %zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 42 09    	vgetexppd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 19 42 09    	vgetexppd \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 09    	vgetexppd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 c9 42 cb    	vgetexppd \{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 a9 42 09    	vgetexppd \(%rcx\)\{eh\}\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 49 04 	vgetexppd 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 29 42 09    	vgetexppd \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a ca    	vcvtpd2ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a ca    	vcvtpd2ps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a ca    	vcvtpd2ps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 5a ca    	vcvtpd2ps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 5a ca    	vcvtpd2ps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 5a ca    	vcvtpd2ps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 5a ca    	vcvtpd2ps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 5a ca    	vcvtpd2ps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a ca    	vcvtpd2ps \{rn\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 99 5a ca    	vcvtpd2ps \{rd\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 a9 5a ca    	vcvtpd2ps \{ru\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 b9 5a ca    	vcvtpd2ps \{rz\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps \{rn\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps \{rn\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps \{rd\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps \{ru\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps \{rz\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 c9 5a ca    	vcvtpd2ps \{rn\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 d9 5a ca    	vcvtpd2ps \{rd\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 e9 5a ca    	vcvtpd2ps \{ru\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 f9 5a ca    	vcvtpd2ps \{rz\}\{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 5a 09    	vcvtpd2ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 5a 09    	vcvtpd2ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 5a 09    	vcvtpd2ps \(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 09    	vcvtpd2ps \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 49 04 	vcvtpd2ps 0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 5a 09    	vcvtpd2ps \(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 cb cb 07 	vcvtfxpntps2dq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 cb cb 07 	vcvtfxpntps2dq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 49 04 07 	vcvtfxpntps2dq \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 cb 09 07 	vcvtfxpntps2dq \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 ca cb 07 	vcvtfxpntps2udq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 ca cb 07 	vcvtfxpntps2udq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 49 04 07 	vcvtfxpntps2udq \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 ca 09 07 	vcvtfxpntps2udq \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 cb 07 	vrndfxpntps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 52 cb 07 	vrndfxpntps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 52 cb 07 	vrndfxpntps \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 52 09 07 	vrndfxpntps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 52 49 04 07 	vrndfxpntps \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 52 09 07 	vrndfxpntps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 cb 07 	vgetmantps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 26 cb 07 	vgetmantps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 26 cb 07 	vgetmantps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 26 cb 07 	vgetmantps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 26 cb 07 	vgetmantps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 26 cb 07 	vgetmantps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 26 cb 07 	vgetmantps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 26 cb 07 	vgetmantps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 c9 26 cb 07 	vgetmantps \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 26 09 07 	vgetmantps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 19 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 29 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 49 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 59 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 39 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 79 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 26 49 04 07 	vgetmantps \$0x7,0x80\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 69 26 09 07 	vgetmantps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 49 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 59 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 69 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 79 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 39 e6 cb 07 	vcvtfxpntpd2dq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb c9 e6 cb 07 	vcvtfxpntpd2dq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 49 04 07 	vcvtfxpntpd2dq \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 e6 09 07 	vcvtfxpntpd2dq \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 49 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 59 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 69 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 79 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 39 ca cb 07 	vcvtfxpntpd2udq \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb c9 ca cb 07 	vcvtfxpntpd2udq \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 89 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 09 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 19 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 49 04 07 	vcvtfxpntpd2udq \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 fb 29 ca 09 07 	vcvtfxpntpd2udq \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 cb 07 	vrndfxpntpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 49 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 59 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 69 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 79 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 39 52 cb 07 	vrndfxpntpd \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 c9 52 cb 07 	vrndfxpntpd \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 49 04 07 	vrndfxpntpd \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 52 09 07 	vrndfxpntpd \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 cb 07 	vgetmantpd \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 cb 07 	vgetmantpd \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 cb 07 	vgetmantpd \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 49 26 cb 07 	vgetmantpd \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 59 26 cb 07 	vgetmantpd \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 69 26 cb 07 	vgetmantpd \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 79 26 cb 07 	vgetmantpd \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 39 26 cb 07 	vgetmantpd \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 c9 26 cb 07 	vgetmantpd \{sae\},\$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 89 26 09 07 	vgetmantpd \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 09 26 09 07 	vgetmantpd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 19 26 09 07 	vgetmantpd \$0x7,\(%rcx\)\{1to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 09 07 	vgetmantpd \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 49 04 07 	vgetmantpd \$0x7,0x80\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 f9 29 26 09 07 	vgetmantpd \$0x7,\(%rcx\)\{4to8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 f3 07 	vpslld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 f3 07 	vpslld \$0x7,%zmm3\{cdab\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 f3 07 	vpslld \$0x7,%zmm3\{badc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 f3 07 	vpslld \$0x7,%zmm3\{aaaa\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 f3 07 	vpslld \$0x7,%zmm3\{bbbb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 f3 07 	vpslld \$0x7,%zmm3\{cccc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 f3 07 	vpslld \$0x7,%zmm3\{dddd\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 39 72 f3 07 	vpslld \$0x7,%zmm3\{dacb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 f3 07 	vpslld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 31 07 	vpslld \$0x7,\(%rcx\)\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 31 07 	vpslld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 31 07 	vpslld \$0x7,\(%rcx\)\{1to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 31 07 	vpslld \$0x7,\(%rcx\)\{4to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 31 07 	vpslld \$0x7,\(%rcx\)\{uint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 31 07 	vpslld \$0x7,\(%rcx\)\{sint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 31 07 	vpslld \$0x7,\(%rcx\)\{uint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 31 07 	vpslld \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 71 04 07 	vpslld \$0x7,0x80\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 31 07 	vpslld \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 e3 07 	vpsrad \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 e3 07 	vpsrad \$0x7,%zmm3\{cdab\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 e3 07 	vpsrad \$0x7,%zmm3\{badc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 e3 07 	vpsrad \$0x7,%zmm3\{aaaa\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 e3 07 	vpsrad \$0x7,%zmm3\{bbbb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 e3 07 	vpsrad \$0x7,%zmm3\{cccc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 e3 07 	vpsrad \$0x7,%zmm3\{dddd\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 39 72 e3 07 	vpsrad \$0x7,%zmm3\{dacb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 e3 07 	vpsrad \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 21 07 	vpsrad \$0x7,\(%rcx\)\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 21 07 	vpsrad \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 21 07 	vpsrad \$0x7,\(%rcx\)\{1to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 21 07 	vpsrad \$0x7,\(%rcx\)\{4to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 21 07 	vpsrad \$0x7,\(%rcx\)\{uint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 21 07 	vpsrad \$0x7,\(%rcx\)\{sint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 21 07 	vpsrad \$0x7,\(%rcx\)\{uint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 21 07 	vpsrad \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 61 04 07 	vpsrad \$0x7,0x80\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 21 07 	vpsrad \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 d3 07 	vpsrld \$0x7,%zmm3,%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 d3 07 	vpsrld \$0x7,%zmm3\{cdab\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 d3 07 	vpsrld \$0x7,%zmm3\{badc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 d3 07 	vpsrld \$0x7,%zmm3\{aaaa\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 d3 07 	vpsrld \$0x7,%zmm3\{bbbb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 d3 07 	vpsrld \$0x7,%zmm3\{cccc\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 d3 07 	vpsrld \$0x7,%zmm3\{dddd\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 39 72 d3 07 	vpsrld \$0x7,%zmm3\{dacb\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 d3 07 	vpsrld \$0x7,%zmm3\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 89 72 11 07 	vpsrld \$0x7,\(%rcx\)\{eh\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 09 72 11 07 	vpsrld \$0x7,\(%rcx\),%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 19 72 11 07 	vpsrld \$0x7,\(%rcx\)\{1to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 29 72 11 07 	vpsrld \$0x7,\(%rcx\)\{4to16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 49 72 11 07 	vpsrld \$0x7,\(%rcx\)\{uint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 59 72 11 07 	vpsrld \$0x7,\(%rcx\)\{sint8\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 69 72 11 07 	vpsrld \$0x7,\(%rcx\)\{uint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 11 07 	vpsrld \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 51 04 07 	vpsrld \$0x7,0x80\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f1 71 79 72 11 07 	vpsrld \$0x7,\(%rcx\)\{sint16\},%zmm1
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 39 cb cb 07 	vcvtfxpntdq2ps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 99 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rd\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 a9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{ru\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 b9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rz\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 cb cb 07 	vcvtfxpntdq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 49 04 07 	vcvtfxpntdq2ps \$0x7,0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 cb 09 07 	vcvtfxpntdq2ps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 39 ca cb 07 	vcvtfxpntudq2ps \$0x7,%zmm3\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 99 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rd\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 a9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{ru\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 b9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rz\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 c9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rn\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 d9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rd\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 e9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{ru\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 f9 ca cb 07 	vcvtfxpntudq2ps \$0x7,\{rz\}\{sae\},%zmm3,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 89 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 09 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 19 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{1to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 29 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{4to16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 49 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 59 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 69 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 49 04 07 	vcvtfxpntudq2ps \$0x7,0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 78 79 ca 09 07 	vcvtfxpntudq2ps \$0x7,\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d5 09    	vloadunpackhps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d5 09    	vloadunpackhps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d5 09    	vloadunpackhps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d5 09    	vloadunpackhps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d5 09    	vloadunpackhps \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d5 09    	vloadunpackhps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 09    	vloadunpackhps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 49 40 	vloadunpackhps 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d5 09    	vloadunpackhps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d1 09    	vloadunpacklps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d1 09    	vloadunpacklps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d1 09    	vloadunpacklps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d1 09    	vloadunpacklps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d1 09    	vloadunpacklps \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d1 09    	vloadunpacklps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 09    	vloadunpacklps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 49 40 	vloadunpacklps 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d1 09    	vloadunpacklps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 09    	vmovaps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 09    	vmovaps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 28 09    	vmovaps \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 09    	vmovaps \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 09    	vmovaps \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 09    	vmovaps \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 09    	vmovaps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 49 04 	vmovaps 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 09    	vmovaps \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 1a 09    	vbroadcastf32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 1a 09    	vbroadcastf32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 1a 09    	vbroadcastf32x4 \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 1a 09    	vbroadcastf32x4 \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 1a 09    	vbroadcastf32x4 \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 1a 09    	vbroadcastf32x4 \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 09    	vbroadcastf32x4 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 49 10 	vbroadcastf32x4 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 1a 09    	vbroadcastf32x4 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 18 09    	vbroadcastss \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 18 09    	vbroadcastss \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 18 09    	vbroadcastss \(%rcx\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 18 09    	vbroadcastss \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 18 09    	vbroadcastss \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 18 09    	vbroadcastss \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 09    	vbroadcastss \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 49 40 	vbroadcastss 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 18 09    	vbroadcastss \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 89 67 09    	vbroadcastaddss \(%rcx\)\{eh\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 09 67 09    	vbroadcastaddss \(%rcx\),%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 39 67 09    	vbroadcastaddss \(%rcx\)\{float16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 49 67 09    	vbroadcastaddss \(%rcx\)\{uint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 59 67 09    	vbroadcastaddss \(%rcx\)\{sint8\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 69 67 09    	vbroadcastaddss \(%rcx\)\{uint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 09    	vbroadcastaddss \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 49 40 	vbroadcastaddss 0x80\(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 69 79 67 09    	vbroadcastaddss \(%rcx\)\{sint16\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d6 09    	vloadunpackhgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d6 09    	vloadunpackhgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d6 09    	vloadunpackhgps \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d6 09    	vloadunpackhgps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d6 09    	vloadunpackhgps \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d6 09    	vloadunpackhgps \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 09    	vloadunpackhgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 49 40 	vloadunpackhgps 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d6 09    	vloadunpackhgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d2 09    	vloadunpacklgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d2 09    	vloadunpacklgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d2 09    	vloadunpacklgps \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d2 09    	vloadunpacklgps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d2 09    	vloadunpacklgps \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d2 09    	vloadunpacklgps \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 09    	vloadunpacklgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 49 40 	vloadunpacklgps 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d2 09    	vloadunpacklgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 09    	vmovgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 09    	vmovgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 68 09    	vmovgps \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 09    	vmovgps \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 09    	vmovgps \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 09    	vmovgps \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 09    	vmovgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 49 04 	vmovgps 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 09    	vmovgps \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4a 09    	vbroadcastgf32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4a 09    	vbroadcastgf32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 4a 09    	vbroadcastgf32x4 \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4a 09    	vbroadcastgf32x4 \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4a 09    	vbroadcastgf32x4 \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4a 09    	vbroadcastgf32x4 \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 09    	vbroadcastgf32x4 \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 49 10 	vbroadcastgf32x4 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4a 09    	vbroadcastgf32x4 \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 48 09    	vbroadcastgss \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 48 09    	vbroadcastgss \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 48 09    	vbroadcastgss \(%rcx\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 48 09    	vbroadcastgss \(%rcx\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 48 09    	vbroadcastgss \(%rcx\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 48 09    	vbroadcastgss \(%rcx\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 09    	vbroadcastgss \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 49 40 	vbroadcastgss 0x80\(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 48 09    	vbroadcastgss \(%rcx\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d7 09    	vloadunpackhpgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d7 09    	vloadunpackhpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 19 d7 09    	vloadunpackhpgps \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d7 09    	vloadunpackhpgps \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d7 09    	vloadunpackhpgps \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 49 20 	vloadunpackhpgps 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d7 09    	vloadunpackhpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d3 09    	vloadunpacklpgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d3 09    	vloadunpacklpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 19 d3 09    	vloadunpacklpgps \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 29 d3 09    	vloadunpacklpgps \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 39 d3 09    	vloadunpacklpgps \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 49 20 	vloadunpacklpgps 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d3 09    	vloadunpacklpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 09    	vmovpgps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 09    	vmovpgps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 69 09    	vmovpgps \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 09    	vmovpgps \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 09    	vmovpgps \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 09    	vmovpgps \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 09    	vmovpgps \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 09    	vmovpgps \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 09    	vmovpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 49 02 	vmovpgps 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 09    	vmovpgps \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 4b 09    	vbroadcastpgf32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 49 08 	vbroadcastpgf32x4 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 4b 09    	vbroadcastpgf32x4 \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 49 09    	vbroadcastpgss \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 49 09    	vbroadcastpgss \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 49 09    	vbroadcastpgss \(%rcx\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 49 09    	vbroadcastpgss \(%rcx\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 49 09    	vbroadcastpgss \(%rcx\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 49 09    	vbroadcastpgss \(%rcx\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 49 09    	vbroadcastpgss \(%rcx\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 49 09    	vbroadcastpgss \(%rcx\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 09    	vbroadcastpgss \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 49 20 	vbroadcastpgss 0x80\(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 49 09    	vbroadcastpgss \(%rcx\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d5 09    	vloadunpackhpd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 49 10 	vloadunpackhpd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d5 09    	vloadunpackhpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d1 09    	vloadunpacklpd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 49 10 	vloadunpacklpd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d1 09    	vloadunpacklpd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 09    	vmovapd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 49 02 	vmovapd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 09    	vmovapd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 1b 09    	vbroadcastf64x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 49 04 	vbroadcastf64x4 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 1b 09    	vbroadcastf64x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 19 09    	vbroadcastsd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 49 10 	vbroadcastsd 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 19 09    	vbroadcastsd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d4 09    	vloadunpackhd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d4 09    	vloadunpackhd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d4 09    	vloadunpackhd \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d4 09    	vloadunpackhd \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d4 09    	vloadunpackhd \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 09    	vloadunpackhd \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 49 40 	vloadunpackhd 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d4 09    	vloadunpackhd \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 89 d0 09    	vloadunpackld \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 09 d0 09    	vloadunpackld \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 49 d0 09    	vloadunpackld \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 59 d0 09    	vloadunpackld \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 69 d0 09    	vloadunpackld \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 09    	vloadunpackld \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 49 40 	vloadunpackld 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 78 79 d0 09    	vloadunpackld \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f 09    	vmovdqa32 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f 09    	vmovdqa32 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 49 6f 09    	vmovdqa32 \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f 09    	vmovdqa32 \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f 09    	vmovdqa32 \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 09    	vmovdqa32 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 49 04 	vmovdqa32 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f 09    	vmovdqa32 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 5a 09    	vbroadcasti32x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 5a 09    	vbroadcasti32x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 5a 09    	vbroadcasti32x4 \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 5a 09    	vbroadcasti32x4 \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 5a 09    	vbroadcasti32x4 \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 09    	vbroadcasti32x4 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 49 10 	vbroadcasti32x4 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 5a 09    	vbroadcasti32x4 \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 58 09    	vpbroadcastd \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 58 09    	vpbroadcastd \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 58 09    	vpbroadcastd \(%rcx\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 58 09    	vpbroadcastd \(%rcx\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 58 09    	vpbroadcastd \(%rcx\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 09    	vpbroadcastd \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 49 40 	vpbroadcastd 0x80\(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 58 09    	vpbroadcastd \(%rcx\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d4 09    	vloadunpackhq \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 49 10 	vloadunpackhq 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d4 09    	vloadunpackhq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 89 d0 09    	vloadunpacklq \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 49 10 	vloadunpacklq 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f8 09 d0 09    	vloadunpacklq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f 09    	vmovdqa64 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 49 02 	vmovdqa64 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f 09    	vmovdqa64 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 5b 09    	vbroadcasti64x4 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 49 04 	vbroadcasti64x4 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 5b 09    	vbroadcasti64x4 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 59 09    	vpbroadcastq \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 49 10 	vpbroadcastq 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 09 59 09    	vpbroadcastq \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 71 09 03 ca 08 	valignd \$0x8,%zmm2,%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 71 09 03 09 08 	valignd \$0x8,\(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 71 09 03 49 02 08 	valignd \$0x8,0x80\(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 71 09 03 09 08 	valignd \$0x8,\(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b ca    	vcvtps2srgb8 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 6b ca    	vcvtps2srgb8 \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 09    	vcvtps2srgb8 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 6b 09    	vcvtps2srgb8 \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 49 02 	vcvtps2srgb8 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6b 09    	vcvtps2srgb8 \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 ca    	vexp223ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 c8 ca    	vexp223ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 09    	vexp223ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 c8 09    	vexp223ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 49 02 	vexp223ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c8 09    	vexp223ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 ca    	vlog2ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 c9 ca    	vlog2ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 09    	vlog2ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 c9 09    	vlog2ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 49 02 	vlog2ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 c9 09    	vlog2ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca ca    	vrcp23ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 ca ca    	vrcp23ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 09    	vrcp23ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 ca 09    	vrcp23ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 49 02 	vrcp23ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 ca 09    	vrcp23ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb ca    	vrsqrt23ps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 c9 cb ca    	vrsqrt23ps \{sae\},%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 09    	vrsqrt23ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 cb 09    	vrsqrt23ps \(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 49 02 	vrsqrt23ps 0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 cb 09    	vrsqrt23ps \(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 71 09 36 ca    	vpermd %zmm2,%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 71 09 36 09    	vpermd \(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 71 09 36 49 02 	vpermd 0x80\(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 71 09 36 09    	vpermd \(%rcx\),%zmm1,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 07 ca 07 	vpermf32x4 \$0x7,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 89 07 09 07 	vpermf32x4 \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 07 49 02 07 	vpermf32x4 \$0x7,0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f3 79 09 07 09 07 	vpermf32x4 \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 70 ca 07 	vpshufd \$0x7,%zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 70 09 07 	vpshufd \$0x7,\(%rcx\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 70 49 02 07 	vpshufd \$0x7,0x80\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 70 09 07 	vpshufd \$0x7,\(%rcx\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d5 09    	vpackstorehps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d5 09    	vpackstorehps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d5 09    	vpackstorehps %zmm1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d5 09    	vpackstorehps %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d5 09    	vpackstorehps %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d5 09    	vpackstorehps %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d5 09    	vpackstorehps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d5 49 40 	vpackstorehps %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 79 d5 09    	vpackstorehps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d1 09    	vpackstorelps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d1 09    	vpackstorelps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d1 09    	vpackstorelps %zmm1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d1 09    	vpackstorelps %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d1 09    	vpackstorelps %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d1 09    	vpackstorelps %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d1 09    	vpackstorelps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d1 49 40 	vpackstorelps %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 79 d1 09    	vpackstorelps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 29 09    	vmovaps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 29 09    	vmovaps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 29 09    	vmovaps %zmm1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 29 09    	vmovaps %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 29 09    	vmovaps %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 29 09    	vmovaps %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 29 09    	vmovaps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 78 29 49 04 	vmovaps %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 79 29 09    	vmovaps %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d6 09    	vpackstorehgps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d6 09    	vpackstorehgps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d6 09    	vpackstorehgps %zmm1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d6 09    	vpackstorehgps %zmm1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d6 09    	vpackstorehgps %zmm1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d6 09    	vpackstorehgps %zmm1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d6 09    	vpackstorehgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d6 49 40 	vpackstorehgps %zmm1\{snorm16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 79 d6 09    	vpackstorehgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d2 09    	vpackstorelgps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d2 09    	vpackstorelgps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 d2 09    	vpackstorelgps %zmm1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d2 09    	vpackstorelgps %zmm1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d2 09    	vpackstorelgps %zmm1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d2 09    	vpackstorelgps %zmm1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d2 09    	vpackstorelgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d2 49 40 	vpackstorelgps %zmm1\{snorm16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 79 d2 09    	vpackstorelgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 6a 09    	vmovgps %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 6a 09    	vmovgps %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 6a 09    	vmovgps %zmm1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 6a 09    	vmovgps %zmm1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 6a 09    	vmovgps %zmm1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 6a 09    	vmovgps %zmm1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 6a 09    	vmovgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 6a 49 04 	vmovgps %zmm1\{snorm16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 79 6a 09    	vmovgps %zmm1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d5 09    	vpackstorehpd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d5 49 10 	vpackstorehpd %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d5 09    	vpackstorehpd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d1 09    	vpackstorelpd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d1 49 10 	vpackstorelpd %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d1 09    	vpackstorelpd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 29 09    	vmovapd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 08 29 49 02 	vmovapd %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 f9 09 29 09    	vmovapd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d4 09    	vpackstorehd %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d4 09    	vpackstorehd %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d4 09    	vpackstorehd %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d4 09    	vpackstorehd %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d4 09    	vpackstorehd %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d4 09    	vpackstorehd %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d4 49 40 	vpackstorehd %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 79 d4 09    	vpackstorehd %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 d0 09    	vpackstoreld %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 d0 09    	vpackstoreld %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 d0 09    	vpackstoreld %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 d0 09    	vpackstoreld %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 d0 09    	vpackstoreld %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 d0 09    	vpackstoreld %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 78 d0 49 40 	vpackstoreld %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 79 79 d0 09    	vpackstoreld %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 7f 09    	vmovdqa32 %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 7f 09    	vmovdqa32 %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 49 7f 09    	vmovdqa32 %zmm1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 59 7f 09    	vmovdqa32 %zmm1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 69 7f 09    	vmovdqa32 %zmm1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 7f 09    	vmovdqa32 %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 78 7f 49 04 	vmovdqa32 %zmm1\{sint16\},0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 79 79 7f 09    	vmovdqa32 %zmm1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d4 09    	vpackstorehq %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d4 49 10 	vpackstorehq %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d4 09    	vpackstorehq %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 89 d0 09    	vpackstorelq %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 f9 08 d0 49 10 	vpackstorelq %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f2 f9 09 d0 09    	vpackstorelq %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 7f 09    	vmovdqa64 %zmm1,\(%rcx\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 08 7f 49 02 	vmovdqa64 %zmm1,0x80\(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 f9 09 7f 09    	vmovdqa64 %zmm1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 28 ca    	vmovapd %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 28 ca    	vmovapd %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 28 ca    	vmovapd %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 28 ca    	vmovapd %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 28 ca    	vmovapd %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 28 ca    	vmovapd %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 28 ca    	vmovapd %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 28 ca    	vmovapd %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 28 ca    	vmovapd %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 09 28 ca    	vmovaps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 19 28 ca    	vmovaps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 29 28 ca    	vmovaps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 49 28 ca    	vmovaps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 59 28 ca    	vmovaps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 69 28 ca    	vmovaps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 79 28 ca    	vmovaps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 39 28 ca    	vmovaps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 78 89 28 ca    	vmovaps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 09 6f ca    	vmovdqa32 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 19 6f ca    	vmovdqa32 %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 29 6f ca    	vmovdqa32 %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 49 6f ca    	vmovdqa32 %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 59 6f ca    	vmovdqa32 %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 69 6f ca    	vmovdqa32 %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 79 6f ca    	vmovdqa32 %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 39 6f ca    	vmovdqa32 %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 79 89 6f ca    	vmovdqa32 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 09 6f ca    	vmovdqa64 %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 19 6f ca    	vmovdqa64 %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 29 6f ca    	vmovdqa64 %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 49 6f ca    	vmovdqa64 %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 59 6f ca    	vmovdqa64 %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 69 6f ca    	vmovdqa64 %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 79 6f ca    	vmovdqa64 %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 39 6f ca    	vmovdqa64 %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 f9 89 6f ca    	vmovdqa64 %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 68 ca    	vmovgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 68 ca    	vmovgps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 68 ca    	vmovgps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 68 ca    	vmovgps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 68 ca    	vmovgps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 68 ca    	vmovgps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 68 ca    	vmovgps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 68 ca    	vmovgps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 68 ca    	vmovgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 09 69 ca    	vmovpgps %zmm2,%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 19 69 ca    	vmovpgps %zmm2\{cdab\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 29 69 ca    	vmovpgps %zmm2\{badc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 49 69 ca    	vmovpgps %zmm2\{aaaa\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 59 69 ca    	vmovpgps %zmm2\{bbbb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 69 69 ca    	vmovpgps %zmm2\{cccc\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 79 69 ca    	vmovpgps %zmm2\{dddd\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 39 69 ca    	vmovpgps %zmm2\{dacb\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 f2 79 89 69 ca    	vmovpgps %zmm2\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{uint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{sint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{uint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{sint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 4c 52 40 	vpscatterdd %zmm1\{sint16\},0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a0 8c 55 99 00 00 00 	vpscatterdd %zmm1\{sint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 a2 8c 55 99 00 00 00 	vscatterdps %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 a2 8c 55 99 00 00 00 	vscatterdps %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{float16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{uint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{sint8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{uint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{sint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 4c 52 40 	vscatterdps %zmm1\{sint16\},0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 a2 8c 55 99 00 00 00 	vscatterdps %zmm1\{sint16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{float16rz\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{unorm8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{snorm8\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{unorm16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{snorm16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 4c 52 40 	vscatterdgps %zmm1\{snorm16\},0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c0 8c 55 99 00 00 00 	vscatterdgps %zmm1\{snorm16\},0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 4c 52 40 	vgatherpf0dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 8c 55 99 00 00 00 	vgatherpf0dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 44 52 40 	vgatherpf0hintdps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 84 55 99 00 00 00 	vgatherpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 54 52 40 	vgatherpf1dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 94 55 99 00 00 00 	vgatherpf1dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 6c 52 40 	vscatterpf0dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 ac 55 99 00 00 00 	vscatterpf0dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 64 52 40 	vscatterpf0hintdps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 a4 55 99 00 00 00 	vscatterpf0hintdps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 0a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 3a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{float16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 4a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{uint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 5a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{sint8\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 6a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{uint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 7a c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k2\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 74 52 40 	vscatterpf1dps 0x80\(%rdx,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 c6 b4 55 99 00 00 00 	vscatterpf1dps 0x99\(%rbp,%zmm10,2\)\{sint16\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 84 55 99 00 00 00 	vgatherpf0hintdpd 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 44 52 10 	vgatherpf0hintdpd 0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 84 55 99 00 00 00 	vgatherpf0hintdpd 0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 c6 a4 55 99 00 00 00 	vscatterpf0hintdpd 0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 64 52 10 	vscatterpf0hintdpd 0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 c6 a4 55 99 00 00 00 	vscatterpf0hintdpd 0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{float16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 4c 52 20 	vgatherdps 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 92 8c 55 99 00 00 00 	vgatherdps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 92 8c 55 99 00 00 00 	vgatherdpd 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 4c 52 10 	vgatherdpd 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 92 8c 55 99 00 00 00 	vgatherdpd 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 29 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{srgb8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{unorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{snorm8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{unorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\)\{snorm16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 4c 52 20 	vgatherdgps 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b0 8c 55 99 00 00 00 	vgatherdgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 19 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{float11A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 29 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{float11B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 39 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{float10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm10A\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm10B\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm10C\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\)\{unorm2D\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 4c 52 20 	vgatherdpgps 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 b2 8c 55 99 00 00 00 	vgatherdpgps 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 89 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 49 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{uint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 59 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{sint8\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 69 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{uint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 79 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\)\{sint16\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 4c 52 20 	vpgatherdd 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 79 09 90 8c 55 99 00 00 00 	vpgatherdd 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 90 8c 55 99 00 00 00 	vpgatherdq 0x99\(%rbp,%zmm10,2\)\{eh\},%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 4c 52 10 	vpgatherdq 0x80\(%rdx,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 90 8c 55 99 00 00 00 	vpgatherdq 0x99\(%rbp,%zmm10,2\),%zmm1\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 89 a0 8c 55 99 00 00 00 	vpscatterdq %zmm1,0x99\(%rbp,%zmm10,2\)\{eh\}\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 4c 52 10 	vpscatterdq %zmm1,0x80\(%rdx,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	62 b2 f9 09 a0 8c 55 99 00 00 00 	vpscatterdq %zmm1,0x99\(%rbp,%zmm10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	c4 e2 7b f0 d8       	bitinterleave11 %eax,%ebx
[ 	]*[a-f0-9]+:	c4 42 7b f0 d1       	bitinterleave11 %r9d,%r10d
[ 	]*[a-f0-9]+:	c4 e2 7a f0 d8       	bitinterleave21 %eax,%ebx
[ 	]*[a-f0-9]+:	c4 42 7a f0 d1       	bitinterleave21 %r9d,%r10d
[ 	]*[a-f0-9]+:	c5 fa bd d8          	lzcnt  %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7a bd d1       	lzcnt  %r9d,%r10d
[ 	]*[a-f0-9]+:	c4 e2 7b f1 d8       	quadmask %eax,%ebx
[ 	]*[a-f0-9]+:	c4 42 7b f1 d1       	quadmask %r9d,%r10d
[ 	]*[a-f0-9]+:	c5 fa b8 d8          	popcnt %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7a b8 d1       	popcnt %r9d,%r10d
[ 	]*[a-f0-9]+:	c5 fa bc d8          	tzcnt  %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7a bc d1       	tzcnt  %r9d,%r10d
[ 	]*[a-f0-9]+:	c5 fb bc d8          	tzcnti %eax,%ebx
[ 	]*[a-f0-9]+:	c4 41 7b bc d1       	tzcnti %r9d,%r10d
[ 	]*[a-f0-9]+:	c4 e2 fb f0 d8       	bitinterleave11 %rax,%rbx
[ 	]*[a-f0-9]+:	c4 42 fb f0 f9       	bitinterleave11 %r9,%r15
[ 	]*[a-f0-9]+:	c4 e2 fa f0 d8       	bitinterleave21 %rax,%rbx
[ 	]*[a-f0-9]+:	c4 42 fa f0 f9       	bitinterleave21 %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fa bd d8       	lzcnt  %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fa bd f9       	lzcnt  %r9,%r15
[ 	]*[a-f0-9]+:	c4 e2 fb f1 d8       	quadmask %rax,%rbx
[ 	]*[a-f0-9]+:	c4 42 fb f1 f9       	quadmask %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fa b8 d8       	popcnt %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fa b8 f9       	popcnt %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fa bc d8       	tzcnt  %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fa bc f9       	tzcnt  %r9,%r15
[ 	]*[a-f0-9]+:	c4 e1 fb bc d8       	tzcnti %rax,%rbx
[ 	]*[a-f0-9]+:	c4 41 fb bc f9       	tzcnti %r9,%r15
[ 	]*[a-f0-9]+:	c5 fb ae 39          	clevict0 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 7b 08 ae 79 02 	clevict0 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 fb ae 39          	clevict0 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 fa ae 39          	clevict1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 7a 08 ae 79 02 	clevict1 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 fa ae 39          	clevict1 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 09          	vprefetch0 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 49 02 	vprefetch0 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 09          	vprefetch0 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 11          	vprefetch1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 51 02 	vprefetch1 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 11          	vprefetch1 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 19          	vprefetch2 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 59 02 	vprefetch2 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 19          	vprefetch2 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 29          	vprefetche0 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 69 02 	vprefetche0 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 29          	vprefetche0 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 31          	vprefetche1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 71 02 	vprefetche1 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 31          	vprefetche1 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 39          	vprefetche2 \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 79 02 	vprefetche2 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 39          	vprefetche2 \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 21          	vprefetchenta \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 61 02 	vprefetchenta 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 21          	vprefetchenta \(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 01          	vprefetchnta \(%rcx\)
[ 	]*[a-f0-9]+:	62 f1 78 08 18 41 02 	vprefetchnta 0x80\(%rcx\)
[ 	]*[a-f0-9]+:	c5 f8 18 01          	vprefetchnta \(%rcx\)
[ 	]*[a-f0-9]+:	c5 fa ae f0          	delay  %eax
[ 	]*[a-f0-9]+:	c4 c1 7a ae f1       	delay  %r9d
[ 	]*[a-f0-9]+:	c5 fb ae f0          	spflt  %eax
[ 	]*[a-f0-9]+:	c4 c1 7b ae f1       	spflt  %r9d
[ 	]*[a-f0-9]+:	c4 e1 fa ae f0       	delay  %rax
[ 	]*[a-f0-9]+:	c4 c1 fa ae f1       	delay  %r9
[ 	]*[a-f0-9]+:	c4 e1 fb ae f0       	spflt  %rax
[ 	]*[a-f0-9]+:	c4 c1 fb ae f1       	spflt  %r9
[ 	]*[a-f0-9]+:	62 81 59 11 db e6    	vpandd %zmm30\{cdab\},%zmm20,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 01 59 11 db f6    	vpandd %zmm30\{cdab\},%zmm20,%zmm30\{%k1\}
[ 	]*[a-f0-9]+:	62 01 09 11 db f6    	vpandd %zmm30\{cdab\},%zmm30,%zmm30\{%k1\}
[ 	]*[a-f0-9]+:	62 81 09 11 db e6    	vpandd %zmm30\{cdab\},%zmm30,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 81 29 19 db e6    	vpandd %zmm30\{cdab\},%zmm10,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 f1 59 01 72 31 07 	vpslld \$0x7,\(%rcx\),%zmm20
[ 	]*[a-f0-9]+:	62 91 59 01 72 f6 07 	vpslld \$0x7,%zmm30,%zmm20
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 78 56 34 12 	vpadcd 0x12345678,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 78 56 34 12 	vpadcd 0x12345678,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 55 00 	vpadcd 0x0\(%rbp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 65 00 	vpadcd 0x0\(%rbp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 95 99 00 00 00 	vpadcd 0x99\(%rbp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a5 99 00 00 00 	vpadcd 0x99\(%rbp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 52 69 09 5c 97 99 00 00 00 	vpadcd 0x99\(%r15\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a7 99 00 00 00 	vpadcd 0x99\(%r15\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 15 99 00 00 00 	vpadcd 0x99\(%rip\),%k2,%zmm10\{%k1\}        # ca50 <_start\+0xca50>
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 25 99 00 00 00 	vpadcd 0x99\(%rip\),%k2,%zmm20\{%k1\}        # ca5a <_start\+0xca5a>
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 24 99 00 00 00 	vpadcd 0x99\(%rsp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 24 99 00 00 00 	vpadcd 0x99\(%rsp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 52 69 09 5c 94 24 99 00 00 00 	vpadcd 0x99\(%r12\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 c2 69 09 5c a4 24 99 00 00 00 	vpadcd 0x99\(%r12\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 67 ff ff ff 	vpadcd 0xffffffffffffff67,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 67 ff ff ff 	vpadcd 0xffffffffffffff67,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 65 67 ff ff ff 	vpadcd -0x99\(,%riz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 65 67 ff ff ff 	vpadcd -0x99\(,%riz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 23 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 23 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 94 63 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c a4 63 67 ff ff ff 	vpadcd -0x99\(%rbx,%riz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 bc 67 ff ff ff 	vpadcd -0x99\(%r12,%r15,4\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 bc 67 ff ff ff 	vpadcd -0x99\(%r12,%r15,4\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 69 09 5c 94 f8 67 ff ff ff 	vpadcd -0x99\(%r8,%r15,8\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 69 09 5c a4 f8 67 ff ff ff 	vpadcd -0x99\(%r8,%r15,8\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 a5 67 ff ff ff 	vpadcd -0x99\(%rbp,%r12,4\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 a5 67 ff ff ff 	vpadcd -0x99\(%rbp,%r12,4\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 69 09 5c 94 2c 67 ff ff ff 	vpadcd -0x99\(%rsp,%r13,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 69 09 5c a4 2c 67 ff ff ff 	vpadcd -0x99\(%rsp,%r13,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 99 00 00 00 	vgatherdps 0x99\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 99 00 00 00 	vgatherdps 0x99\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 08 	vgatherdps \(%rax,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 08 	vgatherdps \(%rax,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 10 	vgatherdps \(%rax,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 10 	vgatherdps \(%rax,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 94 48 67 ff ff ff 	vgatherdps -0x99\(%rax,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 a4 48 67 ff ff ff 	vgatherdps -0x99\(%rax,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 94 50 f7 ff ff ff 	vgatherdps -0x9\(%rax,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 a4 50 f7 ff ff ff 	vgatherdps -0x9\(%rax,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 52 79 09 92 94 8c 67 ff ff ff 	vgatherdps -0x99\(%r12,%zmm1,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 c2 79 09 92 a4 8c 67 ff ff ff 	vgatherdps -0x99\(%r12,%zmm1,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 79 09 92 94 94 09 00 00 00 	vgatherdps 0x9\(%r12,%zmm10,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 79 09 92 a4 94 09 00 00 00 	vgatherdps 0x9\(%r12,%zmm10,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 12 79 01 92 94 b5 99 00 00 00 	vgatherdps 0x99\(%r13,%zmm30,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 82 79 01 92 a4 b5 99 00 00 00 	vgatherdps 0x99\(%r13,%zmm30,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 69 09 5c 14 25 34 12 00 00 	vpadcd 0x1234,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 69 09 5c 24 25 34 12 00 00 	vpadcd 0x1234,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 55 00 	vpadcd 0x0\(%ebp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 65 00 	vpadcd 0x0\(%ebp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 95 99 00 00 00 	vpadcd 0x99\(%ebp\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a5 99 00 00 00 	vpadcd 0x99\(%ebp\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 25 99 00 00 00 	addr32 vpadcd 0x99,%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 25 99 00 00 00 	addr32 vpadcd 0x99,%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 14 65 99 00 00 00 	addr32 vpadcd 0x99\(,%eiz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c 24 65 99 00 00 00 	addr32 vpadcd 0x99\(,%eiz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 20 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 20 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 60 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,2\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 60 99 00 00 00 	vpadcd 0x99\(%eax,%eiz,2\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 98 99 00 00 00 	vpadcd 0x99\(%eax,%ebx,4\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 98 99 00 00 00 	vpadcd 0x99\(%eax,%ebx,4\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 cc 99 00 00 00 	vpadcd 0x99\(%esp,%ecx,8\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 cc 99 00 00 00 	vpadcd 0x99\(%esp,%ecx,8\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 69 09 5c 94 15 99 00 00 00 	vpadcd 0x99\(%ebp,%edx,1\),%k2,%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 69 09 5c a4 15 99 00 00 00 	vpadcd 0x99\(%ebp,%edx,1\),%k2,%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 00 00 00 00 	vgatherdps 0x0\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 00 00 00 00 	vgatherdps 0x0\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 00 00 00 00 	vgatherdps 0x0\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 0d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 15 f7 ff ff ff 	vgatherdps -0x9\(,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 72 79 09 92 14 4d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 e2 79 09 92 24 4d 67 ff ff ff 	vgatherdps -0x99\(,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	62 32 79 09 92 14 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	62 a2 79 09 92 24 55 09 00 00 00 	vgatherdps 0x9\(,%zmm10,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 14 08 	vgatherdps \(%eax,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 24 08 	vgatherdps \(%eax,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 32 79 09 92 14 10 	vgatherdps \(%eax,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 a2 79 09 92 24 10 	vgatherdps \(%eax,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 08 67 ff ff ff 	vgatherdps -0x99\(%eax,%zmm1,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 08 67 ff ff ff 	vgatherdps -0x99\(%eax,%zmm1,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 32 79 09 92 94 10 f7 ff ff ff 	vgatherdps -0x9\(%eax,%zmm10,1\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 a2 79 09 92 a4 10 f7 ff ff ff 	vgatherdps -0x9\(%eax,%zmm10,1\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 48 67 ff ff ff 	vgatherdps -0x99\(%eax,%zmm1,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 48 67 ff ff ff 	vgatherdps -0x99\(%eax,%zmm1,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 78 09 00 00 00 	vgatherdps 0x9\(%eax,%zmm7,2\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 78 09 00 00 00 	vgatherdps 0x9\(%eax,%zmm7,2\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 8d 99 00 00 00 	vgatherdps 0x99\(%ebp,%zmm1,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 8d 99 00 00 00 	vgatherdps 0x99\(%ebp,%zmm1,4\),%zmm20\{%k1\}
[ 	]*[a-f0-9]+:	67 62 72 79 09 92 94 bd f7 ff ff ff 	vgatherdps -0x9\(%ebp,%zmm7,4\),%zmm10\{%k1\}
[ 	]*[a-f0-9]+:	67 62 e2 79 09 92 a4 bd f7 ff ff ff 	vgatherdps -0x9\(%ebp,%zmm7,4\),%zmm20\{%k1\}
#pass

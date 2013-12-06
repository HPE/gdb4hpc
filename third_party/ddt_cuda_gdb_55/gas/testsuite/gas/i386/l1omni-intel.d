#as: -march=l1om
#objdump: -dw -Mintel
#name: L1OMni ops (Intel mode)
#source: l1omni.s

.*: +file format .*


Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	62 cd ca                	vkand  k2,k1
[ 	]*[a-f0-9]+:	62 dd ca                	vkandn k2,k1
[ 	]*[a-f0-9]+:	62 ed ca                	vkandnr k2,k1
[ 	]*[a-f0-9]+:	62 fc ca                	vkmov  k2,k1
[ 	]*[a-f0-9]+:	62 fb ca                	vkmovlhb k2,k1
[ 	]*[a-f0-9]+:	62 fd ca                	vknot  k2,k1
[ 	]*[a-f0-9]+:	62 ce ca                	vkor   k2,k1
[ 	]*[a-f0-9]+:	62 de ca                	vkortest k2,k1
[ 	]*[a-f0-9]+:	62 ef ca                	vkswapb k2,k1
[ 	]*[a-f0-9]+:	62 fe ca                	vkxnor k2,k1
[ 	]*[a-f0-9]+:	62 ee ca                	vkxor  k2,k1
[ 	]*[a-f0-9]+:	66 62 ec c2             	vkmov  k2,ax
[ 	]*[a-f0-9]+:	66 62 cc ca             	vkmov  k2,r9w
[ 	]*[a-f0-9]+:	62 ec c2                	vkmov  k2,eax
[ 	]*[a-f0-9]+:	62 cc ca                	vkmov  k2,r9d
[ 	]*[a-f0-9]+:	66 62 dc c8             	vkmov  ax,k1
[ 	]*[a-f0-9]+:	66 62 9c c9             	vkmov  r9w,k1
[ 	]*[a-f0-9]+:	62 dc c8                	vkmov  eax,k1
[ 	]*[a-f0-9]+:	62 9c c9                	vkmov  r9d,k1
[ 	]*[a-f0-9]+:	d6 00 11 45 cb          	vadcpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 45 cb          	vadcpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 45 cb          	vadcpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 45 cb          	vadcpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 45 cb          	vadcpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 45 cb          	vadcpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 45 cb          	vadcpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 45 cb          	vadcpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 55 cb          	vsbbpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 5d cb          	vsbbrpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 48 cb          	vaddnps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 48 cb          	vaddnps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 48 cb          	vaddnps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 48 cb          	vaddnps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 48 cb          	vaddnps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 48 cb          	vaddnps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 48 cb          	vaddnps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 48 cb          	vaddnps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 40 cb          	vaddps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 40 cb          	vaddps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 40 cb          	vaddps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 40 cb          	vaddps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 40 cb          	vaddps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 40 cb          	vaddps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 40 cb          	vaddps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 40 cb          	vaddps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 46 cb          	vaddsetsps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 86 cb          	vclampzps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 86 cb          	vclampzps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 86 cb          	vclampzps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 86 cb          	vclampzps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 86 cb          	vclampzps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 86 cb          	vclampzps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 86 cb          	vclampzps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 86 cb          	vclampzps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 64 cb          	vmadd132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 62 cb          	vmadd213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 60 cb          	vmadd231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 66 cb          	vmadd233ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 9e cb          	vmaxabsps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 88 cb          	vmaxps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 88 cb          	vmaxps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 88 cb          	vmaxps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 88 cb          	vmaxps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 88 cb          	vmaxps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 88 cb          	vmaxps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 88 cb          	vmaxps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 88 cb          	vmaxps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8c cb          	vminps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8c cb          	vminps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8c cb          	vminps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8c cb          	vminps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8c cb          	vminps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8c cb          	vminps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8c cb          	vminps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8c cb          	vminps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 6c cb          	vmsub132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 6a cb          	vmsub213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 68 cb          	vmsub231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 80 cb          	vmulps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 80 cb          	vmulps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 80 cb          	vmulps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 80 cb          	vmulps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 80 cb          	vmulps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 80 cb          	vmulps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 80 cb          	vmulps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 80 cb          	vmulps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 50 cb          	vsubps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 50 cb          	vsubps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 50 cb          	vsubps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 50 cb          	vsubps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 50 cb          	vsubps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 50 cb          	vsubps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 50 cb          	vsubps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 50 cb          	vsubps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 58 cb          	vsubrps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 58 cb          	vsubrps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 58 cb          	vsubrps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 58 cb          	vsubrps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 58 cb          	vsubrps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 58 cb          	vsubrps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 58 cb          	vsubrps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 58 cb          	vsubrps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 cb       	vaddnpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 cb       	vaddpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 40 cb       	vaddpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 cb       	vaddpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 40 cb       	vaddpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 40 cb       	vaddpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 40 cb       	vaddpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 40 cb       	vaddpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 40 cb       	vaddpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 cb       	vmadd132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 cb       	vmadd213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 cb       	vmadd231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 cb       	vmaxpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c cb       	vminpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 8c cb       	vminpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c cb       	vminpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 8c cb       	vminpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 8c cb       	vminpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 8c cb       	vminpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 8c cb       	vminpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 8c cb       	vminpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c cb       	vmsub132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a cb       	vmsub213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 cb       	vmsub231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 cb       	vmulpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 80 cb       	vmulpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 cb       	vmulpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 80 cb       	vmulpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 80 cb       	vmulpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 80 cb       	vmulpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 80 cb       	vmulpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 80 cb       	vmulpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 cb       	vsubpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 50 cb       	vsubpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 cb       	vsubpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 50 cb       	vsubpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 50 cb       	vsubpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 50 cb       	vsubpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 50 cb       	vsubpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 50 cb       	vsubpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 cb       	vsubrpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	d6 00 11 41 cb          	vaddpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 41 cb          	vaddpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 41 cb          	vaddpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 41 cb          	vaddpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 41 cb          	vaddpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 41 cb          	vaddpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 41 cb          	vaddpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 41 cb          	vaddpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 47 cb          	vaddsetspi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 cb          	vandnpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 cb          	vandpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a5 cb          	vandpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 cb          	vandpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a5 cb          	vandpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 cb          	vandpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 cb          	vandpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 cb          	vandpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 cb          	vandpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 87 cb          	vclampzpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 61 cb          	vmadd231pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 67 cb          	vmadd233pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 89 cb          	vmaxpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8a cb          	vmaxpu v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8d cb          	vminpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8d cb          	vminpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8d cb          	vminpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8d cb          	vminpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8d cb          	vminpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8d cb          	vminpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8d cb          	vminpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8d cb          	vminpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8e cb          	vminpu v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8e cb          	vminpu v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8e cb          	vminpu v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8e cb          	vminpu v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8e cb          	vminpu v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8e cb          	vminpu v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8e cb          	vminpu v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8e cb          	vminpu v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 81 cb          	vmulhpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 82 cb          	vmulhpu v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 83 cb          	vmullpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 83 cb          	vmullpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 83 cb          	vmullpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 83 cb          	vmullpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 83 cb          	vmullpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 83 cb          	vmullpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 83 cb          	vmullpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 83 cb          	vmullpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 cb          	vorpi  v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 cb          	vscaleps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 cb          	vsllpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 cb          	vsrapi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 cb          	vsrlpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 51 cb          	vsubpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 51 cb          	vsubpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 51 cb          	vsubpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 51 cb          	vsubpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 51 cb          	vsubpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 51 cb          	vsubpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 51 cb          	vsubpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 51 cb          	vsubpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 59 cb          	vsubrpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 cb          	vxorpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 12 30 cb          	vtestpi k1\{k2\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 12 30 cb          	vtestpi k1\{k2\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 12 30 cb          	vtestpi k1\{k2\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 12 30 cb          	vtestpi k1\{k2\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 12 30 cb          	vtestpi k1\{k2\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 12 30 cb          	vtestpi k1\{k2\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 12 30 cb          	vtestpi k1\{k2\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 12 30 cb          	vtestpi k1\{k2\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 cb       	vandnpq v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 cb       	vandpq v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 cb       	vandpq v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 cb       	vandpq v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a5 cb       	vandpq v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a5 cb       	vandpq v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a5 cb       	vandpq v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a5 cb       	vandpq v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a5 cb       	vandpq v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 cb       	vorpq  v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 cb       	vxorpq v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 cb       	vcmppd k1\{k2\},v2,v3,unord
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{cdab\},unord
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{badc\},unord
[ 	]*[a-f0-9]+:	66 d6 3c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{aaaa\},unord
[ 	]*[a-f0-9]+:	66 d6 4c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{bbbb\},unord
[ 	]*[a-f0-9]+:	66 d6 5c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{cccc\},unord
[ 	]*[a-f0-9]+:	66 d6 6c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dddd\},unord
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	66 d6 70 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	66 d6 80 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	66 d6 74 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	66 d6 84 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	66 d6 78 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	66 d6 88 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	66 d6 8c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},unord
[ 	]*[a-f0-9]+:	66 d6 70 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	66 d6 80 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	66 d6 74 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	66 d6 84 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	66 d6 78 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	66 d6 88 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	66 d6 7c 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},ord
[ 	]*[a-f0-9]+:	66 d6 8c 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},ord
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to8\},unord
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to8\},unord
[ 	]*[a-f0-9]+:	d6 04 12 21 cb          	vcmppi k1\{k2\},v2,v3,lt
[ 	]*[a-f0-9]+:	d6 14 12 21 cb          	vcmppi k1\{k2\},v2,v3\{cdab\},lt
[ 	]*[a-f0-9]+:	d6 24 12 21 cb          	vcmppi k1\{k2\},v2,v3\{badc\},lt
[ 	]*[a-f0-9]+:	d6 34 12 21 cb          	vcmppi k1\{k2\},v2,v3\{aaaa\},lt
[ 	]*[a-f0-9]+:	d6 44 12 21 cb          	vcmppi k1\{k2\},v2,v3\{bbbb\},lt
[ 	]*[a-f0-9]+:	d6 54 12 21 cb          	vcmppi k1\{k2\},v2,v3\{cccc\},lt
[ 	]*[a-f0-9]+:	d6 64 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dddd\},lt
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 70 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	d6 80 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 84 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	d6 78 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	d6 88 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	d6 70 12 25 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	d6 80 12 25 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	d6 74 12 25 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	d6 84 12 25 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	d6 78 12 25 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	d6 88 12 25 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 14 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\},lt
[ 	]*[a-f0-9]+:	d6 24 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\},lt
[ 	]*[a-f0-9]+:	d6 44 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\},lt
[ 	]*[a-f0-9]+:	d6 54 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint8\},lt
[ 	]*[a-f0-9]+:	d6 64 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint16\},lt
[ 	]*[a-f0-9]+:	d6 74 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},lt
[ 	]*[a-f0-9]+:	d6 04 12 22 cb          	vcmppu k1\{k2\},v2,v3,lt
[ 	]*[a-f0-9]+:	d6 14 12 22 cb          	vcmppu k1\{k2\},v2,v3\{cdab\},lt
[ 	]*[a-f0-9]+:	d6 24 12 22 cb          	vcmppu k1\{k2\},v2,v3\{badc\},lt
[ 	]*[a-f0-9]+:	d6 34 12 22 cb          	vcmppu k1\{k2\},v2,v3\{aaaa\},lt
[ 	]*[a-f0-9]+:	d6 44 12 22 cb          	vcmppu k1\{k2\},v2,v3\{bbbb\},lt
[ 	]*[a-f0-9]+:	d6 54 12 22 cb          	vcmppu k1\{k2\},v2,v3\{cccc\},lt
[ 	]*[a-f0-9]+:	d6 64 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dddd\},lt
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 70 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	d6 80 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 84 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	d6 78 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	d6 88 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	d6 70 12 26 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	d6 80 12 26 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	d6 74 12 26 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	d6 84 12 26 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	d6 78 12 26 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	d6 88 12 26 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 14 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\},lt
[ 	]*[a-f0-9]+:	d6 24 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\},lt
[ 	]*[a-f0-9]+:	d6 44 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\},lt
[ 	]*[a-f0-9]+:	d6 54 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint8\},lt
[ 	]*[a-f0-9]+:	d6 64 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint16\},lt
[ 	]*[a-f0-9]+:	d6 74 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},lt
[ 	]*[a-f0-9]+:	d6 0c 12 20 cb          	vcmpps k1\{k2\},v2,v3,unord
[ 	]*[a-f0-9]+:	d6 1c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{cdab\},unord
[ 	]*[a-f0-9]+:	d6 2c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{badc\},unord
[ 	]*[a-f0-9]+:	d6 3c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{aaaa\},unord
[ 	]*[a-f0-9]+:	d6 4c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{bbbb\},unord
[ 	]*[a-f0-9]+:	d6 5c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{cccc\},unord
[ 	]*[a-f0-9]+:	d6 6c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dddd\},unord
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	d6 70 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	d6 80 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 74 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 84 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	d6 78 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	d6 88 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	d6 8c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},unord
[ 	]*[a-f0-9]+:	d6 70 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	d6 80 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	d6 74 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	d6 84 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	d6 78 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	d6 88 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	d6 7c 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},ord
[ 	]*[a-f0-9]+:	d6 8c 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},ord
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	d6 1c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\},unord
[ 	]*[a-f0-9]+:	d6 2c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\},unord
[ 	]*[a-f0-9]+:	d6 4c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\},unord
[ 	]*[a-f0-9]+:	d6 5c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{unorm8\},unord
[ 	]*[a-f0-9]+:	d6 6c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{float16\},unord
[ 	]*[a-f0-9]+:	d6 7c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},unord
[ 	]*[a-f0-9]+:	d6 00 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3,rn,0x2
[ 	]*[a-f0-9]+:	d6 10 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{cdab\},rn,0x2
[ 	]*[a-f0-9]+:	d6 20 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{badc\},rn,0x2
[ 	]*[a-f0-9]+:	d6 30 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{aaaa\},rn,0x2
[ 	]*[a-f0-9]+:	d6 40 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{bbbb\},rn,0x2
[ 	]*[a-f0-9]+:	d6 50 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{cccc\},rn,0x2
[ 	]*[a-f0-9]+:	d6 60 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dddd\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dacb\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dacb\},rn,0x2
[ 	]*[a-f0-9]+:	d6 80 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 d1 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dacb\},rz,0x2
[ 	]*[a-f0-9]+:	d6 80 d1 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x2
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x2
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x2
[ 	]*[a-f0-9]+:	d6 10 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 20 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 40 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,0x2
[ 	]*[a-f0-9]+:	d6 50 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,0x2
[ 	]*[a-f0-9]+:	d6 60 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 00 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3,0x2
[ 	]*[a-f0-9]+:	d6 10 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{cdab\},0x2
[ 	]*[a-f0-9]+:	d6 20 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{badc\},0x2
[ 	]*[a-f0-9]+:	d6 30 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	d6 40 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	d6 50 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{cccc\},0x2
[ 	]*[a-f0-9]+:	d6 60 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{dddd\},0x2
[ 	]*[a-f0-9]+:	d6 70 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{dacb\},0x2
[ 	]*[a-f0-9]+:	d6 80 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x2
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 10 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	d6 20 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	d6 40 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	d6 50 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 60 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	d6 70 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	66 d6 00 09 91 cb       	vcvtpd2pi v1\{k1\},v3,rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{cdab\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{badc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{aaaa\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{bbbb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{cccc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{dddd\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 49 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rd,0x1
[ 	]*[a-f0-9]+:	66 d6 00 49 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rd,0x1
[ 	]*[a-f0-9]+:	66 d6 70 89 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},ru,0x1
[ 	]*[a-f0-9]+:	66 d6 00 89 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],ru,0x1
[ 	]*[a-f0-9]+:	66 d6 70 c9 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 c9 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rz,0x1
[ 	]*[a-f0-9]+:	66 d6 80 c9 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 cb       	vcvtpd2ps v1\{k1\},v3,rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{cdab\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{badc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{aaaa\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{bbbb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{cccc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{dddd\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 49 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rd,0x1
[ 	]*[a-f0-9]+:	66 d6 00 49 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rd,0x1
[ 	]*[a-f0-9]+:	66 d6 70 89 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},ru,0x1
[ 	]*[a-f0-9]+:	66 d6 00 89 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],ru,0x1
[ 	]*[a-f0-9]+:	66 d6 70 c9 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 c9 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rz,0x1
[ 	]*[a-f0-9]+:	66 d6 80 c9 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 cb       	vcvtpd2pu v1\{k1\},v3,rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{cdab\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{badc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{aaaa\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{bbbb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{cccc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{dddd\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 49 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rd,0x1
[ 	]*[a-f0-9]+:	66 d6 00 49 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rd,0x1
[ 	]*[a-f0-9]+:	66 d6 70 89 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},ru,0x1
[ 	]*[a-f0-9]+:	66 d6 00 89 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],ru,0x1
[ 	]*[a-f0-9]+:	66 d6 70 c9 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 c9 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rz,0x1
[ 	]*[a-f0-9]+:	66 d6 80 c9 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 95 cb       	vcvtpi2pd v1\{k1\},v3,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{cdab\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{badc\},0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{aaaa\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{bbbb\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{cccc\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{dddd\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{dacb\},0x1
[ 	]*[a-f0-9]+:	66 d6 80 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 96 cb       	vcvtpu2pd v1\{k1\},v3,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{cdab\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{badc\},0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{aaaa\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{bbbb\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{cccc\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{dddd\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{dacb\},0x1
[ 	]*[a-f0-9]+:	66 d6 80 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	d6 00 21 95 cb          	vcvtpi2ps v1\{k1\},v3,.16b
[ 	]*[a-f0-9]+:	d6 10 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{cdab\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{badc\},.16b
[ 	]*[a-f0-9]+:	d6 30 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{aaaa\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{bbbb\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{cccc\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{dddd\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 01 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.0b
[ 	]*[a-f0-9]+:	d6 70 09 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.4b
[ 	]*[a-f0-9]+:	d6 70 11 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.5b
[ 	]*[a-f0-9]+:	d6 70 19 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.8b
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 29 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.24b
[ 	]*[a-f0-9]+:	d6 70 31 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.31b
[ 	]*[a-f0-9]+:	d6 70 39 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.32b
[ 	]*[a-f0-9]+:	d6 80 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},.16b
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 10 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},.16b
[ 	]*[a-f0-9]+:	d6 00 21 96 cb          	vcvtpu2ps v1\{k1\},v3,.16b
[ 	]*[a-f0-9]+:	d6 10 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{cdab\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{badc\},.16b
[ 	]*[a-f0-9]+:	d6 30 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{aaaa\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{bbbb\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{cccc\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{dddd\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 01 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.0b
[ 	]*[a-f0-9]+:	d6 70 09 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.4b
[ 	]*[a-f0-9]+:	d6 70 11 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.5b
[ 	]*[a-f0-9]+:	d6 70 19 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.8b
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 29 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.24b
[ 	]*[a-f0-9]+:	d6 70 31 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.31b
[ 	]*[a-f0-9]+:	d6 70 39 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.32b
[ 	]*[a-f0-9]+:	d6 80 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},.16b
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 10 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},.16b
[ 	]*[a-f0-9]+:	66 d6 00 09 94 cb       	vcvtps2pd v1\{k1\},v3,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 94 cb       	vcvtps2pd v1\{k1\},v3\{cdab\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 94 cb       	vcvtps2pd v1\{k1\},v3\{badc\},0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 94 cb       	vcvtps2pd v1\{k1\},v3\{aaaa\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 94 cb       	vcvtps2pd v1\{k1\},v3\{bbbb\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 94 cb       	vcvtps2pd v1\{k1\},v3\{cccc\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 94 cb       	vcvtps2pd v1\{k1\},v3\{dddd\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 94 cb       	vcvtps2pd v1\{k1\},v3\{dacb\},0x1
[ 	]*[a-f0-9]+:	66 d6 80 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	d6 00 21 91 cb          	vcvtps2pi v1\{k1\},v3,rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 91 cb          	vcvtps2pi v1\{k1\},v3\{cdab\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 91 cb          	vcvtps2pi v1\{k1\},v3\{badc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 30 21 91 cb          	vcvtps2pi v1\{k1\},v3\{aaaa\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 91 cb          	vcvtps2pi v1\{k1\},v3\{bbbb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 91 cb          	vcvtps2pi v1\{k1\},v3\{cccc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 91 cb          	vcvtps2pi v1\{k1\},v3\{dddd\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 01 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 70 09 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.4b
[ 	]*[a-f0-9]+:	d6 70 11 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.5b
[ 	]*[a-f0-9]+:	d6 70 19 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.8b
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 29 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.24b
[ 	]*[a-f0-9]+:	d6 70 31 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.31b
[ 	]*[a-f0-9]+:	d6 70 39 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.32b
[ 	]*[a-f0-9]+:	d6 80 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 92 cb          	vcvtps2pu v1\{k1\},v3,rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 92 cb          	vcvtps2pu v1\{k1\},v3\{cdab\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 92 cb          	vcvtps2pu v1\{k1\},v3\{badc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 30 21 92 cb          	vcvtps2pu v1\{k1\},v3\{aaaa\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 92 cb          	vcvtps2pu v1\{k1\},v3\{bbbb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 92 cb          	vcvtps2pu v1\{k1\},v3\{cccc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 92 cb          	vcvtps2pu v1\{k1\},v3\{dddd\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 01 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 70 09 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.4b
[ 	]*[a-f0-9]+:	d6 70 11 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.5b
[ 	]*[a-f0-9]+:	d6 70 19 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.8b
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 29 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.24b
[ 	]*[a-f0-9]+:	d6 70 31 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.31b
[ 	]*[a-f0-9]+:	d6 70 39 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.32b
[ 	]*[a-f0-9]+:	d6 80 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 98 cb          	vroundps v1\{k1\},v3,rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 98 cb          	vroundps v1\{k1\},v3\{cdab\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 98 cb          	vroundps v1\{k1\},v3\{badc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 30 21 98 cb          	vroundps v1\{k1\},v3\{aaaa\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 98 cb          	vroundps v1\{k1\},v3\{bbbb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 98 cb          	vroundps v1\{k1\},v3\{cccc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 98 cb          	vroundps v1\{k1\},v3\{dddd\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 01 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 70 09 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.4b
[ 	]*[a-f0-9]+:	d6 70 11 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.5b
[ 	]*[a-f0-9]+:	d6 70 19 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.8b
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 29 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.24b
[ 	]*[a-f0-9]+:	d6 70 31 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.31b
[ 	]*[a-f0-9]+:	d6 70 39 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.32b
[ 	]*[a-f0-9]+:	d6 80 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 cb          	vlog2lutps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b1 cb          	vlog2lutps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 cb          	vlog2lutps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b1 cb          	vlog2lutps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 cb          	vlog2lutps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 cb          	vlog2lutps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 cb          	vlog2lutps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 cb          	vlog2lutps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 cb          	vrcpresps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 c0 cb          	vrcpresps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 cb          	vrcpresps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 c0 cb          	vrcpresps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 cb          	vrcpresps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 cb          	vrcpresps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 cb          	vrcpresps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 cb          	vrcpresps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 cb          	vrsqrtlutps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 cb          	vgetexpps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b4 cb          	vgetexpps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 cb          	vgetexpps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b4 cb          	vgetexpps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 cb          	vgetexpps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 cb          	vgetexpps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 cb          	vgetexpps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 cb          	vgetexpps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 cb          	vexp2lutps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b0 cb          	vexp2lutps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 cb          	vexp2lutps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b0 cb          	vexp2lutps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 cb          	vexp2lutps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 cb          	vexp2lutps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 cb          	vexp2lutps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 cb          	vexp2lutps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 cb 02 00    	vfixupps v1\{k1\},v3,0x2
[ 	]*[a-f0-9]+:	d6 10 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{cdab\},0x2
[ 	]*[a-f0-9]+:	d6 20 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{badc\},0x2
[ 	]*[a-f0-9]+:	d6 30 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	d6 40 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	d6 50 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{cccc\},0x2
[ 	]*[a-f0-9]+:	d6 60 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{dddd\},0x2
[ 	]*[a-f0-9]+:	d6 70 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{dacb\},0x2
[ 	]*[a-f0-9]+:	d6 80 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x2
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 10 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	d6 20 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	d6 40 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	d6 50 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 60 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	d6 70 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	d6 00 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{cdab\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{badc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 30 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{aaaa\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{bbbb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{cccc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{dddd\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{dacb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 80 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 80 01 10 09          	vloadd v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 10 09 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float16\}
[ 	]*[a-f0-9]+:	d6 20 09 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float16\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 19 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 20 19 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 21 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 20 21 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 29 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 29 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 31 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 20 31 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 20 39 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 41 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 20 41 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 49 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 20 49 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 51 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 20 51 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 20 59 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 61 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 20 61 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 20 69 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 71 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 20 71 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 20 79 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 81 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 20 81 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 89 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 20 89 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 91 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 20 91 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 99 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 20 99 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 20 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 10 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 20 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float10C\}
[ 	]*[a-f0-9]+:	66 d6 80 01 10 09       	vloadq v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 10 09       	vloadq v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 01 10 09       	vloadq v1\{k1\},\[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 01 10 09       	vloadq v1\{k1\},\[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	d6 80 01 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 13 09          	vloadunpackhd v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 00 09 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 00 19 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 21 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 81 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 89 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 91 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 99 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a9 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 b1 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 80 01 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 12 09          	vloadunpackld v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 00 09 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 00 19 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 21 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 81 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 89 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 91 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 99 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a9 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 b1 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{cdab\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{badc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 30 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{aaaa\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{bbbb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{cccc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{dddd\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{dacb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 80 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 01 f1 ca 02 02    	vshuf128x32 v1\{k1\},v2,\{0002\},\{aaac\}
[ 	]*[a-f0-9]+:	d6 80 01 f1 09 02 02    	vshuf128x32 v1\{k1\},\[rcx\]\{nt\},\{0002\},\{aaac\}
[ 	]*[a-f0-9]+:	d6 80 01 14 09          	vstored \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 00 01 14 09          	vstored \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 00 09 14 09          	vstored \[rcx\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 00 11 14 09          	vstored \[rcx\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 21 14 09          	vstored \[rcx\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 14 09          	vstored \[rcx\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 14 09          	vstored \[rcx\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 14 09          	vstored \[rcx\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 14 09          	vstored \[rcx\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 14 09          	vstored \[rcx\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 14 09          	vstored \[rcx\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 14 09          	vstored \[rcx\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 14 09          	vstored \[rcx\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 14 09          	vstored \[rcx\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 14 09          	vstored \[rcx\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 14 09          	vstored \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 01 14 09          	vstored \[rcx\]\{k1\},v1\{a\}
[ 	]*[a-f0-9]+:	d6 10 09 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 11 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 21 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 29 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 31 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 41 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 49 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 51 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 61 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 71 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 20 01 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}
[ 	]*[a-f0-9]+:	d6 20 09 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{float16\}
[ 	]*[a-f0-9]+:	d6 20 11 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 20 21 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 20 29 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 31 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 20 39 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 20 41 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 20 49 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 20 51 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 20 59 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 20 61 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 20 69 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 20 71 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 20 79 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint16i\}
[ 	]*[a-f0-9]+:	66 d6 80 01 14 09       	vstoreq \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	66 d6 00 01 14 09       	vstoreq \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	66 d6 10 01 14 09       	vstoreq \[rcx\]\{k1\},v1\{a\}
[ 	]*[a-f0-9]+:	66 d6 20 01 14 09       	vstoreq \[rcx\]\{k1\},v1\{dcba\}
[ 	]*[a-f0-9]+:	66 d6 80 01 13 09       	vloadunpackhq v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 13 09       	vloadunpackhq v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	66 d6 80 01 12 09       	vloadunpacklq v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 12 09       	vloadunpacklq v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 80 01 17 09          	vpackstorehd \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 00 01 17 09          	vpackstorehd \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 00 09 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 00 11 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 21 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 80 01 16 09          	vpackstoreld \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 00 01 16 09          	vpackstoreld \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 00 09 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 00 11 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 21 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	66 d6 80 01 17 09       	vpackstorehq \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	66 d6 00 01 17 09       	vpackstorehq \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	66 d6 80 01 16 09       	vpackstorelq \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	66 d6 00 01 16 09       	vpackstorelq \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 90 01 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 10 01 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 10 09 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 10 11 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 21 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 29 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 31 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 41 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 49 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 51 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 61 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 71 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{nt\}
[ 	]*[a-f0-9]+:	d6 10 01 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]
[ 	]*[a-f0-9]+:	d6 10 09 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float16\}
[ 	]*[a-f0-9]+:	d6 10 19 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 21 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 29 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 31 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 41 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 49 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 51 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 61 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 71 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 81 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 89 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 91 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 99 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a1 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 a9 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b1 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 90 01 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 90 01 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float10C\}
[ 	]*[a-f0-9]+:	66 62 f4 c3             	bitinterleave11 bx,ax
[ 	]*[a-f0-9]+:	66 62 94 cf             	bitinterleave11 r15w,r9w
[ 	]*[a-f0-9]+:	66 62 f5 c3             	bitinterleave21 bx,ax
[ 	]*[a-f0-9]+:	66 62 95 cf             	bitinterleave21 r15w,r9w
[ 	]*[a-f0-9]+:	66 62 f2 c3             	bsff   bx,ax
[ 	]*[a-f0-9]+:	66 62 92 cf             	bsff   r15w,r9w
[ 	]*[a-f0-9]+:	66 62 f0 c3             	bsfi   bx,ax
[ 	]*[a-f0-9]+:	66 62 90 cf             	bsfi   r15w,r9w
[ 	]*[a-f0-9]+:	66 62 f3 c3             	bsrf   bx,ax
[ 	]*[a-f0-9]+:	66 62 93 cf             	bsrf   r15w,r9w
[ 	]*[a-f0-9]+:	66 62 f1 c3             	bsri   bx,ax
[ 	]*[a-f0-9]+:	66 62 91 cf             	bsri   r15w,r9w
[ 	]*[a-f0-9]+:	66 62 f8 c3             	countbits bx,ax
[ 	]*[a-f0-9]+:	66 62 98 cf             	countbits r15w,r9w
[ 	]*[a-f0-9]+:	66 62 f9 c3             	quadmask16 bx,ax
[ 	]*[a-f0-9]+:	66 62 99 cf             	quadmask16 r15w,r9w
[ 	]*[a-f0-9]+:	62 f4 c3                	bitinterleave11 ebx,eax
[ 	]*[a-f0-9]+:	62 94 cf                	bitinterleave11 r15d,r9d
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 ebx,eax
[ 	]*[a-f0-9]+:	62 95 cf                	bitinterleave21 r15d,r9d
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   ebx,eax
[ 	]*[a-f0-9]+:	62 92 cf                	bsff   r15d,r9d
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   ebx,eax
[ 	]*[a-f0-9]+:	62 90 cf                	bsfi   r15d,r9d
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   ebx,eax
[ 	]*[a-f0-9]+:	62 93 cf                	bsrf   r15d,r9d
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   ebx,eax
[ 	]*[a-f0-9]+:	62 91 cf                	bsri   r15d,r9d
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits ebx,eax
[ 	]*[a-f0-9]+:	62 98 cf                	countbits r15d,r9d
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 ebx,eax
[ 	]*[a-f0-9]+:	62 99 cf                	quadmask16 r15d,r9d
[ 	]*[a-f0-9]+:	62 e4 c3                	bitinterleave11 rbx,rax
[ 	]*[a-f0-9]+:	62 84 cf                	bitinterleave11 r15,r9
[ 	]*[a-f0-9]+:	62 e5 c3                	bitinterleave21 rbx,rax
[ 	]*[a-f0-9]+:	62 85 cf                	bitinterleave21 r15,r9
[ 	]*[a-f0-9]+:	62 e2 c3                	bsff   rbx,rax
[ 	]*[a-f0-9]+:	62 82 cf                	bsff   r15,r9
[ 	]*[a-f0-9]+:	62 e0 c3                	bsfi   rbx,rax
[ 	]*[a-f0-9]+:	62 80 cf                	bsfi   r15,r9
[ 	]*[a-f0-9]+:	62 e3 c3                	bsrf   rbx,rax
[ 	]*[a-f0-9]+:	62 83 cf                	bsrf   r15,r9
[ 	]*[a-f0-9]+:	62 e1 c3                	bsri   rbx,rax
[ 	]*[a-f0-9]+:	62 81 cf                	bsri   r15,r9
[ 	]*[a-f0-9]+:	62 e8 c3                	countbits rbx,rax
[ 	]*[a-f0-9]+:	62 88 cf                	countbits r15,r9
[ 	]*[a-f0-9]+:	62 e9 c3                	quadmask16 rbx,rax
[ 	]*[a-f0-9]+:	62 89 cf                	quadmask16 r15,r9
[ 	]*[a-f0-9]+:	62 eb 01                	clevict1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 09                	clevict2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 cb 39                	vprefetch1 \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 db 19                	vprefetch2 \[rcx\],0x3
[ 	]*[a-f0-9]+:	62 eb 11                	ldvxcsr DWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 19                	stvxcsr DWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 62 df c0             	delay  ax
[ 	]*[a-f0-9]+:	66 62 9f c1             	delay  r9w
[ 	]*[a-f0-9]+:	66 62 df c8             	spflt  ax
[ 	]*[a-f0-9]+:	66 62 9f c9             	spflt  r9w
[ 	]*[a-f0-9]+:	62 df c0                	delay  eax
[ 	]*[a-f0-9]+:	62 9f c1                	delay  r9d
[ 	]*[a-f0-9]+:	62 df c8                	spflt  eax
[ 	]*[a-f0-9]+:	62 9f c9                	spflt  r9d
[ 	]*[a-f0-9]+:	62 cf c0                	delay  rax
[ 	]*[a-f0-9]+:	62 8f c1                	delay  r9
[ 	]*[a-f0-9]+:	62 cf c8                	spflt  rax
[ 	]*[a-f0-9]+:	62 8f c9                	spflt  r9
[ 	]*[a-f0-9]+:	d6 00 50 04 c0          	delay  0xa
[ 	]*[a-f0-9]+:	66 62 f6 c3 0b 00 02 1f 	insertfield bx,ax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	66 62 96 cf 0b 00 02 1f 	insertfield r15w,r9w,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	66 62 f7 c3 0b 00 02 1f 	rotatefield bx,ax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	66 62 97 cf 0b 00 02 1f 	rotatefield r15w,r9w,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 f6 c3 0b 00 02 1f    	insertfield ebx,eax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 96 cf 0b 00 02 1f    	insertfield r15d,r9d,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 f7 c3 0b 00 02 1f    	rotatefield ebx,eax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 97 cf 0b 00 02 1f    	rotatefield r15d,r9d,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 e6 c3 0b 00 02 1f    	insertfield rbx,rax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 86 cf 0b 00 02 1f    	insertfield r15,r9,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 e7 c3 0b 00 02 1f    	rotatefield rbx,rax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 87 cf 0b 00 02 1f    	rotatefield r15,r9,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 17 11 45 d6          	vadcpi v10\{k1\},k2,v30\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 67 a1 48 d6       	vaddnpd v10\{k1\},v20,v30\{dddd\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 78 56 34 12 	vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0x12345678
[ 	]*[a-f0-9]+:	d6 04 11 45 55 00       	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbp\+0x0\]
[ 	]*[a-f0-9]+:	d6 04 11 45 14 24       	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rsp\]
[ 	]*[a-f0-9]+:	d6 04 11 45 95 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbp\+0x99\]
[ 	]*[a-f0-9]+:	d6 05 11 45 97 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r15\+0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 15 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rip\+0x99\]        # 30f3 <_start\+0x30f3>
[ 	]*[a-f0-9]+:	d6 04 11 45 94 24 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rsp\+0x99\]
[ 	]*[a-f0-9]+:	d6 05 11 45 94 24 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r12\+0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0xffffffffffffff67
[ 	]*[a-f0-9]+:	d6 04 11 45 14 65 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[riz\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 94 23 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 94 63 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 07 11 45 94 bc 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r12\+r15\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 07 11 45 94 f8 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r8\+r15\*8-0x99\]
[ 	]*[a-f0-9]+:	d6 06 11 45 94 ad 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbp\+r13\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 06 11 45 94 24 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rsp\+r12\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 00 00 00 00 	vgatherd v10\{k1\},\[v1\*1\+0x0\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 00 00 00 00 	vgatherd v10\{k1\},\[v10\*1\+0x0\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 67 ff ff ff 	vgatherd v10\{k1\},\[v1\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 67 ff ff ff 	vgatherd v10\{k1\},\[v10\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0b       	vgatherd v10\{k1\},\[rbx\+v1\*1\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 13       	vgatherd v10\{k1\},\[rbx\+v10\*1\]
[ 	]*[a-f0-9]+:	d6 04 01 18 94 0b 67 ff ff ff 	vgatherd v10\{k1\},\[rbx\+v1\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 14 01 18 94 13 67 ff ff ff 	vgatherd v10\{k1\},\[rbx\+v10\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 04 01 18 94 4b 67 ff ff ff 	vgatherd v10\{k1\},\[rbx\+v1\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 14 01 18 94 53 67 ff ff ff 	vgatherd v10\{k1\},\[rbx\+v10\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 05 01 18 94 8c 67 ff ff ff 	vgatherd v10\{k1\},\[r12\+v1\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 15 01 18 94 94 67 ff ff ff 	vgatherd v10\{k1\},\[r12\+v10\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 34 12 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0x1234
[ 	]*[a-f0-9]+:	67 d6 04 11 45 55 00    	vadcpi v10\{k1\},k2,ZMMWORD PTR \[ebp\+0x0\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 24    	vadcpi v10\{k1\},k2,ZMMWORD PTR \[esp\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 95 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[ebp\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 25 99 00 00 00 	addr32 vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0x99
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 65 99 00 00 00 	addr32 vadcpi v10\{k1\},k2,ZMMWORD PTR \[eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 20 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 60 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 98 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[eax\+ebx\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 cc 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[esp\+ecx\*8\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 15 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[ebp\+edx\*1\+0x99\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 00 00 00 00 	vgatherd v10\{k1\},\[v1\*1\+0x0\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 00 00 00 00 	vgatherd v10\{k1\},\[v10\*1\+0x0\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 67 ff ff ff 	vgatherd v10\{k1\},\[v1\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 67 ff ff ff 	vgatherd v10\{k1\},\[v10\*1-0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 14 08    	vgatherd v10\{k1\},\[eax\+v1\*1\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 14 10    	vgatherd v10\{k1\},\[eax\+v10\*1\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 08 99 00 00 00 	vgatherd v10\{k1\},\[eax\+v1\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 10 99 00 00 00 	vgatherd v10\{k1\},\[eax\+v10\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 48 99 00 00 00 	vgatherd v10\{k1\},\[eax\+v1\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 50 99 00 00 00 	vgatherd v10\{k1\},\[eax\+v10\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 8d 99 00 00 00 	vgatherd v10\{k1\},\[ebp\+v1\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 95 99 00 00 00 	vgatherd v10\{k1\},\[ebp\+v10\*4\+0x99\]
[ 	]*[a-f0-9]+:	62 cd ca                	vkand  k2,k1
[ 	]*[a-f0-9]+:	62 dd ca                	vkandn k2,k1
[ 	]*[a-f0-9]+:	62 ed ca                	vkandnr k2,k1
[ 	]*[a-f0-9]+:	62 fc ca                	vkmov  k2,k1
[ 	]*[a-f0-9]+:	62 fb ca                	vkmovlhb k2,k1
[ 	]*[a-f0-9]+:	62 fd ca                	vknot  k2,k1
[ 	]*[a-f0-9]+:	62 ce ca                	vkor   k2,k1
[ 	]*[a-f0-9]+:	62 de ca                	vkortest k2,k1
[ 	]*[a-f0-9]+:	62 ef ca                	vkswapb k2,k1
[ 	]*[a-f0-9]+:	62 fe ca                	vkxnor k2,k1
[ 	]*[a-f0-9]+:	62 ee ca                	vkxor  k2,k1
[ 	]*[a-f0-9]+:	66 62 ec c2             	vkmov  k2,ax
[ 	]*[a-f0-9]+:	66 62 cc ca             	vkmov  k2,r9w
[ 	]*[a-f0-9]+:	62 ec c2                	vkmov  k2,eax
[ 	]*[a-f0-9]+:	62 cc ca                	vkmov  k2,r9d
[ 	]*[a-f0-9]+:	66 62 dc c8             	vkmov  ax,k1
[ 	]*[a-f0-9]+:	66 62 9c c9             	vkmov  r9w,k1
[ 	]*[a-f0-9]+:	62 dc c8                	vkmov  eax,k1
[ 	]*[a-f0-9]+:	62 9c c9                	vkmov  r9d,k1
[ 	]*[a-f0-9]+:	d6 00 11 45 cb          	vadcpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 45 cb          	vadcpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 45 cb          	vadcpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 45 cb          	vadcpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 45 cb          	vadcpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 45 cb          	vadcpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 45 cb          	vadcpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 45 cb          	vadcpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 45 09          	vadcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 43 cb          	vaddsetcpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 43 09          	vaddsetcpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 55 cb          	vsbbpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 55 cb          	vsbbpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 55 09          	vsbbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 5d cb          	vsbbrpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 5d cb          	vsbbrpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 5d 09          	vsbbrpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 5b cb          	vsubrsetbpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 5b 09          	vsubrsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3
[ 	]*[a-f0-9]+:	d6 10 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 53 cb          	vsubsetbpi v1\{k1\},k2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 53 09          	vsubsetbpi v1\{k1\},k2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 48 cb          	vaddnps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 48 cb          	vaddnps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 48 cb          	vaddnps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 48 cb          	vaddnps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 48 cb          	vaddnps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 48 cb          	vaddnps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 48 cb          	vaddnps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 48 cb          	vaddnps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 48 09          	vaddnps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 40 cb          	vaddps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 40 cb          	vaddps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 40 cb          	vaddps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 40 cb          	vaddps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 40 cb          	vaddps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 40 cb          	vaddps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 40 cb          	vaddps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 40 cb          	vaddps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 40 09          	vaddps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 46 cb          	vaddsetsps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 46 cb          	vaddsetsps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 46 09          	vaddsetsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 86 cb          	vclampzps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 86 cb          	vclampzps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 86 cb          	vclampzps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 86 cb          	vclampzps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 86 cb          	vclampzps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 86 cb          	vclampzps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 86 cb          	vclampzps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 86 cb          	vclampzps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 86 09          	vclampzps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 64 cb          	vmadd132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 64 cb          	vmadd132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 64 09          	vmadd132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 62 cb          	vmadd213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 62 cb          	vmadd213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 62 09          	vmadd213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 60 cb          	vmadd231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 60 cb          	vmadd231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 60 09          	vmadd231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 66 cb          	vmadd233ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 66 cb          	vmadd233ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 66 09          	vmadd233ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 74 cb          	vmaddn132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 74 09          	vmaddn132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 72 cb          	vmaddn213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 72 09          	vmaddn213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 70 cb          	vmaddn231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 70 09          	vmaddn231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 9e cb          	vmaxabsps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 9e cb          	vmaxabsps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 9e 09          	vmaxabsps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 88 cb          	vmaxps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 88 cb          	vmaxps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 88 cb          	vmaxps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 88 cb          	vmaxps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 88 cb          	vmaxps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 88 cb          	vmaxps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 88 cb          	vmaxps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 88 cb          	vmaxps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 88 09          	vmaxps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8c cb          	vminps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8c cb          	vminps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8c cb          	vminps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8c cb          	vminps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8c cb          	vminps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8c cb          	vminps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8c cb          	vminps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8c cb          	vminps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8c 09          	vminps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 6c cb          	vmsub132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 6c cb          	vmsub132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 6c 09          	vmsub132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 6a cb          	vmsub213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 6a cb          	vmsub213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 6a 09          	vmsub213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 68 cb          	vmsub231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 68 cb          	vmsub231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 68 09          	vmsub231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 7c cb          	vmsubr132ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 7c 09          	vmsubr132ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 7a cb          	vmsubr213ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 7a 09          	vmsubr213ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 78 cb          	vmsubr231ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 78 09          	vmsubr231ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 7e cb          	vmsubr23c1ps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 7e 09          	vmsubr23c1ps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 80 cb          	vmulps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 80 cb          	vmulps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 80 cb          	vmulps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 80 cb          	vmulps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 80 cb          	vmulps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 80 cb          	vmulps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 80 cb          	vmulps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 80 cb          	vmulps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 80 09          	vmulps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 cb          	vrcprefineps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 09          	vrcprefineps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 50 cb          	vsubps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 50 cb          	vsubps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 50 cb          	vsubps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 50 cb          	vsubps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 50 cb          	vsubps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 50 cb          	vsubps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 50 cb          	vsubps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 50 cb          	vsubps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 50 09          	vsubps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 58 cb          	vsubrps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 58 cb          	vsubrps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 58 cb          	vsubrps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 58 cb          	vsubrps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 58 cb          	vsubrps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 58 cb          	vsubrps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 58 cb          	vsubrps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 58 cb          	vsubrps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 58 09          	vsubrps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 cb       	vaddnpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 48 cb       	vaddnpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 09       	vaddnpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 cb       	vaddpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 40 cb       	vaddpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 cb       	vaddpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 40 cb       	vaddpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 40 cb       	vaddpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 40 cb       	vaddpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 40 cb       	vaddpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 40 cb       	vaddpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 09       	vaddpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 cb       	vmadd132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 64 cb       	vmadd132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 09       	vmadd132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 cb       	vmadd213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 62 cb       	vmadd213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 09       	vmadd213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 cb       	vmadd231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 60 cb       	vmadd231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 09       	vmadd231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 74 cb       	vmaddn132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 09       	vmaddn132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 72 cb       	vmaddn213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 09       	vmaddn213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 70 cb       	vmaddn231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 09       	vmaddn231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 cb       	vmaxpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 88 cb       	vmaxpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 09       	vmaxpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c cb       	vminpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 8c cb       	vminpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c cb       	vminpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 8c cb       	vminpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 8c cb       	vminpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 8c cb       	vminpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 8c cb       	vminpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 8c cb       	vminpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c 09       	vminpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c cb       	vmsub132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6c cb       	vmsub132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c 09       	vmsub132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a cb       	vmsub213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6a cb       	vmsub213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a 09       	vmsub213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 cb       	vmsub231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 68 cb       	vmsub231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 09       	vmsub231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7c cb       	vmsubr132pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c 09       	vmsubr132pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7a cb       	vmsubr213pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a 09       	vmsubr213pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 78 cb       	vmsubr231pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 09       	vmsubr231pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7e cb       	vmsubr23c1pd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e 09       	vmsubr23c1pd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 cb       	vmulpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 80 cb       	vmulpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 cb       	vmulpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 80 cb       	vmulpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 80 cb       	vmulpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 80 cb       	vmulpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 80 cb       	vmulpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 80 cb       	vmulpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 09       	vmulpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 cb       	vsubpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 50 cb       	vsubpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 cb       	vsubpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 50 cb       	vsubpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 50 cb       	vsubpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 50 cb       	vsubpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 50 cb       	vsubpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 50 cb       	vsubpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 09       	vsubpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 cb       	vsubrpd v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 58 cb       	vsubrpd v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 09       	vsubrpd v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	d6 00 11 41 cb          	vaddpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 41 cb          	vaddpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 41 cb          	vaddpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 41 cb          	vaddpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 41 cb          	vaddpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 41 cb          	vaddpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 41 cb          	vaddpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 41 cb          	vaddpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 41 09          	vaddpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 47 cb          	vaddsetspi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 47 cb          	vaddsetspi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 47 09          	vaddsetspi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 cb          	vandnpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 cb          	vandnpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 09          	vandnpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 cb          	vandpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a5 cb          	vandpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 cb          	vandpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a5 cb          	vandpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 cb          	vandpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 cb          	vandpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 cb          	vandpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 cb          	vandpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 09          	vandpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 cb          	vbitinterleave11pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 09          	vbitinterleave11pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 cb          	vbitinterleave21pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 09          	vbitinterleave21pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 87 cb          	vclampzpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 87 cb          	vclampzpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 87 09          	vclampzpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 61 cb          	vmadd231pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 61 cb          	vmadd231pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 61 09          	vmadd231pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 67 cb          	vmadd233pi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 67 cb          	vmadd233pi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 67 09          	vmadd233pi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 89 cb          	vmaxpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 89 cb          	vmaxpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 89 09          	vmaxpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8a cb          	vmaxpu v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8a cb          	vmaxpu v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8a 09          	vmaxpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8d cb          	vminpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8d cb          	vminpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8d cb          	vminpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8d cb          	vminpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8d cb          	vminpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8d cb          	vminpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8d cb          	vminpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8d cb          	vminpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8d 09          	vminpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 8e cb          	vminpu v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 8e cb          	vminpu v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 8e cb          	vminpu v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 8e cb          	vminpu v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 8e cb          	vminpu v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 8e cb          	vminpu v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 8e cb          	vminpu v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 8e cb          	vminpu v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 8e 09          	vminpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 81 cb          	vmulhpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 81 cb          	vmulhpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 81 09          	vmulhpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 82 cb          	vmulhpu v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 82 cb          	vmulhpu v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 82 09          	vmulhpu v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 83 cb          	vmullpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 83 cb          	vmullpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 83 cb          	vmullpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 83 cb          	vmullpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 83 cb          	vmullpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 83 cb          	vmullpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 83 cb          	vmullpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 83 cb          	vmullpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 83 09          	vmullpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 cb          	vorpi  v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 cb          	vorpi  v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 09          	vorpi  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 cb          	vscaleps v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 cb          	vscaleps v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 09          	vscaleps v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 cb          	vsllpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 cb          	vsllpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 09          	vsllpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 cb          	vsrapi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 cb          	vsrapi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 09          	vsrapi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 cb          	vsrlpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 cb          	vsrlpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 09          	vsrlpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 51 cb          	vsubpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 51 cb          	vsubpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 51 cb          	vsubpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 51 cb          	vsubpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 51 cb          	vsubpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 51 cb          	vsubpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 51 cb          	vsubpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 51 cb          	vsubpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 51 09          	vsubpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 59 cb          	vsubrpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 59 cb          	vsubrpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 59 09          	vsubrpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 cb          	vxorpi v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 cb          	vxorpi v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 09          	vxorpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 12 30 cb          	vtestpi k1\{k2\},v2,v3
[ 	]*[a-f0-9]+:	d6 10 12 30 cb          	vtestpi k1\{k2\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 12 30 cb          	vtestpi k1\{k2\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 12 30 cb          	vtestpi k1\{k2\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 12 30 cb          	vtestpi k1\{k2\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 12 30 cb          	vtestpi k1\{k2\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 12 30 cb          	vtestpi k1\{k2\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 12 30 cb          	vtestpi k1\{k2\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 12 30 09          	vtestpi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 cb       	vandnpq v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a4 cb       	vandnpq v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 09       	vandnpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 cb       	vandpq v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 cb       	vandpq v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 cb       	vandpq v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a5 cb       	vandpq v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a5 cb       	vandpq v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a5 cb       	vandpq v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a5 cb       	vandpq v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a5 cb       	vandpq v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 09       	vandpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 cb       	vorpq  v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a6 cb       	vorpq  v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 09       	vorpq  v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 cb       	vxorpq v1\{k1\},v2,v3
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{badc\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{aaaa\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{bbbb\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{cccc\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{dddd\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a7 cb       	vxorpq v1\{k1\},v2,v3\{dacb\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 09       	vxorpq v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 cb       	vcmppd k1\{k2\},v2,v3,unord
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{cdab\},unord
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{badc\},unord
[ 	]*[a-f0-9]+:	66 d6 3c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{aaaa\},unord
[ 	]*[a-f0-9]+:	66 d6 4c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{bbbb\},unord
[ 	]*[a-f0-9]+:	66 d6 5c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{cccc\},unord
[ 	]*[a-f0-9]+:	66 d6 6c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dddd\},unord
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	66 d6 70 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	66 d6 80 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	66 d6 74 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	66 d6 84 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	66 d6 78 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	66 d6 88 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	66 d6 8c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},unord
[ 	]*[a-f0-9]+:	66 d6 70 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	66 d6 80 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	66 d6 74 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	66 d6 84 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	66 d6 78 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	66 d6 88 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	66 d6 7c 12 24 cb       	vcmppd k1\{k2\},v2,v3\{dacb\},ord
[ 	]*[a-f0-9]+:	66 d6 8c 12 24 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},ord
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to8\},unord
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to8\},unord
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 09       	vcmppd k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to8\},unord
[ 	]*[a-f0-9]+:	d6 04 12 21 cb          	vcmppi k1\{k2\},v2,v3,lt
[ 	]*[a-f0-9]+:	d6 14 12 21 cb          	vcmppi k1\{k2\},v2,v3\{cdab\},lt
[ 	]*[a-f0-9]+:	d6 24 12 21 cb          	vcmppi k1\{k2\},v2,v3\{badc\},lt
[ 	]*[a-f0-9]+:	d6 34 12 21 cb          	vcmppi k1\{k2\},v2,v3\{aaaa\},lt
[ 	]*[a-f0-9]+:	d6 44 12 21 cb          	vcmppi k1\{k2\},v2,v3\{bbbb\},lt
[ 	]*[a-f0-9]+:	d6 54 12 21 cb          	vcmppi k1\{k2\},v2,v3\{cccc\},lt
[ 	]*[a-f0-9]+:	d6 64 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dddd\},lt
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 70 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	d6 80 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 84 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	d6 78 12 21 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	d6 88 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	d6 70 12 25 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	d6 80 12 25 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	d6 74 12 25 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	d6 84 12 25 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	d6 78 12 25 cb          	vcmppi k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	d6 88 12 25 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 14 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\},lt
[ 	]*[a-f0-9]+:	d6 24 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\},lt
[ 	]*[a-f0-9]+:	d6 44 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\},lt
[ 	]*[a-f0-9]+:	d6 54 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint8\},lt
[ 	]*[a-f0-9]+:	d6 64 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint16\},lt
[ 	]*[a-f0-9]+:	d6 74 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},lt
[ 	]*[a-f0-9]+:	d6 74 12 21 09          	vcmppi k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},lt
[ 	]*[a-f0-9]+:	d6 04 12 22 cb          	vcmppu k1\{k2\},v2,v3,lt
[ 	]*[a-f0-9]+:	d6 14 12 22 cb          	vcmppu k1\{k2\},v2,v3\{cdab\},lt
[ 	]*[a-f0-9]+:	d6 24 12 22 cb          	vcmppu k1\{k2\},v2,v3\{badc\},lt
[ 	]*[a-f0-9]+:	d6 34 12 22 cb          	vcmppu k1\{k2\},v2,v3\{aaaa\},lt
[ 	]*[a-f0-9]+:	d6 44 12 22 cb          	vcmppu k1\{k2\},v2,v3\{bbbb\},lt
[ 	]*[a-f0-9]+:	d6 54 12 22 cb          	vcmppu k1\{k2\},v2,v3\{cccc\},lt
[ 	]*[a-f0-9]+:	d6 64 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dddd\},lt
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 70 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	d6 80 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 84 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	d6 78 12 22 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	d6 88 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	d6 70 12 26 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	d6 80 12 26 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	d6 74 12 26 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	d6 84 12 26 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	d6 78 12 26 cb          	vcmppu k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	d6 88 12 26 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\],lt
[ 	]*[a-f0-9]+:	d6 14 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\},lt
[ 	]*[a-f0-9]+:	d6 24 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\},lt
[ 	]*[a-f0-9]+:	d6 44 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\},lt
[ 	]*[a-f0-9]+:	d6 54 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint8\},lt
[ 	]*[a-f0-9]+:	d6 64 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint16\},lt
[ 	]*[a-f0-9]+:	d6 74 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},lt
[ 	]*[a-f0-9]+:	d6 74 12 22 09          	vcmppu k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},lt
[ 	]*[a-f0-9]+:	d6 0c 12 20 cb          	vcmpps k1\{k2\},v2,v3,unord
[ 	]*[a-f0-9]+:	d6 1c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{cdab\},unord
[ 	]*[a-f0-9]+:	d6 2c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{badc\},unord
[ 	]*[a-f0-9]+:	d6 3c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{aaaa\},unord
[ 	]*[a-f0-9]+:	d6 4c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{bbbb\},unord
[ 	]*[a-f0-9]+:	d6 5c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{cccc\},unord
[ 	]*[a-f0-9]+:	d6 6c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dddd\},unord
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	d6 70 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},eq
[ 	]*[a-f0-9]+:	d6 80 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},eq
[ 	]*[a-f0-9]+:	d6 74 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},lt
[ 	]*[a-f0-9]+:	d6 84 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},lt
[ 	]*[a-f0-9]+:	d6 78 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},le
[ 	]*[a-f0-9]+:	d6 88 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},le
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},unord
[ 	]*[a-f0-9]+:	d6 8c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},unord
[ 	]*[a-f0-9]+:	d6 70 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},neq
[ 	]*[a-f0-9]+:	d6 80 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},neq
[ 	]*[a-f0-9]+:	d6 74 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},nlt
[ 	]*[a-f0-9]+:	d6 84 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nlt
[ 	]*[a-f0-9]+:	d6 78 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},nle
[ 	]*[a-f0-9]+:	d6 88 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},nle
[ 	]*[a-f0-9]+:	d6 7c 12 24 cb          	vcmpps k1\{k2\},v2,v3\{dacb\},ord
[ 	]*[a-f0-9]+:	d6 8c 12 24 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{nt\},ord
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\],unord
[ 	]*[a-f0-9]+:	d6 1c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{1to16\},unord
[ 	]*[a-f0-9]+:	d6 2c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{4to16\},unord
[ 	]*[a-f0-9]+:	d6 4c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{uint8\},unord
[ 	]*[a-f0-9]+:	d6 5c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{unorm8\},unord
[ 	]*[a-f0-9]+:	d6 6c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{float16\},unord
[ 	]*[a-f0-9]+:	d6 7c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},unord
[ 	]*[a-f0-9]+:	d6 7c 12 20 09          	vcmpps k1\{k2\},v2,ZMMWORD PTR \[rcx\]\{sint16\},unord
[ 	]*[a-f0-9]+:	d6 00 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3,rn,0x2
[ 	]*[a-f0-9]+:	d6 10 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{cdab\},rn,0x2
[ 	]*[a-f0-9]+:	d6 20 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{badc\},rn,0x2
[ 	]*[a-f0-9]+:	d6 30 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{aaaa\},rn,0x2
[ 	]*[a-f0-9]+:	d6 40 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{bbbb\},rn,0x2
[ 	]*[a-f0-9]+:	d6 50 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{cccc\},rn,0x2
[ 	]*[a-f0-9]+:	d6 60 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dddd\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dacb\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dacb\},rn,0x2
[ 	]*[a-f0-9]+:	d6 80 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 d1 9b cb          	vcvtinsps2f11 v1\{k1\},v3\{dacb\},rz,0x2
[ 	]*[a-f0-9]+:	d6 80 d1 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x2
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x2
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x2
[ 	]*[a-f0-9]+:	d6 10 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 20 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 40 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,0x2
[ 	]*[a-f0-9]+:	d6 50 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,0x2
[ 	]*[a-f0-9]+:	d6 60 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 70 11 9b 09          	vcvtinsps2f11 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,0x2
[ 	]*[a-f0-9]+:	d6 00 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3,0x2
[ 	]*[a-f0-9]+:	d6 10 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{cdab\},0x2
[ 	]*[a-f0-9]+:	d6 20 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{badc\},0x2
[ 	]*[a-f0-9]+:	d6 30 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	d6 40 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	d6 50 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{cccc\},0x2
[ 	]*[a-f0-9]+:	d6 60 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{dddd\},0x2
[ 	]*[a-f0-9]+:	d6 70 11 9a cb          	vcvtinsps2u10 v1\{k1\},v3\{dacb\},0x2
[ 	]*[a-f0-9]+:	d6 80 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x2
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 10 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	d6 20 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	d6 40 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	d6 50 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 60 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	d6 70 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	d6 70 11 9a 09          	vcvtinsps2u10 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	66 d6 00 09 91 cb       	vcvtpd2pi v1\{k1\},v3,rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{cdab\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{badc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{aaaa\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{bbbb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{cccc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{dddd\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 49 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rd,0x1
[ 	]*[a-f0-9]+:	66 d6 00 49 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rd,0x1
[ 	]*[a-f0-9]+:	66 d6 70 89 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},ru,0x1
[ 	]*[a-f0-9]+:	66 d6 00 89 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],ru,0x1
[ 	]*[a-f0-9]+:	66 d6 70 c9 91 cb       	vcvtpd2pi v1\{k1\},v3\{dacb\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 c9 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rz,0x1
[ 	]*[a-f0-9]+:	66 d6 80 c9 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 91 09       	vcvtpd2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 cb       	vcvtpd2ps v1\{k1\},v3,rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{cdab\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{badc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{aaaa\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{bbbb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{cccc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{dddd\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 49 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rd,0x1
[ 	]*[a-f0-9]+:	66 d6 00 49 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rd,0x1
[ 	]*[a-f0-9]+:	66 d6 70 89 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},ru,0x1
[ 	]*[a-f0-9]+:	66 d6 00 89 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],ru,0x1
[ 	]*[a-f0-9]+:	66 d6 70 c9 90 cb       	vcvtpd2ps v1\{k1\},v3\{dacb\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 c9 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rz,0x1
[ 	]*[a-f0-9]+:	66 d6 80 c9 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 90 09       	vcvtpd2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 cb       	vcvtpd2pu v1\{k1\},v3,rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{cdab\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{badc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{aaaa\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{bbbb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{cccc\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{dddd\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 70 49 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rd,0x1
[ 	]*[a-f0-9]+:	66 d6 00 49 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rd,0x1
[ 	]*[a-f0-9]+:	66 d6 70 89 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},ru,0x1
[ 	]*[a-f0-9]+:	66 d6 00 89 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],ru,0x1
[ 	]*[a-f0-9]+:	66 d6 70 c9 92 cb       	vcvtpd2pu v1\{k1\},v3\{dacb\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 c9 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rz,0x1
[ 	]*[a-f0-9]+:	66 d6 80 c9 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rz,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{1to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 92 09       	vcvtpd2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{4to8\},rn,0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 95 cb       	vcvtpi2pd v1\{k1\},v3,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{cdab\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{badc\},0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{aaaa\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{bbbb\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{cccc\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{dddd\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 95 cb       	vcvtpi2pd v1\{k1\},v3\{dacb\},0x1
[ 	]*[a-f0-9]+:	66 d6 80 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 95 09       	vcvtpi2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 96 cb       	vcvtpu2pd v1\{k1\},v3,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{cdab\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{badc\},0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{aaaa\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{bbbb\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{cccc\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{dddd\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 96 cb       	vcvtpu2pd v1\{k1\},v3\{dacb\},0x1
[ 	]*[a-f0-9]+:	66 d6 80 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 96 09       	vcvtpu2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	d6 00 21 95 cb          	vcvtpi2ps v1\{k1\},v3,.16b
[ 	]*[a-f0-9]+:	d6 10 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{cdab\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{badc\},.16b
[ 	]*[a-f0-9]+:	d6 30 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{aaaa\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{bbbb\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{cccc\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{dddd\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 01 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.0b
[ 	]*[a-f0-9]+:	d6 70 09 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.4b
[ 	]*[a-f0-9]+:	d6 70 11 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.5b
[ 	]*[a-f0-9]+:	d6 70 19 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.8b
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 29 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.24b
[ 	]*[a-f0-9]+:	d6 70 31 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.31b
[ 	]*[a-f0-9]+:	d6 70 39 95 cb          	vcvtpi2ps v1\{k1\},v3\{dacb\},.32b
[ 	]*[a-f0-9]+:	d6 80 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},.16b
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 10 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 95 09          	vcvtpi2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},.16b
[ 	]*[a-f0-9]+:	d6 00 21 96 cb          	vcvtpu2ps v1\{k1\},v3,.16b
[ 	]*[a-f0-9]+:	d6 10 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{cdab\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{badc\},.16b
[ 	]*[a-f0-9]+:	d6 30 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{aaaa\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{bbbb\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{cccc\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{dddd\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 01 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.0b
[ 	]*[a-f0-9]+:	d6 70 09 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.4b
[ 	]*[a-f0-9]+:	d6 70 11 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.5b
[ 	]*[a-f0-9]+:	d6 70 19 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.8b
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.16b
[ 	]*[a-f0-9]+:	d6 70 29 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.24b
[ 	]*[a-f0-9]+:	d6 70 31 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.31b
[ 	]*[a-f0-9]+:	d6 70 39 96 cb          	vcvtpu2ps v1\{k1\},v3\{dacb\},.32b
[ 	]*[a-f0-9]+:	d6 80 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},.16b
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\],.16b
[ 	]*[a-f0-9]+:	d6 10 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},.16b
[ 	]*[a-f0-9]+:	d6 20 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},.16b
[ 	]*[a-f0-9]+:	d6 40 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},.16b
[ 	]*[a-f0-9]+:	d6 50 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},.16b
[ 	]*[a-f0-9]+:	d6 60 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},.16b
[ 	]*[a-f0-9]+:	d6 70 21 96 09          	vcvtpu2ps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},.16b
[ 	]*[a-f0-9]+:	66 d6 00 09 94 cb       	vcvtps2pd v1\{k1\},v3,0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 94 cb       	vcvtps2pd v1\{k1\},v3\{cdab\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 94 cb       	vcvtps2pd v1\{k1\},v3\{badc\},0x1
[ 	]*[a-f0-9]+:	66 d6 30 09 94 cb       	vcvtps2pd v1\{k1\},v3\{aaaa\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 94 cb       	vcvtps2pd v1\{k1\},v3\{bbbb\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 94 cb       	vcvtps2pd v1\{k1\},v3\{cccc\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 94 cb       	vcvtps2pd v1\{k1\},v3\{dddd\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 94 cb       	vcvtps2pd v1\{k1\},v3\{dacb\},0x1
[ 	]*[a-f0-9]+:	66 d6 80 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\],0x1
[ 	]*[a-f0-9]+:	66 d6 10 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 20 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x1
[ 	]*[a-f0-9]+:	66 d6 40 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x1
[ 	]*[a-f0-9]+:	66 d6 50 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x1
[ 	]*[a-f0-9]+:	66 d6 60 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	66 d6 70 09 94 09       	vcvtps2pd v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x1
[ 	]*[a-f0-9]+:	d6 00 21 91 cb          	vcvtps2pi v1\{k1\},v3,rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 91 cb          	vcvtps2pi v1\{k1\},v3\{cdab\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 91 cb          	vcvtps2pi v1\{k1\},v3\{badc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 30 21 91 cb          	vcvtps2pi v1\{k1\},v3\{aaaa\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 91 cb          	vcvtps2pi v1\{k1\},v3\{bbbb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 91 cb          	vcvtps2pi v1\{k1\},v3\{cccc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 91 cb          	vcvtps2pi v1\{k1\},v3\{dddd\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 01 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 70 09 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.4b
[ 	]*[a-f0-9]+:	d6 70 11 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.5b
[ 	]*[a-f0-9]+:	d6 70 19 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.8b
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 29 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.24b
[ 	]*[a-f0-9]+:	d6 70 31 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.31b
[ 	]*[a-f0-9]+:	d6 70 39 91 cb          	vcvtps2pi v1\{k1\},v3\{dacb\},rn,.32b
[ 	]*[a-f0-9]+:	d6 80 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 91 09          	vcvtps2pi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 92 cb          	vcvtps2pu v1\{k1\},v3,rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 92 cb          	vcvtps2pu v1\{k1\},v3\{cdab\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 92 cb          	vcvtps2pu v1\{k1\},v3\{badc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 30 21 92 cb          	vcvtps2pu v1\{k1\},v3\{aaaa\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 92 cb          	vcvtps2pu v1\{k1\},v3\{bbbb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 92 cb          	vcvtps2pu v1\{k1\},v3\{cccc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 92 cb          	vcvtps2pu v1\{k1\},v3\{dddd\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 01 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 70 09 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.4b
[ 	]*[a-f0-9]+:	d6 70 11 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.5b
[ 	]*[a-f0-9]+:	d6 70 19 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.8b
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 29 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.24b
[ 	]*[a-f0-9]+:	d6 70 31 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.31b
[ 	]*[a-f0-9]+:	d6 70 39 92 cb          	vcvtps2pu v1\{k1\},v3\{dacb\},rn,.32b
[ 	]*[a-f0-9]+:	d6 80 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 92 09          	vcvtps2pu v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 98 cb          	vroundps v1\{k1\},v3,rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 98 cb          	vroundps v1\{k1\},v3\{cdab\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 98 cb          	vroundps v1\{k1\},v3\{badc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 30 21 98 cb          	vroundps v1\{k1\},v3\{aaaa\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 98 cb          	vroundps v1\{k1\},v3\{bbbb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 98 cb          	vroundps v1\{k1\},v3\{cccc\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 98 cb          	vroundps v1\{k1\},v3\{dddd\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 01 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.0b
[ 	]*[a-f0-9]+:	d6 70 09 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.4b
[ 	]*[a-f0-9]+:	d6 70 11 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.5b
[ 	]*[a-f0-9]+:	d6 70 19 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.8b
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 29 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.24b
[ 	]*[a-f0-9]+:	d6 70 31 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.31b
[ 	]*[a-f0-9]+:	d6 70 39 98 cb          	vroundps v1\{k1\},v3\{dacb\},rn,.32b
[ 	]*[a-f0-9]+:	d6 80 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\],rn,.16b
[ 	]*[a-f0-9]+:	d6 10 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 20 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 40 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 50 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},rn,.16b
[ 	]*[a-f0-9]+:	d6 60 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 70 21 98 09          	vroundps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},rn,.16b
[ 	]*[a-f0-9]+:	d6 00 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 9d cb          	vcvtps2srgb8 v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 01 9d 09          	vcvtps2srgb8 v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 cb          	vlog2lutps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b1 cb          	vlog2lutps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 cb          	vlog2lutps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b1 cb          	vlog2lutps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 cb          	vlog2lutps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 cb          	vlog2lutps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 cb          	vlog2lutps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 cb          	vlog2lutps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 09          	vlog2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 cb          	vrcpresps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 c0 cb          	vrcpresps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 cb          	vrcpresps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 c0 cb          	vrcpresps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 cb          	vrcpresps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 cb          	vrcpresps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 cb          	vrcpresps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 cb          	vrcpresps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 09          	vrcpresps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 cb          	vrsqrtlutps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 cb          	vrsqrtlutps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 09          	vrsqrtlutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 cb          	vgetexpps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b4 cb          	vgetexpps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 cb          	vgetexpps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b4 cb          	vgetexpps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 cb          	vgetexpps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 cb          	vgetexpps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 cb          	vgetexpps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 cb          	vgetexpps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 09          	vgetexpps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 cb          	vexp2lutps v1\{k1\},v3
[ 	]*[a-f0-9]+:	d6 10 01 b0 cb          	vexp2lutps v1\{k1\},v3\{cdab\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 cb          	vexp2lutps v1\{k1\},v3\{badc\}
[ 	]*[a-f0-9]+:	d6 30 01 b0 cb          	vexp2lutps v1\{k1\},v3\{aaaa\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 cb          	vexp2lutps v1\{k1\},v3\{bbbb\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 cb          	vexp2lutps v1\{k1\},v3\{cccc\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 cb          	vexp2lutps v1\{k1\},v3\{dddd\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 cb          	vexp2lutps v1\{k1\},v3\{dacb\}
[ 	]*[a-f0-9]+:	d6 80 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 09          	vexp2lutps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 cb 02 00    	vfixupps v1\{k1\},v3,0x2
[ 	]*[a-f0-9]+:	d6 10 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{cdab\},0x2
[ 	]*[a-f0-9]+:	d6 20 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{badc\},0x2
[ 	]*[a-f0-9]+:	d6 30 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{aaaa\},0x2
[ 	]*[a-f0-9]+:	d6 40 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{bbbb\},0x2
[ 	]*[a-f0-9]+:	d6 50 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{cccc\},0x2
[ 	]*[a-f0-9]+:	d6 60 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{dddd\},0x2
[ 	]*[a-f0-9]+:	d6 70 01 f0 cb 02 00    	vfixupps v1\{k1\},v3\{dacb\},0x2
[ 	]*[a-f0-9]+:	d6 80 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0x2
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\],0x2
[ 	]*[a-f0-9]+:	d6 10 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0x2
[ 	]*[a-f0-9]+:	d6 20 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0x2
[ 	]*[a-f0-9]+:	d6 40 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0x2
[ 	]*[a-f0-9]+:	d6 50 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{unorm8\},0x2
[ 	]*[a-f0-9]+:	d6 60 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{float16\},0x2
[ 	]*[a-f0-9]+:	d6 70 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	d6 70 01 f0 09 02 00    	vfixupps v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0x2
[ 	]*[a-f0-9]+:	d6 00 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{cdab\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{badc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 30 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{aaaa\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{bbbb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{cccc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{dddd\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 11 f2 cb 8b f8    	vinsertfieldpi v1\{k1\},v2,v3\{dacb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 80 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{nt\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{1to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{4to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{uint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 11 f2 09 8b f8    	vinsertfieldpi v1\{k1\},v2,ZMMWORD PTR \[rcx\]\{sint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 80 01 10 09          	vloadd v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 10 01 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}
[ 	]*[a-f0-9]+:	d6 20 01 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 10 09 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float16\}
[ 	]*[a-f0-9]+:	d6 20 09 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float16\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 19 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 20 19 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 21 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 20 21 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 29 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 29 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 31 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 20 31 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 20 39 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 41 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 20 41 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 49 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 20 49 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 51 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 20 51 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 20 59 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 61 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 20 61 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 20 69 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 71 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 20 71 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 20 79 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 81 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 20 81 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 89 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 20 89 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 91 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 20 91 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 99 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 20 99 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 20 a9 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 10 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{1to16\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 20 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 20 b1 10 09          	vloadd v1\{k1\},\[rcx\]\{4to16\}\{float10C\}
[ 	]*[a-f0-9]+:	66 d6 80 01 10 09       	vloadq v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 10 09       	vloadq v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	66 d6 10 01 10 09       	vloadq v1\{k1\},\[rcx\]\{1to8\}
[ 	]*[a-f0-9]+:	66 d6 20 01 10 09       	vloadq v1\{k1\},\[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	66 d6 20 01 10 09       	vloadq v1\{k1\},\[rcx\]\{4to8\}
[ 	]*[a-f0-9]+:	d6 80 01 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 13 09          	vloadunpackhd v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 00 09 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 00 19 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 21 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 81 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 89 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 91 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 99 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a9 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 b1 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 00 b1 13 09          	vloadunpackhd v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 80 01 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 12 09          	vloadunpackld v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 00 09 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float16\}
[ 	]*[a-f0-9]+:	d6 00 19 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 00 21 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 81 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 00 89 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 00 91 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 00 99 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 00 a1 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 00 a9 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 00 b1 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 00 b1 12 09          	vloadunpackld v1\{k1\},\[rcx\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{cdab\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{badc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 30 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{aaaa\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{bbbb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{cccc\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{dddd\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 01 f3 cb 8b f8    	vrotatefieldpi v1\{k1\},v3\{dacb\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 80 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{nt\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\],0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 10 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{1to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 20 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{4to16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 40 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{uint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 50 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint8\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 60 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{uint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 70 01 f3 09 8b f8    	vrotatefieldpi v1\{k1\},ZMMWORD PTR \[rcx\]\{sint16\},0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 00 01 f1 ca 02 02    	vshuf128x32 v1\{k1\},v2,\{0002\},\{aaac\}
[ 	]*[a-f0-9]+:	d6 80 01 f1 09 02 02    	vshuf128x32 v1\{k1\},\[rcx\]\{nt\},\{0002\},\{aaac\}
[ 	]*[a-f0-9]+:	d6 00 01 f1 09 02 02    	vshuf128x32 v1\{k1\},\[rcx\],\{0002\},\{aaac\}
[ 	]*[a-f0-9]+:	d6 80 01 14 09          	vstored \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 00 01 14 09          	vstored \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 00 01 14 09          	vstored \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 00 09 14 09          	vstored \[rcx\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 00 11 14 09          	vstored \[rcx\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 21 14 09          	vstored \[rcx\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 14 09          	vstored \[rcx\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 14 09          	vstored \[rcx\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 14 09          	vstored \[rcx\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 14 09          	vstored \[rcx\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 14 09          	vstored \[rcx\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 14 09          	vstored \[rcx\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 14 09          	vstored \[rcx\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 14 09          	vstored \[rcx\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 14 09          	vstored \[rcx\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 14 09          	vstored \[rcx\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 14 09          	vstored \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 01 14 09          	vstored \[rcx\]\{k1\},v1\{a\}
[ 	]*[a-f0-9]+:	d6 10 09 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 11 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 21 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 29 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 31 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 41 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 49 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 51 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 61 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 71 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 14 09          	vstored \[rcx\]\{k1\},v1\{a\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 20 01 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}
[ 	]*[a-f0-9]+:	d6 20 09 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{float16\}
[ 	]*[a-f0-9]+:	d6 20 11 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 20 21 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 20 29 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 20 31 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 20 39 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 20 41 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 20 49 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 20 51 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 20 59 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 20 61 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 20 69 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 20 71 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 20 79 14 09          	vstored \[rcx\]\{k1\},v1\{dcba\}\{sint16i\}
[ 	]*[a-f0-9]+:	66 d6 80 01 14 09       	vstoreq \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	66 d6 00 01 14 09       	vstoreq \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	66 d6 00 01 14 09       	vstoreq \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	66 d6 10 01 14 09       	vstoreq \[rcx\]\{k1\},v1\{a\}
[ 	]*[a-f0-9]+:	66 d6 20 01 14 09       	vstoreq \[rcx\]\{k1\},v1\{dcba\}
[ 	]*[a-f0-9]+:	66 d6 80 01 13 09       	vloadunpackhq v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 13 09       	vloadunpackhq v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	66 d6 80 01 12 09       	vloadunpacklq v1\{k1\},\[rcx\]\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 12 09       	vloadunpacklq v1\{k1\},\[rcx\]
[ 	]*[a-f0-9]+:	d6 80 01 17 09          	vpackstorehd \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 00 01 17 09          	vpackstorehd \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 00 09 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 00 11 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 21 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 17 09          	vpackstorehd \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 80 01 16 09          	vpackstoreld \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 00 01 16 09          	vpackstoreld \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 00 09 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 00 11 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 00 21 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 00 29 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 00 31 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 00 39 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 00 41 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 00 49 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 00 51 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 00 59 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 00 61 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 00 69 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 00 71 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 00 79 16 09          	vpackstoreld \[rcx\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	66 d6 80 01 17 09       	vpackstorehq \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	66 d6 00 01 17 09       	vpackstorehq \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	66 d6 80 01 16 09       	vpackstorelq \[rcx\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	66 d6 00 01 16 09       	vpackstorelq \[rcx\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 90 01 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\},v1
[ 	]*[a-f0-9]+:	d6 10 01 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1
[ 	]*[a-f0-9]+:	d6 10 09 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{float16\}
[ 	]*[a-f0-9]+:	d6 10 11 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 21 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 29 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 31 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 41 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 49 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 51 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 61 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 71 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 1c 8c 55 99 00 00 00 	vscatterd \[rbp\+v10\*2\+0x99\]\{k1\},v1\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 1d 84 55 99 00 00 00 	vscatterpfd \[rbp\+v10\*2\+0x99\]\{k1\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 1f 84 55 99 00 00 00 	vscatterpf2d \[rbp\+v10\*2\+0x99\]\{k1\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{nt\}
[ 	]*[a-f0-9]+:	d6 10 01 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]
[ 	]*[a-f0-9]+:	d6 10 09 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float16\}
[ 	]*[a-f0-9]+:	d6 10 19 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 21 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 29 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 31 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 39 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 41 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 49 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 51 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 59 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 61 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 69 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 71 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 81 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 89 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 91 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 99 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a1 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 a9 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b1 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 10 b1 18 8c 55 99 00 00 00 	vgatherd v1\{k1\},\[rbp\+v10\*2\+0x99\]\{float10C\}
[ 	]*[a-f0-9]+:	d6 90 01 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 10 b2 19 84 55 99 00 00 00 	vgatherpfd \[rbp\+v10\*2\+0x99\]\{k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 90 01 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 10 b2 1b 84 55 99 00 00 00 	vgatherpf2d \[rbp\+v10\*2\+0x99\]\{k2\}\{float10C\}
[ 	]*[a-f0-9]+:	66 62 f4 c3             	bitinterleave11 bx,ax
[ 	]*[a-f0-9]+:	66 62 94 ca             	bitinterleave11 r10w,r9w
[ 	]*[a-f0-9]+:	66 62 f5 c3             	bitinterleave21 bx,ax
[ 	]*[a-f0-9]+:	66 62 95 ca             	bitinterleave21 r10w,r9w
[ 	]*[a-f0-9]+:	66 62 f2 c3             	bsff   bx,ax
[ 	]*[a-f0-9]+:	66 62 92 ca             	bsff   r10w,r9w
[ 	]*[a-f0-9]+:	66 62 f0 c3             	bsfi   bx,ax
[ 	]*[a-f0-9]+:	66 62 90 ca             	bsfi   r10w,r9w
[ 	]*[a-f0-9]+:	66 62 f3 c3             	bsrf   bx,ax
[ 	]*[a-f0-9]+:	66 62 93 ca             	bsrf   r10w,r9w
[ 	]*[a-f0-9]+:	66 62 f1 c3             	bsri   bx,ax
[ 	]*[a-f0-9]+:	66 62 91 ca             	bsri   r10w,r9w
[ 	]*[a-f0-9]+:	66 62 f8 c3             	countbits bx,ax
[ 	]*[a-f0-9]+:	66 62 98 ca             	countbits r10w,r9w
[ 	]*[a-f0-9]+:	66 62 f9 c3             	quadmask16 bx,ax
[ 	]*[a-f0-9]+:	66 62 99 ca             	quadmask16 r10w,r9w
[ 	]*[a-f0-9]+:	62 f4 c3                	bitinterleave11 ebx,eax
[ 	]*[a-f0-9]+:	62 94 ca                	bitinterleave11 r10d,r9d
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 ebx,eax
[ 	]*[a-f0-9]+:	62 95 ca                	bitinterleave21 r10d,r9d
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   ebx,eax
[ 	]*[a-f0-9]+:	62 92 ca                	bsff   r10d,r9d
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   ebx,eax
[ 	]*[a-f0-9]+:	62 90 ca                	bsfi   r10d,r9d
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   ebx,eax
[ 	]*[a-f0-9]+:	62 93 ca                	bsrf   r10d,r9d
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   ebx,eax
[ 	]*[a-f0-9]+:	62 91 ca                	bsri   r10d,r9d
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits ebx,eax
[ 	]*[a-f0-9]+:	62 98 ca                	countbits r10d,r9d
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 ebx,eax
[ 	]*[a-f0-9]+:	62 99 ca                	quadmask16 r10d,r9d
[ 	]*[a-f0-9]+:	62 e4 c3                	bitinterleave11 rbx,rax
[ 	]*[a-f0-9]+:	62 84 cf                	bitinterleave11 r15,r9
[ 	]*[a-f0-9]+:	62 e5 c3                	bitinterleave21 rbx,rax
[ 	]*[a-f0-9]+:	62 85 cf                	bitinterleave21 r15,r9
[ 	]*[a-f0-9]+:	62 e2 c3                	bsff   rbx,rax
[ 	]*[a-f0-9]+:	62 82 cf                	bsff   r15,r9
[ 	]*[a-f0-9]+:	62 e0 c3                	bsfi   rbx,rax
[ 	]*[a-f0-9]+:	62 80 cf                	bsfi   r15,r9
[ 	]*[a-f0-9]+:	62 e3 c3                	bsrf   rbx,rax
[ 	]*[a-f0-9]+:	62 83 cf                	bsrf   r15,r9
[ 	]*[a-f0-9]+:	62 e1 c3                	bsri   rbx,rax
[ 	]*[a-f0-9]+:	62 81 cf                	bsri   r15,r9
[ 	]*[a-f0-9]+:	62 e8 c3                	countbits rbx,rax
[ 	]*[a-f0-9]+:	62 88 cf                	countbits r15,r9
[ 	]*[a-f0-9]+:	62 e9 c3                	quadmask16 rbx,rax
[ 	]*[a-f0-9]+:	62 89 cf                	quadmask16 r15,r9
[ 	]*[a-f0-9]+:	62 eb 01                	clevict1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 01                	clevict1 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 09                	clevict2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 09                	clevict2 BYTE PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 cb 39                	vprefetch1 \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 cb 39                	vprefetch1 \[rcx\],0x7
[ 	]*[a-f0-9]+:	62 db 11                	vprefetch2 \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 db 11                	vprefetch2 \[rcx\],0x2
[ 	]*[a-f0-9]+:	62 eb 11                	ldvxcsr DWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 11                	ldvxcsr DWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 19                	stvxcsr DWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	62 eb 19                	stvxcsr DWORD PTR \[rcx\]
[ 	]*[a-f0-9]+:	66 62 df c0             	delay  ax
[ 	]*[a-f0-9]+:	66 62 9f c1             	delay  r9w
[ 	]*[a-f0-9]+:	66 62 df c8             	spflt  ax
[ 	]*[a-f0-9]+:	66 62 9f c9             	spflt  r9w
[ 	]*[a-f0-9]+:	62 df c0                	delay  eax
[ 	]*[a-f0-9]+:	62 9f c1                	delay  r9d
[ 	]*[a-f0-9]+:	62 df c8                	spflt  eax
[ 	]*[a-f0-9]+:	62 9f c9                	spflt  r9d
[ 	]*[a-f0-9]+:	62 cf c0                	delay  rax
[ 	]*[a-f0-9]+:	62 8f c1                	delay  r9
[ 	]*[a-f0-9]+:	62 cf c8                	spflt  rax
[ 	]*[a-f0-9]+:	62 8f c9                	spflt  r9
[ 	]*[a-f0-9]+:	d6 00 50 04 c0          	delay  0xa
[ 	]*[a-f0-9]+:	66 62 f6 c3 0b 00 02 1f 	insertfield bx,ax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	66 62 96 ca 0b 00 02 1f 	insertfield r10w,r9w,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	66 62 f7 c3 0b 00 02 1f 	rotatefield bx,ax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	66 62 97 ca 0b 00 02 1f 	rotatefield r10w,r9w,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 f6 c3 0b 00 02 1f    	insertfield ebx,eax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 96 ca 0b 00 02 1f    	insertfield r10d,r9d,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 f7 c3 0b 00 02 1f    	rotatefield ebx,eax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 97 ca 0b 00 02 1f    	rotatefield r10d,r9d,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 e6 c3 0b 00 02 1f    	insertfield rbx,rax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 86 cf 0b 00 02 1f    	insertfield r15,r9,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 e7 c3 0b 00 02 1f    	rotatefield rbx,rax,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	62 87 cf 0b 00 02 1f    	rotatefield r15,r9,0xb,0x2,0x1f
[ 	]*[a-f0-9]+:	d6 17 11 45 d6          	vadcpi v10\{k1\},k2,v30\{cdab\}
[ 	]*[a-f0-9]+:	66 d6 67 a1 48 d6       	vaddnpd v10\{k1\},v20,v30\{dddd\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 78 56 34 12 	vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0x12345678
[ 	]*[a-f0-9]+:	d6 04 11 45 55 00       	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbp\+0x0\]
[ 	]*[a-f0-9]+:	d6 04 11 45 95 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbp\+0x99\]
[ 	]*[a-f0-9]+:	d6 05 11 45 97 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r15\+0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 15 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rip\+0x99\]        # 65b8 <_start\+0x65b8>
[ 	]*[a-f0-9]+:	d6 04 11 45 94 24 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rsp\+0x99\]
[ 	]*[a-f0-9]+:	d6 05 11 45 94 24 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r12\+0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0xffffffffffffff67
[ 	]*[a-f0-9]+:	d6 04 11 45 14 65 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[riz\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 94 23 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 94 63 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbx\+riz\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 07 11 45 94 bc 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r12\+r15\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 07 11 45 94 f8 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[r8\+r15\*8-0x99\]
[ 	]*[a-f0-9]+:	d6 06 11 45 94 a5 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rbp\+r12\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 06 11 45 94 2c 67 ff ff ff 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[rsp\+r13\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 00 00 00 00 	vgatherd v10\{k1\},\[v1\*1\+0x0\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 00 00 00 00 	vgatherd v10\{k1\},\[v10\*1\+0x0\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 67 ff ff ff 	vgatherd v10\{k1\},\[v1\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 67 ff ff ff 	vgatherd v10\{k1\},\[v10\*1-0x99\]
[ 	]*[a-f0-9]+:	d6 04 01 18 14 08       	vgatherd v10\{k1\},\[rax\+v1\*1\]
[ 	]*[a-f0-9]+:	d6 14 01 18 14 10       	vgatherd v10\{k1\},\[rax\+v10\*1\]
[ 	]*[a-f0-9]+:	d6 04 01 18 94 48 67 ff ff ff 	vgatherd v10\{k1\},\[rax\+v1\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 14 01 18 94 50 67 ff ff ff 	vgatherd v10\{k1\},\[rax\+v10\*2-0x99\]
[ 	]*[a-f0-9]+:	d6 05 01 18 94 8c 67 ff ff ff 	vgatherd v10\{k1\},\[r12\+v1\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 15 01 18 94 94 67 ff ff ff 	vgatherd v10\{k1\},\[r12\+v10\*4-0x99\]
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 34 12 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0x1234
[ 	]*[a-f0-9]+:	67 d6 04 11 45 55 00    	vadcpi v10\{k1\},k2,ZMMWORD PTR \[ebp\+0x0\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 95 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[ebp\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 25 99 00 00 00 	addr32 vadcpi v10\{k1\},k2,ZMMWORD PTR ds:0x99
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 65 99 00 00 00 	addr32 vadcpi v10\{k1\},k2,ZMMWORD PTR \[eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 20 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 60 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[eax\+eiz\*2\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 98 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[eax\+ebx\*4\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 cc 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[esp\+ecx\*8\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 15 99 00 00 00 	vadcpi v10\{k1\},k2,ZMMWORD PTR \[ebp\+edx\*1\+0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 14 08    	vgatherd v10\{k1\},\[eax\+v1\*1\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 14 10    	vgatherd v10\{k1\},\[eax\+v10\*1\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 14 08    	vgatherd v10\{k1\},\[eax\+v1\*1\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 14 10    	vgatherd v10\{k1\},\[eax\+v10\*1\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 48 67 ff ff ff 	vgatherd v10\{k1\},\[eax\+v1\*2-0x99\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 50 67 ff ff ff 	vgatherd v10\{k1\},\[eax\+v10\*2-0x99\]
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 8b 67 ff ff ff 	vgatherd v10\{k1\},\[ebx\+v1\*4-0x99\]
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 93 67 ff ff ff 	vgatherd v10\{k1\},\[ebx\+v10\*4-0x99\]
#pass

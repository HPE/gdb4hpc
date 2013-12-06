#as: -march=l1om
#objdump: -dw
#name: L1OMni ops

.*: +file format .*


Disassembly of section .text:

0+ <_start>:
[ 	]*[a-f0-9]+:	62 cd ca                	vkand  %k1,%k2
[ 	]*[a-f0-9]+:	62 dd ca                	vkandn %k1,%k2
[ 	]*[a-f0-9]+:	62 ed ca                	vkandnr %k1,%k2
[ 	]*[a-f0-9]+:	62 fc ca                	vkmov  %k1,%k2
[ 	]*[a-f0-9]+:	62 fb ca                	vkmovlhb %k1,%k2
[ 	]*[a-f0-9]+:	62 fd ca                	vknot  %k1,%k2
[ 	]*[a-f0-9]+:	62 ce ca                	vkor   %k1,%k2
[ 	]*[a-f0-9]+:	62 de ca                	vkortest %k1,%k2
[ 	]*[a-f0-9]+:	62 ef ca                	vkswapb %k1,%k2
[ 	]*[a-f0-9]+:	62 fe ca                	vkxnor %k1,%k2
[ 	]*[a-f0-9]+:	62 ee ca                	vkxor  %k1,%k2
[ 	]*[a-f0-9]+:	66 62 ec c2             	vkmov  %ax,%k2
[ 	]*[a-f0-9]+:	66 62 cc ca             	vkmov  %r9w,%k2
[ 	]*[a-f0-9]+:	62 ec c2                	vkmov  %eax,%k2
[ 	]*[a-f0-9]+:	62 cc ca                	vkmov  %r9d,%k2
[ 	]*[a-f0-9]+:	66 62 dc c8             	vkmov  %k1,%ax
[ 	]*[a-f0-9]+:	66 62 9c c9             	vkmov  %k1,%r9w
[ 	]*[a-f0-9]+:	62 dc c8                	vkmov  %k1,%eax
[ 	]*[a-f0-9]+:	62 9c c9                	vkmov  %k1,%r9d
[ 	]*[a-f0-9]+:	d6 00 11 45 cb          	vadcpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 45 cb          	vadcpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 45 cb          	vadcpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 45 cb          	vadcpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 45 cb          	vadcpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 45 cb          	vadcpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 45 cb          	vadcpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 45 cb          	vadcpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 45 09          	vadcpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 45 09          	vadcpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 45 09          	vadcpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 45 09          	vadcpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 45 09          	vadcpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 45 09          	vadcpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 45 09          	vadcpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 43 cb          	vaddsetcpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 43 cb          	vaddsetcpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 43 cb          	vaddsetcpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 43 cb          	vaddsetcpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 43 cb          	vaddsetcpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 43 cb          	vaddsetcpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 43 cb          	vaddsetcpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 43 cb          	vaddsetcpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 43 09          	vaddsetcpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 43 09          	vaddsetcpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 43 09          	vaddsetcpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 43 09          	vaddsetcpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 43 09          	vaddsetcpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 43 09          	vaddsetcpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 43 09          	vaddsetcpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 55 cb          	vsbbpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 55 cb          	vsbbpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 55 cb          	vsbbpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 55 cb          	vsbbpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 55 cb          	vsbbpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 55 cb          	vsbbpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 55 cb          	vsbbpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 55 cb          	vsbbpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 55 09          	vsbbpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 55 09          	vsbbpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 55 09          	vsbbpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 55 09          	vsbbpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 55 09          	vsbbpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 55 09          	vsbbpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 55 09          	vsbbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5d cb          	vsbbrpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5d cb          	vsbbrpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5d cb          	vsbbrpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 5d cb          	vsbbrpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5d cb          	vsbbrpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5d cb          	vsbbrpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5d cb          	vsbbrpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5d cb          	vsbbrpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 5d 09          	vsbbrpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5d 09          	vsbbrpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5d 09          	vsbbrpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5d 09          	vsbbrpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5d 09          	vsbbrpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5d 09          	vsbbrpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5d 09          	vsbbrpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5b cb          	vsubrsetbpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5b cb          	vsubrsetbpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5b cb          	vsubrsetbpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 5b cb          	vsubrsetbpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5b cb          	vsubrsetbpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5b cb          	vsubrsetbpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5b cb          	vsubrsetbpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5b cb          	vsubrsetbpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 5b 09          	vsubrsetbpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5b 09          	vsubrsetbpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5b 09          	vsubrsetbpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5b 09          	vsubrsetbpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5b 09          	vsubrsetbpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5b 09          	vsubrsetbpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5b 09          	vsubrsetbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 53 cb          	vsubsetbpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 53 cb          	vsubsetbpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 53 cb          	vsubsetbpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 53 cb          	vsubsetbpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 53 cb          	vsubsetbpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 53 cb          	vsubsetbpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 53 cb          	vsubsetbpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 53 cb          	vsubsetbpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 53 09          	vsubsetbpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 53 09          	vsubsetbpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 53 09          	vsubsetbpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 53 09          	vsubsetbpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 53 09          	vsubsetbpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 53 09          	vsubsetbpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 53 09          	vsubsetbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 48 cb          	vaddnps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 48 cb          	vaddnps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 48 cb          	vaddnps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 48 cb          	vaddnps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 48 cb          	vaddnps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 48 cb          	vaddnps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 48 cb          	vaddnps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 48 cb          	vaddnps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 48 09          	vaddnps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 48 09          	vaddnps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 48 09          	vaddnps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 48 09          	vaddnps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 48 09          	vaddnps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 48 09          	vaddnps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 48 09          	vaddnps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 40 cb          	vaddps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 40 cb          	vaddps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 40 cb          	vaddps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 40 cb          	vaddps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 40 cb          	vaddps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 40 cb          	vaddps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 40 cb          	vaddps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 40 cb          	vaddps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 40 09          	vaddps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 40 09          	vaddps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 40 09          	vaddps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 40 09          	vaddps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 40 09          	vaddps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 40 09          	vaddps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 40 09          	vaddps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 46 cb          	vaddsetsps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 46 cb          	vaddsetsps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 46 cb          	vaddsetsps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 46 cb          	vaddsetsps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 46 cb          	vaddsetsps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 46 cb          	vaddsetsps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 46 cb          	vaddsetsps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 46 cb          	vaddsetsps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 46 09          	vaddsetsps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 46 09          	vaddsetsps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 46 09          	vaddsetsps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 46 09          	vaddsetsps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 46 09          	vaddsetsps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 46 09          	vaddsetsps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 46 09          	vaddsetsps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 86 cb          	vclampzps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 86 cb          	vclampzps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 86 cb          	vclampzps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 86 cb          	vclampzps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 86 cb          	vclampzps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 86 cb          	vclampzps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 86 cb          	vclampzps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 86 cb          	vclampzps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 86 09          	vclampzps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 86 09          	vclampzps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 86 09          	vclampzps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 86 09          	vclampzps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 86 09          	vclampzps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 86 09          	vclampzps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 86 09          	vclampzps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 64 cb          	vmadd132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 64 cb          	vmadd132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 64 cb          	vmadd132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 64 cb          	vmadd132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 64 cb          	vmadd132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 64 cb          	vmadd132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 64 cb          	vmadd132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 64 cb          	vmadd132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 64 09          	vmadd132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 64 09          	vmadd132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 64 09          	vmadd132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 64 09          	vmadd132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 64 09          	vmadd132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 64 09          	vmadd132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 64 09          	vmadd132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 62 cb          	vmadd213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 62 cb          	vmadd213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 62 cb          	vmadd213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 62 cb          	vmadd213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 62 cb          	vmadd213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 62 cb          	vmadd213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 62 cb          	vmadd213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 62 cb          	vmadd213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 62 09          	vmadd213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 62 09          	vmadd213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 62 09          	vmadd213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 62 09          	vmadd213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 62 09          	vmadd213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 62 09          	vmadd213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 62 09          	vmadd213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 60 cb          	vmadd231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 60 cb          	vmadd231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 60 cb          	vmadd231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 60 cb          	vmadd231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 60 cb          	vmadd231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 60 cb          	vmadd231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 60 cb          	vmadd231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 60 cb          	vmadd231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 60 09          	vmadd231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 60 09          	vmadd231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 60 09          	vmadd231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 60 09          	vmadd231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 60 09          	vmadd231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 60 09          	vmadd231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 60 09          	vmadd231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 66 cb          	vmadd233ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 66 cb          	vmadd233ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 66 cb          	vmadd233ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 66 cb          	vmadd233ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 66 cb          	vmadd233ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 66 cb          	vmadd233ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 66 cb          	vmadd233ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 66 cb          	vmadd233ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 66 09          	vmadd233ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 66 09          	vmadd233ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 66 09          	vmadd233ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 66 09          	vmadd233ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 66 09          	vmadd233ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 66 09          	vmadd233ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 66 09          	vmadd233ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 74 cb          	vmaddn132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 74 cb          	vmaddn132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 74 cb          	vmaddn132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 74 cb          	vmaddn132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 74 cb          	vmaddn132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 74 cb          	vmaddn132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 74 cb          	vmaddn132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 74 cb          	vmaddn132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 74 09          	vmaddn132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 74 09          	vmaddn132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 74 09          	vmaddn132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 74 09          	vmaddn132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 74 09          	vmaddn132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 74 09          	vmaddn132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 74 09          	vmaddn132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 72 cb          	vmaddn213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 72 cb          	vmaddn213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 72 cb          	vmaddn213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 72 cb          	vmaddn213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 72 cb          	vmaddn213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 72 cb          	vmaddn213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 72 cb          	vmaddn213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 72 cb          	vmaddn213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 72 09          	vmaddn213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 72 09          	vmaddn213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 72 09          	vmaddn213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 72 09          	vmaddn213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 72 09          	vmaddn213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 72 09          	vmaddn213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 72 09          	vmaddn213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 70 cb          	vmaddn231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 70 cb          	vmaddn231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 70 cb          	vmaddn231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 70 cb          	vmaddn231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 70 cb          	vmaddn231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 70 cb          	vmaddn231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 70 cb          	vmaddn231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 70 cb          	vmaddn231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 70 09          	vmaddn231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 70 09          	vmaddn231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 70 09          	vmaddn231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 70 09          	vmaddn231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 70 09          	vmaddn231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 70 09          	vmaddn231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 70 09          	vmaddn231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9e cb          	vmaxabsps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9e cb          	vmaxabsps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9e cb          	vmaxabsps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 9e cb          	vmaxabsps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9e cb          	vmaxabsps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9e cb          	vmaxabsps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9e cb          	vmaxabsps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9e cb          	vmaxabsps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 9e 09          	vmaxabsps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9e 09          	vmaxabsps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9e 09          	vmaxabsps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9e 09          	vmaxabsps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9e 09          	vmaxabsps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9e 09          	vmaxabsps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9e 09          	vmaxabsps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 88 cb          	vmaxps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 88 cb          	vmaxps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 88 cb          	vmaxps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 88 cb          	vmaxps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 88 cb          	vmaxps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 88 cb          	vmaxps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 88 cb          	vmaxps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 88 cb          	vmaxps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 88 09          	vmaxps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 88 09          	vmaxps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 88 09          	vmaxps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 88 09          	vmaxps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 88 09          	vmaxps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 88 09          	vmaxps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 88 09          	vmaxps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8c cb          	vminps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8c cb          	vminps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8c cb          	vminps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8c cb          	vminps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8c cb          	vminps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8c cb          	vminps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8c cb          	vminps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8c cb          	vminps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8c 09          	vminps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8c 09          	vminps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8c 09          	vminps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8c 09          	vminps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8c 09          	vminps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8c 09          	vminps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8c 09          	vminps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6c cb          	vmsub132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6c cb          	vmsub132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6c cb          	vmsub132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 6c cb          	vmsub132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6c cb          	vmsub132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6c cb          	vmsub132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6c cb          	vmsub132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6c cb          	vmsub132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 6c 09          	vmsub132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6c 09          	vmsub132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6c 09          	vmsub132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6c 09          	vmsub132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6c 09          	vmsub132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6c 09          	vmsub132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6c 09          	vmsub132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6a cb          	vmsub213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6a cb          	vmsub213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6a cb          	vmsub213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 6a cb          	vmsub213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6a cb          	vmsub213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6a cb          	vmsub213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6a cb          	vmsub213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6a cb          	vmsub213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 6a 09          	vmsub213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6a 09          	vmsub213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6a 09          	vmsub213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6a 09          	vmsub213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6a 09          	vmsub213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6a 09          	vmsub213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6a 09          	vmsub213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 68 cb          	vmsub231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 68 cb          	vmsub231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 68 cb          	vmsub231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 68 cb          	vmsub231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 68 cb          	vmsub231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 68 cb          	vmsub231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 68 cb          	vmsub231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 68 cb          	vmsub231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 68 09          	vmsub231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 68 09          	vmsub231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 68 09          	vmsub231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 68 09          	vmsub231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 68 09          	vmsub231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 68 09          	vmsub231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 68 09          	vmsub231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7c cb          	vmsubr132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7c cb          	vmsubr132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7c cb          	vmsubr132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 7c cb          	vmsubr132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7c cb          	vmsubr132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7c cb          	vmsubr132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7c cb          	vmsubr132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7c cb          	vmsubr132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 7c 09          	vmsubr132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7c 09          	vmsubr132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7c 09          	vmsubr132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7c 09          	vmsubr132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7c 09          	vmsubr132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7c 09          	vmsubr132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7c 09          	vmsubr132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7a cb          	vmsubr213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7a cb          	vmsubr213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7a cb          	vmsubr213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 7a cb          	vmsubr213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7a cb          	vmsubr213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7a cb          	vmsubr213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7a cb          	vmsubr213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7a cb          	vmsubr213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 7a 09          	vmsubr213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7a 09          	vmsubr213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7a 09          	vmsubr213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7a 09          	vmsubr213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7a 09          	vmsubr213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7a 09          	vmsubr213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7a 09          	vmsubr213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 78 cb          	vmsubr231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 78 cb          	vmsubr231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 78 cb          	vmsubr231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 78 cb          	vmsubr231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 78 cb          	vmsubr231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 78 cb          	vmsubr231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 78 cb          	vmsubr231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 78 cb          	vmsubr231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 78 09          	vmsubr231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 78 09          	vmsubr231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 78 09          	vmsubr231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 78 09          	vmsubr231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 78 09          	vmsubr231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 78 09          	vmsubr231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 78 09          	vmsubr231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7e cb          	vmsubr23c1ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7e cb          	vmsubr23c1ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7e cb          	vmsubr23c1ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 7e cb          	vmsubr23c1ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7e cb          	vmsubr23c1ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7e cb          	vmsubr23c1ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7e cb          	vmsubr23c1ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7e cb          	vmsubr23c1ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 7e 09          	vmsubr23c1ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7e 09          	vmsubr23c1ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7e 09          	vmsubr23c1ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7e 09          	vmsubr23c1ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7e 09          	vmsubr23c1ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7e 09          	vmsubr23c1ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7e 09          	vmsubr23c1ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 80 cb          	vmulps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 80 cb          	vmulps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 80 cb          	vmulps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 80 cb          	vmulps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 80 cb          	vmulps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 80 cb          	vmulps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 80 cb          	vmulps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 80 cb          	vmulps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 80 09          	vmulps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 80 09          	vmulps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 80 09          	vmulps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 80 09          	vmulps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 80 09          	vmulps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 80 09          	vmulps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 80 09          	vmulps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 cb          	vrcprefineps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 c2 cb          	vrcprefineps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 cb          	vrcprefineps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 c2 cb          	vrcprefineps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 cb          	vrcprefineps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 cb          	vrcprefineps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 cb          	vrcprefineps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 cb          	vrcprefineps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 c2 09          	vrcprefineps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 c2 09          	vrcprefineps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 09          	vrcprefineps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 09          	vrcprefineps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 09          	vrcprefineps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 09          	vrcprefineps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 09          	vrcprefineps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 50 cb          	vsubps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 50 cb          	vsubps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 50 cb          	vsubps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 50 cb          	vsubps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 50 cb          	vsubps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 50 cb          	vsubps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 50 cb          	vsubps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 50 cb          	vsubps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 50 09          	vsubps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 50 09          	vsubps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 50 09          	vsubps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 50 09          	vsubps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 50 09          	vsubps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 50 09          	vsubps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 50 09          	vsubps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 58 cb          	vsubrps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 58 cb          	vsubrps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 58 cb          	vsubrps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 58 cb          	vsubrps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 58 cb          	vsubrps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 58 cb          	vsubrps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 58 cb          	vsubrps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 58 cb          	vsubrps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 58 09          	vsubrps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 58 09          	vsubrps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 58 09          	vsubrps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 58 09          	vsubrps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 58 09          	vsubrps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 58 09          	vsubrps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 58 09          	vsubrps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 cb       	vaddnpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 48 cb       	vaddnpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 cb       	vaddnpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 48 cb       	vaddnpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 48 cb       	vaddnpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 48 cb       	vaddnpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 48 cb       	vaddnpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 48 cb       	vaddnpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 48 09       	vaddnpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 48 09       	vaddnpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 09       	vaddnpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 cb       	vaddpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 40 cb       	vaddpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 cb       	vaddpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 40 cb       	vaddpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 40 cb       	vaddpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 40 cb       	vaddpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 40 cb       	vaddpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 40 cb       	vaddpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 40 09       	vaddpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 40 09       	vaddpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 09       	vaddpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 cb       	vmadd132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 64 cb       	vmadd132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 cb       	vmadd132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 64 cb       	vmadd132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 64 cb       	vmadd132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 64 cb       	vmadd132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 64 cb       	vmadd132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 64 cb       	vmadd132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 64 09       	vmadd132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 64 09       	vmadd132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 09       	vmadd132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 cb       	vmadd213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 62 cb       	vmadd213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 cb       	vmadd213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 62 cb       	vmadd213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 62 cb       	vmadd213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 62 cb       	vmadd213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 62 cb       	vmadd213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 62 cb       	vmadd213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 62 09       	vmadd213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 62 09       	vmadd213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 09       	vmadd213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 cb       	vmadd231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 60 cb       	vmadd231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 cb       	vmadd231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 60 cb       	vmadd231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 60 cb       	vmadd231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 60 cb       	vmadd231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 60 cb       	vmadd231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 60 cb       	vmadd231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 60 09       	vmadd231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 60 09       	vmadd231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 09       	vmadd231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 cb       	vmaddn132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 74 cb       	vmaddn132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 cb       	vmaddn132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 74 cb       	vmaddn132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 74 cb       	vmaddn132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 74 cb       	vmaddn132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 74 cb       	vmaddn132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 74 cb       	vmaddn132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 74 09       	vmaddn132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 74 09       	vmaddn132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 09       	vmaddn132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 cb       	vmaddn213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 72 cb       	vmaddn213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 cb       	vmaddn213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 72 cb       	vmaddn213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 72 cb       	vmaddn213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 72 cb       	vmaddn213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 72 cb       	vmaddn213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 72 cb       	vmaddn213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 72 09       	vmaddn213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 72 09       	vmaddn213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 09       	vmaddn213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 cb       	vmaddn231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 70 cb       	vmaddn231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 cb       	vmaddn231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 70 cb       	vmaddn231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 70 cb       	vmaddn231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 70 cb       	vmaddn231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 70 cb       	vmaddn231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 70 cb       	vmaddn231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 70 09       	vmaddn231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 70 09       	vmaddn231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 09       	vmaddn231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 cb       	vmaxpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 88 cb       	vmaxpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 cb       	vmaxpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 88 cb       	vmaxpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 88 cb       	vmaxpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 88 cb       	vmaxpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 88 cb       	vmaxpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 88 cb       	vmaxpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 88 09       	vmaxpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 88 09       	vmaxpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 09       	vmaxpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c cb       	vminpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 8c cb       	vminpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c cb       	vminpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 8c cb       	vminpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 8c cb       	vminpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 8c cb       	vminpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 8c cb       	vminpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 8c cb       	vminpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 8c 09       	vminpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 8c 09       	vminpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c 09       	vminpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c cb       	vmsub132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6c cb       	vmsub132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c cb       	vmsub132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6c cb       	vmsub132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6c cb       	vmsub132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6c cb       	vmsub132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6c cb       	vmsub132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6c cb       	vmsub132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6c 09       	vmsub132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6c 09       	vmsub132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c 09       	vmsub132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a cb       	vmsub213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6a cb       	vmsub213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a cb       	vmsub213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6a cb       	vmsub213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6a cb       	vmsub213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6a cb       	vmsub213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6a cb       	vmsub213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6a cb       	vmsub213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6a 09       	vmsub213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6a 09       	vmsub213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a 09       	vmsub213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 cb       	vmsub231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 68 cb       	vmsub231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 cb       	vmsub231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 68 cb       	vmsub231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 68 cb       	vmsub231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 68 cb       	vmsub231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 68 cb       	vmsub231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 68 cb       	vmsub231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 68 09       	vmsub231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 68 09       	vmsub231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 09       	vmsub231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c cb       	vmsubr132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7c cb       	vmsubr132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c cb       	vmsubr132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7c cb       	vmsubr132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7c cb       	vmsubr132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7c cb       	vmsubr132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7c cb       	vmsubr132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7c cb       	vmsubr132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7c 09       	vmsubr132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7c 09       	vmsubr132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c 09       	vmsubr132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a cb       	vmsubr213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7a cb       	vmsubr213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a cb       	vmsubr213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7a cb       	vmsubr213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7a cb       	vmsubr213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7a cb       	vmsubr213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7a cb       	vmsubr213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7a cb       	vmsubr213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7a 09       	vmsubr213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7a 09       	vmsubr213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a 09       	vmsubr213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 cb       	vmsubr231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 78 cb       	vmsubr231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 cb       	vmsubr231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 78 cb       	vmsubr231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 78 cb       	vmsubr231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 78 cb       	vmsubr231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 78 cb       	vmsubr231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 78 cb       	vmsubr231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 78 09       	vmsubr231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 78 09       	vmsubr231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 09       	vmsubr231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e cb       	vmsubr23c1pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7e cb       	vmsubr23c1pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e cb       	vmsubr23c1pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7e cb       	vmsubr23c1pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7e cb       	vmsubr23c1pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7e cb       	vmsubr23c1pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7e cb       	vmsubr23c1pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7e cb       	vmsubr23c1pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7e 09       	vmsubr23c1pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7e 09       	vmsubr23c1pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e 09       	vmsubr23c1pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 cb       	vmulpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 80 cb       	vmulpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 cb       	vmulpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 80 cb       	vmulpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 80 cb       	vmulpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 80 cb       	vmulpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 80 cb       	vmulpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 80 cb       	vmulpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 80 09       	vmulpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 80 09       	vmulpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 09       	vmulpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 cb       	vsubpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 50 cb       	vsubpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 cb       	vsubpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 50 cb       	vsubpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 50 cb       	vsubpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 50 cb       	vsubpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 50 cb       	vsubpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 50 cb       	vsubpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 50 09       	vsubpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 50 09       	vsubpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 09       	vsubpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 cb       	vsubrpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 58 cb       	vsubrpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 cb       	vsubrpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 58 cb       	vsubrpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 58 cb       	vsubrpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 58 cb       	vsubrpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 58 cb       	vsubrpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 58 cb       	vsubrpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 58 09       	vsubrpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 58 09       	vsubrpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 09       	vsubrpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 41 cb          	vaddpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 41 cb          	vaddpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 41 cb          	vaddpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 41 cb          	vaddpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 41 cb          	vaddpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 41 cb          	vaddpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 41 cb          	vaddpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 41 cb          	vaddpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 41 09          	vaddpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 41 09          	vaddpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 41 09          	vaddpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 41 09          	vaddpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 41 09          	vaddpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 41 09          	vaddpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 41 09          	vaddpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 47 cb          	vaddsetspi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 47 cb          	vaddsetspi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 47 cb          	vaddsetspi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 47 cb          	vaddsetspi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 47 cb          	vaddsetspi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 47 cb          	vaddsetspi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 47 cb          	vaddsetspi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 47 cb          	vaddsetspi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 47 09          	vaddsetspi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 47 09          	vaddsetspi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 47 09          	vaddsetspi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 47 09          	vaddsetspi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 47 09          	vaddsetspi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 47 09          	vaddsetspi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 47 09          	vaddsetspi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 cb          	vandnpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a4 cb          	vandnpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 cb          	vandnpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a4 cb          	vandnpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 cb          	vandnpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 cb          	vandnpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 cb          	vandnpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 cb          	vandnpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a4 09          	vandnpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a4 09          	vandnpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 09          	vandnpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 09          	vandnpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 09          	vandnpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 09          	vandnpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 09          	vandnpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 cb          	vandpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a5 cb          	vandpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 cb          	vandpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a5 cb          	vandpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 cb          	vandpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 cb          	vandpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 cb          	vandpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 cb          	vandpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a5 09          	vandpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a5 09          	vandpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 09          	vandpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 09          	vandpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 09          	vandpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 09          	vandpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 09          	vandpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 cb          	vbitinterleave11pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a8 cb          	vbitinterleave11pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 cb          	vbitinterleave11pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a8 cb          	vbitinterleave11pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 cb          	vbitinterleave11pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 cb          	vbitinterleave11pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 cb          	vbitinterleave11pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 cb          	vbitinterleave11pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a8 09          	vbitinterleave11pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a8 09          	vbitinterleave11pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 09          	vbitinterleave11pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 09          	vbitinterleave11pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 09          	vbitinterleave11pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 09          	vbitinterleave11pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 09          	vbitinterleave11pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 cb          	vbitinterleave21pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a9 cb          	vbitinterleave21pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 cb          	vbitinterleave21pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a9 cb          	vbitinterleave21pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 cb          	vbitinterleave21pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 cb          	vbitinterleave21pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 cb          	vbitinterleave21pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 cb          	vbitinterleave21pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a9 09          	vbitinterleave21pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a9 09          	vbitinterleave21pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 09          	vbitinterleave21pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 09          	vbitinterleave21pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 09          	vbitinterleave21pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 09          	vbitinterleave21pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 09          	vbitinterleave21pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 87 cb          	vclampzpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 87 cb          	vclampzpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 87 cb          	vclampzpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 87 cb          	vclampzpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 87 cb          	vclampzpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 87 cb          	vclampzpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 87 cb          	vclampzpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 87 cb          	vclampzpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 87 09          	vclampzpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 87 09          	vclampzpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 87 09          	vclampzpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 87 09          	vclampzpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 87 09          	vclampzpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 87 09          	vclampzpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 87 09          	vclampzpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 61 cb          	vmadd231pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 61 cb          	vmadd231pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 61 cb          	vmadd231pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 61 cb          	vmadd231pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 61 cb          	vmadd231pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 61 cb          	vmadd231pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 61 cb          	vmadd231pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 61 cb          	vmadd231pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 61 09          	vmadd231pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 61 09          	vmadd231pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 61 09          	vmadd231pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 61 09          	vmadd231pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 61 09          	vmadd231pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 61 09          	vmadd231pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 61 09          	vmadd231pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 67 cb          	vmadd233pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 67 cb          	vmadd233pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 67 cb          	vmadd233pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 67 cb          	vmadd233pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 67 cb          	vmadd233pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 67 cb          	vmadd233pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 67 cb          	vmadd233pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 67 cb          	vmadd233pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 67 09          	vmadd233pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 67 09          	vmadd233pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 67 09          	vmadd233pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 67 09          	vmadd233pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 67 09          	vmadd233pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 67 09          	vmadd233pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 67 09          	vmadd233pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 89 cb          	vmaxpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 89 cb          	vmaxpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 89 cb          	vmaxpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 89 cb          	vmaxpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 89 cb          	vmaxpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 89 cb          	vmaxpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 89 cb          	vmaxpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 89 cb          	vmaxpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 89 09          	vmaxpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 89 09          	vmaxpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 89 09          	vmaxpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 89 09          	vmaxpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 89 09          	vmaxpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 89 09          	vmaxpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 89 09          	vmaxpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8a cb          	vmaxpu %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8a cb          	vmaxpu %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8a cb          	vmaxpu %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8a cb          	vmaxpu %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8a cb          	vmaxpu %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8a cb          	vmaxpu %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8a cb          	vmaxpu %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8a cb          	vmaxpu %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8a 09          	vmaxpu \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8a 09          	vmaxpu \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8a 09          	vmaxpu \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8a 09          	vmaxpu \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8a 09          	vmaxpu \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8a 09          	vmaxpu \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8a 09          	vmaxpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8d cb          	vminpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8d cb          	vminpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8d cb          	vminpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8d cb          	vminpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8d cb          	vminpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8d cb          	vminpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8d cb          	vminpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8d cb          	vminpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8d 09          	vminpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8d 09          	vminpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8d 09          	vminpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8d 09          	vminpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8d 09          	vminpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8d 09          	vminpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8d 09          	vminpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8e cb          	vminpu %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8e cb          	vminpu %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8e cb          	vminpu %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8e cb          	vminpu %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8e cb          	vminpu %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8e cb          	vminpu %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8e cb          	vminpu %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8e cb          	vminpu %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8e 09          	vminpu \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8e 09          	vminpu \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8e 09          	vminpu \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8e 09          	vminpu \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8e 09          	vminpu \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8e 09          	vminpu \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8e 09          	vminpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 81 cb          	vmulhpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 81 cb          	vmulhpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 81 cb          	vmulhpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 81 cb          	vmulhpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 81 cb          	vmulhpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 81 cb          	vmulhpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 81 cb          	vmulhpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 81 cb          	vmulhpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 81 09          	vmulhpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 81 09          	vmulhpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 81 09          	vmulhpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 81 09          	vmulhpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 81 09          	vmulhpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 81 09          	vmulhpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 81 09          	vmulhpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 82 cb          	vmulhpu %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 82 cb          	vmulhpu %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 82 cb          	vmulhpu %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 82 cb          	vmulhpu %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 82 cb          	vmulhpu %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 82 cb          	vmulhpu %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 82 cb          	vmulhpu %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 82 cb          	vmulhpu %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 82 09          	vmulhpu \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 82 09          	vmulhpu \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 82 09          	vmulhpu \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 82 09          	vmulhpu \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 82 09          	vmulhpu \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 82 09          	vmulhpu \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 82 09          	vmulhpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 83 cb          	vmullpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 83 cb          	vmullpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 83 cb          	vmullpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 83 cb          	vmullpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 83 cb          	vmullpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 83 cb          	vmullpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 83 cb          	vmullpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 83 cb          	vmullpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 83 09          	vmullpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 83 09          	vmullpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 83 09          	vmullpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 83 09          	vmullpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 83 09          	vmullpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 83 09          	vmullpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 83 09          	vmullpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 cb          	vorpi  %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a6 cb          	vorpi  %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 cb          	vorpi  %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a6 cb          	vorpi  %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 cb          	vorpi  %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 cb          	vorpi  %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 cb          	vorpi  %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 cb          	vorpi  %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a6 09          	vorpi  \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a6 09          	vorpi  \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 09          	vorpi  \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 09          	vorpi  \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 09          	vorpi  \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 09          	vorpi  \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 09          	vorpi  \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 cb          	vscaleps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 b8 cb          	vscaleps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 cb          	vscaleps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 b8 cb          	vscaleps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 cb          	vscaleps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 cb          	vscaleps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 cb          	vscaleps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 cb          	vscaleps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 b8 09          	vscaleps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 b8 09          	vscaleps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 09          	vscaleps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 09          	vscaleps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 09          	vscaleps \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 09          	vscaleps \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 09          	vscaleps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 cb          	vsllpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a0 cb          	vsllpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 cb          	vsllpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a0 cb          	vsllpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 cb          	vsllpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 cb          	vsllpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 cb          	vsllpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 cb          	vsllpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a0 09          	vsllpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a0 09          	vsllpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 09          	vsllpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 09          	vsllpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 09          	vsllpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 09          	vsllpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 09          	vsllpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 cb          	vsrapi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a1 cb          	vsrapi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 cb          	vsrapi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a1 cb          	vsrapi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 cb          	vsrapi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 cb          	vsrapi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 cb          	vsrapi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 cb          	vsrapi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a1 09          	vsrapi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a1 09          	vsrapi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 09          	vsrapi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 09          	vsrapi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 09          	vsrapi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 09          	vsrapi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 09          	vsrapi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 cb          	vsrlpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a2 cb          	vsrlpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 cb          	vsrlpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a2 cb          	vsrlpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 cb          	vsrlpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 cb          	vsrlpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 cb          	vsrlpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 cb          	vsrlpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a2 09          	vsrlpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a2 09          	vsrlpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 09          	vsrlpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 09          	vsrlpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 09          	vsrlpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 09          	vsrlpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 09          	vsrlpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 51 cb          	vsubpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 51 cb          	vsubpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 51 cb          	vsubpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 51 cb          	vsubpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 51 cb          	vsubpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 51 cb          	vsubpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 51 cb          	vsubpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 51 cb          	vsubpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 51 09          	vsubpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 51 09          	vsubpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 51 09          	vsubpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 51 09          	vsubpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 51 09          	vsubpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 51 09          	vsubpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 51 09          	vsubpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 59 cb          	vsubrpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 59 cb          	vsubrpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 59 cb          	vsubrpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 59 cb          	vsubrpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 59 cb          	vsubrpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 59 cb          	vsubrpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 59 cb          	vsubrpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 59 cb          	vsubrpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 59 09          	vsubrpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 59 09          	vsubrpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 59 09          	vsubrpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 59 09          	vsubrpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 59 09          	vsubrpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 59 09          	vsubrpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 59 09          	vsubrpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 cb          	vxorpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a7 cb          	vxorpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 cb          	vxorpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a7 cb          	vxorpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 cb          	vxorpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 cb          	vxorpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 cb          	vxorpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 cb          	vxorpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a7 09          	vxorpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a7 09          	vxorpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 09          	vxorpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 09          	vxorpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 09          	vxorpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 09          	vxorpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 09          	vxorpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 12 30 cb          	vtestpi %v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 12 30 cb          	vtestpi %v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 20 12 30 cb          	vtestpi %v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 30 12 30 cb          	vtestpi %v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 40 12 30 cb          	vtestpi %v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 50 12 30 cb          	vtestpi %v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 60 12 30 cb          	vtestpi %v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 30 cb          	vtestpi %v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 30 09          	vtestpi \(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi \(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi \(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 12 30 09          	vtestpi \(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 20 12 30 09          	vtestpi \(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 40 12 30 09          	vtestpi \(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 50 12 30 09          	vtestpi \(%rcx\)\{sint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 60 12 30 09          	vtestpi \(%rcx\)\{uint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 30 09          	vtestpi \(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 cb       	vandnpq %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 cb       	vandnpq %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 cb       	vandnpq %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a4 cb       	vandnpq %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a4 cb       	vandnpq %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a4 cb       	vandnpq %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a4 cb       	vandnpq %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a4 cb       	vandnpq %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a4 09       	vandnpq \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 09       	vandnpq \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 09       	vandnpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 cb       	vandpq %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 cb       	vandpq %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 cb       	vandpq %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a5 cb       	vandpq %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a5 cb       	vandpq %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a5 cb       	vandpq %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a5 cb       	vandpq %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a5 cb       	vandpq %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a5 09       	vandpq \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 09       	vandpq \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 09       	vandpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 cb       	vorpq  %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 cb       	vorpq  %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 cb       	vorpq  %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a6 cb       	vorpq  %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a6 cb       	vorpq  %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a6 cb       	vorpq  %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a6 cb       	vorpq  %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a6 cb       	vorpq  %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a6 09       	vorpq  \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 09       	vorpq  \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 09       	vorpq  \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 cb       	vxorpq %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 cb       	vxorpq %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 cb       	vxorpq %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a7 cb       	vxorpq %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a7 cb       	vxorpq %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a7 cb       	vxorpq %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a7 cb       	vxorpq %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a7 cb       	vxorpq %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a7 09       	vxorpq \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 09       	vxorpq \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 09       	vxorpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 cb       	vcmppd \{unord\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 cb       	vcmppd \{unord\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 cb       	vcmppd \{unord\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 3c 12 20 cb       	vcmppd \{unord\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 4c 12 20 cb       	vcmppd \{unord\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 5c 12 20 cb       	vcmppd \{unord\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 6c 12 20 cb       	vcmppd \{unord\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 70 12 20 cb       	vcmppd \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 80 12 20 09       	vcmppd \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 74 12 20 cb       	vcmppd \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 84 12 20 09       	vcmppd \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 78 12 20 cb       	vcmppd \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 88 12 20 09       	vcmppd \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 8c 12 20 09       	vcmppd \{unord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 70 12 24 cb       	vcmppd \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 80 12 24 09       	vcmppd \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 74 12 24 cb       	vcmppd \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 84 12 24 09       	vcmppd \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 78 12 24 cb       	vcmppd \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 88 12 24 09       	vcmppd \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 7c 12 24 cb       	vcmppd \{ord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 8c 12 24 09       	vcmppd \{ord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 09       	vcmppd \{unord\},\(%rcx\)\{1to8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 09       	vcmppd \{unord\},\(%rcx\)\{4to8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 21 cb          	vcmppi \{lt\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 21 cb          	vcmppi \{lt\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 21 cb          	vcmppi \{lt\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 34 12 21 cb          	vcmppi \{lt\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 21 cb          	vcmppi \{lt\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 21 cb          	vcmppi \{lt\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 21 cb          	vcmppi \{lt\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 21 cb          	vcmppi \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 21 09          	vcmppi \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 21 09          	vcmppi \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 21 cb          	vcmppi \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 21 09          	vcmppi \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 25 cb          	vcmppi \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 25 09          	vcmppi \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 25 cb          	vcmppi \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 25 09          	vcmppi \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 25 cb          	vcmppi \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 25 09          	vcmppi \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 21 09          	vcmppi \{lt\},\(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 21 09          	vcmppi \{lt\},\(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 21 09          	vcmppi \{lt\},\(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 21 09          	vcmppi \{lt\},\(%rcx\)\{sint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 21 09          	vcmppi \{lt\},\(%rcx\)\{uint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 21 09          	vcmppi \{lt\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 22 cb          	vcmppu \{lt\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 22 cb          	vcmppu \{lt\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 22 cb          	vcmppu \{lt\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 34 12 22 cb          	vcmppu \{lt\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 22 cb          	vcmppu \{lt\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 22 cb          	vcmppu \{lt\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 22 cb          	vcmppu \{lt\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 22 cb          	vcmppu \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 22 09          	vcmppu \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 22 09          	vcmppu \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 22 cb          	vcmppu \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 22 09          	vcmppu \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 26 cb          	vcmppu \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 26 09          	vcmppu \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 26 cb          	vcmppu \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 26 09          	vcmppu \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 26 cb          	vcmppu \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 26 09          	vcmppu \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 22 09          	vcmppu \{lt\},\(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 22 09          	vcmppu \{lt\},\(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 22 09          	vcmppu \{lt\},\(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 22 09          	vcmppu \{lt\},\(%rcx\)\{sint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 22 09          	vcmppu \{lt\},\(%rcx\)\{uint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 22 09          	vcmppu \{lt\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 0c 12 20 cb          	vcmpps \{unord\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 1c 12 20 cb          	vcmpps \{unord\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 2c 12 20 cb          	vcmpps \{unord\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 3c 12 20 cb          	vcmpps \{unord\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 4c 12 20 cb          	vcmpps \{unord\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 5c 12 20 cb          	vcmpps \{unord\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 6c 12 20 cb          	vcmpps \{unord\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 20 cb          	vcmpps \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 20 09          	vcmpps \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 20 cb          	vcmpps \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 20 09          	vcmpps \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 20 cb          	vcmpps \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 20 09          	vcmpps \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 8c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 24 cb          	vcmpps \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 24 09          	vcmpps \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 24 cb          	vcmpps \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 24 09          	vcmpps \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 24 cb          	vcmpps \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 24 09          	vcmpps \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 24 cb          	vcmpps \{ord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 8c 12 24 09          	vcmpps \{ord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 1c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 2c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 4c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 5c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{unorm8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 6c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{float16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 00 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 d1 9b cb          	vcvtinsps2f11 \$0x2,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 d1 9b 09          	vcvtinsps2f11 \$0x2,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9a cb          	vcvtinsps2u10 \$0x2,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 49 91 cb       	vcvtpd2pi \$0x1,\{rd\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 49 91 09       	vcvtpd2pi \$0x1,\{rd\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 89 91 cb       	vcvtpd2pi \$0x1,\{ru\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 89 91 09       	vcvtpd2pi \$0x1,\{ru\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 c9 91 cb       	vcvtpd2pi \$0x1,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 c9 91 09       	vcvtpd2pi \$0x1,\{rz\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 c9 91 09       	vcvtpd2pi \$0x1,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 49 90 cb       	vcvtpd2ps \$0x1,\{rd\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 49 90 09       	vcvtpd2ps \$0x1,\{rd\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 89 90 cb       	vcvtpd2ps \$0x1,\{ru\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 89 90 09       	vcvtpd2ps \$0x1,\{ru\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 c9 90 cb       	vcvtpd2ps \$0x1,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 c9 90 09       	vcvtpd2ps \$0x1,\{rz\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 c9 90 09       	vcvtpd2ps \$0x1,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 49 92 cb       	vcvtpd2pu \$0x1,\{rd\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 49 92 09       	vcvtpd2pu \$0x1,\{rd\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 89 92 cb       	vcvtpd2pu \$0x1,\{ru\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 89 92 09       	vcvtpd2pu \$0x1,\{ru\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 c9 92 cb       	vcvtpd2pu \$0x1,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 c9 92 09       	vcvtpd2pu \$0x1,\{rz\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 c9 92 09       	vcvtpd2pu \$0x1,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 95 cb       	vcvtpi2pd \$0x1,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 95 cb       	vcvtpi2pd \$0x1,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 95 cb       	vcvtpi2pd \$0x1,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 95 cb       	vcvtpi2pd \$0x1,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 95 cb       	vcvtpi2pd \$0x1,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 95 cb       	vcvtpi2pd \$0x1,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 95 cb       	vcvtpi2pd \$0x1,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 95 cb       	vcvtpi2pd \$0x1,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 96 cb       	vcvtpu2pd \$0x1,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 96 cb       	vcvtpu2pd \$0x1,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 96 cb       	vcvtpu2pd \$0x1,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 96 cb       	vcvtpu2pd \$0x1,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 96 cb       	vcvtpu2pd \$0x1,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 96 cb       	vcvtpu2pd \$0x1,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 96 cb       	vcvtpu2pd \$0x1,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 96 cb       	vcvtpu2pd \$0x1,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 95 cb          	vcvtpi2ps \{.16b\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 95 cb          	vcvtpi2ps \{.0b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 95 cb          	vcvtpi2ps \{.4b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 95 cb          	vcvtpi2ps \{.5b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 95 cb          	vcvtpi2ps \{.8b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 95 cb          	vcvtpi2ps \{.24b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 95 cb          	vcvtpi2ps \{.31b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 95 cb          	vcvtpi2ps \{.32b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 96 cb          	vcvtpu2ps \{.16b\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 96 cb          	vcvtpu2ps \{.0b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 96 cb          	vcvtpu2ps \{.4b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 96 cb          	vcvtpu2ps \{.5b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 96 cb          	vcvtpu2ps \{.8b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 96 cb          	vcvtpu2ps \{.24b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 96 cb          	vcvtpu2ps \{.31b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 96 cb          	vcvtpu2ps \{.32b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 94 cb       	vcvtps2pd \$0x1,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 94 cb       	vcvtps2pd \$0x1,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 94 cb       	vcvtps2pd \$0x1,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 94 cb       	vcvtps2pd \$0x1,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 94 cb       	vcvtps2pd \$0x1,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 94 cb       	vcvtps2pd \$0x1,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 94 cb       	vcvtps2pd \$0x1,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 94 cb       	vcvtps2pd \$0x1,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 91 cb          	vcvtps2pi \{.0b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 91 cb          	vcvtps2pi \{.4b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 91 cb          	vcvtps2pi \{.5b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 91 cb          	vcvtps2pi \{.8b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 91 cb          	vcvtps2pi \{.24b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 91 cb          	vcvtps2pi \{.31b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 91 cb          	vcvtps2pi \{.32b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 92 cb          	vcvtps2pu \{.0b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 92 cb          	vcvtps2pu \{.4b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 92 cb          	vcvtps2pu \{.5b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 92 cb          	vcvtps2pu \{.8b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 92 cb          	vcvtps2pu \{.24b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 92 cb          	vcvtps2pu \{.31b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 92 cb          	vcvtps2pu \{.32b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 98 cb          	vroundps \{.16b\},\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 98 cb          	vroundps \{.0b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 98 cb          	vroundps \{.4b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 98 cb          	vroundps \{.5b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 98 cb          	vroundps \{.8b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 98 cb          	vroundps \{.24b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 98 cb          	vroundps \{.31b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 98 cb          	vroundps \{.32b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 9d cb          	vcvtps2srgb8 %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 9d cb          	vcvtps2srgb8 %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 9d cb          	vcvtps2srgb8 %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 9d cb          	vcvtps2srgb8 %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 9d cb          	vcvtps2srgb8 %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9d cb          	vcvtps2srgb8 %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 9d cb          	vcvtps2srgb8 %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 9d cb          	vcvtps2srgb8 %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 cb          	vlog2lutps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b1 cb          	vlog2lutps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 cb          	vlog2lutps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b1 cb          	vlog2lutps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 cb          	vlog2lutps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 cb          	vlog2lutps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 cb          	vlog2lutps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 cb          	vlog2lutps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b1 09          	vlog2lutps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b1 09          	vlog2lutps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 09          	vlog2lutps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 09          	vlog2lutps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 09          	vlog2lutps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 09          	vlog2lutps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 09          	vlog2lutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 cb          	vrcpresps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 c0 cb          	vrcpresps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 cb          	vrcpresps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 c0 cb          	vrcpresps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 cb          	vrcpresps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 cb          	vrcpresps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 cb          	vrcpresps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 cb          	vrcpresps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 c0 09          	vrcpresps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 c0 09          	vrcpresps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 09          	vrcpresps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 09          	vrcpresps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 09          	vrcpresps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 09          	vrcpresps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 09          	vrcpresps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 cb          	vrsqrtlutps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b2 cb          	vrsqrtlutps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 cb          	vrsqrtlutps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b2 cb          	vrsqrtlutps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 cb          	vrsqrtlutps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 cb          	vrsqrtlutps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 cb          	vrsqrtlutps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 cb          	vrsqrtlutps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b2 09          	vrsqrtlutps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b2 09          	vrsqrtlutps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 09          	vrsqrtlutps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 09          	vrsqrtlutps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 09          	vrsqrtlutps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 09          	vrsqrtlutps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 09          	vrsqrtlutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 cb          	vgetexpps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b4 cb          	vgetexpps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 cb          	vgetexpps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b4 cb          	vgetexpps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 cb          	vgetexpps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 cb          	vgetexpps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 cb          	vgetexpps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 cb          	vgetexpps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b4 09          	vgetexpps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b4 09          	vgetexpps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 09          	vgetexpps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 09          	vgetexpps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 09          	vgetexpps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 09          	vgetexpps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 09          	vgetexpps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 cb          	vexp2lutps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b0 cb          	vexp2lutps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 cb          	vexp2lutps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b0 cb          	vexp2lutps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 cb          	vexp2lutps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 cb          	vexp2lutps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 cb          	vexp2lutps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 cb          	vexp2lutps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b0 09          	vexp2lutps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b0 09          	vexp2lutps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 09          	vexp2lutps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 09          	vexp2lutps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 09          	vexp2lutps \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 09          	vexp2lutps \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 09          	vexp2lutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 cb 02 00    	vfixupps \$0x2,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 10 09          	vloadd \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 10 09          	vloadd \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 10 09          	vloadd \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 10 09          	vloadd \(%rcx\)\{1to16\}\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 09 10 09          	vloadd \(%rcx\)\{4to16\}\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 19 10 09          	vloadd \(%rcx\)\{1to16\}\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 19 10 09          	vloadd \(%rcx\)\{4to16\}\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 10 09          	vloadd \(%rcx\)\{1to16\}\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 10 09          	vloadd \(%rcx\)\{4to16\}\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 10 09          	vloadd \(%rcx\)\{1to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 10 09          	vloadd \(%rcx\)\{4to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 31 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 10 09          	vloadd \(%rcx\)\{1to16\}\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 39 10 09          	vloadd \(%rcx\)\{4to16\}\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 10 09          	vloadd \(%rcx\)\{1to16\}\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 41 10 09          	vloadd \(%rcx\)\{4to16\}\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 10 09          	vloadd \(%rcx\)\{1to16\}\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 49 10 09          	vloadd \(%rcx\)\{4to16\}\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 51 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 10 09          	vloadd \(%rcx\)\{1to16\}\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 59 10 09          	vloadd \(%rcx\)\{4to16\}\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 10 09          	vloadd \(%rcx\)\{1to16\}\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 61 10 09          	vloadd \(%rcx\)\{4to16\}\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 10 09          	vloadd \(%rcx\)\{1to16\}\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 69 10 09          	vloadd \(%rcx\)\{4to16\}\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 10 09          	vloadd \(%rcx\)\{1to16\}\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 71 10 09          	vloadd \(%rcx\)\{4to16\}\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 10 09          	vloadd \(%rcx\)\{1to16\}\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 79 10 09          	vloadd \(%rcx\)\{4to16\}\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 81 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 81 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 89 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 89 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 91 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 91 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 99 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 99 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a1 10 09          	vloadd \(%rcx\)\{1to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 09          	vloadd \(%rcx\)\{4to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a9 10 09          	vloadd \(%rcx\)\{1to16\}\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a9 10 09          	vloadd \(%rcx\)\{4to16\}\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 b1 10 09          	vloadd \(%rcx\)\{1to16\}\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 b1 10 09          	vloadd \(%rcx\)\{4to16\}\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 10 09       	vloadq \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 01 10 09       	vloadq \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 01 10 09       	vloadq \(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 01 10 09       	vloadq \(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 13 09          	vloadunpackhd \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 13 09          	vloadunpackhd \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 13 09          	vloadunpackhd \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 13 09          	vloadunpackhd \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 13 09          	vloadunpackhd \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 13 09          	vloadunpackhd \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 13 09          	vloadunpackhd \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 13 09          	vloadunpackhd \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 13 09          	vloadunpackhd \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 13 09          	vloadunpackhd \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 13 09          	vloadunpackhd \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 13 09          	vloadunpackhd \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 13 09          	vloadunpackhd \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 13 09          	vloadunpackhd \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 13 09          	vloadunpackhd \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 13 09          	vloadunpackhd \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 13 09          	vloadunpackhd \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 13 09          	vloadunpackhd \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 13 09          	vloadunpackhd \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 13 09          	vloadunpackhd \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 09          	vloadunpackhd \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 13 09          	vloadunpackhd \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 13 09          	vloadunpackhd \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 12 09          	vloadunpackld \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 12 09          	vloadunpackld \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 12 09          	vloadunpackld \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 12 09          	vloadunpackld \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 12 09          	vloadunpackld \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 12 09          	vloadunpackld \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 12 09          	vloadunpackld \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 12 09          	vloadunpackld \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 12 09          	vloadunpackld \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 12 09          	vloadunpackld \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 12 09          	vloadunpackld \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 12 09          	vloadunpackld \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 12 09          	vloadunpackld \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 12 09          	vloadunpackld \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 12 09          	vloadunpackld \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 12 09          	vloadunpackld \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 12 09          	vloadunpackld \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 12 09          	vloadunpackld \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 12 09          	vloadunpackld \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 12 09          	vloadunpackld \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 12 09          	vloadunpackld \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 12 09          	vloadunpackld \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 12 09          	vloadunpackld \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f1 ca 02 02    	vshuf128x32 \{aaac\},\{0002\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 f1 09 02 02    	vshuf128x32 \{aaac\},\{0002\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 14 09          	vstored %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 14 09          	vstored %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 14 09          	vstored %v1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 14 09          	vstored %v1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 14 09          	vstored %v1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 14 09          	vstored %v1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 14 09          	vstored %v1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 14 09          	vstored %v1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 14 09          	vstored %v1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 14 09          	vstored %v1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 14 09          	vstored %v1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 14 09          	vstored %v1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 14 09          	vstored %v1\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 14 09          	vstored %v1\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 14 09          	vstored %v1\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 14 09          	vstored %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 14 09          	vstored %v1\{a\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 14 09          	vstored %v1\{a\}\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 14 09          	vstored %v1\{a\}\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 14 09          	vstored %v1\{a\}\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 14 09          	vstored %v1\{a\}\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 14 09          	vstored %v1\{a\}\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 14 09          	vstored %v1\{a\}\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 14 09          	vstored %v1\{a\}\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 14 09          	vstored %v1\{a\}\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 14 09          	vstored %v1\{a\}\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 14 09          	vstored %v1\{a\}\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 14 09          	vstored %v1\{a\}\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 14 09          	vstored %v1\{a\}\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 14 09          	vstored %v1\{a\}\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 14 09          	vstored %v1\{a\}\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 14 09          	vstored %v1\{dcba\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 09 14 09          	vstored %v1\{dcba\}\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 14 09          	vstored %v1\{dcba\}\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 14 09          	vstored %v1\{dcba\}\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 14 09          	vstored %v1\{dcba\}\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 31 14 09          	vstored %v1\{dcba\}\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 39 14 09          	vstored %v1\{dcba\}\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 41 14 09          	vstored %v1\{dcba\}\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 49 14 09          	vstored %v1\{dcba\}\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 51 14 09          	vstored %v1\{dcba\}\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 59 14 09          	vstored %v1\{dcba\}\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 61 14 09          	vstored %v1\{dcba\}\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 69 14 09          	vstored %v1\{dcba\}\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 71 14 09          	vstored %v1\{dcba\}\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 79 14 09          	vstored %v1\{dcba\}\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 14 09       	vstoreq %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 14 09       	vstoreq %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 01 14 09       	vstoreq %v1\{a\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 01 14 09       	vstoreq %v1\{dcba\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 13 09       	vloadunpackhq \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 01 13 09       	vloadunpackhq \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 12 09       	vloadunpacklq \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 01 12 09       	vloadunpacklq \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 17 09          	vpackstorehd %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 17 09          	vpackstorehd %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 17 09          	vpackstorehd %v1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 17 09          	vpackstorehd %v1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 17 09          	vpackstorehd %v1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 17 09          	vpackstorehd %v1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 17 09          	vpackstorehd %v1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 17 09          	vpackstorehd %v1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 17 09          	vpackstorehd %v1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 17 09          	vpackstorehd %v1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 17 09          	vpackstorehd %v1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 17 09          	vpackstorehd %v1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 17 09          	vpackstorehd %v1\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 17 09          	vpackstorehd %v1\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 17 09          	vpackstorehd %v1\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 17 09          	vpackstorehd %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 16 09          	vpackstoreld %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 16 09          	vpackstoreld %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 16 09          	vpackstoreld %v1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 16 09          	vpackstoreld %v1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 16 09          	vpackstoreld %v1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 16 09          	vpackstoreld %v1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 16 09          	vpackstoreld %v1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 16 09          	vpackstoreld %v1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 16 09          	vpackstoreld %v1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 16 09          	vpackstoreld %v1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 16 09          	vpackstoreld %v1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 16 09          	vpackstoreld %v1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 16 09          	vpackstoreld %v1\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 16 09          	vpackstoreld %v1\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 16 09          	vpackstoreld %v1\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 16 09          	vpackstoreld %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 17 09       	vpackstorehq %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 17 09       	vpackstorehq %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 16 09       	vpackstorelq %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 16 09       	vpackstorelq %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 1c 8c 55 99 00 00 00 	vscatterd %v1,0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 01 1c 8c 55 99 00 00 00 	vscatterd %v1,0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 1c 8c 55 99 00 00 00 	vscatterd %v1\{float16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 1c 8c 55 99 00 00 00 	vscatterd %v1\{float16rz\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 1c 8c 55 99 00 00 00 	vscatterd %v1\{unorm8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 1c 8c 55 99 00 00 00 	vscatterd %v1\{snorm8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 1c 8c 55 99 00 00 00 	vscatterd %v1\{unorm16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 1c 8c 55 99 00 00 00 	vscatterd %v1\{snorm16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint8i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint8i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint16i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint16i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 19 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 81 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 89 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 91 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 99 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a1 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a9 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 b1 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 90 01 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float10C\}
[ 	]*[a-f0-9]+:	66 62 f4 c3             	bitinterleave11 %ax,%bx
[ 	]*[a-f0-9]+:	66 62 94 cf             	bitinterleave11 %r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f5 c3             	bitinterleave21 %ax,%bx
[ 	]*[a-f0-9]+:	66 62 95 cf             	bitinterleave21 %r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f2 c3             	bsff   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 92 cf             	bsff   %r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f0 c3             	bsfi   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 90 cf             	bsfi   %r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f3 c3             	bsrf   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 93 cf             	bsrf   %r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f1 c3             	bsri   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 91 cf             	bsri   %r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f8 c3             	countbits %ax,%bx
[ 	]*[a-f0-9]+:	66 62 98 cf             	countbits %r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f9 c3             	quadmask16 %ax,%bx
[ 	]*[a-f0-9]+:	66 62 99 cf             	quadmask16 %r9w,%r15w
[ 	]*[a-f0-9]+:	62 f4 c3                	bitinterleave11 %eax,%ebx
[ 	]*[a-f0-9]+:	62 94 cf                	bitinterleave11 %r9d,%r15d
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 %eax,%ebx
[ 	]*[a-f0-9]+:	62 95 cf                	bitinterleave21 %r9d,%r15d
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   %eax,%ebx
[ 	]*[a-f0-9]+:	62 92 cf                	bsff   %r9d,%r15d
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   %eax,%ebx
[ 	]*[a-f0-9]+:	62 90 cf                	bsfi   %r9d,%r15d
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   %eax,%ebx
[ 	]*[a-f0-9]+:	62 93 cf                	bsrf   %r9d,%r15d
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   %eax,%ebx
[ 	]*[a-f0-9]+:	62 91 cf                	bsri   %r9d,%r15d
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits %eax,%ebx
[ 	]*[a-f0-9]+:	62 98 cf                	countbits %r9d,%r15d
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 %eax,%ebx
[ 	]*[a-f0-9]+:	62 99 cf                	quadmask16 %r9d,%r15d
[ 	]*[a-f0-9]+:	62 e4 c3                	bitinterleave11 %rax,%rbx
[ 	]*[a-f0-9]+:	62 84 cf                	bitinterleave11 %r9,%r15
[ 	]*[a-f0-9]+:	62 e5 c3                	bitinterleave21 %rax,%rbx
[ 	]*[a-f0-9]+:	62 85 cf                	bitinterleave21 %r9,%r15
[ 	]*[a-f0-9]+:	62 e2 c3                	bsff   %rax,%rbx
[ 	]*[a-f0-9]+:	62 82 cf                	bsff   %r9,%r15
[ 	]*[a-f0-9]+:	62 e0 c3                	bsfi   %rax,%rbx
[ 	]*[a-f0-9]+:	62 80 cf                	bsfi   %r9,%r15
[ 	]*[a-f0-9]+:	62 e3 c3                	bsrf   %rax,%rbx
[ 	]*[a-f0-9]+:	62 83 cf                	bsrf   %r9,%r15
[ 	]*[a-f0-9]+:	62 e1 c3                	bsri   %rax,%rbx
[ 	]*[a-f0-9]+:	62 81 cf                	bsri   %r9,%r15
[ 	]*[a-f0-9]+:	62 e8 c3                	countbits %rax,%rbx
[ 	]*[a-f0-9]+:	62 88 cf                	countbits %r9,%r15
[ 	]*[a-f0-9]+:	62 e9 c3                	quadmask16 %rax,%rbx
[ 	]*[a-f0-9]+:	62 89 cf                	quadmask16 %r9,%r15
[ 	]*[a-f0-9]+:	62 eb 01                	clevict1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 09                	clevict2 \(%rcx\)
[ 	]*[a-f0-9]+:	62 cb 39                	vprefetch1 \$0x7,\(%rcx\)
[ 	]*[a-f0-9]+:	62 db 19                	vprefetch2 \$0x3,\(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 11                	ldvxcsr \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 19                	stvxcsr \(%rcx\)
[ 	]*[a-f0-9]+:	66 62 df c0             	delay  %ax
[ 	]*[a-f0-9]+:	66 62 9f c1             	delay  %r9w
[ 	]*[a-f0-9]+:	66 62 df c8             	spflt  %ax
[ 	]*[a-f0-9]+:	66 62 9f c9             	spflt  %r9w
[ 	]*[a-f0-9]+:	62 df c0                	delay  %eax
[ 	]*[a-f0-9]+:	62 9f c1                	delay  %r9d
[ 	]*[a-f0-9]+:	62 df c8                	spflt  %eax
[ 	]*[a-f0-9]+:	62 9f c9                	spflt  %r9d
[ 	]*[a-f0-9]+:	62 cf c0                	delay  %rax
[ 	]*[a-f0-9]+:	62 8f c1                	delay  %r9
[ 	]*[a-f0-9]+:	62 cf c8                	spflt  %rax
[ 	]*[a-f0-9]+:	62 8f c9                	spflt  %r9
[ 	]*[a-f0-9]+:	d6 00 50 04 c0          	delay  \$0xa
[ 	]*[a-f0-9]+:	66 62 f6 c3 0b 00 02 1f 	insertfield \$0x1f,\$0x2,\$0xb,%ax,%bx
[ 	]*[a-f0-9]+:	66 62 96 cf 0b 00 02 1f 	insertfield \$0x1f,\$0x2,\$0xb,%r9w,%r15w
[ 	]*[a-f0-9]+:	66 62 f7 c3 0b 00 02 1f 	rotatefield \$0x1f,\$0x2,\$0xb,%ax,%bx
[ 	]*[a-f0-9]+:	66 62 97 cf 0b 00 02 1f 	rotatefield \$0x1f,\$0x2,\$0xb,%r9w,%r15w
[ 	]*[a-f0-9]+:	62 f6 c3 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%eax,%ebx
[ 	]*[a-f0-9]+:	62 96 cf 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%r9d,%r15d
[ 	]*[a-f0-9]+:	62 f7 c3 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%eax,%ebx
[ 	]*[a-f0-9]+:	62 97 cf 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%r9d,%r15d
[ 	]*[a-f0-9]+:	62 e6 c3 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%rax,%rbx
[ 	]*[a-f0-9]+:	62 86 cf 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%r9,%r15
[ 	]*[a-f0-9]+:	62 e7 c3 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%rax,%rbx
[ 	]*[a-f0-9]+:	62 87 cf 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%r9,%r15
[ 	]*[a-f0-9]+:	d6 17 11 45 d6          	vadcpi %v30\{cdab\},%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 67 a1 48 d6       	vaddnpd %v30\{dddd\},%v20,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 78 56 34 12 	vadcpi 0x12345678,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 55 00       	vadcpi 0x0\(%rbp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 24       	vadcpi \(%rsp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 95 99 00 00 00 	vadcpi 0x99\(%rbp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 05 11 45 97 99 00 00 00 	vadcpi 0x99\(%r15\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 15 99 00 00 00 	vadcpi 0x99\(%rip\),%k2,%v10\{%k1\}        # 30f3 <_start\+0x30f3>
[ 	]*[a-f0-9]+:	d6 04 11 45 94 24 99 00 00 00 	vadcpi 0x99\(%rsp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 05 11 45 94 24 99 00 00 00 	vadcpi 0x99\(%r12\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 67 ff ff ff 	vadcpi 0xffffffffffffff67,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 65 67 ff ff ff 	vadcpi -0x99\(,%riz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 94 23 67 ff ff ff 	vadcpi -0x99\(%rbx,%riz,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 94 63 67 ff ff ff 	vadcpi -0x99\(%rbx,%riz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 07 11 45 94 bc 67 ff ff ff 	vadcpi -0x99\(%r12,%r15,4\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 07 11 45 94 f8 67 ff ff ff 	vadcpi -0x99\(%r8,%r15,8\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 06 11 45 94 ad 67 ff ff ff 	vadcpi -0x99\(%rbp,%r13,4\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 06 11 45 94 24 67 ff ff ff 	vadcpi -0x99\(%rsp,%r12,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 00 00 00 00 	vgatherd 0x0\(,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 00 00 00 00 	vgatherd 0x0\(,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 67 ff ff ff 	vgatherd -0x99\(,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 67 ff ff ff 	vgatherd -0x99\(,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0b       	vgatherd \(%rbx,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 13       	vgatherd \(%rbx,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 94 0b 67 ff ff ff 	vgatherd -0x99\(%rbx,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 94 13 67 ff ff ff 	vgatherd -0x99\(%rbx,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 94 4b 67 ff ff ff 	vgatherd -0x99\(%rbx,%v1,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 94 53 67 ff ff ff 	vgatherd -0x99\(%rbx,%v10,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 05 01 18 94 8c 67 ff ff ff 	vgatherd -0x99\(%r12,%v1,4\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 15 01 18 94 94 67 ff ff ff 	vgatherd -0x99\(%r12,%v10,4\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 34 12 00 00 	vadcpi 0x1234,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 55 00    	vadcpi 0x0\(%ebp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 24    	vadcpi \(%esp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 95 99 00 00 00 	vadcpi 0x99\(%ebp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 25 99 00 00 00 	addr32 vadcpi 0x99,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 65 99 00 00 00 	addr32 vadcpi 0x99\(,%eiz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 20 99 00 00 00 	vadcpi 0x99\(%eax,%eiz,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 60 99 00 00 00 	vadcpi 0x99\(%eax,%eiz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 98 99 00 00 00 	vadcpi 0x99\(%eax,%ebx,4\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 cc 99 00 00 00 	vadcpi 0x99\(%esp,%ecx,8\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 15 99 00 00 00 	vadcpi 0x99\(%ebp,%edx,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 00 00 00 00 	vgatherd 0x0\(,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 00 00 00 00 	vgatherd 0x0\(,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 67 ff ff ff 	vgatherd -0x99\(,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 67 ff ff ff 	vgatherd -0x99\(,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 14 08    	vgatherd \(%eax,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 14 10    	vgatherd \(%eax,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 08 99 00 00 00 	vgatherd 0x99\(%eax,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 10 99 00 00 00 	vgatherd 0x99\(%eax,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 48 99 00 00 00 	vgatherd 0x99\(%eax,%v1,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 50 99 00 00 00 	vgatherd 0x99\(%eax,%v10,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 8d 99 00 00 00 	vgatherd 0x99\(%ebp,%v1,4\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 95 99 00 00 00 	vgatherd 0x99\(%ebp,%v10,4\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	62 cd ca                	vkand  %k1,%k2
[ 	]*[a-f0-9]+:	62 dd ca                	vkandn %k1,%k2
[ 	]*[a-f0-9]+:	62 ed ca                	vkandnr %k1,%k2
[ 	]*[a-f0-9]+:	62 fc ca                	vkmov  %k1,%k2
[ 	]*[a-f0-9]+:	62 fb ca                	vkmovlhb %k1,%k2
[ 	]*[a-f0-9]+:	62 fd ca                	vknot  %k1,%k2
[ 	]*[a-f0-9]+:	62 ce ca                	vkor   %k1,%k2
[ 	]*[a-f0-9]+:	62 de ca                	vkortest %k1,%k2
[ 	]*[a-f0-9]+:	62 ef ca                	vkswapb %k1,%k2
[ 	]*[a-f0-9]+:	62 fe ca                	vkxnor %k1,%k2
[ 	]*[a-f0-9]+:	62 ee ca                	vkxor  %k1,%k2
[ 	]*[a-f0-9]+:	66 62 ec c2             	vkmov  %ax,%k2
[ 	]*[a-f0-9]+:	66 62 cc ca             	vkmov  %r9w,%k2
[ 	]*[a-f0-9]+:	62 ec c2                	vkmov  %eax,%k2
[ 	]*[a-f0-9]+:	62 cc ca                	vkmov  %r9d,%k2
[ 	]*[a-f0-9]+:	66 62 dc c8             	vkmov  %k1,%ax
[ 	]*[a-f0-9]+:	66 62 9c c9             	vkmov  %k1,%r9w
[ 	]*[a-f0-9]+:	62 dc c8                	vkmov  %k1,%eax
[ 	]*[a-f0-9]+:	62 9c c9                	vkmov  %k1,%r9d
[ 	]*[a-f0-9]+:	d6 00 11 45 cb          	vadcpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 45 cb          	vadcpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 45 cb          	vadcpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 45 cb          	vadcpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 45 cb          	vadcpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 45 cb          	vadcpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 45 cb          	vadcpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 45 cb          	vadcpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 45 09          	vadcpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 45 09          	vadcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 45 09          	vadcpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 45 09          	vadcpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 45 09          	vadcpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 45 09          	vadcpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 45 09          	vadcpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 45 09          	vadcpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 45 09          	vadcpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 43 cb          	vaddsetcpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 43 cb          	vaddsetcpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 43 cb          	vaddsetcpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 43 cb          	vaddsetcpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 43 cb          	vaddsetcpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 43 cb          	vaddsetcpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 43 cb          	vaddsetcpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 43 cb          	vaddsetcpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 43 09          	vaddsetcpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 43 09          	vaddsetcpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 43 09          	vaddsetcpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 43 09          	vaddsetcpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 43 09          	vaddsetcpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 43 09          	vaddsetcpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 43 09          	vaddsetcpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 43 09          	vaddsetcpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 43 09          	vaddsetcpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 55 cb          	vsbbpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 55 cb          	vsbbpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 55 cb          	vsbbpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 55 cb          	vsbbpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 55 cb          	vsbbpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 55 cb          	vsbbpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 55 cb          	vsbbpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 55 cb          	vsbbpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 55 09          	vsbbpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 55 09          	vsbbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 55 09          	vsbbpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 55 09          	vsbbpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 55 09          	vsbbpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 55 09          	vsbbpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 55 09          	vsbbpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 55 09          	vsbbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 55 09          	vsbbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5d cb          	vsbbrpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5d cb          	vsbbrpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5d cb          	vsbbrpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 5d cb          	vsbbrpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5d cb          	vsbbrpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5d cb          	vsbbrpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5d cb          	vsbbrpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5d cb          	vsbbrpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 5d 09          	vsbbrpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5d 09          	vsbbrpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5d 09          	vsbbrpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5d 09          	vsbbrpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5d 09          	vsbbrpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5d 09          	vsbbrpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5d 09          	vsbbrpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5d 09          	vsbbrpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5d 09          	vsbbrpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5b cb          	vsubrsetbpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5b cb          	vsubrsetbpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5b cb          	vsubrsetbpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 5b cb          	vsubrsetbpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5b cb          	vsubrsetbpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5b cb          	vsubrsetbpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5b cb          	vsubrsetbpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5b cb          	vsubrsetbpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 5b 09          	vsubrsetbpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 5b 09          	vsubrsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 5b 09          	vsubrsetbpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 5b 09          	vsubrsetbpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 5b 09          	vsubrsetbpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 5b 09          	vsubrsetbpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 5b 09          	vsubrsetbpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5b 09          	vsubrsetbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 5b 09          	vsubrsetbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 53 cb          	vsubsetbpi %v3,%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 53 cb          	vsubsetbpi %v3\{cdab\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 53 cb          	vsubsetbpi %v3\{badc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 53 cb          	vsubsetbpi %v3\{aaaa\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 53 cb          	vsubsetbpi %v3\{bbbb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 53 cb          	vsubsetbpi %v3\{cccc\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 53 cb          	vsubsetbpi %v3\{dddd\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 53 cb          	vsubsetbpi %v3\{dacb\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 53 09          	vsubsetbpi \(%rcx\)\{nt\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 53 09          	vsubsetbpi \(%rcx\),%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 53 09          	vsubsetbpi \(%rcx\)\{1to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 53 09          	vsubsetbpi \(%rcx\)\{4to16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 53 09          	vsubsetbpi \(%rcx\)\{uint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 53 09          	vsubsetbpi \(%rcx\)\{sint8\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 53 09          	vsubsetbpi \(%rcx\)\{uint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 53 09          	vsubsetbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 53 09          	vsubsetbpi \(%rcx\)\{sint16\},%k2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 48 cb          	vaddnps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 48 cb          	vaddnps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 48 cb          	vaddnps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 48 cb          	vaddnps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 48 cb          	vaddnps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 48 cb          	vaddnps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 48 cb          	vaddnps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 48 cb          	vaddnps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 48 09          	vaddnps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 48 09          	vaddnps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 48 09          	vaddnps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 48 09          	vaddnps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 48 09          	vaddnps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 48 09          	vaddnps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 48 09          	vaddnps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 48 09          	vaddnps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 48 09          	vaddnps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 40 cb          	vaddps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 40 cb          	vaddps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 40 cb          	vaddps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 40 cb          	vaddps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 40 cb          	vaddps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 40 cb          	vaddps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 40 cb          	vaddps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 40 cb          	vaddps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 40 09          	vaddps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 40 09          	vaddps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 40 09          	vaddps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 40 09          	vaddps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 40 09          	vaddps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 40 09          	vaddps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 40 09          	vaddps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 40 09          	vaddps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 40 09          	vaddps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 46 cb          	vaddsetsps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 46 cb          	vaddsetsps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 46 cb          	vaddsetsps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 46 cb          	vaddsetsps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 46 cb          	vaddsetsps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 46 cb          	vaddsetsps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 46 cb          	vaddsetsps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 46 cb          	vaddsetsps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 46 09          	vaddsetsps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 46 09          	vaddsetsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 46 09          	vaddsetsps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 46 09          	vaddsetsps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 46 09          	vaddsetsps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 46 09          	vaddsetsps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 46 09          	vaddsetsps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 46 09          	vaddsetsps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 46 09          	vaddsetsps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 86 cb          	vclampzps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 86 cb          	vclampzps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 86 cb          	vclampzps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 86 cb          	vclampzps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 86 cb          	vclampzps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 86 cb          	vclampzps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 86 cb          	vclampzps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 86 cb          	vclampzps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 86 09          	vclampzps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 86 09          	vclampzps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 86 09          	vclampzps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 86 09          	vclampzps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 86 09          	vclampzps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 86 09          	vclampzps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 86 09          	vclampzps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 86 09          	vclampzps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 86 09          	vclampzps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 64 cb          	vmadd132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 64 cb          	vmadd132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 64 cb          	vmadd132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 64 cb          	vmadd132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 64 cb          	vmadd132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 64 cb          	vmadd132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 64 cb          	vmadd132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 64 cb          	vmadd132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 64 09          	vmadd132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 64 09          	vmadd132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 64 09          	vmadd132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 64 09          	vmadd132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 64 09          	vmadd132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 64 09          	vmadd132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 64 09          	vmadd132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 64 09          	vmadd132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 64 09          	vmadd132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 62 cb          	vmadd213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 62 cb          	vmadd213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 62 cb          	vmadd213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 62 cb          	vmadd213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 62 cb          	vmadd213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 62 cb          	vmadd213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 62 cb          	vmadd213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 62 cb          	vmadd213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 62 09          	vmadd213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 62 09          	vmadd213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 62 09          	vmadd213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 62 09          	vmadd213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 62 09          	vmadd213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 62 09          	vmadd213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 62 09          	vmadd213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 62 09          	vmadd213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 62 09          	vmadd213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 60 cb          	vmadd231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 60 cb          	vmadd231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 60 cb          	vmadd231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 60 cb          	vmadd231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 60 cb          	vmadd231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 60 cb          	vmadd231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 60 cb          	vmadd231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 60 cb          	vmadd231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 60 09          	vmadd231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 60 09          	vmadd231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 60 09          	vmadd231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 60 09          	vmadd231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 60 09          	vmadd231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 60 09          	vmadd231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 60 09          	vmadd231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 60 09          	vmadd231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 60 09          	vmadd231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 66 cb          	vmadd233ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 66 cb          	vmadd233ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 66 cb          	vmadd233ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 66 cb          	vmadd233ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 66 cb          	vmadd233ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 66 cb          	vmadd233ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 66 cb          	vmadd233ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 66 cb          	vmadd233ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 66 09          	vmadd233ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 66 09          	vmadd233ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 66 09          	vmadd233ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 66 09          	vmadd233ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 66 09          	vmadd233ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 66 09          	vmadd233ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 66 09          	vmadd233ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 66 09          	vmadd233ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 66 09          	vmadd233ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 74 cb          	vmaddn132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 74 cb          	vmaddn132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 74 cb          	vmaddn132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 74 cb          	vmaddn132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 74 cb          	vmaddn132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 74 cb          	vmaddn132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 74 cb          	vmaddn132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 74 cb          	vmaddn132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 74 09          	vmaddn132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 74 09          	vmaddn132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 74 09          	vmaddn132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 74 09          	vmaddn132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 74 09          	vmaddn132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 74 09          	vmaddn132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 74 09          	vmaddn132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 74 09          	vmaddn132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 74 09          	vmaddn132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 72 cb          	vmaddn213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 72 cb          	vmaddn213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 72 cb          	vmaddn213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 72 cb          	vmaddn213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 72 cb          	vmaddn213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 72 cb          	vmaddn213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 72 cb          	vmaddn213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 72 cb          	vmaddn213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 72 09          	vmaddn213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 72 09          	vmaddn213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 72 09          	vmaddn213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 72 09          	vmaddn213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 72 09          	vmaddn213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 72 09          	vmaddn213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 72 09          	vmaddn213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 72 09          	vmaddn213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 72 09          	vmaddn213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 70 cb          	vmaddn231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 70 cb          	vmaddn231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 70 cb          	vmaddn231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 70 cb          	vmaddn231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 70 cb          	vmaddn231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 70 cb          	vmaddn231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 70 cb          	vmaddn231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 70 cb          	vmaddn231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 70 09          	vmaddn231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 70 09          	vmaddn231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 70 09          	vmaddn231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 70 09          	vmaddn231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 70 09          	vmaddn231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 70 09          	vmaddn231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 70 09          	vmaddn231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 70 09          	vmaddn231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 70 09          	vmaddn231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9e cb          	vmaxabsps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9e cb          	vmaxabsps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9e cb          	vmaxabsps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 9e cb          	vmaxabsps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9e cb          	vmaxabsps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9e cb          	vmaxabsps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9e cb          	vmaxabsps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9e cb          	vmaxabsps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 9e 09          	vmaxabsps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9e 09          	vmaxabsps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9e 09          	vmaxabsps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9e 09          	vmaxabsps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9e 09          	vmaxabsps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9e 09          	vmaxabsps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9e 09          	vmaxabsps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9e 09          	vmaxabsps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9e 09          	vmaxabsps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 88 cb          	vmaxps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 88 cb          	vmaxps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 88 cb          	vmaxps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 88 cb          	vmaxps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 88 cb          	vmaxps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 88 cb          	vmaxps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 88 cb          	vmaxps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 88 cb          	vmaxps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 88 09          	vmaxps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 88 09          	vmaxps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 88 09          	vmaxps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 88 09          	vmaxps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 88 09          	vmaxps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 88 09          	vmaxps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 88 09          	vmaxps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 88 09          	vmaxps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 88 09          	vmaxps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8c cb          	vminps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8c cb          	vminps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8c cb          	vminps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8c cb          	vminps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8c cb          	vminps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8c cb          	vminps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8c cb          	vminps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8c cb          	vminps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8c 09          	vminps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8c 09          	vminps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8c 09          	vminps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8c 09          	vminps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8c 09          	vminps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8c 09          	vminps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8c 09          	vminps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8c 09          	vminps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8c 09          	vminps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6c cb          	vmsub132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6c cb          	vmsub132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6c cb          	vmsub132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 6c cb          	vmsub132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6c cb          	vmsub132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6c cb          	vmsub132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6c cb          	vmsub132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6c cb          	vmsub132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 6c 09          	vmsub132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6c 09          	vmsub132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6c 09          	vmsub132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6c 09          	vmsub132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6c 09          	vmsub132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6c 09          	vmsub132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6c 09          	vmsub132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6c 09          	vmsub132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6c 09          	vmsub132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6a cb          	vmsub213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6a cb          	vmsub213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6a cb          	vmsub213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 6a cb          	vmsub213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6a cb          	vmsub213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6a cb          	vmsub213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6a cb          	vmsub213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6a cb          	vmsub213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 6a 09          	vmsub213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 6a 09          	vmsub213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 6a 09          	vmsub213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 6a 09          	vmsub213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 6a 09          	vmsub213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 6a 09          	vmsub213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 6a 09          	vmsub213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6a 09          	vmsub213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 6a 09          	vmsub213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 68 cb          	vmsub231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 68 cb          	vmsub231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 68 cb          	vmsub231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 68 cb          	vmsub231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 68 cb          	vmsub231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 68 cb          	vmsub231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 68 cb          	vmsub231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 68 cb          	vmsub231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 68 09          	vmsub231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 68 09          	vmsub231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 68 09          	vmsub231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 68 09          	vmsub231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 68 09          	vmsub231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 68 09          	vmsub231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 68 09          	vmsub231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 68 09          	vmsub231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 68 09          	vmsub231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7c cb          	vmsubr132ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7c cb          	vmsubr132ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7c cb          	vmsubr132ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 7c cb          	vmsubr132ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7c cb          	vmsubr132ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7c cb          	vmsubr132ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7c cb          	vmsubr132ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7c cb          	vmsubr132ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 7c 09          	vmsubr132ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7c 09          	vmsubr132ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7c 09          	vmsubr132ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7c 09          	vmsubr132ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7c 09          	vmsubr132ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7c 09          	vmsubr132ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7c 09          	vmsubr132ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7c 09          	vmsubr132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7c 09          	vmsubr132ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7a cb          	vmsubr213ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7a cb          	vmsubr213ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7a cb          	vmsubr213ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 7a cb          	vmsubr213ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7a cb          	vmsubr213ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7a cb          	vmsubr213ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7a cb          	vmsubr213ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7a cb          	vmsubr213ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 7a 09          	vmsubr213ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7a 09          	vmsubr213ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7a 09          	vmsubr213ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7a 09          	vmsubr213ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7a 09          	vmsubr213ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7a 09          	vmsubr213ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7a 09          	vmsubr213ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7a 09          	vmsubr213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7a 09          	vmsubr213ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 78 cb          	vmsubr231ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 78 cb          	vmsubr231ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 78 cb          	vmsubr231ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 78 cb          	vmsubr231ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 78 cb          	vmsubr231ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 78 cb          	vmsubr231ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 78 cb          	vmsubr231ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 78 cb          	vmsubr231ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 78 09          	vmsubr231ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 78 09          	vmsubr231ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 78 09          	vmsubr231ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 78 09          	vmsubr231ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 78 09          	vmsubr231ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 78 09          	vmsubr231ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 78 09          	vmsubr231ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 78 09          	vmsubr231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 78 09          	vmsubr231ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7e cb          	vmsubr23c1ps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7e cb          	vmsubr23c1ps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7e cb          	vmsubr23c1ps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 7e cb          	vmsubr23c1ps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7e cb          	vmsubr23c1ps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7e cb          	vmsubr23c1ps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7e cb          	vmsubr23c1ps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7e cb          	vmsubr23c1ps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 7e 09          	vmsubr23c1ps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 7e 09          	vmsubr23c1ps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 7e 09          	vmsubr23c1ps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 7e 09          	vmsubr23c1ps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 7e 09          	vmsubr23c1ps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 7e 09          	vmsubr23c1ps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 7e 09          	vmsubr23c1ps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7e 09          	vmsubr23c1ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 7e 09          	vmsubr23c1ps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 80 cb          	vmulps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 80 cb          	vmulps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 80 cb          	vmulps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 80 cb          	vmulps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 80 cb          	vmulps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 80 cb          	vmulps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 80 cb          	vmulps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 80 cb          	vmulps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 80 09          	vmulps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 80 09          	vmulps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 80 09          	vmulps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 80 09          	vmulps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 80 09          	vmulps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 80 09          	vmulps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 80 09          	vmulps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 80 09          	vmulps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 80 09          	vmulps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 cb          	vrcprefineps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 c2 cb          	vrcprefineps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 cb          	vrcprefineps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 c2 cb          	vrcprefineps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 cb          	vrcprefineps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 cb          	vrcprefineps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 cb          	vrcprefineps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 cb          	vrcprefineps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 c2 09          	vrcprefineps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 c2 09          	vrcprefineps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 c2 09          	vrcprefineps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 c2 09          	vrcprefineps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 c2 09          	vrcprefineps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 c2 09          	vrcprefineps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 c2 09          	vrcprefineps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 09          	vrcprefineps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 c2 09          	vrcprefineps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 50 cb          	vsubps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 50 cb          	vsubps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 50 cb          	vsubps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 50 cb          	vsubps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 50 cb          	vsubps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 50 cb          	vsubps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 50 cb          	vsubps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 50 cb          	vsubps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 50 09          	vsubps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 50 09          	vsubps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 50 09          	vsubps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 50 09          	vsubps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 50 09          	vsubps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 50 09          	vsubps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 50 09          	vsubps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 50 09          	vsubps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 50 09          	vsubps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 58 cb          	vsubrps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 58 cb          	vsubrps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 58 cb          	vsubrps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 58 cb          	vsubrps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 58 cb          	vsubrps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 58 cb          	vsubrps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 58 cb          	vsubrps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 58 cb          	vsubrps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 58 09          	vsubrps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 58 09          	vsubrps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 58 09          	vsubrps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 58 09          	vsubrps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 58 09          	vsubrps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 58 09          	vsubrps \(%rcx\)\{unorm8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 58 09          	vsubrps \(%rcx\)\{float16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 58 09          	vsubrps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 58 09          	vsubrps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 cb       	vaddnpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 48 cb       	vaddnpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 cb       	vaddnpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 48 cb       	vaddnpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 48 cb       	vaddnpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 48 cb       	vaddnpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 48 cb       	vaddnpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 48 cb       	vaddnpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 48 09       	vaddnpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 48 09       	vaddnpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 48 09       	vaddnpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 09       	vaddnpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 48 09       	vaddnpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 cb       	vaddpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 40 cb       	vaddpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 cb       	vaddpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 40 cb       	vaddpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 40 cb       	vaddpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 40 cb       	vaddpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 40 cb       	vaddpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 40 cb       	vaddpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 40 09       	vaddpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 40 09       	vaddpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 40 09       	vaddpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 09       	vaddpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 40 09       	vaddpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 cb       	vmadd132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 64 cb       	vmadd132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 cb       	vmadd132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 64 cb       	vmadd132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 64 cb       	vmadd132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 64 cb       	vmadd132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 64 cb       	vmadd132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 64 cb       	vmadd132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 64 09       	vmadd132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 64 09       	vmadd132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 64 09       	vmadd132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 09       	vmadd132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 64 09       	vmadd132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 cb       	vmadd213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 62 cb       	vmadd213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 cb       	vmadd213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 62 cb       	vmadd213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 62 cb       	vmadd213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 62 cb       	vmadd213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 62 cb       	vmadd213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 62 cb       	vmadd213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 62 09       	vmadd213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 62 09       	vmadd213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 62 09       	vmadd213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 09       	vmadd213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 62 09       	vmadd213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 cb       	vmadd231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 60 cb       	vmadd231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 cb       	vmadd231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 60 cb       	vmadd231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 60 cb       	vmadd231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 60 cb       	vmadd231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 60 cb       	vmadd231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 60 cb       	vmadd231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 60 09       	vmadd231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 60 09       	vmadd231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 60 09       	vmadd231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 09       	vmadd231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 60 09       	vmadd231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 cb       	vmaddn132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 74 cb       	vmaddn132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 cb       	vmaddn132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 74 cb       	vmaddn132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 74 cb       	vmaddn132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 74 cb       	vmaddn132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 74 cb       	vmaddn132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 74 cb       	vmaddn132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 74 09       	vmaddn132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 74 09       	vmaddn132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 74 09       	vmaddn132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 09       	vmaddn132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 74 09       	vmaddn132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 cb       	vmaddn213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 72 cb       	vmaddn213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 cb       	vmaddn213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 72 cb       	vmaddn213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 72 cb       	vmaddn213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 72 cb       	vmaddn213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 72 cb       	vmaddn213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 72 cb       	vmaddn213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 72 09       	vmaddn213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 72 09       	vmaddn213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 72 09       	vmaddn213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 09       	vmaddn213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 72 09       	vmaddn213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 cb       	vmaddn231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 70 cb       	vmaddn231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 cb       	vmaddn231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 70 cb       	vmaddn231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 70 cb       	vmaddn231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 70 cb       	vmaddn231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 70 cb       	vmaddn231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 70 cb       	vmaddn231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 70 09       	vmaddn231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 70 09       	vmaddn231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 70 09       	vmaddn231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 09       	vmaddn231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 70 09       	vmaddn231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 cb       	vmaxpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 88 cb       	vmaxpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 cb       	vmaxpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 88 cb       	vmaxpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 88 cb       	vmaxpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 88 cb       	vmaxpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 88 cb       	vmaxpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 88 cb       	vmaxpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 88 09       	vmaxpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 88 09       	vmaxpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 88 09       	vmaxpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 09       	vmaxpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 88 09       	vmaxpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c cb       	vminpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 8c cb       	vminpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c cb       	vminpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 8c cb       	vminpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 8c cb       	vminpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 8c cb       	vminpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 8c cb       	vminpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 8c cb       	vminpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 8c 09       	vminpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 8c 09       	vminpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 8c 09       	vminpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c 09       	vminpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 8c 09       	vminpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c cb       	vmsub132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6c cb       	vmsub132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c cb       	vmsub132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6c cb       	vmsub132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6c cb       	vmsub132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6c cb       	vmsub132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6c cb       	vmsub132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6c cb       	vmsub132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6c 09       	vmsub132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6c 09       	vmsub132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6c 09       	vmsub132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c 09       	vmsub132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6c 09       	vmsub132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a cb       	vmsub213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6a cb       	vmsub213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a cb       	vmsub213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 6a cb       	vmsub213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 6a cb       	vmsub213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 6a cb       	vmsub213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 6a cb       	vmsub213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 6a cb       	vmsub213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 6a 09       	vmsub213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 6a 09       	vmsub213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 6a 09       	vmsub213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a 09       	vmsub213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 6a 09       	vmsub213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 cb       	vmsub231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 68 cb       	vmsub231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 cb       	vmsub231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 68 cb       	vmsub231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 68 cb       	vmsub231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 68 cb       	vmsub231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 68 cb       	vmsub231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 68 cb       	vmsub231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 68 09       	vmsub231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 68 09       	vmsub231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 68 09       	vmsub231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 09       	vmsub231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 68 09       	vmsub231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c cb       	vmsubr132pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7c cb       	vmsubr132pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c cb       	vmsubr132pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7c cb       	vmsubr132pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7c cb       	vmsubr132pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7c cb       	vmsubr132pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7c cb       	vmsubr132pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7c cb       	vmsubr132pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7c 09       	vmsubr132pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7c 09       	vmsubr132pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7c 09       	vmsubr132pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c 09       	vmsubr132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7c 09       	vmsubr132pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a cb       	vmsubr213pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7a cb       	vmsubr213pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a cb       	vmsubr213pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7a cb       	vmsubr213pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7a cb       	vmsubr213pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7a cb       	vmsubr213pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7a cb       	vmsubr213pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7a cb       	vmsubr213pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7a 09       	vmsubr213pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7a 09       	vmsubr213pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7a 09       	vmsubr213pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a 09       	vmsubr213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7a 09       	vmsubr213pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 cb       	vmsubr231pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 78 cb       	vmsubr231pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 cb       	vmsubr231pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 78 cb       	vmsubr231pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 78 cb       	vmsubr231pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 78 cb       	vmsubr231pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 78 cb       	vmsubr231pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 78 cb       	vmsubr231pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 78 09       	vmsubr231pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 78 09       	vmsubr231pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 78 09       	vmsubr231pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 09       	vmsubr231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 78 09       	vmsubr231pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e cb       	vmsubr23c1pd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7e cb       	vmsubr23c1pd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e cb       	vmsubr23c1pd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 7e cb       	vmsubr23c1pd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 7e cb       	vmsubr23c1pd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 7e cb       	vmsubr23c1pd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 7e cb       	vmsubr23c1pd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 7e cb       	vmsubr23c1pd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 7e 09       	vmsubr23c1pd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 7e 09       	vmsubr23c1pd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 7e 09       	vmsubr23c1pd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e 09       	vmsubr23c1pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 7e 09       	vmsubr23c1pd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 cb       	vmulpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 80 cb       	vmulpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 cb       	vmulpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 80 cb       	vmulpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 80 cb       	vmulpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 80 cb       	vmulpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 80 cb       	vmulpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 80 cb       	vmulpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 80 09       	vmulpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 80 09       	vmulpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 80 09       	vmulpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 09       	vmulpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 80 09       	vmulpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 cb       	vsubpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 50 cb       	vsubpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 cb       	vsubpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 50 cb       	vsubpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 50 cb       	vsubpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 50 cb       	vsubpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 50 cb       	vsubpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 50 cb       	vsubpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 50 09       	vsubpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 50 09       	vsubpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 50 09       	vsubpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 09       	vsubpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 50 09       	vsubpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 cb       	vsubrpd %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 58 cb       	vsubrpd %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 cb       	vsubrpd %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 58 cb       	vsubrpd %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 58 cb       	vsubrpd %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 58 cb       	vsubrpd %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 58 cb       	vsubrpd %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 58 cb       	vsubrpd %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 58 09       	vsubrpd \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 58 09       	vsubrpd \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 58 09       	vsubrpd \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 09       	vsubrpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 58 09       	vsubrpd \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 41 cb          	vaddpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 41 cb          	vaddpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 41 cb          	vaddpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 41 cb          	vaddpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 41 cb          	vaddpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 41 cb          	vaddpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 41 cb          	vaddpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 41 cb          	vaddpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 41 09          	vaddpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 41 09          	vaddpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 41 09          	vaddpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 41 09          	vaddpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 41 09          	vaddpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 41 09          	vaddpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 41 09          	vaddpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 41 09          	vaddpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 41 09          	vaddpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 47 cb          	vaddsetspi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 47 cb          	vaddsetspi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 47 cb          	vaddsetspi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 47 cb          	vaddsetspi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 47 cb          	vaddsetspi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 47 cb          	vaddsetspi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 47 cb          	vaddsetspi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 47 cb          	vaddsetspi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 47 09          	vaddsetspi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 47 09          	vaddsetspi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 47 09          	vaddsetspi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 47 09          	vaddsetspi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 47 09          	vaddsetspi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 47 09          	vaddsetspi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 47 09          	vaddsetspi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 47 09          	vaddsetspi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 47 09          	vaddsetspi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 cb          	vandnpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a4 cb          	vandnpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 cb          	vandnpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a4 cb          	vandnpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 cb          	vandnpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 cb          	vandnpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 cb          	vandnpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 cb          	vandnpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a4 09          	vandnpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a4 09          	vandnpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a4 09          	vandnpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a4 09          	vandnpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a4 09          	vandnpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a4 09          	vandnpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a4 09          	vandnpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 09          	vandnpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a4 09          	vandnpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 cb          	vandpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a5 cb          	vandpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 cb          	vandpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a5 cb          	vandpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 cb          	vandpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 cb          	vandpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 cb          	vandpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 cb          	vandpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a5 09          	vandpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a5 09          	vandpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a5 09          	vandpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a5 09          	vandpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a5 09          	vandpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a5 09          	vandpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a5 09          	vandpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 09          	vandpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a5 09          	vandpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 cb          	vbitinterleave11pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a8 cb          	vbitinterleave11pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 cb          	vbitinterleave11pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a8 cb          	vbitinterleave11pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 cb          	vbitinterleave11pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 cb          	vbitinterleave11pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 cb          	vbitinterleave11pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 cb          	vbitinterleave11pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a8 09          	vbitinterleave11pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a8 09          	vbitinterleave11pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a8 09          	vbitinterleave11pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a8 09          	vbitinterleave11pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a8 09          	vbitinterleave11pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a8 09          	vbitinterleave11pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a8 09          	vbitinterleave11pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 09          	vbitinterleave11pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a8 09          	vbitinterleave11pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 cb          	vbitinterleave21pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a9 cb          	vbitinterleave21pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 cb          	vbitinterleave21pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a9 cb          	vbitinterleave21pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 cb          	vbitinterleave21pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 cb          	vbitinterleave21pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 cb          	vbitinterleave21pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 cb          	vbitinterleave21pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a9 09          	vbitinterleave21pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a9 09          	vbitinterleave21pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a9 09          	vbitinterleave21pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a9 09          	vbitinterleave21pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a9 09          	vbitinterleave21pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a9 09          	vbitinterleave21pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a9 09          	vbitinterleave21pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 09          	vbitinterleave21pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a9 09          	vbitinterleave21pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 87 cb          	vclampzpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 87 cb          	vclampzpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 87 cb          	vclampzpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 87 cb          	vclampzpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 87 cb          	vclampzpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 87 cb          	vclampzpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 87 cb          	vclampzpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 87 cb          	vclampzpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 87 09          	vclampzpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 87 09          	vclampzpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 87 09          	vclampzpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 87 09          	vclampzpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 87 09          	vclampzpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 87 09          	vclampzpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 87 09          	vclampzpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 87 09          	vclampzpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 87 09          	vclampzpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 61 cb          	vmadd231pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 61 cb          	vmadd231pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 61 cb          	vmadd231pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 61 cb          	vmadd231pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 61 cb          	vmadd231pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 61 cb          	vmadd231pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 61 cb          	vmadd231pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 61 cb          	vmadd231pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 61 09          	vmadd231pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 61 09          	vmadd231pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 61 09          	vmadd231pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 61 09          	vmadd231pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 61 09          	vmadd231pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 61 09          	vmadd231pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 61 09          	vmadd231pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 61 09          	vmadd231pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 61 09          	vmadd231pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 67 cb          	vmadd233pi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 67 cb          	vmadd233pi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 67 cb          	vmadd233pi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 67 cb          	vmadd233pi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 67 cb          	vmadd233pi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 67 cb          	vmadd233pi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 67 cb          	vmadd233pi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 67 cb          	vmadd233pi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 67 09          	vmadd233pi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 67 09          	vmadd233pi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 67 09          	vmadd233pi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 67 09          	vmadd233pi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 67 09          	vmadd233pi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 67 09          	vmadd233pi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 67 09          	vmadd233pi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 67 09          	vmadd233pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 67 09          	vmadd233pi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 89 cb          	vmaxpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 89 cb          	vmaxpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 89 cb          	vmaxpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 89 cb          	vmaxpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 89 cb          	vmaxpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 89 cb          	vmaxpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 89 cb          	vmaxpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 89 cb          	vmaxpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 89 09          	vmaxpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 89 09          	vmaxpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 89 09          	vmaxpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 89 09          	vmaxpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 89 09          	vmaxpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 89 09          	vmaxpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 89 09          	vmaxpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 89 09          	vmaxpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 89 09          	vmaxpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8a cb          	vmaxpu %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8a cb          	vmaxpu %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8a cb          	vmaxpu %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8a cb          	vmaxpu %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8a cb          	vmaxpu %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8a cb          	vmaxpu %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8a cb          	vmaxpu %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8a cb          	vmaxpu %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8a 09          	vmaxpu \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8a 09          	vmaxpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8a 09          	vmaxpu \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8a 09          	vmaxpu \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8a 09          	vmaxpu \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8a 09          	vmaxpu \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8a 09          	vmaxpu \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8a 09          	vmaxpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8a 09          	vmaxpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8d cb          	vminpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8d cb          	vminpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8d cb          	vminpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8d cb          	vminpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8d cb          	vminpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8d cb          	vminpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8d cb          	vminpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8d cb          	vminpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8d 09          	vminpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8d 09          	vminpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8d 09          	vminpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8d 09          	vminpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8d 09          	vminpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8d 09          	vminpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8d 09          	vminpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8d 09          	vminpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8d 09          	vminpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8e cb          	vminpu %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8e cb          	vminpu %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8e cb          	vminpu %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 8e cb          	vminpu %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8e cb          	vminpu %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8e cb          	vminpu %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8e cb          	vminpu %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8e cb          	vminpu %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 8e 09          	vminpu \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 8e 09          	vminpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 8e 09          	vminpu \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 8e 09          	vminpu \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 8e 09          	vminpu \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 8e 09          	vminpu \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 8e 09          	vminpu \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8e 09          	vminpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 8e 09          	vminpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 81 cb          	vmulhpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 81 cb          	vmulhpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 81 cb          	vmulhpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 81 cb          	vmulhpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 81 cb          	vmulhpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 81 cb          	vmulhpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 81 cb          	vmulhpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 81 cb          	vmulhpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 81 09          	vmulhpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 81 09          	vmulhpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 81 09          	vmulhpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 81 09          	vmulhpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 81 09          	vmulhpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 81 09          	vmulhpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 81 09          	vmulhpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 81 09          	vmulhpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 81 09          	vmulhpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 82 cb          	vmulhpu %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 82 cb          	vmulhpu %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 82 cb          	vmulhpu %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 82 cb          	vmulhpu %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 82 cb          	vmulhpu %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 82 cb          	vmulhpu %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 82 cb          	vmulhpu %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 82 cb          	vmulhpu %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 82 09          	vmulhpu \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 82 09          	vmulhpu \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 82 09          	vmulhpu \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 82 09          	vmulhpu \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 82 09          	vmulhpu \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 82 09          	vmulhpu \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 82 09          	vmulhpu \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 82 09          	vmulhpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 82 09          	vmulhpu \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 83 cb          	vmullpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 83 cb          	vmullpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 83 cb          	vmullpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 83 cb          	vmullpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 83 cb          	vmullpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 83 cb          	vmullpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 83 cb          	vmullpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 83 cb          	vmullpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 83 09          	vmullpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 83 09          	vmullpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 83 09          	vmullpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 83 09          	vmullpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 83 09          	vmullpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 83 09          	vmullpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 83 09          	vmullpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 83 09          	vmullpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 83 09          	vmullpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 cb          	vorpi  %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a6 cb          	vorpi  %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 cb          	vorpi  %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a6 cb          	vorpi  %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 cb          	vorpi  %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 cb          	vorpi  %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 cb          	vorpi  %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 cb          	vorpi  %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a6 09          	vorpi  \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a6 09          	vorpi  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a6 09          	vorpi  \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a6 09          	vorpi  \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a6 09          	vorpi  \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a6 09          	vorpi  \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a6 09          	vorpi  \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 09          	vorpi  \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a6 09          	vorpi  \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 cb          	vscaleps %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 b8 cb          	vscaleps %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 cb          	vscaleps %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 b8 cb          	vscaleps %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 cb          	vscaleps %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 cb          	vscaleps %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 cb          	vscaleps %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 cb          	vscaleps %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 b8 09          	vscaleps \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 b8 09          	vscaleps \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 b8 09          	vscaleps \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 b8 09          	vscaleps \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 b8 09          	vscaleps \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 b8 09          	vscaleps \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 b8 09          	vscaleps \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 09          	vscaleps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 b8 09          	vscaleps \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 cb          	vsllpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a0 cb          	vsllpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 cb          	vsllpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a0 cb          	vsllpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 cb          	vsllpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 cb          	vsllpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 cb          	vsllpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 cb          	vsllpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a0 09          	vsllpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a0 09          	vsllpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a0 09          	vsllpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a0 09          	vsllpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a0 09          	vsllpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a0 09          	vsllpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a0 09          	vsllpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 09          	vsllpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a0 09          	vsllpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 cb          	vsrapi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a1 cb          	vsrapi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 cb          	vsrapi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a1 cb          	vsrapi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 cb          	vsrapi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 cb          	vsrapi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 cb          	vsrapi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 cb          	vsrapi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a1 09          	vsrapi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a1 09          	vsrapi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a1 09          	vsrapi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a1 09          	vsrapi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a1 09          	vsrapi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a1 09          	vsrapi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a1 09          	vsrapi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 09          	vsrapi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a1 09          	vsrapi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 cb          	vsrlpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a2 cb          	vsrlpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 cb          	vsrlpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a2 cb          	vsrlpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 cb          	vsrlpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 cb          	vsrlpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 cb          	vsrlpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 cb          	vsrlpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a2 09          	vsrlpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a2 09          	vsrlpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a2 09          	vsrlpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a2 09          	vsrlpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a2 09          	vsrlpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a2 09          	vsrlpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a2 09          	vsrlpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 09          	vsrlpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a2 09          	vsrlpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 51 cb          	vsubpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 51 cb          	vsubpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 51 cb          	vsubpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 51 cb          	vsubpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 51 cb          	vsubpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 51 cb          	vsubpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 51 cb          	vsubpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 51 cb          	vsubpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 51 09          	vsubpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 51 09          	vsubpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 51 09          	vsubpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 51 09          	vsubpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 51 09          	vsubpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 51 09          	vsubpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 51 09          	vsubpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 51 09          	vsubpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 51 09          	vsubpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 59 cb          	vsubrpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 59 cb          	vsubrpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 59 cb          	vsubrpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 59 cb          	vsubrpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 59 cb          	vsubrpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 59 cb          	vsubrpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 59 cb          	vsubrpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 59 cb          	vsubrpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 59 09          	vsubrpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 59 09          	vsubrpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 59 09          	vsubrpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 59 09          	vsubrpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 59 09          	vsubrpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 59 09          	vsubrpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 59 09          	vsubrpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 59 09          	vsubrpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 59 09          	vsubrpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 cb          	vxorpi %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a7 cb          	vxorpi %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 cb          	vxorpi %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 a7 cb          	vxorpi %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 cb          	vxorpi %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 cb          	vxorpi %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 cb          	vxorpi %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 cb          	vxorpi %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 a7 09          	vxorpi \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 a7 09          	vxorpi \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 a7 09          	vxorpi \(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 a7 09          	vxorpi \(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 a7 09          	vxorpi \(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 a7 09          	vxorpi \(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 a7 09          	vxorpi \(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 09          	vxorpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 a7 09          	vxorpi \(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 12 30 cb          	vtestpi %v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 12 30 cb          	vtestpi %v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 20 12 30 cb          	vtestpi %v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 30 12 30 cb          	vtestpi %v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 40 12 30 cb          	vtestpi %v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 50 12 30 cb          	vtestpi %v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 60 12 30 cb          	vtestpi %v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 30 cb          	vtestpi %v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 30 09          	vtestpi \(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi \(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 00 12 30 09          	vtestpi \(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 12 30 09          	vtestpi \(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 20 12 30 09          	vtestpi \(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 40 12 30 09          	vtestpi \(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 50 12 30 09          	vtestpi \(%rcx\)\{sint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 60 12 30 09          	vtestpi \(%rcx\)\{uint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 30 09          	vtestpi \(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 30 09          	vtestpi \(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 cb       	vandnpq %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 cb       	vandnpq %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 cb       	vandnpq %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a4 cb       	vandnpq %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a4 cb       	vandnpq %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a4 cb       	vandnpq %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a4 cb       	vandnpq %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a4 cb       	vandnpq %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a4 09       	vandnpq \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a4 09       	vandnpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a4 09       	vandnpq \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 09       	vandnpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a4 09       	vandnpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 cb       	vandpq %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 cb       	vandpq %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 cb       	vandpq %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a5 cb       	vandpq %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a5 cb       	vandpq %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a5 cb       	vandpq %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a5 cb       	vandpq %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a5 cb       	vandpq %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a5 09       	vandpq \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a5 09       	vandpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a5 09       	vandpq \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 09       	vandpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a5 09       	vandpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 cb       	vorpq  %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 cb       	vorpq  %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 cb       	vorpq  %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a6 cb       	vorpq  %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a6 cb       	vorpq  %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a6 cb       	vorpq  %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a6 cb       	vorpq  %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a6 cb       	vorpq  %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a6 09       	vorpq  \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a6 09       	vorpq  \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a6 09       	vorpq  \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 09       	vorpq  \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a6 09       	vorpq  \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 cb       	vxorpq %v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 cb       	vxorpq %v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 cb       	vxorpq %v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 11 a7 cb       	vxorpq %v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 11 a7 cb       	vxorpq %v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 11 a7 cb       	vxorpq %v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 11 a7 cb       	vxorpq %v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 11 a7 cb       	vxorpq %v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 11 a7 09       	vxorpq \(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 11 a7 09       	vxorpq \(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 11 a7 09       	vxorpq \(%rcx\)\{1to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 09       	vxorpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 11 a7 09       	vxorpq \(%rcx\)\{4to8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 cb       	vcmppd \{unord\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 cb       	vcmppd \{unord\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 cb       	vcmppd \{unord\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 3c 12 20 cb       	vcmppd \{unord\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 4c 12 20 cb       	vcmppd \{unord\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 5c 12 20 cb       	vcmppd \{unord\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 6c 12 20 cb       	vcmppd \{unord\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 70 12 20 cb       	vcmppd \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 80 12 20 09       	vcmppd \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 74 12 20 cb       	vcmppd \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 84 12 20 09       	vcmppd \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 78 12 20 cb       	vcmppd \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 88 12 20 09       	vcmppd \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 7c 12 20 cb       	vcmppd \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 8c 12 20 09       	vcmppd \{unord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 70 12 24 cb       	vcmppd \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 80 12 24 09       	vcmppd \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 74 12 24 cb       	vcmppd \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 84 12 24 09       	vcmppd \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 78 12 24 cb       	vcmppd \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 88 12 24 09       	vcmppd \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 7c 12 24 cb       	vcmppd \{ord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 8c 12 24 09       	vcmppd \{ord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 0c 12 20 09       	vcmppd \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 1c 12 20 09       	vcmppd \{unord\},\(%rcx\)\{1to8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 09       	vcmppd \{unord\},\(%rcx\)\{4to8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	66 d6 2c 12 20 09       	vcmppd \{unord\},\(%rcx\)\{4to8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 21 cb          	vcmppi \{lt\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 21 cb          	vcmppi \{lt\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 21 cb          	vcmppi \{lt\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 34 12 21 cb          	vcmppi \{lt\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 21 cb          	vcmppi \{lt\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 21 cb          	vcmppi \{lt\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 21 cb          	vcmppi \{lt\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 21 cb          	vcmppi \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 21 09          	vcmppi \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 21 cb          	vcmppi \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 21 09          	vcmppi \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 21 cb          	vcmppi \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 21 09          	vcmppi \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 25 cb          	vcmppi \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 25 09          	vcmppi \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 25 cb          	vcmppi \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 25 09          	vcmppi \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 25 cb          	vcmppi \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 25 09          	vcmppi \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 21 09          	vcmppi \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 21 09          	vcmppi \{lt\},\(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 21 09          	vcmppi \{lt\},\(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 21 09          	vcmppi \{lt\},\(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 21 09          	vcmppi \{lt\},\(%rcx\)\{sint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 21 09          	vcmppi \{lt\},\(%rcx\)\{uint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 21 09          	vcmppi \{lt\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 21 09          	vcmppi \{lt\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 22 cb          	vcmppu \{lt\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 22 cb          	vcmppu \{lt\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 22 cb          	vcmppu \{lt\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 34 12 22 cb          	vcmppu \{lt\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 22 cb          	vcmppu \{lt\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 22 cb          	vcmppu \{lt\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 22 cb          	vcmppu \{lt\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 22 cb          	vcmppu \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 22 09          	vcmppu \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 22 cb          	vcmppu \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 22 09          	vcmppu \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 22 cb          	vcmppu \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 22 09          	vcmppu \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 26 cb          	vcmppu \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 26 09          	vcmppu \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 26 cb          	vcmppu \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 26 09          	vcmppu \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 26 cb          	vcmppu \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 26 09          	vcmppu \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 04 12 22 09          	vcmppu \{lt\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 14 12 22 09          	vcmppu \{lt\},\(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 24 12 22 09          	vcmppu \{lt\},\(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 44 12 22 09          	vcmppu \{lt\},\(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 54 12 22 09          	vcmppu \{lt\},\(%rcx\)\{sint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 64 12 22 09          	vcmppu \{lt\},\(%rcx\)\{uint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 22 09          	vcmppu \{lt\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 22 09          	vcmppu \{lt\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 0c 12 20 cb          	vcmpps \{unord\},%v3,%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 1c 12 20 cb          	vcmpps \{unord\},%v3\{cdab\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 2c 12 20 cb          	vcmpps \{unord\},%v3\{badc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 3c 12 20 cb          	vcmpps \{unord\},%v3\{aaaa\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 4c 12 20 cb          	vcmpps \{unord\},%v3\{bbbb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 5c 12 20 cb          	vcmpps \{unord\},%v3\{cccc\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 6c 12 20 cb          	vcmpps \{unord\},%v3\{dddd\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 20 cb          	vcmpps \{eq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 20 09          	vcmpps \{eq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 20 cb          	vcmpps \{lt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 20 09          	vcmpps \{lt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 20 cb          	vcmpps \{le\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 20 09          	vcmpps \{le\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 20 cb          	vcmpps \{unord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 8c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 70 12 24 cb          	vcmpps \{neq\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 80 12 24 09          	vcmpps \{neq\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 74 12 24 cb          	vcmpps \{nlt\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 84 12 24 09          	vcmpps \{nlt\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 78 12 24 cb          	vcmpps \{nle\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 88 12 24 09          	vcmpps \{nle\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 24 cb          	vcmpps \{ord\},%v3\{dacb\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 8c 12 24 09          	vcmpps \{ord\},\(%rcx\)\{nt\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 0c 12 20 09          	vcmpps \{unord\},\(%rcx\),%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 1c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{1to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 2c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{4to16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 4c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{uint8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 5c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{unorm8\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 6c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{float16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 7c 12 20 09          	vcmpps \{unord\},\(%rcx\)\{sint16\},%v2,%k1\{%k2\}
[ 	]*[a-f0-9]+:	d6 00 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9b cb          	vcvtinsps2f11 \$0x2,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 d1 9b cb          	vcvtinsps2f11 \$0x2,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 d1 9b 09          	vcvtinsps2f11 \$0x2,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9b 09          	vcvtinsps2f11 \$0x2,\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9a cb          	vcvtinsps2u10 \$0x2,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9a cb          	vcvtinsps2u10 \$0x2,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 9a 09          	vcvtinsps2u10 \$0x2,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 91 cb       	vcvtpd2pi \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 49 91 cb       	vcvtpd2pi \$0x1,\{rd\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 49 91 09       	vcvtpd2pi \$0x1,\{rd\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 89 91 cb       	vcvtpd2pi \$0x1,\{ru\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 89 91 09       	vcvtpd2pi \$0x1,\{ru\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 c9 91 cb       	vcvtpd2pi \$0x1,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 c9 91 09       	vcvtpd2pi \$0x1,\{rz\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 c9 91 09       	vcvtpd2pi \$0x1,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 91 09       	vcvtpd2pi \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 90 cb       	vcvtpd2ps \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 49 90 cb       	vcvtpd2ps \$0x1,\{rd\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 49 90 09       	vcvtpd2ps \$0x1,\{rd\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 89 90 cb       	vcvtpd2ps \$0x1,\{ru\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 89 90 09       	vcvtpd2ps \$0x1,\{ru\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 c9 90 cb       	vcvtpd2ps \$0x1,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 c9 90 09       	vcvtpd2ps \$0x1,\{rz\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 c9 90 09       	vcvtpd2ps \$0x1,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 90 09       	vcvtpd2ps \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 92 cb       	vcvtpd2pu \$0x1,\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 49 92 cb       	vcvtpd2pu \$0x1,\{rd\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 49 92 09       	vcvtpd2pu \$0x1,\{rd\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 89 92 cb       	vcvtpd2pu \$0x1,\{ru\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 89 92 09       	vcvtpd2pu \$0x1,\{ru\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 c9 92 cb       	vcvtpd2pu \$0x1,\{rz\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 c9 92 09       	vcvtpd2pu \$0x1,\{rz\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 c9 92 09       	vcvtpd2pu \$0x1,\{rz\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 92 09       	vcvtpd2pu \$0x1,\{rn\},\(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 95 cb       	vcvtpi2pd \$0x1,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 95 cb       	vcvtpi2pd \$0x1,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 95 cb       	vcvtpi2pd \$0x1,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 95 cb       	vcvtpi2pd \$0x1,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 95 cb       	vcvtpi2pd \$0x1,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 95 cb       	vcvtpi2pd \$0x1,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 95 cb       	vcvtpi2pd \$0x1,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 95 cb       	vcvtpi2pd \$0x1,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 95 09       	vcvtpi2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 96 cb       	vcvtpu2pd \$0x1,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 96 cb       	vcvtpu2pd \$0x1,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 96 cb       	vcvtpu2pd \$0x1,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 96 cb       	vcvtpu2pd \$0x1,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 96 cb       	vcvtpu2pd \$0x1,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 96 cb       	vcvtpu2pd \$0x1,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 96 cb       	vcvtpu2pd \$0x1,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 96 cb       	vcvtpu2pd \$0x1,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 96 09       	vcvtpu2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 95 cb          	vcvtpi2ps \{.16b\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 95 cb          	vcvtpi2ps \{.0b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 95 cb          	vcvtpi2ps \{.4b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 95 cb          	vcvtpi2ps \{.5b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 95 cb          	vcvtpi2ps \{.8b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 95 cb          	vcvtpi2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 95 cb          	vcvtpi2ps \{.24b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 95 cb          	vcvtpi2ps \{.31b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 95 cb          	vcvtpi2ps \{.32b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 95 09          	vcvtpi2ps \{.16b\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 96 cb          	vcvtpu2ps \{.16b\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 96 cb          	vcvtpu2ps \{.0b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 96 cb          	vcvtpu2ps \{.4b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 96 cb          	vcvtpu2ps \{.5b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 96 cb          	vcvtpu2ps \{.8b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 96 cb          	vcvtpu2ps \{.16b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 96 cb          	vcvtpu2ps \{.24b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 96 cb          	vcvtpu2ps \{.31b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 96 cb          	vcvtpu2ps \{.32b\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 96 09          	vcvtpu2ps \{.16b\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 94 cb       	vcvtps2pd \$0x1,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 94 cb       	vcvtps2pd \$0x1,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 94 cb       	vcvtps2pd \$0x1,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 30 09 94 cb       	vcvtps2pd \$0x1,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 94 cb       	vcvtps2pd \$0x1,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 94 cb       	vcvtps2pd \$0x1,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 94 cb       	vcvtps2pd \$0x1,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 94 cb       	vcvtps2pd \$0x1,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 09 94 09       	vcvtps2pd \$0x1,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 40 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 50 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 60 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 70 09 94 09       	vcvtps2pd \$0x1,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 91 cb          	vcvtps2pi \{.0b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 91 cb          	vcvtps2pi \{.4b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 91 cb          	vcvtps2pi \{.5b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 91 cb          	vcvtps2pi \{.8b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 91 cb          	vcvtps2pi \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 91 cb          	vcvtps2pi \{.24b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 91 cb          	vcvtps2pi \{.31b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 91 cb          	vcvtps2pi \{.32b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 91 09          	vcvtps2pi \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 92 cb          	vcvtps2pu \{.0b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 92 cb          	vcvtps2pu \{.4b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 92 cb          	vcvtps2pu \{.5b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 92 cb          	vcvtps2pu \{.8b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 92 cb          	vcvtps2pu \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 92 cb          	vcvtps2pu \{.24b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 92 cb          	vcvtps2pu \{.31b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 92 cb          	vcvtps2pu \{.32b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 92 09          	vcvtps2pu \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 98 cb          	vroundps \{.16b\},\{rn\},%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 98 cb          	vroundps \{.0b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 09 98 cb          	vroundps \{.4b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 98 cb          	vroundps \{.5b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 19 98 cb          	vroundps \{.8b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 98 cb          	vroundps \{.16b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 29 98 cb          	vroundps \{.24b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 31 98 cb          	vroundps \{.31b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 39 98 cb          	vroundps \{.32b\},\{rn\},%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 21 98 09          	vroundps \{.16b\},\{rn\},\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 9d cb          	vcvtps2srgb8 %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 9d cb          	vcvtps2srgb8 %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 9d cb          	vcvtps2srgb8 %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 9d cb          	vcvtps2srgb8 %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 9d cb          	vcvtps2srgb8 %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9d cb          	vcvtps2srgb8 %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 9d cb          	vcvtps2srgb8 %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 9d cb          	vcvtps2srgb8 %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 9d 09          	vcvtps2srgb8 \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 9d 09          	vcvtps2srgb8 \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 cb          	vlog2lutps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b1 cb          	vlog2lutps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 cb          	vlog2lutps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b1 cb          	vlog2lutps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 cb          	vlog2lutps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 cb          	vlog2lutps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 cb          	vlog2lutps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 cb          	vlog2lutps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b1 09          	vlog2lutps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b1 09          	vlog2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b1 09          	vlog2lutps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b1 09          	vlog2lutps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b1 09          	vlog2lutps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b1 09          	vlog2lutps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b1 09          	vlog2lutps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 09          	vlog2lutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b1 09          	vlog2lutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 cb          	vrcpresps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 c0 cb          	vrcpresps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 cb          	vrcpresps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 c0 cb          	vrcpresps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 cb          	vrcpresps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 cb          	vrcpresps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 cb          	vrcpresps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 cb          	vrcpresps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 c0 09          	vrcpresps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 c0 09          	vrcpresps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 c0 09          	vrcpresps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 c0 09          	vrcpresps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 c0 09          	vrcpresps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 c0 09          	vrcpresps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 c0 09          	vrcpresps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 09          	vrcpresps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 c0 09          	vrcpresps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 cb          	vrsqrtlutps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b2 cb          	vrsqrtlutps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 cb          	vrsqrtlutps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b2 cb          	vrsqrtlutps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 cb          	vrsqrtlutps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 cb          	vrsqrtlutps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 cb          	vrsqrtlutps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 cb          	vrsqrtlutps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b2 09          	vrsqrtlutps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b2 09          	vrsqrtlutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b2 09          	vrsqrtlutps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b2 09          	vrsqrtlutps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b2 09          	vrsqrtlutps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b2 09          	vrsqrtlutps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b2 09          	vrsqrtlutps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 09          	vrsqrtlutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b2 09          	vrsqrtlutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 cb          	vgetexpps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b4 cb          	vgetexpps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 cb          	vgetexpps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b4 cb          	vgetexpps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 cb          	vgetexpps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 cb          	vgetexpps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 cb          	vgetexpps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 cb          	vgetexpps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b4 09          	vgetexpps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b4 09          	vgetexpps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b4 09          	vgetexpps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b4 09          	vgetexpps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b4 09          	vgetexpps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b4 09          	vgetexpps \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b4 09          	vgetexpps \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 09          	vgetexpps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b4 09          	vgetexpps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 cb          	vexp2lutps %v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b0 cb          	vexp2lutps %v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 cb          	vexp2lutps %v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 b0 cb          	vexp2lutps %v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 cb          	vexp2lutps %v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 cb          	vexp2lutps %v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 cb          	vexp2lutps %v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 cb          	vexp2lutps %v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 b0 09          	vexp2lutps \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 b0 09          	vexp2lutps \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 b0 09          	vexp2lutps \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 b0 09          	vexp2lutps \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 b0 09          	vexp2lutps \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 b0 09          	vexp2lutps \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 b0 09          	vexp2lutps \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 09          	vexp2lutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 b0 09          	vexp2lutps \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 cb 02 00    	vfixupps \$0x2,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f0 cb 02 00    	vfixupps \$0x2,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f0 09 02 00    	vfixupps \$0x2,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3,%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{cdab\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{badc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{aaaa\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{bbbb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{cccc\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{dddd\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 f2 cb 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,%v3\{dacb\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{nt\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{1to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{4to16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint8\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 11 f2 09 8b f8    	vinsertfieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint16\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 10 09          	vloadd \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 10 09          	vloadd \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 10 09          	vloadd \(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 10 09          	vloadd \(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 10 09          	vloadd \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 10 09          	vloadd \(%rcx\)\{1to16\}\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 09 10 09          	vloadd \(%rcx\)\{4to16\}\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 10 09          	vloadd \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 19 10 09          	vloadd \(%rcx\)\{1to16\}\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 19 10 09          	vloadd \(%rcx\)\{4to16\}\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 10 09          	vloadd \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 10 09          	vloadd \(%rcx\)\{1to16\}\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 10 09          	vloadd \(%rcx\)\{4to16\}\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 10 09          	vloadd \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 10 09          	vloadd \(%rcx\)\{1to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 10 09          	vloadd \(%rcx\)\{4to16\}\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 10 09          	vloadd \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 31 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 10 09          	vloadd \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 10 09          	vloadd \(%rcx\)\{1to16\}\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 39 10 09          	vloadd \(%rcx\)\{4to16\}\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 10 09          	vloadd \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 10 09          	vloadd \(%rcx\)\{1to16\}\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 41 10 09          	vloadd \(%rcx\)\{4to16\}\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 10 09          	vloadd \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 10 09          	vloadd \(%rcx\)\{1to16\}\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 49 10 09          	vloadd \(%rcx\)\{4to16\}\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 10 09          	vloadd \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 51 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 10 09          	vloadd \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 10 09          	vloadd \(%rcx\)\{1to16\}\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 59 10 09          	vloadd \(%rcx\)\{4to16\}\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 10 09          	vloadd \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 10 09          	vloadd \(%rcx\)\{1to16\}\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 61 10 09          	vloadd \(%rcx\)\{4to16\}\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 10 09          	vloadd \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 10 09          	vloadd \(%rcx\)\{1to16\}\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 69 10 09          	vloadd \(%rcx\)\{4to16\}\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 10 09          	vloadd \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 10 09          	vloadd \(%rcx\)\{1to16\}\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 71 10 09          	vloadd \(%rcx\)\{4to16\}\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 10 09          	vloadd \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 10 09          	vloadd \(%rcx\)\{1to16\}\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 79 10 09          	vloadd \(%rcx\)\{4to16\}\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 10 09          	vloadd \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 81 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 81 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 10 09          	vloadd \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 89 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 89 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 10 09          	vloadd \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 91 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 91 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 10 09          	vloadd \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 99 10 09          	vloadd \(%rcx\)\{1to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 99 10 09          	vloadd \(%rcx\)\{4to16\}\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 10 09          	vloadd \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a1 10 09          	vloadd \(%rcx\)\{1to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a1 10 09          	vloadd \(%rcx\)\{4to16\}\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 10 09          	vloadd \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a9 10 09          	vloadd \(%rcx\)\{1to16\}\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 a9 10 09          	vloadd \(%rcx\)\{4to16\}\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 10 09          	vloadd \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 b1 10 09          	vloadd \(%rcx\)\{1to16\}\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 b1 10 09          	vloadd \(%rcx\)\{4to16\}\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 b1 10 09          	vloadd \(%rcx\)\{4to16\}\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 10 09       	vloadq \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 01 10 09       	vloadq \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 01 10 09       	vloadq \(%rcx\)\{1to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 01 10 09       	vloadq \(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 01 10 09       	vloadq \(%rcx\)\{4to8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 13 09          	vloadunpackhd \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 13 09          	vloadunpackhd \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 13 09          	vloadunpackhd \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 13 09          	vloadunpackhd \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 13 09          	vloadunpackhd \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 13 09          	vloadunpackhd \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 13 09          	vloadunpackhd \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 13 09          	vloadunpackhd \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 13 09          	vloadunpackhd \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 13 09          	vloadunpackhd \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 13 09          	vloadunpackhd \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 13 09          	vloadunpackhd \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 13 09          	vloadunpackhd \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 13 09          	vloadunpackhd \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 13 09          	vloadunpackhd \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 13 09          	vloadunpackhd \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 13 09          	vloadunpackhd \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 13 09          	vloadunpackhd \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 13 09          	vloadunpackhd \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 13 09          	vloadunpackhd \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 13 09          	vloadunpackhd \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 13 09          	vloadunpackhd \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 13 09          	vloadunpackhd \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 13 09          	vloadunpackhd \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 12 09          	vloadunpackld \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 12 09          	vloadunpackld \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 12 09          	vloadunpackld \(%rcx\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 19 12 09          	vloadunpackld \(%rcx\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 12 09          	vloadunpackld \(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 12 09          	vloadunpackld \(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 12 09          	vloadunpackld \(%rcx\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 12 09          	vloadunpackld \(%rcx\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 12 09          	vloadunpackld \(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 12 09          	vloadunpackld \(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 12 09          	vloadunpackld \(%rcx\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 12 09          	vloadunpackld \(%rcx\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 12 09          	vloadunpackld \(%rcx\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 12 09          	vloadunpackld \(%rcx\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 12 09          	vloadunpackld \(%rcx\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 12 09          	vloadunpackld \(%rcx\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 81 12 09          	vloadunpackld \(%rcx\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 89 12 09          	vloadunpackld \(%rcx\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 91 12 09          	vloadunpackld \(%rcx\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 99 12 09          	vloadunpackld \(%rcx\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a1 12 09          	vloadunpackld \(%rcx\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 a9 12 09          	vloadunpackld \(%rcx\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 12 09          	vloadunpackld \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 b1 12 09          	vloadunpackld \(%rcx\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{cdab\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{badc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 30 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{aaaa\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{bbbb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{cccc\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{dddd\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f3 cb 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,%v3\{dacb\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{1to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{4to16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 40 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 50 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 60 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 70 01 f3 09 8b f8    	vrotatefieldpi \$0x1f,\$0x2,\$0xb,\(%rcx\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f1 ca 02 02    	vshuf128x32 \{aaac\},\{0002\},%v2,%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 f1 09 02 02    	vshuf128x32 \{aaac\},\{0002\},\(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 f1 09 02 02    	vshuf128x32 \{aaac\},\{0002\},\(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 14 09          	vstored %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 14 09          	vstored %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 01 14 09          	vstored %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 14 09          	vstored %v1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 14 09          	vstored %v1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 14 09          	vstored %v1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 14 09          	vstored %v1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 14 09          	vstored %v1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 14 09          	vstored %v1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 14 09          	vstored %v1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 14 09          	vstored %v1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 14 09          	vstored %v1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 14 09          	vstored %v1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 14 09          	vstored %v1\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 14 09          	vstored %v1\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 14 09          	vstored %v1\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 14 09          	vstored %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 14 09          	vstored %v1\{a\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 14 09          	vstored %v1\{a\}\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 14 09          	vstored %v1\{a\}\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 14 09          	vstored %v1\{a\}\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 14 09          	vstored %v1\{a\}\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 14 09          	vstored %v1\{a\}\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 14 09          	vstored %v1\{a\}\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 14 09          	vstored %v1\{a\}\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 14 09          	vstored %v1\{a\}\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 14 09          	vstored %v1\{a\}\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 14 09          	vstored %v1\{a\}\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 14 09          	vstored %v1\{a\}\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 14 09          	vstored %v1\{a\}\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 14 09          	vstored %v1\{a\}\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 14 09          	vstored %v1\{a\}\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 01 14 09          	vstored %v1\{dcba\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 09 14 09          	vstored %v1\{dcba\}\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 11 14 09          	vstored %v1\{dcba\}\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 21 14 09          	vstored %v1\{dcba\}\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 29 14 09          	vstored %v1\{dcba\}\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 31 14 09          	vstored %v1\{dcba\}\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 39 14 09          	vstored %v1\{dcba\}\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 41 14 09          	vstored %v1\{dcba\}\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 49 14 09          	vstored %v1\{dcba\}\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 51 14 09          	vstored %v1\{dcba\}\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 59 14 09          	vstored %v1\{dcba\}\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 61 14 09          	vstored %v1\{dcba\}\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 69 14 09          	vstored %v1\{dcba\}\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 71 14 09          	vstored %v1\{dcba\}\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 20 79 14 09          	vstored %v1\{dcba\}\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 14 09       	vstoreq %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 14 09       	vstoreq %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 01 14 09       	vstoreq %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 10 01 14 09       	vstoreq %v1\{a\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 20 01 14 09       	vstoreq %v1\{dcba\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 13 09       	vloadunpackhq \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 01 13 09       	vloadunpackhq \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 12 09       	vloadunpacklq \(%rcx\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 00 01 12 09       	vloadunpacklq \(%rcx\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 17 09          	vpackstorehd %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 17 09          	vpackstorehd %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 17 09          	vpackstorehd %v1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 17 09          	vpackstorehd %v1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 17 09          	vpackstorehd %v1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 17 09          	vpackstorehd %v1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 17 09          	vpackstorehd %v1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 17 09          	vpackstorehd %v1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 17 09          	vpackstorehd %v1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 17 09          	vpackstorehd %v1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 17 09          	vpackstorehd %v1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 17 09          	vpackstorehd %v1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 17 09          	vpackstorehd %v1\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 17 09          	vpackstorehd %v1\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 17 09          	vpackstorehd %v1\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 17 09          	vpackstorehd %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 17 09          	vpackstorehd %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 80 01 16 09          	vpackstoreld %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 00 01 16 09          	vpackstoreld %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 09 16 09          	vpackstoreld %v1\{float16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 11 16 09          	vpackstoreld %v1\{float16rz\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 21 16 09          	vpackstoreld %v1\{uint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 29 16 09          	vpackstoreld %v1\{sint8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 31 16 09          	vpackstoreld %v1\{unorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 39 16 09          	vpackstoreld %v1\{snorm8\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 41 16 09          	vpackstoreld %v1\{uint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 49 16 09          	vpackstoreld %v1\{sint16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 51 16 09          	vpackstoreld %v1\{unorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 59 16 09          	vpackstoreld %v1\{snorm16\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 61 16 09          	vpackstoreld %v1\{uint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 69 16 09          	vpackstoreld %v1\{sint8i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 71 16 09          	vpackstoreld %v1\{uint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 16 09          	vpackstoreld %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 00 79 16 09          	vpackstoreld %v1\{sint16i\},\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 17 09       	vpackstorehq %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 17 09       	vpackstorehq %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 80 01 16 09       	vpackstorelq %v1,\(%rcx\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	66 d6 00 01 16 09       	vpackstorelq %v1,\(%rcx\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 1c 8c 55 99 00 00 00 	vscatterd %v1,0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 01 1c 8c 55 99 00 00 00 	vscatterd %v1,0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 1c 8c 55 99 00 00 00 	vscatterd %v1\{float16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 11 1c 8c 55 99 00 00 00 	vscatterd %v1\{float16rz\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 1c 8c 55 99 00 00 00 	vscatterd %v1\{unorm8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 1c 8c 55 99 00 00 00 	vscatterd %v1\{snorm8\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 1c 8c 55 99 00 00 00 	vscatterd %v1\{unorm16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 1c 8c 55 99 00 00 00 	vscatterd %v1\{snorm16\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint8i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint8i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 1c 8c 55 99 00 00 00 	vscatterd %v1\{uint16i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint16i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 1c 8c 55 99 00 00 00 	vscatterd %v1\{sint16i\},0x99\(%rbp,%v10,2\)\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 1d 84 55 99 00 00 00 	vscatterpfd 0x99\(%rbp,%v10,2\)\{%k1\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 12 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float16rz\}
[ 	]*[a-f0-9]+:	d6 10 22 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 79 1f 84 55 99 00 00 00 	vscatterpf2d 0x99\(%rbp,%v10,2\)\{%k1\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 90 01 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{nt\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 01 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\),%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 09 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 19 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{srgb8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 21 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 29 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 31 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 39 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{snorm8\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 41 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 49 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 51 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 59 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{snorm16\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 61 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 69 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint8i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 71 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{uint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 79 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{sint16i\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 81 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm10A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 89 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm10B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 91 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 99 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{unorm2D\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a1 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float11A\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 a9 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float11B\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 b1 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 10 b1 18 8c 55 99 00 00 00 	vgatherd 0x99\(%rbp,%v10,2\)\{float10C\},%v1\{%k1\}
[ 	]*[a-f0-9]+:	d6 90 01 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 10 b2 19 84 55 99 00 00 00 	vgatherpfd 0x99\(%rbp,%v10,2\)\{%k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 90 01 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k1\}\{nt\}
[ 	]*[a-f0-9]+:	d6 10 02 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}
[ 	]*[a-f0-9]+:	d6 10 0a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float16\}
[ 	]*[a-f0-9]+:	d6 10 1a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{srgb8\}
[ 	]*[a-f0-9]+:	d6 10 22 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8\}
[ 	]*[a-f0-9]+:	d6 10 2a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8\}
[ 	]*[a-f0-9]+:	d6 10 32 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm8\}
[ 	]*[a-f0-9]+:	d6 10 3a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm8\}
[ 	]*[a-f0-9]+:	d6 10 42 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16\}
[ 	]*[a-f0-9]+:	d6 10 4a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16\}
[ 	]*[a-f0-9]+:	d6 10 52 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm16\}
[ 	]*[a-f0-9]+:	d6 10 5a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{snorm16\}
[ 	]*[a-f0-9]+:	d6 10 62 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint8i\}
[ 	]*[a-f0-9]+:	d6 10 6a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint8i\}
[ 	]*[a-f0-9]+:	d6 10 72 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{uint16i\}
[ 	]*[a-f0-9]+:	d6 10 7a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{sint16i\}
[ 	]*[a-f0-9]+:	d6 10 82 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10A\}
[ 	]*[a-f0-9]+:	d6 10 8a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10B\}
[ 	]*[a-f0-9]+:	d6 10 92 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm10C\}
[ 	]*[a-f0-9]+:	d6 10 9a 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{unorm2D\}
[ 	]*[a-f0-9]+:	d6 10 a2 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float11A\}
[ 	]*[a-f0-9]+:	d6 10 aa 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float11B\}
[ 	]*[a-f0-9]+:	d6 10 b2 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float10C\}
[ 	]*[a-f0-9]+:	d6 10 b2 1b 84 55 99 00 00 00 	vgatherpf2d 0x99\(%rbp,%v10,2\)\{%k2\}\{float10C\}
[ 	]*[a-f0-9]+:	66 62 f4 c3             	bitinterleave11 %ax,%bx
[ 	]*[a-f0-9]+:	66 62 94 ca             	bitinterleave11 %r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f5 c3             	bitinterleave21 %ax,%bx
[ 	]*[a-f0-9]+:	66 62 95 ca             	bitinterleave21 %r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f2 c3             	bsff   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 92 ca             	bsff   %r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f0 c3             	bsfi   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 90 ca             	bsfi   %r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f3 c3             	bsrf   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 93 ca             	bsrf   %r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f1 c3             	bsri   %ax,%bx
[ 	]*[a-f0-9]+:	66 62 91 ca             	bsri   %r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f8 c3             	countbits %ax,%bx
[ 	]*[a-f0-9]+:	66 62 98 ca             	countbits %r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f9 c3             	quadmask16 %ax,%bx
[ 	]*[a-f0-9]+:	66 62 99 ca             	quadmask16 %r9w,%r10w
[ 	]*[a-f0-9]+:	62 f4 c3                	bitinterleave11 %eax,%ebx
[ 	]*[a-f0-9]+:	62 94 ca                	bitinterleave11 %r9d,%r10d
[ 	]*[a-f0-9]+:	62 f5 c3                	bitinterleave21 %eax,%ebx
[ 	]*[a-f0-9]+:	62 95 ca                	bitinterleave21 %r9d,%r10d
[ 	]*[a-f0-9]+:	62 f2 c3                	bsff   %eax,%ebx
[ 	]*[a-f0-9]+:	62 92 ca                	bsff   %r9d,%r10d
[ 	]*[a-f0-9]+:	62 f0 c3                	bsfi   %eax,%ebx
[ 	]*[a-f0-9]+:	62 90 ca                	bsfi   %r9d,%r10d
[ 	]*[a-f0-9]+:	62 f3 c3                	bsrf   %eax,%ebx
[ 	]*[a-f0-9]+:	62 93 ca                	bsrf   %r9d,%r10d
[ 	]*[a-f0-9]+:	62 f1 c3                	bsri   %eax,%ebx
[ 	]*[a-f0-9]+:	62 91 ca                	bsri   %r9d,%r10d
[ 	]*[a-f0-9]+:	62 f8 c3                	countbits %eax,%ebx
[ 	]*[a-f0-9]+:	62 98 ca                	countbits %r9d,%r10d
[ 	]*[a-f0-9]+:	62 f9 c3                	quadmask16 %eax,%ebx
[ 	]*[a-f0-9]+:	62 99 ca                	quadmask16 %r9d,%r10d
[ 	]*[a-f0-9]+:	62 e4 c3                	bitinterleave11 %rax,%rbx
[ 	]*[a-f0-9]+:	62 84 cf                	bitinterleave11 %r9,%r15
[ 	]*[a-f0-9]+:	62 e5 c3                	bitinterleave21 %rax,%rbx
[ 	]*[a-f0-9]+:	62 85 cf                	bitinterleave21 %r9,%r15
[ 	]*[a-f0-9]+:	62 e2 c3                	bsff   %rax,%rbx
[ 	]*[a-f0-9]+:	62 82 cf                	bsff   %r9,%r15
[ 	]*[a-f0-9]+:	62 e0 c3                	bsfi   %rax,%rbx
[ 	]*[a-f0-9]+:	62 80 cf                	bsfi   %r9,%r15
[ 	]*[a-f0-9]+:	62 e3 c3                	bsrf   %rax,%rbx
[ 	]*[a-f0-9]+:	62 83 cf                	bsrf   %r9,%r15
[ 	]*[a-f0-9]+:	62 e1 c3                	bsri   %rax,%rbx
[ 	]*[a-f0-9]+:	62 81 cf                	bsri   %r9,%r15
[ 	]*[a-f0-9]+:	62 e8 c3                	countbits %rax,%rbx
[ 	]*[a-f0-9]+:	62 88 cf                	countbits %r9,%r15
[ 	]*[a-f0-9]+:	62 e9 c3                	quadmask16 %rax,%rbx
[ 	]*[a-f0-9]+:	62 89 cf                	quadmask16 %r9,%r15
[ 	]*[a-f0-9]+:	62 eb 01                	clevict1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 01                	clevict1 \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 09                	clevict2 \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 09                	clevict2 \(%rcx\)
[ 	]*[a-f0-9]+:	62 cb 39                	vprefetch1 \$0x7,\(%rcx\)
[ 	]*[a-f0-9]+:	62 cb 39                	vprefetch1 \$0x7,\(%rcx\)
[ 	]*[a-f0-9]+:	62 db 11                	vprefetch2 \$0x2,\(%rcx\)
[ 	]*[a-f0-9]+:	62 db 11                	vprefetch2 \$0x2,\(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 11                	ldvxcsr \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 11                	ldvxcsr \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 19                	stvxcsr \(%rcx\)
[ 	]*[a-f0-9]+:	62 eb 19                	stvxcsr \(%rcx\)
[ 	]*[a-f0-9]+:	66 62 df c0             	delay  %ax
[ 	]*[a-f0-9]+:	66 62 9f c1             	delay  %r9w
[ 	]*[a-f0-9]+:	66 62 df c8             	spflt  %ax
[ 	]*[a-f0-9]+:	66 62 9f c9             	spflt  %r9w
[ 	]*[a-f0-9]+:	62 df c0                	delay  %eax
[ 	]*[a-f0-9]+:	62 9f c1                	delay  %r9d
[ 	]*[a-f0-9]+:	62 df c8                	spflt  %eax
[ 	]*[a-f0-9]+:	62 9f c9                	spflt  %r9d
[ 	]*[a-f0-9]+:	62 cf c0                	delay  %rax
[ 	]*[a-f0-9]+:	62 8f c1                	delay  %r9
[ 	]*[a-f0-9]+:	62 cf c8                	spflt  %rax
[ 	]*[a-f0-9]+:	62 8f c9                	spflt  %r9
[ 	]*[a-f0-9]+:	d6 00 50 04 c0          	delay  \$0xa
[ 	]*[a-f0-9]+:	66 62 f6 c3 0b 00 02 1f 	insertfield \$0x1f,\$0x2,\$0xb,%ax,%bx
[ 	]*[a-f0-9]+:	66 62 96 ca 0b 00 02 1f 	insertfield \$0x1f,\$0x2,\$0xb,%r9w,%r10w
[ 	]*[a-f0-9]+:	66 62 f7 c3 0b 00 02 1f 	rotatefield \$0x1f,\$0x2,\$0xb,%ax,%bx
[ 	]*[a-f0-9]+:	66 62 97 ca 0b 00 02 1f 	rotatefield \$0x1f,\$0x2,\$0xb,%r9w,%r10w
[ 	]*[a-f0-9]+:	62 f6 c3 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%eax,%ebx
[ 	]*[a-f0-9]+:	62 96 ca 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%r9d,%r10d
[ 	]*[a-f0-9]+:	62 f7 c3 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%eax,%ebx
[ 	]*[a-f0-9]+:	62 97 ca 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%r9d,%r10d
[ 	]*[a-f0-9]+:	62 e6 c3 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%rax,%rbx
[ 	]*[a-f0-9]+:	62 86 cf 0b 00 02 1f    	insertfield \$0x1f,\$0x2,\$0xb,%r9,%r15
[ 	]*[a-f0-9]+:	62 e7 c3 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%rax,%rbx
[ 	]*[a-f0-9]+:	62 87 cf 0b 00 02 1f    	rotatefield \$0x1f,\$0x2,\$0xb,%r9,%r15
[ 	]*[a-f0-9]+:	d6 17 11 45 d6          	vadcpi %v30\{cdab\},%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	66 d6 67 a1 48 d6       	vaddnpd %v30\{dddd\},%v20,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 78 56 34 12 	vadcpi 0x12345678,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 55 00       	vadcpi 0x0\(%rbp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 95 99 00 00 00 	vadcpi 0x99\(%rbp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 05 11 45 97 99 00 00 00 	vadcpi 0x99\(%r15\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 15 99 00 00 00 	vadcpi 0x99\(%rip\),%k2,%v10\{%k1\}        # 65b8 <_start\+0x65b8>
[ 	]*[a-f0-9]+:	d6 04 11 45 94 24 99 00 00 00 	vadcpi 0x99\(%rsp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 05 11 45 94 24 99 00 00 00 	vadcpi 0x99\(%r12\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 67 ff ff ff 	vadcpi 0xffffffffffffff67,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 65 67 ff ff ff 	vadcpi -0x99\(,%riz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 94 23 67 ff ff ff 	vadcpi -0x99\(%rbx,%riz,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 94 63 67 ff ff ff 	vadcpi -0x99\(%rbx,%riz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 07 11 45 94 bc 67 ff ff ff 	vadcpi -0x99\(%r12,%r15,4\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 07 11 45 94 f8 67 ff ff ff 	vadcpi -0x99\(%r8,%r15,8\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 06 11 45 94 a5 67 ff ff ff 	vadcpi -0x99\(%rbp,%r12,4\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 06 11 45 94 2c 67 ff ff ff 	vadcpi -0x99\(%rsp,%r13,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 00 00 00 00 	vgatherd 0x0\(,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 00 00 00 00 	vgatherd 0x0\(,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 0d 67 ff ff ff 	vgatherd -0x99\(,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 15 67 ff ff ff 	vgatherd -0x99\(,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 14 08       	vgatherd \(%rax,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 14 10       	vgatherd \(%rax,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 01 18 94 48 67 ff ff ff 	vgatherd -0x99\(%rax,%v1,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 14 01 18 94 50 67 ff ff ff 	vgatherd -0x99\(%rax,%v10,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 05 01 18 94 8c 67 ff ff ff 	vgatherd -0x99\(%r12,%v1,4\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 15 01 18 94 94 67 ff ff ff 	vgatherd -0x99\(%r12,%v10,4\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	d6 04 11 45 14 25 34 12 00 00 	vadcpi 0x1234,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 55 00    	vadcpi 0x0\(%ebp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 95 99 00 00 00 	vadcpi 0x99\(%ebp\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 25 99 00 00 00 	addr32 vadcpi 0x99,%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 14 65 99 00 00 00 	addr32 vadcpi 0x99\(,%eiz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 20 99 00 00 00 	vadcpi 0x99\(%eax,%eiz,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 60 99 00 00 00 	vadcpi 0x99\(%eax,%eiz,2\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 98 99 00 00 00 	vadcpi 0x99\(%eax,%ebx,4\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 cc 99 00 00 00 	vadcpi 0x99\(%esp,%ecx,8\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 11 45 94 15 99 00 00 00 	vadcpi 0x99\(%ebp,%edx,1\),%k2,%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 14 08    	vgatherd \(%eax,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 14 10    	vgatherd \(%eax,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 14 08    	vgatherd \(%eax,%v1,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 14 10    	vgatherd \(%eax,%v10,1\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 48 67 ff ff ff 	vgatherd -0x99\(%eax,%v1,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 50 67 ff ff ff 	vgatherd -0x99\(%eax,%v10,2\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 04 01 18 94 8b 67 ff ff ff 	vgatherd -0x99\(%ebx,%v1,4\),%v10\{%k1\}
[ 	]*[a-f0-9]+:	67 d6 14 01 18 94 93 67 ff ff ff 	vgatherd -0x99\(%ebx,%v10,4\),%v10\{%k1\}
#pass

.*: Assembler messages:
.*:3: Error: .*
.*:4: Error: .*
.*:5: Error: .*
.*:6: Error: .*
.*:7: Error: .*
.*:8: Error: .*
.*:9: Error: .*
.*:10: Error: .*
.*:11: Error: .*
.*:12: Error: .*
.*:13: Error: .*
.*:14: Error: .*
.*:15: Error: .*
.*:16: Error: .*
.*:17: Error: .*
.*:18: Error: .*
.*:19: Error: .*
.*:20: Error: .*
.*:21: Error: .*
.*:22: Error: .*
.*:23: Error: .*
.*:24: Error: .*
.*:25: Error: .*
.*:26: Error: .*
.*:27: Error: .*
.*:28: Error: .*
.*:29: Error: .*
.*:30: Error: .*
.*:31: Error: .*
.*:32: Error: .*
.*:33: Error: .*
.*:34: Error: .*
.*:35: Error: .*
.*:37: Error: .*
.*:38: Error: .*
.*:39: Error: .*
.*:40: Error: .*
.*:41: Error: .*
.*:42: Error: .*
.*:43: Error: .*
.*:44: Error: .*
.*:45: Error: .*
.*:46: Error: .*
.*:47: Error: .*
.*:48: Error: .*
.*:49: Error: .*
.*:50: Error: .*
.*:51: Error: .*
.*:52: Error: .*
.*:53: Error: .*
.*:54: Error: .*
.*:57: Error: .*
.*:58: Error: .*
.*:59: Error: .*
.*:60: Error: .*
.*:60: Error: .*
.*:61: Error: .*
.*:62: Error: .*
.*:62: Error: .*
.*:63: Error: .*
.*:64: Error: .*
.*:65: Error: .*
.*:66: Error: .*
.*:67: Error: .*
.*:68: Error: .*
.*:69: Error: .*
.*:70: Error: .*
.*:71: Error: .*
GAS LISTING .*


[ 	]*1[ 	]+\.text
[ 	]*2[ 	]+_start:
[ 	]*3[ 	]+vkmov %k0,%rax
[ 	]*4[ 	]+vkmov %k0,%k3\{%k1\}
[ 	]*5[ 	]+vkmov %k0\{nt\},%k3\{%k1\}
[ 	]*6[ 	]+vkmov %k0\{srgb8\}\{NT\}\{1to16\},%k3\{%k1\}
[ 	]*7[ 	]+vfixupps \$0, %v2 \{unorm8\}, %v0 \{%k1\}
[ 	]*8[ 	]+vfixupps \$0, \(%rax\) \{NT\}\{cccc\}, %v0 \{%k1\}
[ 	]*9[ 	]+vmaxabsps  %v2 \{dcba\}, %v1\{%k7\}, %v0
[ 	]*10[ 	]+vmaxabsps  %v2, %v1\{%k7\}, %v0\{dcba\}
[ 	]*11[ 	]+vstored %v0\{cdab\}\{sint8\},\(%rax\)\{%k1\}\{NT\}
[ 	]*12[ 	]+vmaxabsps  %v2, %v1, %v0\{dcba\}
[ 	]*13[ 	]+vcvtinsps2f11 \{rn\},\$0,%v2\{aaaa\},%v0\{%k1\}
[ 	]*14[ 	]+vcvtinsps2f11 \$0,\{\.0b\},%v2\{aaaa\},%v0\{%k1\}
[ 	]*15[ 	]+vcvtinsps2u10 \{\.0b\},%v2,%v0\{%k1\}
[ 	]*16[ 	]+vcvtinsps2f11 \{\.0b\},\{rn\},%v2,%v0
[ 	]*17[ 	]+vcvtinsps2f11 \$0,\{rd\},%v2,%v0
[ 	]*18[ 	]+vcmppi %k2\{%k1\}, %v2, %v1, \{eq\}
[ 	]*19[ 	]+vmaxabsps  %v2, %v1, %v0 \{%k8\}
[ 	]*20[ 	]+vmaxabsps  %v2 \{xxxx\}, %v1, %v0
[ 	]*21[ 	]+vcmpbmskpu \{float16\}, \{le\}, %v30\{bbbb\}, %v10, %k0\{%k2\}
[ 	]*22[ 	]+vcmpbmskpu \{snorm16\}, \{le\}, 0x11223344\(%r10,%r15\)\{uint8\}, %v6, %k1\{%k2\}
[ 	]*23[ 	]+vcmppd \{sint8i\}, %v28\{cdab\}, %v8, %k6\{%k2\}
[ 	]*24[ 	]+vcmppd \{float16\}, 287454020\(%rcx,%rdi\)\{4to8\}, %v27, %k4\{%k4\}
[ 	]*25[ 	]+vcmppi \{float16rz\}, %v7\{aaaa\}, %v16, %k6\{%k1\}
[ 	]*26[ 	]+vcvtinsps2f11 \$4, \{rz\}, %v4\{badc\}, %v20\{%k4\}
[ 	]*27[ 	]+vcvtinsps2f11 \$5, \{rz\}, %v4\{badc\}, %v20\{%k4\}
[ 	]*28[ 	]+vcvtinsps2f11 \$6, \{rz\}, %v4\{badc\}, %v20\{%k4\}
[ 	]*29[ 	]+vcvtinsps2f11 \$7, \{rz\}, %v4\{badc\}, %v20\{%k4\}
[ 	]*30[ 	]+vcvtinsps2f11 \$8, \{rz\}, %v4\{badc\}, %v20\{%k4\}
[ 	]*31[ 	]+vcvtinsps2u10 \$4, %v4\{badc\}, %v20\{%k4\}
[ 	]*32[ 	]+vcvtinsps2u10 \$5, %v4\{badc\}, %v20\{%k4\}
[ 	]*33[ 	]+vcvtinsps2u10 \$6, %v4\{badc\}, %v20\{%k4\}
[ 	]*34[ 	]+vcvtinsps2u10 \$7, %v4\{badc\}, %v20\{%k4\}
[ 	]*35[ 	]+vcvtinsps2u10 \$8, %v4\{badc\}, %v20\{%k4\}
[ 	]*36[ 	]+\# All the following should be illegal for L1OMni
[ 	]*37[ 	]+vinsertfieldpi \$0x0, \$0x1F, \$0, \$0x1FFFFF, %v8, %v3\{%k7\}
[ 	]*38[ 	]+vinsertfieldpi \$0x0, \$0x1F, \$0, \{unorm16\}, %v8, %v3\{%k7\}
[ 	]*39[ 	]+vcvtpd2pi \$6, \{rd\}, %v9\{bbbb\}, %v11\{%k3\}
[ 	]*40[ 	]+vinsertfieldpi \$0x0, \$0x1F, \$0, \{unorm16\}, %v8, %v3\{%k7\}
[ 	]*41[ 	]+vinsertfieldpi \$0x0, \$0x1F, \$0, \$0x1FFFFF, %v8, %v3\{%k7\}
[ 	]*42[ 	]+vcvtpi2pd \$2, 1122867\(%rcx,%r14,0x4\)\{sint8\},%v19
[ 	]*43[ 	]+vgatherd \(%rax,%v3,4\), %v0
[ 	]*44[ 	]+vgatherd \(%rax,%v3,4\), %v0\{%k0\}
[ 	]*45[ 	]+vgatherpfd \(%rax,%v3,4\)\{%k0\}
[ 	]*46[ 	]+vscatterd %v11,0x80\(,%v1,1\)
[ 	]*47[ 	]+vscatterd %v11,0x80\(,%v1,1\)\{%k0\}
[ 	]*48[ 	]+vscatterpfd 0x80\(,%v1,1\)\{%k0\}
[ 	]*49[ 	]+vgatherd \(%rax\), %v1\{%k1\}
[ 	]*50[ 	]+vgatherpfd 4\(%rax,%rbx\)\{%k1\}
[ 	]*51[ 	]+vscatterd %v11,0x80\{%k1\}
[ 	]*52[ 	]+vscatterpfd 0x80\(,%rbx,1\)\{%k1\}
[ 	]*53[ 	]+vaddsetsps  %v2, %v1, %v0
[ 	]*54[ 	]+vaddsetspi  %v2, %v1, %v0
[ 	]*55[ 	]+
[ 	]*56[ 	]+\.intel_syntax noprefix
[ 	]*57[ 	]+vkmov k0,rax
GAS LISTING .*


[ 	]*58[ 	]+vkmov rax,k1
[ 	]*59[ 	]+vgatherd v0,\[rax\+v3\*4\]
[ 	]*60[ 	]+vgatherd v0\{k0\},\[rax\+v3\*4\]
[ 	]*61[ 	]+vgatherpfd k0,\[rax\+v3\*4\]
[ 	]*62[ 	]+vscatterd \[v1\*1\+0x80\]\{k0\},v11
[ 	]*63[ 	]+vscatterd \[v1\*1\+0x80\],v11
[ 	]*64[ 	]+vscatterpfd k0,\[rax\+v6\*4\]\{nt\}
[ 	]*65[ 	]+vgatherd v0\{k2\},\[rax\]
[ 	]*66[ 	]+vgatherpfd k1,\[rax\]
[ 	]*67[ 	]+vscatterd PTR \[0x80\]\{k1\},v11
[ 	]*68[ 	]+vscatterpfd k1,\[rax\]\{nt\}
[ 	]*69[ 	]+vaddsetsps  v2, v1, v0
[ 	]*70[ 	]+vaddsetspi  v2, v1, v0
[ 	]*71[ 	]+delay \$32

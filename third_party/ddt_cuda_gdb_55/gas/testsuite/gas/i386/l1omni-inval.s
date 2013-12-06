	.text
_start:
	vkmov %k0,%rax
	vkmov %k0,%k3{%k1}
	vkmov %k0{nt},%k3{%k1}
	vkmov %k0{srgb8}{NT}{1to16},%k3{%k1}
	 vfixupps $0, %v2 {unorm8}, %v0 {%k1}
	 vfixupps $0, (%rax) {NT}{cccc}, %v0 {%k1}
	vmaxabsps  %v2 {dcba}, %v1{%k7}, %v0
	vmaxabsps  %v2, %v1{%k7}, %v0{dcba}
	vstored %v0{cdab}{sint8},(%rax){%k1}{NT}
	vmaxabsps  %v2, %v1, %v0{dcba}
	vcvtinsps2f11 {rn},$0,%v2{aaaa},%v0{%k1}
	vcvtinsps2f11 $0,{.0b},%v2{aaaa},%v0{%k1}
	 vcvtinsps2u10 {.0b},%v2,%v0{%k1}
	 vcvtinsps2f11 {.0b},{rn},%v2,%v0
	 vcvtinsps2f11 $0,{rd},%v2,%v0
	vcmppi %k2{%k1}, %v2, %v1, {eq}
	vmaxabsps  %v2, %v1, %v0 {%k8}
	vmaxabsps  %v2 {xxxx}, %v1, %v0
vcmpbmskpu {float16}, {le}, %v30{bbbb}, %v10, %k0{%k2}
vcmpbmskpu {snorm16}, {le}, 0x11223344(%r10,%r15){uint8}, %v6, %k1{%k2}
vcmppd {sint8i}, %v28{cdab}, %v8, %k6{%k2}
vcmppd {float16}, 287454020(%rcx,%rdi){4to8}, %v27, %k4{%k4}
vcmppi {float16rz}, %v7{aaaa}, %v16, %k6{%k1}
vcvtinsps2f11 $4, {rz}, %v4{badc}, %v20{%k4}
vcvtinsps2f11 $5, {rz}, %v4{badc}, %v20{%k4}
vcvtinsps2f11 $6, {rz}, %v4{badc}, %v20{%k4}
vcvtinsps2f11 $7, {rz}, %v4{badc}, %v20{%k4}
vcvtinsps2f11 $8, {rz}, %v4{badc}, %v20{%k4}
vcvtinsps2u10 $4, %v4{badc}, %v20{%k4}
vcvtinsps2u10 $5, %v4{badc}, %v20{%k4}
vcvtinsps2u10 $6, %v4{badc}, %v20{%k4}
vcvtinsps2u10 $7, %v4{badc}, %v20{%k4}
vcvtinsps2u10 $8, %v4{badc}, %v20{%k4}
# All the following should be illegal for L1OMni
vinsertfieldpi $0x0, $0x1F, $0, $0x1FFFFF, %v8, %v3{%k7}
vinsertfieldpi $0x0, $0x1F, $0, {unorm16}, %v8, %v3{%k7}
vcvtpd2pi $6, {rd}, %v9{bbbb}, %v11{%k3}
vinsertfieldpi $0x0, $0x1F, $0, {unorm16}, %v8, %v3{%k7}
vinsertfieldpi $0x0, $0x1F, $0, $0x1FFFFF, %v8, %v3{%k7}
vcvtpi2pd $2, 1122867(%rcx,%r14,0x4){sint8},%v19
	vgatherd (%rax,%v3,4), %v0
	vgatherd (%rax,%v3,4), %v0{%k0}
	vgatherpfd (%rax,%v3,4){%k0}
	vscatterd %v11,0x80(,%v1,1)
	vscatterd %v11,0x80(,%v1,1){%k0}
	vscatterpfd 0x80(,%v1,1){%k0}
	vgatherd (%rax), %v1{%k1}
	vgatherpfd 4(%rax,%rbx){%k1}
	vscatterd %v11,0x80{%k1}
	vscatterpfd 0x80(,%rbx,1){%k1}
	vaddsetsps  %v2, %v1, %v0
	vaddsetspi  %v2, %v1, %v0

	.intel_syntax noprefix
	vkmov k0,rax
	vkmov rax,k1
	vgatherd v0,[rax+v3*4]
	vgatherd v0{k0},[rax+v3*4]
	vgatherpfd k0,[rax+v3*4]
	vscatterd [v1*1+0x80]{k0},v11
	vscatterd [v1*1+0x80],v11
	vscatterpfd k0,[rax+v6*4]{nt}
	vgatherd v0{k2},[rax]
	vgatherpfd k1,[rax]
	vscatterd PTR [0x80]{k1},v11
	vscatterpfd k1,[rax]{nt}
	vaddsetsps  v2, v1, v0
	vaddsetspi  v2, v1, v0
	delay $32

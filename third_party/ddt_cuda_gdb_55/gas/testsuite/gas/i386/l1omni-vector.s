	.text
_start:
	vmaxabsps  %v2, %v1, %v0
	vmaxabsps  %v2 {dcba}, %v1, %v0
	vmaxabsps  %v2 {dcba}, %v1, %v0 {%k7}
	vmaxabsps  %v2 {cdab}, %v1, %v0 {%k7}
	vmaxabsps  %v2 {badc}, %v1, %v0 {%k7}
	vmaxabsps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmaxabsps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmaxabsps  %v2 {cccc}, %v1, %v0 {%k7}
	vmaxabsps  %v2 {dddd}, %v1, %v0 {%k7}
	vmaxabsps  %v2 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v7 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v8 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v9 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v15 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v16 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v17 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v31 {dacb}, %v1, %v0 {%k7}
	vmaxabsps  %v31 {dacb}, %v1, %v8 {%k7}
	vmaxabsps  %v31 {dacb}, %v1, %v9 {%k7}
	vmaxabsps  %v31 {dacb}, %v1, %v15 {%k7}
	vmaxabsps  %v31 {dacb}, %v8, %v15 {%k7}
	vmaxabsps  %v31 {dacb}, %v9, %v15 {%k7}
	vmaxabsps  %v31 {dacb}, %v15, %v15 {%k7}
	vmaxabsps  %v31 {dacb}, %v31, %v31 {%k7}

	vmaxabsps  (%rax), %v1, %v0{%k7}
	vmaxabsps  (%rax) {1to16}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {4to16}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {sint16}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {float16}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {uint8}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {nt}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {NT}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmaxabsps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}

	vmaxabsps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxabsps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxabsps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}

	vaddsetcpi %v2, %k1, %v0
	vaddsetcpi %v2 {dcba}, %k0, %v0 {%k7}

	vaddsetcpi 4(%r8,%r9,1){NT}, %k2, %v1{%k7}
	vaddsetcpi 64(%r8,%r9,1){NT}, %k3, %v2{%k7}

	vcvtps2srgb8 %v2, %v0
	vcvtps2srgb8 %v2, %v0 {%k1}
	vcvtps2srgb8 %v2 {aaaa}, %v0 {%k1}
	vcvtps2srgb8 %v2 {bbbb}, %v0 {%k1}
	vcvtps2srgb8 %v2 {cccc}, %v0 {%k1}
	vcvtps2srgb8 %v31 {cccc}, %v31 {%k7}

	vcvtps2srgb8 (%rax), %v0{%k1}
	vcvtps2srgb8 (%r8), %v0{%k1}
	vcvtps2srgb8 (%r15), %v0{%k1}
	vcvtps2srgb8 (%rax){unorm8}, %v0{%k1}
	vcvtps2srgb8 (%rax){NT}, %v0{%k1}
	vcvtps2srgb8 (%rax){NT}{unorm8}, %v0{%k1}
	vcvtps2srgb8 (%rax){unorm8}{NT}, %v0{%k1}

	vcvtps2srgb8 4(%r8,%r9,1){NT}, %v1{%k7}
	vcvtps2srgb8 64(%r8,%r9,1){NT}, %v2{%k7}

	vcvtps2srgb8 %v2 {dcba}, %v0 {%k7}

	vfixupps $0, %v2, %v0
	vfixupps $0, %v2, %v0 {%k1}
	vfixupps $0, %v2{aaaa}, %v0 {%k1}
	vfixupps $0, %v2{bbbb}, %v0 {%k1}
	vfixupps $0, %v2{cccc}, %v0 {%k1}
	vfixupps $0, %v31{cccc}, %v31 {%k7}
	vfixupps $0, (%rax) {unorm8}, %v0 {%k1}
	vfixupps $0, (%rax) {NT}, %v0 {%k1}
	vfixupps $0, (%rax) {NT}{unorm8}, %v0 {%k1}
	vfixupps $0, (%rax) {unorm8}{NT}, %v0 {%k1}
	vfixupps $1, (%rax) {unorm8}{NT}, %v0 {%k1}
	vfixupps $7, (%rax) {unorm8}{NT}, %v0 {%k1}
	vfixupps $127, (%rax) {unorm8}{NT}, %v0 {%k1}
	vfixupps $256, (%rax) {unorm8}{NT}, %v0 {%k1}
	vfixupps $16384, (%rax) {unorm8}{NT}, %v0 {%k1}
	vfixupps $65535, (%rax) {unorm8}{NT}, %v0 {%k1}
	vfixupps $65536, (%rax) {unorm8}{NT}, %v0 {%k1}

	vloadd (%rax), %v0{%k1}
	vloadd (%rax) {16to16}, %v0{%k1}
	vloadd (%rax) {1to16}, %v0{%k1}
	vloadd (%rax) {NT}, %v0{%k1}
	vloadd (%rax) {srgb8}, %v0{%k1}
	vloadd (%rax) {srgb8}{1to16}, %v0{%k1}
	vloadd (%rax) {4to16}{srgb8}, %v0{%k1}
	vloadd (%rax) {4to16}{NT}, %v0{%k1}
	vloadd (%rax) {srgb8}{NT}, %v0{%k1}
	vloadd (%rax) {NT}{1to16}{srgb8}, %v0{%k1}
	vloadd (%rax) {srgb8}{NT}{1to16}, %v0{%k1}

	vstored %v0,(%rax){%k1}
	vstored %v0,(%rax){%k1}{NT}
	vstored %v0{dcba},(%rax){%k1}
	vstored %v0{sint8},(%rax){%k1}
	vstored %v0{sint8}{dcba},(%rax){%k1}
	vstored %v0{dcba}{sint8},(%rax){%k1}
	vstored %v0{dcba},(%rax){%k1}{NT}
	vstored %v0{sint8},(%rax){%k1}{NT}
	vstored %v0{a}{sint8},(%rax){%k1}{NT}
	vstored %v0{a}{sint8},(%rax){%k1}{NT}
	vstored %v0{sint8}{a},(%rax){%k1}{NT}
	vstored %v0{float16rz},128(%rax){%k1}

	vadcpi %v2 {aaaa}, %k1, %v0 {%k1}
	vadcpi %v2 {bbbb}, %k1, %v0 {%k1}
	vadcpi %v2 {cccc}, %k1, %v0 {%k1}
	vadcpi %v31 {cccc}, %k7, %v31 {%k7}
	vadcpi %v2, %k1, %v0 {%k1}

	vadcpi (%rax), %k1, %v0 {%k1}
	vadcpi (%r8), %k1, %v0 {%k1}
	vadcpi (%r15), %k1, %v0 {%k1}
	vadcpi (%rax) {sint8}, %k1, %v0 {%k1}
	vadcpi (%rax) {NT}, %k1, %v0 {%k1}
	vadcpi (%rax) {NT}{sint8}, %k1, %v0 {%k1}
	vadcpi (%rax) {sint8}{NT}, %k1, %v0 {%k1}

	vaddpi  %v2, %v1, %v0
	vaddpi  %v2 {dcba}, %v1, %v0
	vaddpi  %v2 {dcba}, %v1, %v0 {%k7}
	vaddpi  %v2 {cdab}, %v1, %v0 {%k7}
	vaddpi  %v2 {badc}, %v1, %v0 {%k7}
	vaddpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vaddpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vaddpi  %v2 {cccc}, %v1, %v0 {%k7}
	vaddpi  %v2 {dddd}, %v1, %v0 {%k7}
	vaddpi  %v2 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v7 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v8 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v9 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v15 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v16 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v17 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v31 {dacb}, %v1, %v0 {%k7}
	vaddpi  %v31 {dacb}, %v1, %v8 {%k7}
	vaddpi  %v31 {dacb}, %v1, %v9 {%k7}
	vaddpi  %v31 {dacb}, %v1, %v15 {%k7}
	vaddpi  %v31 {dacb}, %v8, %v15 {%k7}
	vaddpi  %v31 {dacb}, %v9, %v15 {%k7}
	vaddpi  %v31 {dacb}, %v15, %v15 {%k7}
	vaddpi  %v31 {dacb}, %v31, %v31 {%k7}

	vaddpi  (%rax), %v1, %v0{%k7}
	vaddpi  (%rax) {1to16}, %v1, %v0{%k7}
	vaddpi  (%rax) {4to16}, %v1, %v0{%k7}
	vaddpi  (%rax) {sint8}, %v1, %v0{%k7}
	vaddpi  (%rax) {sint16}, %v1, %v0{%k7}
	vaddpi  (%rax) {uint16}, %v1, %v0{%k7}
	vaddpi  (%rax) {uint8}, %v1, %v0{%k7}
	vaddpi  (%rax) {nt}, %v1, %v0{%k7}
	vaddpi  (%rax) {NT}, %v1, %v0{%k7}
	vaddpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vaddpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}

	vaddpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}

	vcvtinsps2f11 $0,{rn},%v2,%v0
	vcvtinsps2f11 $0,{rn},%v2,%v0{%k1}
	vcvtinsps2f11 $0,{rn},%v2{aaaa},%v0{%k1}
	vcvtinsps2f11 $0,{rn},%v2{bbbb},%v0{%k1}
	vcvtinsps2f11 $0,{rn},%v2{cccc},%v0{%k1}
	vcvtinsps2f11 $0,{rn},%v31{cccc},%v31{%k7}
	vcvtinsps2f11 $0,{rn},(%rax),%v0{%k1}
	vcvtinsps2f11 $0,{rn},(%r8),%v0{%k1}
	vcvtinsps2f11 $0,{rn},(%r15),%v0{%k1}
	vcvtinsps2f11 $0,{rn},(%rax){unorm8},%v0{%k1}
	vcvtinsps2f11 $0,{rn},(%rax){NT},%v0{%k1}
	vcvtinsps2f11 $0,{rn},(%rax){NT}{unorm8},%v0{%k1}
#	vcvtinsps2f11 $0,{rd},(%rax){unorm8}{NT},%v0{%k1}
#	vcvtinsps2f11 $0,{ru},(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $0,{rz},(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $0,{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $1,{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $2,{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $2,{rn},0x16(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $2,{rn},0x16(%rax,%rbx,4){unorm8}{NT},%v0{%k1}

	vcvtpi2ps {.0b},%v2,%v0
	vcvtpi2ps {.0b},%v2,%v0{%k1}
	vcvtpi2ps {.0b},%v2{aaaa},%v0{%k1}
	vcvtpi2ps {.0b},%v2{bbbb},%v0{%k1}
	vcvtpi2ps {.0b},%v2{cccc},%v0{%k1}
	vcvtpi2ps {.0b},%v31{cccc},%v31{%k7}
	vcvtpi2ps {.0b},(%rax),%v0{%k1}
	vcvtpi2ps {.0b},(%r8),%v0{%k1}
	vcvtpi2ps {.0b},(%r15),%v0{%k1}
	vcvtpi2ps {.0b},(%rax){sint8},%v0{%k1}
	vcvtpi2ps {.0b},(%rax){NT},%v0{%k1}
	vcvtpi2ps {.0b},(%rax){NT}{sint8},%v0{%k1}
	vcvtpi2ps {.4b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.5b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.8b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.16b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.24b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.31b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.32b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.0b},16(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.0b},8(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.0b},4(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.0b},16(%rax){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.0b},16(%rax,%rbx,4){sint8}{NT},%v0{%k1}

	vcvtps2pi {.0b},{rn},%v2,%v0
	vcvtps2pi {.0b},{rn},%v2,%v0{%k1}
	vcvtps2pi {.0b},{rn},%v2{aaaa},%v0{%k1}
	vcvtps2pi {.0b},{rn},%v2{bbbb},%v0{%k1}
	vcvtps2pi {.0b},{rn},%v2{cccc},%v0{%k1}
	vcvtps2pi {.0b},{rn},%v31{cccc},%v31{%k7}
	vcvtps2pi {.0b},{rn},(%rax),%v0{%k1}
	vcvtps2pi {.0b},{rn},(%r8),%v0{%k1}
	vcvtps2pi {.0b},{rn},(%r15),%v0{%k1}
	vcvtps2pi {.0b},{rn},(%rax){unorm8},%v0{%k1}
	vcvtps2pi {.0b},{rn},(%rax){NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},(%rax){NT}{unorm8},%v0{%k1}
	vcvtps2pi {.4b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.5b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.8b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.16b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.24b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.31b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{rd},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{ru},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{rz},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},16(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},8(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},4(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},16(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},16(%rax,%rbx,4){unorm8}{NT},%v0{%k1}

	vinsertfieldpi $0, $0, $0, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $1, $0, $0, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $0, $1, $0, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $0, $0, $1, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $1, $0, $1, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $1, $1, $1, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $15, $7, $1, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $3, $2, $8, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $19, $3, $16, %v2, %v1, %v0 {%k1}
	vinsertfieldpi $19, $3, $16, %v2{cccc}, %v1, %v0 {%k7}
	vinsertfieldpi $19, $3, $16, 16(%rax){sint8}{NT}, %v1, %v0 {%k7}
	vinsertfieldpi $19, $3, $16, 1(%rax){sint8}{NT}, %v1, %v0 {%k7}
	vinsertfieldpi $19, $3, $16, 16(%rax,%rbx,4){sint8}{NT}, %v1, %v0 {%k7}
	vinsertfieldpi $19, $3, $16, 1(%rax,%rbx,4){sint8}{NT}, %v1, %v0 {%k7}

	vrotatefieldpi $0, $0, $0, %v2, %v0 {%k1}
	vrotatefieldpi $1, $0, $0, %v2, %v0 {%k1}
	vrotatefieldpi $0, $1, $0, %v2,  %v0 {%k1}
	vrotatefieldpi $0, $0, $1, %v2,  %v0 {%k1}
	vrotatefieldpi $1, $0, $1, %v2,  %v0 {%k1}
	vrotatefieldpi $1, $1, $1, %v2,  %v0 {%k1}
	vrotatefieldpi $15, $7, $1, %v2, %v0 {%k1}
	vrotatefieldpi $3, $2, $8, %v2,  %v0 {%k1}
	vrotatefieldpi $19, $3, $16, %v2,  %v0 {%k1}
	vrotatefieldpi $19, $3, $16, %v2{cccc}, %v0 {%k7}
	vrotatefieldpi $19, $3, $16, 16(%rax){sint8}{NT},  %v0 {%k7}
	vrotatefieldpi $19, $3, $16, 1(%rax){sint8}{NT},  %v0 {%k7}
	vrotatefieldpi $19, $3, $16, 16(%rax,%rbx,4){sint8}{NT}, %v0 {%k7}
	vrotatefieldpi $19, $3, $16, 1(%rax,%rbx,4){sint8}{NT}, %v0 {%k7}

	vcmppi {eq},%v1,%v0,%k1 {%k1}
	vcmppi {eq},%v1{aaaa},%v0,%k1 {%k1}
	vcmppi {eq},%v1{bbbb},%v0,%k1 {%k1}
	vcmppi {eq},%v1,%v0,%k7 {%k1}
	vcmppi {eq},%v1,%v0,%k1 {%k7}
	vcmppi {eq},%v1,%v31,%k1 {%k1}
	vcmppi {eq},%v31,%v0,%k1 {%k1}
	vcmppi {eq},%v1,%v0,%k1 {%k1}
	vcmppi {eq},%v1,%v0,%k1 {%k1}
	vcmppi {eq},%v1,%v0,%k1 {%k1}
	vcmppi {lt},%v1,%v0,%k1 {%k1}
	vcmppi {le},%v1,%v0,%k1 {%k1}
	vcmppi {neq},%v1,%v0,%k1 {%k1}
	vcmppi {nlt},%v1,%v0,%k1 {%k1}
	vcmppi {nle},%v1,%v0,%k1 {%k1}
	vcmppi {eq},(%rax),%v0,%k1 {%k1}
	vcmppi {eq},(%rax),%v0,%k1 {%k1}
	vcmppi {eq},(%rax),%v0,%k1 {%k1}
	vcmppi {eq},(%rax),%v0,%k1 {%k1}
	vcmppi {lt},(%rax),%v0,%k1 {%k1}
	vcmppi {le},(%rax),%v0,%k1 {%k1}
	vcmppi {neq},(%rax),%v0,%k1 {%k1}
	vcmppi {nlt},(%rax),%v0,%k1 {%k1}
	vcmppi {nle},(%rax),%v0,%k1 {%k1}
	vcmppi {eq},(%rax){NT},%v0,%k1 {%k1}
	vcmppi {eq},(%rax){sint8},%v0,%k1 {%k1}
	vcmppi {eq},(%rax){NT}{sint8},%v0,%k1 {%k1}
	vcmppi {eq},(%rax){sint8}{NT},%v0,%k1 {%k1}

	vcmppu {eq},%v1,%v0,%k1 {%k1}
	vcmppu {eq},%v1{aaaa},%v0,%k1 {%k1}
	vcmppu {eq},%v1{bbbb},%v0,%k1 {%k1}
	vcmppu {eq},%v1,%v0,%k7 {%k1}
	vcmppu {eq},%v1,%v0,%k1 {%k7}
	vcmppu {eq},%v1,%v31,%k1 {%k1}
	vcmppu {eq},%v31,%v0,%k1 {%k1}
	vcmppu {eq},%v1,%v0,%k1 {%k1}
	vcmppu {eq},%v1,%v0,%k1 {%k1}
	vcmppu {eq},%v1,%v0,%k1 {%k1}
	vcmppu {lt},%v1,%v0,%k1 {%k1}
	vcmppu {le},%v1,%v0,%k1 {%k1}
	vcmppu {neq},%v1,%v0,%k1 {%k1}
	vcmppu {nlt},%v1,%v0,%k1 {%k1}
	vcmppu {nle},%v1,%v0,%k1 {%k1}
	vcmppu {eq},(%rax),%v0,%k1 {%k1}
	vcmppu {eq},(%rax),%v0,%k1 {%k1}
	vcmppu {eq},(%rax),%v0,%k1 {%k1}
	vcmppu {eq},(%rax),%v0,%k1 {%k1}
	vcmppu {lt},(%rax),%v0,%k1 {%k1}
	vcmppu {le},(%rax),%v0,%k1 {%k1}
	vcmppu {neq},(%rax),%v0,%k1 {%k1}
	vcmppu {nlt},(%rax),%v0,%k1 {%k1}
	vcmppu {nle},(%rax),%v0,%k1 {%k1}
	vcmppu {eq},(%rax){NT},%v0,%k1 {%k1}
	vcmppu {eq},(%rax){sint8},%v0,%k1 {%k1}
	vcmppu {eq},(%rax){NT}{sint8},%v0,%k1 {%k1}
	vcmppu {eq},(%rax){sint8}{NT},%v0,%k1 {%k1}

	vcmpps {eq},%v1,%v0,%k1 {%k1}
	vcmpps {eq},%v1{aaaa},%v0,%k1 {%k1}
	vcmpps {eq},%v1{bbbb},%v0,%k1 {%k1}
	vcmpps {eq},%v1,%v0,%k7 {%k1}
	vcmpps {eq},%v1,%v0,%k1 {%k7}
	vcmpps {eq},%v1,%v31,%k1 {%k1}
	vcmpps {eq},%v31,%v0,%k1 {%k1}
	vcmpps {eq},%v1,%v0,%k1 {%k1}
	vcmpps {eq},%v1,%v0,%k1 {%k1}
	vcmpps {eq},%v1,%v0,%k1 {%k1}
	vcmpps {lt},%v1,%v0,%k1 {%k1}
	vcmpps {le},%v1,%v0,%k1 {%k1}
	vcmpps {neq},%v1,%v0,%k1 {%k1}
	vcmpps {nlt},%v1,%v0,%k1 {%k1}
	vcmpps {nle},%v1,%v0,%k1 {%k1}
	vcmpps {eq},(%rax),%v0,%k1 {%k1}
	vcmpps {eq},(%rax),%v0,%k1 {%k1}
	vcmpps {eq},(%rax),%v0,%k1 {%k1}
	vcmpps {eq},(%rax),%v0,%k1 {%k1}
	vcmpps {lt},(%rax),%v0,%k1 {%k1}
	vcmpps {le},(%rax),%v0,%k1 {%k1}
	vcmpps {neq},(%rax),%v0,%k1 {%k1}
	vcmpps {nlt},(%rax),%v0,%k1 {%k1}
	vcmpps {nle},(%rax),%v0,%k1 {%k1}
	vcmpps {eq},(%rax){NT},%v0,%k1 {%k1}
	vcmpps {eq},(%rax){unorm8},%v0,%k1 {%k1}
	vcmpps {eq},(%rax){NT}{unorm8},%v0,%k1 {%k1}
	vcmpps {eq},(%rax){unorm8}{NT},%v0,%k1 {%k1}
	vcmpps {unord},(%rax){unorm8}{NT},%v0,%k1 {%k1}
	vcmpps {ord},(%rax){unorm8}{NT},%v0,%k1 {%k1}

	vcvtinsps2u10 $0,%v2,%v0
	vcvtinsps2u10 $0,%v2,%v0{%k1}
	vcvtinsps2u10 $0,%v2{aaaa},%v0{%k1}
	vcvtinsps2u10 $0,%v2{bbbb},%v0{%k1}
	vcvtinsps2u10 $0,%v2{cccc},%v0{%k1}
	vcvtinsps2u10 $0,%v31{cccc},%v31{%k7}
	vcvtinsps2u10 $0,(%rax),%v0{%k1}
	vcvtinsps2u10 $0,(%r8),%v0{%k1}
	vcvtinsps2u10 $0,(%r15),%v0{%k1}
	vcvtinsps2u10 $0,(%rax){unorm8},%v0{%k1}
	vcvtinsps2u10 $0,(%rax){NT},%v0{%k1}
	vcvtinsps2u10 $0,(%rax){NT}{unorm8},%v0{%k1}
	vcvtinsps2u10 $0,(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $0,(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $0,(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $0,(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $1,(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $2,(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $2,16(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $2,0x16(%rax){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $2,0x16(%rax,%rbx,4){unorm8}{NT},%v0{%k1}

	vcvtps2pu {.0b},{rn},%v2,%v0
	vcvtps2pu {.0b},{rn},%v2,%v0{%k1}
	vcvtps2pu {.0b},{rn},%v2{aaaa},%v0{%k1}
	vcvtps2pu {.0b},{rn},%v2{bbbb},%v0{%k1}
	vcvtps2pu {.0b},{rn},%v2{cccc},%v0{%k1}
	vcvtps2pu {.0b},{rn},%v31{cccc},%v31{%k7}
	vcvtps2pu {.0b},{rn},(%rax),%v0{%k1}
	vcvtps2pu {.0b},{rn},(%r8),%v0{%k1}
	vcvtps2pu {.0b},{rn},(%r15),%v0{%k1}
	vcvtps2pu {.0b},{rn},(%rax){unorm8},%v0{%k1}
	vcvtps2pu {.0b},{rn},(%rax){NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},(%rax){NT}{unorm8},%v0{%k1}
	vcvtps2pu {.4b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.5b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.8b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.16b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.24b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.31b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{rd},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{ru},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{rz},(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},16(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},8(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},4(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},16(%rax){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},16(%rax,%rbx,4){unorm8}{NT},%v0{%k1}

	vcvtpu2ps {.0b},%v2,%v0
	vcvtpu2ps {.0b},%v2,%v0{%k1}
	vcvtpu2ps {.0b},%v2{aaaa},%v0{%k1}
	vcvtpu2ps {.0b},%v2{bbbb},%v0{%k1}
	vcvtpu2ps {.0b},%v2{cccc},%v0{%k1}
	vcvtpu2ps {.0b},%v31{cccc},%v31{%k7}
	vcvtpu2ps {.0b},(%rax),%v0{%k1}
	vcvtpu2ps {.0b},(%r8),%v0{%k1}
	vcvtpu2ps {.0b},(%r15),%v0{%k1}
	vcvtpu2ps {.0b},(%rax){sint8},%v0{%k1}
	vcvtpu2ps {.0b},(%rax){NT},%v0{%k1}
	vcvtpu2ps {.0b},(%rax){NT}{sint8},%v0{%k1}
	vcvtpu2ps {.4b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.5b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.8b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.16b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.24b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.31b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.32b},(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.0b},16(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.0b},8(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.0b},4(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.0b},16(%rax){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.0b},16(%rax,%rbx,4){sint8}{NT},%v0{%k1}

	vloadunpackld (%rax), %v0
	vloadunpackld (%rax), %v7
	vloadunpackld (%rax), %v8
	vloadunpackld (%rax), %v15
	vloadunpackld (%rax), %v31
	vloadunpackld (%r8), %v0
	vloadunpackld (%r15), %v0
	vloadunpackld (%r15), %v31
	vloadunpackld (%rax){nt}, %v0
	vloadunpackld (%rax), %v0{%k1}
	vloadunpackld (%rax){nt}, %v0{%k1}

	vloadunpackhd (%rax), %v0
	vloadunpackhd (%rax){nt}, %v0
	vloadunpackhd (%rax), %v0{%k1}
	vloadunpackhd (%rax){nt}, %v0{%k1}

	vpackstoreld %v0,(%rax)
	vpackstoreld %v0,(%rax){NT}
	vpackstoreld %v0,(%rax){%k1}
	vpackstoreld %v0,(%rax){%k1}{NT}
	vpackstoreld %v0,(%r8)
	vpackstoreld %v0,(%r15){NT}
	vpackstoreld %v0,(%rax){%k7}
	vpackstoreld %v15,(%rax){%k1}{NT}
	vpackstoreld %v0,128(%rax){%k1}{nt}

	vpackstorehd %v0,(%rax)
	vpackstorehd %v0,(%rax){NT}
	vpackstorehd %v0,(%rax){%k1}
	vpackstorehd %v0,(%rax){%k1}{NT}
	vpackstorehd %v0,(%r8)
	vpackstorehd %v0,(%r15){NT}
	vpackstorehd %v0,(%rax){%k7}
	vpackstorehd %v15,(%rax){%k1}{NT}
	vpackstorehd %v0,128(%rax){%k1}{nt}

	vroundps {.0b},{rn},%v2,%v0
	vroundps {.0b},{rn},%v2,%v0{%k1}
	vroundps {.0b},{rn},%v2{aaaa},%v0{%k1}
	vroundps {.0b},{rn},%v2{bbbb},%v0{%k1}
	vroundps {.0b},{rn},%v2{cccc},%v0{%k1}
	vroundps {.0b},{rn},%v31{cccc},%v31{%k7}
	vroundps {.0b},{rn},(%rax),%v0{%k1}
	vroundps {.0b},{rn},(%r8),%v0{%k1}
	vroundps {.0b},{rn},(%r15),%v0{%k1}
	vroundps {.0b},{rn},(%rax){unorm8},%v0{%k1}
	vroundps {.0b},{rn},(%rax){NT},%v0{%k1}
	vroundps {.0b},{rn},(%rax){NT}{unorm8},%v0{%k1}
	vroundps {.4b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.5b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.8b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.16b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.24b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.31b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{rn},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{rd},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{ru},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{rz},(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.0b},{rn},16(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.0b},{rn},8(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.0b},{rn},4(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.0b},{rn},16(%rax){unorm8}{NT},%v0{%k1}
	vroundps {.0b},{rn},16(%rax,%rbx,4){unorm8}{NT},%v0{%k1}

	vtestpi  %v1, %v0, %k1
	vtestpi  %v1 {aaaa}, %v0, %k1
	vtestpi  %v1 {bbbb}, %v0, %k1
	vtestpi  %v1, %v0, %k7
	vtestpi  %v1, %v0, %k1{%k7}
	vtestpi  %v1, %v31, %k1
	vtestpi  %v31, %v0, %k1

	vtestpi  (%rax), %v0, %k1
	vtestpi  (%rax), %v0, %k7
	vtestpi  (%rax), %v0, %k1{%k7}
	vtestpi  (%rax), %v31, %k1
	vtestpi  (%r15), %v0, %k1
	vtestpi  (%rax) {NT}, %v0, %k1
	vtestpi  (%rax) {sint8}, %v0, %k1
	vtestpi  (%rax) {sint8} {NT}, %v0, %k1
	vtestpi  (%rax) {NT}{sint8}, %v0, %k1

	vtestpi  4(%r8,%r9,1){NT}, %v1, %k1{%k7}
	vtestpi  64(%r8,%r9,1){NT}, %v1, %k1{%k7}
	vtestpi  -64(%r8,%r9,1){NT}, %v1, %k1{%k7}

	vshuf128x32 {aaaa}, {0000}, %v2, %v0
	vshuf128x32 {aaaa}, {0000}, %v31, %v31{%k7}
	vshuf128x32 {aaaa}, {0000}, (%rax), %v0
	vshuf128x32 {aaaa}, {0000}, (%r8), %v0
	vshuf128x32 {aaaa}, {0000}, (%rax) {NT}, %v0
	vshuf128x32 {aaaa}, {0000}, (%r15), %v0
	vshuf128x32 {aaad}, {0000}, %v2, %v0
	vshuf128x32 {aadd}, {0000}, %v2, %v0
	vshuf128x32 {addd}, {0000}, %v2, %v0
	vshuf128x32 {dddd}, {0000}, %v2, %v0
	vshuf128x32 {dddd}, {0002}, %v2, %v0
	vshuf128x32 {dddd}, {0202}, %v2, %v0
	vshuf128x32 {dddd}, {3333}, %v2, %v0
	vshuf128x32 {ABCD}, {3333}, %v2, %v0

	vexp2lutps %v2, %v0
	vexp2lutps %v2, %v0 {%k1}
	vexp2lutps %v2 {aaaa}, %v0 {%k1}
	vexp2lutps %v2 {bbbb}, %v0 {%k1}
	vexp2lutps %v2 {cccc}, %v0 {%k1}
	vexp2lutps %v31 {cccc}, %v31 {%k7}
	vexp2lutps (%rax), %v0{%k1}
	vexp2lutps (%r8), %v0{%k1}
	vexp2lutps (%r15), %v0{%k1}
	vexp2lutps (%rax){sint8}, %v0{%k1}
	vexp2lutps (%rax){NT}, %v0{%k1}
	vexp2lutps (%rax){NT}{sint8}, %v0{%k1}
	vexp2lutps (%rax){sint8}{NT}, %v0{%k1}
	vexp2lutps 4(%r8,%r9,1){NT}, %v1{%k7}
	vexp2lutps 64(%r8,%r9,1){NT}, %v2{%k7}
	vexp2lutps %v2 {dcba}, %v0 {%k7}

	vgetexpps %v2, %v0
	vgetexpps %v2, %v0 {%k1}
	vgetexpps %v2 {aaaa}, %v0 {%k1}
	vgetexpps %v2 {bbbb}, %v0 {%k1}
	vgetexpps %v2 {cccc}, %v0 {%k1}
	vgetexpps %v31 {cccc}, %v31 {%k7}
	vgetexpps (%rax), %v0{%k1}
	vgetexpps (%r8), %v0{%k1}
	vgetexpps (%r15), %v0{%k1}
	vgetexpps (%rax){unorm8}, %v0{%k1}
	vgetexpps (%rax){NT}, %v0{%k1}
	vgetexpps (%rax){NT}{unorm8}, %v0{%k1}
	vgetexpps (%rax){unorm8}{NT}, %v0{%k1}
	vgetexpps 4(%r8,%r9,1){NT}, %v1{%k7}
	vgetexpps 64(%r8,%r9,1){NT}, %v2{%k7}
	vgetexpps %v2 {dcba}, %v0 {%k7}

	vrcpresps %v2, %v0
	vrcpresps %v2, %v0 {%k1}
	vrcpresps %v2 {aaaa}, %v0 {%k1}
	vrcpresps %v2 {bbbb}, %v0 {%k1}
	vrcpresps %v2 {cccc}, %v0 {%k1}
	vrcpresps %v31 {cccc}, %v31 {%k7}
	vrcpresps (%rax), %v0{%k1}
	vrcpresps (%r8), %v0{%k1}
	vrcpresps (%r15), %v0{%k1}
	vrcpresps (%rax){unorm8}, %v0{%k1}
	vrcpresps (%rax){NT}, %v0{%k1}
	vrcpresps (%rax){NT}{unorm8}, %v0{%k1}
	vrcpresps (%rax){unorm8}{NT}, %v0{%k1}
	vrcpresps 4(%r8,%r9,1){NT}, %v1{%k7}
	vrcpresps 64(%r8,%r9,1){NT}, %v2{%k7}
	vrcpresps %v2 {dcba}, %v0 {%k7}

	vrsqrtlutps %v2, %v0
	vrsqrtlutps %v2, %v0 {%k1}
	vrsqrtlutps %v2 {aaaa}, %v0 {%k1}
	vrsqrtlutps %v2 {bbbb}, %v0 {%k1}
	vrsqrtlutps %v2 {cccc}, %v0 {%k1}
	vrsqrtlutps %v31 {cccc}, %v31 {%k7}
	vrsqrtlutps (%rax), %v0{%k1}
	vrsqrtlutps (%r8), %v0{%k1}
	vrsqrtlutps (%r15), %v0{%k1}
	vrsqrtlutps (%rax){unorm8}, %v0{%k1}
	vrsqrtlutps (%rax){NT}, %v0{%k1}
	vrsqrtlutps (%rax){NT}{unorm8}, %v0{%k1}
	vrsqrtlutps (%rax){unorm8}{NT}, %v0{%k1}
	vrsqrtlutps 4(%r8,%r9,1){NT}, %v1{%k7}
	vrsqrtlutps 64(%r8,%r9,1){NT}, %v2{%k7}
	vrsqrtlutps %v2 {dcba}, %v0 {%k7}

	vsbbpi %v2, %k1, %v0
	vsbbpi %v2 {dcba}, %k0, %v0 {%k7}
	vsbbpi 4(%r8,%r9,1){NT}, %k2, %v1{%k7}
	vsbbpi 64(%r8,%r9,1){NT}, %k3, %v2{%k7}

	vsbbrpi %v2, %k1, %v0
	vsbbrpi %v2 {dcba}, %k0, %v0 {%k7}
	vsbbrpi 4(%r8,%r9,1){NT}, %k2, %v1{%k7}
	vsbbrpi 64(%r8,%r9,1){NT}, %k3, %v2{%k7}

	vsubrsetbpi %v2, %k1, %v0
	vsubrsetbpi %v2 {dcba}, %k0, %v0 {%k7}
	vsubrsetbpi 4(%r8,%r9,1){NT}, %k2, %v1{%k7}
	vsubrsetbpi 64(%r8,%r9,1){NT}, %k3, %v2{%k7}

	vsubsetbpi %v2, %k1, %v0
	vsubsetbpi %v2 {dcba}, %k0, %v0 {%k7}
	vsubsetbpi 4(%r8,%r9,1){NT}, %k2, %v1{%k7}
	vsubsetbpi 64(%r8,%r9,1){NT}, %k3, %v2{%k7}

	vlog2lutps %v2, %v0
	vlog2lutps %v2, %v0 {%k1}
	vlog2lutps %v2 {aaaa}, %v0 {%k1}
	vlog2lutps %v2 {bbbb}, %v0 {%k1}
	vlog2lutps %v2 {cccc}, %v0 {%k1}
	vlog2lutps %v31 {cccc}, %v31 {%k7}
	vlog2lutps (%rax), %v0{%k1}
	vlog2lutps (%r8), %v0{%k1}
	vlog2lutps (%r15), %v0{%k1}
	vlog2lutps (%rax){unorm8}, %v0{%k1}
	vlog2lutps (%rax){NT}, %v0{%k1}
	vlog2lutps (%rax){NT}{unorm8}, %v0{%k1}
	vlog2lutps (%rax){unorm8}{NT}, %v0{%k1}
	vlog2lutps 4(%r8,%r9,1){NT}, %v1{%k7}
	vlog2lutps 64(%r8,%r9,1){NT}, %v2{%k7}
	vlog2lutps %v2 {dcba}, %v0 {%k7}

vcmppi {le},(%rax),%v2,%k1
vcmppi {le},(%rax),%v2,%k1{%k2}
vcmpps {le},(%rax),%v3,%k1
vcmppu {le},(%rax),%v9,%k1
vcmppi {le},%v1,%v2,%k1
vcmppi {le},%v2,%v2,%k1{%k2}
vcmpps {le},%v3,%v3,%k1
vcmppu {le},%v4,%v9,%k1
	vloadd 64(,%rax), %v0
	vloadd -64(,%rax), %v0
	vaddnps  %v2, %v1, %v0
	vaddnps  %v2 {dcba}, %v1, %v0
	vaddnps  %v2 {dcba}, %v1, %v0 {%k7}
	vaddnps  %v2 {cdab}, %v1, %v0 {%k7}
	vaddnps  %v2 {badc}, %v1, %v0 {%k7}
	vaddnps  %v2 {aaaa}, %v1, %v0 {%k7}
	vaddnps  %v2 {bbbb}, %v1, %v0 {%k7}
	vaddnps  %v2 {cccc}, %v1, %v0 {%k7}
	vaddnps  %v2 {dddd}, %v1, %v0 {%k7}
	vaddnps  %v2 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v7 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v8 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v9 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v15 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v16 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v17 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v31 {dacb}, %v1, %v0 {%k7}
	vaddnps  %v31 {dacb}, %v1, %v8 {%k7}
	vaddnps  %v31 {dacb}, %v1, %v9 {%k7}
	vaddnps  %v31 {dacb}, %v1, %v15 {%k7}
	vaddnps  %v31 {dacb}, %v8, %v15 {%k7}
	vaddnps  %v31 {dacb}, %v9, %v15 {%k7}
	vaddnps  %v31 {dacb}, %v15, %v15 {%k7}
	vaddnps  %v31 {dacb}, %v31, %v31 {%k7}
	vaddnps  (%rax), %v1, %v0{%k7}
	vaddnps  (%rax) {1to16}, %v1, %v0{%k7}
	vaddnps  (%rax) {4to16}, %v1, %v0{%k7}
	vaddnps  (%rax) {unorm8}, %v1, %v0{%k7}
	vaddnps  (%rax) {sint16}, %v1, %v0{%k7}
	vaddnps  (%rax) {float16}, %v1, %v0{%k7}
	vaddnps  (%rax) {uint8}, %v1, %v0{%k7}
	vaddnps  (%rax) {nt}, %v1, %v0{%k7}
	vaddnps  (%rax) {NT}, %v1, %v0{%k7}
	vaddnps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vaddnps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vaddnps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddnps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddnps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddnpd  %v2, %v1, %v0
	vaddnpd  %v2 {dcba}, %v1, %v0
	vaddnpd  %v2 {dcba}, %v1, %v0 {%k7}
	vaddnpd  %v2 {cdab}, %v1, %v0 {%k7}
	vaddnpd  %v2 {badc}, %v1, %v0 {%k7}
	vaddnpd  %v2 {aaaa}, %v1, %v0 {%k7}
	vaddnpd  %v2 {bbbb}, %v1, %v0 {%k7}
	vaddnpd  %v2 {cccc}, %v1, %v0 {%k7}
	vaddnpd  %v2 {dddd}, %v1, %v0 {%k7}
	vaddnpd  %v2 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v7 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v8 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v9 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v15 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v16 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v17 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v31 {dacb}, %v1, %v0 {%k7}
	vaddnpd  %v31 {dacb}, %v1, %v8 {%k7}
	vaddnpd  %v31 {dacb}, %v1, %v9 {%k7}
	vaddnpd  %v31 {dacb}, %v1, %v15 {%k7}
	vaddnpd  %v31 {dacb}, %v8, %v15 {%k7}
	vaddnpd  %v31 {dacb}, %v9, %v15 {%k7}
	vaddnpd  %v31 {dacb}, %v15, %v15 {%k7}
	vaddnpd  %v31 {dacb}, %v31, %v31 {%k7}
	vaddnpd  (%rax), %v1, %v0{%k7}
	vaddnpd  (%rax) {8to8}, %v1, %v0{%k7}
	vaddnpd  (%rax) {1to8}, %v1, %v0{%k7}
	vaddnpd  (%rax) {4to8}, %v1, %v0{%k7}
	vaddnpd  (%rax) {nt}, %v1, %v0{%k7}
	vaddnpd  (%rax) {NT}, %v1, %v0{%k7}
	vaddnpd  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddnpd  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddnpd  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}

	vloadd 1(%rax) {NT}{1to16}{srgb8}, %v0{%k1}
	vloadd 1(%rax) {NT}{1to16}{uint16}, %v0{%k1}
	vloadd 2(%rax) {NT}{1to16}{uint16}, %v0{%k1}
	vloadd 1(%rax) {NT}{16to16}{srgb8}, %v0{%k1}
	vloadd 1(%rax) {NT}{4to16}{srgb8}, %v0{%k1}

	vloadd 1(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 2(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 3(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 4(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 5(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 6(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 7(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 8(%rax) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd 9(%rax) {NT}{4to16}{uint16}, %v0{%k1}

	vloadd 16(%rax) {NT}{srgb8}, %v0{%k1}
	vloadd 15(%rax) {NT}{srgb8}, %v0{%k1}
	vloadd 1(%rax) {NT}{srgb8}, %v0{%k1}
	vloadd 1(%rax) {NT}, %v0{%k1}
	vloadd 32(%rax) {NT}, %v0{%k1}
	vloadd 64(%rax) {NT}, %v0{%k1}

	vloadunpackhd 127(%rax) {srgb8}{NT}, %v0{%k1}
	vloadunpackhd 128(%rax) {NT}, %v0{%k1}
	vloadunpackhd 128(%rax) {srgb8}{NT}, %v0{%k1}
	vloadunpackld 127(%rax) {srgb8}{NT}, %v0{%k1}
	vloadunpackld 128(%rax) {NT}, %v0{%k1}
	vloadunpackld 128(%rax) {srgb8}{NT}, %v0{%k1}
	vloadunpackhq 128(%rax) {NT}, %v0{%k1}
	vloadunpacklq 128(%rax) {NT}, %v0{%k1}
	vloadd (%rax) {srgb8}{NT}{1to16}, %v0{%k1}
	vloadd 128(%rax) {srgb8}{NT}, %v0{%k1}
	vloadd 126(%rax) {srgb8}{NT}{1to16}, %v0{%k1}
	vloadd 128(%rax) {srgb8}{NT}{4to16}, %v0{%k1}
	vstored %v0{sint8}{a},(%rax)
	vstored %v0{sint8},128(%rax)
	vstored %v0{sint8}{a},126(%rax)
	vstored %v0{sint8}{dcba},128(%rax)
	vpackstorehd %v0{sint8},127(%rax)
	vpackstorehd %v0{sint8},128(%rax)
	vpackstorehd %v0,128(%rax){NT}
	vpackstoreld %v0{sint8},127(%rax)
	vpackstoreld %v0{sint8},128(%rax)
	vpackstoreld %v0,128(%rax){NT}
	vpackstorehq %v0,128(%rax){NT}
	vpackstorelq %v0,128(%rax){NT}

vcvtpu2pd $1, 1122867(%rcx,%r14,0x4){sint8},%v19
vcvtpu2ps $1, 1122867(%rcx,%r14,0x4){sint8},%v19
vcvtps2pd $0, %v2, %v18{%k6}
vloadunpackhd 0x1122(%rdx,%rcx,2){uint8}, %v19{%k4}
vloadunpackld 0x1122(%rax,%rcx,0x1){nt}{unorm10A}, %v11
vpackstorehd %v14{sint16}, 17(%r10,%rbp,0x4){nt}{%k2}
vpackstoreld %v23{sint8}, 17(%rdx,%rdi,1){%k6}
vprefetch1 $0, 17(%r12,%r8,0x2)
vprefetch2 $0, 0(%rbp,%rdi,0x1)
	vgatherd (%rax,%v3,4), %v0{%k1}
	vgatherd (%rax,%v22,8){nt}, %v0{%k1}
	vgatherd 127(%rax,%v30,1) {srgb8}{NT}, %v0{%k1}
	vgatherd 128(%rax,%v10,2) {NT}, %v0{%k1}
	vgatherd 128(%rax,%v9,4) {srgb8}{NT}, %v0{%k1}
	vgatherd (,%v11,1), %v10{%k1}

	vgatherpfd (%rax,%v3,4){%k7}
	vgatherpfd (%rax,%v4,8){nt}{%k6}
	vgatherpfd 127(%rax,%v5) {srgb8}{NT}{%k5}
	vgatherpfd 128(%rax,%v6,2) {NT}{%k4}
	vgatherpfd 128(%rax,%v7,4) {srgb8}{NT}{%k3}
	vgatherpfd (,%v3,4){%k7}

	vscatterd %v11,0x80(,%v1,1){%k1}
	vscatterd %v11,(,%v1,1){%k1}
	vscatterd %v11,(%rax,%v1,1){%k1}
	vscatterd %v10,(%rax,%v2,2){%k1}{NT}
	vscatterd %v9,(%rax,%v3,4){%k1}
	vscatterd %v8{sint8},(%rax,%v3,8){%k1}
	vscatterd %v7{sint8},(%rax,%v4){%k1}
	vscatterd %v6{sint8},(%rax,%v5,2){%k1}
	vscatterd %v5,(%rax,%v6,4){%k1}{NT}
	vscatterd %v4{sint8},(%rax,%v7,8){%k1}{NT}
	vscatterd %v3{sint8},(%rax,%v8,1){%k1}{NT}
	vscatterd %v2{sint8},(%rax,%v9,2){%k1}{NT}
	vscatterd %v1{sint8},(%rax,%v10,4){%k1}{NT}
	vscatterd %v0{float16rz},128(%rax,%v11,8){%k1}

	vscatterpfd (%rax,%v6,4){NT}{%k1}
	vscatterpfd (%rax,%v0,4){NT}{%k1}
	vscatterpfd (%rax,%v1,1){%k1}
	vscatterpfd (%rax,%v2,2){NT}{%k1}
	vscatterpfd (%rax,%v3,4){%k1}
	vscatterpfd 0x80(%rax,%v3,4){%k1}
	vscatterpfd (,%v1,1){%k1}
	vscatterpfd 0x80(,%v1,1){%k1}
	vscatterpfd (%rax,%v3,8){sint8}{%k1}
	vscatterpfd (%rax,%v4,1){sint8}{%k1}
	vscatterpfd (%rax,%v5,2){sint8}{%k1}
	vscatterpfd (%rax,%v7,8){sint8}{NT}{%k1}
	vscatterpfd (%rax,%v8,1){sint8}{NT}{%k1}
	vscatterpfd (%rax,%v9,2){sint8}{NT}{%k1}
	vscatterpfd (%rax,%v10,4){sint8}{NT}{%k1}
	vscatterpfd 128(%rax,%v11,8){float16rz}{%k1}

vcvtpi2pd $1, 1122867(%rcx,%r14,0x4){sint8},%v19
vcvtps2pd $1, 1122867(%rcx,%r14,0x4){uint8},%v19
vcvtpu2pd $1, 1122867(%rcx,%r14,0x4){sint8},%v19
	vcvtpd2pi $1,{rn},%v2,%v0
	vcvtpd2pi $1,{rn},%v2,%v0{%k1}
	vcvtpd2pi $1,{rn},%v2{aaaa},%v0{%k1}
	vcvtpd2pi $1,{rn},%v2{bbbb},%v0{%k1}
	vcvtpd2pi $1,{rn},%v2{cccc},%v0{%k1}
	vcvtpd2pi $1,{rn},%v31{cccc},%v31{%k7}
	vcvtpd2pi $1,{rn},(%rax),%v0{%k1}
	vcvtpd2pi $1,{rn},(%r8),%v0{%k1}
	vcvtpd2pi $1,{rn},(%r15),%v0{%k1}
	vcvtpd2pi $1,{rn},(%rax){1to8},%v0{%k1}
	vcvtpd2pi $1,{rn},(%rax){NT},%v0{%k1}
	vcvtpd2pi $1,{rn},(%rax){NT}{1to8},%v0{%k1}
	vcvtpd2pi $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rd},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{ru},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rz},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $1,{rn},16(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $1,{rn},8(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $1,{rn},4(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $1,{rn},16(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pi $1,{rn},16(%rax,%rbx,4){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},%v2,%v0
	vcvtpd2pu $1,{rn},%v2,%v0{%k1}
	vcvtpd2pu $1,{rn},%v2{aaaa},%v0{%k1}
	vcvtpd2pu $1,{rn},%v2{bbbb},%v0{%k1}
	vcvtpd2pu $1,{rn},%v2{cccc},%v0{%k1}
	vcvtpd2pu $1,{rn},%v31{cccc},%v31{%k7}
	vcvtpd2pu $1,{rn},(%rax),%v0{%k1}
	vcvtpd2pu $1,{rn},(%r8),%v0{%k1}
	vcvtpd2pu $1,{rn},(%r15),%v0{%k1}
	vcvtpd2pu $1,{rn},(%rax){1to8},%v0{%k1}
	vcvtpd2pu $1,{rn},(%rax){NT},%v0{%k1}
	vcvtpd2pu $1,{rn},(%rax){NT}{1to8},%v0{%k1}
	vcvtpd2pu $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rd},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{ru},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rz},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},16(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},8(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},4(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},16(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},16(%rax,%rbx,4){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},%v2,%v0
	vcvtpd2ps $1,{rn},%v2,%v0{%k1}
	vcvtpd2ps $1,{rn},%v2{aaaa},%v0{%k1}
	vcvtpd2ps $1,{rn},%v2{bbbb},%v0{%k1}
	vcvtpd2ps $1,{rn},%v2{cccc},%v0{%k1}
	vcvtpd2ps $1,{rn},%v31{cccc},%v31{%k7}
	vcvtpd2ps $1,{rn},(%rax),%v0{%k1}
	vcvtpd2ps $1,{rn},(%r8),%v0{%k1}
	vcvtpd2ps $1,{rn},(%r15),%v0{%k1}
	vcvtpd2ps $1,{rn},(%rax){1to8},%v0{%k1}
	vcvtpd2ps $1,{rn},(%rax){NT},%v0{%k1}
	vcvtpd2ps $1,{rn},(%rax){NT}{1to8},%v0{%k1}
	vcvtpd2ps $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rn},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rd},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{ru},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rz},(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},16(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},8(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},4(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},16(%rax){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},16(%rax,%rbx,4){1to8}{NT},%v0{%k1}

	vcmppd    {ord}, %v2, %v1, %k1
	vloadd    4386(%r10,%rdi,8){float11B}{4to16}{nt}, %v27

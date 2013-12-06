	.text
_start:
	vadcpi foo(%rip), %k1, %v0 {%k1}
	vadcpi foo(%rip) {NT}, %k1, %v0 {%k1}
	vadcpi foo(%rip) {NT}{sint8}, %k1, %v0 {%k1}
	vadcpi foo(%rip) {sint8}, %k1, %v0 {%k1}
	vadcpi foo(%rip) {sint8}{NT}, %k1, %v0 {%k1}
	vaddnpd  foo(%rip) {1to8}, %v1, %v0{%k7}
	vaddnpd  foo(%rip) {4to8}, %v1, %v0{%k7}
	vaddnpd  foo(%rip) {8to8}, %v1, %v0{%k7}
	vaddnpd  foo(%rip) {nt}, %v1, %v0{%k7}
	vaddnpd  foo(%rip) {NT}, %v1, %v0{%k7}
	vaddnpd  foo(%rip){NT}, %v1, %v0{%k7}
	vaddnpd  foo(%rip), %v1, %v0{%k7}
	vaddnps  foo(%rip) {1to16}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {4to16}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {float16}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {NT}{unorm8}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {nt}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {NT}, %v1, %v0{%k7}
	vaddnps  foo(%rip){NT}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {sint16}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {uint8}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {unorm8} {NT}, %v1, %v0{%k7}
	vaddnps  foo(%rip) {unorm8}, %v1, %v0{%k7}
	vaddnps  foo(%rip), %v1, %v0{%k7}
	vaddpi  foo(%rip) {1to16}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {4to16}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {NT}{sint8}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {nt}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {NT}, %v1, %v0{%k7}
	vaddpi  foo(%rip){NT}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {sint16}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {sint8} {NT}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {sint8}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {uint16}, %v1, %v0{%k7}
	vaddpi  foo(%rip) {uint8}, %v1, %v0{%k7}
	vaddpi  foo(%rip), %v1, %v0{%k7}
	vaddsetcpi foo(%rip){NT}, %k2, %v1{%k7}
	vaddsetcpi foo(%rip){NT}, %k3, %v2{%k7}
	vcmppi {eq},foo(%rip){NT}{sint8},%v0,%k1 {%k1}
	vcmppi {eq},foo(%rip){NT},%v0,%k1 {%k1}
	vcmppi {eq},foo(%rip){sint8}{NT},%v0,%k1 {%k1}
	vcmppi {eq},foo(%rip){sint8},%v0,%k1 {%k1}
	vcmppi {eq},foo(%rip),%v0,%k1 {%k1}
	vcmppi {le},foo(%rip),%v0,%k1 {%k1}
vcmppi {le},foo(%rip),%v2,%k1
vcmppi {le},foo(%rip),%v2,%k1{%k2}
	vcmppi {lt},foo(%rip),%v0,%k1 {%k1}
	vcmppi {neq},foo(%rip),%v0,%k1 {%k1}
	vcmppi {nle},foo(%rip),%v0,%k1 {%k1}
	vcmppi {nlt},foo(%rip),%v0,%k1 {%k1}
	vcmpps {eq},foo(%rip){NT}{unorm8},%v0,%k1 {%k1}
	vcmpps {eq},foo(%rip){NT},%v0,%k1 {%k1}
	vcmpps {eq},foo(%rip){unorm8}{NT},%v0,%k1 {%k1}
	vcmpps {eq},foo(%rip){unorm8},%v0,%k1 {%k1}
	vcmpps {eq},foo(%rip),%v0,%k1 {%k1}
	vcmpps {le},foo(%rip),%v0,%k1 {%k1}
vcmpps {le},foo(%rip),%v3,%k1
	vcmpps {lt},foo(%rip),%v0,%k1 {%k1}
	vcmpps {neq},foo(%rip),%v0,%k1 {%k1}
	vcmpps {nle},foo(%rip),%v0,%k1 {%k1}
	vcmpps {nlt},foo(%rip),%v0,%k1 {%k1}
	vcmpps {ord},foo(%rip){unorm8}{NT},%v0,%k1 {%k1}
	vcmpps {unord},foo(%rip){unorm8}{NT},%v0,%k1 {%k1}
	vcmppu {eq},foo(%rip){NT}{sint8},%v0,%k1 {%k1}
	vcmppu {eq},foo(%rip){NT},%v0,%k1 {%k1}
	vcmppu {eq},foo(%rip){sint8}{NT},%v0,%k1 {%k1}
	vcmppu {eq},foo(%rip){sint8},%v0,%k1 {%k1}
	vcmppu {eq},foo(%rip),%v0,%k1 {%k1}
	vcmppu {le},foo(%rip),%v0,%k1 {%k1}
vcmppu {le},foo(%rip),%v9,%k1
	vcmppu {lt},foo(%rip),%v0,%k1 {%k1}
	vcmppu {neq},foo(%rip),%v0,%k1 {%k1}
	vcmppu {nle},foo(%rip),%v0,%k1 {%k1}
	vcmppu {nlt},foo(%rip),%v0,%k1 {%k1}
	vcvtinsps2f11 $0,{rn},foo(%rip){NT}{unorm8},%v0{%k1}
	vcvtinsps2f11 $0,{rn},foo(%rip){NT},%v0{%k1}
	vcvtinsps2f11 $0,{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $0,{rn},foo(%rip){unorm8},%v0{%k1}
	vcvtinsps2f11 $0,{rn},foo(%rip),%v0{%k1}
	vcvtinsps2f11 $0,{rz},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $1,{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtinsps2f11 $2,{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $0,foo(%rip){NT}{unorm8},%v0{%k1}
	vcvtinsps2u10 $0,foo(%rip){NT},%v0{%k1}
	vcvtinsps2u10 $0,foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $0,foo(%rip){unorm8},%v0{%k1}
	vcvtinsps2u10 $0,foo(%rip),%v0{%k1}
	vcvtinsps2u10 $1,foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtinsps2u10 $2,foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rd},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rn},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{ru},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pi $0,{rz},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pi $1,{rn},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pi $1,{rn},foo(%rip){1to8},%v0{%k1}
	vcvtpd2pi $1,{rn},foo(%rip){NT}{1to8},%v0{%k1}
	vcvtpd2pi $1,{rn},foo(%rip){NT},%v0{%k1}
	vcvtpd2pi $1,{rn},foo(%rip),%v0{%k1}
	vcvtpd2ps $0,{rd},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rn},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{ru},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2ps $0,{rz},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2ps $1,{rn},foo(%rip){1to8},%v0{%k1}
	vcvtpd2ps $1,{rn},foo(%rip){NT}{1to8},%v0{%k1}
	vcvtpd2ps $1,{rn},foo(%rip){NT},%v0{%k1}
	vcvtpd2ps $1,{rn},foo(%rip),%v0{%k1}
	vcvtpd2pu $0,{rd},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rn},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{ru},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pu $0,{rz},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},foo(%rip){1to8}{NT},%v0{%k1}
	vcvtpd2pu $1,{rn},foo(%rip){1to8},%v0{%k1}
	vcvtpd2pu $1,{rn},foo(%rip){NT}{1to8},%v0{%k1}
	vcvtpd2pu $1,{rn},foo(%rip){NT},%v0{%k1}
	vcvtpd2pu $1,{rn},foo(%rip),%v0{%k1}
vcvtpi2pd $1, foo(%rip){sint8},%v19
	vcvtpi2ps {.0b},foo(%rip){NT}{sint8},%v0{%k1}
	vcvtpi2ps {.0b},foo(%rip){NT},%v0{%k1}
	vcvtpi2ps {.0b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.0b},foo(%rip){sint8},%v0{%k1}
	vcvtpi2ps {.0b},foo(%rip),%v0{%k1}
	vcvtpi2ps {.16b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.24b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.31b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.32b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.4b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.5b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpi2ps {.8b},foo(%rip){sint8}{NT},%v0{%k1}
vcvtps2pd $1, foo(%rip){uint8},%v19
	vcvtps2pi {.0b},{rn},foo(%rip){NT}{unorm8},%v0{%k1}
	vcvtps2pi {.0b},{rn},foo(%rip){NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.0b},{rn},foo(%rip){unorm8},%v0{%k1}
	vcvtps2pi {.0b},{rn},foo(%rip),%v0{%k1}
	vcvtps2pi {.16b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.24b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.31b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{rd},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{ru},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.32b},{rz},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.4b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.5b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pi {.8b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},foo(%rip){NT}{unorm8},%v0{%k1}
	vcvtps2pu {.0b},{rn},foo(%rip){NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.0b},{rn},foo(%rip){unorm8},%v0{%k1}
	vcvtps2pu {.0b},{rn},foo(%rip),%v0{%k1}
	vcvtps2pu {.16b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.24b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.31b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{rd},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{ru},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.32b},{rz},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.4b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.5b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2pu {.8b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vcvtps2srgb8 foo(%rip){NT}{unorm8}, %v0{%k1}
	vcvtps2srgb8 foo(%rip){NT}, %v0{%k1}
	vcvtps2srgb8 foo(%rip){NT}, %v1{%k7}
	vcvtps2srgb8 foo(%rip){NT}, %v2{%k7}
	vcvtps2srgb8 foo(%rip){unorm8}{NT}, %v0{%k1}
	vcvtps2srgb8 foo(%rip){unorm8}, %v0{%k1}
	vcvtps2srgb8 foo(%rip), %v0{%k1}
vcvtpu2pd $1, foo(%rip){sint8},%v19
	vcvtpu2ps {.0b},foo(%rip){NT}{sint8},%v0{%k1}
	vcvtpu2ps {.0b},foo(%rip){NT},%v0{%k1}
	vcvtpu2ps {.0b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.0b},foo(%rip){sint8},%v0{%k1}
	vcvtpu2ps {.0b},foo(%rip),%v0{%k1}
	vcvtpu2ps {.16b},foo(%rip){sint8}{NT},%v0{%k1}
vcvtpu2ps $1, foo(%rip){sint8},%v19
	vcvtpu2ps {.24b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.31b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.32b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.4b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.5b},foo(%rip){sint8}{NT},%v0{%k1}
	vcvtpu2ps {.8b},foo(%rip){sint8}{NT},%v0{%k1}
	vexp2lutps foo(%rip){NT}{sint8}, %v0{%k1}
	vexp2lutps foo(%rip){NT}, %v0{%k1}
	vexp2lutps foo(%rip){NT}, %v1{%k7}
	vexp2lutps foo(%rip){NT}, %v2{%k7}
	vexp2lutps foo(%rip){sint8}{NT}, %v0{%k1}
	vexp2lutps foo(%rip){sint8}, %v0{%k1}
	vexp2lutps foo(%rip), %v0{%k1}
	vfixupps $0, foo(%rip) {NT}{unorm8}, %v0 {%k1}
	vfixupps $0, foo(%rip) {NT}, %v0 {%k1}
	vfixupps $0, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vfixupps $0, foo(%rip) {unorm8}, %v0 {%k1}
	vfixupps $127, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vfixupps $16384, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vfixupps $1, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vfixupps $256, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vfixupps $65535, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vfixupps $65536, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vfixupps $7, foo(%rip) {unorm8}{NT}, %v0 {%k1}
	vgetexpps foo(%rip){NT}{unorm8}, %v0{%k1}
	vgetexpps foo(%rip){NT}, %v0{%k1}
	vgetexpps foo(%rip){NT}, %v1{%k7}
	vgetexpps foo(%rip){NT}, %v2{%k7}
	vgetexpps foo(%rip){unorm8}{NT}, %v0{%k1}
	vgetexpps foo(%rip){unorm8}, %v0{%k1}
	vgetexpps foo(%rip), %v0{%k1}
	vinsertfieldpi $19, $3, $16, foo(%rip){sint8}{NT}, %v1, %v0 {%k7}
	vloadd foo(%rip) {16to16}, %v0{%k1}
	vloadd foo(%rip) {1to16}, %v0{%k1}
	vloadd foo(%rip) {4to16}{NT}, %v0{%k1}
	vloadd foo(%rip) {4to16}{srgb8}, %v0{%k1}
	vloadd    foo(%rip){float11B}{4to16}{nt}, %v27
	vloadd foo(%rip) {NT}{16to16}{srgb8}, %v0{%k1}
	vloadd foo(%rip) {NT}{1to16}{srgb8}, %v0{%k1}
	vloadd foo(%rip) {NT}{1to16}{uint16}, %v0{%k1}
	vloadd foo(%rip) {NT}{4to16}{srgb8}, %v0{%k1}
	vloadd foo(%rip) {NT}{4to16}{uint16}, %v0{%k1}
	vloadd foo(%rip) {NT}{srgb8}, %v0{%k1}
	vloadd foo(%rip) {NT}, %v0{%k1}
	vloadd foo(%rip) {srgb8}{1to16}, %v0{%k1}
	vloadd foo(%rip) {srgb8}{NT}{1to16}, %v0{%k1}
	vloadd foo(%rip) {srgb8}{NT}{4to16}, %v0{%k1}
	vloadd foo(%rip) {srgb8}{NT}, %v0{%k1}
	vloadd foo(%rip) {srgb8}, %v0{%k1}
	vloadd foo(%rip), %v0
	vloadd foo(%rip), %v0{%k1}
	vloadunpackhd foo(%rip){nt}, %v0
	vloadunpackhd foo(%rip){nt}, %v0{%k1}
	vloadunpackhd foo(%rip) {NT}, %v0{%k1}
	vloadunpackhd foo(%rip) {srgb8}{NT}, %v0{%k1}
vloadunpackhd foo(%rip){uint8}, %v19{%k4}
	vloadunpackhd foo(%rip), %v0
	vloadunpackhd foo(%rip), %v0{%k1}
	vloadunpackhq foo(%rip) {NT}, %v0{%k1}
vloadunpackld foo(%rip){nt}{unorm10A}, %v11
	vloadunpackld foo(%rip){nt}, %v0
	vloadunpackld foo(%rip){nt}, %v0{%k1}
	vloadunpackld foo(%rip) {NT}, %v0{%k1}
	vloadunpackld foo(%rip) {srgb8}{NT}, %v0{%k1}
	vloadunpackld foo(%rip), %v0
	vloadunpackld foo(%rip), %v0{%k1}
	vloadunpackld foo(%rip), %v15
	vloadunpackld foo(%rip), %v31
	vloadunpackld foo(%rip), %v7
	vloadunpackld foo(%rip), %v8
	vloadunpacklq foo(%rip) {NT}, %v0{%k1}
	vlog2lutps foo(%rip){NT}{unorm8}, %v0{%k1}
	vlog2lutps foo(%rip){NT}, %v0{%k1}
	vlog2lutps foo(%rip){NT}, %v1{%k7}
	vlog2lutps foo(%rip){NT}, %v2{%k7}
	vlog2lutps foo(%rip){unorm8}{NT}, %v0{%k1}
	vlog2lutps foo(%rip){unorm8}, %v0{%k1}
	vlog2lutps foo(%rip), %v0{%k1}
	vmaxabsps  foo(%rip) {1to16}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {4to16}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {float16}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {NT}{unorm8}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {nt}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {NT}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip){NT}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {sint16}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {uint8}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {unorm8} {NT}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip) {unorm8}, %v1, %v0{%k7}
	vmaxabsps  foo(%rip), %v1, %v0{%k7}
	vpackstorehd %v0,foo(%rip)
	vpackstorehd %v0,foo(%rip){%k1}
	vpackstorehd %v0,foo(%rip){%k1}{nt}
	vpackstorehd %v0,foo(%rip){%k1}{NT}
	vpackstorehd %v0,foo(%rip){%k7}
	vpackstorehd %v0,foo(%rip){NT}
	vpackstorehd %v0{sint8},foo(%rip)
vpackstorehd %v14{sint16}, foo(%rip){nt}{%k2}
	vpackstorehd %v15,foo(%rip){%k1}{NT}
	vpackstorehq %v0,foo(%rip){NT}
	vpackstoreld %v0,foo(%rip)
	vpackstoreld %v0,foo(%rip){%k1}
	vpackstoreld %v0,foo(%rip){%k1}{nt}
	vpackstoreld %v0,foo(%rip){%k1}{NT}
	vpackstoreld %v0,foo(%rip){%k7}
	vpackstoreld %v0,foo(%rip){NT}
	vpackstoreld %v0{sint8},foo(%rip)
	vpackstoreld %v15,foo(%rip){%k1}{NT}
vpackstoreld %v23{sint8}, foo(%rip){%k6}
	vpackstorelq %v0,foo(%rip){NT}
vprefetch1 $0, foo(%rip)
vprefetch2 $0, foo(%rip)
	vrcpresps foo(%rip){NT}{unorm8}, %v0{%k1}
	vrcpresps foo(%rip){NT}, %v0{%k1}
	vrcpresps foo(%rip){NT}, %v1{%k7}
	vrcpresps foo(%rip){NT}, %v2{%k7}
	vrcpresps foo(%rip){unorm8}{NT}, %v0{%k1}
	vrcpresps foo(%rip){unorm8}, %v0{%k1}
	vrcpresps foo(%rip), %v0{%k1}
	vrotatefieldpi $19, $3, $16, foo(%rip){sint8}{NT},  %v0 {%k7}
	vrotatefieldpi $19, $3, $16, foo(%rip){sint8}{NT}, %v0 {%k7}
	vroundps {.0b},{rn},foo(%rip){NT}{unorm8},%v0{%k1}
	vroundps {.0b},{rn},foo(%rip){NT},%v0{%k1}
	vroundps {.0b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.0b},{rn},foo(%rip){unorm8},%v0{%k1}
	vroundps {.0b},{rn},foo(%rip),%v0{%k1}
	vroundps {.16b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.24b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.31b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{rd},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{ru},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.32b},{rz},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.4b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.5b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vroundps {.8b},{rn},foo(%rip){unorm8}{NT},%v0{%k1}
	vrsqrtlutps foo(%rip){NT}{unorm8}, %v0{%k1}
	vrsqrtlutps foo(%rip){NT}, %v0{%k1}
	vrsqrtlutps foo(%rip){NT}, %v1{%k7}
	vrsqrtlutps foo(%rip){NT}, %v2{%k7}
	vrsqrtlutps foo(%rip){unorm8}{NT}, %v0{%k1}
	vrsqrtlutps foo(%rip){unorm8}, %v0{%k1}
	vrsqrtlutps foo(%rip), %v0{%k1}
	vsbbpi foo(%rip){NT}, %k2, %v1{%k7}
	vsbbpi foo(%rip){NT}, %k3, %v2{%k7}
	vsbbrpi foo(%rip){NT}, %k2, %v1{%k7}
	vsbbrpi foo(%rip){NT}, %k3, %v2{%k7}
	vshuf128x32 {aaaa}, {0000}, foo(%rip) {NT}, %v0
	vshuf128x32 {aaaa}, {0000}, foo(%rip), %v0
	vstored %v0{a}{sint8},foo(%rip){%k1}{NT}
	vstored %v0{dcba},foo(%rip){%k1}
	vstored %v0{dcba},foo(%rip){%k1}{NT}
	vstored %v0{dcba}{sint8},foo(%rip){%k1}
	vstored %v0{float16rz},foo(%rip){%k1}
	vstored %v0,foo(%rip){%k1}
	vstored %v0,foo(%rip){%k1}{NT}
	vstored %v0{sint8}{a},foo(%rip)
	vstored %v0{sint8}{a},foo(%rip){%k1}{NT}
	vstored %v0{sint8}{dcba},foo(%rip)
	vstored %v0{sint8}{dcba},foo(%rip){%k1}
	vstored %v0{sint8},foo(%rip)
	vstored %v0{sint8},foo(%rip){%k1}
	vstored %v0{sint8},foo(%rip){%k1}{NT}
	vsubrsetbpi foo(%rip){NT}, %k2, %v1{%k7}
	vsubrsetbpi foo(%rip){NT}, %k3, %v2{%k7}
	vsubsetbpi foo(%rip){NT}, %k2, %v1{%k7}
	vsubsetbpi foo(%rip){NT}, %k3, %v2{%k7}
	vtestpi  foo(%rip) {NT}{sint8}, %v0, %k1
	vtestpi  foo(%rip) {NT}, %v0, %k1
	vtestpi  foo(%rip){NT}, %v1, %k1{%k7}
	vtestpi  foo(%rip) {sint8} {NT}, %v0, %k1
	vtestpi  foo(%rip) {sint8}, %v0, %k1
	vtestpi  foo(%rip), %v0, %k1
	vtestpi  foo(%rip), %v0, %k1{%k7}
	vtestpi  foo(%rip), %v0, %k7
	vtestpi  foo(%rip), %v31, %k1
	clevict1 foo(%rip)
	ldvxcsr foo(%rip)
	ldvxcsrl foo(%rip)
	stvxcsr foo(%rip)
	stvxcsrl foo(%rip)
	vprefetch1 $0,foo(%rip)
	vprefetch1 $1,foo(%rip)
	vprefetch1 $7,foo(%rip)
	vprefetch2 $0,foo(%rip)
	vprefetch2 $1,foo(%rip)
	vprefetch2 $3,foo(%rip)

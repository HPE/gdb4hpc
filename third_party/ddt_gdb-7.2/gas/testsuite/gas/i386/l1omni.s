# Check L1OMni instructions

	.allow_index_reg
	.text
_start:

# Tests for op k1, k2
	vkand %k1,%k2
	vkandn %k1,%k2
	vkandnr %k1,%k2
	vkmov %k1,%k2
	vkmovlhb %k1,%k2
	vknot %k1,%k2
	vkor %k1,%k2
	vkortest %k1,%k2
	vkswapb %k1,%k2
	vkxnor %k1,%k2
	vkxor %k1,%k2

# Tests for op r16, k2
	vkmov %ax,%k2
	vkmov %r9w,%k2

# Tests for op r32, k2
	vkmov %eax,%k2
	vkmov %r9d,%k2

# Tests for op k1, r16
	vkmov %k1,%ax
	vkmov %k1,%r9w

# Tests for op k1, r32
	vkmov %k1,%eax
	vkmov %k1,%r9d

# Tests for op v1{k1}, k2, si(v3/mt)
	vadcpi %v3,%k2,%v1{%k1}
	vadcpi %v3{cdab},%k2,%v1{%k1}
	vadcpi %v3{badc},%k2,%v1{%k1}
	vadcpi %v3{aaaa},%k2,%v1{%k1}
	vadcpi %v3{bbbb},%k2,%v1{%k1}
	vadcpi %v3{cccc},%k2,%v1{%k1}
	vadcpi %v3{dddd},%k2,%v1{%k1}
	vadcpi %v3{dacb},%k2,%v1{%k1}
	vadcpi (%rcx){nt},%k2,%v1{%k1}
	vadcpi (%rcx),%k2,%v1{%k1}
	vadcpi (%rcx){16to16},%k2,%v1{%k1}
	vadcpi (%rcx){1to16},%k2,%v1{%k1}
	vadcpi (%rcx){4to16},%k2,%v1{%k1}
	vadcpi (%rcx){uint8},%k2,%v1{%k1}
	vadcpi (%rcx){sint8},%k2,%v1{%k1}
	vadcpi (%rcx){uint16},%k2,%v1{%k1}
	vadcpi (%rcx){sint16},%k2,%v1{%k1}
	vaddsetcpi %v3,%k2,%v1{%k1}
	vaddsetcpi %v3{cdab},%k2,%v1{%k1}
	vaddsetcpi %v3{badc},%k2,%v1{%k1}
	vaddsetcpi %v3{aaaa},%k2,%v1{%k1}
	vaddsetcpi %v3{bbbb},%k2,%v1{%k1}
	vaddsetcpi %v3{cccc},%k2,%v1{%k1}
	vaddsetcpi %v3{dddd},%k2,%v1{%k1}
	vaddsetcpi %v3{dacb},%k2,%v1{%k1}
	vaddsetcpi (%rcx){nt},%k2,%v1{%k1}
	vaddsetcpi (%rcx),%k2,%v1{%k1}
	vaddsetcpi (%rcx){16to16},%k2,%v1{%k1}
	vaddsetcpi (%rcx){1to16},%k2,%v1{%k1}
	vaddsetcpi (%rcx){4to16},%k2,%v1{%k1}
	vaddsetcpi (%rcx){uint8},%k2,%v1{%k1}
	vaddsetcpi (%rcx){sint8},%k2,%v1{%k1}
	vaddsetcpi (%rcx){uint16},%k2,%v1{%k1}
	vaddsetcpi (%rcx){sint16},%k2,%v1{%k1}
	vsbbpi %v3,%k2,%v1{%k1}
	vsbbpi %v3{cdab},%k2,%v1{%k1}
	vsbbpi %v3{badc},%k2,%v1{%k1}
	vsbbpi %v3{aaaa},%k2,%v1{%k1}
	vsbbpi %v3{bbbb},%k2,%v1{%k1}
	vsbbpi %v3{cccc},%k2,%v1{%k1}
	vsbbpi %v3{dddd},%k2,%v1{%k1}
	vsbbpi %v3{dacb},%k2,%v1{%k1}
	vsbbpi (%rcx){nt},%k2,%v1{%k1}
	vsbbpi (%rcx),%k2,%v1{%k1}
	vsbbpi (%rcx){16to16},%k2,%v1{%k1}
	vsbbpi (%rcx){1to16},%k2,%v1{%k1}
	vsbbpi (%rcx){4to16},%k2,%v1{%k1}
	vsbbpi (%rcx){uint8},%k2,%v1{%k1}
	vsbbpi (%rcx){sint8},%k2,%v1{%k1}
	vsbbpi (%rcx){uint16},%k2,%v1{%k1}
	vsbbpi (%rcx){sint16},%k2,%v1{%k1}
	vsbbrpi %v3,%k2,%v1{%k1}
	vsbbrpi %v3{cdab},%k2,%v1{%k1}
	vsbbrpi %v3{badc},%k2,%v1{%k1}
	vsbbrpi %v3{aaaa},%k2,%v1{%k1}
	vsbbrpi %v3{bbbb},%k2,%v1{%k1}
	vsbbrpi %v3{cccc},%k2,%v1{%k1}
	vsbbrpi %v3{dddd},%k2,%v1{%k1}
	vsbbrpi %v3{dacb},%k2,%v1{%k1}
	vsbbrpi (%rcx){nt},%k2,%v1{%k1}
	vsbbrpi (%rcx),%k2,%v1{%k1}
	vsbbrpi (%rcx){16to16},%k2,%v1{%k1}
	vsbbrpi (%rcx){1to16},%k2,%v1{%k1}
	vsbbrpi (%rcx){4to16},%k2,%v1{%k1}
	vsbbrpi (%rcx){uint8},%k2,%v1{%k1}
	vsbbrpi (%rcx){sint8},%k2,%v1{%k1}
	vsbbrpi (%rcx){uint16},%k2,%v1{%k1}
	vsbbrpi (%rcx){sint16},%k2,%v1{%k1}
	vsubrsetbpi %v3,%k2,%v1{%k1}
	vsubrsetbpi %v3{cdab},%k2,%v1{%k1}
	vsubrsetbpi %v3{badc},%k2,%v1{%k1}
	vsubrsetbpi %v3{aaaa},%k2,%v1{%k1}
	vsubrsetbpi %v3{bbbb},%k2,%v1{%k1}
	vsubrsetbpi %v3{cccc},%k2,%v1{%k1}
	vsubrsetbpi %v3{dddd},%k2,%v1{%k1}
	vsubrsetbpi %v3{dacb},%k2,%v1{%k1}
	vsubrsetbpi (%rcx){nt},%k2,%v1{%k1}
	vsubrsetbpi (%rcx),%k2,%v1{%k1}
	vsubrsetbpi (%rcx){16to16},%k2,%v1{%k1}
	vsubrsetbpi (%rcx){1to16},%k2,%v1{%k1}
	vsubrsetbpi (%rcx){4to16},%k2,%v1{%k1}
	vsubrsetbpi (%rcx){uint8},%k2,%v1{%k1}
	vsubrsetbpi (%rcx){sint8},%k2,%v1{%k1}
	vsubrsetbpi (%rcx){uint16},%k2,%v1{%k1}
	vsubrsetbpi (%rcx){sint16},%k2,%v1{%k1}
	vsubsetbpi %v3,%k2,%v1{%k1}
	vsubsetbpi %v3{cdab},%k2,%v1{%k1}
	vsubsetbpi %v3{badc},%k2,%v1{%k1}
	vsubsetbpi %v3{aaaa},%k2,%v1{%k1}
	vsubsetbpi %v3{bbbb},%k2,%v1{%k1}
	vsubsetbpi %v3{cccc},%k2,%v1{%k1}
	vsubsetbpi %v3{dddd},%k2,%v1{%k1}
	vsubsetbpi %v3{dacb},%k2,%v1{%k1}
	vsubsetbpi (%rcx){nt},%k2,%v1{%k1}
	vsubsetbpi (%rcx),%k2,%v1{%k1}
	vsubsetbpi (%rcx){16to16},%k2,%v1{%k1}
	vsubsetbpi (%rcx){1to16},%k2,%v1{%k1}
	vsubsetbpi (%rcx){4to16},%k2,%v1{%k1}
	vsubsetbpi (%rcx){uint8},%k2,%v1{%k1}
	vsubsetbpi (%rcx){sint8},%k2,%v1{%k1}
	vsubsetbpi (%rcx){uint16},%k2,%v1{%k1}
	vsubsetbpi (%rcx){sint16},%k2,%v1{%k1}

# Tests for op v1{k1}, v2, sf(v3/mt)
	vaddnps %v3,%v2,%v1{%k1}
	vaddnps %v3{cdab},%v2,%v1{%k1}
	vaddnps %v3{badc},%v2,%v1{%k1}
	vaddnps %v3{aaaa},%v2,%v1{%k1}
	vaddnps %v3{bbbb},%v2,%v1{%k1}
	vaddnps %v3{cccc},%v2,%v1{%k1}
	vaddnps %v3{dddd},%v2,%v1{%k1}
	vaddnps %v3{dacb},%v2,%v1{%k1}
	vaddnps (%rcx){nt},%v2,%v1{%k1}
	vaddnps (%rcx),%v2,%v1{%k1}
	vaddnps (%rcx){16to16},%v2,%v1{%k1}
	vaddnps (%rcx){1to16},%v2,%v1{%k1}
	vaddnps (%rcx){4to16},%v2,%v1{%k1}
	vaddnps (%rcx){uint8},%v2,%v1{%k1}
	vaddnps (%rcx){unorm8},%v2,%v1{%k1}
	vaddnps (%rcx){float16},%v2,%v1{%k1}
	vaddnps (%rcx){sint16},%v2,%v1{%k1}
	vaddps %v3,%v2,%v1{%k1}
	vaddps %v3{cdab},%v2,%v1{%k1}
	vaddps %v3{badc},%v2,%v1{%k1}
	vaddps %v3{aaaa},%v2,%v1{%k1}
	vaddps %v3{bbbb},%v2,%v1{%k1}
	vaddps %v3{cccc},%v2,%v1{%k1}
	vaddps %v3{dddd},%v2,%v1{%k1}
	vaddps %v3{dacb},%v2,%v1{%k1}
	vaddps (%rcx){nt},%v2,%v1{%k1}
	vaddps (%rcx),%v2,%v1{%k1}
	vaddps (%rcx){16to16},%v2,%v1{%k1}
	vaddps (%rcx){1to16},%v2,%v1{%k1}
	vaddps (%rcx){4to16},%v2,%v1{%k1}
	vaddps (%rcx){uint8},%v2,%v1{%k1}
	vaddps (%rcx){unorm8},%v2,%v1{%k1}
	vaddps (%rcx){float16},%v2,%v1{%k1}
	vaddps (%rcx){sint16},%v2,%v1{%k1}
	vaddsetsps %v3,%v2,%v1{%k1}
	vaddsetsps %v3{cdab},%v2,%v1{%k1}
	vaddsetsps %v3{badc},%v2,%v1{%k1}
	vaddsetsps %v3{aaaa},%v2,%v1{%k1}
	vaddsetsps %v3{bbbb},%v2,%v1{%k1}
	vaddsetsps %v3{cccc},%v2,%v1{%k1}
	vaddsetsps %v3{dddd},%v2,%v1{%k1}
	vaddsetsps %v3{dacb},%v2,%v1{%k1}
	vaddsetsps (%rcx){nt},%v2,%v1{%k1}
	vaddsetsps (%rcx),%v2,%v1{%k1}
	vaddsetsps (%rcx){16to16},%v2,%v1{%k1}
	vaddsetsps (%rcx){1to16},%v2,%v1{%k1}
	vaddsetsps (%rcx){4to16},%v2,%v1{%k1}
	vaddsetsps (%rcx){uint8},%v2,%v1{%k1}
	vaddsetsps (%rcx){unorm8},%v2,%v1{%k1}
	vaddsetsps (%rcx){float16},%v2,%v1{%k1}
	vaddsetsps (%rcx){sint16},%v2,%v1{%k1}
	vclampzps %v3,%v2,%v1{%k1}
	vclampzps %v3{cdab},%v2,%v1{%k1}
	vclampzps %v3{badc},%v2,%v1{%k1}
	vclampzps %v3{aaaa},%v2,%v1{%k1}
	vclampzps %v3{bbbb},%v2,%v1{%k1}
	vclampzps %v3{cccc},%v2,%v1{%k1}
	vclampzps %v3{dddd},%v2,%v1{%k1}
	vclampzps %v3{dacb},%v2,%v1{%k1}
	vclampzps (%rcx){nt},%v2,%v1{%k1}
	vclampzps (%rcx),%v2,%v1{%k1}
	vclampzps (%rcx){16to16},%v2,%v1{%k1}
	vclampzps (%rcx){1to16},%v2,%v1{%k1}
	vclampzps (%rcx){4to16},%v2,%v1{%k1}
	vclampzps (%rcx){uint8},%v2,%v1{%k1}
	vclampzps (%rcx){unorm8},%v2,%v1{%k1}
	vclampzps (%rcx){float16},%v2,%v1{%k1}
	vclampzps (%rcx){sint16},%v2,%v1{%k1}
	vmadd132ps %v3,%v2,%v1{%k1}
	vmadd132ps %v3{cdab},%v2,%v1{%k1}
	vmadd132ps %v3{badc},%v2,%v1{%k1}
	vmadd132ps %v3{aaaa},%v2,%v1{%k1}
	vmadd132ps %v3{bbbb},%v2,%v1{%k1}
	vmadd132ps %v3{cccc},%v2,%v1{%k1}
	vmadd132ps %v3{dddd},%v2,%v1{%k1}
	vmadd132ps %v3{dacb},%v2,%v1{%k1}
	vmadd132ps (%rcx){nt},%v2,%v1{%k1}
	vmadd132ps (%rcx),%v2,%v1{%k1}
	vmadd132ps (%rcx){16to16},%v2,%v1{%k1}
	vmadd132ps (%rcx){1to16},%v2,%v1{%k1}
	vmadd132ps (%rcx){4to16},%v2,%v1{%k1}
	vmadd132ps (%rcx){uint8},%v2,%v1{%k1}
	vmadd132ps (%rcx){unorm8},%v2,%v1{%k1}
	vmadd132ps (%rcx){float16},%v2,%v1{%k1}
	vmadd132ps (%rcx){sint16},%v2,%v1{%k1}
	vmadd213ps %v3,%v2,%v1{%k1}
	vmadd213ps %v3{cdab},%v2,%v1{%k1}
	vmadd213ps %v3{badc},%v2,%v1{%k1}
	vmadd213ps %v3{aaaa},%v2,%v1{%k1}
	vmadd213ps %v3{bbbb},%v2,%v1{%k1}
	vmadd213ps %v3{cccc},%v2,%v1{%k1}
	vmadd213ps %v3{dddd},%v2,%v1{%k1}
	vmadd213ps %v3{dacb},%v2,%v1{%k1}
	vmadd213ps (%rcx){nt},%v2,%v1{%k1}
	vmadd213ps (%rcx),%v2,%v1{%k1}
	vmadd213ps (%rcx){16to16},%v2,%v1{%k1}
	vmadd213ps (%rcx){1to16},%v2,%v1{%k1}
	vmadd213ps (%rcx){4to16},%v2,%v1{%k1}
	vmadd213ps (%rcx){uint8},%v2,%v1{%k1}
	vmadd213ps (%rcx){unorm8},%v2,%v1{%k1}
	vmadd213ps (%rcx){float16},%v2,%v1{%k1}
	vmadd213ps (%rcx){sint16},%v2,%v1{%k1}
	vmadd231ps %v3,%v2,%v1{%k1}
	vmadd231ps %v3{cdab},%v2,%v1{%k1}
	vmadd231ps %v3{badc},%v2,%v1{%k1}
	vmadd231ps %v3{aaaa},%v2,%v1{%k1}
	vmadd231ps %v3{bbbb},%v2,%v1{%k1}
	vmadd231ps %v3{cccc},%v2,%v1{%k1}
	vmadd231ps %v3{dddd},%v2,%v1{%k1}
	vmadd231ps %v3{dacb},%v2,%v1{%k1}
	vmadd231ps (%rcx){nt},%v2,%v1{%k1}
	vmadd231ps (%rcx),%v2,%v1{%k1}
	vmadd231ps (%rcx){16to16},%v2,%v1{%k1}
	vmadd231ps (%rcx){1to16},%v2,%v1{%k1}
	vmadd231ps (%rcx){4to16},%v2,%v1{%k1}
	vmadd231ps (%rcx){uint8},%v2,%v1{%k1}
	vmadd231ps (%rcx){unorm8},%v2,%v1{%k1}
	vmadd231ps (%rcx){float16},%v2,%v1{%k1}
	vmadd231ps (%rcx){sint16},%v2,%v1{%k1}
	vmadd233ps %v3,%v2,%v1{%k1}
	vmadd233ps %v3{cdab},%v2,%v1{%k1}
	vmadd233ps %v3{badc},%v2,%v1{%k1}
	vmadd233ps %v3{aaaa},%v2,%v1{%k1}
	vmadd233ps %v3{bbbb},%v2,%v1{%k1}
	vmadd233ps %v3{cccc},%v2,%v1{%k1}
	vmadd233ps %v3{dddd},%v2,%v1{%k1}
	vmadd233ps %v3{dacb},%v2,%v1{%k1}
	vmadd233ps (%rcx){nt},%v2,%v1{%k1}
	vmadd233ps (%rcx),%v2,%v1{%k1}
	vmadd233ps (%rcx){16to16},%v2,%v1{%k1}
	vmadd233ps (%rcx){1to16},%v2,%v1{%k1}
	vmadd233ps (%rcx){4to16},%v2,%v1{%k1}
	vmadd233ps (%rcx){uint8},%v2,%v1{%k1}
	vmadd233ps (%rcx){unorm8},%v2,%v1{%k1}
	vmadd233ps (%rcx){float16},%v2,%v1{%k1}
	vmadd233ps (%rcx){sint16},%v2,%v1{%k1}
	vmaddn132ps %v3,%v2,%v1{%k1}
	vmaddn132ps %v3{cdab},%v2,%v1{%k1}
	vmaddn132ps %v3{badc},%v2,%v1{%k1}
	vmaddn132ps %v3{aaaa},%v2,%v1{%k1}
	vmaddn132ps %v3{bbbb},%v2,%v1{%k1}
	vmaddn132ps %v3{cccc},%v2,%v1{%k1}
	vmaddn132ps %v3{dddd},%v2,%v1{%k1}
	vmaddn132ps %v3{dacb},%v2,%v1{%k1}
	vmaddn132ps (%rcx){nt},%v2,%v1{%k1}
	vmaddn132ps (%rcx),%v2,%v1{%k1}
	vmaddn132ps (%rcx){16to16},%v2,%v1{%k1}
	vmaddn132ps (%rcx){1to16},%v2,%v1{%k1}
	vmaddn132ps (%rcx){4to16},%v2,%v1{%k1}
	vmaddn132ps (%rcx){uint8},%v2,%v1{%k1}
	vmaddn132ps (%rcx){unorm8},%v2,%v1{%k1}
	vmaddn132ps (%rcx){float16},%v2,%v1{%k1}
	vmaddn132ps (%rcx){sint16},%v2,%v1{%k1}
	vmaddn213ps %v3,%v2,%v1{%k1}
	vmaddn213ps %v3{cdab},%v2,%v1{%k1}
	vmaddn213ps %v3{badc},%v2,%v1{%k1}
	vmaddn213ps %v3{aaaa},%v2,%v1{%k1}
	vmaddn213ps %v3{bbbb},%v2,%v1{%k1}
	vmaddn213ps %v3{cccc},%v2,%v1{%k1}
	vmaddn213ps %v3{dddd},%v2,%v1{%k1}
	vmaddn213ps %v3{dacb},%v2,%v1{%k1}
	vmaddn213ps (%rcx){nt},%v2,%v1{%k1}
	vmaddn213ps (%rcx),%v2,%v1{%k1}
	vmaddn213ps (%rcx){16to16},%v2,%v1{%k1}
	vmaddn213ps (%rcx){1to16},%v2,%v1{%k1}
	vmaddn213ps (%rcx){4to16},%v2,%v1{%k1}
	vmaddn213ps (%rcx){uint8},%v2,%v1{%k1}
	vmaddn213ps (%rcx){unorm8},%v2,%v1{%k1}
	vmaddn213ps (%rcx){float16},%v2,%v1{%k1}
	vmaddn213ps (%rcx){sint16},%v2,%v1{%k1}
	vmaddn231ps %v3,%v2,%v1{%k1}
	vmaddn231ps %v3{cdab},%v2,%v1{%k1}
	vmaddn231ps %v3{badc},%v2,%v1{%k1}
	vmaddn231ps %v3{aaaa},%v2,%v1{%k1}
	vmaddn231ps %v3{bbbb},%v2,%v1{%k1}
	vmaddn231ps %v3{cccc},%v2,%v1{%k1}
	vmaddn231ps %v3{dddd},%v2,%v1{%k1}
	vmaddn231ps %v3{dacb},%v2,%v1{%k1}
	vmaddn231ps (%rcx){nt},%v2,%v1{%k1}
	vmaddn231ps (%rcx),%v2,%v1{%k1}
	vmaddn231ps (%rcx){16to16},%v2,%v1{%k1}
	vmaddn231ps (%rcx){1to16},%v2,%v1{%k1}
	vmaddn231ps (%rcx){4to16},%v2,%v1{%k1}
	vmaddn231ps (%rcx){uint8},%v2,%v1{%k1}
	vmaddn231ps (%rcx){unorm8},%v2,%v1{%k1}
	vmaddn231ps (%rcx){float16},%v2,%v1{%k1}
	vmaddn231ps (%rcx){sint16},%v2,%v1{%k1}
	vmaxabsps %v3,%v2,%v1{%k1}
	vmaxabsps %v3{cdab},%v2,%v1{%k1}
	vmaxabsps %v3{badc},%v2,%v1{%k1}
	vmaxabsps %v3{aaaa},%v2,%v1{%k1}
	vmaxabsps %v3{bbbb},%v2,%v1{%k1}
	vmaxabsps %v3{cccc},%v2,%v1{%k1}
	vmaxabsps %v3{dddd},%v2,%v1{%k1}
	vmaxabsps %v3{dacb},%v2,%v1{%k1}
	vmaxabsps (%rcx){nt},%v2,%v1{%k1}
	vmaxabsps (%rcx),%v2,%v1{%k1}
	vmaxabsps (%rcx){16to16},%v2,%v1{%k1}
	vmaxabsps (%rcx){1to16},%v2,%v1{%k1}
	vmaxabsps (%rcx){4to16},%v2,%v1{%k1}
	vmaxabsps (%rcx){uint8},%v2,%v1{%k1}
	vmaxabsps (%rcx){unorm8},%v2,%v1{%k1}
	vmaxabsps (%rcx){float16},%v2,%v1{%k1}
	vmaxabsps (%rcx){sint16},%v2,%v1{%k1}
	vmaxps %v3,%v2,%v1{%k1}
	vmaxps %v3{cdab},%v2,%v1{%k1}
	vmaxps %v3{badc},%v2,%v1{%k1}
	vmaxps %v3{aaaa},%v2,%v1{%k1}
	vmaxps %v3{bbbb},%v2,%v1{%k1}
	vmaxps %v3{cccc},%v2,%v1{%k1}
	vmaxps %v3{dddd},%v2,%v1{%k1}
	vmaxps %v3{dacb},%v2,%v1{%k1}
	vmaxps (%rcx){nt},%v2,%v1{%k1}
	vmaxps (%rcx),%v2,%v1{%k1}
	vmaxps (%rcx){16to16},%v2,%v1{%k1}
	vmaxps (%rcx){1to16},%v2,%v1{%k1}
	vmaxps (%rcx){4to16},%v2,%v1{%k1}
	vmaxps (%rcx){uint8},%v2,%v1{%k1}
	vmaxps (%rcx){unorm8},%v2,%v1{%k1}
	vmaxps (%rcx){float16},%v2,%v1{%k1}
	vmaxps (%rcx){sint16},%v2,%v1{%k1}
	vminps %v3,%v2,%v1{%k1}
	vminps %v3{cdab},%v2,%v1{%k1}
	vminps %v3{badc},%v2,%v1{%k1}
	vminps %v3{aaaa},%v2,%v1{%k1}
	vminps %v3{bbbb},%v2,%v1{%k1}
	vminps %v3{cccc},%v2,%v1{%k1}
	vminps %v3{dddd},%v2,%v1{%k1}
	vminps %v3{dacb},%v2,%v1{%k1}
	vminps (%rcx){nt},%v2,%v1{%k1}
	vminps (%rcx),%v2,%v1{%k1}
	vminps (%rcx){16to16},%v2,%v1{%k1}
	vminps (%rcx){1to16},%v2,%v1{%k1}
	vminps (%rcx){4to16},%v2,%v1{%k1}
	vminps (%rcx){uint8},%v2,%v1{%k1}
	vminps (%rcx){unorm8},%v2,%v1{%k1}
	vminps (%rcx){float16},%v2,%v1{%k1}
	vminps (%rcx){sint16},%v2,%v1{%k1}
	vmsub132ps %v3,%v2,%v1{%k1}
	vmsub132ps %v3{cdab},%v2,%v1{%k1}
	vmsub132ps %v3{badc},%v2,%v1{%k1}
	vmsub132ps %v3{aaaa},%v2,%v1{%k1}
	vmsub132ps %v3{bbbb},%v2,%v1{%k1}
	vmsub132ps %v3{cccc},%v2,%v1{%k1}
	vmsub132ps %v3{dddd},%v2,%v1{%k1}
	vmsub132ps %v3{dacb},%v2,%v1{%k1}
	vmsub132ps (%rcx){nt},%v2,%v1{%k1}
	vmsub132ps (%rcx),%v2,%v1{%k1}
	vmsub132ps (%rcx){16to16},%v2,%v1{%k1}
	vmsub132ps (%rcx){1to16},%v2,%v1{%k1}
	vmsub132ps (%rcx){4to16},%v2,%v1{%k1}
	vmsub132ps (%rcx){uint8},%v2,%v1{%k1}
	vmsub132ps (%rcx){unorm8},%v2,%v1{%k1}
	vmsub132ps (%rcx){float16},%v2,%v1{%k1}
	vmsub132ps (%rcx){sint16},%v2,%v1{%k1}
	vmsub213ps %v3,%v2,%v1{%k1}
	vmsub213ps %v3{cdab},%v2,%v1{%k1}
	vmsub213ps %v3{badc},%v2,%v1{%k1}
	vmsub213ps %v3{aaaa},%v2,%v1{%k1}
	vmsub213ps %v3{bbbb},%v2,%v1{%k1}
	vmsub213ps %v3{cccc},%v2,%v1{%k1}
	vmsub213ps %v3{dddd},%v2,%v1{%k1}
	vmsub213ps %v3{dacb},%v2,%v1{%k1}
	vmsub213ps (%rcx){nt},%v2,%v1{%k1}
	vmsub213ps (%rcx),%v2,%v1{%k1}
	vmsub213ps (%rcx){16to16},%v2,%v1{%k1}
	vmsub213ps (%rcx){1to16},%v2,%v1{%k1}
	vmsub213ps (%rcx){4to16},%v2,%v1{%k1}
	vmsub213ps (%rcx){uint8},%v2,%v1{%k1}
	vmsub213ps (%rcx){unorm8},%v2,%v1{%k1}
	vmsub213ps (%rcx){float16},%v2,%v1{%k1}
	vmsub213ps (%rcx){sint16},%v2,%v1{%k1}
	vmsub231ps %v3,%v2,%v1{%k1}
	vmsub231ps %v3{cdab},%v2,%v1{%k1}
	vmsub231ps %v3{badc},%v2,%v1{%k1}
	vmsub231ps %v3{aaaa},%v2,%v1{%k1}
	vmsub231ps %v3{bbbb},%v2,%v1{%k1}
	vmsub231ps %v3{cccc},%v2,%v1{%k1}
	vmsub231ps %v3{dddd},%v2,%v1{%k1}
	vmsub231ps %v3{dacb},%v2,%v1{%k1}
	vmsub231ps (%rcx){nt},%v2,%v1{%k1}
	vmsub231ps (%rcx),%v2,%v1{%k1}
	vmsub231ps (%rcx){16to16},%v2,%v1{%k1}
	vmsub231ps (%rcx){1to16},%v2,%v1{%k1}
	vmsub231ps (%rcx){4to16},%v2,%v1{%k1}
	vmsub231ps (%rcx){uint8},%v2,%v1{%k1}
	vmsub231ps (%rcx){unorm8},%v2,%v1{%k1}
	vmsub231ps (%rcx){float16},%v2,%v1{%k1}
	vmsub231ps (%rcx){sint16},%v2,%v1{%k1}
	vmsubr132ps %v3,%v2,%v1{%k1}
	vmsubr132ps %v3{cdab},%v2,%v1{%k1}
	vmsubr132ps %v3{badc},%v2,%v1{%k1}
	vmsubr132ps %v3{aaaa},%v2,%v1{%k1}
	vmsubr132ps %v3{bbbb},%v2,%v1{%k1}
	vmsubr132ps %v3{cccc},%v2,%v1{%k1}
	vmsubr132ps %v3{dddd},%v2,%v1{%k1}
	vmsubr132ps %v3{dacb},%v2,%v1{%k1}
	vmsubr132ps (%rcx){nt},%v2,%v1{%k1}
	vmsubr132ps (%rcx),%v2,%v1{%k1}
	vmsubr132ps (%rcx){16to16},%v2,%v1{%k1}
	vmsubr132ps (%rcx){1to16},%v2,%v1{%k1}
	vmsubr132ps (%rcx){4to16},%v2,%v1{%k1}
	vmsubr132ps (%rcx){uint8},%v2,%v1{%k1}
	vmsubr132ps (%rcx){unorm8},%v2,%v1{%k1}
	vmsubr132ps (%rcx){float16},%v2,%v1{%k1}
	vmsubr132ps (%rcx){sint16},%v2,%v1{%k1}
	vmsubr213ps %v3,%v2,%v1{%k1}
	vmsubr213ps %v3{cdab},%v2,%v1{%k1}
	vmsubr213ps %v3{badc},%v2,%v1{%k1}
	vmsubr213ps %v3{aaaa},%v2,%v1{%k1}
	vmsubr213ps %v3{bbbb},%v2,%v1{%k1}
	vmsubr213ps %v3{cccc},%v2,%v1{%k1}
	vmsubr213ps %v3{dddd},%v2,%v1{%k1}
	vmsubr213ps %v3{dacb},%v2,%v1{%k1}
	vmsubr213ps (%rcx){nt},%v2,%v1{%k1}
	vmsubr213ps (%rcx),%v2,%v1{%k1}
	vmsubr213ps (%rcx){16to16},%v2,%v1{%k1}
	vmsubr213ps (%rcx){1to16},%v2,%v1{%k1}
	vmsubr213ps (%rcx){4to16},%v2,%v1{%k1}
	vmsubr213ps (%rcx){uint8},%v2,%v1{%k1}
	vmsubr213ps (%rcx){unorm8},%v2,%v1{%k1}
	vmsubr213ps (%rcx){float16},%v2,%v1{%k1}
	vmsubr213ps (%rcx){sint16},%v2,%v1{%k1}
	vmsubr231ps %v3,%v2,%v1{%k1}
	vmsubr231ps %v3{cdab},%v2,%v1{%k1}
	vmsubr231ps %v3{badc},%v2,%v1{%k1}
	vmsubr231ps %v3{aaaa},%v2,%v1{%k1}
	vmsubr231ps %v3{bbbb},%v2,%v1{%k1}
	vmsubr231ps %v3{cccc},%v2,%v1{%k1}
	vmsubr231ps %v3{dddd},%v2,%v1{%k1}
	vmsubr231ps %v3{dacb},%v2,%v1{%k1}
	vmsubr231ps (%rcx){nt},%v2,%v1{%k1}
	vmsubr231ps (%rcx),%v2,%v1{%k1}
	vmsubr231ps (%rcx){16to16},%v2,%v1{%k1}
	vmsubr231ps (%rcx){1to16},%v2,%v1{%k1}
	vmsubr231ps (%rcx){4to16},%v2,%v1{%k1}
	vmsubr231ps (%rcx){uint8},%v2,%v1{%k1}
	vmsubr231ps (%rcx){unorm8},%v2,%v1{%k1}
	vmsubr231ps (%rcx){float16},%v2,%v1{%k1}
	vmsubr231ps (%rcx){sint16},%v2,%v1{%k1}
	vmsubr23c1ps %v3,%v2,%v1{%k1}
	vmsubr23c1ps %v3{cdab},%v2,%v1{%k1}
	vmsubr23c1ps %v3{badc},%v2,%v1{%k1}
	vmsubr23c1ps %v3{aaaa},%v2,%v1{%k1}
	vmsubr23c1ps %v3{bbbb},%v2,%v1{%k1}
	vmsubr23c1ps %v3{cccc},%v2,%v1{%k1}
	vmsubr23c1ps %v3{dddd},%v2,%v1{%k1}
	vmsubr23c1ps %v3{dacb},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){nt},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx),%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){16to16},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){1to16},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){4to16},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){uint8},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){unorm8},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){float16},%v2,%v1{%k1}
	vmsubr23c1ps (%rcx){sint16},%v2,%v1{%k1}
	vmulps %v3,%v2,%v1{%k1}
	vmulps %v3{cdab},%v2,%v1{%k1}
	vmulps %v3{badc},%v2,%v1{%k1}
	vmulps %v3{aaaa},%v2,%v1{%k1}
	vmulps %v3{bbbb},%v2,%v1{%k1}
	vmulps %v3{cccc},%v2,%v1{%k1}
	vmulps %v3{dddd},%v2,%v1{%k1}
	vmulps %v3{dacb},%v2,%v1{%k1}
	vmulps (%rcx){nt},%v2,%v1{%k1}
	vmulps (%rcx),%v2,%v1{%k1}
	vmulps (%rcx){16to16},%v2,%v1{%k1}
	vmulps (%rcx){1to16},%v2,%v1{%k1}
	vmulps (%rcx){4to16},%v2,%v1{%k1}
	vmulps (%rcx){uint8},%v2,%v1{%k1}
	vmulps (%rcx){unorm8},%v2,%v1{%k1}
	vmulps (%rcx){float16},%v2,%v1{%k1}
	vmulps (%rcx){sint16},%v2,%v1{%k1}
	vrcprefineps %v3,%v2,%v1{%k1}
	vrcprefineps %v3{cdab},%v2,%v1{%k1}
	vrcprefineps %v3{badc},%v2,%v1{%k1}
	vrcprefineps %v3{aaaa},%v2,%v1{%k1}
	vrcprefineps %v3{bbbb},%v2,%v1{%k1}
	vrcprefineps %v3{cccc},%v2,%v1{%k1}
	vrcprefineps %v3{dddd},%v2,%v1{%k1}
	vrcprefineps %v3{dacb},%v2,%v1{%k1}
	vrcprefineps (%rcx){nt},%v2,%v1{%k1}
	vrcprefineps (%rcx),%v2,%v1{%k1}
	vrcprefineps (%rcx){16to16},%v2,%v1{%k1}
	vrcprefineps (%rcx){1to16},%v2,%v1{%k1}
	vrcprefineps (%rcx){4to16},%v2,%v1{%k1}
	vrcprefineps (%rcx){uint8},%v2,%v1{%k1}
	vrcprefineps (%rcx){unorm8},%v2,%v1{%k1}
	vrcprefineps (%rcx){float16},%v2,%v1{%k1}
	vrcprefineps (%rcx){sint16},%v2,%v1{%k1}
	vsubps %v3,%v2,%v1{%k1}
	vsubps %v3{cdab},%v2,%v1{%k1}
	vsubps %v3{badc},%v2,%v1{%k1}
	vsubps %v3{aaaa},%v2,%v1{%k1}
	vsubps %v3{bbbb},%v2,%v1{%k1}
	vsubps %v3{cccc},%v2,%v1{%k1}
	vsubps %v3{dddd},%v2,%v1{%k1}
	vsubps %v3{dacb},%v2,%v1{%k1}
	vsubps (%rcx){nt},%v2,%v1{%k1}
	vsubps (%rcx),%v2,%v1{%k1}
	vsubps (%rcx){16to16},%v2,%v1{%k1}
	vsubps (%rcx){1to16},%v2,%v1{%k1}
	vsubps (%rcx){4to16},%v2,%v1{%k1}
	vsubps (%rcx){uint8},%v2,%v1{%k1}
	vsubps (%rcx){unorm8},%v2,%v1{%k1}
	vsubps (%rcx){float16},%v2,%v1{%k1}
	vsubps (%rcx){sint16},%v2,%v1{%k1}
	vsubrps %v3,%v2,%v1{%k1}
	vsubrps %v3{cdab},%v2,%v1{%k1}
	vsubrps %v3{badc},%v2,%v1{%k1}
	vsubrps %v3{aaaa},%v2,%v1{%k1}
	vsubrps %v3{bbbb},%v2,%v1{%k1}
	vsubrps %v3{cccc},%v2,%v1{%k1}
	vsubrps %v3{dddd},%v2,%v1{%k1}
	vsubrps %v3{dacb},%v2,%v1{%k1}
	vsubrps (%rcx){nt},%v2,%v1{%k1}
	vsubrps (%rcx),%v2,%v1{%k1}
	vsubrps (%rcx){16to16},%v2,%v1{%k1}
	vsubrps (%rcx){1to16},%v2,%v1{%k1}
	vsubrps (%rcx){4to16},%v2,%v1{%k1}
	vsubrps (%rcx){uint8},%v2,%v1{%k1}
	vsubrps (%rcx){unorm8},%v2,%v1{%k1}
	vsubrps (%rcx){float16},%v2,%v1{%k1}
	vsubrps (%rcx){sint16},%v2,%v1{%k1}

# Tests for op v1{k1}, v2, sd(v3/mt)
	vaddnpd %v3,%v2,%v1{%k1}
	vaddnpd %v3{cdab},%v2,%v1{%k1}
	vaddnpd %v3{badc},%v2,%v1{%k1}
	vaddnpd %v3{aaaa},%v2,%v1{%k1}
	vaddnpd %v3{bbbb},%v2,%v1{%k1}
	vaddnpd %v3{cccc},%v2,%v1{%k1}
	vaddnpd %v3{dddd},%v2,%v1{%k1}
	vaddnpd %v3{dacb},%v2,%v1{%k1}
	vaddnpd (%rcx){nt},%v2,%v1{%k1}
	vaddnpd (%rcx),%v2,%v1{%k1}
	vaddnpd (%rcx){8to8},%v2,%v1{%k1}
	vaddnpd (%rcx){1to8},%v2,%v1{%k1}
	vaddnpd (%rcx){4to8},%v2,%v1{%k1}
	vaddpd %v3,%v2,%v1{%k1}
	vaddpd %v3{cdab},%v2,%v1{%k1}
	vaddpd %v3{badc},%v2,%v1{%k1}
	vaddpd %v3{aaaa},%v2,%v1{%k1}
	vaddpd %v3{bbbb},%v2,%v1{%k1}
	vaddpd %v3{cccc},%v2,%v1{%k1}
	vaddpd %v3{dddd},%v2,%v1{%k1}
	vaddpd %v3{dacb},%v2,%v1{%k1}
	vaddpd (%rcx){nt},%v2,%v1{%k1}
	vaddpd (%rcx),%v2,%v1{%k1}
	vaddpd (%rcx){8to8},%v2,%v1{%k1}
	vaddpd (%rcx){1to8},%v2,%v1{%k1}
	vaddpd (%rcx){4to8},%v2,%v1{%k1}
	vmadd132pd %v3,%v2,%v1{%k1}
	vmadd132pd %v3{cdab},%v2,%v1{%k1}
	vmadd132pd %v3{badc},%v2,%v1{%k1}
	vmadd132pd %v3{aaaa},%v2,%v1{%k1}
	vmadd132pd %v3{bbbb},%v2,%v1{%k1}
	vmadd132pd %v3{cccc},%v2,%v1{%k1}
	vmadd132pd %v3{dddd},%v2,%v1{%k1}
	vmadd132pd %v3{dacb},%v2,%v1{%k1}
	vmadd132pd (%rcx){nt},%v2,%v1{%k1}
	vmadd132pd (%rcx),%v2,%v1{%k1}
	vmadd132pd (%rcx){8to8},%v2,%v1{%k1}
	vmadd132pd (%rcx){1to8},%v2,%v1{%k1}
	vmadd132pd (%rcx){4to8},%v2,%v1{%k1}
	vmadd213pd %v3,%v2,%v1{%k1}
	vmadd213pd %v3{cdab},%v2,%v1{%k1}
	vmadd213pd %v3{badc},%v2,%v1{%k1}
	vmadd213pd %v3{aaaa},%v2,%v1{%k1}
	vmadd213pd %v3{bbbb},%v2,%v1{%k1}
	vmadd213pd %v3{cccc},%v2,%v1{%k1}
	vmadd213pd %v3{dddd},%v2,%v1{%k1}
	vmadd213pd %v3{dacb},%v2,%v1{%k1}
	vmadd213pd (%rcx){nt},%v2,%v1{%k1}
	vmadd213pd (%rcx),%v2,%v1{%k1}
	vmadd213pd (%rcx){8to8},%v2,%v1{%k1}
	vmadd213pd (%rcx){1to8},%v2,%v1{%k1}
	vmadd213pd (%rcx){4to8},%v2,%v1{%k1}
	vmadd231pd %v3,%v2,%v1{%k1}
	vmadd231pd %v3{cdab},%v2,%v1{%k1}
	vmadd231pd %v3{badc},%v2,%v1{%k1}
	vmadd231pd %v3{aaaa},%v2,%v1{%k1}
	vmadd231pd %v3{bbbb},%v2,%v1{%k1}
	vmadd231pd %v3{cccc},%v2,%v1{%k1}
	vmadd231pd %v3{dddd},%v2,%v1{%k1}
	vmadd231pd %v3{dacb},%v2,%v1{%k1}
	vmadd231pd (%rcx){nt},%v2,%v1{%k1}
	vmadd231pd (%rcx),%v2,%v1{%k1}
	vmadd231pd (%rcx){8to8},%v2,%v1{%k1}
	vmadd231pd (%rcx){1to8},%v2,%v1{%k1}
	vmadd231pd (%rcx){4to8},%v2,%v1{%k1}
	vmaddn132pd %v3,%v2,%v1{%k1}
	vmaddn132pd %v3{cdab},%v2,%v1{%k1}
	vmaddn132pd %v3{badc},%v2,%v1{%k1}
	vmaddn132pd %v3{aaaa},%v2,%v1{%k1}
	vmaddn132pd %v3{bbbb},%v2,%v1{%k1}
	vmaddn132pd %v3{cccc},%v2,%v1{%k1}
	vmaddn132pd %v3{dddd},%v2,%v1{%k1}
	vmaddn132pd %v3{dacb},%v2,%v1{%k1}
	vmaddn132pd (%rcx){nt},%v2,%v1{%k1}
	vmaddn132pd (%rcx),%v2,%v1{%k1}
	vmaddn132pd (%rcx){8to8},%v2,%v1{%k1}
	vmaddn132pd (%rcx){1to8},%v2,%v1{%k1}
	vmaddn132pd (%rcx){4to8},%v2,%v1{%k1}
	vmaddn213pd %v3,%v2,%v1{%k1}
	vmaddn213pd %v3{cdab},%v2,%v1{%k1}
	vmaddn213pd %v3{badc},%v2,%v1{%k1}
	vmaddn213pd %v3{aaaa},%v2,%v1{%k1}
	vmaddn213pd %v3{bbbb},%v2,%v1{%k1}
	vmaddn213pd %v3{cccc},%v2,%v1{%k1}
	vmaddn213pd %v3{dddd},%v2,%v1{%k1}
	vmaddn213pd %v3{dacb},%v2,%v1{%k1}
	vmaddn213pd (%rcx){nt},%v2,%v1{%k1}
	vmaddn213pd (%rcx),%v2,%v1{%k1}
	vmaddn213pd (%rcx){8to8},%v2,%v1{%k1}
	vmaddn213pd (%rcx){1to8},%v2,%v1{%k1}
	vmaddn213pd (%rcx){4to8},%v2,%v1{%k1}
	vmaddn231pd %v3,%v2,%v1{%k1}
	vmaddn231pd %v3{cdab},%v2,%v1{%k1}
	vmaddn231pd %v3{badc},%v2,%v1{%k1}
	vmaddn231pd %v3{aaaa},%v2,%v1{%k1}
	vmaddn231pd %v3{bbbb},%v2,%v1{%k1}
	vmaddn231pd %v3{cccc},%v2,%v1{%k1}
	vmaddn231pd %v3{dddd},%v2,%v1{%k1}
	vmaddn231pd %v3{dacb},%v2,%v1{%k1}
	vmaddn231pd (%rcx){nt},%v2,%v1{%k1}
	vmaddn231pd (%rcx),%v2,%v1{%k1}
	vmaddn231pd (%rcx){8to8},%v2,%v1{%k1}
	vmaddn231pd (%rcx){1to8},%v2,%v1{%k1}
	vmaddn231pd (%rcx){4to8},%v2,%v1{%k1}
	vmaxpd %v3,%v2,%v1{%k1}
	vmaxpd %v3{cdab},%v2,%v1{%k1}
	vmaxpd %v3{badc},%v2,%v1{%k1}
	vmaxpd %v3{aaaa},%v2,%v1{%k1}
	vmaxpd %v3{bbbb},%v2,%v1{%k1}
	vmaxpd %v3{cccc},%v2,%v1{%k1}
	vmaxpd %v3{dddd},%v2,%v1{%k1}
	vmaxpd %v3{dacb},%v2,%v1{%k1}
	vmaxpd (%rcx){nt},%v2,%v1{%k1}
	vmaxpd (%rcx),%v2,%v1{%k1}
	vmaxpd (%rcx){8to8},%v2,%v1{%k1}
	vmaxpd (%rcx){1to8},%v2,%v1{%k1}
	vmaxpd (%rcx){4to8},%v2,%v1{%k1}
	vminpd %v3,%v2,%v1{%k1}
	vminpd %v3{cdab},%v2,%v1{%k1}
	vminpd %v3{badc},%v2,%v1{%k1}
	vminpd %v3{aaaa},%v2,%v1{%k1}
	vminpd %v3{bbbb},%v2,%v1{%k1}
	vminpd %v3{cccc},%v2,%v1{%k1}
	vminpd %v3{dddd},%v2,%v1{%k1}
	vminpd %v3{dacb},%v2,%v1{%k1}
	vminpd (%rcx){nt},%v2,%v1{%k1}
	vminpd (%rcx),%v2,%v1{%k1}
	vminpd (%rcx){8to8},%v2,%v1{%k1}
	vminpd (%rcx){1to8},%v2,%v1{%k1}
	vminpd (%rcx){4to8},%v2,%v1{%k1}
	vmsub132pd %v3,%v2,%v1{%k1}
	vmsub132pd %v3{cdab},%v2,%v1{%k1}
	vmsub132pd %v3{badc},%v2,%v1{%k1}
	vmsub132pd %v3{aaaa},%v2,%v1{%k1}
	vmsub132pd %v3{bbbb},%v2,%v1{%k1}
	vmsub132pd %v3{cccc},%v2,%v1{%k1}
	vmsub132pd %v3{dddd},%v2,%v1{%k1}
	vmsub132pd %v3{dacb},%v2,%v1{%k1}
	vmsub132pd (%rcx){nt},%v2,%v1{%k1}
	vmsub132pd (%rcx),%v2,%v1{%k1}
	vmsub132pd (%rcx){8to8},%v2,%v1{%k1}
	vmsub132pd (%rcx){1to8},%v2,%v1{%k1}
	vmsub132pd (%rcx){4to8},%v2,%v1{%k1}
	vmsub213pd %v3,%v2,%v1{%k1}
	vmsub213pd %v3{cdab},%v2,%v1{%k1}
	vmsub213pd %v3{badc},%v2,%v1{%k1}
	vmsub213pd %v3{aaaa},%v2,%v1{%k1}
	vmsub213pd %v3{bbbb},%v2,%v1{%k1}
	vmsub213pd %v3{cccc},%v2,%v1{%k1}
	vmsub213pd %v3{dddd},%v2,%v1{%k1}
	vmsub213pd %v3{dacb},%v2,%v1{%k1}
	vmsub213pd (%rcx){nt},%v2,%v1{%k1}
	vmsub213pd (%rcx),%v2,%v1{%k1}
	vmsub213pd (%rcx){8to8},%v2,%v1{%k1}
	vmsub213pd (%rcx){1to8},%v2,%v1{%k1}
	vmsub213pd (%rcx){4to8},%v2,%v1{%k1}
	vmsub231pd %v3,%v2,%v1{%k1}
	vmsub231pd %v3{cdab},%v2,%v1{%k1}
	vmsub231pd %v3{badc},%v2,%v1{%k1}
	vmsub231pd %v3{aaaa},%v2,%v1{%k1}
	vmsub231pd %v3{bbbb},%v2,%v1{%k1}
	vmsub231pd %v3{cccc},%v2,%v1{%k1}
	vmsub231pd %v3{dddd},%v2,%v1{%k1}
	vmsub231pd %v3{dacb},%v2,%v1{%k1}
	vmsub231pd (%rcx){nt},%v2,%v1{%k1}
	vmsub231pd (%rcx),%v2,%v1{%k1}
	vmsub231pd (%rcx){8to8},%v2,%v1{%k1}
	vmsub231pd (%rcx){1to8},%v2,%v1{%k1}
	vmsub231pd (%rcx){4to8},%v2,%v1{%k1}
	vmsubr132pd %v3,%v2,%v1{%k1}
	vmsubr132pd %v3{cdab},%v2,%v1{%k1}
	vmsubr132pd %v3{badc},%v2,%v1{%k1}
	vmsubr132pd %v3{aaaa},%v2,%v1{%k1}
	vmsubr132pd %v3{bbbb},%v2,%v1{%k1}
	vmsubr132pd %v3{cccc},%v2,%v1{%k1}
	vmsubr132pd %v3{dddd},%v2,%v1{%k1}
	vmsubr132pd %v3{dacb},%v2,%v1{%k1}
	vmsubr132pd (%rcx){nt},%v2,%v1{%k1}
	vmsubr132pd (%rcx),%v2,%v1{%k1}
	vmsubr132pd (%rcx){8to8},%v2,%v1{%k1}
	vmsubr132pd (%rcx){1to8},%v2,%v1{%k1}
	vmsubr132pd (%rcx){4to8},%v2,%v1{%k1}
	vmsubr213pd %v3,%v2,%v1{%k1}
	vmsubr213pd %v3{cdab},%v2,%v1{%k1}
	vmsubr213pd %v3{badc},%v2,%v1{%k1}
	vmsubr213pd %v3{aaaa},%v2,%v1{%k1}
	vmsubr213pd %v3{bbbb},%v2,%v1{%k1}
	vmsubr213pd %v3{cccc},%v2,%v1{%k1}
	vmsubr213pd %v3{dddd},%v2,%v1{%k1}
	vmsubr213pd %v3{dacb},%v2,%v1{%k1}
	vmsubr213pd (%rcx){nt},%v2,%v1{%k1}
	vmsubr213pd (%rcx),%v2,%v1{%k1}
	vmsubr213pd (%rcx){8to8},%v2,%v1{%k1}
	vmsubr213pd (%rcx){1to8},%v2,%v1{%k1}
	vmsubr213pd (%rcx){4to8},%v2,%v1{%k1}
	vmsubr231pd %v3,%v2,%v1{%k1}
	vmsubr231pd %v3{cdab},%v2,%v1{%k1}
	vmsubr231pd %v3{badc},%v2,%v1{%k1}
	vmsubr231pd %v3{aaaa},%v2,%v1{%k1}
	vmsubr231pd %v3{bbbb},%v2,%v1{%k1}
	vmsubr231pd %v3{cccc},%v2,%v1{%k1}
	vmsubr231pd %v3{dddd},%v2,%v1{%k1}
	vmsubr231pd %v3{dacb},%v2,%v1{%k1}
	vmsubr231pd (%rcx){nt},%v2,%v1{%k1}
	vmsubr231pd (%rcx),%v2,%v1{%k1}
	vmsubr231pd (%rcx){8to8},%v2,%v1{%k1}
	vmsubr231pd (%rcx){1to8},%v2,%v1{%k1}
	vmsubr231pd (%rcx){4to8},%v2,%v1{%k1}
	vmsubr23c1pd %v3,%v2,%v1{%k1}
	vmsubr23c1pd %v3{cdab},%v2,%v1{%k1}
	vmsubr23c1pd %v3{badc},%v2,%v1{%k1}
	vmsubr23c1pd %v3{aaaa},%v2,%v1{%k1}
	vmsubr23c1pd %v3{bbbb},%v2,%v1{%k1}
	vmsubr23c1pd %v3{cccc},%v2,%v1{%k1}
	vmsubr23c1pd %v3{dddd},%v2,%v1{%k1}
	vmsubr23c1pd %v3{dacb},%v2,%v1{%k1}
	vmsubr23c1pd (%rcx){nt},%v2,%v1{%k1}
	vmsubr23c1pd (%rcx),%v2,%v1{%k1}
	vmsubr23c1pd (%rcx){8to8},%v2,%v1{%k1}
	vmsubr23c1pd (%rcx){1to8},%v2,%v1{%k1}
	vmsubr23c1pd (%rcx){4to8},%v2,%v1{%k1}
	vmulpd %v3,%v2,%v1{%k1}
	vmulpd %v3{cdab},%v2,%v1{%k1}
	vmulpd %v3{badc},%v2,%v1{%k1}
	vmulpd %v3{aaaa},%v2,%v1{%k1}
	vmulpd %v3{bbbb},%v2,%v1{%k1}
	vmulpd %v3{cccc},%v2,%v1{%k1}
	vmulpd %v3{dddd},%v2,%v1{%k1}
	vmulpd %v3{dacb},%v2,%v1{%k1}
	vmulpd (%rcx){nt},%v2,%v1{%k1}
	vmulpd (%rcx),%v2,%v1{%k1}
	vmulpd (%rcx){8to8},%v2,%v1{%k1}
	vmulpd (%rcx){1to8},%v2,%v1{%k1}
	vmulpd (%rcx){4to8},%v2,%v1{%k1}
	vsubpd %v3,%v2,%v1{%k1}
	vsubpd %v3{cdab},%v2,%v1{%k1}
	vsubpd %v3{badc},%v2,%v1{%k1}
	vsubpd %v3{aaaa},%v2,%v1{%k1}
	vsubpd %v3{bbbb},%v2,%v1{%k1}
	vsubpd %v3{cccc},%v2,%v1{%k1}
	vsubpd %v3{dddd},%v2,%v1{%k1}
	vsubpd %v3{dacb},%v2,%v1{%k1}
	vsubpd (%rcx){nt},%v2,%v1{%k1}
	vsubpd (%rcx),%v2,%v1{%k1}
	vsubpd (%rcx){8to8},%v2,%v1{%k1}
	vsubpd (%rcx){1to8},%v2,%v1{%k1}
	vsubpd (%rcx){4to8},%v2,%v1{%k1}
	vsubrpd %v3,%v2,%v1{%k1}
	vsubrpd %v3{cdab},%v2,%v1{%k1}
	vsubrpd %v3{badc},%v2,%v1{%k1}
	vsubrpd %v3{aaaa},%v2,%v1{%k1}
	vsubrpd %v3{bbbb},%v2,%v1{%k1}
	vsubrpd %v3{cccc},%v2,%v1{%k1}
	vsubrpd %v3{dddd},%v2,%v1{%k1}
	vsubrpd %v3{dacb},%v2,%v1{%k1}
	vsubrpd (%rcx){nt},%v2,%v1{%k1}
	vsubrpd (%rcx),%v2,%v1{%k1}
	vsubrpd (%rcx){8to8},%v2,%v1{%k1}
	vsubrpd (%rcx){1to8},%v2,%v1{%k1}
	vsubrpd (%rcx){4to8},%v2,%v1{%k1}

# Tests for op v1{k1}, v2, si(v3/mt)
	vaddpi %v3,%v2,%v1{%k1}
	vaddpi %v3{cdab},%v2,%v1{%k1}
	vaddpi %v3{badc},%v2,%v1{%k1}
	vaddpi %v3{aaaa},%v2,%v1{%k1}
	vaddpi %v3{bbbb},%v2,%v1{%k1}
	vaddpi %v3{cccc},%v2,%v1{%k1}
	vaddpi %v3{dddd},%v2,%v1{%k1}
	vaddpi %v3{dacb},%v2,%v1{%k1}
	vaddpi (%rcx){nt},%v2,%v1{%k1}
	vaddpi (%rcx),%v2,%v1{%k1}
	vaddpi (%rcx){16to16},%v2,%v1{%k1}
	vaddpi (%rcx){1to16},%v2,%v1{%k1}
	vaddpi (%rcx){4to16},%v2,%v1{%k1}
	vaddpi (%rcx){uint8},%v2,%v1{%k1}
	vaddpi (%rcx){sint8},%v2,%v1{%k1}
	vaddpi (%rcx){uint16},%v2,%v1{%k1}
	vaddpi (%rcx){sint16},%v2,%v1{%k1}
	vaddsetspi %v3,%v2,%v1{%k1}
	vaddsetspi %v3{cdab},%v2,%v1{%k1}
	vaddsetspi %v3{badc},%v2,%v1{%k1}
	vaddsetspi %v3{aaaa},%v2,%v1{%k1}
	vaddsetspi %v3{bbbb},%v2,%v1{%k1}
	vaddsetspi %v3{cccc},%v2,%v1{%k1}
	vaddsetspi %v3{dddd},%v2,%v1{%k1}
	vaddsetspi %v3{dacb},%v2,%v1{%k1}
	vaddsetspi (%rcx){nt},%v2,%v1{%k1}
	vaddsetspi (%rcx),%v2,%v1{%k1}
	vaddsetspi (%rcx){16to16},%v2,%v1{%k1}
	vaddsetspi (%rcx){1to16},%v2,%v1{%k1}
	vaddsetspi (%rcx){4to16},%v2,%v1{%k1}
	vaddsetspi (%rcx){uint8},%v2,%v1{%k1}
	vaddsetspi (%rcx){sint8},%v2,%v1{%k1}
	vaddsetspi (%rcx){uint16},%v2,%v1{%k1}
	vaddsetspi (%rcx){sint16},%v2,%v1{%k1}
	vandnpi %v3,%v2,%v1{%k1}
	vandnpi %v3{cdab},%v2,%v1{%k1}
	vandnpi %v3{badc},%v2,%v1{%k1}
	vandnpi %v3{aaaa},%v2,%v1{%k1}
	vandnpi %v3{bbbb},%v2,%v1{%k1}
	vandnpi %v3{cccc},%v2,%v1{%k1}
	vandnpi %v3{dddd},%v2,%v1{%k1}
	vandnpi %v3{dacb},%v2,%v1{%k1}
	vandnpi (%rcx){nt},%v2,%v1{%k1}
	vandnpi (%rcx),%v2,%v1{%k1}
	vandnpi (%rcx){16to16},%v2,%v1{%k1}
	vandnpi (%rcx){1to16},%v2,%v1{%k1}
	vandnpi (%rcx){4to16},%v2,%v1{%k1}
	vandnpi (%rcx){uint8},%v2,%v1{%k1}
	vandnpi (%rcx){sint8},%v2,%v1{%k1}
	vandnpi (%rcx){uint16},%v2,%v1{%k1}
	vandnpi (%rcx){sint16},%v2,%v1{%k1}
	vandpi %v3,%v2,%v1{%k1}
	vandpi %v3{cdab},%v2,%v1{%k1}
	vandpi %v3{badc},%v2,%v1{%k1}
	vandpi %v3{aaaa},%v2,%v1{%k1}
	vandpi %v3{bbbb},%v2,%v1{%k1}
	vandpi %v3{cccc},%v2,%v1{%k1}
	vandpi %v3{dddd},%v2,%v1{%k1}
	vandpi %v3{dacb},%v2,%v1{%k1}
	vandpi (%rcx){nt},%v2,%v1{%k1}
	vandpi (%rcx),%v2,%v1{%k1}
	vandpi (%rcx){16to16},%v2,%v1{%k1}
	vandpi (%rcx){1to16},%v2,%v1{%k1}
	vandpi (%rcx){4to16},%v2,%v1{%k1}
	vandpi (%rcx){uint8},%v2,%v1{%k1}
	vandpi (%rcx){sint8},%v2,%v1{%k1}
	vandpi (%rcx){uint16},%v2,%v1{%k1}
	vandpi (%rcx){sint16},%v2,%v1{%k1}
	vbitinterleave11pi %v3,%v2,%v1{%k1}
	vbitinterleave11pi %v3{cdab},%v2,%v1{%k1}
	vbitinterleave11pi %v3{badc},%v2,%v1{%k1}
	vbitinterleave11pi %v3{aaaa},%v2,%v1{%k1}
	vbitinterleave11pi %v3{bbbb},%v2,%v1{%k1}
	vbitinterleave11pi %v3{cccc},%v2,%v1{%k1}
	vbitinterleave11pi %v3{dddd},%v2,%v1{%k1}
	vbitinterleave11pi %v3{dacb},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){nt},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx),%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){16to16},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){1to16},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){4to16},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){uint8},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){sint8},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){uint16},%v2,%v1{%k1}
	vbitinterleave11pi (%rcx){sint16},%v2,%v1{%k1}
	vbitinterleave21pi %v3,%v2,%v1{%k1}
	vbitinterleave21pi %v3{cdab},%v2,%v1{%k1}
	vbitinterleave21pi %v3{badc},%v2,%v1{%k1}
	vbitinterleave21pi %v3{aaaa},%v2,%v1{%k1}
	vbitinterleave21pi %v3{bbbb},%v2,%v1{%k1}
	vbitinterleave21pi %v3{cccc},%v2,%v1{%k1}
	vbitinterleave21pi %v3{dddd},%v2,%v1{%k1}
	vbitinterleave21pi %v3{dacb},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){nt},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx),%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){16to16},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){1to16},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){4to16},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){uint8},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){sint8},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){uint16},%v2,%v1{%k1}
	vbitinterleave21pi (%rcx){sint16},%v2,%v1{%k1}
	vclampzpi %v3,%v2,%v1{%k1}
	vclampzpi %v3{cdab},%v2,%v1{%k1}
	vclampzpi %v3{badc},%v2,%v1{%k1}
	vclampzpi %v3{aaaa},%v2,%v1{%k1}
	vclampzpi %v3{bbbb},%v2,%v1{%k1}
	vclampzpi %v3{cccc},%v2,%v1{%k1}
	vclampzpi %v3{dddd},%v2,%v1{%k1}
	vclampzpi %v3{dacb},%v2,%v1{%k1}
	vclampzpi (%rcx){nt},%v2,%v1{%k1}
	vclampzpi (%rcx),%v2,%v1{%k1}
	vclampzpi (%rcx){16to16},%v2,%v1{%k1}
	vclampzpi (%rcx){1to16},%v2,%v1{%k1}
	vclampzpi (%rcx){4to16},%v2,%v1{%k1}
	vclampzpi (%rcx){uint8},%v2,%v1{%k1}
	vclampzpi (%rcx){sint8},%v2,%v1{%k1}
	vclampzpi (%rcx){uint16},%v2,%v1{%k1}
	vclampzpi (%rcx){sint16},%v2,%v1{%k1}
	vmadd231pi %v3,%v2,%v1{%k1}
	vmadd231pi %v3{cdab},%v2,%v1{%k1}
	vmadd231pi %v3{badc},%v2,%v1{%k1}
	vmadd231pi %v3{aaaa},%v2,%v1{%k1}
	vmadd231pi %v3{bbbb},%v2,%v1{%k1}
	vmadd231pi %v3{cccc},%v2,%v1{%k1}
	vmadd231pi %v3{dddd},%v2,%v1{%k1}
	vmadd231pi %v3{dacb},%v2,%v1{%k1}
	vmadd231pi (%rcx){nt},%v2,%v1{%k1}
	vmadd231pi (%rcx),%v2,%v1{%k1}
	vmadd231pi (%rcx){16to16},%v2,%v1{%k1}
	vmadd231pi (%rcx){1to16},%v2,%v1{%k1}
	vmadd231pi (%rcx){4to16},%v2,%v1{%k1}
	vmadd231pi (%rcx){uint8},%v2,%v1{%k1}
	vmadd231pi (%rcx){sint8},%v2,%v1{%k1}
	vmadd231pi (%rcx){uint16},%v2,%v1{%k1}
	vmadd231pi (%rcx){sint16},%v2,%v1{%k1}
	vmadd233pi %v3,%v2,%v1{%k1}
	vmadd233pi %v3{cdab},%v2,%v1{%k1}
	vmadd233pi %v3{badc},%v2,%v1{%k1}
	vmadd233pi %v3{aaaa},%v2,%v1{%k1}
	vmadd233pi %v3{bbbb},%v2,%v1{%k1}
	vmadd233pi %v3{cccc},%v2,%v1{%k1}
	vmadd233pi %v3{dddd},%v2,%v1{%k1}
	vmadd233pi %v3{dacb},%v2,%v1{%k1}
	vmadd233pi (%rcx){nt},%v2,%v1{%k1}
	vmadd233pi (%rcx),%v2,%v1{%k1}
	vmadd233pi (%rcx){16to16},%v2,%v1{%k1}
	vmadd233pi (%rcx){1to16},%v2,%v1{%k1}
	vmadd233pi (%rcx){4to16},%v2,%v1{%k1}
	vmadd233pi (%rcx){uint8},%v2,%v1{%k1}
	vmadd233pi (%rcx){sint8},%v2,%v1{%k1}
	vmadd233pi (%rcx){uint16},%v2,%v1{%k1}
	vmadd233pi (%rcx){sint16},%v2,%v1{%k1}
	vmaxpi %v3,%v2,%v1{%k1}
	vmaxpi %v3{cdab},%v2,%v1{%k1}
	vmaxpi %v3{badc},%v2,%v1{%k1}
	vmaxpi %v3{aaaa},%v2,%v1{%k1}
	vmaxpi %v3{bbbb},%v2,%v1{%k1}
	vmaxpi %v3{cccc},%v2,%v1{%k1}
	vmaxpi %v3{dddd},%v2,%v1{%k1}
	vmaxpi %v3{dacb},%v2,%v1{%k1}
	vmaxpi (%rcx){nt},%v2,%v1{%k1}
	vmaxpi (%rcx),%v2,%v1{%k1}
	vmaxpi (%rcx){16to16},%v2,%v1{%k1}
	vmaxpi (%rcx){1to16},%v2,%v1{%k1}
	vmaxpi (%rcx){4to16},%v2,%v1{%k1}
	vmaxpi (%rcx){uint8},%v2,%v1{%k1}
	vmaxpi (%rcx){sint8},%v2,%v1{%k1}
	vmaxpi (%rcx){uint16},%v2,%v1{%k1}
	vmaxpi (%rcx){sint16},%v2,%v1{%k1}
	vmaxpu %v3,%v2,%v1{%k1}
	vmaxpu %v3{cdab},%v2,%v1{%k1}
	vmaxpu %v3{badc},%v2,%v1{%k1}
	vmaxpu %v3{aaaa},%v2,%v1{%k1}
	vmaxpu %v3{bbbb},%v2,%v1{%k1}
	vmaxpu %v3{cccc},%v2,%v1{%k1}
	vmaxpu %v3{dddd},%v2,%v1{%k1}
	vmaxpu %v3{dacb},%v2,%v1{%k1}
	vmaxpu (%rcx){nt},%v2,%v1{%k1}
	vmaxpu (%rcx),%v2,%v1{%k1}
	vmaxpu (%rcx){16to16},%v2,%v1{%k1}
	vmaxpu (%rcx){1to16},%v2,%v1{%k1}
	vmaxpu (%rcx){4to16},%v2,%v1{%k1}
	vmaxpu (%rcx){uint8},%v2,%v1{%k1}
	vmaxpu (%rcx){sint8},%v2,%v1{%k1}
	vmaxpu (%rcx){uint16},%v2,%v1{%k1}
	vmaxpu (%rcx){sint16},%v2,%v1{%k1}
	vminpi %v3,%v2,%v1{%k1}
	vminpi %v3{cdab},%v2,%v1{%k1}
	vminpi %v3{badc},%v2,%v1{%k1}
	vminpi %v3{aaaa},%v2,%v1{%k1}
	vminpi %v3{bbbb},%v2,%v1{%k1}
	vminpi %v3{cccc},%v2,%v1{%k1}
	vminpi %v3{dddd},%v2,%v1{%k1}
	vminpi %v3{dacb},%v2,%v1{%k1}
	vminpi (%rcx){nt},%v2,%v1{%k1}
	vminpi (%rcx),%v2,%v1{%k1}
	vminpi (%rcx){16to16},%v2,%v1{%k1}
	vminpi (%rcx){1to16},%v2,%v1{%k1}
	vminpi (%rcx){4to16},%v2,%v1{%k1}
	vminpi (%rcx){uint8},%v2,%v1{%k1}
	vminpi (%rcx){sint8},%v2,%v1{%k1}
	vminpi (%rcx){uint16},%v2,%v1{%k1}
	vminpi (%rcx){sint16},%v2,%v1{%k1}
	vminpu %v3,%v2,%v1{%k1}
	vminpu %v3{cdab},%v2,%v1{%k1}
	vminpu %v3{badc},%v2,%v1{%k1}
	vminpu %v3{aaaa},%v2,%v1{%k1}
	vminpu %v3{bbbb},%v2,%v1{%k1}
	vminpu %v3{cccc},%v2,%v1{%k1}
	vminpu %v3{dddd},%v2,%v1{%k1}
	vminpu %v3{dacb},%v2,%v1{%k1}
	vminpu (%rcx){nt},%v2,%v1{%k1}
	vminpu (%rcx),%v2,%v1{%k1}
	vminpu (%rcx){16to16},%v2,%v1{%k1}
	vminpu (%rcx){1to16},%v2,%v1{%k1}
	vminpu (%rcx){4to16},%v2,%v1{%k1}
	vminpu (%rcx){uint8},%v2,%v1{%k1}
	vminpu (%rcx){sint8},%v2,%v1{%k1}
	vminpu (%rcx){uint16},%v2,%v1{%k1}
	vminpu (%rcx){sint16},%v2,%v1{%k1}
	vmulhpi %v3,%v2,%v1{%k1}
	vmulhpi %v3{cdab},%v2,%v1{%k1}
	vmulhpi %v3{badc},%v2,%v1{%k1}
	vmulhpi %v3{aaaa},%v2,%v1{%k1}
	vmulhpi %v3{bbbb},%v2,%v1{%k1}
	vmulhpi %v3{cccc},%v2,%v1{%k1}
	vmulhpi %v3{dddd},%v2,%v1{%k1}
	vmulhpi %v3{dacb},%v2,%v1{%k1}
	vmulhpi (%rcx){nt},%v2,%v1{%k1}
	vmulhpi (%rcx),%v2,%v1{%k1}
	vmulhpi (%rcx){16to16},%v2,%v1{%k1}
	vmulhpi (%rcx){1to16},%v2,%v1{%k1}
	vmulhpi (%rcx){4to16},%v2,%v1{%k1}
	vmulhpi (%rcx){uint8},%v2,%v1{%k1}
	vmulhpi (%rcx){sint8},%v2,%v1{%k1}
	vmulhpi (%rcx){uint16},%v2,%v1{%k1}
	vmulhpi (%rcx){sint16},%v2,%v1{%k1}
	vmulhpu %v3,%v2,%v1{%k1}
	vmulhpu %v3{cdab},%v2,%v1{%k1}
	vmulhpu %v3{badc},%v2,%v1{%k1}
	vmulhpu %v3{aaaa},%v2,%v1{%k1}
	vmulhpu %v3{bbbb},%v2,%v1{%k1}
	vmulhpu %v3{cccc},%v2,%v1{%k1}
	vmulhpu %v3{dddd},%v2,%v1{%k1}
	vmulhpu %v3{dacb},%v2,%v1{%k1}
	vmulhpu (%rcx){nt},%v2,%v1{%k1}
	vmulhpu (%rcx),%v2,%v1{%k1}
	vmulhpu (%rcx){16to16},%v2,%v1{%k1}
	vmulhpu (%rcx){1to16},%v2,%v1{%k1}
	vmulhpu (%rcx){4to16},%v2,%v1{%k1}
	vmulhpu (%rcx){uint8},%v2,%v1{%k1}
	vmulhpu (%rcx){sint8},%v2,%v1{%k1}
	vmulhpu (%rcx){uint16},%v2,%v1{%k1}
	vmulhpu (%rcx){sint16},%v2,%v1{%k1}
	vmullpi %v3,%v2,%v1{%k1}
	vmullpi %v3{cdab},%v2,%v1{%k1}
	vmullpi %v3{badc},%v2,%v1{%k1}
	vmullpi %v3{aaaa},%v2,%v1{%k1}
	vmullpi %v3{bbbb},%v2,%v1{%k1}
	vmullpi %v3{cccc},%v2,%v1{%k1}
	vmullpi %v3{dddd},%v2,%v1{%k1}
	vmullpi %v3{dacb},%v2,%v1{%k1}
	vmullpi (%rcx){nt},%v2,%v1{%k1}
	vmullpi (%rcx),%v2,%v1{%k1}
	vmullpi (%rcx){16to16},%v2,%v1{%k1}
	vmullpi (%rcx){1to16},%v2,%v1{%k1}
	vmullpi (%rcx){4to16},%v2,%v1{%k1}
	vmullpi (%rcx){uint8},%v2,%v1{%k1}
	vmullpi (%rcx){sint8},%v2,%v1{%k1}
	vmullpi (%rcx){uint16},%v2,%v1{%k1}
	vmullpi (%rcx){sint16},%v2,%v1{%k1}
	vorpi %v3,%v2,%v1{%k1}
	vorpi %v3{cdab},%v2,%v1{%k1}
	vorpi %v3{badc},%v2,%v1{%k1}
	vorpi %v3{aaaa},%v2,%v1{%k1}
	vorpi %v3{bbbb},%v2,%v1{%k1}
	vorpi %v3{cccc},%v2,%v1{%k1}
	vorpi %v3{dddd},%v2,%v1{%k1}
	vorpi %v3{dacb},%v2,%v1{%k1}
	vorpi (%rcx){nt},%v2,%v1{%k1}
	vorpi (%rcx),%v2,%v1{%k1}
	vorpi (%rcx){16to16},%v2,%v1{%k1}
	vorpi (%rcx){1to16},%v2,%v1{%k1}
	vorpi (%rcx){4to16},%v2,%v1{%k1}
	vorpi (%rcx){uint8},%v2,%v1{%k1}
	vorpi (%rcx){sint8},%v2,%v1{%k1}
	vorpi (%rcx){uint16},%v2,%v1{%k1}
	vorpi (%rcx){sint16},%v2,%v1{%k1}
	vscaleps %v3,%v2,%v1{%k1}
	vscaleps %v3{cdab},%v2,%v1{%k1}
	vscaleps %v3{badc},%v2,%v1{%k1}
	vscaleps %v3{aaaa},%v2,%v1{%k1}
	vscaleps %v3{bbbb},%v2,%v1{%k1}
	vscaleps %v3{cccc},%v2,%v1{%k1}
	vscaleps %v3{dddd},%v2,%v1{%k1}
	vscaleps %v3{dacb},%v2,%v1{%k1}
	vscaleps (%rcx){nt},%v2,%v1{%k1}
	vscaleps (%rcx),%v2,%v1{%k1}
	vscaleps (%rcx){16to16},%v2,%v1{%k1}
	vscaleps (%rcx){1to16},%v2,%v1{%k1}
	vscaleps (%rcx){4to16},%v2,%v1{%k1}
	vscaleps (%rcx){uint8},%v2,%v1{%k1}
	vscaleps (%rcx){sint8},%v2,%v1{%k1}
	vscaleps (%rcx){uint16},%v2,%v1{%k1}
	vscaleps (%rcx){sint16},%v2,%v1{%k1}
	vsllpi %v3,%v2,%v1{%k1}
	vsllpi %v3{cdab},%v2,%v1{%k1}
	vsllpi %v3{badc},%v2,%v1{%k1}
	vsllpi %v3{aaaa},%v2,%v1{%k1}
	vsllpi %v3{bbbb},%v2,%v1{%k1}
	vsllpi %v3{cccc},%v2,%v1{%k1}
	vsllpi %v3{dddd},%v2,%v1{%k1}
	vsllpi %v3{dacb},%v2,%v1{%k1}
	vsllpi (%rcx){nt},%v2,%v1{%k1}
	vsllpi (%rcx),%v2,%v1{%k1}
	vsllpi (%rcx){16to16},%v2,%v1{%k1}
	vsllpi (%rcx){1to16},%v2,%v1{%k1}
	vsllpi (%rcx){4to16},%v2,%v1{%k1}
	vsllpi (%rcx){uint8},%v2,%v1{%k1}
	vsllpi (%rcx){sint8},%v2,%v1{%k1}
	vsllpi (%rcx){uint16},%v2,%v1{%k1}
	vsllpi (%rcx){sint16},%v2,%v1{%k1}
	vsrapi %v3,%v2,%v1{%k1}
	vsrapi %v3{cdab},%v2,%v1{%k1}
	vsrapi %v3{badc},%v2,%v1{%k1}
	vsrapi %v3{aaaa},%v2,%v1{%k1}
	vsrapi %v3{bbbb},%v2,%v1{%k1}
	vsrapi %v3{cccc},%v2,%v1{%k1}
	vsrapi %v3{dddd},%v2,%v1{%k1}
	vsrapi %v3{dacb},%v2,%v1{%k1}
	vsrapi (%rcx){nt},%v2,%v1{%k1}
	vsrapi (%rcx),%v2,%v1{%k1}
	vsrapi (%rcx){16to16},%v2,%v1{%k1}
	vsrapi (%rcx){1to16},%v2,%v1{%k1}
	vsrapi (%rcx){4to16},%v2,%v1{%k1}
	vsrapi (%rcx){uint8},%v2,%v1{%k1}
	vsrapi (%rcx){sint8},%v2,%v1{%k1}
	vsrapi (%rcx){uint16},%v2,%v1{%k1}
	vsrapi (%rcx){sint16},%v2,%v1{%k1}
	vsrlpi %v3,%v2,%v1{%k1}
	vsrlpi %v3{cdab},%v2,%v1{%k1}
	vsrlpi %v3{badc},%v2,%v1{%k1}
	vsrlpi %v3{aaaa},%v2,%v1{%k1}
	vsrlpi %v3{bbbb},%v2,%v1{%k1}
	vsrlpi %v3{cccc},%v2,%v1{%k1}
	vsrlpi %v3{dddd},%v2,%v1{%k1}
	vsrlpi %v3{dacb},%v2,%v1{%k1}
	vsrlpi (%rcx){nt},%v2,%v1{%k1}
	vsrlpi (%rcx),%v2,%v1{%k1}
	vsrlpi (%rcx){16to16},%v2,%v1{%k1}
	vsrlpi (%rcx){1to16},%v2,%v1{%k1}
	vsrlpi (%rcx){4to16},%v2,%v1{%k1}
	vsrlpi (%rcx){uint8},%v2,%v1{%k1}
	vsrlpi (%rcx){sint8},%v2,%v1{%k1}
	vsrlpi (%rcx){uint16},%v2,%v1{%k1}
	vsrlpi (%rcx){sint16},%v2,%v1{%k1}
	vsubpi %v3,%v2,%v1{%k1}
	vsubpi %v3{cdab},%v2,%v1{%k1}
	vsubpi %v3{badc},%v2,%v1{%k1}
	vsubpi %v3{aaaa},%v2,%v1{%k1}
	vsubpi %v3{bbbb},%v2,%v1{%k1}
	vsubpi %v3{cccc},%v2,%v1{%k1}
	vsubpi %v3{dddd},%v2,%v1{%k1}
	vsubpi %v3{dacb},%v2,%v1{%k1}
	vsubpi (%rcx){nt},%v2,%v1{%k1}
	vsubpi (%rcx),%v2,%v1{%k1}
	vsubpi (%rcx){16to16},%v2,%v1{%k1}
	vsubpi (%rcx){1to16},%v2,%v1{%k1}
	vsubpi (%rcx){4to16},%v2,%v1{%k1}
	vsubpi (%rcx){uint8},%v2,%v1{%k1}
	vsubpi (%rcx){sint8},%v2,%v1{%k1}
	vsubpi (%rcx){uint16},%v2,%v1{%k1}
	vsubpi (%rcx){sint16},%v2,%v1{%k1}
	vsubrpi %v3,%v2,%v1{%k1}
	vsubrpi %v3{cdab},%v2,%v1{%k1}
	vsubrpi %v3{badc},%v2,%v1{%k1}
	vsubrpi %v3{aaaa},%v2,%v1{%k1}
	vsubrpi %v3{bbbb},%v2,%v1{%k1}
	vsubrpi %v3{cccc},%v2,%v1{%k1}
	vsubrpi %v3{dddd},%v2,%v1{%k1}
	vsubrpi %v3{dacb},%v2,%v1{%k1}
	vsubrpi (%rcx){nt},%v2,%v1{%k1}
	vsubrpi (%rcx),%v2,%v1{%k1}
	vsubrpi (%rcx){16to16},%v2,%v1{%k1}
	vsubrpi (%rcx){1to16},%v2,%v1{%k1}
	vsubrpi (%rcx){4to16},%v2,%v1{%k1}
	vsubrpi (%rcx){uint8},%v2,%v1{%k1}
	vsubrpi (%rcx){sint8},%v2,%v1{%k1}
	vsubrpi (%rcx){uint16},%v2,%v1{%k1}
	vsubrpi (%rcx){sint16},%v2,%v1{%k1}
	vxorpi %v3,%v2,%v1{%k1}
	vxorpi %v3{cdab},%v2,%v1{%k1}
	vxorpi %v3{badc},%v2,%v1{%k1}
	vxorpi %v3{aaaa},%v2,%v1{%k1}
	vxorpi %v3{bbbb},%v2,%v1{%k1}
	vxorpi %v3{cccc},%v2,%v1{%k1}
	vxorpi %v3{dddd},%v2,%v1{%k1}
	vxorpi %v3{dacb},%v2,%v1{%k1}
	vxorpi (%rcx){nt},%v2,%v1{%k1}
	vxorpi (%rcx),%v2,%v1{%k1}
	vxorpi (%rcx){16to16},%v2,%v1{%k1}
	vxorpi (%rcx){1to16},%v2,%v1{%k1}
	vxorpi (%rcx){4to16},%v2,%v1{%k1}
	vxorpi (%rcx){uint8},%v2,%v1{%k1}
	vxorpi (%rcx){sint8},%v2,%v1{%k1}
	vxorpi (%rcx){uint16},%v2,%v1{%k1}
	vxorpi (%rcx){sint16},%v2,%v1{%k1}

# Tests for op k2{k1}, v2, si(v3/mt)
	vtestpi %v3,%v2,%k1{%k2}
	vtestpi %v3{cdab},%v2,%k1{%k2}
	vtestpi %v3{badc},%v2,%k1{%k2}
	vtestpi %v3{aaaa},%v2,%k1{%k2}
	vtestpi %v3{bbbb},%v2,%k1{%k2}
	vtestpi %v3{cccc},%v2,%k1{%k2}
	vtestpi %v3{dddd},%v2,%k1{%k2}
	vtestpi %v3{dacb},%v2,%k1{%k2}
	vtestpi (%rcx){nt},%v2,%k1{%k2}
	vtestpi (%rcx),%v2,%k1{%k2}
	vtestpi (%rcx){16to16},%v2,%k1{%k2}
	vtestpi (%rcx){1to16},%v2,%k1{%k2}
	vtestpi (%rcx){4to16},%v2,%k1{%k2}
	vtestpi (%rcx){uint8},%v2,%k1{%k2}
	vtestpi (%rcx){sint8},%v2,%k1{%k2}
	vtestpi (%rcx){uint16},%v2,%k1{%k2}
	vtestpi (%rcx){sint16},%v2,%k1{%k2}

# Tests for op v1{k1}, v2, sq(v3/mt)
	vandnpq %v3,%v2,%v1{%k1}
	vandnpq %v3{cdab},%v2,%v1{%k1}
	vandnpq %v3{badc},%v2,%v1{%k1}
	vandnpq %v3{aaaa},%v2,%v1{%k1}
	vandnpq %v3{bbbb},%v2,%v1{%k1}
	vandnpq %v3{cccc},%v2,%v1{%k1}
	vandnpq %v3{dddd},%v2,%v1{%k1}
	vandnpq %v3{dacb},%v2,%v1{%k1}
	vandnpq (%rcx){nt},%v2,%v1{%k1}
	vandnpq (%rcx),%v2,%v1{%k1}
	vandnpq (%rcx){8to8},%v2,%v1{%k1}
	vandnpq (%rcx){1to8},%v2,%v1{%k1}
	vandnpq (%rcx){4to8},%v2,%v1{%k1}
	vandpq %v3,%v2,%v1{%k1}
	vandpq %v3{cdab},%v2,%v1{%k1}
	vandpq %v3{badc},%v2,%v1{%k1}
	vandpq %v3{aaaa},%v2,%v1{%k1}
	vandpq %v3{bbbb},%v2,%v1{%k1}
	vandpq %v3{cccc},%v2,%v1{%k1}
	vandpq %v3{dddd},%v2,%v1{%k1}
	vandpq %v3{dacb},%v2,%v1{%k1}
	vandpq (%rcx){nt},%v2,%v1{%k1}
	vandpq (%rcx),%v2,%v1{%k1}
	vandpq (%rcx){8to8},%v2,%v1{%k1}
	vandpq (%rcx){1to8},%v2,%v1{%k1}
	vandpq (%rcx){4to8},%v2,%v1{%k1}
	vorpq %v3,%v2,%v1{%k1}
	vorpq %v3{cdab},%v2,%v1{%k1}
	vorpq %v3{badc},%v2,%v1{%k1}
	vorpq %v3{aaaa},%v2,%v1{%k1}
	vorpq %v3{bbbb},%v2,%v1{%k1}
	vorpq %v3{cccc},%v2,%v1{%k1}
	vorpq %v3{dddd},%v2,%v1{%k1}
	vorpq %v3{dacb},%v2,%v1{%k1}
	vorpq (%rcx){nt},%v2,%v1{%k1}
	vorpq (%rcx),%v2,%v1{%k1}
	vorpq (%rcx){8to8},%v2,%v1{%k1}
	vorpq (%rcx){1to8},%v2,%v1{%k1}
	vorpq (%rcx){4to8},%v2,%v1{%k1}
	vxorpq %v3,%v2,%v1{%k1}
	vxorpq %v3{cdab},%v2,%v1{%k1}
	vxorpq %v3{badc},%v2,%v1{%k1}
	vxorpq %v3{aaaa},%v2,%v1{%k1}
	vxorpq %v3{bbbb},%v2,%v1{%k1}
	vxorpq %v3{cccc},%v2,%v1{%k1}
	vxorpq %v3{dddd},%v2,%v1{%k1}
	vxorpq %v3{dacb},%v2,%v1{%k1}
	vxorpq (%rcx){nt},%v2,%v1{%k1}
	vxorpq (%rcx),%v2,%v1{%k1}
	vxorpq (%rcx){8to8},%v2,%v1{%k1}
	vxorpq (%rcx){1to8},%v2,%v1{%k1}
	vxorpq (%rcx){4to8},%v2,%v1{%k1}

# Tests for op k2{k1}, v1, sd(v2/mt), cmppd
	vcmppd {unord},%v3,%v2,%k1{%k2}
	vcmppd {unord},%v3{cdab},%v2,%k1{%k2}
	vcmppd {unord},%v3{badc},%v2,%k1{%k2}
	vcmppd {unord},%v3{aaaa},%v2,%k1{%k2}
	vcmppd {unord},%v3{bbbb},%v2,%k1{%k2}
	vcmppd {unord},%v3{cccc},%v2,%k1{%k2}
	vcmppd {unord},%v3{dddd},%v2,%k1{%k2}
	vcmppd {unord},%v3{dacb},%v2,%k1{%k2}
	vcmppd {eq},%v3{dacb},%v2,%k1{%k2}
	vcmppd {eq},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {lt},%v3{dacb},%v2,%k1{%k2}
	vcmppd {lt},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {le},%v3{dacb},%v2,%k1{%k2}
	vcmppd {le},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {unord},%v3{dacb},%v2,%k1{%k2}
	vcmppd {unord},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {neq},%v3{dacb},%v2,%k1{%k2}
	vcmppd {neq},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {nlt},%v3{dacb},%v2,%k1{%k2}
	vcmppd {nlt},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {nle},%v3{dacb},%v2,%k1{%k2}
	vcmppd {nle},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {ord},%v3{dacb},%v2,%k1{%k2}
	vcmppd {ord},(%rcx){nt},%v2,%k1{%k2}
	vcmppd {unord},(%rcx),%v2,%k1{%k2}
	vcmppd {unord},(%rcx){8to8},%v2,%k1{%k2}
	vcmppd {unord},(%rcx){1to8},%v2,%k1{%k2}
	vcmppd {unord},(%rcx){4to8},%v2,%k1{%k2}

# Tests for op k2{k1}, v1, si(v2/mt), cmppi
	vcmppi {lt},%v3,%v2,%k1{%k2}
	vcmppi {lt},%v3{cdab},%v2,%k1{%k2}
	vcmppi {lt},%v3{badc},%v2,%k1{%k2}
	vcmppi {lt},%v3{aaaa},%v2,%k1{%k2}
	vcmppi {lt},%v3{bbbb},%v2,%k1{%k2}
	vcmppi {lt},%v3{cccc},%v2,%k1{%k2}
	vcmppi {lt},%v3{dddd},%v2,%k1{%k2}
	vcmppi {lt},%v3{dacb},%v2,%k1{%k2}
	vcmppi {eq},%v3{dacb},%v2,%k1{%k2}
	vcmppi {eq},(%rcx){nt},%v2,%k1{%k2}
	vcmppi {lt},%v3{dacb},%v2,%k1{%k2}
	vcmppi {lt},(%rcx){nt},%v2,%k1{%k2}
	vcmppi {le},%v3{dacb},%v2,%k1{%k2}
	vcmppi {le},(%rcx){nt},%v2,%k1{%k2}
	vcmppi {neq},%v3{dacb},%v2,%k1{%k2}
	vcmppi {neq},(%rcx){nt},%v2,%k1{%k2}
	vcmppi {nlt},%v3{dacb},%v2,%k1{%k2}
	vcmppi {nlt},(%rcx){nt},%v2,%k1{%k2}
	vcmppi {nle},%v3{dacb},%v2,%k1{%k2}
	vcmppi {nle},(%rcx){nt},%v2,%k1{%k2}
	vcmppi {lt},(%rcx),%v2,%k1{%k2}
	vcmppi {lt},(%rcx){16to16},%v2,%k1{%k2}
	vcmppi {lt},(%rcx){1to16},%v2,%k1{%k2}
	vcmppi {lt},(%rcx){4to16},%v2,%k1{%k2}
	vcmppi {lt},(%rcx){uint8},%v2,%k1{%k2}
	vcmppi {lt},(%rcx){sint8},%v2,%k1{%k2}
	vcmppi {lt},(%rcx){uint16},%v2,%k1{%k2}
	vcmppi {lt},(%rcx){sint16},%v2,%k1{%k2}
	vcmppu {lt},%v3,%v2,%k1{%k2}
	vcmppu {lt},%v3{cdab},%v2,%k1{%k2}
	vcmppu {lt},%v3{badc},%v2,%k1{%k2}
	vcmppu {lt},%v3{aaaa},%v2,%k1{%k2}
	vcmppu {lt},%v3{bbbb},%v2,%k1{%k2}
	vcmppu {lt},%v3{cccc},%v2,%k1{%k2}
	vcmppu {lt},%v3{dddd},%v2,%k1{%k2}
	vcmppu {lt},%v3{dacb},%v2,%k1{%k2}
	vcmppu {eq},%v3{dacb},%v2,%k1{%k2}
	vcmppu {eq},(%rcx){nt},%v2,%k1{%k2}
	vcmppu {lt},%v3{dacb},%v2,%k1{%k2}
	vcmppu {lt},(%rcx){nt},%v2,%k1{%k2}
	vcmppu {le},%v3{dacb},%v2,%k1{%k2}
	vcmppu {le},(%rcx){nt},%v2,%k1{%k2}
	vcmppu {neq},%v3{dacb},%v2,%k1{%k2}
	vcmppu {neq},(%rcx){nt},%v2,%k1{%k2}
	vcmppu {nlt},%v3{dacb},%v2,%k1{%k2}
	vcmppu {nlt},(%rcx){nt},%v2,%k1{%k2}
	vcmppu {nle},%v3{dacb},%v2,%k1{%k2}
	vcmppu {nle},(%rcx){nt},%v2,%k1{%k2}
	vcmppu {lt},(%rcx),%v2,%k1{%k2}
	vcmppu {lt},(%rcx){16to16},%v2,%k1{%k2}
	vcmppu {lt},(%rcx){1to16},%v2,%k1{%k2}
	vcmppu {lt},(%rcx){4to16},%v2,%k1{%k2}
	vcmppu {lt},(%rcx){uint8},%v2,%k1{%k2}
	vcmppu {lt},(%rcx){sint8},%v2,%k1{%k2}
	vcmppu {lt},(%rcx){uint16},%v2,%k1{%k2}
	vcmppu {lt},(%rcx){sint16},%v2,%k1{%k2}

# Tests for op k2{k1}, v1, sf(v2/mt), cmpps
	vcmpps {unord},%v3,%v2,%k1{%k2}
	vcmpps {unord},%v3{cdab},%v2,%k1{%k2}
	vcmpps {unord},%v3{badc},%v2,%k1{%k2}
	vcmpps {unord},%v3{aaaa},%v2,%k1{%k2}
	vcmpps {unord},%v3{bbbb},%v2,%k1{%k2}
	vcmpps {unord},%v3{cccc},%v2,%k1{%k2}
	vcmpps {unord},%v3{dddd},%v2,%k1{%k2}
	vcmpps {unord},%v3{dacb},%v2,%k1{%k2}
	vcmpps {eq},%v3{dacb},%v2,%k1{%k2}
	vcmpps {eq},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {lt},%v3{dacb},%v2,%k1{%k2}
	vcmpps {lt},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {le},%v3{dacb},%v2,%k1{%k2}
	vcmpps {le},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {unord},%v3{dacb},%v2,%k1{%k2}
	vcmpps {unord},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {neq},%v3{dacb},%v2,%k1{%k2}
	vcmpps {neq},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {nlt},%v3{dacb},%v2,%k1{%k2}
	vcmpps {nlt},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {nle},%v3{dacb},%v2,%k1{%k2}
	vcmpps {nle},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {ord},%v3{dacb},%v2,%k1{%k2}
	vcmpps {ord},(%rcx){nt},%v2,%k1{%k2}
	vcmpps {unord},(%rcx),%v2,%k1{%k2}
	vcmpps {unord},(%rcx){16to16},%v2,%k1{%k2}
	vcmpps {unord},(%rcx){1to16},%v2,%k1{%k2}
	vcmpps {unord},(%rcx){4to16},%v2,%k1{%k2}
	vcmpps {unord},(%rcx){uint8},%v2,%k1{%k2}
	vcmpps {unord},(%rcx){unorm8},%v2,%k1{%k2}
	vcmpps {unord},(%rcx){float16},%v2,%k1{%k2}
	vcmpps {unord},(%rcx){sint16},%v2,%k1{%k2}

# Tests for op v1{k1}, sf(v2/mt), rc1, field
	vcvtinsps2f11 $2,{rn},%v3,%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{cdab},%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{badc},%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{aaaa},%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{bbbb},%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{cccc},%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{dddd},%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{dacb},%v1{%k1}
	vcvtinsps2f11 $2,{rn},%v3{dacb},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){nt},%v1{%k1}
	vcvtinsps2f11 $2,{rz},%v3{dacb},%v1{%k1}
	vcvtinsps2f11 $2,{rz},(%rcx){nt},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx),%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){16to16},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){1to16},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){4to16},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){uint8},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){unorm8},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){float16},%v1{%k1}
	vcvtinsps2f11 $2,{rn},(%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, sf(v2/mt), field
	vcvtinsps2u10 $2,%v3,%v1{%k1}
	vcvtinsps2u10 $2,%v3{cdab},%v1{%k1}
	vcvtinsps2u10 $2,%v3{badc},%v1{%k1}
	vcvtinsps2u10 $2,%v3{aaaa},%v1{%k1}
	vcvtinsps2u10 $2,%v3{bbbb},%v1{%k1}
	vcvtinsps2u10 $2,%v3{cccc},%v1{%k1}
	vcvtinsps2u10 $2,%v3{dddd},%v1{%k1}
	vcvtinsps2u10 $2,%v3{dacb},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){nt},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx),%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){16to16},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){1to16},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){4to16},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){uint8},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){unorm8},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){float16},%v1{%k1}
	vcvtinsps2u10 $2,(%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, sd(v2/mt), rc2, half
	vcvtpd2pi $1,{rn},%v3,%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{cdab},%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{badc},%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{aaaa},%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{bbbb},%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{cccc},%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{dddd},%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{dacb},%v1{%k1}
	vcvtpd2pi $1,{rn},%v3{dacb},%v1{%k1}
	vcvtpd2pi $1,{rn},(%rcx),%v1{%k1}
	vcvtpd2pi $1,{rd},%v3{dacb},%v1{%k1}
	vcvtpd2pi $1,{rd},(%rcx),%v1{%k1}
	vcvtpd2pi $1,{ru},%v3{dacb},%v1{%k1}
	vcvtpd2pi $1,{ru},(%rcx),%v1{%k1}
	vcvtpd2pi $1,{rz},%v3{dacb},%v1{%k1}
	vcvtpd2pi $1,{rz},(%rcx),%v1{%k1}
	vcvtpd2pi $1,{rz},(%rcx){nt},%v1{%k1}
	vcvtpd2pi $1,{rn},(%rcx),%v1{%k1}
	vcvtpd2pi $1,{rn},(%rcx){8to8},%v1{%k1}
	vcvtpd2pi $1,{rn},(%rcx){1to8},%v1{%k1}
	vcvtpd2pi $1,{rn},(%rcx){4to8},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3,%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{cdab},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{badc},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{aaaa},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{bbbb},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{cccc},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{dddd},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{dacb},%v1{%k1}
	vcvtpd2ps $1,{rn},%v3{dacb},%v1{%k1}
	vcvtpd2ps $1,{rn},(%rcx),%v1{%k1}
	vcvtpd2ps $1,{rd},%v3{dacb},%v1{%k1}
	vcvtpd2ps $1,{rd},(%rcx),%v1{%k1}
	vcvtpd2ps $1,{ru},%v3{dacb},%v1{%k1}
	vcvtpd2ps $1,{ru},(%rcx),%v1{%k1}
	vcvtpd2ps $1,{rz},%v3{dacb},%v1{%k1}
	vcvtpd2ps $1,{rz},(%rcx),%v1{%k1}
	vcvtpd2ps $1,{rz},(%rcx){nt},%v1{%k1}
	vcvtpd2ps $1,{rn},(%rcx),%v1{%k1}
	vcvtpd2ps $1,{rn},(%rcx){8to8},%v1{%k1}
	vcvtpd2ps $1,{rn},(%rcx){1to8},%v1{%k1}
	vcvtpd2ps $1,{rn},(%rcx){4to8},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3,%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{cdab},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{badc},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{aaaa},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{bbbb},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{cccc},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{dddd},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{dacb},%v1{%k1}
	vcvtpd2pu $1,{rn},%v3{dacb},%v1{%k1}
	vcvtpd2pu $1,{rn},(%rcx),%v1{%k1}
	vcvtpd2pu $1,{rd},%v3{dacb},%v1{%k1}
	vcvtpd2pu $1,{rd},(%rcx),%v1{%k1}
	vcvtpd2pu $1,{ru},%v3{dacb},%v1{%k1}
	vcvtpd2pu $1,{ru},(%rcx),%v1{%k1}
	vcvtpd2pu $1,{rz},%v3{dacb},%v1{%k1}
	vcvtpd2pu $1,{rz},(%rcx),%v1{%k1}
	vcvtpd2pu $1,{rz},(%rcx){nt},%v1{%k1}
	vcvtpd2pu $1,{rn},(%rcx),%v1{%k1}
	vcvtpd2pu $1,{rn},(%rcx){8to8},%v1{%k1}
	vcvtpd2pu $1,{rn},(%rcx){1to8},%v1{%k1}
	vcvtpd2pu $1,{rn},(%rcx){4to8},%v1{%k1}

# Tests for op v1{k1}, si(v2/mt), half
	vcvtpi2pd $1,%v3,%v1{%k1}
	vcvtpi2pd $1,%v3{cdab},%v1{%k1}
	vcvtpi2pd $1,%v3{badc},%v1{%k1}
	vcvtpi2pd $1,%v3{aaaa},%v1{%k1}
	vcvtpi2pd $1,%v3{bbbb},%v1{%k1}
	vcvtpi2pd $1,%v3{cccc},%v1{%k1}
	vcvtpi2pd $1,%v3{dddd},%v1{%k1}
	vcvtpi2pd $1,%v3{dacb},%v1{%k1}
	vcvtpi2pd $1,(%rcx){nt},%v1{%k1}
	vcvtpi2pd $1,(%rcx),%v1{%k1}
	vcvtpi2pd $1,(%rcx){16to16},%v1{%k1}
	vcvtpi2pd $1,(%rcx){1to16},%v1{%k1}
	vcvtpi2pd $1,(%rcx){4to16},%v1{%k1}
	vcvtpi2pd $1,(%rcx){uint8},%v1{%k1}
	vcvtpi2pd $1,(%rcx){sint8},%v1{%k1}
	vcvtpi2pd $1,(%rcx){uint16},%v1{%k1}
	vcvtpi2pd $1,(%rcx){sint16},%v1{%k1}
	vcvtpu2pd $1,%v3,%v1{%k1}
	vcvtpu2pd $1,%v3{cdab},%v1{%k1}
	vcvtpu2pd $1,%v3{badc},%v1{%k1}
	vcvtpu2pd $1,%v3{aaaa},%v1{%k1}
	vcvtpu2pd $1,%v3{bbbb},%v1{%k1}
	vcvtpu2pd $1,%v3{cccc},%v1{%k1}
	vcvtpu2pd $1,%v3{dddd},%v1{%k1}
	vcvtpu2pd $1,%v3{dacb},%v1{%k1}
	vcvtpu2pd $1,(%rcx){nt},%v1{%k1}
	vcvtpu2pd $1,(%rcx),%v1{%k1}
	vcvtpu2pd $1,(%rcx){16to16},%v1{%k1}
	vcvtpu2pd $1,(%rcx){1to16},%v1{%k1}
	vcvtpu2pd $1,(%rcx){4to16},%v1{%k1}
	vcvtpu2pd $1,(%rcx){uint8},%v1{%k1}
	vcvtpu2pd $1,(%rcx){sint8},%v1{%k1}
	vcvtpu2pd $1,(%rcx){uint16},%v1{%k1}
	vcvtpu2pd $1,(%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, si(v2/mt), expd
	vcvtpi2ps $4,%v3,%v1{%k1}
	vcvtpi2ps $4,%v3{cdab},%v1{%k1}
	vcvtpi2ps $4,%v3{badc},%v1{%k1}
	vcvtpi2ps $4,%v3{aaaa},%v1{%k1}
	vcvtpi2ps $4,%v3{bbbb},%v1{%k1}
	vcvtpi2ps $4,%v3{cccc},%v1{%k1}
	vcvtpi2ps $4,%v3{dddd},%v1{%k1}
	vcvtpi2ps $4,%v3{dacb},%v1{%k1}
	vcvtpi2ps {.0b},%v3{dacb},%v1{%k1}
	vcvtpi2ps {.4b},%v3{dacb},%v1{%k1}
	vcvtpi2ps {.5b},%v3{dacb},%v1{%k1}
	vcvtpi2ps {.8b},%v3{dacb},%v1{%k1}
	vcvtpi2ps {.16b},%v3{dacb},%v1{%k1}
	vcvtpi2ps {.24b},%v3{dacb},%v1{%k1}
	vcvtpi2ps {.31b},%v3{dacb},%v1{%k1}
	vcvtpi2ps {.32b},%v3{dacb},%v1{%k1}
	vcvtpi2ps $4,(%rcx){nt},%v1{%k1}
	vcvtpi2ps $4,(%rcx),%v1{%k1}
	vcvtpi2ps $4,(%rcx){16to16},%v1{%k1}
	vcvtpi2ps $4,(%rcx){1to16},%v1{%k1}
	vcvtpi2ps $4,(%rcx){4to16},%v1{%k1}
	vcvtpi2ps $4,(%rcx){uint8},%v1{%k1}
	vcvtpi2ps $4,(%rcx){sint8},%v1{%k1}
	vcvtpi2ps $4,(%rcx){uint16},%v1{%k1}
	vcvtpi2ps $4,(%rcx){sint16},%v1{%k1}
	vcvtpu2ps $4,%v3,%v1{%k1}
	vcvtpu2ps $4,%v3{cdab},%v1{%k1}
	vcvtpu2ps $4,%v3{badc},%v1{%k1}
	vcvtpu2ps $4,%v3{aaaa},%v1{%k1}
	vcvtpu2ps $4,%v3{bbbb},%v1{%k1}
	vcvtpu2ps $4,%v3{cccc},%v1{%k1}
	vcvtpu2ps $4,%v3{dddd},%v1{%k1}
	vcvtpu2ps $4,%v3{dacb},%v1{%k1}
	vcvtpu2ps {.0b},%v3{dacb},%v1{%k1}
	vcvtpu2ps {.4b},%v3{dacb},%v1{%k1}
	vcvtpu2ps {.5b},%v3{dacb},%v1{%k1}
	vcvtpu2ps {.8b},%v3{dacb},%v1{%k1}
	vcvtpu2ps {.16b},%v3{dacb},%v1{%k1}
	vcvtpu2ps {.24b},%v3{dacb},%v1{%k1}
	vcvtpu2ps {.31b},%v3{dacb},%v1{%k1}
	vcvtpu2ps {.32b},%v3{dacb},%v1{%k1}
	vcvtpu2ps $4,(%rcx){nt},%v1{%k1}
	vcvtpu2ps $4,(%rcx),%v1{%k1}
	vcvtpu2ps $4,(%rcx){16to16},%v1{%k1}
	vcvtpu2ps $4,(%rcx){1to16},%v1{%k1}
	vcvtpu2ps $4,(%rcx){4to16},%v1{%k1}
	vcvtpu2ps $4,(%rcx){uint8},%v1{%k1}
	vcvtpu2ps $4,(%rcx){sint8},%v1{%k1}
	vcvtpu2ps $4,(%rcx){uint16},%v1{%k1}
	vcvtpu2ps $4,(%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, sf(v2/mt), half
	vcvtps2pd $1,%v3,%v1{%k1}
	vcvtps2pd $1,%v3{cdab},%v1{%k1}
	vcvtps2pd $1,%v3{badc},%v1{%k1}
	vcvtps2pd $1,%v3{aaaa},%v1{%k1}
	vcvtps2pd $1,%v3{bbbb},%v1{%k1}
	vcvtps2pd $1,%v3{cccc},%v1{%k1}
	vcvtps2pd $1,%v3{dddd},%v1{%k1}
	vcvtps2pd $1,%v3{dacb},%v1{%k1}
	vcvtps2pd $1,(%rcx){nt},%v1{%k1}
	vcvtps2pd $1,(%rcx),%v1{%k1}
	vcvtps2pd $1,(%rcx){16to16},%v1{%k1}
	vcvtps2pd $1,(%rcx){1to16},%v1{%k1}
	vcvtps2pd $1,(%rcx){4to16},%v1{%k1}
	vcvtps2pd $1,(%rcx){uint8},%v1{%k1}
	vcvtps2pd $1,(%rcx){unorm8},%v1{%k1}
	vcvtps2pd $1,(%rcx){float16},%v1{%k1}
	vcvtps2pd $1,(%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, sf(v2/mt), rc, expd
	vcvtps2pi $4,{rn},%v3,%v1{%k1}
	vcvtps2pi $4,{rn},%v3{cdab},%v1{%k1}
	vcvtps2pi $4,{rn},%v3{badc},%v1{%k1}
	vcvtps2pi $4,{rn},%v3{aaaa},%v1{%k1}
	vcvtps2pi $4,{rn},%v3{bbbb},%v1{%k1}
	vcvtps2pi $4,{rn},%v3{cccc},%v1{%k1}
	vcvtps2pi $4,{rn},%v3{dddd},%v1{%k1}
	vcvtps2pi $4,{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.0b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.4b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.5b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.8b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.16b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.24b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.31b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi {.32b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){nt},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx),%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){16to16},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){1to16},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){4to16},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){uint8},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){unorm8},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){float16},%v1{%k1}
	vcvtps2pi $4,{rn},(%rcx){sint16},%v1{%k1}
	vcvtps2pu $4,{rn},%v3,%v1{%k1}
	vcvtps2pu $4,{rn},%v3{cdab},%v1{%k1}
	vcvtps2pu $4,{rn},%v3{badc},%v1{%k1}
	vcvtps2pu $4,{rn},%v3{aaaa},%v1{%k1}
	vcvtps2pu $4,{rn},%v3{bbbb},%v1{%k1}
	vcvtps2pu $4,{rn},%v3{cccc},%v1{%k1}
	vcvtps2pu $4,{rn},%v3{dddd},%v1{%k1}
	vcvtps2pu $4,{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.0b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.4b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.5b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.8b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.16b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.24b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.31b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu {.32b},{rn},%v3{dacb},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){nt},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx),%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){16to16},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){1to16},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){4to16},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){uint8},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){unorm8},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){float16},%v1{%k1}
	vcvtps2pu $4,{rn},(%rcx){sint16},%v1{%k1}
	vroundps $4,{rn},%v3,%v1{%k1}
	vroundps $4,{rn},%v3{cdab},%v1{%k1}
	vroundps $4,{rn},%v3{badc},%v1{%k1}
	vroundps $4,{rn},%v3{aaaa},%v1{%k1}
	vroundps $4,{rn},%v3{bbbb},%v1{%k1}
	vroundps $4,{rn},%v3{cccc},%v1{%k1}
	vroundps $4,{rn},%v3{dddd},%v1{%k1}
	vroundps $4,{rn},%v3{dacb},%v1{%k1}
	vroundps {.0b},{rn},%v3{dacb},%v1{%k1}
	vroundps {.4b},{rn},%v3{dacb},%v1{%k1}
	vroundps {.5b},{rn},%v3{dacb},%v1{%k1}
	vroundps {.8b},{rn},%v3{dacb},%v1{%k1}
	vroundps {.16b},{rn},%v3{dacb},%v1{%k1}
	vroundps {.24b},{rn},%v3{dacb},%v1{%k1}
	vroundps {.31b},{rn},%v3{dacb},%v1{%k1}
	vroundps {.32b},{rn},%v3{dacb},%v1{%k1}
	vroundps $4,{rn},(%rcx){nt},%v1{%k1}
	vroundps $4,{rn},(%rcx),%v1{%k1}
	vroundps $4,{rn},(%rcx){16to16},%v1{%k1}
	vroundps $4,{rn},(%rcx){1to16},%v1{%k1}
	vroundps $4,{rn},(%rcx){4to16},%v1{%k1}
	vroundps $4,{rn},(%rcx){uint8},%v1{%k1}
	vroundps $4,{rn},(%rcx){unorm8},%v1{%k1}
	vroundps $4,{rn},(%rcx){float16},%v1{%k1}
	vroundps $4,{rn},(%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, sf(v2/mt)
	vcvtps2srgb8 %v3,%v1{%k1}
	vcvtps2srgb8 %v3{cdab},%v1{%k1}
	vcvtps2srgb8 %v3{badc},%v1{%k1}
	vcvtps2srgb8 %v3{aaaa},%v1{%k1}
	vcvtps2srgb8 %v3{bbbb},%v1{%k1}
	vcvtps2srgb8 %v3{cccc},%v1{%k1}
	vcvtps2srgb8 %v3{dddd},%v1{%k1}
	vcvtps2srgb8 %v3{dacb},%v1{%k1}
	vcvtps2srgb8 (%rcx){nt},%v1{%k1}
	vcvtps2srgb8 (%rcx),%v1{%k1}
	vcvtps2srgb8 (%rcx){16to16},%v1{%k1}
	vcvtps2srgb8 (%rcx){1to16},%v1{%k1}
	vcvtps2srgb8 (%rcx){4to16},%v1{%k1}
	vcvtps2srgb8 (%rcx){uint8},%v1{%k1}
	vcvtps2srgb8 (%rcx){unorm8},%v1{%k1}
	vcvtps2srgb8 (%rcx){float16},%v1{%k1}
	vcvtps2srgb8 (%rcx){sint16},%v1{%k1}
	vlog2lutps %v3,%v1{%k1}
	vlog2lutps %v3{cdab},%v1{%k1}
	vlog2lutps %v3{badc},%v1{%k1}
	vlog2lutps %v3{aaaa},%v1{%k1}
	vlog2lutps %v3{bbbb},%v1{%k1}
	vlog2lutps %v3{cccc},%v1{%k1}
	vlog2lutps %v3{dddd},%v1{%k1}
	vlog2lutps %v3{dacb},%v1{%k1}
	vlog2lutps (%rcx){nt},%v1{%k1}
	vlog2lutps (%rcx),%v1{%k1}
	vlog2lutps (%rcx){16to16},%v1{%k1}
	vlog2lutps (%rcx){1to16},%v1{%k1}
	vlog2lutps (%rcx){4to16},%v1{%k1}
	vlog2lutps (%rcx){uint8},%v1{%k1}
	vlog2lutps (%rcx){unorm8},%v1{%k1}
	vlog2lutps (%rcx){float16},%v1{%k1}
	vlog2lutps (%rcx){sint16},%v1{%k1}
	vrcpresps %v3,%v1{%k1}
	vrcpresps %v3{cdab},%v1{%k1}
	vrcpresps %v3{badc},%v1{%k1}
	vrcpresps %v3{aaaa},%v1{%k1}
	vrcpresps %v3{bbbb},%v1{%k1}
	vrcpresps %v3{cccc},%v1{%k1}
	vrcpresps %v3{dddd},%v1{%k1}
	vrcpresps %v3{dacb},%v1{%k1}
	vrcpresps (%rcx){nt},%v1{%k1}
	vrcpresps (%rcx),%v1{%k1}
	vrcpresps (%rcx){16to16},%v1{%k1}
	vrcpresps (%rcx){1to16},%v1{%k1}
	vrcpresps (%rcx){4to16},%v1{%k1}
	vrcpresps (%rcx){uint8},%v1{%k1}
	vrcpresps (%rcx){unorm8},%v1{%k1}
	vrcpresps (%rcx){float16},%v1{%k1}
	vrcpresps (%rcx){sint16},%v1{%k1}
	vrsqrtlutps %v3,%v1{%k1}
	vrsqrtlutps %v3{cdab},%v1{%k1}
	vrsqrtlutps %v3{badc},%v1{%k1}
	vrsqrtlutps %v3{aaaa},%v1{%k1}
	vrsqrtlutps %v3{bbbb},%v1{%k1}
	vrsqrtlutps %v3{cccc},%v1{%k1}
	vrsqrtlutps %v3{dddd},%v1{%k1}
	vrsqrtlutps %v3{dacb},%v1{%k1}
	vrsqrtlutps (%rcx){nt},%v1{%k1}
	vrsqrtlutps (%rcx),%v1{%k1}
	vrsqrtlutps (%rcx){16to16},%v1{%k1}
	vrsqrtlutps (%rcx){1to16},%v1{%k1}
	vrsqrtlutps (%rcx){4to16},%v1{%k1}
	vrsqrtlutps (%rcx){uint8},%v1{%k1}
	vrsqrtlutps (%rcx){unorm8},%v1{%k1}
	vrsqrtlutps (%rcx){float16},%v1{%k1}
	vrsqrtlutps (%rcx){sint16},%v1{%k1}
	vgetexpps %v3,%v1{%k1}
	vgetexpps %v3{cdab},%v1{%k1}
	vgetexpps %v3{badc},%v1{%k1}
	vgetexpps %v3{aaaa},%v1{%k1}
	vgetexpps %v3{bbbb},%v1{%k1}
	vgetexpps %v3{cccc},%v1{%k1}
	vgetexpps %v3{dddd},%v1{%k1}
	vgetexpps %v3{dacb},%v1{%k1}
	vgetexpps (%rcx){nt},%v1{%k1}
	vgetexpps (%rcx),%v1{%k1}
	vgetexpps (%rcx){16to16},%v1{%k1}
	vgetexpps (%rcx){1to16},%v1{%k1}
	vgetexpps (%rcx){4to16},%v1{%k1}
	vgetexpps (%rcx){uint8},%v1{%k1}
	vgetexpps (%rcx){unorm8},%v1{%k1}
	vgetexpps (%rcx){float16},%v1{%k1}
	vgetexpps (%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, si(v2/mt)
	vexp2lutps %v3,%v1{%k1}
	vexp2lutps %v3{cdab},%v1{%k1}
	vexp2lutps %v3{badc},%v1{%k1}
	vexp2lutps %v3{aaaa},%v1{%k1}
	vexp2lutps %v3{bbbb},%v1{%k1}
	vexp2lutps %v3{cccc},%v1{%k1}
	vexp2lutps %v3{dddd},%v1{%k1}
	vexp2lutps %v3{dacb},%v1{%k1}
	vexp2lutps (%rcx){nt},%v1{%k1}
	vexp2lutps (%rcx),%v1{%k1}
	vexp2lutps (%rcx){16to16},%v1{%k1}
	vexp2lutps (%rcx){1to16},%v1{%k1}
	vexp2lutps (%rcx){4to16},%v1{%k1}
	vexp2lutps (%rcx){uint8},%v1{%k1}
	vexp2lutps (%rcx){sint8},%v1{%k1}
	vexp2lutps (%rcx){uint16},%v1{%k1}
	vexp2lutps (%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, sf(v2/mt), imm21
	vfixupps $2,%v3,%v1{%k1}
	vfixupps $2,%v3{cdab},%v1{%k1}
	vfixupps $2,%v3{badc},%v1{%k1}
	vfixupps $2,%v3{aaaa},%v1{%k1}
	vfixupps $2,%v3{bbbb},%v1{%k1}
	vfixupps $2,%v3{cccc},%v1{%k1}
	vfixupps $2,%v3{dddd},%v1{%k1}
	vfixupps $2,%v3{dacb},%v1{%k1}
	vfixupps $2,(%rcx){nt},%v1{%k1}
	vfixupps $2,(%rcx),%v1{%k1}
	vfixupps $2,(%rcx){16to16},%v1{%k1}
	vfixupps $2,(%rcx){1to16},%v1{%k1}
	vfixupps $2,(%rcx){4to16},%v1{%k1}
	vfixupps $2,(%rcx){uint8},%v1{%k1}
	vfixupps $2,(%rcx){unorm8},%v1{%k1}
	vfixupps $2,(%rcx){float16},%v1{%k1}
	vfixupps $2,(%rcx){sint16},%v1{%k1}

# Tests for op v1{k1}, v2, si(v3/mt), rot, bil, bih
	vinsertfieldpi $31,$2,$11,%v3,%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,%v3{cdab},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,%v3{badc},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,%v3{aaaa},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,%v3{bbbb},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,%v3{cccc},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,%v3{dddd},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,%v3{dacb},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){nt},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx),%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){16to16},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){1to16},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){4to16},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){uint8},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){sint8},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){uint16},%v2,%v1{%k1}
	vinsertfieldpi $31,$2,$11,(%rcx){sint16},%v2,%v1{%k1}

# Tests for op v1{k1}, sb(f(mt))
	vloadd (%rcx){nt},%v1{%k1}
	vloadd (%rcx),%v1{%k1}
	vloadd (%rcx){16to16},%v1{%k1}
	vloadd (%rcx){1to16},%v1{%k1}
	vloadd (%rcx){4to16},%v1{%k1}
	vloadd (%rcx){float16},%v1{%k1}
	vloadd (%rcx){float16}{16to16},%v1{%k1}
	vloadd (%rcx){float16}{1to16},%v1{%k1}
	vloadd (%rcx){float16}{4to16},%v1{%k1}
	vloadd (%rcx){srgb8},%v1{%k1}
	vloadd (%rcx){srgb8}{16to16},%v1{%k1}
	vloadd (%rcx){srgb8}{1to16},%v1{%k1}
	vloadd (%rcx){srgb8}{4to16},%v1{%k1}
	vloadd (%rcx){uint8},%v1{%k1}
	vloadd (%rcx){uint8}{16to16},%v1{%k1}
	vloadd (%rcx){uint8}{1to16},%v1{%k1}
	vloadd (%rcx){uint8}{4to16},%v1{%k1}
	vloadd (%rcx){sint8},%v1{%k1}
	vloadd (%rcx){sint8}{16to16},%v1{%k1}
	vloadd (%rcx){sint8}{1to16},%v1{%k1}
	vloadd (%rcx){sint8}{4to16},%v1{%k1}
	vloadd (%rcx){unorm8},%v1{%k1}
	vloadd (%rcx){unorm8}{16to16},%v1{%k1}
	vloadd (%rcx){unorm8}{1to16},%v1{%k1}
	vloadd (%rcx){unorm8}{4to16},%v1{%k1}
	vloadd (%rcx){snorm8},%v1{%k1}
	vloadd (%rcx){snorm8}{16to16},%v1{%k1}
	vloadd (%rcx){snorm8}{1to16},%v1{%k1}
	vloadd (%rcx){snorm8}{4to16},%v1{%k1}
	vloadd (%rcx){uint16},%v1{%k1}
	vloadd (%rcx){uint16}{16to16},%v1{%k1}
	vloadd (%rcx){uint16}{1to16},%v1{%k1}
	vloadd (%rcx){uint16}{4to16},%v1{%k1}
	vloadd (%rcx){sint16},%v1{%k1}
	vloadd (%rcx){sint16}{16to16},%v1{%k1}
	vloadd (%rcx){sint16}{1to16},%v1{%k1}
	vloadd (%rcx){sint16}{4to16},%v1{%k1}
	vloadd (%rcx){unorm16},%v1{%k1}
	vloadd (%rcx){unorm16}{16to16},%v1{%k1}
	vloadd (%rcx){unorm16}{1to16},%v1{%k1}
	vloadd (%rcx){unorm16}{4to16},%v1{%k1}
	vloadd (%rcx){snorm16},%v1{%k1}
	vloadd (%rcx){snorm16}{16to16},%v1{%k1}
	vloadd (%rcx){snorm16}{1to16},%v1{%k1}
	vloadd (%rcx){snorm16}{4to16},%v1{%k1}
	vloadd (%rcx){uint8i},%v1{%k1}
	vloadd (%rcx){uint8i}{16to16},%v1{%k1}
	vloadd (%rcx){uint8i}{1to16},%v1{%k1}
	vloadd (%rcx){uint8i}{4to16},%v1{%k1}
	vloadd (%rcx){sint8i},%v1{%k1}
	vloadd (%rcx){sint8i}{16to16},%v1{%k1}
	vloadd (%rcx){sint8i}{1to16},%v1{%k1}
	vloadd (%rcx){sint8i}{4to16},%v1{%k1}
	vloadd (%rcx){uint16i},%v1{%k1}
	vloadd (%rcx){uint16i}{16to16},%v1{%k1}
	vloadd (%rcx){uint16i}{1to16},%v1{%k1}
	vloadd (%rcx){uint16i}{4to16},%v1{%k1}
	vloadd (%rcx){sint16i},%v1{%k1}
	vloadd (%rcx){sint16i}{16to16},%v1{%k1}
	vloadd (%rcx){sint16i}{1to16},%v1{%k1}
	vloadd (%rcx){sint16i}{4to16},%v1{%k1}
	vloadd (%rcx){unorm10A},%v1{%k1}
	vloadd (%rcx){unorm10A}{16to16},%v1{%k1}
	vloadd (%rcx){unorm10A}{1to16},%v1{%k1}
	vloadd (%rcx){unorm10A}{4to16},%v1{%k1}
	vloadd (%rcx){unorm10B},%v1{%k1}
	vloadd (%rcx){unorm10B}{16to16},%v1{%k1}
	vloadd (%rcx){unorm10B}{1to16},%v1{%k1}
	vloadd (%rcx){unorm10B}{4to16},%v1{%k1}
	vloadd (%rcx){unorm10C},%v1{%k1}
	vloadd (%rcx){unorm10C}{16to16},%v1{%k1}
	vloadd (%rcx){unorm10C}{1to16},%v1{%k1}
	vloadd (%rcx){unorm10C}{4to16},%v1{%k1}
	vloadd (%rcx){unorm2D},%v1{%k1}
	vloadd (%rcx){unorm2D}{16to16},%v1{%k1}
	vloadd (%rcx){unorm2D}{1to16},%v1{%k1}
	vloadd (%rcx){unorm2D}{4to16},%v1{%k1}
	vloadd (%rcx){float11A},%v1{%k1}
	vloadd (%rcx){float11A}{16to16},%v1{%k1}
	vloadd (%rcx){float11A}{1to16},%v1{%k1}
	vloadd (%rcx){float11A}{4to16},%v1{%k1}
	vloadd (%rcx){float11B},%v1{%k1}
	vloadd (%rcx){float11B}{16to16},%v1{%k1}
	vloadd (%rcx){float11B}{1to16},%v1{%k1}
	vloadd (%rcx){float11B}{4to16},%v1{%k1}
	vloadd (%rcx){float10C},%v1{%k1}
	vloadd (%rcx){float10C}{16to16},%v1{%k1}
	vloadd (%rcx){float10C}{1to16},%v1{%k1}
	vloadd (%rcx){float10C}{4to16},%v1{%k1}

# Tests for op v1{k1}, sb(fq(mt))
	vloadq (%rcx){nt},%v1{%k1}
	vloadq (%rcx){8to8},%v1{%k1}
	vloadq (%rcx){1to8},%v1{%k1}
	vloadq (%rcx){4to8},%v1{%k1}

# Tests for op v1{k1}, f(mt)
	vloadunpackhd (%rcx){nt},%v1{%k1}
	vloadunpackhd (%rcx),%v1{%k1}
	vloadunpackhd (%rcx){float16},%v1{%k1}
	vloadunpackhd (%rcx){srgb8},%v1{%k1}
	vloadunpackhd (%rcx){uint8},%v1{%k1}
	vloadunpackhd (%rcx){sint8},%v1{%k1}
	vloadunpackhd (%rcx){unorm8},%v1{%k1}
	vloadunpackhd (%rcx){snorm8},%v1{%k1}
	vloadunpackhd (%rcx){uint16},%v1{%k1}
	vloadunpackhd (%rcx){sint16},%v1{%k1}
	vloadunpackhd (%rcx){unorm16},%v1{%k1}
	vloadunpackhd (%rcx){snorm16},%v1{%k1}
	vloadunpackhd (%rcx){uint8i},%v1{%k1}
	vloadunpackhd (%rcx){sint8i},%v1{%k1}
	vloadunpackhd (%rcx){uint16i},%v1{%k1}
	vloadunpackhd (%rcx){sint16i},%v1{%k1}
	vloadunpackhd (%rcx){unorm10A},%v1{%k1}
	vloadunpackhd (%rcx){unorm10B},%v1{%k1}
	vloadunpackhd (%rcx){unorm10C},%v1{%k1}
	vloadunpackhd (%rcx){unorm2D},%v1{%k1}
	vloadunpackhd (%rcx){float11A},%v1{%k1}
	vloadunpackhd (%rcx){float11B},%v1{%k1}
	vloadunpackhd (%rcx){float10C},%v1{%k1}
	vloadunpackld (%rcx){nt},%v1{%k1}
	vloadunpackld (%rcx),%v1{%k1}
	vloadunpackld (%rcx){float16},%v1{%k1}
	vloadunpackld (%rcx){srgb8},%v1{%k1}
	vloadunpackld (%rcx){uint8},%v1{%k1}
	vloadunpackld (%rcx){sint8},%v1{%k1}
	vloadunpackld (%rcx){unorm8},%v1{%k1}
	vloadunpackld (%rcx){snorm8},%v1{%k1}
	vloadunpackld (%rcx){uint16},%v1{%k1}
	vloadunpackld (%rcx){sint16},%v1{%k1}
	vloadunpackld (%rcx){unorm16},%v1{%k1}
	vloadunpackld (%rcx){snorm16},%v1{%k1}
	vloadunpackld (%rcx){uint8i},%v1{%k1}
	vloadunpackld (%rcx){sint8i},%v1{%k1}
	vloadunpackld (%rcx){uint16i},%v1{%k1}
	vloadunpackld (%rcx){sint16i},%v1{%k1}
	vloadunpackld (%rcx){unorm10A},%v1{%k1}
	vloadunpackld (%rcx){unorm10B},%v1{%k1}
	vloadunpackld (%rcx){unorm10C},%v1{%k1}
	vloadunpackld (%rcx){unorm2D},%v1{%k1}
	vloadunpackld (%rcx){float11A},%v1{%k1}
	vloadunpackld (%rcx){float11B},%v1{%k1}
	vloadunpackld (%rcx){float10C},%v1{%k1}

# Tests for op v1{k1}, si(v2/mt), rot, bil, bih
	vrotatefieldpi $31,$2,$11,%v3,%v1{%k1}
	vrotatefieldpi $31,$2,$11,%v3{cdab},%v1{%k1}
	vrotatefieldpi $31,$2,$11,%v3{badc},%v1{%k1}
	vrotatefieldpi $31,$2,$11,%v3{aaaa},%v1{%k1}
	vrotatefieldpi $31,$2,$11,%v3{bbbb},%v1{%k1}
	vrotatefieldpi $31,$2,$11,%v3{cccc},%v1{%k1}
	vrotatefieldpi $31,$2,$11,%v3{dddd},%v1{%k1}
	vrotatefieldpi $31,$2,$11,%v3{dacb},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){nt},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx),%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){16to16},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){1to16},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){4to16},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){uint8},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){sint8},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){uint16},%v1{%k1}
	vrotatefieldpi $31,$2,$11,(%rcx){sint16},%v1{%k1}

# Test for op v1{k1}, v2/mt, perm128, perm32
	vshuf128x32 $2,$2,%v2,%v1{%k1}
	vshuf128x32 $2,$2,(%rcx){nt},%v1{%k1}

# Test for op mt{k1}, ss(d(v1))
	vstored %v1,(%rcx){nt}{%k1}
	vstored %v1,(%rcx){%k1}
	vstored %v1{float16},(%rcx){%k1}
	vstored %v1{float16rz},(%rcx){%k1}
	vstored %v1{uint8},(%rcx){%k1}
	vstored %v1{sint8},(%rcx){%k1}
	vstored %v1{unorm8},(%rcx){%k1}
	vstored %v1{snorm8},(%rcx){%k1}
	vstored %v1{uint16},(%rcx){%k1}
	vstored %v1{sint16},(%rcx){%k1}
	vstored %v1{unorm16},(%rcx){%k1}
	vstored %v1{snorm16},(%rcx){%k1}
	vstored %v1{uint8i},(%rcx){%k1}
	vstored %v1{sint8i},(%rcx){%k1}
	vstored %v1{uint16i},(%rcx){%k1}
	vstored %v1{sint16i},(%rcx){%k1}
	vstored %v1{a},(%rcx){%k1}
	vstored %v1{a}{float16},(%rcx){%k1}
	vstored %v1{a}{float16rz},(%rcx){%k1}
	vstored %v1{a}{uint8},(%rcx){%k1}
	vstored %v1{a}{sint8},(%rcx){%k1}
	vstored %v1{a}{unorm8},(%rcx){%k1}
	vstored %v1{a}{snorm8},(%rcx){%k1}
	vstored %v1{a}{uint16},(%rcx){%k1}
	vstored %v1{a}{sint16},(%rcx){%k1}
	vstored %v1{a}{unorm16},(%rcx){%k1}
	vstored %v1{a}{snorm16},(%rcx){%k1}
	vstored %v1{a}{uint8i},(%rcx){%k1}
	vstored %v1{a}{sint8i},(%rcx){%k1}
	vstored %v1{a}{uint16i},(%rcx){%k1}
	vstored %v1{a}{sint16i},(%rcx){%k1}
	vstored %v1{dcba},(%rcx){%k1}
	vstored %v1{dcba}{float16},(%rcx){%k1}
	vstored %v1{dcba}{float16rz},(%rcx){%k1}
	vstored %v1{dcba}{uint8},(%rcx){%k1}
	vstored %v1{dcba}{sint8},(%rcx){%k1}
	vstored %v1{dcba}{unorm8},(%rcx){%k1}
	vstored %v1{dcba}{snorm8},(%rcx){%k1}
	vstored %v1{dcba}{uint16},(%rcx){%k1}
	vstored %v1{dcba}{sint16},(%rcx){%k1}
	vstored %v1{dcba}{unorm16},(%rcx){%k1}
	vstored %v1{dcba}{snorm16},(%rcx){%k1}
	vstored %v1{dcba}{uint8i},(%rcx){%k1}
	vstored %v1{dcba}{sint8i},(%rcx){%k1}
	vstored %v1{dcba}{uint16i},(%rcx){%k1}
	vstored %v1{dcba}{sint16i},(%rcx){%k1}

# Test for op mt{k1}, ss(dq(v1))
	vstoreq %v1,(%rcx){nt}{%k1}
	vstoreq %v1,(%rcx){%k1}
	vstoreq %v1{a},(%rcx){%k1}
	vstoreq %v1{dcba},(%rcx){%k1}

# Tests for op v1{k1}, fq(mt)
	vloadunpackhq (%rcx){nt},%v1{%k1}
	vloadunpackhq (%rcx),%v1{%k1}
	vloadunpacklq (%rcx){nt},%v1{%k1}
	vloadunpacklq (%rcx),%v1{%k1}

# Tests for op mt{k1}, D(v1)
	vpackstorehd %v1,(%rcx){nt}{%k1}
	vpackstorehd %v1,(%rcx){%k1}
	vpackstorehd %v1{float16},(%rcx){%k1}
	vpackstorehd %v1{float16rz},(%rcx){%k1}
	vpackstorehd %v1{uint8},(%rcx){%k1}
	vpackstorehd %v1{sint8},(%rcx){%k1}
	vpackstorehd %v1{unorm8},(%rcx){%k1}
	vpackstorehd %v1{snorm8},(%rcx){%k1}
	vpackstorehd %v1{uint16},(%rcx){%k1}
	vpackstorehd %v1{sint16},(%rcx){%k1}
	vpackstorehd %v1{unorm16},(%rcx){%k1}
	vpackstorehd %v1{snorm16},(%rcx){%k1}
	vpackstorehd %v1{uint8i},(%rcx){%k1}
	vpackstorehd %v1{sint8i},(%rcx){%k1}
	vpackstorehd %v1{uint16i},(%rcx){%k1}
	vpackstorehd %v1{sint16i},(%rcx){%k1}
	vpackstoreld %v1,(%rcx){nt}{%k1}
	vpackstoreld %v1,(%rcx){%k1}
	vpackstoreld %v1{float16},(%rcx){%k1}
	vpackstoreld %v1{float16rz},(%rcx){%k1}
	vpackstoreld %v1{uint8},(%rcx){%k1}
	vpackstoreld %v1{sint8},(%rcx){%k1}
	vpackstoreld %v1{unorm8},(%rcx){%k1}
	vpackstoreld %v1{snorm8},(%rcx){%k1}
	vpackstoreld %v1{uint16},(%rcx){%k1}
	vpackstoreld %v1{sint16},(%rcx){%k1}
	vpackstoreld %v1{unorm16},(%rcx){%k1}
	vpackstoreld %v1{snorm16},(%rcx){%k1}
	vpackstoreld %v1{uint8i},(%rcx){%k1}
	vpackstoreld %v1{sint8i},(%rcx){%k1}
	vpackstoreld %v1{uint16i},(%rcx){%k1}
	vpackstoreld %v1{sint16i},(%rcx){%k1}

# Tests for op mt{k1}, Dq(v1)
	vpackstorehq %v1,(%rcx){nt}{%k1}
	vpackstorehq %v1,(%rcx){%k1}
	vpackstorelq %v1,(%rcx){nt}{%k1}
	vpackstorelq %v1,(%rcx){%k1}

# Tests for op mvt{k1}, D(v1)
	vscatterd %v1,0x99(%rbp, %v10, 2){nt}{%k1}
	vscatterd %v1,0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{float16},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{float16rz},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{uint8},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{sint8},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{unorm8},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{snorm8},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{uint16},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{sint16},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{unorm16},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{snorm16},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{uint8i},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{sint8i},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{uint16i},0x99(%rbp, %v10, 2){%k1}
	vscatterd %v1{sint16i},0x99(%rbp, %v10, 2){%k1}

# Tests for op d(mvt){k1}
	vscatterpfd 0x99(%rbp, %v10, 2){nt}{%k1}
	vscatterpfd 0x99(%rbp, %v10, 2){%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){float16}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){float16rz}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){uint8}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){sint8}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){unorm8}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){snorm8}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){uint16}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){sint16}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){unorm16}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){snorm16}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){uint8i}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){sint8i}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){uint16i}{%k2}
	vscatterpfd 0x99(%rbp, %v10, 2){sint16i}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){nt}{%k1}
	vscatterpf2d 0x99(%rbp, %v10, 2){%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){float16}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){float16rz}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){uint8}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){sint8}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){unorm8}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){snorm8}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){uint16}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){sint16}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){unorm16}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){snorm16}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){uint8i}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){sint8i}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){uint16i}{%k2}
	vscatterpf2d 0x99(%rbp, %v10, 2){sint16i}{%k2}

# Tests for op v1{k1}, f(mvt)
	vgatherd 0x99(%rbp, %v10, 2){nt},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2),%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){float16},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){srgb8},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){uint8},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){sint8},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){unorm8},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){snorm8},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){uint16},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){sint16},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){unorm16},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){snorm16},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){uint8i},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){sint8i},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){uint16i},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){sint16i},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){unorm10A},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){unorm10B},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){unorm10C},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){unorm2D},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){float11A},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){float11B},%v1{%k1}
	vgatherd 0x99(%rbp, %v10, 2){float10C},%v1{%k1}

# Tests for op f(mvt){k1}
	vgatherpfd 0x99(%rbp, %v10, 2){nt}{%k1}
	vgatherpfd 0x99(%rbp, %v10, 2){%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){float16}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){srgb8}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){uint8}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){sint8}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){unorm8}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){snorm8}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){uint16}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){sint16}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){unorm16}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){snorm16}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){uint8i}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){sint8i}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){uint16i}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){sint16i}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){unorm10A}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){unorm10B}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){unorm10C}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){unorm2D}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){float11A}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){float11B}{%k2}
	vgatherpfd 0x99(%rbp, %v10, 2){float10C}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){nt}{%k1}
	vgatherpf2d 0x99(%rbp, %v10, 2){%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){float16}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){srgb8}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){uint8}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){sint8}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){unorm8}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){snorm8}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){uint16}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){sint16}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){unorm16}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){snorm16}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){uint8i}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){sint8i}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){uint16i}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){sint16i}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){unorm10A}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){unorm10B}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){unorm10C}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){unorm2D}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){float11A}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){float11B}{%k2}
	vgatherpf2d 0x99(%rbp, %v10, 2){float10C}{%k2}

# Test for op r16, r16
	bitinterleave11 %ax,%bx
	bitinterleave11 %r9w,%r15w
	bitinterleave21 %ax,%bx
	bitinterleave21 %r9w,%r15w
	bsff %ax,%bx
	bsff %r9w,%r15w
	bsfi %ax,%bx
	bsfi %r9w,%r15w
	bsrf %ax,%bx
	bsrf %r9w,%r15w
	bsri %ax,%bx
	bsri %r9w,%r15w
	countbits %ax,%bx
	countbits %r9w,%r15w
	quadmask16 %ax,%bx
	quadmask16 %r9w,%r15w

# Test for op r32, r32
	bitinterleave11 %eax,%ebx
	bitinterleave11 %r9d,%r15d
	bitinterleave21 %eax,%ebx
	bitinterleave21 %r9d,%r15d
	bsff %eax,%ebx
	bsff %r9d,%r15d
	bsfi %eax,%ebx
	bsfi %r9d,%r15d
	bsrf %eax,%ebx
	bsrf %r9d,%r15d
	bsri %eax,%ebx
	bsri %r9d,%r15d
	countbits %eax,%ebx
	countbits %r9d,%r15d
	quadmask16 %eax,%ebx
	quadmask16 %r9d,%r15d

# Test for op r64, r64
	bitinterleave11 %rax,%rbx
	bitinterleave11 %r9,%r15
	bitinterleave21 %rax,%rbx
	bitinterleave21 %r9,%r15
	bsff %rax,%rbx
	bsff %r9,%r15
	bsfi %rax,%rbx
	bsfi %r9,%r15
	bsrf %rax,%rbx
	bsrf %r9,%r15
	bsri %rax,%rbx
	bsri %r9,%r15
	countbits %rax,%rbx
	countbits %r9,%r15
	quadmask16 %rax,%rbx
	quadmask16 %r9,%r15

# Test for op m8
	clevict1 (%rcx)
	clevict2 (%rcx)

# Test for op m8, imm8
	vprefetch1 $7,(%rcx)

# Test for op m2, imm2
	vprefetch2 $3,(%rcx)

# Test for op m32
	ldvxcsr (%rcx)
	stvxcsr (%rcx)

# Test for op r16
	delay %ax
	delay %r9w
	spflt %ax
	spflt %r9w

# Test for op r32
	delay %eax
	delay %r9d
	spflt %eax
	spflt %r9d

# Test for op r64
	delay %rax
	delay %r9
	spflt %rax
	spflt %r9

# Test for op imm5
	delay $10

# Test for op r16, r16, rot, bil, bih
	insertfield $31,$2,$11,%ax,%bx
	insertfield $31,$2,$11,%r9w,%r15w
	rotatefield $31,$2,$11,%ax,%bx
	rotatefield $31,$2,$11,%r9w,%r15w

# Test for op r32, r32, rot, bil, bih
	insertfield $31,$2,$11,%eax,%ebx
	insertfield $31,$2,$11,%r9d,%r15d
	rotatefield $31,$2,$11,%eax,%ebx
	rotatefield $31,$2,$11,%r9d,%r15d

# Test for op r64, r64, rot, bil, bih
	insertfield $31,$2,$11,%rax,%rbx
	insertfield $31,$2,$11,%r9,%r15
	rotatefield $31,$2,$11,%rax,%rbx
	rotatefield $31,$2,$11,%r9,%r15

# Generate test cases for different operands forms
	vadcpi %v30{cdab},%k2,%v10{%k1}
	vaddnpd %v30{dddd},%v20,%v10{%k1}
	vadcpi 0x12345678,%k2,%v10{%k1}
	vadcpi (%rbp),%k2,%v10{%k1}
	vadcpi (%rsp),%k2,%v10{%k1}
	vadcpi 0x99(%rbp),%k2,%v10{%k1}
	vadcpi 0x99(%r15),%k2,%v10{%k1}
	vadcpi 0x99(%rip),%k2,%v10{%k1}
	vadcpi 0x99(%rsp),%k2,%v10{%k1}
	vadcpi 0x99(%r12),%k2,%v10{%k1}
	vadcpi -0x99(,%riz),%k2,%v10{%k1}
	vadcpi -0x99(,%riz,2),%k2,%v10{%k1}
	vadcpi -0x99(%rbx,%riz),%k2,%v10{%k1}
	vadcpi -0x99(%rbx,%riz,2),%k2,%v10{%k1}
	vadcpi -0x99(%r12,%r15,4),%k2,%v10{%k1}
	vadcpi -0x99(%r8,%r15,8),%k2,%v10{%k1}
	vadcpi -0x99(%rbp,%r13,4),%k2,%v10{%k1}
	vadcpi -0x99(%rsp,%r12,1),%k2,%v10{%k1}
	vgatherd (,%v1),%v10{%k1}
	vgatherd (,%v10),%v10{%k1}
	vgatherd -0x99(,%v1),%v10{%k1}
	vgatherd -0x99(,%v10),%v10{%k1}
	vgatherd (%rbx,%v1),%v10{%k1}
	vgatherd (%rbx,%v10),%v10{%k1}
	vgatherd -0x99(%rbx,%v1),%v10{%k1}
	vgatherd -0x99(%rbx,%v10),%v10{%k1}
	vgatherd -0x99(%rbx,%v1,2),%v10{%k1}
	vgatherd -0x99(%rbx,%v10,2),%v10{%k1}
	vgatherd -0x99(%r12,%v1,4),%v10{%k1}
	vgatherd -0x99(%r12,%v10,4),%v10{%k1}
	vadcpi 0x1234,%k2,%v10{%k1}
	vadcpi (%ebp),%k2,%v10{%k1}
	vadcpi (%esp),%k2,%v10{%k1}
	vadcpi 0x99(%ebp),%k2,%v10{%k1}
	vadcpi 0x99(,%eiz),%k2,%v10{%k1}
	vadcpi 0x99(,%eiz,2),%k2,%v10{%k1}
	vadcpi 0x99(%eax,%eiz),%k2,%v10{%k1}
	vadcpi 0x99(%eax,%eiz,2),%k2,%v10{%k1}
	vadcpi 0x99(%eax,%ebx,4),%k2,%v10{%k1}
	vadcpi 0x99(%esp,%ecx,8),%k2,%v10{%k1}
	vadcpi 0x99(%ebp,%edx,1),%k2,%v10{%k1}
	vgatherd (,%v1),%v10{%k1}
	vgatherd (,%v10),%v10{%k1}
	vgatherd -0x99(,%v1),%v10{%k1}
	vgatherd -0x99(,%v10),%v10{%k1}
	vgatherd (%eax,%v1),%v10{%k1}
	vgatherd (%eax,%v10),%v10{%k1}
	vgatherd 0x99(%eax,%v1,1),%v10{%k1}
	vgatherd 0x99(%eax,%v10,1),%v10{%k1}
	vgatherd 0x99(%eax,%v1,2),%v10{%k1}
	vgatherd 0x99(%eax,%v10,2),%v10{%k1}
	vgatherd 0x99(%ebp,%v1,4),%v10{%k1}
	vgatherd 0x99(%ebp,%v10,4),%v10{%k1}

	.intel_syntax noprefix

# Tests for op k1, k2
	vkand k2,k1
	vkandn k2,k1
	vkandnr k2,k1
	vkmov k2,k1
	vkmovlhb k2,k1
	vknot k2,k1
	vkor k2,k1
	vkortest k2,k1
	vkswapb k2,k1
	vkxnor k2,k1
	vkxor k2,k1

# Tests for op r16, k2
	vkmov k2,ax
	vkmov k2,r9w

# Tests for op r32, k2
	vkmov k2,eax
	vkmov k2,r9d

# Tests for op k1, r16
	vkmov ax,k1
	vkmov r9w,k1

# Tests for op k1, r32
	vkmov eax,k1
	vkmov r9d,k1

# Tests for op v1{k1}, k2, si(v3/mt)
	vadcpi v1{k1},k2,v3
	vadcpi v1{k1},k2,v3{cdab}
	vadcpi v1{k1},k2,v3{badc}
	vadcpi v1{k1},k2,v3{aaaa}
	vadcpi v1{k1},k2,v3{bbbb}
	vadcpi v1{k1},k2,v3{cccc}
	vadcpi v1{k1},k2,v3{dddd}
	vadcpi v1{k1},k2,v3{dacb}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{nt}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{16to16}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{1to16}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{4to16}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{uint8}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{sint8}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{uint16}
	vadcpi v1{k1},k2,ZMMWORD PTR [rcx]{sint16}
	vadcpi v1{k1},k2,[rcx]{sint16}
	vaddsetcpi v1{k1},k2,v3
	vaddsetcpi v1{k1},k2,v3{cdab}
	vaddsetcpi v1{k1},k2,v3{badc}
	vaddsetcpi v1{k1},k2,v3{aaaa}
	vaddsetcpi v1{k1},k2,v3{bbbb}
	vaddsetcpi v1{k1},k2,v3{cccc}
	vaddsetcpi v1{k1},k2,v3{dddd}
	vaddsetcpi v1{k1},k2,v3{dacb}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{nt}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{16to16}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{1to16}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{4to16}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{uint8}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{sint8}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{uint16}
	vaddsetcpi v1{k1},k2,ZMMWORD PTR [rcx]{sint16}
	vaddsetcpi v1{k1},k2,[rcx]{sint16}
	vsbbpi v1{k1},k2,v3
	vsbbpi v1{k1},k2,v3{cdab}
	vsbbpi v1{k1},k2,v3{badc}
	vsbbpi v1{k1},k2,v3{aaaa}
	vsbbpi v1{k1},k2,v3{bbbb}
	vsbbpi v1{k1},k2,v3{cccc}
	vsbbpi v1{k1},k2,v3{dddd}
	vsbbpi v1{k1},k2,v3{dacb}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{nt}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{16to16}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{1to16}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{4to16}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{uint8}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{sint8}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{uint16}
	vsbbpi v1{k1},k2,ZMMWORD PTR [rcx]{sint16}
	vsbbpi v1{k1},k2,[rcx]{sint16}
	vsbbrpi v1{k1},k2,v3
	vsbbrpi v1{k1},k2,v3{cdab}
	vsbbrpi v1{k1},k2,v3{badc}
	vsbbrpi v1{k1},k2,v3{aaaa}
	vsbbrpi v1{k1},k2,v3{bbbb}
	vsbbrpi v1{k1},k2,v3{cccc}
	vsbbrpi v1{k1},k2,v3{dddd}
	vsbbrpi v1{k1},k2,v3{dacb}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{nt}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{16to16}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{1to16}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{4to16}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{uint8}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{sint8}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{uint16}
	vsbbrpi v1{k1},k2,ZMMWORD PTR [rcx]{sint16}
	vsbbrpi v1{k1},k2,[rcx]{sint16}
	vsubrsetbpi v1{k1},k2,v3
	vsubrsetbpi v1{k1},k2,v3{cdab}
	vsubrsetbpi v1{k1},k2,v3{badc}
	vsubrsetbpi v1{k1},k2,v3{aaaa}
	vsubrsetbpi v1{k1},k2,v3{bbbb}
	vsubrsetbpi v1{k1},k2,v3{cccc}
	vsubrsetbpi v1{k1},k2,v3{dddd}
	vsubrsetbpi v1{k1},k2,v3{dacb}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{nt}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{16to16}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{1to16}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{4to16}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{uint8}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{sint8}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{uint16}
	vsubrsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{sint16}
	vsubrsetbpi v1{k1},k2,[rcx]{sint16}
	vsubsetbpi v1{k1},k2,v3
	vsubsetbpi v1{k1},k2,v3{cdab}
	vsubsetbpi v1{k1},k2,v3{badc}
	vsubsetbpi v1{k1},k2,v3{aaaa}
	vsubsetbpi v1{k1},k2,v3{bbbb}
	vsubsetbpi v1{k1},k2,v3{cccc}
	vsubsetbpi v1{k1},k2,v3{dddd}
	vsubsetbpi v1{k1},k2,v3{dacb}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{nt}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{16to16}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{1to16}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{4to16}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{uint8}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{sint8}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{uint16}
	vsubsetbpi v1{k1},k2,ZMMWORD PTR [rcx]{sint16}
	vsubsetbpi v1{k1},k2,[rcx]{sint16}

# Tests for op v1{k1}, v2, sf(v3/mt)
	vaddnps v1{k1},v2,v3
	vaddnps v1{k1},v2,v3{cdab}
	vaddnps v1{k1},v2,v3{badc}
	vaddnps v1{k1},v2,v3{aaaa}
	vaddnps v1{k1},v2,v3{bbbb}
	vaddnps v1{k1},v2,v3{cccc}
	vaddnps v1{k1},v2,v3{dddd}
	vaddnps v1{k1},v2,v3{dacb}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vaddnps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vaddnps v1{k1},v2,[rcx]{sint16}
	vaddps v1{k1},v2,v3
	vaddps v1{k1},v2,v3{cdab}
	vaddps v1{k1},v2,v3{badc}
	vaddps v1{k1},v2,v3{aaaa}
	vaddps v1{k1},v2,v3{bbbb}
	vaddps v1{k1},v2,v3{cccc}
	vaddps v1{k1},v2,v3{dddd}
	vaddps v1{k1},v2,v3{dacb}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vaddps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vaddps v1{k1},v2,[rcx]{sint16}
	vaddsetsps v1{k1},v2,v3
	vaddsetsps v1{k1},v2,v3{cdab}
	vaddsetsps v1{k1},v2,v3{badc}
	vaddsetsps v1{k1},v2,v3{aaaa}
	vaddsetsps v1{k1},v2,v3{bbbb}
	vaddsetsps v1{k1},v2,v3{cccc}
	vaddsetsps v1{k1},v2,v3{dddd}
	vaddsetsps v1{k1},v2,v3{dacb}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vaddsetsps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vaddsetsps v1{k1},v2,[rcx]{sint16}
	vclampzps v1{k1},v2,v3
	vclampzps v1{k1},v2,v3{cdab}
	vclampzps v1{k1},v2,v3{badc}
	vclampzps v1{k1},v2,v3{aaaa}
	vclampzps v1{k1},v2,v3{bbbb}
	vclampzps v1{k1},v2,v3{cccc}
	vclampzps v1{k1},v2,v3{dddd}
	vclampzps v1{k1},v2,v3{dacb}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vclampzps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vclampzps v1{k1},v2,[rcx]{sint16}
	vmadd132ps v1{k1},v2,v3
	vmadd132ps v1{k1},v2,v3{cdab}
	vmadd132ps v1{k1},v2,v3{badc}
	vmadd132ps v1{k1},v2,v3{aaaa}
	vmadd132ps v1{k1},v2,v3{bbbb}
	vmadd132ps v1{k1},v2,v3{cccc}
	vmadd132ps v1{k1},v2,v3{dddd}
	vmadd132ps v1{k1},v2,v3{dacb}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmadd132ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmadd132ps v1{k1},v2,[rcx]{sint16}
	vmadd213ps v1{k1},v2,v3
	vmadd213ps v1{k1},v2,v3{cdab}
	vmadd213ps v1{k1},v2,v3{badc}
	vmadd213ps v1{k1},v2,v3{aaaa}
	vmadd213ps v1{k1},v2,v3{bbbb}
	vmadd213ps v1{k1},v2,v3{cccc}
	vmadd213ps v1{k1},v2,v3{dddd}
	vmadd213ps v1{k1},v2,v3{dacb}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmadd213ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmadd213ps v1{k1},v2,[rcx]{sint16}
	vmadd231ps v1{k1},v2,v3
	vmadd231ps v1{k1},v2,v3{cdab}
	vmadd231ps v1{k1},v2,v3{badc}
	vmadd231ps v1{k1},v2,v3{aaaa}
	vmadd231ps v1{k1},v2,v3{bbbb}
	vmadd231ps v1{k1},v2,v3{cccc}
	vmadd231ps v1{k1},v2,v3{dddd}
	vmadd231ps v1{k1},v2,v3{dacb}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmadd231ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmadd231ps v1{k1},v2,[rcx]{sint16}
	vmadd233ps v1{k1},v2,v3
	vmadd233ps v1{k1},v2,v3{cdab}
	vmadd233ps v1{k1},v2,v3{badc}
	vmadd233ps v1{k1},v2,v3{aaaa}
	vmadd233ps v1{k1},v2,v3{bbbb}
	vmadd233ps v1{k1},v2,v3{cccc}
	vmadd233ps v1{k1},v2,v3{dddd}
	vmadd233ps v1{k1},v2,v3{dacb}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmadd233ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmadd233ps v1{k1},v2,[rcx]{sint16}
	vmaddn132ps v1{k1},v2,v3
	vmaddn132ps v1{k1},v2,v3{cdab}
	vmaddn132ps v1{k1},v2,v3{badc}
	vmaddn132ps v1{k1},v2,v3{aaaa}
	vmaddn132ps v1{k1},v2,v3{bbbb}
	vmaddn132ps v1{k1},v2,v3{cccc}
	vmaddn132ps v1{k1},v2,v3{dddd}
	vmaddn132ps v1{k1},v2,v3{dacb}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmaddn132ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmaddn132ps v1{k1},v2,[rcx]{sint16}
	vmaddn213ps v1{k1},v2,v3
	vmaddn213ps v1{k1},v2,v3{cdab}
	vmaddn213ps v1{k1},v2,v3{badc}
	vmaddn213ps v1{k1},v2,v3{aaaa}
	vmaddn213ps v1{k1},v2,v3{bbbb}
	vmaddn213ps v1{k1},v2,v3{cccc}
	vmaddn213ps v1{k1},v2,v3{dddd}
	vmaddn213ps v1{k1},v2,v3{dacb}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmaddn213ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmaddn213ps v1{k1},v2,[rcx]{sint16}
	vmaddn231ps v1{k1},v2,v3
	vmaddn231ps v1{k1},v2,v3{cdab}
	vmaddn231ps v1{k1},v2,v3{badc}
	vmaddn231ps v1{k1},v2,v3{aaaa}
	vmaddn231ps v1{k1},v2,v3{bbbb}
	vmaddn231ps v1{k1},v2,v3{cccc}
	vmaddn231ps v1{k1},v2,v3{dddd}
	vmaddn231ps v1{k1},v2,v3{dacb}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmaddn231ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmaddn231ps v1{k1},v2,[rcx]{sint16}
	vmaxabsps v1{k1},v2,v3
	vmaxabsps v1{k1},v2,v3{cdab}
	vmaxabsps v1{k1},v2,v3{badc}
	vmaxabsps v1{k1},v2,v3{aaaa}
	vmaxabsps v1{k1},v2,v3{bbbb}
	vmaxabsps v1{k1},v2,v3{cccc}
	vmaxabsps v1{k1},v2,v3{dddd}
	vmaxabsps v1{k1},v2,v3{dacb}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmaxabsps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmaxabsps v1{k1},v2,[rcx]{sint16}
	vmaxps v1{k1},v2,v3
	vmaxps v1{k1},v2,v3{cdab}
	vmaxps v1{k1},v2,v3{badc}
	vmaxps v1{k1},v2,v3{aaaa}
	vmaxps v1{k1},v2,v3{bbbb}
	vmaxps v1{k1},v2,v3{cccc}
	vmaxps v1{k1},v2,v3{dddd}
	vmaxps v1{k1},v2,v3{dacb}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmaxps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmaxps v1{k1},v2,[rcx]{sint16}
	vminps v1{k1},v2,v3
	vminps v1{k1},v2,v3{cdab}
	vminps v1{k1},v2,v3{badc}
	vminps v1{k1},v2,v3{aaaa}
	vminps v1{k1},v2,v3{bbbb}
	vminps v1{k1},v2,v3{cccc}
	vminps v1{k1},v2,v3{dddd}
	vminps v1{k1},v2,v3{dacb}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vminps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vminps v1{k1},v2,[rcx]{sint16}
	vmsub132ps v1{k1},v2,v3
	vmsub132ps v1{k1},v2,v3{cdab}
	vmsub132ps v1{k1},v2,v3{badc}
	vmsub132ps v1{k1},v2,v3{aaaa}
	vmsub132ps v1{k1},v2,v3{bbbb}
	vmsub132ps v1{k1},v2,v3{cccc}
	vmsub132ps v1{k1},v2,v3{dddd}
	vmsub132ps v1{k1},v2,v3{dacb}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmsub132ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmsub132ps v1{k1},v2,[rcx]{sint16}
	vmsub213ps v1{k1},v2,v3
	vmsub213ps v1{k1},v2,v3{cdab}
	vmsub213ps v1{k1},v2,v3{badc}
	vmsub213ps v1{k1},v2,v3{aaaa}
	vmsub213ps v1{k1},v2,v3{bbbb}
	vmsub213ps v1{k1},v2,v3{cccc}
	vmsub213ps v1{k1},v2,v3{dddd}
	vmsub213ps v1{k1},v2,v3{dacb}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmsub213ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmsub213ps v1{k1},v2,[rcx]{sint16}
	vmsub231ps v1{k1},v2,v3
	vmsub231ps v1{k1},v2,v3{cdab}
	vmsub231ps v1{k1},v2,v3{badc}
	vmsub231ps v1{k1},v2,v3{aaaa}
	vmsub231ps v1{k1},v2,v3{bbbb}
	vmsub231ps v1{k1},v2,v3{cccc}
	vmsub231ps v1{k1},v2,v3{dddd}
	vmsub231ps v1{k1},v2,v3{dacb}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmsub231ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmsub231ps v1{k1},v2,[rcx]{sint16}
	vmsubr132ps v1{k1},v2,v3
	vmsubr132ps v1{k1},v2,v3{cdab}
	vmsubr132ps v1{k1},v2,v3{badc}
	vmsubr132ps v1{k1},v2,v3{aaaa}
	vmsubr132ps v1{k1},v2,v3{bbbb}
	vmsubr132ps v1{k1},v2,v3{cccc}
	vmsubr132ps v1{k1},v2,v3{dddd}
	vmsubr132ps v1{k1},v2,v3{dacb}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmsubr132ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmsubr132ps v1{k1},v2,[rcx]{sint16}
	vmsubr213ps v1{k1},v2,v3
	vmsubr213ps v1{k1},v2,v3{cdab}
	vmsubr213ps v1{k1},v2,v3{badc}
	vmsubr213ps v1{k1},v2,v3{aaaa}
	vmsubr213ps v1{k1},v2,v3{bbbb}
	vmsubr213ps v1{k1},v2,v3{cccc}
	vmsubr213ps v1{k1},v2,v3{dddd}
	vmsubr213ps v1{k1},v2,v3{dacb}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmsubr213ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmsubr213ps v1{k1},v2,[rcx]{sint16}
	vmsubr231ps v1{k1},v2,v3
	vmsubr231ps v1{k1},v2,v3{cdab}
	vmsubr231ps v1{k1},v2,v3{badc}
	vmsubr231ps v1{k1},v2,v3{aaaa}
	vmsubr231ps v1{k1},v2,v3{bbbb}
	vmsubr231ps v1{k1},v2,v3{cccc}
	vmsubr231ps v1{k1},v2,v3{dddd}
	vmsubr231ps v1{k1},v2,v3{dacb}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmsubr231ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmsubr231ps v1{k1},v2,[rcx]{sint16}
	vmsubr23c1ps v1{k1},v2,v3
	vmsubr23c1ps v1{k1},v2,v3{cdab}
	vmsubr23c1ps v1{k1},v2,v3{badc}
	vmsubr23c1ps v1{k1},v2,v3{aaaa}
	vmsubr23c1ps v1{k1},v2,v3{bbbb}
	vmsubr23c1ps v1{k1},v2,v3{cccc}
	vmsubr23c1ps v1{k1},v2,v3{dddd}
	vmsubr23c1ps v1{k1},v2,v3{dacb}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmsubr23c1ps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmsubr23c1ps v1{k1},v2,[rcx]{sint16}
	vmulps v1{k1},v2,v3
	vmulps v1{k1},v2,v3{cdab}
	vmulps v1{k1},v2,v3{badc}
	vmulps v1{k1},v2,v3{aaaa}
	vmulps v1{k1},v2,v3{bbbb}
	vmulps v1{k1},v2,v3{cccc}
	vmulps v1{k1},v2,v3{dddd}
	vmulps v1{k1},v2,v3{dacb}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vmulps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmulps v1{k1},v2,[rcx]{sint16}
	vrcprefineps v1{k1},v2,v3
	vrcprefineps v1{k1},v2,v3{cdab}
	vrcprefineps v1{k1},v2,v3{badc}
	vrcprefineps v1{k1},v2,v3{aaaa}
	vrcprefineps v1{k1},v2,v3{bbbb}
	vrcprefineps v1{k1},v2,v3{cccc}
	vrcprefineps v1{k1},v2,v3{dddd}
	vrcprefineps v1{k1},v2,v3{dacb}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vrcprefineps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vrcprefineps v1{k1},v2,[rcx]{sint16}
	vsubps v1{k1},v2,v3
	vsubps v1{k1},v2,v3{cdab}
	vsubps v1{k1},v2,v3{badc}
	vsubps v1{k1},v2,v3{aaaa}
	vsubps v1{k1},v2,v3{bbbb}
	vsubps v1{k1},v2,v3{cccc}
	vsubps v1{k1},v2,v3{dddd}
	vsubps v1{k1},v2,v3{dacb}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vsubps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vsubps v1{k1},v2,[rcx]{sint16}
	vsubrps v1{k1},v2,v3
	vsubrps v1{k1},v2,v3{cdab}
	vsubrps v1{k1},v2,v3{badc}
	vsubrps v1{k1},v2,v3{aaaa}
	vsubrps v1{k1},v2,v3{bbbb}
	vsubrps v1{k1},v2,v3{cccc}
	vsubrps v1{k1},v2,v3{dddd}
	vsubrps v1{k1},v2,v3{dacb}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{unorm8}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{float16}
	vsubrps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vsubrps v1{k1},v2,[rcx]{sint16}

# Tests for op v1{k1}, v2, sd(v3/mt)
	vaddnpd v1{k1},v2,v3
	vaddnpd v1{k1},v2,v3{cdab}
	vaddnpd v1{k1},v2,v3{badc}
	vaddnpd v1{k1},v2,v3{aaaa}
	vaddnpd v1{k1},v2,v3{bbbb}
	vaddnpd v1{k1},v2,v3{cccc}
	vaddnpd v1{k1},v2,v3{dddd}
	vaddnpd v1{k1},v2,v3{dacb}
	vaddnpd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vaddnpd v1{k1},v2,ZMMWORD PTR [rcx]
	vaddnpd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vaddnpd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vaddnpd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vaddnpd v1{k1},v2,[rcx]{4to8}
	vaddpd v1{k1},v2,v3
	vaddpd v1{k1},v2,v3{cdab}
	vaddpd v1{k1},v2,v3{badc}
	vaddpd v1{k1},v2,v3{aaaa}
	vaddpd v1{k1},v2,v3{bbbb}
	vaddpd v1{k1},v2,v3{cccc}
	vaddpd v1{k1},v2,v3{dddd}
	vaddpd v1{k1},v2,v3{dacb}
	vaddpd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vaddpd v1{k1},v2,ZMMWORD PTR [rcx]
	vaddpd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vaddpd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vaddpd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vaddpd v1{k1},v2,[rcx]{4to8}
	vmadd132pd v1{k1},v2,v3
	vmadd132pd v1{k1},v2,v3{cdab}
	vmadd132pd v1{k1},v2,v3{badc}
	vmadd132pd v1{k1},v2,v3{aaaa}
	vmadd132pd v1{k1},v2,v3{bbbb}
	vmadd132pd v1{k1},v2,v3{cccc}
	vmadd132pd v1{k1},v2,v3{dddd}
	vmadd132pd v1{k1},v2,v3{dacb}
	vmadd132pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd132pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd132pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmadd132pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmadd132pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmadd132pd v1{k1},v2,[rcx]{4to8}
	vmadd213pd v1{k1},v2,v3
	vmadd213pd v1{k1},v2,v3{cdab}
	vmadd213pd v1{k1},v2,v3{badc}
	vmadd213pd v1{k1},v2,v3{aaaa}
	vmadd213pd v1{k1},v2,v3{bbbb}
	vmadd213pd v1{k1},v2,v3{cccc}
	vmadd213pd v1{k1},v2,v3{dddd}
	vmadd213pd v1{k1},v2,v3{dacb}
	vmadd213pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd213pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd213pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmadd213pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmadd213pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmadd213pd v1{k1},v2,[rcx]{4to8}
	vmadd231pd v1{k1},v2,v3
	vmadd231pd v1{k1},v2,v3{cdab}
	vmadd231pd v1{k1},v2,v3{badc}
	vmadd231pd v1{k1},v2,v3{aaaa}
	vmadd231pd v1{k1},v2,v3{bbbb}
	vmadd231pd v1{k1},v2,v3{cccc}
	vmadd231pd v1{k1},v2,v3{dddd}
	vmadd231pd v1{k1},v2,v3{dacb}
	vmadd231pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd231pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd231pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmadd231pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmadd231pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmadd231pd v1{k1},v2,[rcx]{4to8}
	vmaddn132pd v1{k1},v2,v3
	vmaddn132pd v1{k1},v2,v3{cdab}
	vmaddn132pd v1{k1},v2,v3{badc}
	vmaddn132pd v1{k1},v2,v3{aaaa}
	vmaddn132pd v1{k1},v2,v3{bbbb}
	vmaddn132pd v1{k1},v2,v3{cccc}
	vmaddn132pd v1{k1},v2,v3{dddd}
	vmaddn132pd v1{k1},v2,v3{dacb}
	vmaddn132pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaddn132pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmaddn132pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmaddn132pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmaddn132pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmaddn132pd v1{k1},v2,[rcx]{4to8}
	vmaddn213pd v1{k1},v2,v3
	vmaddn213pd v1{k1},v2,v3{cdab}
	vmaddn213pd v1{k1},v2,v3{badc}
	vmaddn213pd v1{k1},v2,v3{aaaa}
	vmaddn213pd v1{k1},v2,v3{bbbb}
	vmaddn213pd v1{k1},v2,v3{cccc}
	vmaddn213pd v1{k1},v2,v3{dddd}
	vmaddn213pd v1{k1},v2,v3{dacb}
	vmaddn213pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaddn213pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmaddn213pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmaddn213pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmaddn213pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmaddn213pd v1{k1},v2,[rcx]{4to8}
	vmaddn231pd v1{k1},v2,v3
	vmaddn231pd v1{k1},v2,v3{cdab}
	vmaddn231pd v1{k1},v2,v3{badc}
	vmaddn231pd v1{k1},v2,v3{aaaa}
	vmaddn231pd v1{k1},v2,v3{bbbb}
	vmaddn231pd v1{k1},v2,v3{cccc}
	vmaddn231pd v1{k1},v2,v3{dddd}
	vmaddn231pd v1{k1},v2,v3{dacb}
	vmaddn231pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaddn231pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmaddn231pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmaddn231pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmaddn231pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmaddn231pd v1{k1},v2,[rcx]{4to8}
	vmaxpd v1{k1},v2,v3
	vmaxpd v1{k1},v2,v3{cdab}
	vmaxpd v1{k1},v2,v3{badc}
	vmaxpd v1{k1},v2,v3{aaaa}
	vmaxpd v1{k1},v2,v3{bbbb}
	vmaxpd v1{k1},v2,v3{cccc}
	vmaxpd v1{k1},v2,v3{dddd}
	vmaxpd v1{k1},v2,v3{dacb}
	vmaxpd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaxpd v1{k1},v2,ZMMWORD PTR [rcx]
	vmaxpd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmaxpd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmaxpd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmaxpd v1{k1},v2,[rcx]{4to8}
	vminpd v1{k1},v2,v3
	vminpd v1{k1},v2,v3{cdab}
	vminpd v1{k1},v2,v3{badc}
	vminpd v1{k1},v2,v3{aaaa}
	vminpd v1{k1},v2,v3{bbbb}
	vminpd v1{k1},v2,v3{cccc}
	vminpd v1{k1},v2,v3{dddd}
	vminpd v1{k1},v2,v3{dacb}
	vminpd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vminpd v1{k1},v2,ZMMWORD PTR [rcx]
	vminpd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vminpd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vminpd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vminpd v1{k1},v2,[rcx]{4to8}
	vmsub132pd v1{k1},v2,v3
	vmsub132pd v1{k1},v2,v3{cdab}
	vmsub132pd v1{k1},v2,v3{badc}
	vmsub132pd v1{k1},v2,v3{aaaa}
	vmsub132pd v1{k1},v2,v3{bbbb}
	vmsub132pd v1{k1},v2,v3{cccc}
	vmsub132pd v1{k1},v2,v3{dddd}
	vmsub132pd v1{k1},v2,v3{dacb}
	vmsub132pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsub132pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmsub132pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmsub132pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmsub132pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmsub132pd v1{k1},v2,[rcx]{4to8}
	vmsub213pd v1{k1},v2,v3
	vmsub213pd v1{k1},v2,v3{cdab}
	vmsub213pd v1{k1},v2,v3{badc}
	vmsub213pd v1{k1},v2,v3{aaaa}
	vmsub213pd v1{k1},v2,v3{bbbb}
	vmsub213pd v1{k1},v2,v3{cccc}
	vmsub213pd v1{k1},v2,v3{dddd}
	vmsub213pd v1{k1},v2,v3{dacb}
	vmsub213pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsub213pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmsub213pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmsub213pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmsub213pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmsub213pd v1{k1},v2,[rcx]{4to8}
	vmsub231pd v1{k1},v2,v3
	vmsub231pd v1{k1},v2,v3{cdab}
	vmsub231pd v1{k1},v2,v3{badc}
	vmsub231pd v1{k1},v2,v3{aaaa}
	vmsub231pd v1{k1},v2,v3{bbbb}
	vmsub231pd v1{k1},v2,v3{cccc}
	vmsub231pd v1{k1},v2,v3{dddd}
	vmsub231pd v1{k1},v2,v3{dacb}
	vmsub231pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsub231pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmsub231pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmsub231pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmsub231pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmsub231pd v1{k1},v2,[rcx]{4to8}
	vmsubr132pd v1{k1},v2,v3
	vmsubr132pd v1{k1},v2,v3{cdab}
	vmsubr132pd v1{k1},v2,v3{badc}
	vmsubr132pd v1{k1},v2,v3{aaaa}
	vmsubr132pd v1{k1},v2,v3{bbbb}
	vmsubr132pd v1{k1},v2,v3{cccc}
	vmsubr132pd v1{k1},v2,v3{dddd}
	vmsubr132pd v1{k1},v2,v3{dacb}
	vmsubr132pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr132pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr132pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmsubr132pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmsubr132pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmsubr132pd v1{k1},v2,[rcx]{4to8}
	vmsubr213pd v1{k1},v2,v3
	vmsubr213pd v1{k1},v2,v3{cdab}
	vmsubr213pd v1{k1},v2,v3{badc}
	vmsubr213pd v1{k1},v2,v3{aaaa}
	vmsubr213pd v1{k1},v2,v3{bbbb}
	vmsubr213pd v1{k1},v2,v3{cccc}
	vmsubr213pd v1{k1},v2,v3{dddd}
	vmsubr213pd v1{k1},v2,v3{dacb}
	vmsubr213pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr213pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr213pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmsubr213pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmsubr213pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmsubr213pd v1{k1},v2,[rcx]{4to8}
	vmsubr231pd v1{k1},v2,v3
	vmsubr231pd v1{k1},v2,v3{cdab}
	vmsubr231pd v1{k1},v2,v3{badc}
	vmsubr231pd v1{k1},v2,v3{aaaa}
	vmsubr231pd v1{k1},v2,v3{bbbb}
	vmsubr231pd v1{k1},v2,v3{cccc}
	vmsubr231pd v1{k1},v2,v3{dddd}
	vmsubr231pd v1{k1},v2,v3{dacb}
	vmsubr231pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr231pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr231pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmsubr231pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmsubr231pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmsubr231pd v1{k1},v2,[rcx]{4to8}
	vmsubr23c1pd v1{k1},v2,v3
	vmsubr23c1pd v1{k1},v2,v3{cdab}
	vmsubr23c1pd v1{k1},v2,v3{badc}
	vmsubr23c1pd v1{k1},v2,v3{aaaa}
	vmsubr23c1pd v1{k1},v2,v3{bbbb}
	vmsubr23c1pd v1{k1},v2,v3{cccc}
	vmsubr23c1pd v1{k1},v2,v3{dddd}
	vmsubr23c1pd v1{k1},v2,v3{dacb}
	vmsubr23c1pd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmsubr23c1pd v1{k1},v2,ZMMWORD PTR [rcx]
	vmsubr23c1pd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmsubr23c1pd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmsubr23c1pd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmsubr23c1pd v1{k1},v2,[rcx]{4to8}
	vmulpd v1{k1},v2,v3
	vmulpd v1{k1},v2,v3{cdab}
	vmulpd v1{k1},v2,v3{badc}
	vmulpd v1{k1},v2,v3{aaaa}
	vmulpd v1{k1},v2,v3{bbbb}
	vmulpd v1{k1},v2,v3{cccc}
	vmulpd v1{k1},v2,v3{dddd}
	vmulpd v1{k1},v2,v3{dacb}
	vmulpd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmulpd v1{k1},v2,ZMMWORD PTR [rcx]
	vmulpd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vmulpd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vmulpd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vmulpd v1{k1},v2,[rcx]{4to8}
	vsubpd v1{k1},v2,v3
	vsubpd v1{k1},v2,v3{cdab}
	vsubpd v1{k1},v2,v3{badc}
	vsubpd v1{k1},v2,v3{aaaa}
	vsubpd v1{k1},v2,v3{bbbb}
	vsubpd v1{k1},v2,v3{cccc}
	vsubpd v1{k1},v2,v3{dddd}
	vsubpd v1{k1},v2,v3{dacb}
	vsubpd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsubpd v1{k1},v2,ZMMWORD PTR [rcx]
	vsubpd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vsubpd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vsubpd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vsubpd v1{k1},v2,[rcx]{4to8}
	vsubrpd v1{k1},v2,v3
	vsubrpd v1{k1},v2,v3{cdab}
	vsubrpd v1{k1},v2,v3{badc}
	vsubrpd v1{k1},v2,v3{aaaa}
	vsubrpd v1{k1},v2,v3{bbbb}
	vsubrpd v1{k1},v2,v3{cccc}
	vsubrpd v1{k1},v2,v3{dddd}
	vsubrpd v1{k1},v2,v3{dacb}
	vsubrpd v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsubrpd v1{k1},v2,ZMMWORD PTR [rcx]
	vsubrpd v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vsubrpd v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vsubrpd v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vsubrpd v1{k1},v2,[rcx]{4to8}

# Tests for op v1{k1}, v2, si(v3/mt)
	vaddpi v1{k1},v2,v3
	vaddpi v1{k1},v2,v3{cdab}
	vaddpi v1{k1},v2,v3{badc}
	vaddpi v1{k1},v2,v3{aaaa}
	vaddpi v1{k1},v2,v3{bbbb}
	vaddpi v1{k1},v2,v3{cccc}
	vaddpi v1{k1},v2,v3{dddd}
	vaddpi v1{k1},v2,v3{dacb}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vaddpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vaddpi v1{k1},v2,[rcx]{sint16}
	vaddsetspi v1{k1},v2,v3
	vaddsetspi v1{k1},v2,v3{cdab}
	vaddsetspi v1{k1},v2,v3{badc}
	vaddsetspi v1{k1},v2,v3{aaaa}
	vaddsetspi v1{k1},v2,v3{bbbb}
	vaddsetspi v1{k1},v2,v3{cccc}
	vaddsetspi v1{k1},v2,v3{dddd}
	vaddsetspi v1{k1},v2,v3{dacb}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vaddsetspi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vaddsetspi v1{k1},v2,[rcx]{sint16}
	vandnpi v1{k1},v2,v3
	vandnpi v1{k1},v2,v3{cdab}
	vandnpi v1{k1},v2,v3{badc}
	vandnpi v1{k1},v2,v3{aaaa}
	vandnpi v1{k1},v2,v3{bbbb}
	vandnpi v1{k1},v2,v3{cccc}
	vandnpi v1{k1},v2,v3{dddd}
	vandnpi v1{k1},v2,v3{dacb}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vandnpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vandnpi v1{k1},v2,[rcx]{sint16}
	vandpi v1{k1},v2,v3
	vandpi v1{k1},v2,v3{cdab}
	vandpi v1{k1},v2,v3{badc}
	vandpi v1{k1},v2,v3{aaaa}
	vandpi v1{k1},v2,v3{bbbb}
	vandpi v1{k1},v2,v3{cccc}
	vandpi v1{k1},v2,v3{dddd}
	vandpi v1{k1},v2,v3{dacb}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vandpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vandpi v1{k1},v2,[rcx]{sint16}
	vbitinterleave11pi v1{k1},v2,v3
	vbitinterleave11pi v1{k1},v2,v3{cdab}
	vbitinterleave11pi v1{k1},v2,v3{badc}
	vbitinterleave11pi v1{k1},v2,v3{aaaa}
	vbitinterleave11pi v1{k1},v2,v3{bbbb}
	vbitinterleave11pi v1{k1},v2,v3{cccc}
	vbitinterleave11pi v1{k1},v2,v3{dddd}
	vbitinterleave11pi v1{k1},v2,v3{dacb}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vbitinterleave11pi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vbitinterleave11pi v1{k1},v2,[rcx]{sint16}
	vbitinterleave21pi v1{k1},v2,v3
	vbitinterleave21pi v1{k1},v2,v3{cdab}
	vbitinterleave21pi v1{k1},v2,v3{badc}
	vbitinterleave21pi v1{k1},v2,v3{aaaa}
	vbitinterleave21pi v1{k1},v2,v3{bbbb}
	vbitinterleave21pi v1{k1},v2,v3{cccc}
	vbitinterleave21pi v1{k1},v2,v3{dddd}
	vbitinterleave21pi v1{k1},v2,v3{dacb}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vbitinterleave21pi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vbitinterleave21pi v1{k1},v2,[rcx]{sint16}
	vclampzpi v1{k1},v2,v3
	vclampzpi v1{k1},v2,v3{cdab}
	vclampzpi v1{k1},v2,v3{badc}
	vclampzpi v1{k1},v2,v3{aaaa}
	vclampzpi v1{k1},v2,v3{bbbb}
	vclampzpi v1{k1},v2,v3{cccc}
	vclampzpi v1{k1},v2,v3{dddd}
	vclampzpi v1{k1},v2,v3{dacb}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vclampzpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vclampzpi v1{k1},v2,[rcx]{sint16}
	vmadd231pi v1{k1},v2,v3
	vmadd231pi v1{k1},v2,v3{cdab}
	vmadd231pi v1{k1},v2,v3{badc}
	vmadd231pi v1{k1},v2,v3{aaaa}
	vmadd231pi v1{k1},v2,v3{bbbb}
	vmadd231pi v1{k1},v2,v3{cccc}
	vmadd231pi v1{k1},v2,v3{dddd}
	vmadd231pi v1{k1},v2,v3{dacb}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vmadd231pi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmadd231pi v1{k1},v2,[rcx]{sint16}
	vmadd233pi v1{k1},v2,v3
	vmadd233pi v1{k1},v2,v3{cdab}
	vmadd233pi v1{k1},v2,v3{badc}
	vmadd233pi v1{k1},v2,v3{aaaa}
	vmadd233pi v1{k1},v2,v3{bbbb}
	vmadd233pi v1{k1},v2,v3{cccc}
	vmadd233pi v1{k1},v2,v3{dddd}
	vmadd233pi v1{k1},v2,v3{dacb}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vmadd233pi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmadd233pi v1{k1},v2,[rcx]{sint16}
	vmaxpi v1{k1},v2,v3
	vmaxpi v1{k1},v2,v3{cdab}
	vmaxpi v1{k1},v2,v3{badc}
	vmaxpi v1{k1},v2,v3{aaaa}
	vmaxpi v1{k1},v2,v3{bbbb}
	vmaxpi v1{k1},v2,v3{cccc}
	vmaxpi v1{k1},v2,v3{dddd}
	vmaxpi v1{k1},v2,v3{dacb}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vmaxpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmaxpi v1{k1},v2,[rcx]{sint16}
	vmaxpu v1{k1},v2,v3
	vmaxpu v1{k1},v2,v3{cdab}
	vmaxpu v1{k1},v2,v3{badc}
	vmaxpu v1{k1},v2,v3{aaaa}
	vmaxpu v1{k1},v2,v3{bbbb}
	vmaxpu v1{k1},v2,v3{cccc}
	vmaxpu v1{k1},v2,v3{dddd}
	vmaxpu v1{k1},v2,v3{dacb}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vmaxpu v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmaxpu v1{k1},v2,[rcx]{sint16}
	vminpi v1{k1},v2,v3
	vminpi v1{k1},v2,v3{cdab}
	vminpi v1{k1},v2,v3{badc}
	vminpi v1{k1},v2,v3{aaaa}
	vminpi v1{k1},v2,v3{bbbb}
	vminpi v1{k1},v2,v3{cccc}
	vminpi v1{k1},v2,v3{dddd}
	vminpi v1{k1},v2,v3{dacb}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vminpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vminpi v1{k1},v2,[rcx]{sint16}
	vminpu v1{k1},v2,v3
	vminpu v1{k1},v2,v3{cdab}
	vminpu v1{k1},v2,v3{badc}
	vminpu v1{k1},v2,v3{aaaa}
	vminpu v1{k1},v2,v3{bbbb}
	vminpu v1{k1},v2,v3{cccc}
	vminpu v1{k1},v2,v3{dddd}
	vminpu v1{k1},v2,v3{dacb}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vminpu v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vminpu v1{k1},v2,[rcx]{sint16}
	vmulhpi v1{k1},v2,v3
	vmulhpi v1{k1},v2,v3{cdab}
	vmulhpi v1{k1},v2,v3{badc}
	vmulhpi v1{k1},v2,v3{aaaa}
	vmulhpi v1{k1},v2,v3{bbbb}
	vmulhpi v1{k1},v2,v3{cccc}
	vmulhpi v1{k1},v2,v3{dddd}
	vmulhpi v1{k1},v2,v3{dacb}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vmulhpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmulhpi v1{k1},v2,[rcx]{sint16}
	vmulhpu v1{k1},v2,v3
	vmulhpu v1{k1},v2,v3{cdab}
	vmulhpu v1{k1},v2,v3{badc}
	vmulhpu v1{k1},v2,v3{aaaa}
	vmulhpu v1{k1},v2,v3{bbbb}
	vmulhpu v1{k1},v2,v3{cccc}
	vmulhpu v1{k1},v2,v3{dddd}
	vmulhpu v1{k1},v2,v3{dacb}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vmulhpu v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmulhpu v1{k1},v2,[rcx]{sint16}
	vmullpi v1{k1},v2,v3
	vmullpi v1{k1},v2,v3{cdab}
	vmullpi v1{k1},v2,v3{badc}
	vmullpi v1{k1},v2,v3{aaaa}
	vmullpi v1{k1},v2,v3{bbbb}
	vmullpi v1{k1},v2,v3{cccc}
	vmullpi v1{k1},v2,v3{dddd}
	vmullpi v1{k1},v2,v3{dacb}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vmullpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vmullpi v1{k1},v2,[rcx]{sint16}
	vorpi v1{k1},v2,v3
	vorpi v1{k1},v2,v3{cdab}
	vorpi v1{k1},v2,v3{badc}
	vorpi v1{k1},v2,v3{aaaa}
	vorpi v1{k1},v2,v3{bbbb}
	vorpi v1{k1},v2,v3{cccc}
	vorpi v1{k1},v2,v3{dddd}
	vorpi v1{k1},v2,v3{dacb}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vorpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vorpi v1{k1},v2,[rcx]{sint16}
	vscaleps v1{k1},v2,v3
	vscaleps v1{k1},v2,v3{cdab}
	vscaleps v1{k1},v2,v3{badc}
	vscaleps v1{k1},v2,v3{aaaa}
	vscaleps v1{k1},v2,v3{bbbb}
	vscaleps v1{k1},v2,v3{cccc}
	vscaleps v1{k1},v2,v3{dddd}
	vscaleps v1{k1},v2,v3{dacb}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vscaleps v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vscaleps v1{k1},v2,[rcx]{sint16}
	vsllpi v1{k1},v2,v3
	vsllpi v1{k1},v2,v3{cdab}
	vsllpi v1{k1},v2,v3{badc}
	vsllpi v1{k1},v2,v3{aaaa}
	vsllpi v1{k1},v2,v3{bbbb}
	vsllpi v1{k1},v2,v3{cccc}
	vsllpi v1{k1},v2,v3{dddd}
	vsllpi v1{k1},v2,v3{dacb}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vsllpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vsllpi v1{k1},v2,[rcx]{sint16}
	vsrapi v1{k1},v2,v3
	vsrapi v1{k1},v2,v3{cdab}
	vsrapi v1{k1},v2,v3{badc}
	vsrapi v1{k1},v2,v3{aaaa}
	vsrapi v1{k1},v2,v3{bbbb}
	vsrapi v1{k1},v2,v3{cccc}
	vsrapi v1{k1},v2,v3{dddd}
	vsrapi v1{k1},v2,v3{dacb}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vsrapi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vsrapi v1{k1},v2,[rcx]{sint16}
	vsrlpi v1{k1},v2,v3
	vsrlpi v1{k1},v2,v3{cdab}
	vsrlpi v1{k1},v2,v3{badc}
	vsrlpi v1{k1},v2,v3{aaaa}
	vsrlpi v1{k1},v2,v3{bbbb}
	vsrlpi v1{k1},v2,v3{cccc}
	vsrlpi v1{k1},v2,v3{dddd}
	vsrlpi v1{k1},v2,v3{dacb}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vsrlpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vsrlpi v1{k1},v2,[rcx]{sint16}
	vsubpi v1{k1},v2,v3
	vsubpi v1{k1},v2,v3{cdab}
	vsubpi v1{k1},v2,v3{badc}
	vsubpi v1{k1},v2,v3{aaaa}
	vsubpi v1{k1},v2,v3{bbbb}
	vsubpi v1{k1},v2,v3{cccc}
	vsubpi v1{k1},v2,v3{dddd}
	vsubpi v1{k1},v2,v3{dacb}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vsubpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vsubpi v1{k1},v2,[rcx]{sint16}
	vsubrpi v1{k1},v2,v3
	vsubrpi v1{k1},v2,v3{cdab}
	vsubrpi v1{k1},v2,v3{badc}
	vsubrpi v1{k1},v2,v3{aaaa}
	vsubrpi v1{k1},v2,v3{bbbb}
	vsubrpi v1{k1},v2,v3{cccc}
	vsubrpi v1{k1},v2,v3{dddd}
	vsubrpi v1{k1},v2,v3{dacb}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vsubrpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vsubrpi v1{k1},v2,[rcx]{sint16}
	vxorpi v1{k1},v2,v3
	vxorpi v1{k1},v2,v3{cdab}
	vxorpi v1{k1},v2,v3{badc}
	vxorpi v1{k1},v2,v3{aaaa}
	vxorpi v1{k1},v2,v3{bbbb}
	vxorpi v1{k1},v2,v3{cccc}
	vxorpi v1{k1},v2,v3{dddd}
	vxorpi v1{k1},v2,v3{dacb}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16}
	vxorpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16}
	vxorpi v1{k1},v2,[rcx]{sint16}

# Tests for op k2{k1}, v2, si(v3/mt)
	vtestpi k1{k2},v2,v3
	vtestpi k1{k2},v2,v3{cdab}
	vtestpi k1{k2},v2,v3{badc}
	vtestpi k1{k2},v2,v3{aaaa}
	vtestpi k1{k2},v2,v3{bbbb}
	vtestpi k1{k2},v2,v3{cccc}
	vtestpi k1{k2},v2,v3{dddd}
	vtestpi k1{k2},v2,v3{dacb}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{nt}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{16to16}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{1to16}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{4to16}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{uint8}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{sint8}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{uint16}
	vtestpi k1{k2},v2,ZMMWORD PTR [rcx]{sint16}
	vtestpi k1{k2},v2,[rcx]{sint16}

# Tests for op v1{k1}, v2, sq(v3/mt)
	vandnpq v1{k1},v2,v3
	vandnpq v1{k1},v2,v3{cdab}
	vandnpq v1{k1},v2,v3{badc}
	vandnpq v1{k1},v2,v3{aaaa}
	vandnpq v1{k1},v2,v3{bbbb}
	vandnpq v1{k1},v2,v3{cccc}
	vandnpq v1{k1},v2,v3{dddd}
	vandnpq v1{k1},v2,v3{dacb}
	vandnpq v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vandnpq v1{k1},v2,ZMMWORD PTR [rcx]
	vandnpq v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vandnpq v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vandnpq v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vandnpq v1{k1},v2,[rcx]{4to8}
	vandpq v1{k1},v2,v3
	vandpq v1{k1},v2,v3{cdab}
	vandpq v1{k1},v2,v3{badc}
	vandpq v1{k1},v2,v3{aaaa}
	vandpq v1{k1},v2,v3{bbbb}
	vandpq v1{k1},v2,v3{cccc}
	vandpq v1{k1},v2,v3{dddd}
	vandpq v1{k1},v2,v3{dacb}
	vandpq v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vandpq v1{k1},v2,ZMMWORD PTR [rcx]
	vandpq v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vandpq v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vandpq v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vandpq v1{k1},v2,[rcx]{4to8}
	vorpq v1{k1},v2,v3
	vorpq v1{k1},v2,v3{cdab}
	vorpq v1{k1},v2,v3{badc}
	vorpq v1{k1},v2,v3{aaaa}
	vorpq v1{k1},v2,v3{bbbb}
	vorpq v1{k1},v2,v3{cccc}
	vorpq v1{k1},v2,v3{dddd}
	vorpq v1{k1},v2,v3{dacb}
	vorpq v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vorpq v1{k1},v2,ZMMWORD PTR [rcx]
	vorpq v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vorpq v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vorpq v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vorpq v1{k1},v2,[rcx]{4to8}
	vxorpq v1{k1},v2,v3
	vxorpq v1{k1},v2,v3{cdab}
	vxorpq v1{k1},v2,v3{badc}
	vxorpq v1{k1},v2,v3{aaaa}
	vxorpq v1{k1},v2,v3{bbbb}
	vxorpq v1{k1},v2,v3{cccc}
	vxorpq v1{k1},v2,v3{dddd}
	vxorpq v1{k1},v2,v3{dacb}
	vxorpq v1{k1},v2,ZMMWORD PTR [rcx]{nt}
	vxorpq v1{k1},v2,ZMMWORD PTR [rcx]
	vxorpq v1{k1},v2,ZMMWORD PTR [rcx]{8to8}
	vxorpq v1{k1},v2,ZMMWORD PTR [rcx]{1to8}
	vxorpq v1{k1},v2,ZMMWORD PTR [rcx]{4to8}
	vxorpq v1{k1},v2,[rcx]{4to8}

# Tests for op k2{k1}, v1, sd(v2/mt), cmppd
	vcmppd k1{k2},v2,v3,unord
	vcmppd k1{k2},v2,v3{cdab},unord
	vcmppd k1{k2},v2,v3{badc},unord
	vcmppd k1{k2},v2,v3{aaaa},unord
	vcmppd k1{k2},v2,v3{bbbb},unord
	vcmppd k1{k2},v2,v3{cccc},unord
	vcmppd k1{k2},v2,v3{dddd},unord
	vcmppd k1{k2},v2,v3{dacb},unord
	vcmppd k1{k2},v2,v3{dacb},eq
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},eq
	vcmppd k1{k2},v2,v3{dacb},lt
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},lt
	vcmppd k1{k2},v2,v3{dacb},le
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},le
	vcmppd k1{k2},v2,v3{dacb},unord
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},unord
	vcmppd k1{k2},v2,v3{dacb},neq
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},neq
	vcmppd k1{k2},v2,v3{dacb},nlt
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},nlt
	vcmppd k1{k2},v2,v3{dacb},nle
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},nle
	vcmppd k1{k2},v2,v3{dacb},ord
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{nt},ord
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx],unord
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{8to8},unord
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{1to8},unord
	vcmppd k1{k2},v2,ZMMWORD PTR [rcx]{4to8},unord
	vcmppd k1{k2},v2,[rcx]{4to8},unord

# Tests for op k2{k1}, v1, si(v2/mt), cmppi
	vcmppi k1{k2},v2,v3,lt
	vcmppi k1{k2},v2,v3{cdab},lt
	vcmppi k1{k2},v2,v3{badc},lt
	vcmppi k1{k2},v2,v3{aaaa},lt
	vcmppi k1{k2},v2,v3{bbbb},lt
	vcmppi k1{k2},v2,v3{cccc},lt
	vcmppi k1{k2},v2,v3{dddd},lt
	vcmppi k1{k2},v2,v3{dacb},lt
	vcmppi k1{k2},v2,v3{dacb},eq
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{nt},eq
	vcmppi k1{k2},v2,v3{dacb},lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{nt},lt
	vcmppi k1{k2},v2,v3{dacb},le
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{nt},le
	vcmppi k1{k2},v2,v3{dacb},neq
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{nt},neq
	vcmppi k1{k2},v2,v3{dacb},nlt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{nt},nlt
	vcmppi k1{k2},v2,v3{dacb},nle
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{nt},nle
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx],lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{16to16},lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{1to16},lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{4to16},lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{uint8},lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{sint8},lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{uint16},lt
	vcmppi k1{k2},v2,ZMMWORD PTR [rcx]{sint16},lt
	vcmppi k1{k2},v2,[rcx]{sint16},lt
	vcmppu k1{k2},v2,v3,lt
	vcmppu k1{k2},v2,v3{cdab},lt
	vcmppu k1{k2},v2,v3{badc},lt
	vcmppu k1{k2},v2,v3{aaaa},lt
	vcmppu k1{k2},v2,v3{bbbb},lt
	vcmppu k1{k2},v2,v3{cccc},lt
	vcmppu k1{k2},v2,v3{dddd},lt
	vcmppu k1{k2},v2,v3{dacb},lt
	vcmppu k1{k2},v2,v3{dacb},eq
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{nt},eq
	vcmppu k1{k2},v2,v3{dacb},lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{nt},lt
	vcmppu k1{k2},v2,v3{dacb},le
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{nt},le
	vcmppu k1{k2},v2,v3{dacb},neq
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{nt},neq
	vcmppu k1{k2},v2,v3{dacb},nlt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{nt},nlt
	vcmppu k1{k2},v2,v3{dacb},nle
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{nt},nle
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx],lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{16to16},lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{1to16},lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{4to16},lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{uint8},lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{sint8},lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{uint16},lt
	vcmppu k1{k2},v2,ZMMWORD PTR [rcx]{sint16},lt
	vcmppu k1{k2},v2,[rcx]{sint16},lt

# Tests for op k2{k1}, v1, sf(v2/mt), cmpps
	vcmpps k1{k2},v2,v3,unord
	vcmpps k1{k2},v2,v3{cdab},unord
	vcmpps k1{k2},v2,v3{badc},unord
	vcmpps k1{k2},v2,v3{aaaa},unord
	vcmpps k1{k2},v2,v3{bbbb},unord
	vcmpps k1{k2},v2,v3{cccc},unord
	vcmpps k1{k2},v2,v3{dddd},unord
	vcmpps k1{k2},v2,v3{dacb},unord
	vcmpps k1{k2},v2,v3{dacb},eq
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},eq
	vcmpps k1{k2},v2,v3{dacb},lt
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},lt
	vcmpps k1{k2},v2,v3{dacb},le
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},le
	vcmpps k1{k2},v2,v3{dacb},unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},unord
	vcmpps k1{k2},v2,v3{dacb},neq
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},neq
	vcmpps k1{k2},v2,v3{dacb},nlt
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},nlt
	vcmpps k1{k2},v2,v3{dacb},nle
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},nle
	vcmpps k1{k2},v2,v3{dacb},ord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{nt},ord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx],unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{16to16},unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{1to16},unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{4to16},unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{uint8},unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{unorm8},unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{float16},unord
	vcmpps k1{k2},v2,ZMMWORD PTR [rcx]{sint16},unord
	vcmpps k1{k2},v2,[rcx]{sint16},unord

# Tests for op v1{k1}, sf(v2/mt), rc1, field
	vcvtinsps2f11 v1{k1},v3,rn,2
	vcvtinsps2f11 v1{k1},v3{cdab},rn,2
	vcvtinsps2f11 v1{k1},v3{badc},rn,2
	vcvtinsps2f11 v1{k1},v3{aaaa},rn,2
	vcvtinsps2f11 v1{k1},v3{bbbb},rn,2
	vcvtinsps2f11 v1{k1},v3{cccc},rn,2
	vcvtinsps2f11 v1{k1},v3{dddd},rn,2
	vcvtinsps2f11 v1{k1},v3{dacb},rn,2
	vcvtinsps2f11 v1{k1},v3{dacb},rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{nt},rn,2
	vcvtinsps2f11 v1{k1},v3{dacb},rz,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{nt},rz,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx],rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{16to16},rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{1to16},rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{4to16},rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{uint8},rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{unorm8},rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{float16},rn,2
	vcvtinsps2f11 v1{k1},ZMMWORD PTR [rcx]{sint16},rn,2
	vcvtinsps2f11 v1{k1},[rcx]{sint16},rn,2

# Tests for op v1{k1}, sf(v2/mt), field
	vcvtinsps2u10 v1{k1},v3,2
	vcvtinsps2u10 v1{k1},v3{cdab},2
	vcvtinsps2u10 v1{k1},v3{badc},2
	vcvtinsps2u10 v1{k1},v3{aaaa},2
	vcvtinsps2u10 v1{k1},v3{bbbb},2
	vcvtinsps2u10 v1{k1},v3{cccc},2
	vcvtinsps2u10 v1{k1},v3{dddd},2
	vcvtinsps2u10 v1{k1},v3{dacb},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{nt},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx],2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{16to16},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{1to16},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{4to16},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{uint8},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{unorm8},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{float16},2
	vcvtinsps2u10 v1{k1},ZMMWORD PTR [rcx]{sint16},2
	vcvtinsps2u10 v1{k1},[rcx]{sint16},2

# Tests for op v1{k1}, sd(v2/mt), rc2, half
	vcvtpd2pi v1{k1},v3,rn,1
	vcvtpd2pi v1{k1},v3{cdab},rn,1
	vcvtpd2pi v1{k1},v3{badc},rn,1
	vcvtpd2pi v1{k1},v3{aaaa},rn,1
	vcvtpd2pi v1{k1},v3{bbbb},rn,1
	vcvtpd2pi v1{k1},v3{cccc},rn,1
	vcvtpd2pi v1{k1},v3{dddd},rn,1
	vcvtpd2pi v1{k1},v3{dacb},rn,1
	vcvtpd2pi v1{k1},v3{dacb},rn,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx],rn,1
	vcvtpd2pi v1{k1},v3{dacb},rd,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx],rd,1
	vcvtpd2pi v1{k1},v3{dacb},ru,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx],ru,1
	vcvtpd2pi v1{k1},v3{dacb},rz,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx],rz,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx]{nt},rz,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx],rn,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx]{8to8},rn,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx]{1to8},rn,1
	vcvtpd2pi v1{k1},ZMMWORD PTR [rcx]{4to8},rn,1
	vcvtpd2pi v1{k1},[rcx]{4to8},rn,1
	vcvtpd2ps v1{k1},v3,rn,1
	vcvtpd2ps v1{k1},v3{cdab},rn,1
	vcvtpd2ps v1{k1},v3{badc},rn,1
	vcvtpd2ps v1{k1},v3{aaaa},rn,1
	vcvtpd2ps v1{k1},v3{bbbb},rn,1
	vcvtpd2ps v1{k1},v3{cccc},rn,1
	vcvtpd2ps v1{k1},v3{dddd},rn,1
	vcvtpd2ps v1{k1},v3{dacb},rn,1
	vcvtpd2ps v1{k1},v3{dacb},rn,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx],rn,1
	vcvtpd2ps v1{k1},v3{dacb},rd,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx],rd,1
	vcvtpd2ps v1{k1},v3{dacb},ru,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx],ru,1
	vcvtpd2ps v1{k1},v3{dacb},rz,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx],rz,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx]{nt},rz,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx],rn,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx]{8to8},rn,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx]{1to8},rn,1
	vcvtpd2ps v1{k1},ZMMWORD PTR [rcx]{4to8},rn,1
	vcvtpd2ps v1{k1},[rcx]{4to8},rn,1
	vcvtpd2pu v1{k1},v3,rn,1
	vcvtpd2pu v1{k1},v3{cdab},rn,1
	vcvtpd2pu v1{k1},v3{badc},rn,1
	vcvtpd2pu v1{k1},v3{aaaa},rn,1
	vcvtpd2pu v1{k1},v3{bbbb},rn,1
	vcvtpd2pu v1{k1},v3{cccc},rn,1
	vcvtpd2pu v1{k1},v3{dddd},rn,1
	vcvtpd2pu v1{k1},v3{dacb},rn,1
	vcvtpd2pu v1{k1},v3{dacb},rn,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx],rn,1
	vcvtpd2pu v1{k1},v3{dacb},rd,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx],rd,1
	vcvtpd2pu v1{k1},v3{dacb},ru,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx],ru,1
	vcvtpd2pu v1{k1},v3{dacb},rz,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx],rz,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx]{nt},rz,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx],rn,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx]{8to8},rn,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx]{1to8},rn,1
	vcvtpd2pu v1{k1},ZMMWORD PTR [rcx]{4to8},rn,1
	vcvtpd2pu v1{k1},[rcx]{4to8},rn,1

# Tests for op v1{k1}, si(v2/mt), half
	vcvtpi2pd v1{k1},v3,1
	vcvtpi2pd v1{k1},v3{cdab},1
	vcvtpi2pd v1{k1},v3{badc},1
	vcvtpi2pd v1{k1},v3{aaaa},1
	vcvtpi2pd v1{k1},v3{bbbb},1
	vcvtpi2pd v1{k1},v3{cccc},1
	vcvtpi2pd v1{k1},v3{dddd},1
	vcvtpi2pd v1{k1},v3{dacb},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{nt},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx],1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{16to16},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{1to16},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{4to16},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{uint8},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{sint8},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{uint16},1
	vcvtpi2pd v1{k1},ZMMWORD PTR [rcx]{sint16},1
	vcvtpi2pd v1{k1},[rcx]{sint16},1
	vcvtpu2pd v1{k1},v3,1
	vcvtpu2pd v1{k1},v3{cdab},1
	vcvtpu2pd v1{k1},v3{badc},1
	vcvtpu2pd v1{k1},v3{aaaa},1
	vcvtpu2pd v1{k1},v3{bbbb},1
	vcvtpu2pd v1{k1},v3{cccc},1
	vcvtpu2pd v1{k1},v3{dddd},1
	vcvtpu2pd v1{k1},v3{dacb},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{nt},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx],1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{16to16},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{1to16},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{4to16},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{uint8},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{sint8},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{uint16},1
	vcvtpu2pd v1{k1},ZMMWORD PTR [rcx]{sint16},1
	vcvtpu2pd v1{k1},[rcx]{sint16},1

# Tests for op v1{k1}, si(v2/mt), expd
	vcvtpi2ps v1{k1},v3,4
	vcvtpi2ps v1{k1},v3{cdab},4
	vcvtpi2ps v1{k1},v3{badc},4
	vcvtpi2ps v1{k1},v3{aaaa},4
	vcvtpi2ps v1{k1},v3{bbbb},4
	vcvtpi2ps v1{k1},v3{cccc},4
	vcvtpi2ps v1{k1},v3{dddd},4
	vcvtpi2ps v1{k1},v3{dacb},4
	vcvtpi2ps v1{k1},v3{dacb},.0b
	vcvtpi2ps v1{k1},v3{dacb},.4b
	vcvtpi2ps v1{k1},v3{dacb},.5b
	vcvtpi2ps v1{k1},v3{dacb},.8b
	vcvtpi2ps v1{k1},v3{dacb},.16b
	vcvtpi2ps v1{k1},v3{dacb},.24b
	vcvtpi2ps v1{k1},v3{dacb},.31b
	vcvtpi2ps v1{k1},v3{dacb},.32b
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{nt},4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx],4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{16to16},4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{1to16},4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{4to16},4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{uint8},4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{sint8},4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{uint16},4
	vcvtpi2ps v1{k1},ZMMWORD PTR [rcx]{sint16},4
	vcvtpi2ps v1{k1},[rcx]{sint16},4
	vcvtpu2ps v1{k1},v3,4
	vcvtpu2ps v1{k1},v3{cdab},4
	vcvtpu2ps v1{k1},v3{badc},4
	vcvtpu2ps v1{k1},v3{aaaa},4
	vcvtpu2ps v1{k1},v3{bbbb},4
	vcvtpu2ps v1{k1},v3{cccc},4
	vcvtpu2ps v1{k1},v3{dddd},4
	vcvtpu2ps v1{k1},v3{dacb},4
	vcvtpu2ps v1{k1},v3{dacb},.0b
	vcvtpu2ps v1{k1},v3{dacb},.4b
	vcvtpu2ps v1{k1},v3{dacb},.5b
	vcvtpu2ps v1{k1},v3{dacb},.8b
	vcvtpu2ps v1{k1},v3{dacb},.16b
	vcvtpu2ps v1{k1},v3{dacb},.24b
	vcvtpu2ps v1{k1},v3{dacb},.31b
	vcvtpu2ps v1{k1},v3{dacb},.32b
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{nt},4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx],4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{16to16},4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{1to16},4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{4to16},4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{uint8},4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{sint8},4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{uint16},4
	vcvtpu2ps v1{k1},ZMMWORD PTR [rcx]{sint16},4
	vcvtpu2ps v1{k1},[rcx]{sint16},4

# Tests for op v1{k1}, sf(v2/mt), half
	vcvtps2pd v1{k1},v3,1
	vcvtps2pd v1{k1},v3{cdab},1
	vcvtps2pd v1{k1},v3{badc},1
	vcvtps2pd v1{k1},v3{aaaa},1
	vcvtps2pd v1{k1},v3{bbbb},1
	vcvtps2pd v1{k1},v3{cccc},1
	vcvtps2pd v1{k1},v3{dddd},1
	vcvtps2pd v1{k1},v3{dacb},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{nt},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx],1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{16to16},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{1to16},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{4to16},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{uint8},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{unorm8},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{float16},1
	vcvtps2pd v1{k1},ZMMWORD PTR [rcx]{sint16},1
	vcvtps2pd v1{k1},[rcx]{sint16},1

# Tests for op v1{k1}, sf(v2/mt), rc, expd
	vcvtps2pi v1{k1},v3,rn,4
	vcvtps2pi v1{k1},v3{cdab},rn,4
	vcvtps2pi v1{k1},v3{badc},rn,4
	vcvtps2pi v1{k1},v3{aaaa},rn,4
	vcvtps2pi v1{k1},v3{bbbb},rn,4
	vcvtps2pi v1{k1},v3{cccc},rn,4
	vcvtps2pi v1{k1},v3{dddd},rn,4
	vcvtps2pi v1{k1},v3{dacb},rn,4
	vcvtps2pi v1{k1},v3{dacb},rn,.0b
	vcvtps2pi v1{k1},v3{dacb},rn,.4b
	vcvtps2pi v1{k1},v3{dacb},rn,.5b
	vcvtps2pi v1{k1},v3{dacb},rn,.8b
	vcvtps2pi v1{k1},v3{dacb},rn,.16b
	vcvtps2pi v1{k1},v3{dacb},rn,.24b
	vcvtps2pi v1{k1},v3{dacb},rn,.31b
	vcvtps2pi v1{k1},v3{dacb},rn,.32b
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{nt},rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx],rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{16to16},rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{1to16},rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{4to16},rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{uint8},rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{unorm8},rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{float16},rn,4
	vcvtps2pi v1{k1},ZMMWORD PTR [rcx]{sint16},rn,4
	vcvtps2pi v1{k1},[rcx]{sint16},rn,4
	vcvtps2pu v1{k1},v3,rn,4
	vcvtps2pu v1{k1},v3{cdab},rn,4
	vcvtps2pu v1{k1},v3{badc},rn,4
	vcvtps2pu v1{k1},v3{aaaa},rn,4
	vcvtps2pu v1{k1},v3{bbbb},rn,4
	vcvtps2pu v1{k1},v3{cccc},rn,4
	vcvtps2pu v1{k1},v3{dddd},rn,4
	vcvtps2pu v1{k1},v3{dacb},rn,4
	vcvtps2pu v1{k1},v3{dacb},rn,.0b
	vcvtps2pu v1{k1},v3{dacb},rn,.4b
	vcvtps2pu v1{k1},v3{dacb},rn,.5b
	vcvtps2pu v1{k1},v3{dacb},rn,.8b
	vcvtps2pu v1{k1},v3{dacb},rn,.16b
	vcvtps2pu v1{k1},v3{dacb},rn,.24b
	vcvtps2pu v1{k1},v3{dacb},rn,.31b
	vcvtps2pu v1{k1},v3{dacb},rn,.32b
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{nt},rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx],rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{16to16},rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{1to16},rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{4to16},rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{uint8},rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{unorm8},rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{float16},rn,4
	vcvtps2pu v1{k1},ZMMWORD PTR [rcx]{sint16},rn,4
	vcvtps2pu v1{k1},[rcx]{sint16},rn,4
	vroundps v1{k1},v3,rn,4
	vroundps v1{k1},v3{cdab},rn,4
	vroundps v1{k1},v3{badc},rn,4
	vroundps v1{k1},v3{aaaa},rn,4
	vroundps v1{k1},v3{bbbb},rn,4
	vroundps v1{k1},v3{cccc},rn,4
	vroundps v1{k1},v3{dddd},rn,4
	vroundps v1{k1},v3{dacb},rn,4
	vroundps v1{k1},v3{dacb},rn,.0b
	vroundps v1{k1},v3{dacb},rn,.4b
	vroundps v1{k1},v3{dacb},rn,.5b
	vroundps v1{k1},v3{dacb},rn,.8b
	vroundps v1{k1},v3{dacb},rn,.16b
	vroundps v1{k1},v3{dacb},rn,.24b
	vroundps v1{k1},v3{dacb},rn,.31b
	vroundps v1{k1},v3{dacb},rn,.32b
	vroundps v1{k1},ZMMWORD PTR [rcx]{nt},rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx],rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx]{16to16},rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx]{1to16},rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx]{4to16},rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx]{uint8},rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx]{unorm8},rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx]{float16},rn,4
	vroundps v1{k1},ZMMWORD PTR [rcx]{sint16},rn,4
	vroundps v1{k1},[rcx]{sint16},rn,4

# Tests for op v1{k1}, sf(v2/mt)
	vcvtps2srgb8 v1{k1},v3
	vcvtps2srgb8 v1{k1},v3{cdab}
	vcvtps2srgb8 v1{k1},v3{badc}
	vcvtps2srgb8 v1{k1},v3{aaaa}
	vcvtps2srgb8 v1{k1},v3{bbbb}
	vcvtps2srgb8 v1{k1},v3{cccc}
	vcvtps2srgb8 v1{k1},v3{dddd}
	vcvtps2srgb8 v1{k1},v3{dacb}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{nt}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{16to16}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{1to16}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{4to16}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{uint8}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{float16}
	vcvtps2srgb8 v1{k1},ZMMWORD PTR [rcx]{sint16}
	vcvtps2srgb8 v1{k1},[rcx]{sint16}
	vlog2lutps v1{k1},v3
	vlog2lutps v1{k1},v3{cdab}
	vlog2lutps v1{k1},v3{badc}
	vlog2lutps v1{k1},v3{aaaa}
	vlog2lutps v1{k1},v3{bbbb}
	vlog2lutps v1{k1},v3{cccc}
	vlog2lutps v1{k1},v3{dddd}
	vlog2lutps v1{k1},v3{dacb}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{nt}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{16to16}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{1to16}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{4to16}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{uint8}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{float16}
	vlog2lutps v1{k1},ZMMWORD PTR [rcx]{sint16}
	vlog2lutps v1{k1},[rcx]{sint16}
	vrcpresps v1{k1},v3
	vrcpresps v1{k1},v3{cdab}
	vrcpresps v1{k1},v3{badc}
	vrcpresps v1{k1},v3{aaaa}
	vrcpresps v1{k1},v3{bbbb}
	vrcpresps v1{k1},v3{cccc}
	vrcpresps v1{k1},v3{dddd}
	vrcpresps v1{k1},v3{dacb}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{nt}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{16to16}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{1to16}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{4to16}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{uint8}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{float16}
	vrcpresps v1{k1},ZMMWORD PTR [rcx]{sint16}
	vrcpresps v1{k1},[rcx]{sint16}
	vrsqrtlutps v1{k1},v3
	vrsqrtlutps v1{k1},v3{cdab}
	vrsqrtlutps v1{k1},v3{badc}
	vrsqrtlutps v1{k1},v3{aaaa}
	vrsqrtlutps v1{k1},v3{bbbb}
	vrsqrtlutps v1{k1},v3{cccc}
	vrsqrtlutps v1{k1},v3{dddd}
	vrsqrtlutps v1{k1},v3{dacb}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{nt}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{16to16}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{1to16}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{4to16}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{uint8}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{float16}
	vrsqrtlutps v1{k1},ZMMWORD PTR [rcx]{sint16}
	vrsqrtlutps v1{k1},[rcx]{sint16}
	vgetexpps v1{k1},v3
	vgetexpps v1{k1},v3{cdab}
	vgetexpps v1{k1},v3{badc}
	vgetexpps v1{k1},v3{aaaa}
	vgetexpps v1{k1},v3{bbbb}
	vgetexpps v1{k1},v3{cccc}
	vgetexpps v1{k1},v3{dddd}
	vgetexpps v1{k1},v3{dacb}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{nt}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{16to16}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{1to16}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{4to16}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{uint8}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{float16}
	vgetexpps v1{k1},ZMMWORD PTR [rcx]{sint16}
	vgetexpps v1{k1},[rcx]{sint16}

# Tests for op v1{k1}, si(v2/mt)
	vexp2lutps v1{k1},v3
	vexp2lutps v1{k1},v3{cdab}
	vexp2lutps v1{k1},v3{badc}
	vexp2lutps v1{k1},v3{aaaa}
	vexp2lutps v1{k1},v3{bbbb}
	vexp2lutps v1{k1},v3{cccc}
	vexp2lutps v1{k1},v3{dddd}
	vexp2lutps v1{k1},v3{dacb}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{nt}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{16to16}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{1to16}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{4to16}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{uint8}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{sint8}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{uint16}
	vexp2lutps v1{k1},ZMMWORD PTR [rcx]{sint16}
	vexp2lutps v1{k1},[rcx]{sint16}

# Tests for op v1{k1}, sf(v2/mt), imm21
	vfixupps v1{k1},v3,2
	vfixupps v1{k1},v3{cdab},2
	vfixupps v1{k1},v3{badc},2
	vfixupps v1{k1},v3{aaaa},2
	vfixupps v1{k1},v3{bbbb},2
	vfixupps v1{k1},v3{cccc},2
	vfixupps v1{k1},v3{dddd},2
	vfixupps v1{k1},v3{dacb},2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{nt},2
	vfixupps v1{k1},ZMMWORD PTR [rcx],2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{16to16},2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{1to16},2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{4to16},2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{uint8},2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{unorm8},2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{float16},2
	vfixupps v1{k1},ZMMWORD PTR [rcx]{sint16},2
	vfixupps v1{k1},[rcx]{sint16},2

# Tests for op v1{k1}, v2, si(v3/mt), rot, bil, bih
	vinsertfieldpi v1{k1},v2,v3,11,2,31
	vinsertfieldpi v1{k1},v2,v3{cdab},11,2,31
	vinsertfieldpi v1{k1},v2,v3{badc},11,2,31
	vinsertfieldpi v1{k1},v2,v3{aaaa},11,2,31
	vinsertfieldpi v1{k1},v2,v3{bbbb},11,2,31
	vinsertfieldpi v1{k1},v2,v3{cccc},11,2,31
	vinsertfieldpi v1{k1},v2,v3{dddd},11,2,31
	vinsertfieldpi v1{k1},v2,v3{dacb},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{nt},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx],11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{16to16},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{1to16},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{4to16},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{uint8},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{sint8},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{uint16},11,2,31
	vinsertfieldpi v1{k1},v2,ZMMWORD PTR [rcx]{sint16},11,2,31
	vinsertfieldpi v1{k1},v2,[rcx]{sint16},11,2,31

# Tests for op v1{k1}, sb(f(mt))
	vloadd v1{k1},ZMMWORD PTR [rcx]{nt}
	vloadd v1{k1},ZMMWORD PTR [rcx]
	vloadd v1{k1},ZMMWORD PTR [rcx]{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float16}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float16}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float16}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{srgb8}
	vloadd v1{k1},ZMMWORD PTR [rcx]{srgb8}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{srgb8}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{srgb8}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm8}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm8}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm8}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm8}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm8}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm8}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm8}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm16}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm16}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm16}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm16}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm16}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{snorm16}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8i}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8i}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8i}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint8i}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8i}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8i}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8i}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint8i}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16i}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16i}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16i}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{uint16i}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16i}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16i}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16i}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{sint16i}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10A}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10A}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10A}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10A}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10B}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10B}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10B}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10B}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10C}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10C}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10C}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm10C}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm2D}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm2D}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm2D}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{unorm2D}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11A}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11A}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11A}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11A}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11B}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11B}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11B}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float11B}{4to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float10C}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float10C}{16to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float10C}{1to16}
	vloadd v1{k1},ZMMWORD PTR [rcx]{float10C}{4to16}
	vloadd v1{k1},[rcx]{float10C}{4to16}

# Tests for op v1{k1}, sb(fq(mt))
	vloadq v1{k1},ZMMWORD PTR [rcx]{nt}
	vloadq v1{k1},ZMMWORD PTR [rcx]{8to8}
	vloadq v1{k1},ZMMWORD PTR [rcx]{1to8}
	vloadq v1{k1},ZMMWORD PTR [rcx]{4to8}
	vloadq v1{k1},[rcx]{4to8}

# Tests for op v1{k1}, f(mt)
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{nt}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{float16}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{srgb8}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{uint8}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{sint8}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{snorm8}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{uint16}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{sint16}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{unorm16}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{snorm16}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{uint8i}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{sint8i}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{uint16i}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{sint16i}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{unorm10A}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{unorm10B}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{unorm10C}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{unorm2D}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{float11A}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{float11B}
	vloadunpackhd v1{k1},ZMMWORD PTR [rcx]{float10C}
	vloadunpackhd v1{k1},[rcx]{float10C}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{nt}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{float16}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{srgb8}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{uint8}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{sint8}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{unorm8}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{snorm8}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{uint16}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{sint16}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{unorm16}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{snorm16}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{uint8i}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{sint8i}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{uint16i}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{sint16i}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{unorm10A}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{unorm10B}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{unorm10C}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{unorm2D}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{float11A}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{float11B}
	vloadunpackld v1{k1},ZMMWORD PTR [rcx]{float10C}
	vloadunpackld v1{k1},[rcx]{float10C}

# Tests for op v1{k1}, si(v2/mt), rot, bil, bih
	vrotatefieldpi v1{k1},v3,11,2,31
	vrotatefieldpi v1{k1},v3{cdab},11,2,31
	vrotatefieldpi v1{k1},v3{badc},11,2,31
	vrotatefieldpi v1{k1},v3{aaaa},11,2,31
	vrotatefieldpi v1{k1},v3{bbbb},11,2,31
	vrotatefieldpi v1{k1},v3{cccc},11,2,31
	vrotatefieldpi v1{k1},v3{dddd},11,2,31
	vrotatefieldpi v1{k1},v3{dacb},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{nt},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx],11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{16to16},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{1to16},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{4to16},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{uint8},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{sint8},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{uint16},11,2,31
	vrotatefieldpi v1{k1},ZMMWORD PTR [rcx]{sint16},11,2,31
	vrotatefieldpi v1{k1},[rcx]{sint16},11,2,31

# Test for op v1{k1}, v2/mt, perm128, perm32
	vshuf128x32 v1{k1},v2,2,2
	vshuf128x32 v1{k1},ZMMWORD PTR [rcx]{nt},2,2
	vshuf128x32 v1{k1},[rcx],2,2

# Test for op mt{k1}, ss(d(v1))
	vstored ZMMWORD PTR [rcx]{nt}{k1},v1
	vstored [rcx]{k1},v1
	vstored ZMMWORD PTR [rcx]{k1},v1
	vstored ZMMWORD PTR [rcx]{k1},v1{float16}
	vstored ZMMWORD PTR [rcx]{k1},v1{float16rz}
	vstored ZMMWORD PTR [rcx]{k1},v1{uint8}
	vstored ZMMWORD PTR [rcx]{k1},v1{sint8}
	vstored ZMMWORD PTR [rcx]{k1},v1{unorm8}
	vstored ZMMWORD PTR [rcx]{k1},v1{snorm8}
	vstored ZMMWORD PTR [rcx]{k1},v1{uint16}
	vstored ZMMWORD PTR [rcx]{k1},v1{sint16}
	vstored ZMMWORD PTR [rcx]{k1},v1{unorm16}
	vstored ZMMWORD PTR [rcx]{k1},v1{snorm16}
	vstored ZMMWORD PTR [rcx]{k1},v1{uint8i}
	vstored ZMMWORD PTR [rcx]{k1},v1{sint8i}
	vstored ZMMWORD PTR [rcx]{k1},v1{uint16i}
	vstored ZMMWORD PTR [rcx]{k1},v1{sint16i}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{float16}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{float16rz}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{uint8}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{sint8}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{unorm8}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{snorm8}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{uint16}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{sint16}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{unorm16}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{snorm16}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{uint8i}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{sint8i}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{uint16i}
	vstored ZMMWORD PTR [rcx]{k1},v1{a}{sint16i}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{float16}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{float16rz}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{uint8}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{sint8}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{unorm8}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{snorm8}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{uint16}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{sint16}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{unorm16}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{snorm16}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{uint8i}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{sint8i}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{uint16i}
	vstored ZMMWORD PTR [rcx]{k1},v1{dcba}{sint16i}

# Test for op mt{k1}, ss(dq(v1))
	vstoreq ZMMWORD PTR [rcx]{nt}{k1},v1
	vstoreq [rcx]{k1},v1
	vstoreq ZMMWORD PTR [rcx]{k1},v1
	vstoreq ZMMWORD PTR [rcx]{k1},v1{a}
	vstoreq ZMMWORD PTR [rcx]{k1},v1{dcba}

# Tests for op v1{k1}, fq(mt)
	vloadunpackhq v1{k1},ZMMWORD PTR [rcx]{nt}
	vloadunpackhq v1{k1},[rcx]
	vloadunpacklq v1{k1},ZMMWORD PTR [rcx]{nt}
	vloadunpacklq v1{k1},[rcx]

# Tests for op mt{k1}, D(v1)
	vpackstorehd ZMMWORD PTR [rcx]{nt}{k1},v1
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{float16}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{float16rz}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{uint8}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{sint8}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{unorm8}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{snorm8}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{uint16}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{sint16}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{unorm16}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{snorm16}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{uint8i}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{sint8i}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{uint16i}
	vpackstorehd ZMMWORD PTR [rcx]{k1},v1{sint16i}
	vpackstorehd [rcx]{k1},v1{sint16i}
	vpackstoreld ZMMWORD PTR [rcx]{nt}{k1},v1
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{float16}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{float16rz}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{uint8}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{sint8}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{unorm8}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{snorm8}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{uint16}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{sint16}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{unorm16}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{snorm16}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{uint8i}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{sint8i}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{uint16i}
	vpackstoreld ZMMWORD PTR [rcx]{k1},v1{sint16i}
	vpackstoreld [rcx]{k1},v1{sint16i}

# Tests for op mt{k1}, Dq(v1)
	vpackstorehq ZMMWORD PTR [rcx]{nt}{k1},v1
	vpackstorehq [rcx]{k1},v1
	vpackstorelq ZMMWORD PTR [rcx]{nt}{k1},v1
	vpackstorelq [rcx]{k1},v1

# Tests for op mvt{k1}, D(v1)
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{nt}{k1},v1
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{float16}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{float16rz}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{uint8}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{sint8}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{unorm8}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{snorm8}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{uint16}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{sint16}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{unorm16}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{snorm16}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{uint8i}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{sint8i}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{uint16i}
	vscatterd ZMMWORD PTR [rbp+v10*2+0x99]{k1},v1{sint16i}
	vscatterd [rbp+v10*2+0x99]{k1},v1{sint16i}

# Tests for op d(mvt){k1}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{nt}{k1}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{float16}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{float16rz}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint8}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint8}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm8}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{snorm8}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint16}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint16}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm16}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{snorm16}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint8i}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint8i}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint16i}{k2}
	vscatterpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint16i}{k2}
	vscatterpfd [rbp+v10*2+0x99]{sint16i}{k1}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{nt}{k1}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{float16}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{float16rz}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint8}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint8}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm8}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{snorm8}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint16}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint16}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm16}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{snorm16}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint8i}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint8i}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint16i}{k2}
	vscatterpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint16i}{k2}
	vscatterpf2d [rbp+v10*2+0x99]{sint16i}{k1}

# Tests for op v1{k1}, f(mvt)
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{nt}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{float16}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{srgb8}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{uint8}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{sint8}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{unorm8}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{snorm8}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{uint16}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{sint16}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{unorm16}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{snorm16}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{uint8i}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{sint8i}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{uint16i}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{sint16i}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{unorm10A}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{unorm10B}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{unorm10C}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{unorm2D}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{float11A}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{float11B}
	vgatherd v1{k1},ZMMWORD PTR [rbp+v10*2+0x99]{float10C}
	vgatherd v1{k1},[rbp+v10*2+0x99]{float10C}

# Tests for op f(mvt){k1}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{nt}{k1}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{float16}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{srgb8}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint8}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint8}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm8}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{snorm8}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint16}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint16}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm16}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{snorm16}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint8i}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint8i}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{uint16i}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{sint16i}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm10A}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm10B}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm10C}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{unorm2D}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{float11A}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{float11B}{k2}
	vgatherpfd ZMMWORD PTR [rbp+v10*2+0x99]{float10C}{k2}
	vgatherpfd [rbp+v10*2+0x99]{float10C}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{nt}{k1}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{float16}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{srgb8}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint8}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint8}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm8}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{snorm8}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint16}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint16}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm16}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{snorm16}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint8i}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint8i}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{uint16i}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{sint16i}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm10A}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm10B}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm10C}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{unorm2D}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{float11A}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{float11B}{k2}
	vgatherpf2d ZMMWORD PTR [rbp+v10*2+0x99]{float10C}{k2}
	vgatherpf2d [rbp+v10*2+0x99]{float10C}{k2}

# Test for op r16, r16
	bitinterleave11 bx,ax
	bitinterleave11 r10w,r9w
	bitinterleave21 bx,ax
	bitinterleave21 r10w,r9w
	bsff bx,ax
	bsff r10w,r9w
	bsfi bx,ax
	bsfi r10w,r9w
	bsrf bx,ax
	bsrf r10w,r9w
	bsri bx,ax
	bsri r10w,r9w
	countbits bx,ax
	countbits r10w,r9w
	quadmask16 bx,ax
	quadmask16 r10w,r9w

# Test for op r32, r32
	bitinterleave11 ebx,eax
	bitinterleave11 r10d,r9d
	bitinterleave21 ebx,eax
	bitinterleave21 r10d,r9d
	bsff ebx,eax
	bsff r10d,r9d
	bsfi ebx,eax
	bsfi r10d,r9d
	bsrf ebx,eax
	bsrf r10d,r9d
	bsri ebx,eax
	bsri r10d,r9d
	countbits ebx,eax
	countbits r10d,r9d
	quadmask16 ebx,eax
	quadmask16 r10d,r9d

# Test for op r64, r64
	bitinterleave11 rbx,rax
	bitinterleave11 r15,r9
	bitinterleave21 rbx,rax
	bitinterleave21 r15,r9
	bsff rbx,rax
	bsff r15,r9
	bsfi rbx,rax
	bsfi r15,r9
	bsrf rbx,rax
	bsrf r15,r9
	bsri rbx,rax
	bsri r15,r9
	countbits rbx,rax
	countbits r15,r9
	quadmask16 rbx,rax
	quadmask16 r15,r9

# Test for op m8
	clevict1 BYTE PTR [rcx]
	clevict1 [rcx]
	clevict2 BYTE PTR [rcx]
	clevict2 [rcx]

# Test for op m8, imm8
	vprefetch1 BYTE PTR [rcx],7
	vprefetch1 [rcx],7

# Test for op m2, imm2
	vprefetch2 BYTE PTR [rcx],2
	vprefetch2 [rcx],2

# Test for op m32
	ldvxcsr DWORD PTR [rcx]
	ldvxcsr [rcx]
	stvxcsr DWORD PTR [rcx]
	stvxcsr [rcx]

# Test for op r16
	delay ax
	delay r9w
	spflt ax
	spflt r9w

# Test for op r32
	delay eax
	delay r9d
	spflt eax
	spflt r9d

# Test for op r64
	delay rax
	delay r9
	spflt rax
	spflt r9

# Test for op imm5
	delay 10

# Test for op r16, r16, rot, bil, bih
	insertfield bx,ax,11,2,31
	insertfield r10w,r9w,11,2,31
	rotatefield bx,ax,11,2,31
	rotatefield r10w,r9w,11,2,31

# Test for op r32, r32, rot, bil, bih
	insertfield ebx,eax,11,2,31
	insertfield r10d,r9d,11,2,31
	rotatefield ebx,eax,11,2,31
	rotatefield r10d,r9d,11,2,31

# Test for op r64, r64, rot, bil, bih
	insertfield rbx,rax,11,2,31
	insertfield r15,r9,11,2,31
	rotatefield rbx,rax,11,2,31
	rotatefield r15,r9,11,2,31

# Generate test cases for different operands forms
	vadcpi v10{k1},k2,v30{cdab}
	vaddnpd v10{k1},v20,v30{dddd}
	vadcpi v10{k1},k2,ds:0x12345678
	vadcpi v10{k1},k2,[rbp]
	vadcpi v10{k1},k2,[rbp+0x99]
	vadcpi v10{k1},k2,[r15+0x99]
	vadcpi v10{k1},k2,[rip+0x99]
	vadcpi v10{k1},k2,[rsp+0x99]
	vadcpi v10{k1},k2,[r12+0x99]
	vadcpi v10{k1},k2,[riz*1-0x99]
	vadcpi v10{k1},k2,[riz*2-0x99]
	vadcpi v10{k1},k2,[rbx+riz*1-0x99]
	vadcpi v10{k1},k2,[rbx+riz*2-0x99]
	vadcpi v10{k1},k2,[r12+r15*4-0x99]
	vadcpi v10{k1},k2,[r8+r15*8-0x99]
	vadcpi v10{k1},k2,[rbp+r12*4-0x99]
	vadcpi v10{k1},k2,[rsp+r13*1-0x99]
	vgatherd v10{k1},[v1*1]
	vgatherd v10{k1},[v10*1]
	vgatherd v10{k1},[v1*1-0x99]
	vgatherd v10{k1},[v10*1-0x99]
	vgatherd v10{k1},[rax+v1]
	vgatherd v10{k1},[rax+v10]
	vgatherd v10{k1},[rax+v1*2-0x99]
	vgatherd v10{k1},[rax+v10*2-0x99]
	vgatherd v10{k1},[r12+v1*4-0x99]
	vgatherd v10{k1},[r12+v10*4-0x99]
	vadcpi v10{k1},k2,ds:0x1234
	vadcpi v10{k1},k2,[ebp]
	vadcpi v10{k1},k2,[ebp+0x99]
	vadcpi v10{k1},k2,[eiz*1+0x99]
	vadcpi v10{k1},k2,[eiz*2+0x99]
	vadcpi v10{k1},k2,[eax+eiz*1+0x99]
	vadcpi v10{k1},k2,[eax+eiz*2+0x99]
	vadcpi v10{k1},k2,[eax+ebx*4+0x99]
	vadcpi v10{k1},k2,[esp+ecx*8+0x99]
	vadcpi v10{k1},k2,[ebp+edx*1+0x99]
	vgatherd v10{k1},[eax+v1]
	vgatherd v10{k1},[eax+v10]
	vgatherd v10{k1},[eax+v1]
	vgatherd v10{k1},[eax+v10]
	vgatherd v10{k1},[eax+v1*2-0x99]
	vgatherd v10{k1},[eax+v10*2-0x99]
	vgatherd v10{k1},[ebx+v1*4-0x99]
	vgatherd v10{k1},[ebx+v10*4-0x99]

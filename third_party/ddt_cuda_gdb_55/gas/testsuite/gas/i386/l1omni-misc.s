	.text
_start:
	vkand %k1, %k0
	vkand %k7, %k1
	vkandn %k1, %k0
	vkandn %k7, %k1
	vkandnr %k1, %k0
	vkandnr %k7, %k1
	vkmovlhb %k1, %k0
	vkmovlhb %k7, %k1
	vknot %k1, %k0
	vknot %k7, %k1
	vkor %k1, %k0
	vkor %k7, %k1
	vkortest %k1, %k0
	vkortest %k7, %k1
	vkswapb %k1, %k0
	vkswapb %k7, %k1
	vkxnor %k1, %k0
	vkxnor %k7, %k1
	vkxor %k1, %k0
	vkxor %k7, %k1
	bitinterleave21 %bx,%ax
	bitinterleave21 %eax,%ebx
	bitinterleave21 %rax,%r13
	bsff %bx,%ax
	bsff %eax,%ebx
	bsff %rax,%r13
	bsfi %bx,%ax
	bsfi %eax,%ebx
	bsfi %rax,%r13
	bsrf %bx,%ax
	bsrf %eax,%ebx
	bsrf %rax,%r13
	bsri %bx,%ax
	bsri %eax,%ebx
	bsri %rax,%r13
	countbits %bx,%ax
	countbits %eax,%ebx
	countbits %rax,%r13
	quadmask16 %bx,%ax
	quadmask16 %eax,%ebx
	quadmask16 %rax,%r13
	bitinterleave21w %ax,%bx
	bitinterleave21l %eax,%ebx
	bitinterleave21q %rax,%r13
	bsffw %ax,%bx
	bsffl %eax,%ebx
	bsffq %rax,%r13
	bsfiw %ax,%bx
	bsfil %eax,%ebx
	bsfiq %rax,%r13
	bsrfw %ax,%bx
	bsrfl %eax,%ebx
	bsrfq %rax,%r13
	bsriw %ax,%bx
	bsril %eax,%ebx
	bsriq %rax,%r13
	countbitsw %ax,%bx
	countbitsl %eax,%ebx
	countbitsq %rax,%r13
	quadmask16w %ax,%bx
	quadmask16l %eax,%ebx
	quadmask16q %rax,%r13
	vaddps  %v2, %v1, %v0
	vaddps  %v2 {dcba}, %v1, %v0
	vaddps  %v2 {dcba}, %v1, %v0 {%k7}
	vaddps  %v2 {cdab}, %v1, %v0 {%k7}
	vaddps  %v2 {badc}, %v1, %v0 {%k7}
	vaddps  %v2 {aaaa}, %v1, %v0 {%k7}
	vaddps  %v2 {bbbb}, %v1, %v0 {%k7}
	vaddps  %v2 {cccc}, %v1, %v0 {%k7}
	vaddps  %v2 {dddd}, %v1, %v0 {%k7}
	vaddps  %v2 {dacb}, %v1, %v0 {%k7}
	vaddps  %v7 {dacb}, %v1, %v0 {%k7}
	vaddps  %v8 {dacb}, %v1, %v0 {%k7}
	vaddps  %v9 {dacb}, %v1, %v0 {%k7}
	vaddps  %v15 {dacb}, %v1, %v0 {%k7}
	vaddps  %v16 {dacb}, %v1, %v0 {%k7}
	vaddps  %v17 {dacb}, %v1, %v0 {%k7}
	vaddps  %v31 {dacb}, %v1, %v0 {%k7}
	vaddps  %v31 {dacb}, %v1, %v8 {%k7}
	vaddps  %v31 {dacb}, %v1, %v9 {%k7}
	vaddps  %v31 {dacb}, %v1, %v15 {%k7}
	vaddps  %v31 {dacb}, %v8, %v15 {%k7}
	vaddps  %v31 {dacb}, %v9, %v15 {%k7}
	vaddps  %v31 {dacb}, %v15, %v15 {%k7}
	vaddps  %v31 {dacb}, %v31, %v31 {%k7}
	vaddps  (%rax), %v1, %v0{%k7}
	vaddps  (%rax) {1to16}, %v1, %v0{%k7}
	vaddps  (%rax) {4to16}, %v1, %v0{%k7}
	vaddps  (%rax) {unorm8}, %v1, %v0{%k7}
	vaddps  (%rax) {sint16}, %v1, %v0{%k7}
	vaddps  (%rax) {float16}, %v1, %v0{%k7}
	vaddps  (%rax) {uint8}, %v1, %v0{%k7}
	vaddps  (%rax) {nt}, %v1, %v0{%k7}
	vaddps  (%rax) {NT}, %v1, %v0{%k7}
	vaddps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vaddps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vaddps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
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
	vclampzps  %v2, %v1, %v0
	vclampzps  %v2 {dcba}, %v1, %v0
	vclampzps  %v2 {dcba}, %v1, %v0 {%k7}
	vclampzps  %v2 {cdab}, %v1, %v0 {%k7}
	vclampzps  %v2 {badc}, %v1, %v0 {%k7}
	vclampzps  %v2 {aaaa}, %v1, %v0 {%k7}
	vclampzps  %v2 {bbbb}, %v1, %v0 {%k7}
	vclampzps  %v2 {cccc}, %v1, %v0 {%k7}
	vclampzps  %v2 {dddd}, %v1, %v0 {%k7}
	vclampzps  %v2 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v7 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v8 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v9 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v15 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v16 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v17 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v31 {dacb}, %v1, %v0 {%k7}
	vclampzps  %v31 {dacb}, %v1, %v8 {%k7}
	vclampzps  %v31 {dacb}, %v1, %v9 {%k7}
	vclampzps  %v31 {dacb}, %v1, %v15 {%k7}
	vclampzps  %v31 {dacb}, %v8, %v15 {%k7}
	vclampzps  %v31 {dacb}, %v9, %v15 {%k7}
	vclampzps  %v31 {dacb}, %v15, %v15 {%k7}
	vclampzps  %v31 {dacb}, %v31, %v31 {%k7}
	vclampzps  (%rax), %v1, %v0{%k7}
	vclampzps  (%rax) {1to16}, %v1, %v0{%k7}
	vclampzps  (%rax) {4to16}, %v1, %v0{%k7}
	vclampzps  (%rax) {unorm8}, %v1, %v0{%k7}
	vclampzps  (%rax) {sint16}, %v1, %v0{%k7}
	vclampzps  (%rax) {float16}, %v1, %v0{%k7}
	vclampzps  (%rax) {uint8}, %v1, %v0{%k7}
	vclampzps  (%rax) {nt}, %v1, %v0{%k7}
	vclampzps  (%rax) {NT}, %v1, %v0{%k7}
	vclampzps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vclampzps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vclampzps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vclampzps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vclampzps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd132ps  %v2, %v1, %v0
	vmadd132ps  %v2 {dcba}, %v1, %v0
	vmadd132ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmadd132ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmadd132ps  %v2 {badc}, %v1, %v0 {%k7}
	vmadd132ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmadd132ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmadd132ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmadd132ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmadd132ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmadd132ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmadd132ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmadd132ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmadd132ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmadd132ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmadd132ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmadd132ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmadd132ps  (%rax), %v1, %v0{%k7}
	vmadd132ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {float16}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {nt}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {NT}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmadd132ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmadd132ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd132ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd132ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd213ps  %v2, %v1, %v0
	vmadd213ps  %v2 {dcba}, %v1, %v0
	vmadd213ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmadd213ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmadd213ps  %v2 {badc}, %v1, %v0 {%k7}
	vmadd213ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmadd213ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmadd213ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmadd213ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmadd213ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmadd213ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmadd213ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmadd213ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmadd213ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmadd213ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmadd213ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmadd213ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmadd213ps  (%rax), %v1, %v0{%k7}
	vmadd213ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {float16}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {nt}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {NT}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmadd213ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmadd213ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd213ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd213ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd231ps  %v2, %v1, %v0
	vmadd231ps  %v2 {dcba}, %v1, %v0
	vmadd231ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmadd231ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmadd231ps  %v2 {badc}, %v1, %v0 {%k7}
	vmadd231ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmadd231ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmadd231ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmadd231ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmadd231ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmadd231ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmadd231ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmadd231ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmadd231ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmadd231ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmadd231ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmadd231ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmadd231ps  (%rax), %v1, %v0{%k7}
	vmadd231ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {float16}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {nt}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {NT}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmadd231ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmadd231ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd231ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd231ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddsetsps  %v2 {dcba}, %v1, %v0 {%k7}
	vaddsetsps  %v2 {cdab}, %v1, %v0 {%k7}
	vaddsetsps  %v2 {badc}, %v1, %v0 {%k7}
	vaddsetsps  %v2 {aaaa}, %v1, %v0 {%k7}
	vaddsetsps  %v2 {bbbb}, %v1, %v0 {%k7}
	vaddsetsps  %v2 {cccc}, %v1, %v0 {%k7}
	vaddsetsps  %v2 {dddd}, %v1, %v0 {%k7}
	vaddsetsps  %v2 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v7 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v8 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v9 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v15 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v16 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v17 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v31 {dacb}, %v1, %v0 {%k7}
	vaddsetsps  %v31 {dacb}, %v1, %v8 {%k7}
	vaddsetsps  %v31 {dacb}, %v1, %v9 {%k7}
	vaddsetsps  %v31 {dacb}, %v1, %v15 {%k7}
	vaddsetsps  %v31 {dacb}, %v8, %v15 {%k7}
	vaddsetsps  %v31 {dacb}, %v9, %v15 {%k7}
	vaddsetsps  %v31 {dacb}, %v15, %v15 {%k7}
	vaddsetsps  %v31 {dacb}, %v31, %v31 {%k7}
	vaddsetsps  (%rax), %v1, %v0{%k7}
	vaddsetsps  (%rax) {1to16}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {4to16}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {unorm8}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {sint16}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {float16}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {uint8}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {nt}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {NT}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vaddsetsps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vaddsetsps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddsetsps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddsetsps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd233ps  %v2, %v1, %v0
	vmadd233ps  %v2 {dcba}, %v1, %v0
	vmadd233ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmadd233ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmadd233ps  %v2 {badc}, %v1, %v0 {%k7}
	vmadd233ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmadd233ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmadd233ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmadd233ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmadd233ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmadd233ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmadd233ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmadd233ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmadd233ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmadd233ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmadd233ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmadd233ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmadd233ps  (%rax), %v1, %v0{%k7}
	vmadd233ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {float16}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {nt}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {NT}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmadd233ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmadd233ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd233ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd233ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn132ps  %v2, %v1, %v0
	vmaddn132ps  %v2 {dcba}, %v1, %v0
	vmaddn132ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmaddn132ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmaddn132ps  %v2 {badc}, %v1, %v0 {%k7}
	vmaddn132ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmaddn132ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmaddn132ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmaddn132ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmaddn132ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmaddn132ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmaddn132ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmaddn132ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmaddn132ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmaddn132ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmaddn132ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmaddn132ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmaddn132ps  (%rax), %v1, %v0{%k7}
	vmaddn132ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {float16}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {nt}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {NT}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmaddn132ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmaddn132ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn132ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn132ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn213ps  %v2, %v1, %v0
	vmaddn213ps  %v2 {dcba}, %v1, %v0
	vmaddn213ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmaddn213ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmaddn213ps  %v2 {badc}, %v1, %v0 {%k7}
	vmaddn213ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmaddn213ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmaddn213ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmaddn213ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmaddn213ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmaddn213ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmaddn213ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmaddn213ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmaddn213ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmaddn213ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmaddn213ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmaddn213ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmaddn213ps  (%rax), %v1, %v0{%k7}
	vmaddn213ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {float16}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {nt}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {NT}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmaddn213ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmaddn213ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn213ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn213ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn231ps  %v2, %v1, %v0
	vmaddn231ps  %v2 {dcba}, %v1, %v0
	vmaddn231ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmaddn231ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmaddn231ps  %v2 {badc}, %v1, %v0 {%k7}
	vmaddn231ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmaddn231ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmaddn231ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmaddn231ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmaddn231ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmaddn231ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmaddn231ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmaddn231ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmaddn231ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmaddn231ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmaddn231ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmaddn231ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmaddn231ps  (%rax), %v1, %v0{%k7}
	vmaddn231ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {float16}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {nt}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {NT}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmaddn231ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmaddn231ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn231ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaddn231ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxps  %v2, %v1, %v0
	vmaxps  %v2 {dcba}, %v1, %v0
	vmaxps  %v2 {dcba}, %v1, %v0 {%k7}
	vmaxps  %v2 {cdab}, %v1, %v0 {%k7}
	vmaxps  %v2 {badc}, %v1, %v0 {%k7}
	vmaxps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmaxps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmaxps  %v2 {cccc}, %v1, %v0 {%k7}
	vmaxps  %v2 {dddd}, %v1, %v0 {%k7}
	vmaxps  %v2 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v7 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v8 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v9 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v15 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v16 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v17 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v31 {dacb}, %v1, %v0 {%k7}
	vmaxps  %v31 {dacb}, %v1, %v8 {%k7}
	vmaxps  %v31 {dacb}, %v1, %v9 {%k7}
	vmaxps  %v31 {dacb}, %v1, %v15 {%k7}
	vmaxps  %v31 {dacb}, %v8, %v15 {%k7}
	vmaxps  %v31 {dacb}, %v9, %v15 {%k7}
	vmaxps  %v31 {dacb}, %v15, %v15 {%k7}
	vmaxps  %v31 {dacb}, %v31, %v31 {%k7}
	vmaxps  (%rax), %v1, %v0{%k7}
	vmaxps  (%rax) {1to16}, %v1, %v0{%k7}
	vmaxps  (%rax) {4to16}, %v1, %v0{%k7}
	vmaxps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmaxps  (%rax) {sint16}, %v1, %v0{%k7}
	vmaxps  (%rax) {float16}, %v1, %v0{%k7}
	vmaxps  (%rax) {uint8}, %v1, %v0{%k7}
	vmaxps  (%rax) {nt}, %v1, %v0{%k7}
	vmaxps  (%rax) {NT}, %v1, %v0{%k7}
	vmaxps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmaxps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmaxps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminps  %v2, %v1, %v0
	vminps  %v2 {dcba}, %v1, %v0
	vminps  %v2 {dcba}, %v1, %v0 {%k7}
	vminps  %v2 {cdab}, %v1, %v0 {%k7}
	vminps  %v2 {badc}, %v1, %v0 {%k7}
	vminps  %v2 {aaaa}, %v1, %v0 {%k7}
	vminps  %v2 {bbbb}, %v1, %v0 {%k7}
	vminps  %v2 {cccc}, %v1, %v0 {%k7}
	vminps  %v2 {dddd}, %v1, %v0 {%k7}
	vminps  %v2 {dacb}, %v1, %v0 {%k7}
	vminps  %v7 {dacb}, %v1, %v0 {%k7}
	vminps  %v8 {dacb}, %v1, %v0 {%k7}
	vminps  %v9 {dacb}, %v1, %v0 {%k7}
	vminps  %v15 {dacb}, %v1, %v0 {%k7}
	vminps  %v16 {dacb}, %v1, %v0 {%k7}
	vminps  %v17 {dacb}, %v1, %v0 {%k7}
	vminps  %v31 {dacb}, %v1, %v0 {%k7}
	vminps  %v31 {dacb}, %v1, %v8 {%k7}
	vminps  %v31 {dacb}, %v1, %v9 {%k7}
	vminps  %v31 {dacb}, %v1, %v15 {%k7}
	vminps  %v31 {dacb}, %v8, %v15 {%k7}
	vminps  %v31 {dacb}, %v9, %v15 {%k7}
	vminps  %v31 {dacb}, %v15, %v15 {%k7}
	vminps  %v31 {dacb}, %v31, %v31 {%k7}
	vminps  (%rax), %v1, %v0{%k7}
	vminps  (%rax) {1to16}, %v1, %v0{%k7}
	vminps  (%rax) {4to16}, %v1, %v0{%k7}
	vminps  (%rax) {unorm8}, %v1, %v0{%k7}
	vminps  (%rax) {sint16}, %v1, %v0{%k7}
	vminps  (%rax) {float16}, %v1, %v0{%k7}
	vminps  (%rax) {uint8}, %v1, %v0{%k7}
	vminps  (%rax) {nt}, %v1, %v0{%k7}
	vminps  (%rax) {NT}, %v1, %v0{%k7}
	vminps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vminps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vminps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub132ps  %v2, %v1, %v0
	vmsub132ps  %v2 {dcba}, %v1, %v0
	vmsub132ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmsub132ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmsub132ps  %v2 {badc}, %v1, %v0 {%k7}
	vmsub132ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmsub132ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmsub132ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmsub132ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmsub132ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmsub132ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmsub132ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmsub132ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmsub132ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmsub132ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmsub132ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmsub132ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmsub132ps  (%rax), %v1, %v0{%k7}
	vmsub132ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {float16}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {nt}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {NT}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmsub132ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmsub132ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub132ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub132ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub213ps  %v2, %v1, %v0
	vmsub213ps  %v2 {dcba}, %v1, %v0
	vmsub213ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmsub213ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmsub213ps  %v2 {badc}, %v1, %v0 {%k7}
	vmsub213ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmsub213ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmsub213ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmsub213ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmsub213ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmsub213ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmsub213ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmsub213ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmsub213ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmsub213ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmsub213ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmsub213ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmsub213ps  (%rax), %v1, %v0{%k7}
	vmsub213ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {float16}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {nt}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {NT}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmsub213ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmsub213ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub213ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub213ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub231ps  %v2, %v1, %v0
	vmsub231ps  %v2 {dcba}, %v1, %v0
	vmsub231ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmsub231ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmsub231ps  %v2 {badc}, %v1, %v0 {%k7}
	vmsub231ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmsub231ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmsub231ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmsub231ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmsub231ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmsub231ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmsub231ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmsub231ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmsub231ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmsub231ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmsub231ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmsub231ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmsub231ps  (%rax), %v1, %v0{%k7}
	vmsub231ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {float16}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {nt}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {NT}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmsub231ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmsub231ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub231ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsub231ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr132ps  %v2, %v1, %v0
	vmsubr132ps  %v2 {dcba}, %v1, %v0
	vmsubr132ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmsubr132ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmsubr132ps  %v2 {badc}, %v1, %v0 {%k7}
	vmsubr132ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmsubr132ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmsubr132ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmsubr132ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmsubr132ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmsubr132ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmsubr132ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmsubr132ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmsubr132ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmsubr132ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmsubr132ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmsubr132ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmsubr132ps  (%rax), %v1, %v0{%k7}
	vmsubr132ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {float16}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {nt}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {NT}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmsubr132ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmsubr132ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr132ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr132ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr213ps  %v2, %v1, %v0
	vmsubr213ps  %v2 {dcba}, %v1, %v0
	vmsubr213ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmsubr213ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmsubr213ps  %v2 {badc}, %v1, %v0 {%k7}
	vmsubr213ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmsubr213ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmsubr213ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmsubr213ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmsubr213ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmsubr213ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmsubr213ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmsubr213ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmsubr213ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmsubr213ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmsubr213ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmsubr213ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmsubr213ps  (%rax), %v1, %v0{%k7}
	vmsubr213ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {float16}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {nt}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {NT}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmsubr213ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmsubr213ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr213ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr213ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr231ps  %v2, %v1, %v0
	vmsubr231ps  %v2 {dcba}, %v1, %v0
	vmsubr231ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmsubr231ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmsubr231ps  %v2 {badc}, %v1, %v0 {%k7}
	vmsubr231ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmsubr231ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmsubr231ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmsubr231ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmsubr231ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmsubr231ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmsubr231ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmsubr231ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmsubr231ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmsubr231ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmsubr231ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmsubr231ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmsubr231ps  (%rax), %v1, %v0{%k7}
	vmsubr231ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {float16}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {nt}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {NT}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmsubr231ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmsubr231ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr231ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr231ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr23c1ps  %v2, %v1, %v0
	vmsubr23c1ps  %v2 {dcba}, %v1, %v0
	vmsubr23c1ps  %v2 {dcba}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v2 {cdab}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v2 {badc}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v2 {cccc}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v2 {dddd}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v2 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v7 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v8 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v9 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v15 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v16 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v17 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v1, %v0 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v1, %v8 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v1, %v9 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v1, %v15 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v8, %v15 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v9, %v15 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v15, %v15 {%k7}
	vmsubr23c1ps  %v31 {dacb}, %v31, %v31 {%k7}
	vmsubr23c1ps  (%rax), %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {1to16}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {4to16}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {sint16}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {float16}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {uint8}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {nt}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {NT}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmsubr23c1ps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmsubr23c1ps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr23c1ps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmsubr23c1ps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulps  %v2, %v1, %v0
	vmulps  %v2 {dcba}, %v1, %v0
	vmulps  %v2 {dcba}, %v1, %v0 {%k7}
	vmulps  %v2 {cdab}, %v1, %v0 {%k7}
	vmulps  %v2 {badc}, %v1, %v0 {%k7}
	vmulps  %v2 {aaaa}, %v1, %v0 {%k7}
	vmulps  %v2 {bbbb}, %v1, %v0 {%k7}
	vmulps  %v2 {cccc}, %v1, %v0 {%k7}
	vmulps  %v2 {dddd}, %v1, %v0 {%k7}
	vmulps  %v2 {dacb}, %v1, %v0 {%k7}
	vmulps  %v7 {dacb}, %v1, %v0 {%k7}
	vmulps  %v8 {dacb}, %v1, %v0 {%k7}
	vmulps  %v9 {dacb}, %v1, %v0 {%k7}
	vmulps  %v15 {dacb}, %v1, %v0 {%k7}
	vmulps  %v16 {dacb}, %v1, %v0 {%k7}
	vmulps  %v17 {dacb}, %v1, %v0 {%k7}
	vmulps  %v31 {dacb}, %v1, %v0 {%k7}
	vmulps  %v31 {dacb}, %v1, %v8 {%k7}
	vmulps  %v31 {dacb}, %v1, %v9 {%k7}
	vmulps  %v31 {dacb}, %v1, %v15 {%k7}
	vmulps  %v31 {dacb}, %v8, %v15 {%k7}
	vmulps  %v31 {dacb}, %v9, %v15 {%k7}
	vmulps  %v31 {dacb}, %v15, %v15 {%k7}
	vmulps  %v31 {dacb}, %v31, %v31 {%k7}
	vmulps  (%rax), %v1, %v0{%k7}
	vmulps  (%rax) {1to16}, %v1, %v0{%k7}
	vmulps  (%rax) {4to16}, %v1, %v0{%k7}
	vmulps  (%rax) {unorm8}, %v1, %v0{%k7}
	vmulps  (%rax) {sint16}, %v1, %v0{%k7}
	vmulps  (%rax) {float16}, %v1, %v0{%k7}
	vmulps  (%rax) {uint8}, %v1, %v0{%k7}
	vmulps  (%rax) {nt}, %v1, %v0{%k7}
	vmulps  (%rax) {NT}, %v1, %v0{%k7}
	vmulps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vmulps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vmulps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vrcprefineps  %v2, %v1, %v0
	vrcprefineps  %v2 {dcba}, %v1, %v0
	vrcprefineps  %v2 {dcba}, %v1, %v0 {%k7}
	vrcprefineps  %v2 {cdab}, %v1, %v0 {%k7}
	vrcprefineps  %v2 {badc}, %v1, %v0 {%k7}
	vrcprefineps  %v2 {aaaa}, %v1, %v0 {%k7}
	vrcprefineps  %v2 {bbbb}, %v1, %v0 {%k7}
	vrcprefineps  %v2 {cccc}, %v1, %v0 {%k7}
	vrcprefineps  %v2 {dddd}, %v1, %v0 {%k7}
	vrcprefineps  %v2 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v7 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v8 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v9 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v15 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v16 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v17 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v31 {dacb}, %v1, %v0 {%k7}
	vrcprefineps  %v31 {dacb}, %v1, %v8 {%k7}
	vrcprefineps  %v31 {dacb}, %v1, %v9 {%k7}
	vrcprefineps  %v31 {dacb}, %v1, %v15 {%k7}
	vrcprefineps  %v31 {dacb}, %v8, %v15 {%k7}
	vrcprefineps  %v31 {dacb}, %v9, %v15 {%k7}
	vrcprefineps  %v31 {dacb}, %v15, %v15 {%k7}
	vrcprefineps  %v31 {dacb}, %v31, %v31 {%k7}
	vrcprefineps  (%rax), %v1, %v0{%k7}
	vrcprefineps  (%rax) {1to16}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {4to16}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {unorm8}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {sint16}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {float16}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {uint8}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {nt}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {NT}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vrcprefineps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vrcprefineps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vrcprefineps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vrcprefineps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubps  %v2, %v1, %v0
	vsubps  %v2 {dcba}, %v1, %v0
	vsubps  %v2 {dcba}, %v1, %v0 {%k7}
	vsubps  %v2 {cdab}, %v1, %v0 {%k7}
	vsubps  %v2 {badc}, %v1, %v0 {%k7}
	vsubps  %v2 {aaaa}, %v1, %v0 {%k7}
	vsubps  %v2 {bbbb}, %v1, %v0 {%k7}
	vsubps  %v2 {cccc}, %v1, %v0 {%k7}
	vsubps  %v2 {dddd}, %v1, %v0 {%k7}
	vsubps  %v2 {dacb}, %v1, %v0 {%k7}
	vsubps  %v7 {dacb}, %v1, %v0 {%k7}
	vsubps  %v8 {dacb}, %v1, %v0 {%k7}
	vsubps  %v9 {dacb}, %v1, %v0 {%k7}
	vsubps  %v15 {dacb}, %v1, %v0 {%k7}
	vsubps  %v16 {dacb}, %v1, %v0 {%k7}
	vsubps  %v17 {dacb}, %v1, %v0 {%k7}
	vsubps  %v31 {dacb}, %v1, %v0 {%k7}
	vsubps  %v31 {dacb}, %v1, %v8 {%k7}
	vsubps  %v31 {dacb}, %v1, %v9 {%k7}
	vsubps  %v31 {dacb}, %v1, %v15 {%k7}
	vsubps  %v31 {dacb}, %v8, %v15 {%k7}
	vsubps  %v31 {dacb}, %v9, %v15 {%k7}
	vsubps  %v31 {dacb}, %v15, %v15 {%k7}
	vsubps  %v31 {dacb}, %v31, %v31 {%k7}
	vsubps  (%rax), %v1, %v0{%k7}
	vsubps  (%rax) {1to16}, %v1, %v0{%k7}
	vsubps  (%rax) {4to16}, %v1, %v0{%k7}
	vsubps  (%rax) {unorm8}, %v1, %v0{%k7}
	vsubps  (%rax) {sint16}, %v1, %v0{%k7}
	vsubps  (%rax) {float16}, %v1, %v0{%k7}
	vsubps  (%rax) {uint8}, %v1, %v0{%k7}
	vsubps  (%rax) {nt}, %v1, %v0{%k7}
	vsubps  (%rax) {NT}, %v1, %v0{%k7}
	vsubps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vsubps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vsubps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubrps  %v2, %v1, %v0
	vsubrps  %v2 {dcba}, %v1, %v0
	vsubrps  %v2 {dcba}, %v1, %v0 {%k7}
	vsubrps  %v2 {cdab}, %v1, %v0 {%k7}
	vsubrps  %v2 {badc}, %v1, %v0 {%k7}
	vsubrps  %v2 {aaaa}, %v1, %v0 {%k7}
	vsubrps  %v2 {bbbb}, %v1, %v0 {%k7}
	vsubrps  %v2 {cccc}, %v1, %v0 {%k7}
	vsubrps  %v2 {dddd}, %v1, %v0 {%k7}
	vsubrps  %v2 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v7 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v8 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v9 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v15 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v16 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v17 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v31 {dacb}, %v1, %v0 {%k7}
	vsubrps  %v31 {dacb}, %v1, %v8 {%k7}
	vsubrps  %v31 {dacb}, %v1, %v9 {%k7}
	vsubrps  %v31 {dacb}, %v1, %v15 {%k7}
	vsubrps  %v31 {dacb}, %v8, %v15 {%k7}
	vsubrps  %v31 {dacb}, %v9, %v15 {%k7}
	vsubrps  %v31 {dacb}, %v15, %v15 {%k7}
	vsubrps  %v31 {dacb}, %v31, %v31 {%k7}
	vsubrps  (%rax), %v1, %v0{%k7}
	vsubrps  (%rax) {1to16}, %v1, %v0{%k7}
	vsubrps  (%rax) {4to16}, %v1, %v0{%k7}
	vsubrps  (%rax) {unorm8}, %v1, %v0{%k7}
	vsubrps  (%rax) {sint16}, %v1, %v0{%k7}
	vsubrps  (%rax) {float16}, %v1, %v0{%k7}
	vsubrps  (%rax) {uint8}, %v1, %v0{%k7}
	vsubrps  (%rax) {nt}, %v1, %v0{%k7}
	vsubrps  (%rax) {NT}, %v1, %v0{%k7}
	vsubrps  (%rax) {unorm8} {NT}, %v1, %v0{%k7}
	vsubrps  (%rax) {NT}{unorm8}, %v1, %v0{%k7}
	vsubrps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubrps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubrps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vandpi  %v2, %v1, %v0
	vandpi  %v2 {dcba}, %v1, %v0
	vandpi  %v2 {dcba}, %v1, %v0 {%k7}
	vandpi  %v2 {cdab}, %v1, %v0 {%k7}
	vandpi  %v2 {badc}, %v1, %v0 {%k7}
	vandpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vandpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vandpi  %v2 {cccc}, %v1, %v0 {%k7}
	vandpi  %v2 {dddd}, %v1, %v0 {%k7}
	vandpi  %v2 {dacb}, %v1, %v0 {%k7}
	vandpi  %v7 {dacb}, %v1, %v0 {%k7}
	vandpi  %v8 {dacb}, %v1, %v0 {%k7}
	vandpi  %v9 {dacb}, %v1, %v0 {%k7}
	vandpi  %v15 {dacb}, %v1, %v0 {%k7}
	vandpi  %v16 {dacb}, %v1, %v0 {%k7}
	vandpi  %v17 {dacb}, %v1, %v0 {%k7}
	vandpi  %v31 {dacb}, %v1, %v0 {%k7}
	vandpi  %v31 {dacb}, %v1, %v8 {%k7}
	vandpi  %v31 {dacb}, %v1, %v9 {%k7}
	vandpi  %v31 {dacb}, %v1, %v15 {%k7}
	vandpi  %v31 {dacb}, %v8, %v15 {%k7}
	vandpi  %v31 {dacb}, %v9, %v15 {%k7}
	vandpi  %v31 {dacb}, %v15, %v15 {%k7}
	vandpi  %v31 {dacb}, %v31, %v31 {%k7}
	vandpi  (%rax), %v1, %v0{%k7}
	vandpi  (%rax) {1to16}, %v1, %v0{%k7}
	vandpi  (%rax) {4to16}, %v1, %v0{%k7}
	vandpi  (%rax) {sint8}, %v1, %v0{%k7}
	vandpi  (%rax) {sint16}, %v1, %v0{%k7}
	vandpi  (%rax) {uint16}, %v1, %v0{%k7}
	vandpi  (%rax) {uint8}, %v1, %v0{%k7}
	vandpi  (%rax) {nt}, %v1, %v0{%k7}
	vandpi  (%rax) {NT}, %v1, %v0{%k7}
	vandpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vandpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vandpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vandpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vandpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vandnpi  %v2, %v1, %v0
	vandnpi  %v2 {dcba}, %v1, %v0
	vandnpi  %v2 {dcba}, %v1, %v0 {%k7}
	vandnpi  %v2 {cdab}, %v1, %v0 {%k7}
	vandnpi  %v2 {badc}, %v1, %v0 {%k7}
	vandnpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vandnpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vandnpi  %v2 {cccc}, %v1, %v0 {%k7}
	vandnpi  %v2 {dddd}, %v1, %v0 {%k7}
	vandnpi  %v2 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v7 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v8 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v9 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v15 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v16 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v17 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v31 {dacb}, %v1, %v0 {%k7}
	vandnpi  %v31 {dacb}, %v1, %v8 {%k7}
	vandnpi  %v31 {dacb}, %v1, %v9 {%k7}
	vandnpi  %v31 {dacb}, %v1, %v15 {%k7}
	vandnpi  %v31 {dacb}, %v8, %v15 {%k7}
	vandnpi  %v31 {dacb}, %v9, %v15 {%k7}
	vandnpi  %v31 {dacb}, %v15, %v15 {%k7}
	vandnpi  %v31 {dacb}, %v31, %v31 {%k7}
	vandnpi  (%rax), %v1, %v0{%k7}
	vandnpi  (%rax) {1to16}, %v1, %v0{%k7}
	vandnpi  (%rax) {4to16}, %v1, %v0{%k7}
	vandnpi  (%rax) {sint8}, %v1, %v0{%k7}
	vandnpi  (%rax) {sint16}, %v1, %v0{%k7}
	vandnpi  (%rax) {uint16}, %v1, %v0{%k7}
	vandnpi  (%rax) {uint8}, %v1, %v0{%k7}
	vandnpi  (%rax) {nt}, %v1, %v0{%k7}
	vandnpi  (%rax) {NT}, %v1, %v0{%k7}
	vandnpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vandnpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vandnpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vandnpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vandnpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vbitinterleave11pi  %v2, %v1, %v0
	vbitinterleave11pi  %v2 {dcba}, %v1, %v0
	vbitinterleave11pi  %v2 {dcba}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v2 {cdab}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v2 {badc}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v2 {aaaa}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v2 {bbbb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v2 {cccc}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v2 {dddd}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v2 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v7 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v8 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v9 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v15 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v16 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v17 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v1, %v0 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v1, %v8 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v1, %v9 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v1, %v15 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v8, %v15 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v9, %v15 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v15, %v15 {%k7}
	vbitinterleave11pi  %v31 {dacb}, %v31, %v31 {%k7}
	vbitinterleave11pi  (%rax), %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {1to16}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {4to16}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {sint8}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {sint16}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {uint16}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {uint8}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {nt}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {NT}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vbitinterleave11pi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vbitinterleave11pi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vbitinterleave11pi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vbitinterleave11pi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vbitinterleave21pi  %v2, %v1, %v0
	vbitinterleave21pi  %v2 {dcba}, %v1, %v0
	vbitinterleave21pi  %v2 {dcba}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v2 {cdab}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v2 {badc}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v2 {aaaa}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v2 {bbbb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v2 {cccc}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v2 {dddd}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v2 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v7 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v8 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v9 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v15 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v16 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v17 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v1, %v0 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v1, %v8 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v1, %v9 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v1, %v15 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v8, %v15 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v9, %v15 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v15, %v15 {%k7}
	vbitinterleave21pi  %v31 {dacb}, %v31, %v31 {%k7}
	vbitinterleave21pi  (%rax), %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {1to16}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {4to16}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {sint8}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {sint16}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {uint16}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {uint8}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {nt}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {NT}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vbitinterleave21pi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vbitinterleave21pi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vbitinterleave21pi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vbitinterleave21pi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vclampzpi  %v2, %v1, %v0
	vclampzpi  %v2 {dcba}, %v1, %v0
	vclampzpi  %v2 {dcba}, %v1, %v0 {%k7}
	vclampzpi  %v2 {cdab}, %v1, %v0 {%k7}
	vclampzpi  %v2 {badc}, %v1, %v0 {%k7}
	vclampzpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vclampzpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vclampzpi  %v2 {cccc}, %v1, %v0 {%k7}
	vclampzpi  %v2 {dddd}, %v1, %v0 {%k7}
	vclampzpi  %v2 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v7 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v8 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v9 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v15 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v16 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v17 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v31 {dacb}, %v1, %v0 {%k7}
	vclampzpi  %v31 {dacb}, %v1, %v8 {%k7}
	vclampzpi  %v31 {dacb}, %v1, %v9 {%k7}
	vclampzpi  %v31 {dacb}, %v1, %v15 {%k7}
	vclampzpi  %v31 {dacb}, %v8, %v15 {%k7}
	vclampzpi  %v31 {dacb}, %v9, %v15 {%k7}
	vclampzpi  %v31 {dacb}, %v15, %v15 {%k7}
	vclampzpi  %v31 {dacb}, %v31, %v31 {%k7}
	vclampzpi  (%rax), %v1, %v0{%k7}
	vclampzpi  (%rax) {1to16}, %v1, %v0{%k7}
	vclampzpi  (%rax) {4to16}, %v1, %v0{%k7}
	vclampzpi  (%rax) {sint8}, %v1, %v0{%k7}
	vclampzpi  (%rax) {sint16}, %v1, %v0{%k7}
	vclampzpi  (%rax) {uint16}, %v1, %v0{%k7}
	vclampzpi  (%rax) {uint8}, %v1, %v0{%k7}
	vclampzpi  (%rax) {nt}, %v1, %v0{%k7}
	vclampzpi  (%rax) {NT}, %v1, %v0{%k7}
	vclampzpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vclampzpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vclampzpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vclampzpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vclampzpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd231pi  %v2, %v1, %v0
	vmadd231pi  %v2 {dcba}, %v1, %v0
	vmadd231pi  %v2 {dcba}, %v1, %v0 {%k7}
	vmadd231pi  %v2 {cdab}, %v1, %v0 {%k7}
	vmadd231pi  %v2 {badc}, %v1, %v0 {%k7}
	vmadd231pi  %v2 {aaaa}, %v1, %v0 {%k7}
	vmadd231pi  %v2 {bbbb}, %v1, %v0 {%k7}
	vmadd231pi  %v2 {cccc}, %v1, %v0 {%k7}
	vmadd231pi  %v2 {dddd}, %v1, %v0 {%k7}
	vmadd231pi  %v2 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v7 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v8 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v9 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v15 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v16 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v17 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v31 {dacb}, %v1, %v0 {%k7}
	vmadd231pi  %v31 {dacb}, %v1, %v8 {%k7}
	vmadd231pi  %v31 {dacb}, %v1, %v9 {%k7}
	vmadd231pi  %v31 {dacb}, %v1, %v15 {%k7}
	vmadd231pi  %v31 {dacb}, %v8, %v15 {%k7}
	vmadd231pi  %v31 {dacb}, %v9, %v15 {%k7}
	vmadd231pi  %v31 {dacb}, %v15, %v15 {%k7}
	vmadd231pi  %v31 {dacb}, %v31, %v31 {%k7}
	vmadd231pi  (%rax), %v1, %v0{%k7}
	vmadd231pi  (%rax) {1to16}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {4to16}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {sint8}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {sint16}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {uint16}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {uint8}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {nt}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {NT}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vmadd231pi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vmadd231pi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd231pi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd231pi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddsetspi  %v2 {dcba}, %v1, %v0 {%k7}
	vaddsetspi  %v2 {cdab}, %v1, %v0 {%k7}
	vaddsetspi  %v2 {badc}, %v1, %v0 {%k7}
	vaddsetspi  %v2 {aaaa}, %v1, %v0 {%k7}
	vaddsetspi  %v2 {bbbb}, %v1, %v0 {%k7}
	vaddsetspi  %v2 {cccc}, %v1, %v0 {%k7}
	vaddsetspi  %v2 {dddd}, %v1, %v0 {%k7}
	vaddsetspi  %v2 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v7 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v8 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v9 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v15 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v16 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v17 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v31 {dacb}, %v1, %v0 {%k7}
	vaddsetspi  %v31 {dacb}, %v1, %v8 {%k7}
	vaddsetspi  %v31 {dacb}, %v1, %v9 {%k7}
	vaddsetspi  %v31 {dacb}, %v1, %v15 {%k7}
	vaddsetspi  %v31 {dacb}, %v8, %v15 {%k7}
	vaddsetspi  %v31 {dacb}, %v9, %v15 {%k7}
	vaddsetspi  %v31 {dacb}, %v15, %v15 {%k7}
	vaddsetspi  %v31 {dacb}, %v31, %v31 {%k7}
	vaddsetspi  (%rax), %v1, %v0{%k7}
	vaddsetspi  (%rax) {1to16}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {4to16}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {sint8}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {sint16}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {uint16}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {uint8}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {nt}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {NT}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vaddsetspi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vaddsetspi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddsetspi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vaddsetspi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd233pi  %v2, %v1, %v0
	vmadd233pi  %v2 {dcba}, %v1, %v0
	vmadd233pi  %v2 {dcba}, %v1, %v0 {%k7}
	vmadd233pi  %v2 {cdab}, %v1, %v0 {%k7}
	vmadd233pi  %v2 {badc}, %v1, %v0 {%k7}
	vmadd233pi  %v2 {aaaa}, %v1, %v0 {%k7}
	vmadd233pi  %v2 {bbbb}, %v1, %v0 {%k7}
	vmadd233pi  %v2 {cccc}, %v1, %v0 {%k7}
	vmadd233pi  %v2 {dddd}, %v1, %v0 {%k7}
	vmadd233pi  %v2 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v7 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v8 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v9 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v15 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v16 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v17 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v31 {dacb}, %v1, %v0 {%k7}
	vmadd233pi  %v31 {dacb}, %v1, %v8 {%k7}
	vmadd233pi  %v31 {dacb}, %v1, %v9 {%k7}
	vmadd233pi  %v31 {dacb}, %v1, %v15 {%k7}
	vmadd233pi  %v31 {dacb}, %v8, %v15 {%k7}
	vmadd233pi  %v31 {dacb}, %v9, %v15 {%k7}
	vmadd233pi  %v31 {dacb}, %v15, %v15 {%k7}
	vmadd233pi  %v31 {dacb}, %v31, %v31 {%k7}
	vmadd233pi  (%rax), %v1, %v0{%k7}
	vmadd233pi  (%rax) {1to16}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {4to16}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {sint8}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {sint16}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {uint16}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {uint8}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {nt}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {NT}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vmadd233pi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vmadd233pi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd233pi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmadd233pi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxpi  %v2, %v1, %v0
	vmaxpi  %v2 {dcba}, %v1, %v0
	vmaxpi  %v2 {dcba}, %v1, %v0 {%k7}
	vmaxpi  %v2 {cdab}, %v1, %v0 {%k7}
	vmaxpi  %v2 {badc}, %v1, %v0 {%k7}
	vmaxpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vmaxpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vmaxpi  %v2 {cccc}, %v1, %v0 {%k7}
	vmaxpi  %v2 {dddd}, %v1, %v0 {%k7}
	vmaxpi  %v2 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v7 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v8 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v9 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v15 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v16 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v17 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v31 {dacb}, %v1, %v0 {%k7}
	vmaxpi  %v31 {dacb}, %v1, %v8 {%k7}
	vmaxpi  %v31 {dacb}, %v1, %v9 {%k7}
	vmaxpi  %v31 {dacb}, %v1, %v15 {%k7}
	vmaxpi  %v31 {dacb}, %v8, %v15 {%k7}
	vmaxpi  %v31 {dacb}, %v9, %v15 {%k7}
	vmaxpi  %v31 {dacb}, %v15, %v15 {%k7}
	vmaxpi  %v31 {dacb}, %v31, %v31 {%k7}
	vmaxpi  (%rax), %v1, %v0{%k7}
	vmaxpi  (%rax) {1to16}, %v1, %v0{%k7}
	vmaxpi  (%rax) {4to16}, %v1, %v0{%k7}
	vmaxpi  (%rax) {sint8}, %v1, %v0{%k7}
	vmaxpi  (%rax) {sint16}, %v1, %v0{%k7}
	vmaxpi  (%rax) {uint16}, %v1, %v0{%k7}
	vmaxpi  (%rax) {uint8}, %v1, %v0{%k7}
	vmaxpi  (%rax) {nt}, %v1, %v0{%k7}
	vmaxpi  (%rax) {NT}, %v1, %v0{%k7}
	vmaxpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vmaxpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vmaxpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxpu  %v2, %v1, %v0
	vmaxpu  %v2 {dcba}, %v1, %v0
	vmaxpu  %v2 {dcba}, %v1, %v0 {%k7}
	vmaxpu  %v2 {cdab}, %v1, %v0 {%k7}
	vmaxpu  %v2 {badc}, %v1, %v0 {%k7}
	vmaxpu  %v2 {aaaa}, %v1, %v0 {%k7}
	vmaxpu  %v2 {bbbb}, %v1, %v0 {%k7}
	vmaxpu  %v2 {cccc}, %v1, %v0 {%k7}
	vmaxpu  %v2 {dddd}, %v1, %v0 {%k7}
	vmaxpu  %v2 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v7 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v8 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v9 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v15 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v16 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v17 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v31 {dacb}, %v1, %v0 {%k7}
	vmaxpu  %v31 {dacb}, %v1, %v8 {%k7}
	vmaxpu  %v31 {dacb}, %v1, %v9 {%k7}
	vmaxpu  %v31 {dacb}, %v1, %v15 {%k7}
	vmaxpu  %v31 {dacb}, %v8, %v15 {%k7}
	vmaxpu  %v31 {dacb}, %v9, %v15 {%k7}
	vmaxpu  %v31 {dacb}, %v15, %v15 {%k7}
	vmaxpu  %v31 {dacb}, %v31, %v31 {%k7}
	vmaxpu  (%rax), %v1, %v0{%k7}
	vmaxpu  (%rax) {1to16}, %v1, %v0{%k7}
	vmaxpu  (%rax) {4to16}, %v1, %v0{%k7}
	vmaxpu  (%rax) {sint8}, %v1, %v0{%k7}
	vmaxpu  (%rax) {sint16}, %v1, %v0{%k7}
	vmaxpu  (%rax) {uint16}, %v1, %v0{%k7}
	vmaxpu  (%rax) {uint8}, %v1, %v0{%k7}
	vmaxpu  (%rax) {nt}, %v1, %v0{%k7}
	vmaxpu  (%rax) {NT}, %v1, %v0{%k7}
	vmaxpu  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vmaxpu  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vmaxpu  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxpu  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmaxpu  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminpi  %v2, %v1, %v0
	vminpi  %v2 {dcba}, %v1, %v0
	vminpi  %v2 {dcba}, %v1, %v0 {%k7}
	vminpi  %v2 {cdab}, %v1, %v0 {%k7}
	vminpi  %v2 {badc}, %v1, %v0 {%k7}
	vminpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vminpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vminpi  %v2 {cccc}, %v1, %v0 {%k7}
	vminpi  %v2 {dddd}, %v1, %v0 {%k7}
	vminpi  %v2 {dacb}, %v1, %v0 {%k7}
	vminpi  %v7 {dacb}, %v1, %v0 {%k7}
	vminpi  %v8 {dacb}, %v1, %v0 {%k7}
	vminpi  %v9 {dacb}, %v1, %v0 {%k7}
	vminpi  %v15 {dacb}, %v1, %v0 {%k7}
	vminpi  %v16 {dacb}, %v1, %v0 {%k7}
	vminpi  %v17 {dacb}, %v1, %v0 {%k7}
	vminpi  %v31 {dacb}, %v1, %v0 {%k7}
	vminpi  %v31 {dacb}, %v1, %v8 {%k7}
	vminpi  %v31 {dacb}, %v1, %v9 {%k7}
	vminpi  %v31 {dacb}, %v1, %v15 {%k7}
	vminpi  %v31 {dacb}, %v8, %v15 {%k7}
	vminpi  %v31 {dacb}, %v9, %v15 {%k7}
	vminpi  %v31 {dacb}, %v15, %v15 {%k7}
	vminpi  %v31 {dacb}, %v31, %v31 {%k7}
	vminpi  (%rax), %v1, %v0{%k7}
	vminpi  (%rax) {1to16}, %v1, %v0{%k7}
	vminpi  (%rax) {4to16}, %v1, %v0{%k7}
	vminpi  (%rax) {sint8}, %v1, %v0{%k7}
	vminpi  (%rax) {sint16}, %v1, %v0{%k7}
	vminpi  (%rax) {uint16}, %v1, %v0{%k7}
	vminpi  (%rax) {uint8}, %v1, %v0{%k7}
	vminpi  (%rax) {nt}, %v1, %v0{%k7}
	vminpi  (%rax) {NT}, %v1, %v0{%k7}
	vminpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vminpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vminpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminpu  %v2, %v1, %v0
	vminpu  %v2 {dcba}, %v1, %v0
	vminpu  %v2 {dcba}, %v1, %v0 {%k7}
	vminpu  %v2 {cdab}, %v1, %v0 {%k7}
	vminpu  %v2 {badc}, %v1, %v0 {%k7}
	vminpu  %v2 {aaaa}, %v1, %v0 {%k7}
	vminpu  %v2 {bbbb}, %v1, %v0 {%k7}
	vminpu  %v2 {cccc}, %v1, %v0 {%k7}
	vminpu  %v2 {dddd}, %v1, %v0 {%k7}
	vminpu  %v2 {dacb}, %v1, %v0 {%k7}
	vminpu  %v7 {dacb}, %v1, %v0 {%k7}
	vminpu  %v8 {dacb}, %v1, %v0 {%k7}
	vminpu  %v9 {dacb}, %v1, %v0 {%k7}
	vminpu  %v15 {dacb}, %v1, %v0 {%k7}
	vminpu  %v16 {dacb}, %v1, %v0 {%k7}
	vminpu  %v17 {dacb}, %v1, %v0 {%k7}
	vminpu  %v31 {dacb}, %v1, %v0 {%k7}
	vminpu  %v31 {dacb}, %v1, %v8 {%k7}
	vminpu  %v31 {dacb}, %v1, %v9 {%k7}
	vminpu  %v31 {dacb}, %v1, %v15 {%k7}
	vminpu  %v31 {dacb}, %v8, %v15 {%k7}
	vminpu  %v31 {dacb}, %v9, %v15 {%k7}
	vminpu  %v31 {dacb}, %v15, %v15 {%k7}
	vminpu  %v31 {dacb}, %v31, %v31 {%k7}
	vminpu  (%rax), %v1, %v0{%k7}
	vminpu  (%rax) {1to16}, %v1, %v0{%k7}
	vminpu  (%rax) {4to16}, %v1, %v0{%k7}
	vminpu  (%rax) {sint8}, %v1, %v0{%k7}
	vminpu  (%rax) {sint16}, %v1, %v0{%k7}
	vminpu  (%rax) {uint16}, %v1, %v0{%k7}
	vminpu  (%rax) {uint8}, %v1, %v0{%k7}
	vminpu  (%rax) {nt}, %v1, %v0{%k7}
	vminpu  (%rax) {NT}, %v1, %v0{%k7}
	vminpu  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vminpu  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vminpu  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminpu  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vminpu  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulhpi  %v2, %v1, %v0
	vmulhpi  %v2 {dcba}, %v1, %v0
	vmulhpi  %v2 {dcba}, %v1, %v0 {%k7}
	vmulhpi  %v2 {cdab}, %v1, %v0 {%k7}
	vmulhpi  %v2 {badc}, %v1, %v0 {%k7}
	vmulhpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vmulhpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vmulhpi  %v2 {cccc}, %v1, %v0 {%k7}
	vmulhpi  %v2 {dddd}, %v1, %v0 {%k7}
	vmulhpi  %v2 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v7 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v8 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v9 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v15 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v16 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v17 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v31 {dacb}, %v1, %v0 {%k7}
	vmulhpi  %v31 {dacb}, %v1, %v8 {%k7}
	vmulhpi  %v31 {dacb}, %v1, %v9 {%k7}
	vmulhpi  %v31 {dacb}, %v1, %v15 {%k7}
	vmulhpi  %v31 {dacb}, %v8, %v15 {%k7}
	vmulhpi  %v31 {dacb}, %v9, %v15 {%k7}
	vmulhpi  %v31 {dacb}, %v15, %v15 {%k7}
	vmulhpi  %v31 {dacb}, %v31, %v31 {%k7}
	vmulhpi  (%rax), %v1, %v0{%k7}
	vmulhpi  (%rax) {1to16}, %v1, %v0{%k7}
	vmulhpi  (%rax) {4to16}, %v1, %v0{%k7}
	vmulhpi  (%rax) {sint8}, %v1, %v0{%k7}
	vmulhpi  (%rax) {sint16}, %v1, %v0{%k7}
	vmulhpi  (%rax) {uint16}, %v1, %v0{%k7}
	vmulhpi  (%rax) {uint8}, %v1, %v0{%k7}
	vmulhpi  (%rax) {nt}, %v1, %v0{%k7}
	vmulhpi  (%rax) {NT}, %v1, %v0{%k7}
	vmulhpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vmulhpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vmulhpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulhpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulhpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulhpu  %v2, %v1, %v0
	vmulhpu  %v2 {dcba}, %v1, %v0
	vmulhpu  %v2 {dcba}, %v1, %v0 {%k7}
	vmulhpu  %v2 {cdab}, %v1, %v0 {%k7}
	vmulhpu  %v2 {badc}, %v1, %v0 {%k7}
	vmulhpu  %v2 {aaaa}, %v1, %v0 {%k7}
	vmulhpu  %v2 {bbbb}, %v1, %v0 {%k7}
	vmulhpu  %v2 {cccc}, %v1, %v0 {%k7}
	vmulhpu  %v2 {dddd}, %v1, %v0 {%k7}
	vmulhpu  %v2 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v7 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v8 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v9 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v15 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v16 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v17 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v31 {dacb}, %v1, %v0 {%k7}
	vmulhpu  %v31 {dacb}, %v1, %v8 {%k7}
	vmulhpu  %v31 {dacb}, %v1, %v9 {%k7}
	vmulhpu  %v31 {dacb}, %v1, %v15 {%k7}
	vmulhpu  %v31 {dacb}, %v8, %v15 {%k7}
	vmulhpu  %v31 {dacb}, %v9, %v15 {%k7}
	vmulhpu  %v31 {dacb}, %v15, %v15 {%k7}
	vmulhpu  %v31 {dacb}, %v31, %v31 {%k7}
	vmulhpu  (%rax), %v1, %v0{%k7}
	vmulhpu  (%rax) {1to16}, %v1, %v0{%k7}
	vmulhpu  (%rax) {4to16}, %v1, %v0{%k7}
	vmulhpu  (%rax) {sint8}, %v1, %v0{%k7}
	vmulhpu  (%rax) {sint16}, %v1, %v0{%k7}
	vmulhpu  (%rax) {uint16}, %v1, %v0{%k7}
	vmulhpu  (%rax) {uint8}, %v1, %v0{%k7}
	vmulhpu  (%rax) {nt}, %v1, %v0{%k7}
	vmulhpu  (%rax) {NT}, %v1, %v0{%k7}
	vmulhpu  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vmulhpu  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vmulhpu  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulhpu  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmulhpu  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmullpi  %v2, %v1, %v0
	vmullpi  %v2 {dcba}, %v1, %v0
	vmullpi  %v2 {dcba}, %v1, %v0 {%k7}
	vmullpi  %v2 {cdab}, %v1, %v0 {%k7}
	vmullpi  %v2 {badc}, %v1, %v0 {%k7}
	vmullpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vmullpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vmullpi  %v2 {cccc}, %v1, %v0 {%k7}
	vmullpi  %v2 {dddd}, %v1, %v0 {%k7}
	vmullpi  %v2 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v7 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v8 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v9 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v15 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v16 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v17 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v31 {dacb}, %v1, %v0 {%k7}
	vmullpi  %v31 {dacb}, %v1, %v8 {%k7}
	vmullpi  %v31 {dacb}, %v1, %v9 {%k7}
	vmullpi  %v31 {dacb}, %v1, %v15 {%k7}
	vmullpi  %v31 {dacb}, %v8, %v15 {%k7}
	vmullpi  %v31 {dacb}, %v9, %v15 {%k7}
	vmullpi  %v31 {dacb}, %v15, %v15 {%k7}
	vmullpi  %v31 {dacb}, %v31, %v31 {%k7}
	vmullpi  (%rax), %v1, %v0{%k7}
	vmullpi  (%rax) {1to16}, %v1, %v0{%k7}
	vmullpi  (%rax) {4to16}, %v1, %v0{%k7}
	vmullpi  (%rax) {sint8}, %v1, %v0{%k7}
	vmullpi  (%rax) {sint16}, %v1, %v0{%k7}
	vmullpi  (%rax) {uint16}, %v1, %v0{%k7}
	vmullpi  (%rax) {uint8}, %v1, %v0{%k7}
	vmullpi  (%rax) {nt}, %v1, %v0{%k7}
	vmullpi  (%rax) {NT}, %v1, %v0{%k7}
	vmullpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vmullpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vmullpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmullpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vmullpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vorpi  %v2, %v1, %v0
	vorpi  %v2 {dcba}, %v1, %v0
	vorpi  %v2 {dcba}, %v1, %v0 {%k7}
	vorpi  %v2 {cdab}, %v1, %v0 {%k7}
	vorpi  %v2 {badc}, %v1, %v0 {%k7}
	vorpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vorpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vorpi  %v2 {cccc}, %v1, %v0 {%k7}
	vorpi  %v2 {dddd}, %v1, %v0 {%k7}
	vorpi  %v2 {dacb}, %v1, %v0 {%k7}
	vorpi  %v7 {dacb}, %v1, %v0 {%k7}
	vorpi  %v8 {dacb}, %v1, %v0 {%k7}
	vorpi  %v9 {dacb}, %v1, %v0 {%k7}
	vorpi  %v15 {dacb}, %v1, %v0 {%k7}
	vorpi  %v16 {dacb}, %v1, %v0 {%k7}
	vorpi  %v17 {dacb}, %v1, %v0 {%k7}
	vorpi  %v31 {dacb}, %v1, %v0 {%k7}
	vorpi  %v31 {dacb}, %v1, %v8 {%k7}
	vorpi  %v31 {dacb}, %v1, %v9 {%k7}
	vorpi  %v31 {dacb}, %v1, %v15 {%k7}
	vorpi  %v31 {dacb}, %v8, %v15 {%k7}
	vorpi  %v31 {dacb}, %v9, %v15 {%k7}
	vorpi  %v31 {dacb}, %v15, %v15 {%k7}
	vorpi  %v31 {dacb}, %v31, %v31 {%k7}
	vorpi  (%rax), %v1, %v0{%k7}
	vorpi  (%rax) {1to16}, %v1, %v0{%k7}
	vorpi  (%rax) {4to16}, %v1, %v0{%k7}
	vorpi  (%rax) {sint8}, %v1, %v0{%k7}
	vorpi  (%rax) {sint16}, %v1, %v0{%k7}
	vorpi  (%rax) {uint16}, %v1, %v0{%k7}
	vorpi  (%rax) {uint8}, %v1, %v0{%k7}
	vorpi  (%rax) {nt}, %v1, %v0{%k7}
	vorpi  (%rax) {NT}, %v1, %v0{%k7}
	vorpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vorpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vorpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vorpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vorpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsllpi  %v2, %v1, %v0
	vsllpi  %v2 {dcba}, %v1, %v0
	vsllpi  %v2 {dcba}, %v1, %v0 {%k7}
	vsllpi  %v2 {cdab}, %v1, %v0 {%k7}
	vsllpi  %v2 {badc}, %v1, %v0 {%k7}
	vsllpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vsllpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vsllpi  %v2 {cccc}, %v1, %v0 {%k7}
	vsllpi  %v2 {dddd}, %v1, %v0 {%k7}
	vsllpi  %v2 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v7 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v8 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v9 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v15 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v16 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v17 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v31 {dacb}, %v1, %v0 {%k7}
	vsllpi  %v31 {dacb}, %v1, %v8 {%k7}
	vsllpi  %v31 {dacb}, %v1, %v9 {%k7}
	vsllpi  %v31 {dacb}, %v1, %v15 {%k7}
	vsllpi  %v31 {dacb}, %v8, %v15 {%k7}
	vsllpi  %v31 {dacb}, %v9, %v15 {%k7}
	vsllpi  %v31 {dacb}, %v15, %v15 {%k7}
	vsllpi  %v31 {dacb}, %v31, %v31 {%k7}
	vsllpi  (%rax), %v1, %v0{%k7}
	vsllpi  (%rax) {1to16}, %v1, %v0{%k7}
	vsllpi  (%rax) {4to16}, %v1, %v0{%k7}
	vsllpi  (%rax) {sint8}, %v1, %v0{%k7}
	vsllpi  (%rax) {sint16}, %v1, %v0{%k7}
	vsllpi  (%rax) {uint16}, %v1, %v0{%k7}
	vsllpi  (%rax) {uint8}, %v1, %v0{%k7}
	vsllpi  (%rax) {nt}, %v1, %v0{%k7}
	vsllpi  (%rax) {NT}, %v1, %v0{%k7}
	vsllpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vsllpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vsllpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsllpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsllpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsrapi  %v2, %v1, %v0
	vsrapi  %v2 {dcba}, %v1, %v0
	vsrapi  %v2 {dcba}, %v1, %v0 {%k7}
	vsrapi  %v2 {cdab}, %v1, %v0 {%k7}
	vsrapi  %v2 {badc}, %v1, %v0 {%k7}
	vsrapi  %v2 {aaaa}, %v1, %v0 {%k7}
	vsrapi  %v2 {bbbb}, %v1, %v0 {%k7}
	vsrapi  %v2 {cccc}, %v1, %v0 {%k7}
	vsrapi  %v2 {dddd}, %v1, %v0 {%k7}
	vsrapi  %v2 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v7 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v8 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v9 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v15 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v16 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v17 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v31 {dacb}, %v1, %v0 {%k7}
	vsrapi  %v31 {dacb}, %v1, %v8 {%k7}
	vsrapi  %v31 {dacb}, %v1, %v9 {%k7}
	vsrapi  %v31 {dacb}, %v1, %v15 {%k7}
	vsrapi  %v31 {dacb}, %v8, %v15 {%k7}
	vsrapi  %v31 {dacb}, %v9, %v15 {%k7}
	vsrapi  %v31 {dacb}, %v15, %v15 {%k7}
	vsrapi  %v31 {dacb}, %v31, %v31 {%k7}
	vsrapi  (%rax), %v1, %v0{%k7}
	vsrapi  (%rax) {1to16}, %v1, %v0{%k7}
	vsrapi  (%rax) {4to16}, %v1, %v0{%k7}
	vsrapi  (%rax) {sint8}, %v1, %v0{%k7}
	vsrapi  (%rax) {sint16}, %v1, %v0{%k7}
	vsrapi  (%rax) {uint16}, %v1, %v0{%k7}
	vsrapi  (%rax) {uint8}, %v1, %v0{%k7}
	vsrapi  (%rax) {nt}, %v1, %v0{%k7}
	vsrapi  (%rax) {NT}, %v1, %v0{%k7}
	vsrapi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vsrapi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vsrapi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsrapi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsrapi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsrlpi  %v2, %v1, %v0
	vsrlpi  %v2 {dcba}, %v1, %v0
	vsrlpi  %v2 {dcba}, %v1, %v0 {%k7}
	vsrlpi  %v2 {cdab}, %v1, %v0 {%k7}
	vsrlpi  %v2 {badc}, %v1, %v0 {%k7}
	vsrlpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vsrlpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vsrlpi  %v2 {cccc}, %v1, %v0 {%k7}
	vsrlpi  %v2 {dddd}, %v1, %v0 {%k7}
	vsrlpi  %v2 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v7 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v8 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v9 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v15 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v16 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v17 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v31 {dacb}, %v1, %v0 {%k7}
	vsrlpi  %v31 {dacb}, %v1, %v8 {%k7}
	vsrlpi  %v31 {dacb}, %v1, %v9 {%k7}
	vsrlpi  %v31 {dacb}, %v1, %v15 {%k7}
	vsrlpi  %v31 {dacb}, %v8, %v15 {%k7}
	vsrlpi  %v31 {dacb}, %v9, %v15 {%k7}
	vsrlpi  %v31 {dacb}, %v15, %v15 {%k7}
	vsrlpi  %v31 {dacb}, %v31, %v31 {%k7}
	vsrlpi  (%rax), %v1, %v0{%k7}
	vsrlpi  (%rax) {1to16}, %v1, %v0{%k7}
	vsrlpi  (%rax) {4to16}, %v1, %v0{%k7}
	vsrlpi  (%rax) {sint8}, %v1, %v0{%k7}
	vsrlpi  (%rax) {sint16}, %v1, %v0{%k7}
	vsrlpi  (%rax) {uint16}, %v1, %v0{%k7}
	vsrlpi  (%rax) {uint8}, %v1, %v0{%k7}
	vsrlpi  (%rax) {nt}, %v1, %v0{%k7}
	vsrlpi  (%rax) {NT}, %v1, %v0{%k7}
	vsrlpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vsrlpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vsrlpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsrlpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsrlpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubpi  %v2, %v1, %v0
	vsubpi  %v2 {dcba}, %v1, %v0
	vsubpi  %v2 {dcba}, %v1, %v0 {%k7}
	vsubpi  %v2 {cdab}, %v1, %v0 {%k7}
	vsubpi  %v2 {badc}, %v1, %v0 {%k7}
	vsubpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vsubpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vsubpi  %v2 {cccc}, %v1, %v0 {%k7}
	vsubpi  %v2 {dddd}, %v1, %v0 {%k7}
	vsubpi  %v2 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v7 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v8 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v9 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v15 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v16 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v17 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v31 {dacb}, %v1, %v0 {%k7}
	vsubpi  %v31 {dacb}, %v1, %v8 {%k7}
	vsubpi  %v31 {dacb}, %v1, %v9 {%k7}
	vsubpi  %v31 {dacb}, %v1, %v15 {%k7}
	vsubpi  %v31 {dacb}, %v8, %v15 {%k7}
	vsubpi  %v31 {dacb}, %v9, %v15 {%k7}
	vsubpi  %v31 {dacb}, %v15, %v15 {%k7}
	vsubpi  %v31 {dacb}, %v31, %v31 {%k7}
	vsubpi  (%rax), %v1, %v0{%k7}
	vsubpi  (%rax) {1to16}, %v1, %v0{%k7}
	vsubpi  (%rax) {4to16}, %v1, %v0{%k7}
	vsubpi  (%rax) {sint8}, %v1, %v0{%k7}
	vsubpi  (%rax) {sint16}, %v1, %v0{%k7}
	vsubpi  (%rax) {uint16}, %v1, %v0{%k7}
	vsubpi  (%rax) {uint8}, %v1, %v0{%k7}
	vsubpi  (%rax) {nt}, %v1, %v0{%k7}
	vsubpi  (%rax) {NT}, %v1, %v0{%k7}
	vsubpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vsubpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vsubpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubrpi  %v2, %v1, %v0
	vsubrpi  %v2 {dcba}, %v1, %v0
	vsubrpi  %v2 {dcba}, %v1, %v0 {%k7}
	vsubrpi  %v2 {cdab}, %v1, %v0 {%k7}
	vsubrpi  %v2 {badc}, %v1, %v0 {%k7}
	vsubrpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vsubrpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vsubrpi  %v2 {cccc}, %v1, %v0 {%k7}
	vsubrpi  %v2 {dddd}, %v1, %v0 {%k7}
	vsubrpi  %v2 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v7 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v8 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v9 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v15 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v16 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v17 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v31 {dacb}, %v1, %v0 {%k7}
	vsubrpi  %v31 {dacb}, %v1, %v8 {%k7}
	vsubrpi  %v31 {dacb}, %v1, %v9 {%k7}
	vsubrpi  %v31 {dacb}, %v1, %v15 {%k7}
	vsubrpi  %v31 {dacb}, %v8, %v15 {%k7}
	vsubrpi  %v31 {dacb}, %v9, %v15 {%k7}
	vsubrpi  %v31 {dacb}, %v15, %v15 {%k7}
	vsubrpi  %v31 {dacb}, %v31, %v31 {%k7}
	vsubrpi  (%rax), %v1, %v0{%k7}
	vsubrpi  (%rax) {1to16}, %v1, %v0{%k7}
	vsubrpi  (%rax) {4to16}, %v1, %v0{%k7}
	vsubrpi  (%rax) {sint8}, %v1, %v0{%k7}
	vsubrpi  (%rax) {sint16}, %v1, %v0{%k7}
	vsubrpi  (%rax) {uint16}, %v1, %v0{%k7}
	vsubrpi  (%rax) {uint8}, %v1, %v0{%k7}
	vsubrpi  (%rax) {nt}, %v1, %v0{%k7}
	vsubrpi  (%rax) {NT}, %v1, %v0{%k7}
	vsubrpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vsubrpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vsubrpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubrpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vsubrpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vxorpi  %v2, %v1, %v0
	vxorpi  %v2 {dcba}, %v1, %v0
	vxorpi  %v2 {dcba}, %v1, %v0 {%k7}
	vxorpi  %v2 {cdab}, %v1, %v0 {%k7}
	vxorpi  %v2 {badc}, %v1, %v0 {%k7}
	vxorpi  %v2 {aaaa}, %v1, %v0 {%k7}
	vxorpi  %v2 {bbbb}, %v1, %v0 {%k7}
	vxorpi  %v2 {cccc}, %v1, %v0 {%k7}
	vxorpi  %v2 {dddd}, %v1, %v0 {%k7}
	vxorpi  %v2 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v7 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v8 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v9 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v15 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v16 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v17 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v31 {dacb}, %v1, %v0 {%k7}
	vxorpi  %v31 {dacb}, %v1, %v8 {%k7}
	vxorpi  %v31 {dacb}, %v1, %v9 {%k7}
	vxorpi  %v31 {dacb}, %v1, %v15 {%k7}
	vxorpi  %v31 {dacb}, %v8, %v15 {%k7}
	vxorpi  %v31 {dacb}, %v9, %v15 {%k7}
	vxorpi  %v31 {dacb}, %v15, %v15 {%k7}
	vxorpi  %v31 {dacb}, %v31, %v31 {%k7}
	vxorpi  (%rax), %v1, %v0{%k7}
	vxorpi  (%rax) {1to16}, %v1, %v0{%k7}
	vxorpi  (%rax) {4to16}, %v1, %v0{%k7}
	vxorpi  (%rax) {sint8}, %v1, %v0{%k7}
	vxorpi  (%rax) {sint16}, %v1, %v0{%k7}
	vxorpi  (%rax) {uint16}, %v1, %v0{%k7}
	vxorpi  (%rax) {uint8}, %v1, %v0{%k7}
	vxorpi  (%rax) {nt}, %v1, %v0{%k7}
	vxorpi  (%rax) {NT}, %v1, %v0{%k7}
	vxorpi  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vxorpi  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vxorpi  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vxorpi  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vxorpi  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vscaleps  %v2, %v1, %v0
	vscaleps  %v2 {dcba}, %v1, %v0
	vscaleps  %v2 {dcba}, %v1, %v0 {%k7}
	vscaleps  %v2 {cdab}, %v1, %v0 {%k7}
	vscaleps  %v2 {badc}, %v1, %v0 {%k7}
	vscaleps  %v2 {aaaa}, %v1, %v0 {%k7}
	vscaleps  %v2 {bbbb}, %v1, %v0 {%k7}
	vscaleps  %v2 {cccc}, %v1, %v0 {%k7}
	vscaleps  %v2 {dddd}, %v1, %v0 {%k7}
	vscaleps  %v2 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v7 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v8 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v9 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v15 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v16 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v17 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v31 {dacb}, %v1, %v0 {%k7}
	vscaleps  %v31 {dacb}, %v1, %v8 {%k7}
	vscaleps  %v31 {dacb}, %v1, %v9 {%k7}
	vscaleps  %v31 {dacb}, %v1, %v15 {%k7}
	vscaleps  %v31 {dacb}, %v8, %v15 {%k7}
	vscaleps  %v31 {dacb}, %v9, %v15 {%k7}
	vscaleps  %v31 {dacb}, %v15, %v15 {%k7}
	vscaleps  %v31 {dacb}, %v31, %v31 {%k7}
	vscaleps  (%rax), %v1, %v0{%k7}
	vscaleps  (%rax) {1to16}, %v1, %v0{%k7}
	vscaleps  (%rax) {4to16}, %v1, %v0{%k7}
	vscaleps  (%rax) {sint8}, %v1, %v0{%k7}
	vscaleps  (%rax) {sint16}, %v1, %v0{%k7}
	vscaleps  (%rax) {uint16}, %v1, %v0{%k7}
	vscaleps  (%rax) {uint8}, %v1, %v0{%k7}
	vscaleps  (%rax) {nt}, %v1, %v0{%k7}
	vscaleps  (%rax) {NT}, %v1, %v0{%k7}
	vscaleps  (%rax) {sint8} {NT}, %v1, %v0{%k7}
	vscaleps  (%rax) {NT}{sint8}, %v1, %v0{%k7}
	vscaleps  4(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vscaleps  64(%r8,%r9,1){NT}, %v1, %v0{%k7}
	vscaleps  -64(%r8,%r9,1){NT}, %v1, %v0{%k7}

vloadd 0x40(%rcx){sint8}{16to16},%v1{%k1}
vloadd 0x100(%rcx){sint8}{16to16},%v1{%k1}
vloadd 0x40(%rcx){unorm2D}{16to16},%v1{%k1}
vloadd 0x100(%rcx){unorm2D}{16to16},%v1{%k1}
vloadd 0x40(%rcx){float11A}{16to16},%v1{%k1}
vloadd 0x100(%rcx){float11A}{16to16},%v1{%k1}
vloadd 0x40(%rcx){sint8}{4to16},%v1{%k1}
vloadd 0x100(%rcx){sint8}{4to16},%v1{%k1}
vloadd 0x40(%rcx){unorm2D}{4to16},%v1{%k1}
vloadd 0x100(%rcx){unorm2D}{4to16},%v1{%k1}
vloadd 0x40(%rcx){float11A}{4to16},%v1{%k1}
vloadd 0x100(%rcx){float11A}{4to16},%v1{%k1}
vloadunpackhd 0x40(%rcx){sint8},%v1{%k1}
vloadunpackhd 0x100(%rcx){sint8},%v1{%k1}
vloadunpackhd 0x40(%rcx){sint16},%v1{%k1}
vloadunpackhd 0x80(%rcx){sint16},%v1{%k1}
vloadunpackhd 0x40(%rcx){unorm2D},%v1{%k1}
vloadunpackhd 0x100(%rcx){unorm2D},%v1{%k1}
vloadunpackhd 0x40(%rcx){float11A},%v1{%k1}
vloadunpackhd 0x100(%rcx){float11A},%v1{%k1}

	.intel_syntax noprefix
vkmov  k0, bx
vkmov  k0, ebx
vkmov  k1, ax
vkmov  k1, eax
vkmov  k2, ax
vkmov  k2, eax
vkmov  k3, ax
vkmov  k3, eax
		bitinterleave11       ax, ax
                    bitinterleave11       eax, eax
                    bitinterleave11       rax, rax
                    vprefetch1       [edx], 0
                    vprefetch1       [edx], 1
                    vprefetch1       [edx], 7
                    vprefetch2       [edx], 0
                    vprefetch2       [edx], 1
                    vprefetch2       [edx], 3
                    vprefetch1       [edx + 64], 0
                    clevict1         [edx]
                    delay              ax
                    delay              eax
                    delay              rax
                    delay              r12w
                    delay              r12d
                    delay              r12
                    insertfield        ax, ax, 0, 0, 0
                    insertfield        ax, ax, 65535, 0, 0
                    insertfield        ax, ax, 0, 255, 0
                    insertfield        ax, ax, 65535, 255, 0
                    insertfield        ax, ax, 0, 0, 255
                    insertfield        ax, ax, 65535, 255, 255
	vshuf128x32 v0, DS: 0xffffffffcaf00000, {3110}, {aadd}
	vshuf128x32 v0, CS: 0xffffffffcaf00000, {3110}, {aadd}
	vmaxabsps v0,v1,v2
	vmaxabsps v0,v1,v2
	vmaxabsps v0{k7},v1,v2
	vmaxabsps v0{k7},v1,v2{cdab}
	vmaxabsps v0{k7},v1,v2{badc}
	vmaxabsps v0{k7},v1,v2{aaaa}
	vmaxabsps v0{k7},v1,v2{bbbb}
	vmaxabsps v0{k7},v1,v2{cccc}
	vmaxabsps v0{k7},v1,v2{dddd}
	vmaxabsps v0{k7},v1,v2{dacb}
	vmaxabsps v0{k7},v1,v7{dacb}
	vmaxabsps v0{k7},v1,v8{dacb}
	vmaxabsps v0{k7},v1,v9{dacb}
	vmaxabsps v0{k7},v1,v15{dacb}
	vmaxabsps v0{k7},v1,v16{dacb}
	vmaxabsps v0{k7},v1,v17{dacb}
	vmaxabsps v0{k7},v1,v31{dacb}
	vmaxabsps v8{k7},v1,v31{dacb}
	vmaxabsps v9{k7},v1,v31{dacb}
	vmaxabsps v15{k7},v1,v31{dacb}
	vmaxabsps v15{k7},v8,v31{dacb}
	vmaxabsps v15{k7},v9,v31{dacb}
	vmaxabsps v15{k7},v15,v31{dacb}
	vmaxabsps v31{k7},v31,v31{dacb}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{1to16}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{4to16}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{unorm8}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{sint16}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{float16}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{uint8}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{nt}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{nt}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{nt}{unorm8}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [rax]{nt}{unorm8}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [r8+r9+0x4]{nt}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [r8+r9+64]{nt}
	vmaxabsps v0{k7},v1,ZMMWORD PTR [r8+r9-64]{nt}
	vaddsetcpi v0,k1,v2
	vaddsetcpi v0{k7},k0,v2
	vaddsetcpi v1{k7},k2,ZMMWORD PTR [r8+r9+0x4]{nt}
	vaddsetcpi v2{k7},k3,ZMMWORD PTR [r8+r9+64]{nt}
	vcvtps2srgb8 v0,v2
	vcvtps2srgb8 v0{k1},v2
	vcvtps2srgb8 v0{k1},v2{aaaa}
	vcvtps2srgb8 v0{k1},v2{bbbb}
	vcvtps2srgb8 v0{k1},v2{cccc}
	vcvtps2srgb8 v31{k7},v31{cccc}
	vcvtps2srgb8 v0{k1},ZMMWORD PTR [rax]
	vcvtps2srgb8 v0{k1},ZMMWORD PTR [r8]
	vcvtps2srgb8 v0{k1},ZMMWORD PTR [r15]
	vcvtps2srgb8 v0{k1},ZMMWORD PTR [rax]{unorm8}
	vcvtps2srgb8 v0{k1},ZMMWORD PTR [rax]{nt}
	vcvtps2srgb8 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vcvtps2srgb8 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vcvtps2srgb8 v1{k7},ZMMWORD PTR [r8+r9+0x4]{nt}
	vcvtps2srgb8 v2{k7},ZMMWORD PTR [r8+r9+64]{nt}
	vcvtps2srgb8 v0{k7},v2
	vfixupps v0,v2,0x0
	vfixupps v0{k1},v2,0x0
	vfixupps v0{k1},v2{aaaa},0x0
	vfixupps v0{k1},v2{bbbb},0x0
	vfixupps v0{k1},v2{cccc},0x0
	vfixupps v31{k7},v31{cccc},0x0
	vfixupps v0{k1},ZMMWORD PTR [rax]{unorm8},0x0
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt},0x0
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x0
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x0
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x1
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x7
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x7f
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x100
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x4000
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0xffff
	vfixupps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x10000
	vloadd  v0{k1},[rax]
	vloadd  v0{k1},[rax]
	vloadd  v0{k1},[rax]{1to16}
	vloadd  v0{k1},[rax]{nt}
	vloadd  v0{k1},[rax]{srgb8}
	vloadd  v0{k1},[rax]{1to16}{srgb8}
	vloadd  v0{k1},[rax]{4to16}{srgb8}
	vloadd  v0{k1},[rax]{nt}{4to16}
	vloadd  v0{k1},[rax]{nt}{srgb8}
	vloadd  v0{k1},[rax]{nt}{1to16}{srgb8}
	vloadd  v0{k1},[rax]{nt}{1to16}{srgb8}
	vstored [rax]{k1},v0
	vstored [rax]{k1}{nt},v0
	vstored [rax]{k1},v0{dcba}
	vstored [rax]{k1},v0{sint8}
	vstored [rax]{k1},v0{dcba}{sint8}
	vstored [rax]{k1},v0{dcba}{sint8}
	vstored [rax]{k1}{nt},v0{dcba}
	vstored [rax]{k1}{nt},v0{sint8}
	vstored [rax]{k1}{nt},v0{a}{sint8}
	vstored [rax]{k1}{nt},v0{a}{sint8}
	vstored [rax]{k1}{nt},v0{a}{sint8}
	vstored [rax+128]{k1},v0{float16rz}
	vadcpi v0{k1},k1,v2{aaaa}
	vadcpi v0{k1},k1,v2{bbbb}
	vadcpi v0{k1},k1,v2{cccc}
	vadcpi v31{k7},k7,v31{cccc}
	vadcpi v0{k1},k1,v2
	vadcpi v0{k1},k1,ZMMWORD PTR [rax]
	vadcpi v0{k1},k1,ZMMWORD PTR [r8]
	vadcpi v0{k1},k1,ZMMWORD PTR [r15]
	vadcpi v0{k1},k1,ZMMWORD PTR [rax]{sint8}
	vadcpi v0{k1},k1,ZMMWORD PTR [rax]{nt}
	vadcpi v0{k1},k1,ZMMWORD PTR [rax]{nt}{sint8}
	vadcpi v0{k1},k1,ZMMWORD PTR [rax]{nt}{sint8}
	vaddpi v0,v1,v2
	vaddpi v0,v1,v2
	vaddpi v0{k7},v1,v2
	vaddpi v0{k7},v1,v2{cdab}
	vaddpi v0{k7},v1,v2{badc}
	vaddpi v0{k7},v1,v2{aaaa}
	vaddpi v0{k7},v1,v2{bbbb}
	vaddpi v0{k7},v1,v2{cccc}
	vaddpi v0{k7},v1,v2{dddd}
	vaddpi v0{k7},v1,v2{dacb}
	vaddpi v0{k7},v1,v7{dacb}
	vaddpi v0{k7},v1,v8{dacb}
	vaddpi v0{k7},v1,v9{dacb}
	vaddpi v0{k7},v1,v15{dacb}
	vaddpi v0{k7},v1,v16{dacb}
	vaddpi v0{k7},v1,v17{dacb}
	vaddpi v0{k7},v1,v31{dacb}
	vaddpi v8{k7},v1,v31{dacb}
	vaddpi v9{k7},v1,v31{dacb}
	vaddpi v15{k7},v1,v31{dacb}
	vaddpi v15{k7},v8,v31{dacb}
	vaddpi v15{k7},v9,v31{dacb}
	vaddpi v15{k7},v15,v31{dacb}
	vaddpi v31{k7},v31,v31{dacb}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{1to16}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{4to16}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{sint8}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{sint16}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{uint16}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{uint8}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{nt}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{nt}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{nt}{sint8}
	vaddpi v0{k7},v1,ZMMWORD PTR [rax]{nt}{sint8}
	vaddpi v0{k7},v1,ZMMWORD PTR [r8+r9+0x4]{nt}
	vaddpi v0{k7},v1,ZMMWORD PTR [r8+r9+64]{nt}
	vaddpi v0{k7},v1,ZMMWORD PTR [r8+r9-64]{nt}
	vcvtinsps2f11 v0,v2,rn,0x0
	vcvtinsps2f11 v0{k1},v2,rn,0x0
	vcvtinsps2f11 v0{k1},v2{aaaa},rn,0x0
	vcvtinsps2f11 v0{k1},v2{bbbb},rn,0x0
	vcvtinsps2f11 v0{k1},v2{cccc},rn,0x0
	vcvtinsps2f11 v31{k7},v31{cccc},rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax],rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [r8],rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [r15],rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax]{unorm8},rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax]{nt},rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rz,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,0x0
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,0x1
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,0x2
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax+0x16]{nt}{unorm8},rn,0x2
	vcvtinsps2f11 v0{k1},ZMMWORD PTR [rax+rbx*4+0x16]{nt}{unorm8},rn,0x2
	vcvtpi2ps v0,v2,.0b
	vcvtpi2ps v0{k1},v2,.0b
	vcvtpi2ps v0{k1},v2{aaaa},.0b
	vcvtpi2ps v0{k1},v2{bbbb},.0b
	vcvtpi2ps v0{k1},v2{cccc},.0b
	vcvtpi2ps v31{k7},v31{cccc},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax],.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [r8],.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [r15],.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{sint8},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.4b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.5b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.8b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.16b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.24b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.31b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.32b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax+16]{nt}{sint8},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax+0x8]{nt}{sint8},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax+0x4]{nt}{sint8},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax+16]{nt}{sint8},.0b
	vcvtpi2ps v0{k1},ZMMWORD PTR [rax+rbx*4+16]{nt}{sint8},.0b
	vcvtps2pi v0,v2,rn,.0b
	vcvtps2pi v0{k1},v2,rn,.0b
	vcvtps2pi v0{k1},v2{aaaa},rn,.0b
	vcvtps2pi v0{k1},v2{bbbb},rn,.0b
	vcvtps2pi v0{k1},v2{cccc},rn,.0b
	vcvtps2pi v31{k7},v31{cccc},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax],rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [r8],rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [r15],rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{unorm8},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.4b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.5b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.8b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.16b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.24b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.31b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.32b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rd,.32b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},ru,.32b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rz,.32b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax+16]{nt}{unorm8},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax+0x8]{nt}{unorm8},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax+0x4]{nt}{unorm8},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax+16]{nt}{unorm8},rn,.0b
	vcvtps2pi v0{k1},ZMMWORD PTR [rax+rbx*4+16]{nt}{unorm8},rn,.0b
	vinsertfieldpi v0{k1},v1,v2,0x0,0x0,0x0
	vinsertfieldpi v0{k1},v1,v2,0x0,0x0,0x1
	vinsertfieldpi v0{k1},v1,v2,0x0,0x1,0x0
	vinsertfieldpi v0{k1},v1,v2,0x1,0x0,0x0
	vinsertfieldpi v0{k1},v1,v2,0x1,0x0,0x1
	vinsertfieldpi v0{k1},v1,v2,0x1,0x1,0x1
	vinsertfieldpi v0{k1},v1,v2,0x1,0x7,0xf
	vinsertfieldpi v0{k1},v1,v2,0x8,0x2,0x3
	vinsertfieldpi v0{k1},v1,v2,0x10,0x3,0x13
	vinsertfieldpi v0{k7},v1,v2{cccc},0x10,0x3,0x13
	vinsertfieldpi v0{k7},v1,ZMMWORD PTR [rax+16]{nt}{sint8},0x10,0x3,0x13
	vinsertfieldpi v0{k7},v1,ZMMWORD PTR [rax+0x1]{nt}{sint8},0x10,0x3,0x13
	vinsertfieldpi v0{k7},v1,ZMMWORD PTR [rax+rbx*4+16]{nt}{sint8},0x10,0x3,0x13
	vinsertfieldpi v0{k7},v1,ZMMWORD PTR [rax+rbx*4+0x1]{nt}{sint8},0x10,0x3,0x13
	vrotatefieldpi v0{k1},v2,0x0,0x0,0x0
	vrotatefieldpi v0{k1},v2,0x0,0x0,0x1
	vrotatefieldpi v0{k1},v2,0x0,0x1,0x0
	vrotatefieldpi v0{k1},v2,0x1,0x0,0x0
	vrotatefieldpi v0{k1},v2,0x1,0x0,0x1
	vrotatefieldpi v0{k1},v2,0x1,0x1,0x1
	vrotatefieldpi v0{k1},v2,0x1,0x7,0xf
	vrotatefieldpi v0{k1},v2,0x8,0x2,0x3
	vrotatefieldpi v0{k1},v2,0x10,0x3,0x13
	vrotatefieldpi v0{k7},v2{cccc},0x10,0x3,0x13
	vrotatefieldpi v0{k7},ZMMWORD PTR [rax+16]{nt}{sint8},0x10,0x3,0x13
	vrotatefieldpi v0{k7},ZMMWORD PTR [rax+0x1]{nt}{sint8},0x10,0x3,0x13
	vrotatefieldpi v0{k7},ZMMWORD PTR [rax+rbx*4+16]{nt}{sint8},0x10,0x3,0x13
	vrotatefieldpi v0{k7},ZMMWORD PTR [rax+rbx*4+0x1]{nt}{sint8},0x10,0x3,0x13
	vcmppi k1{k1},v0,v1,eq
	vcmppi k1{k1},v0,v1{aaaa},eq
	vcmppi k1{k1},v0,v1{bbbb},eq
	vcmppi k7{k1},v0,v1,eq
	vcmppi k1{k7},v0,v1,eq
	vcmppi k1{k1},v31,v1,eq
	vcmppi k1{k1},v0,v7,eq
	vcmppi k1{k1},v0,v1,eq
	vcmppi k1{k1},v0,v1,eq
	vcmppi k1{k1},v0,v1,eq
	vcmppi k1{k1},v0,v1,lt
	vcmppi k1{k1},v0,v1,le
	vcmppi k1{k1},v0,v1,neq
	vcmppi k1{k1},v0,v1,nlt
	vcmppi k1{k1},v0,v1,nle
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],lt
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],le
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],neq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],nlt
	vcmppi k1{k1},v0,ZMMWORD PTR [rax],nle
	vcmppi k1{k1},v0,ZMMWORD PTR [rax]{nt},eq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax]{sint8},eq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax]{nt}{sint8},eq
	vcmppi k1{k1},v0,ZMMWORD PTR [rax]{nt}{sint8},eq
	vcmppu k1{k1},v0,v1,eq
	vcmppu k1{k1},v0,v1{aaaa},eq
	vcmppu k1{k1},v0,v1{bbbb},eq
	vcmppu k7{k1},v0,v1,eq
	vcmppu k1{k7},v0,v1,eq
	vcmppu k1{k1},v31,v1,eq
	vcmppu k1{k1},v0,v7,eq
	vcmppu k1{k1},v0,v1,eq
	vcmppu k1{k1},v0,v1,eq
	vcmppu k1{k1},v0,v1,eq
	vcmppu k1{k1},v0,v1,lt
	vcmppu k1{k1},v0,v1,le
	vcmppu k1{k1},v0,v1,neq
	vcmppu k1{k1},v0,v1,nlt
	vcmppu k1{k1},v0,v1,nle
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],lt
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],le
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],neq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],nlt
	vcmppu k1{k1},v0,ZMMWORD PTR [rax],nle
	vcmppu k1{k1},v0,ZMMWORD PTR [rax]{nt},eq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax]{sint8},eq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax]{nt}{sint8},eq
	vcmppu k1{k1},v0,ZMMWORD PTR [rax]{nt}{sint8},eq
	vcmpps k1{k1},v0,v1,eq
	vcmpps k1{k1},v0,v1{aaaa},eq
	vcmpps k1{k1},v0,v1{bbbb},eq
	vcmpps k7{k1},v0,v1,eq
	vcmpps k1{k7},v0,v1,eq
	vcmpps k1{k1},v31,v1,eq
	vcmpps k1{k1},v0,v7,eq
	vcmpps k1{k1},v0,v1,eq
	vcmpps k1{k1},v0,v1,eq
	vcmpps k1{k1},v0,v1,eq
	vcmpps k1{k1},v0,v1,lt
	vcmpps k1{k1},v0,v1,le
	vcmpps k1{k1},v0,v1,neq
	vcmpps k1{k1},v0,v1,nlt
	vcmpps k1{k1},v0,v1,nle
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],lt
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],le
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],neq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],nlt
	vcmpps k1{k1},v0,ZMMWORD PTR [rax],nle
	vcmpps k1{k1},v0,ZMMWORD PTR [rax]{nt},eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax]{unorm8},eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax]{nt}{unorm8},eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax]{nt}{unorm8},eq
	vcmpps k1{k1},v0,ZMMWORD PTR [rax]{nt}{unorm8},unord
	vcmpps k1{k1},v0,ZMMWORD PTR [rax]{nt}{unorm8},ord
	vcvtinsps2u10 v0,v2,0x0
	vcvtinsps2u10 v0{k1},v2,0x0
	vcvtinsps2u10 v0{k1},v2{aaaa},0x0
	vcvtinsps2u10 v0{k1},v2{bbbb},0x0
	vcvtinsps2u10 v0{k1},v2{cccc},0x0
	vcvtinsps2u10 v31{k7},v31{cccc},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax],0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [r8],0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [r15],0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{unorm8},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x0
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x1
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},0x2
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax+16]{nt}{unorm8},0x2
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax+0x16]{nt}{unorm8},0x2
	vcvtinsps2u10 v0{k1},ZMMWORD PTR [rax+rbx*4+0x16]{nt}{unorm8},0x2
	vcvtps2pu v0,v2,rn,.0b
	vcvtps2pu v0{k1},v2,rn,.0b
	vcvtps2pu v0{k1},v2{aaaa},rn,.0b
	vcvtps2pu v0{k1},v2{bbbb},rn,.0b
	vcvtps2pu v0{k1},v2{cccc},rn,.0b
	vcvtps2pu v31{k7},v31{cccc},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax],rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [r8],rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [r15],rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{unorm8},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.4b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.5b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.8b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.16b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.24b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.31b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.32b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rd,.32b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},ru,.32b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rz,.32b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax+16]{nt}{unorm8},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax+0x8]{nt}{unorm8},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax+0x4]{nt}{unorm8},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax+16]{nt}{unorm8},rn,.0b
	vcvtps2pu v0{k1},ZMMWORD PTR [rax+rbx*4+16]{nt}{unorm8},rn,.0b
	vcvtpu2ps v0,v2,.0b
	vcvtpu2ps v0{k1},v2,.0b
	vcvtpu2ps v0{k1},v2{aaaa},.0b
	vcvtpu2ps v0{k1},v2{bbbb},.0b
	vcvtpu2ps v0{k1},v2{cccc},.0b
	vcvtpu2ps v31{k7},v31{cccc},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax],.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [r8],.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [r15],.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{sint8},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.4b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.5b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.8b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.16b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.24b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.31b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax]{nt}{sint8},.32b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax+16]{nt}{sint8},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax+0x8]{nt}{sint8},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax+0x4]{nt}{sint8},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax+16]{nt}{sint8},.0b
	vcvtpu2ps v0{k1},ZMMWORD PTR [rax+rbx*4+16]{nt}{sint8},.0b
	vloadunpackld v0,[rax]
	vloadunpackld v7,[rax]
	vloadunpackld v8,[rax]
	vloadunpackld v15,[rax]
	vloadunpackld v31,[rax]
	vloadunpackld v0,[r8]
	vloadunpackld v0,[r15]
	vloadunpackld v31,[r15]
	vloadunpackld v0,[rax]{nt}
	vloadunpackld v0{k1},[rax]
	vloadunpackld v0{k1},[rax]{nt}
	vloadunpackhd v0,[rax]
	vloadunpackhd v0,[rax]{nt}
	vloadunpackhd v0{k1},[rax]
	vloadunpackhd v0{k1},[rax]{nt}
	vpackstoreld [rax],v0
	vpackstoreld [rax]{nt},v0
	vpackstoreld [rax]{k1},v0
	vpackstoreld [rax]{k1}{nt},v0
	vpackstoreld [r8],v0
	vpackstoreld [r15]{nt},v0
	vpackstoreld [rax]{k7},v0
	vpackstoreld [rax]{k1}{nt},v15
	vpackstoreld [rax+128]{k1}{nt},v0
	vpackstorehd [rax],v0
	vpackstorehd [rax]{nt},v0
	vpackstorehd [rax]{k1},v0
	vpackstorehd [rax]{k1}{nt},v0
	vpackstorehd [r8],v0
	vpackstorehd [r15]{nt},v0
	vpackstorehd [rax]{k7},v0
	vpackstorehd [rax]{k1}{nt},v15
	vpackstorehd [rax+128]{k1}{nt},v0
	vroundps v0,v2,rn,.0b
	vroundps v0{k1},v2,rn,.0b
	vroundps v0{k1},v2{aaaa},rn,.0b
	vroundps v0{k1},v2{bbbb},rn,.0b
	vroundps v0{k1},v2{cccc},rn,.0b
	vroundps v31{k7},v31{cccc},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax],rn,.0b
	vroundps v0{k1},ZMMWORD PTR [r8],rn,.0b
	vroundps v0{k1},ZMMWORD PTR [r15],rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax]{unorm8},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.4b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.5b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.8b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.16b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.24b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.31b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rn,.32b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rd,.32b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},ru,.32b
	vroundps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8},rz,.32b
	vroundps v0{k1},ZMMWORD PTR [rax+16]{nt}{unorm8},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax+0x8]{nt}{unorm8},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax+0x4]{nt}{unorm8},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax+16]{nt}{unorm8},rn,.0b
	vroundps v0{k1},ZMMWORD PTR [rax+rbx*4+16]{nt}{unorm8},rn,.0b
	vtestpi k1,v0,v1
	vtestpi k1,v0,v1{aaaa}
	vtestpi k1,v0,v1{bbbb}
	vtestpi k7,v0,v1
	vtestpi k1{k7},v0,v1
	vtestpi k1,v31,v1
	vtestpi k1,v0,v31
	vtestpi k1,v0,ZMMWORD PTR [rax]
	vtestpi k7,v0,ZMMWORD PTR [rax]
	vtestpi k1{k7},v0,ZMMWORD PTR [rax]
	vtestpi k1,v31,ZMMWORD PTR [rax]
	vtestpi k1,v0,ZMMWORD PTR [r15]
	vtestpi k1,v0,ZMMWORD PTR [rax]{nt}
	vtestpi k1,v0,ZMMWORD PTR [rax]{sint8}
	vtestpi k1,v0,ZMMWORD PTR [rax]{nt}{sint8}
	vtestpi k1,v0,ZMMWORD PTR [rax]{nt}{sint8}
	vtestpi k1{k7},v1,ZMMWORD PTR [r8+r9+0x4]{nt}
	vtestpi k1{k7},v1,ZMMWORD PTR [r8+r9+64]{nt}
	vtestpi k1{k7},v1,ZMMWORD PTR [r8+r9-64]{nt}
	vshuf128x32 v0,v2,{0000},{aaaa}
	vshuf128x32 v31{k7},v31,{0000},{aaaa}
	vshuf128x32 v0,[rax],{0000},{aaaa}
	vshuf128x32 v0,[r8],{0000},{aaaa}
	vshuf128x32 v0,[rax]{nt},{0000},{aaaa}
	vshuf128x32 v0,[r15],{0000},{aaaa}
	vshuf128x32 v0,v2,{0000},{aaad}
	vshuf128x32 v0,v2,{0000},{aadd}
	vshuf128x32 v0,v2,{0000},{addd}
	vshuf128x32 v0,v2,{0000},{dddd}
	vshuf128x32 v0,v2,{0002},{dddd}
	vshuf128x32 v0,v2,{0202},{dddd}
	vshuf128x32 v0,v2,{3333},{dddd}
	vshuf128x32 v0,v2,{3333},{abcd}
	vexp2lutps v0,v2
	vexp2lutps v0{k1},v2
	vexp2lutps v0{k1},v2{aaaa}
	vexp2lutps v0{k1},v2{bbbb}
	vexp2lutps v0{k1},v2{cccc}
	vexp2lutps v31{k7},v31{cccc}
	vexp2lutps v0{k1},ZMMWORD PTR [rax]
	vexp2lutps v0{k1},ZMMWORD PTR [r8]
	vexp2lutps v0{k1},ZMMWORD PTR [r15]
	vexp2lutps v0{k1},ZMMWORD PTR [rax]{sint8}
	vexp2lutps v0{k1},ZMMWORD PTR [rax]{nt}
	vexp2lutps v0{k1},ZMMWORD PTR [rax]{nt}{sint8}
	vexp2lutps v0{k1},ZMMWORD PTR [rax]{nt}{sint8}
	vexp2lutps v1{k7},ZMMWORD PTR [r8+r9+0x4]{nt}
	vexp2lutps v2{k7},ZMMWORD PTR [r8+r9+64]{nt}
	vexp2lutps v0{k7},v2
	vgetexpps v0,v2
	vgetexpps v0{k1},v2
	vgetexpps v0{k1},v2{aaaa}
	vgetexpps v0{k1},v2{bbbb}
	vgetexpps v0{k1},v2{cccc}
	vgetexpps v31{k7},v31{cccc}
	vgetexpps v0{k1},ZMMWORD PTR [rax]
	vgetexpps v0{k1},ZMMWORD PTR [r8]
	vgetexpps v0{k1},ZMMWORD PTR [r15]
	vgetexpps v0{k1},ZMMWORD PTR [rax]{unorm8}
	vgetexpps v0{k1},ZMMWORD PTR [rax]{nt}
	vgetexpps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vgetexpps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vgetexpps v1{k7},ZMMWORD PTR [r8+r9+0x4]{nt}
	vgetexpps v2{k7},ZMMWORD PTR [r8+r9+64]{nt}
	vgetexpps v0{k7},v2
	vrcpresps v0,v2
	vrcpresps v0{k1},v2
	vrcpresps v0{k1},v2{aaaa}
	vrcpresps v0{k1},v2{bbbb}
	vrcpresps v0{k1},v2{cccc}
	vrcpresps v31{k7},v31{cccc}
	vrcpresps v0{k1},ZMMWORD PTR [rax]
	vrcpresps v0{k1},ZMMWORD PTR [r8]
	vrcpresps v0{k1},ZMMWORD PTR [r15]
	vrcpresps v0{k1},ZMMWORD PTR [rax]{unorm8}
	vrcpresps v0{k1},ZMMWORD PTR [rax]{nt}
	vrcpresps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vrcpresps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vrcpresps v1{k7},ZMMWORD PTR [r8+r9+0x4]{nt}
	vrcpresps v2{k7},ZMMWORD PTR [r8+r9+64]{nt}
	vrcpresps v0{k7},v2
	vrsqrtlutps v0,v2
	vrsqrtlutps v0{k1},v2
	vrsqrtlutps v0{k1},v2{aaaa}
	vrsqrtlutps v0{k1},v2{bbbb}
	vrsqrtlutps v0{k1},v2{cccc}
	vrsqrtlutps v31{k7},v31{cccc}
	vrsqrtlutps v0{k1},ZMMWORD PTR [rax]
	vrsqrtlutps v0{k1},ZMMWORD PTR [r8]
	vrsqrtlutps v0{k1},ZMMWORD PTR [r15]
	vrsqrtlutps v0{k1},ZMMWORD PTR [rax]{unorm8}
	vrsqrtlutps v0{k1},ZMMWORD PTR [rax]{nt}
	vrsqrtlutps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vrsqrtlutps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vrsqrtlutps v1{k7},ZMMWORD PTR [r8+r9+0x4]{nt}
	vrsqrtlutps v2{k7},ZMMWORD PTR [r8+r9+64]{nt}
	vrsqrtlutps v0{k7},v2
	vsbbpi v0,k1,v2
	vsbbpi v0{k7},k0,v2
	vsbbpi v1{k7},k2,ZMMWORD PTR [r8+r9+0x4]{nt}
	vsbbpi v2{k7},k3,ZMMWORD PTR [r8+r9+64]{nt}
	vsbbrpi v0,k1,v2
	vsbbrpi v0{k7},k0,v2
	vsbbrpi v1{k7},k2,ZMMWORD PTR [r8+r9+0x4]{nt}
	vsbbrpi v2{k7},k3,ZMMWORD PTR [r8+r9+64]{nt}
	vsubrsetbpi v0,k1,v2
	vsubrsetbpi v0{k7},k0,v2
	vsubrsetbpi v1{k7},k2,ZMMWORD PTR [r8+r9+0x4]{nt}
	vsubrsetbpi v2{k7},k3,ZMMWORD PTR [r8+r9+64]{nt}
	vsubsetbpi v0,k1,v2
	vsubsetbpi v0{k7},k0,v2
	vsubsetbpi v1{k7},k2,ZMMWORD PTR [r8+r9+0x4]{nt}
	vsubsetbpi v2{k7},k3,ZMMWORD PTR [r8+r9+64]{nt}
	vlog2lutps v0,v2
	vlog2lutps v0{k1},v2
	vlog2lutps v0{k1},v2{aaaa}
	vlog2lutps v0{k1},v2{bbbb}
	vlog2lutps v0{k1},v2{cccc}
	vlog2lutps v31{k7},v31{cccc}
	vlog2lutps v0{k1},ZMMWORD PTR [rax]
	vlog2lutps v0{k1},ZMMWORD PTR [r8]
	vlog2lutps v0{k1},ZMMWORD PTR [r15]
	vlog2lutps v0{k1},ZMMWORD PTR [rax]{unorm8}
	vlog2lutps v0{k1},ZMMWORD PTR [rax]{nt}
	vlog2lutps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vlog2lutps v0{k1},ZMMWORD PTR [rax]{nt}{unorm8}
	vlog2lutps v1{k7},ZMMWORD PTR [r8+r9+0x4]{nt}
	vlog2lutps v2{k7},ZMMWORD PTR [r8+r9+64]{nt}
	vlog2lutps v0{k7},v2
	vcmppi k1,v2,ZMMWORD PTR [rax],le
	vcmppi k1{k2},v2,ZMMWORD PTR [rax],le
	vcmpps k1,v3,ZMMWORD PTR [rax],le
	vcmppu k1,v9,ZMMWORD PTR [rax],le
	vcmppi k1,v2,v1,le
	vcmppi k1{k2},v2,v2,le
	vcmpps k1,v3,v3,le
	vcmppu k1,v9,v4,le
	vloadd  v0,[rax+0x40]
	vloadd  v0,[rax-0x40]
                    spflt              ax
                    spflt              eax
                    spflt              rax
                    spflt              r12w
                    spflt              r12d
                    spflt              r12
	vgatherd v0{k1},[rax+v3*4]
	vgatherd v0{k1},[rax+v22*8]{nt}
	vgatherd v0{k1},[rax+v30*1+0x7f]{nt}{srgb8}
	vgatherd v0{k1},[rax+v10*2+0x80]{nt}
	vgatherd v0{k1},[rax+v9*4+0x80]{nt}{srgb8}
	vgatherpfd [rax+v3*4]{k7}
	vgatherpfd [rax+v3*8]{nt}{k7}
	vgatherpfd [rax+v5*1+0x7f]{nt}{srgb8}{k5}
	vgatherpfd [rax+v6*2+0x80]{nt}{k4}
	vgatherpfd [rax+v7*4+0x80]{nt}{srgb8}{k3}
	vscatterd [v1*1+0x80]{k1},v11
	vscatterd [rax+v2*1]{k1}{nt},v11
	vscatterd [rax+v2*2]{k1}{nt},v10
	vscatterd [rax+v3*4]{k1},v9
	vscatterd [rax+v3*8]{k1},v8{sint8}
	vscatterd [rax+v4*1]{k1},v7{sint8}
	vscatterd [rax+v5*2]{k1},v6{sint8}
	vscatterd [rax+v6*4]{k1}{nt},v5
	vscatterd [rax+v7*8]{k1}{nt},v4{sint8}
	vscatterd [rax+v8*1]{k1}{nt},v3{sint8}
	vscatterd [rax+v9*2]{k1}{nt},v2{sint8}
	vscatterd [rax+v10*4]{k1}{nt},v1{sint8}
	vscatterd [v11*1+0x80]{k1},v0{float16rz}
	vscatterpfd [rax+v6*4]{nt}{k1}
	vscatterpfd [rax+v0*4]{nt}{k1}
	vscatterpfd [rax+v1*1]{k1}
	vscatterpfd [rax+v2*2]{nt}{k1}
	vscatterpfd [rax+v3*4]{k1}
	vscatterpfd [rax+v3*4+0x80]{k1}
	vscatterpfd [v1*1+0x80]{k1}
	vscatterpfd [rax+v3*8]{sint8}{k1}
	vscatterpfd [rax+v4*1]{sint8}{k1}
	vscatterpfd [rax+v5*2]{sint8}{k1}
	vscatterpfd [rax+v7*8]{sint8}{k1}
	vscatterpfd [rax+v8*1]{nt}{sint8}{k1}
	vscatterpfd [rax+v9*2]{nt}{sint8}{k1}
	vscatterpfd [rax+v10*4]{nt}{sint8}{k1}
	vscatterpfd [rax+v11*8+0x80]{nt}{float16rz}{k1}

vloadd v1{k1},[rcx+0x40]{sint8}
vloadd v1{k1},[rcx+0x100]{sint8}
vloadd v1{k1},[rcx+0x40]{unorm2D}
vloadd v1{k1},[rcx+0x100]{unorm2D}
vloadd v1{k1},[rcx+0x40]{float11A}
vloadd v1{k1},[rcx+0x100]{float11A}
vloadd v1{k1},[rcx+0x40]{4to16}{sint8}
vloadd v1{k1},[rcx+0x100]{4to16}{sint8}
vloadd v1{k1},[rcx+0x40]{4to16}{unorm2D}
vloadd v1{k1},[rcx+0x100]{4to16}{unorm2D}
vloadd v1{k1},[rcx+0x40]{4to16}{float11A}
vloadd v1{k1},[rcx+0x100]{4to16}{float11A}
vloadunpackhd v1{k1},[rcx+0x40]{sint8}
vloadunpackhd v1{k1},[rcx+0x100]{sint8}
vloadunpackhd v1{k1},[rcx+0x40]{sint16}
vloadunpackhd v1{k1},[rcx+0x80]{sint16}
vloadunpackhd v1{k1},[rcx+0x40]{unorm2D}
vloadunpackhd v1{k1},[rcx+0x100]{unorm2D}
vloadunpackhd v1{k1},[rcx+0x40]{float11A}
vloadunpackhd v1{k1},[rcx+0x100]{float11A}

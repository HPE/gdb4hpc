	.text
.L1:
	mov    $0x40e0c400,%eax
	jnz	foo
	jknzd	foo,%k7
	jknzd	.L1,%k6
	jknzd	.L1,%k0
	jknzd	foo,%k1
	jkzd	foo,%k2
	jkzd	.L1,%k3
	jkzd	.L1,%k4
	jkzd	foo,%k5
	jknzd	.+0x909090,%k6
	jkzd	.+0x909090,%k6

	.intel_syntax noprefix
	mov    eax,0x40e0c400
	jnz	foo
	jknzd	k7,foo
	jknzd	k6,.L1
	jknzd	k0,.L1
	jknzd	k1,foo
	jkzd	k2,foo
	jkzd	k3,.L1
	jkzd	k4,.L1
	jkzd	k5,foo
	jknzd	k6,.+0x909090
	jkzd	k6,.+0x909090

	nop

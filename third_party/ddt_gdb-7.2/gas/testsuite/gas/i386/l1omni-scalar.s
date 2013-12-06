	.text
_start:
	bitinterleave11 %ax,%ax
	bitinterleave11 %bx,%ax
	bitinterleave11 %bx,%r8w
	bitinterleave11 %eax,%eax
	bitinterleave11 %ebx,%eax
	bitinterleave11 %ebx,%r8d
	bitinterleave11 %rbx,%r8
	bitinterleave11 %eax,%eax
	bitinterleave11 %rax,%rax
	bitinterleave11 %rbx,%rax

	bitinterleave11w %ax,%ax
	bitinterleave11l %eax,%eax
	bitinterleave11q %rax,%rax

	vprefetch1 $0,(%rdx)
	vprefetch1 $1,(%rdx)
	vprefetch1 $7,(%rdx)
	vprefetch1 $1,64(%rdx)
	vprefetch1 $1,4(%rdx)
	vprefetch1 $1,-64(%rdx)

	clevict1 (%rdx)
	clevict1 (%rdx)
	clevict1 (%rdx)
	clevict1 (%r8)
	clevict1 64(%rdx)
	clevict1 4(%rdx)
	clevict1 -64(%rdx)

	clevict2 (%rdx)
	clevict2 (%rdx)
	clevict2 (%rdx)
	clevict2 (%r8)
	clevict2 64(%rdx)
	clevict2 4(%rdx)
	clevict2 -64(%rdx)

	delay %r9d
	delay %r9w
	delay %ebx
	delay %bx

	insertfield $0, $0, $0, %ax,%ax
	insertfield $0, $0, $65535, %ax,%ax
	insertfield $0, $255, $0, %ax,%ax
	insertfield $0, $255, $65535, %ax,%ax
	insertfield $255, $0, $0, %ax,%ax
	insertfield $255, $255, $65535, %ax,%ax

	insertfield $0, $0, $0, %eax,%eax
	insertfield $0, $0, $0, %rax,%rax

	insertfieldw  $0, $0, $0, %ax,%ax
	insertfieldl  $0, $0, $0, %eax,%eax
	insertfieldq  $0, $0, $0, %rax,%rax

	vprefetch2 $0,(%edx)
	vprefetch2 $1,(%edx)
	vprefetch2 $3,(%edx)
	vprefetch2 $0,(%rdx)
	vprefetch2 $1,(%rdx)
	vprefetch2 $3,(%rdx)
	vprefetch2 $1,64(%rdx)
	vprefetch2 $1,4(%rdx)
	vprefetch2 $1,-64(%rdx)

	rotatefield $0, $0, $0, %ax,%ax
	rotatefield $0, $0, $65535, %ax,%ax
	rotatefield $0, $255, $0, %ax,%ax
	rotatefield $0, $255, $65535, %ax,%ax
	rotatefield $255, $0, $0, %ax,%ax
	rotatefield $255, $255, $65535, %ax,%ax

	rotatefield $0, $0, $0, %eax,%eax
	rotatefield $0, $0, $0, %rax,%rax

	rotatefieldw  $0, $0, $0, %ax,%ax
	rotatefieldl  $0, $0, $0, %eax,%eax
	rotatefieldq  $0, $0, $0, %rax,%rax

	clevict1 (%r10)
	delay %r8w
	delay %r8d
	delay %r8
	delay %ax
	delay %eax
	delay %rax

	ldvxcsr (%rax)
	stvxcsr (%rax)
	ldvxcsrl (%rax)
	stvxcsrl (%rax)

	spflt %r8w
	spflt %r8d
	spflt %r8
	spflt %ax
	spflt %eax
	spflt %rax

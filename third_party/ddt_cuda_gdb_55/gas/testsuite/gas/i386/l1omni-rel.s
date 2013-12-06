	.text
_start:
	vfixupps $0x1FFFFF,foo(%rax),%v13
	vfixupps $0x1FFFFF,foo(%rax,%r12,2),%v13
	vfixupps $0, foo(%rip) {NT}{unorm8}, %v0 {%k1}
	vinsertfieldpi $19, $3, $16, foo(%rax){sint8}{NT}, %v1, %v0 {%k7}
	vinsertfieldpi $19, $3, $16, foo(%rax,%rbx,4){sint8}{NT}, %v1, %v0 {%k7}
	vinsertfieldpi $19, $3, $16, foo(%rip){sint8}{NT}, %v1, %v0 {%k7}
	vrotatefieldpi $19, $3, $16, foo(%rax){sint8}{NT}, %v0 {%k7}
	vrotatefieldpi $19, $3, $16, foo(%rax,%rbx,4){sint8}{NT}, %v0 {%k7}
	vrotatefieldpi $19, $3, $16, foo(%rip){sint8}{NT}, %v0 {%k7}
	vshuf128x32 {aaaa}, {0000}, foo(%r8), %v0
	vshuf128x32 {aaaa}, {0000}, foo(%r8,%r14,8), %v0
	vshuf128x32 {aaaa}, {0000}, foo(%rip), %v0


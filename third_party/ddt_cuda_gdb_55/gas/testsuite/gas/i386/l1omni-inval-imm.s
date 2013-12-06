.text
# All the following should be illegal
cmppd {unord},(%eax),%xmm2	# Comparison immediate instead of immediate
cmppd {ru},(%eax),%xmm2		# Rounding immediate instead of immediate
cmppd {.32b},(%eax),%xmm2	# expadj immediate instead of immediate
mov {ru},%rax			# Comparison immediate instead of register
mov {.5b},%rax			# expadj immediate instead of register

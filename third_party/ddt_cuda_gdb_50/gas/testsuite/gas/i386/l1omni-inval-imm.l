.*: Assembler messages:
.*:3: Error: .*
.*:4: Error: .*
.*:5: Error: .*
.*:6: Error: .*
.*:7: Error: .*
GAS LISTING .*


   1 [ 	]*.text
   2 [ 	]*# All the following should be illegal
   3 [ 	]*cmppd \{unord\},\(%eax\),%xmm2[	]+#.+
   4 [ 	]*cmppd \{ru\},\(%eax\),%xmm2[	]+#.+
   5 [ 	]*cmppd \{.32b\},\(%eax\),%xmm2[	]+#.+
   6 [ 	]*mov \{ru\},%rax[	]+#.+
   7 [ 	]*mov \{.5b\},%rax[	]+#.+

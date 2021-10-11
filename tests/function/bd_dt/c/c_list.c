// designed to test edge cases of the list command.

#include <stdio.h>

int main() {
	for (int i=0; i<3; i++) {
		for (int j=0; j<i; j++) {
			int foo	=	j;
			foo		+=	i;
			printf("\tfoo is: \"\t%d\n", foo);	// print foo
		}
	}
}

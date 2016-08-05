#include <stdlib.h>
#include <stdio.h>
#include <sys/resource.h>
#include <errno.h>

int main() {
    struct rlimit limit;
    limit.rlim_cur = RLIM_INFINITY;
    limit.rlim_max = RLIM_INFINITY;

    // Set the core file size limit
    if (setrlimit(RLIMIT_CORE, &limit) != 0) {
        printf("setrlimit() failed with errno=%d\n", errno);
    }

    abort();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

enum {
	BLOCK_SIZE = 4096UL,
};

#define ROUNDUP(X, Y) ((((X) + 1) * (Y)) / (Y))

int main(int argc, char **argv)
{
	char *s;
	size_t size;
	unsigned long r, l;

	r = strtoul((const char *)argv[1], NULL, 10);
	if (r == ULONG_MAX && (errno == ERANGE || errno == EINVAL)) {
		perror("strtoul");
		return 1;
	}

	printf("%lu\n", r);

	size = ROUNDUP(r, BLOCK_SIZE);

	printf("%lu\n", size);

	if (posix_memalign((void **)&s, BLOCK_SIZE, size)) {
		perror("posix_memalign");
		return 1;
	}

	printf("[%016lx-%016lx]\n",
	       (unsigned long)s,
	       (unsigned long)(s + size));

	for (l = 0; l < size; l += BLOCK_SIZE) {
		*(unsigned long *)(s + l) = l;
	}

	puts("done");

	pause();
	
	return 0;
}

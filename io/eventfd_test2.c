#include <sys/eventfd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>             /* Definition of uint64_t */

#define handle_error(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

int
main(int argc, char *argv[])
{
	int efd, j;
	ssize_t s;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <num>...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	efd = eventfd(0, 0);
	if (efd == -1)
		handle_error("eventfd");

	switch (fork()) {
	case 0:
		for (j = 1; j < argc; j++) {
		   printf("Child writing %s to efd\n", argv[j]);
		   s = write(efd, argv[j], strlen(argv[j]));
		   if (s != sizeof(uint64_t))
			   handle_error("write");
		}
		printf("Child completed write loop\n");

		exit(EXIT_SUCCESS);

	default:
		sleep(2);

		printf("Parent about to read\n");
		char buf[16];
		s = read(efd, (void*)buf, sizeof(buf));
		if (s != sizeof(uint64_t))
			handle_error("read");
		printf("Parent read %s (0x%llx) from efd\n",
			buf, buf);
		exit(EXIT_SUCCESS);

	case -1:
		handle_error("fork");
	}
}
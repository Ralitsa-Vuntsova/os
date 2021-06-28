#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	}

	int pf[2];

	if (pipe(pf) == -1) {
		errx(1, "Could not create pipe");
	}

	pid_t child_pid = fork();
	if (child_pid == -1) {
		err(1, "Could not fork.");
	}

	if (child_pid == 0) {
		close(pf[1]);

		// wc works with stdin and stdout, but we want to work with the pipe so we replace the stdin with the read end of the pipe
		close(0);
		dup(pf[0]);

//		sleep(5);
		if (execlp("wc", "wc", "-c", NULL) == -1) {
			err(1, "Could not exec.");
		}
	}

	close(pf[0]);
	write(pf[1], argv[1], strlen(argv[1]));
	close(pf[1]);
	wait(NULL);

	exit(0);
}

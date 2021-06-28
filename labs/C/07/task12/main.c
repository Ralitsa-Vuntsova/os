/*
	./main cmd1 ... cmdN 
	1) execs each command in seperate child processes
	2) saves the number of successful commands and the number of error commands
*/

#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int successful = 0;
	int failed = 0;

	for (int i = 1; i < argc; i++) {
		int pid = fork();
		if (pid < 0) {
			err(1, "Error in fork");
		}
		if (pid == 0) {
			if (execlp(argv[i], argv[i], (char*) NULL) == -1) {
				err(2, "Error executing %s", argv[i]);
			}
		}

		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			successful++;
		} else {
			failed++;
		}
	}	

	printf("successful: %d\n", successful);
	printf("failed: %d\n", failed);

	exit(0);
}

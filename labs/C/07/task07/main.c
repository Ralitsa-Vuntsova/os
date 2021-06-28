#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Usage: %s command_name", argv[0]);
	}

	pid_t pid = fork();
	if(pid == -1){
		err(2, "Could not fork");
	}
	if (pid == 0) {
		if (execlp(argv[1], argv[1], (char*) NULL) == -1) {
			err(3, "Error executing command");
		}
	}
	
	// parent
	int status;
	int wait_code = wait(&status);
	if(wait_code == -1){
		err(4, "Error while waiting");
	}
	if(!WIFEXITED(status)){
		err(5, "Error while waiting");
	}
	if (WEXITSTATUS(status) != 0) {
		err(6, "Error while waiting");
	}

	printf("%s\n", argv[1]);

	exit(0);
}

#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "Usage: %s <file.txt>", argv[0]);
	}

	int pf[2];

	if(pipe(pf) == -1){
		err(2, "Could not create pipe");
	}

	pid_t child_pid = fork();

	if(child_pid == -1){
		err(3, "Could not fork");
	}
	if(child_pid == 0){
		close(pf[0]);
		if(dup2(pf[1], 1) == -1){
			err(4, "Could not duplicate file descriptor");
		}
		if(execlp("cat", "cat",argv[1], NULL) == -1){
			err(5, "Could not exec command");
		}
	}

	close(pf[1]);
	if(dup2(pf[0], 0) == -1){
		err(6, "Could not duplicate file descriptor");
	}
	if(execlp("sort", "sort", NULL) == -1){
		err(7, "Could not exec command");
	}
	
	wait(NULL);
	exit(0);
}

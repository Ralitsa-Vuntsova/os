// priema kato parametur - ime na fail
// ./main.c a.txt

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "Invalid number of arguments. Usage: %s file.txt", argv[0]);
	}

	char* myfifo = "/tmp/myfifo";
	mkfifo(myfifo, 0666);

	int fd = open(myfifo, O_WRONLY);
	if(fd == -1){
		err(2, "Could not open named pipe");
	}

	pid_t child_pid = fork();
	if(child_pid == -1){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(3, "Could not fork");
	}
	if(child_pid == 0){
		dup2(fd, 1); // proverka
		if(execlp("cat", "cat", argv[1], 0) == -1){
			int olderr = errno;
			close(fd);
			errno = olderr;
			err(4, "Could not exec");
		}
	}
	wait(NULL);

	close(fd);
	exit(0);
}

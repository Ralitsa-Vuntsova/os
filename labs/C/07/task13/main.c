/*
./main cmd1 cmd2 file(O_CREAT)
exec commands in series
if exec(cmd1) is successful -> write name cmd1 to file
else error message
*/

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "Usage: %s cmd1 cmd2 file", argv[0]);
	}

	int pid = fork();
	if (pid < 0) {
		err(2, "Error in fork");
	}
	if (pid == 0) {
		if (execlp(argv[1], argv[1], (char*) NULL) == -1) {
			err(3, "Error executing %s", argv[1]);
		}
	} 

	int status;
	waitpid(pid, &status, 0);
	int fd = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		err(4, "Error opening %s", argv[3]);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		ssize_t len = strlen(argv[1]);
		if (write(fd, argv[1], len) != len) {
			int olderrno = errno;
			close(fd);
			errno = olderrno;
			err(5, "Error writing to %s", argv[3]);
		}
	}
	else {
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(6, "Error while waiting for process to terminate successfully");	
	}

	int pid2 = fork();
    	if (pid2 < 0) {
        	err(2, "Error in fork");
    	}
    	if (pid2 == 0) {
        	if (execlp(argv[2], argv[2], (char*) NULL) == -1) {
            		err(3, "Error executing %s", argv[2]);
        	}
    	}

	wait(NULL);

	exit(0);
}

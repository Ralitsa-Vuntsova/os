#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
	if(argc!=2)
		errx(1, "Invalid number of arguments. Usage: %s <filename>", argv[0]);

	const char *file = argv[1];
	
	int fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
	if(fd == -1)
		err(2, "could not open file %s", file);

	pid_t child_pid = fork();
	if(child_pid == -1)
		err(3, "could not fork process");

	if(child_pid == 0){
		const char *text = "foobar";
		const ssize_t len = strlen(text);		

		if(write(fd, text, len) != len){
			int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(4, "error while writing to file %s", file);
		}

		close(fd);
		exit(0);
	}

	int status;
	int wait_code = wait(&status);

	if(wait_code == -1){
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(5, "could not wait child process");
	}

	if(!WIFEXITED(status)){
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(6, "child did not terminate normally");
	}

	if(WEXITSTATUS(status) != 0){
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(7, "child exit code not 0");
	}
	
	fd = open(file, O_RDONLY);
	if(fd == -1){
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(8,"errow while opening file %s", file);
	}

	char c;
	int i;
	ssize_t read_size;
	while((read_size = read(fd, &c, 1)) > 0){
		i = i + 1;
		write(1, &c, 1);
		if(i % 2 == 0){
			write(1, " ", 1);
		}
	}

	if(read_size == -1){
		const int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(9, "error while writing to STDOUT in parent process");
	}
	write(1, "\n", 1);

	close(fd);
	exit(0);
}

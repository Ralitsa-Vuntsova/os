#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	ssize_t fd = open("foobar.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1)
		err(1, "error while opening file foobar.txt");
	close(fd);

	pid_t child_pid1 = fork();
	if(child_pid1 == -1){
		err(2, "could not fork");
	}

	if(child_pid1 == 0){ // we are in first child process
		ssize_t fd1 = open("foobar.txt", O_WRONLY | O_APPEND);
		if(fd1 == -1){
			int olderr = errno;
			close(fd1);
			errno = olderr;
			err(3, "error while opening file foobar.txt");
		}
		if(write(fd1, "foo", 3) != 3){
			int olderr = errno;
			close(fd1);
			errno = olderr;
			err(4, "error while writing to file foobar.txt");
		}
		close(fd1);
		exit(0);
	}

	if(wait(NULL) == -1){
		err(5, "error while waiting for child");
	}
	
	const pid_t child_pid2 = fork();
	if(child_pid2 == -1){
		err(6, "could not fork");
	}

	if(child_pid2 == 0){ // we are in second child process
		ssize_t fd2 = open("foobar.txt", O_WRONLY | O_APPEND);
		if(fd2 == -1){
			int olderr = errno;
			close(fd2);
			errno = olderr;
			err(7, "error while opening file foobar.txt");	
		}
		if(write(fd2, "bar\n", 4) != 4){
			int olderr = errno;
			close(fd2);
			errno = olderr;
			err(8, "error while writing to file foobar.txt");			
		}

		close(fd2);
		exit(0);
	}
	
	if(wait(NULL) == -1)
		err(8,"could not wait all children processes");

	exit(0);
}

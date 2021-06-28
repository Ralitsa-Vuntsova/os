#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "invalid number of arguments");
	}

	int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd == -1){
		err(2, "error while opening file %s", argv[1]);
	}

	char* str1 = "foo\n";
	char* str2 = "bar\n";

	if(write(fd, str1, 2) != 2){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(3, "error while writing in file %s", argv[1]);
	}

	int pid = fork();
	if(pid == -1){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(4, "error while fork");
	}

	if(pid == 0){
		if(write(fd, str2, 4) != 4){
			int olderr = errno;
			close(fd);
			errno = olderr;
			err(5, "error while writing in file %s", argv[1]);
		}
		exit(0);
	}
	
	int status;
	int wait_code = wait(&status);

	if(wait_code == -1){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(6, "error while wait");
	}
	if(!WIFEXITED(status)){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(6, "error while wait");
	}
	if(WEXITSTATUS(status) != 0){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(6, "error while wait");
	}

	if(write(fd, str1+2, 2) != 2){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(7, "error while writing in file %s", argv[1]);
	}

	close(fd);
	exit(0);
}

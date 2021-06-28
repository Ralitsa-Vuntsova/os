#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	if (argc != 3) errx(1, "Invalid number of arguments");

	int fd1 = -1, fd2 = -1;

	fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
	{
		err(2, "Error while opening file %s", argv[1]);
	}

	fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1)
	{
		int olderr = errno;
		close(fd1);
		errno = olderr;
		err(3, "Error while opening file %s", argv[2]);
	}

	struct stat st;

	if (stat(argv[1], &st) == -1)
	{
		int olderr = errno;
		close(fd1);
		close(fd2);
		errno = olderr;
		err(4, "Error stat for file %s", argv[1]);
	}

	void* buf = malloc(st.st_size);

	if (buf == NULL){
		int olderr = errno;
		close(fd1);
		close(fd2);
		free(buf);
		errno = olderr;
		errx(5,"Not enough memory");
	}

	if (read(fd1, buf, st.st_size) != st.st_size){
		int olderr = errno;
		close(fd1);
		close(fd2);
		free(buf);
		errno = olderr;
		err(6,"Error while reading file %s", argv[1]);
	}
	
	fd1 = open(argv[1], O_TRUNC | O_WRONLY);
	if (fd1 == -1)
	{
		int olderr = errno;
		close(fd1);
		close(fd2);
		free(buf);
		errno = olderr;
		err(7, "Error while opening file %s", argv[1]);
	}
	
	char c[4096];
	ssize_t read_size;
	
	while ((read_size = read(fd2, &c, sizeof(c))) > 0){
		if (write(fd1, &c, read_size) != read_size){
			int olderr = errno;
			close(fd1);
			close(fd2);
			free(buf);
			errno = olderr;
			err(8,"Error while writing in file %s", argv[1]);
		}
	}

	fd2 = open(argv[2], O_TRUNC | O_WRONLY);
	if (fd2 == -1)
	{
		int olderr = errno;
		close(fd1);
		close(fd2);
		free(buf);
		errno = olderr;
		err(9, "Error while opening file %s", argv[2]);
	}

	if (write(fd2, buf, st.st_size) != st.st_size){
		int olderr = errno;
		close(fd1);
		close(fd2);
		free(buf);
		errno = olderr;
		err(10,"Error while writing in file %s", argv[2]);
	}

	close(fd1);
	close(fd2);
	free(buf);

	exit(0);
}

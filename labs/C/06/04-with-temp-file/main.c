#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1, "Invalid number of arguments!");
	}

	int fd1;
	int fd2;
	int fd3;
	char c[4096];

	fd1 = open(argv[1], O_RDWR);
	if(fd1 == -1){
		err(2, "Error opening file %s!", argv[1]);
	}

	fd2 = open(argv[2], O_RDWR);
	if(fd2 == -1){
		int olderr = errno;
		close(fd1);
		errno = olderr;
		err(3, "Error opening file %s!", argv[2]);
	}

	fd3 = open("temporary_file", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd3 == -1){
		int olderr = errno;
		close(fd1);
		close(fd2);
		errno = olderr;
		err(4, "Error opening file %s!", "temporary_file");
	}

	ssize_t read_size;

	while((read_size = read(fd1, &c, sizeof(c))) > 0){
		if(write(fd3, &c, read_size) != read_size)
		{
			int olderr = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno = olderr;
			err(5, "Error while writing in file %s!", "temporary.txt");
		}
	}

	lseek(fd1, 0, SEEK_SET);

	while((read_size = read(fd2, &c, sizeof(c))) > 0){
		if(write(fd1, &c, read_size) != read_size)
		{
			int olderr = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno = olderr;
			err(6, "Error while writing in file %s!", argv[1]);
		}
	}

	lseek(fd3, 0, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);

	while((read_size = read(fd3, &c, sizeof(c))) > 0){
		if(write(fd2, &c, read_size) != read_size)
		{
			int olderr = errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno = olderr;
			err(7, "Error while writing in file %s!", argv[2]);
		}
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}

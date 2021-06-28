#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1, "Invalid number of arguments! Usage: %s <file1> <file2>", argv[0]);
	}

	int fd1 = -1;
	int fd2 = -1;

	fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1)
	{
		err(2, "Error opening file %s!", argv[1]);
	}

	fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR );
	if(fd2 == -1)
	{
		int oldErr = errno;
		close(fd1);
		errno = oldErr;
		err(3, "Error opening file %s!", argv[2]);
	}

	char c[4096];
	ssize_t read_size;

	while((read_size = read(fd1, &c, sizeof(c))) > 0)
	{
		if(write(fd2, &c, read_size) != read_size)
		{
			int oldErr = errno;
			close(fd1);
			close(fd2);
			errno = oldErr;
			err(4, "Error writing in file %s!", argv[2]);
		}
	}

	close(fd1);
	close(fd2);

	exit(0);	
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>

int main()
{
	int fd1;
	int fd2;
	char c;

	if ((fd1 = open("foo.txt", O_RDONLY)) == -1)
	{
		err(1, "Error opening file %s!", "foo.txt");
	}

	if ((fd2 = open("bar.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
	{
		int olderr = errno;
		close(fd1);
		errno = olderr;
		err(2, "Error opening file %s!", "bar.txt");
	}

	while (read(fd1, &c, 1))
	{
		write(fd2, &c, 1);
	}

	close(fd1);
	close(fd2);
	exit(0);
}

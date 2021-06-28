#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		errx(1, "Wrong number of arguments");
	}

	struct stat st;
	if (stat(argv[1], &st) < 0)
	{
		errx(2, "Cannot stat file %s", argv[1]);
	}
	ssize_t size_f1 = st.st_size;

	if (stat(argv[2], &st) < 0)
	{
		errx(3, "Cannot stat file %s", argv[2]);
	}
	ssize_t size_f2 = st.st_size;

	if (size_f1 != size_f2)
	{
		errx(4, "First and second files are with different lengths");
	}

	if (size_f1 == 0 && size_f2 == 0)
	{
		errx(5, "First and second files are empty");
	}

	struct tripple_t
	{
		uint16_t position;
		uint8_t byte_f1; // byte in the first file
		uint8_t byte_f2; // byte in the second file
	};

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0)
	{
		err(6, "%s", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 < 0)
	{
		int olderrno = errno;
		close(fd1);
		errno = olderrno;
		err(7, "%s", argv[2]);
	}
	
	int fd3 = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd3 < 0)
	{
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(8, "%s", argv[3]);
	}

	char one;
	char two;

	struct tripple_t tripple;
	uint16_t pos = 0;

	while (read(fd1, &one, sizeof(one)) > 0 && read(fd2, &two, sizeof(two)) > 0)
	{
		if (one != two)
		{
			tripple.position = pos;
			tripple.byte_f1 = one;
			tripple.byte_f2 = two;

			write(fd3, &tripple, sizeof(tripple));
		}
		pos++;
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		errx(1, "Invalid number of arguments!");
	}

	int fd1;
	int i = 0;
	char c;

	if((fd1 = open(argv[1], O_RDONLY)) == -1)
	{
		err(2, "Error opening file %s!", argv[1]);
	}

	while(read(fd1, &c, 1))
	{
		if(c == '\n')
		{
			i = i + 1;
		}

		write(1, &c, 1);

		if(i == 10)
		{
			break;
		}
	}

	close(fd1);
	exit(0);
}

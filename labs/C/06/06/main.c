#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		errx(1, "Invalid number of arguments!");
	}

	int i;
	for(i = 1; i < argc; i++)
	{
		int fd = open(argv[i], O_RDONLY);
		if(fd == -1)
		{
			warn("Error opening file %s\n!", argv[i]);
			continue;
		}

		char c[4096];
		ssize_t read_size;

		while((read_size = read(fd, &c, sizeof(c))) > 0)
		{
			if(write(1, &c, read_size) != read_size)
			{
				warn("Error while writing to the stdout\n");
			}
		}

		close(fd);
	}

	exit(0);
}

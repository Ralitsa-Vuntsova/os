#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(1, "Invalid number of arguments!");
	}

	int fd1;
	int chars = 0;
	int words = 0;
	int lines = 0;
	char c;

	if ((fd1 = open(argv[1], O_RDONLY)) == -1)
	{
		err(2, "Error opening file %s!", argv[1]);
	}

	while(read(fd1, &c, 1) > 0)
	{
		if(c == '\n'){
			lines++;
			words++;
		}

		if(c == ' '){
			words++;
		}

		chars++;
	}

	printf("File name: %s\nNumber of lines: %d\nNumber of words: %d\nNumber of chars: %d\n", argv[1], lines, words, chars);

	close(fd1);
	exit(0);
}

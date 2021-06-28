/*
	foreach element in patch.bin 
		if orig in patch.bin matches the curr byte in f1.bin
			write new in f2.bin
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

struct content_t 
{
	uint16_t pos;
	uint8_t orig;
	uint8_t change;
};

int main(int argc, const char* argv[])
{
	if (argc != 4)
	{
		errx(1, "Usage: %s patch.bin f1.bin f2.bin ", argv[0]);
	}

	struct stat st;
	
	if(stat(argv[1], &st) < 0)
	{
		err(2, "Fail stat");
	}

	if(st.st_size % sizeof(struct content_t) != 0)
	{
		errx(3, "Invalid size of file %s", argv[1]);
	} 

	if(stat(argv[2], &st) < 0)
	{
		err(2, "Fail stat");
	}
	
	if(st.st_size > 0xFFFF)
	{
		warnx("Size of %s is greater than 0xFFFF", argv[2]);
	}
	
	int fdf1 = open(argv[2], O_RDONLY);
	if (fdf1 == -1) 
	{
		err(4, "Failed to open file %s", argv[2]);
	}

	int fdf2 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fdf2 == -1)
	{
		const int olderrno = errno;
		close(fdf1);
		errno = olderrno;
		err(4, "Failed to open file %s", argv[3]);
	}

	uint8_t a;
	ssize_t r = 0;
	ssize_t w = 0;

	while ( ((r = read(fdf1, &a, sizeof(a))) > 0) &&
			((w = write(fdf2, &a, sizeof(a)) > 0)) ) { }

	if ((r < 0) || (w < 0)) {
		const int olderrno = errno;
		close(fdf1);
		close(fdf2);
		errno = olderrno;
		err(5, "error reading/writing %s -> %s", argv[2], argv[3]);
	}

	close(fdf1);	

	int fdpatch = open(argv[1], O_RDONLY);
	if(fdpatch == -1)
	{	
		const int olderrno = errno;
		close(fdf2);
		errno = olderrno;
		err(4, "Failed to open file %s", argv[1]);
	} 
	
	struct content_t c;
	while(read(fdpatch, &c, sizeof(c)) == sizeof(c))
	{
		if(lseek(fdf2, c.pos, SEEK_SET) == -1)
		{
			err(6, "Cannot seek to %d in %s", c.pos, argv[3]);
		}
		
		read(fdf2, &a, sizeof(a));
		if(a == c.orig) 
		{
			if(lseek(fdf2, -1, SEEK_CUR) == -1) // !!!
			{
				err(6, "Cannot seek to %d in %s", c.pos, argv[3]);
			}

			write(fdf2, &(c.change), sizeof(c.change));
		} 
		else 
		{
			errx(7, "Error");
		}			
	}	
	
	close(fdf2);
	close(fdpatch);
	exit(0);
}


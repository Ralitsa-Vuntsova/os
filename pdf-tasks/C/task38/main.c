#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

struct rstruct_t 
{
	uint16_t pos;
	uint8_t len;
	uint8_t reserved;
};

int main(int argc, const char* argv[]) 
{
    if(argc != 5) 
    {
        errx(1, "Usage: ./main f1.dat f1.idx f2.dat f2.idx");
    }

    struct stat st;

    if (stat(argv[2], &st) < 0) 
    {
        err(2, "Failed stat");
    }

    if (st.st_size % sizeof(uint32_t) != 0) 
    {
        errx(3, "Invalid size of file %s", argv[2]);
    }
    
    if (stat(argv[1], &st) < 0) 
    {
		err(2, "Failed stat");
    }

    if (st.st_size > 0xFFFF) 
    {
		warnx("Size of %s greater than 0xFFFF", argv[1]);
    }	

    int f1dat = open(argv[1], O_RDONLY);
    if(f1dat == -1) 
    {
        err(4, "Failed to open the file");
    }

    int f1idx = open(argv[2], O_RDONLY);
    if(f1idx == -1) 
    {
        const int olderrno = errno; 
        close(f1dat);
        errno = olderrno;
        err(4, "Failed to open the file");
    }

    int f2dat = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR, S_IRWXG);
    if(f2dat == -1){
        const int olderrno = errno;
        close(f1dat);
        close(f1idx);
        errno = olderrno;
        err(4, "Failed to open the file");
    }

    int f2idx = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR, S_IRWXG);
    if(f2idx == -1){
        const int olderrno = errno;
        close(f1dat);
        close(f1idx);
        close(f2dat);
        errno = olderrno;
        err(4, "Failed to open the file");
    }

/*
	foreach X in f1.idx
		if string defined by X matches 'A' - 'Z'
	 		write string in f2.dat
			write proper Y entry in f2.idx
*/

	struct rstruct_t el;
	struct rstruct_t y = {0, 0, 0};
	while(read(f1idx, &el, sizeof(el)) == sizeof(el)) 
	{
		uint8_t a;
		if(lseek(f1dat, el.pos, SEEK_SET) == -1) // !!!
		{
			err(6, "Cannot seek to %d in %s", el.pos, argv[1]);
		}
		
		read(f1dat, &a, sizeof(a));
		 		
		if (a >= 'A' && a <= 'Z') 
		{
			y.len = el.len;
			
			write(f2dat, &a, sizeof(a));			
			for(int i = 1; i < el.len; i++)
			{
				read(f1dat, &a, sizeof(a));	
		i		write(f2dat, &a, sizeof(a));
			}

			write(f2idx, &y, sizeof(y));			

			y.pos += y.len;
		}
	}

	close(f1dat);
	close(f1idx);
	close(f2dat);
	close(f2idx);

    exit(0);
}




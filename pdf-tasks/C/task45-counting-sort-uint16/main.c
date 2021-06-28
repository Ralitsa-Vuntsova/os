#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, const char* argv[]) {
/*
	while(read input.bin file)
		keep statistics for the numbers in an array

	foreach element in the counting-sort-array
		 write in output.bin
	
*/
	if(argc != 3)
	{
		errx(1, "Usage: ./main input.bin output.bin");
	}
	
	struct stat st;
	if(stat(argv[1], &st) < 0)
	{
		err(2, "Failed stat for file %s", argv[1]);
	}
	
	if(st.st_size % sizeof(uint16_t) != 0)
	{
		errx(3, "Invalid size of file %s", argv[1]);
	}
	
	if(st.st_size / sizeof(uint16_t) > 0xFFFF)
	{
		warnx("Size of %s is greater than 0xFFFF. Problems may occur", argv[1]);
	}
	
	int fdinput = open(argv[1], O_RDONLY);
	if(fdinput == -1)
	{
		err(4, "Failed to open file %s", argv[1]);
	}	
	
	uint16_t counting[0xFFFF];
 	for (int i = 0; i < 0xFFFF; i++)
	{
		counting[i] = 0;
	}
	
	uint16_t el;
	while(read(fdinput, &el, sizeof(el)) == sizeof(el))
	{
		counting[el]++;
	}
	close(fdinput);
	
	int fdoutput = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fdoutput == -1)
	{
		const int olderrno = errno;
		close(fdinput);
		errno = olderrno;
		err(4, "Failed to open file %s", argv[2]);
	}

	// uint16_t = [0; 65536) = [0x0000; 0xFFFF)
	// uint16_t = [0; 65535] = [0x0000; 0xFEFF) little endian

	for(uint16_t i = 0; i < 0xFFFF; i++) 
	{
		while(counting[i] > 0) 
		{
			write(fdoutput, &i, sizeof(i));
			counting[i]--; 
		}
	}
	
	close(fdoutput);

	exit(0);
}


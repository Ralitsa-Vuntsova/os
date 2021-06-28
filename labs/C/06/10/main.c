#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "Invalid number of arguments. Usage %s <--option> <bin_file>", argv[0]);
	}

	struct stat st;
	if(stat(argv[2], &st) == -1){
		err(2, "Could not stat file %s", argv[2]);
	}

	if(st.st_size % sizeof(uint16_t) != 0){
		err(3, "Error in file %s", argv[2]);
	}

	int fd;
	fd = open(argv[2], O_RDONLY);
	if(fd < 0){
		err(4, "Error opening file %s", argv[2]);
	}

	uint8_t number;
	uint8_t max = 0;
	uint8_t min = 255;
	ssize_t read_size;

	while((read_size = read(fd, &number, sizeof(number))) == sizeof(number)){
		if(strcmp("--min", argv[1]) == 0 && number <= min){
			min = number;
		}
		if(strcmp("--max", argv[1]) == 0 && number >= max){
			max = number;
		}
		if(strcmp("--print", argv[1]) == 0){
			printf("%d\n", number);
		}
	}

	if(read_size == -1){
		int olderr = errno;
		close(fd);
		errno = olderr;
		err(5, "Error reading from file %s", argv[2]);
	}

	if(strcmp("--min", argv[1]) == 0){
		printf("%d\n", min);
	}
	if(strcmp("--max", argv[1]) == 0){
		printf("%d\n", max);
	}
	
	close(fd);
	exit(0);
}

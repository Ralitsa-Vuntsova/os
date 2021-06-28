#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>

int cmp(const void* a, const void* b) {
	return *((uint16_t*)a) - *((uint16_t*)b);
}

int main(int argc, char* argv[]){
	if (argc != 3) {
		errx(1, "Wrong argument count");
	}

	struct stat st;
	if (stat(argv[1], &st) < 0) {
		err(2, "failed stat for %s", argv[1]);
	}
	if (st.st_size % sizeof(uint16_t) != 0) {
		errx(3, "wrong size of %s", argv[1]);
	}

	uint16_t num_el = st.st_size / sizeof(uint16_t);
	uint16_t half = num_el / 2;

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		err(4, "failed open of %s", argv[1]);
	}

	int fd_temp1 = open("temp1", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_temp1 < 0) {
		int olderrno = errno;
		close(fd);
		errno = olderrno;
       	err(5, "failed open of temp1");
    }

	uint16_t *p = malloc(half*sizeof(uint16_t));
	if (!p) {
		err(20, "failed malloc");
	}

	size_t res = read(fd, p, half*sizeof(uint16_t));
    	if (res != half*sizeof(uint16_t)) {
        	int olderrno = errno;
        	free(p);
        	close(fd);
      		close(fd_temp1);
        	errno = olderrno;
        	err(6,"failed to read from %s", argv[1]);
    	}

	qsort(p, half, sizeof(uint16_t), cmp);

	res = write(fd_temp1, p, half*sizeof(uint16_t));
	if (res != half*sizeof(uint16_t)) {
		int olderrno = errno;
		close(fd);
		close(fd_temp1);
		free(p);
		errno = olderrno;
		err(7, "failed write to temp1");
	}

	free(p);	

	half = num_el - half;

	int fd_temp2 = open("temp2", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    	if (fd_temp2 < 0) {
       		int olderrno = errno;
       		close(fd);
       		close(fd_temp1);
			errno = olderrno;
       		err(8, "failed open of temp2");
    	}

    p = malloc(half*sizeof(uint16_t));
	if (!p) {
        	err(20, "failed malloc");
    	}

    	res = read(fd, p, half*sizeof(uint16_t));
    	if (res != half*sizeof(uint16_t)) {
        	int olderrno = errno;
        	free(p);
        	close(fd);
			close(fd_temp1);
        	close(fd_temp2);
        	errno = olderrno;
        	err(9,"failed to read from %s", argv[1]);
   	    }

    	qsort(p, half, sizeof(uint16_t), cmp);
    
		res = write(fd_temp2, p, half*sizeof(uint16_t));
    	if (res != half*sizeof(uint16_t)) {
        	int olderrno = errno;
        	close(fd);
        	close(fd_temp1);
			close(fd_temp2);
        	free(p);
        	errno = olderrno;
        	err(10, "failed write to temp1");
    	}
	
	free(p);

	if (lseek(fd_temp1, 0, SEEK_SET) < 0) {
		int olderrno = errno;
        	close(fd);
        	close(fd_temp1);
        	close(fd_temp2);
        	errno = olderrno;
			err(11, "failed lseek for temp1");
	}

	if (lseek(fd_temp2, 0, SEEK_SET) < 0) {
        	int olderrno = errno;
        	close(fd);
        	close(fd_temp1);
        	close(fd_temp2);
        	errno = olderrno;
			err(12, "failed lseek for temp2");
    }

	fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		int olderrno = errno;
        	close(fd_temp1);
        	close(fd_temp2);
        	errno = olderrno;
        	err(13, "failed open of %s", argv[2]);
	}	
	
	uint16_t a;
	uint16_t b;

	while ((read(fd_temp1, &a, sizeof(a)) == sizeof(a)) && (read(fd_temp2, &b, sizeof(b)) == sizeof(b))) {
        	if (a <= b) {
            		write(fd, &a, sizeof(a));
            		lseek(fd_temp2, -1*sizeof(b), SEEK_CUR);
        	} else {
            		write(fd, &b, sizeof(b));
            		lseek(fd_temp1, -1*sizeof(a), SEEK_CUR);
        	}
    	}

	if (read(fd_temp2, &b, sizeof(b)) != sizeof(b)) {
		write(fd, &a, sizeof(a));
	} else {
		write(fd, &b, sizeof(b));
	}

    	while (read(fd_temp1, &a, sizeof(a)) == sizeof(a)) {
        	write(fd, &a, sizeof(a));
    	}

    	while (read(fd_temp2, &b, sizeof(b)) == sizeof(b)) {
        	write(fd, &b, sizeof(b));
    	}

    	close(fd_temp1);
    	close(fd_temp2);
    	close(fd);

	exit(0);
}

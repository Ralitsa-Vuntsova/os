#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

int cmp(const void* a, const void* b)
{
    return *((unsigned char*)a) - *((unsigned char*)b);
}

int main(int argc, char* argv[])
{
    if(argc != 3){
		errx(1, "Invalid number of arguments. Usage %s <arg1> <arg2>", argv[0]);
	}

    int fd1 = open(argv[1], O_RDONLY);
    if(fd1 < 0)
        err(2, "error openning file %s", argv[1]);

    struct stat st;
    if(stat(argv[1], &st) == -1)
        err(3, "error stat for file %s", argv[1]);

    int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd2 < 0){
	    int olderrno = errno;
	    close(fd1);
	    errno = olderrno;
	    err(4, "error opening file %s", argv[2]);
    }

    if(st.st_size == 0)
    {
		int olderrno = errno;
        close(fd1);
		close(fd2);
        errno = olderrno;
		err(5, "first file is empty");
    }

    void *buf = malloc(st.st_size);
    if(!buf)
    {
        int olderrno = errno;
        close(fd1);
        close(fd2);
        errno = olderrno;
        err(5, "no memory");
    }

    if(read(fd1, buf, st.st_size) != st.st_size)
    {
        int olderrno = errno;
		free(buf);
        close(fd1);
        close(fd2);
        errno = olderrno;
        err(6, "error while reading file %s", argv[1]);
    }

    qsort(buf, st.st_size, sizeof(uint8_t), cmp);

    if(write(fd2, buf, st.st_size) != st.st_size)
    {
        int olderrno = errno;
        free(buf);
        close(fd1);
        close(fd2);
        errno = olderrno;
        err(7, "error while writing file %s", argv[2]);
    }

    free(buf);
    close(fd1);
    close(fd2);
    exit(0);
}

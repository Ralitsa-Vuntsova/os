#include <stdio.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>                                  
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

const size_t BUFFER_SIZE = 4096;

int main(int argc, char * argv[]) {
    int fd;

    if (argc != 2) {
        errx(1, "Missing argument!");
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        err(1, "Cannot open file %s", argv[1]);
    }

    uint32_t counts[256] = { 0 };
    uint8_t buffer[BUFFER_SIZE];
    ssize_t read_size;
    uint32_t total_size = 0;

    while ((read_size = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < read_size; ++i) {
            counts[buffer[i]]++;
            total_size++;
        }
    }

    // reset marker to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    uint16_t byte_id = 0;

    // while the counts are not fully traversed
    while (byte_id < 256) {
        // fill the buffer for writing
        size_t pos = 0;
        while (pos < BUFFER_SIZE && byte_id < 256) {
            if (counts[byte_id] == 0) {
                byte_id++; // move to next byte
            } else {
                buffer[pos] = byte_id; // write the byte inside the buffer
                counts[byte_id]--; // decrement counter for this byte
                pos++; // move one step forward in the buffer
            }
        }

        ssize_t write_size = pos;
        if (write(fd, buffer, write_size) != write_size) {
	    const int olderrno = errno;
	    close(fd);
	    errno = olderrno;
            err(3, "cannot write to file");
        }
    }

    close(fd);
    exit(0);
}


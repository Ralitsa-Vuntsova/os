#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

// DISCLAIMER! this solution works only with relative path names

void copy_file_to_dir(char * src_filename, char * dest_dirname) {
	// dest_dirname + '/' + src_filename + '\0'
	char * dest_filename = malloc(strlen(dest_dirname) + 1 + strlen(src_filename) + 1);

	strncpy(dest_filename, dest_dirname, strlen(dest_dirname)); 
	strncat(dest_filename, "/", 2); // dest_filename += "/"; where "/" == { '/', '\0' }
	strncat(dest_filename, src_filename, strlen(src_filename)); // dest_filename += src_filename

	printf("copying file %s to dest %s\n", src_filename, dest_filename);

	int src_fd;
	if ((src_fd = open(src_filename, O_RDONLY)) == -1) {
		err(2, "cannot open source file %s", src_filename);
	}

	int dest_fd = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (dest_fd == -1) {
		int olderrno = errno;	
		close(src_fd);
		errno = olderrno;
		err(2, "cannot open dest file %s", dest_filename);
	}

	char buff[4096];
	ssize_t read_size;

	while ((read_size = read(src_fd, buff, sizeof(buff))) > 0) {
		if (read_size != write(dest_fd, buff, read_size)) {
			int olderrno = errno;
			close(src_fd);
			close(dest_fd);
			errno = olderrno;
			err(2, "cannot copy %ld bytes from %s to %s", read_size, src_filename, dest_filename);
		}
	}

	close(src_fd);
	close(dest_fd);
	free(dest_filename);
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		errx(1, "not enough arguments");
	}

	// is the last argument a directory?
	char * dest_name = argv[argc - 1];

	struct stat dest_stats;

	if (stat(dest_name, &dest_stats) == -1) {
		err(2, "cannot stat file %s", dest_name);
	}

	if (!S_ISDIR(dest_stats.st_mode)) {
		errx(3, "last argument expected to be a directory");
	}

	for (int i = 1; i < argc - 1; ++i) {
		copy_file_to_dir(argv[i], dest_name);
	}

	return 0;
}

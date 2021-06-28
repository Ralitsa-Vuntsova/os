#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

struct header_t{
	uint32_t magic;
	uint8_t header_version;
	uint8_t data_version;
	uint16_t count;
	uint32_t reserved1;
	uint32_t reserved2;
};

struct version1_t{
	uint16_t offset;
	uint8_t original_byte;
	uint8_t new_byte;
};

struct version2_t{
	uint32_t offset;
	uint16_t original_word;
	uint16_t new_word;
};

int main(int argc, char* argv[]){
	if (argc != 4){
		errx(1, "Invalid number of arguments. Usage: %s <patch.bin> <f1.bin> <f2.bin>", argv[0]);
	}

	int patchfd;
	if ((patchfd = open(argv[1], O_RDONLY)) == -1){
		err(2, "Could not open file %s", argv[1]);
	}
	struct header_t header;
	if ((read(patchfd, &header, sizeof(header))) != sizeof(header)){
		int olderr = errno;
		close(patchfd);
		errno = olderr;
		err(3, "Could not read header from patch file %s", argv[1]);
	}
	size_t version1_size = sizeof(struct version1_t);
	size_t version2_size = sizeof(struct version2_t);
	size_t header_size = sizeof(struct header_t);
	size_t size;
	if(header.data_version == 0){
		size = version1_size;
	}
	else{
		size = version2_size;
	}
	// razmera na patch.bin trqbva da e header_size + (count*size)
	struct stat st;
	if(stat(argv[1], &st) == -1){
		int olderr = errno;
        close(patchfd);
        errno = olderr;
		err(4, "Could not stat file %s", argv[1]);
	}
	if(st.st_size%(header_size+(size*header.count)) != 0){
		int olderr = errno;
        close(patchfd);
        errno = olderr;
		errx(5, "Invalid size of file %s", argv[1]);
	}
	// razmera na f1 trqbva da e uint8_t za version1 i uint16_t za version2
	if(size == version1_size){
		if(stat(argv[2], &st) == -1){
			int olderr = errno;
        	close(patchfd);
        	errno = olderr;
			err(6, "Could not stat file %s", argv[2]);
		}
		if(st.st_size%sizeof(uint8_t) != 0){
			int olderr = errno;
        	close(patchfd);
        	errno = olderr;
			errx(7, "Invalid size of file %s", argv[2]);
		}
	}
	else{
		if(stat(argv[2], &st) == -1){
			int olderr = errno;
        	close(patchfd);
        	errno = olderr;
			err(6, "Could not stat file %s", argv[2]);
        }
		if(st.st_size%sizeof(uint16_t) != 0){
			int olderr = errno;
     	    close(patchfd);
        	errno = olderr;
			errx(7, "Invalid size of file %s", argv[2]);
		}
	}
	// trqbva da otvorq f1 i da suzdam f2
	int f1fd = open(argv[2], O_RDONLY);
	if(f1fd == -1){
		int olderr = errno;
        close(patchfd);
        errno = olderr;
		err(8, "Could not open file %s", argv[2]);
	}

	int f2fd = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR, S_IWUSR);
	if(f2fd == -1){
		int olderr = errno;
        close(patchfd);
		close(f1fd);
        errno = olderr;
		err(9, "Could not open file %s", argv[3]);
	}
	// trqbva da kopiram f1 v f2
	if(size == version1_size){
		uint8_t a;
		while ((read(f1fd, &a, sizeof(uint8_t))) == sizeof(uint8_t)){
			if(write(f2fd, &a, sizeof(a)) != sizeof(uint8_t)){
				int olderr = errno;
	        	close(patchfd);
    	    	close(f1fd);
				close(f2fd);
        		errno = olderr;
        		err(10, "Could not write in file %s", argv[3]);
			}
		}
	}
	else{
		uint16_t a;
        while ((read(f1fd, &a, sizeof(uint16_t))) == sizeof(uint16_t)){
            if(write(f2fd, &a, sizeof(a)) != sizeof(uint16_t)){
                int olderr = errno;
                close(patchfd);
                close(f1fd);
                close(f2fd);
                errno = olderr;
                err(10, "Could not write in file %s", argv[3]);
            }
        }
	}
	// trqbva da promenq f2 vuz osnova na patch - cheta f1 i f2 ednovremenno i sledq za promeni
	if(size == version1_size)
	{
		struct version1_t version1;
		while((read(patchfd, &version1, version1_size)) == sizeof(struct version1_t))
		{
			lseek(f2fd, version1.offset, SEEK_SET); // proverka
			uint8_t b;
			if ((read(f2fd, &b, sizeof(b))) == sizeof(b)) // lipsva else
			{
				if(b == version1.original_byte)
				{
					lseek(f2fd, -1, SEEK_CUR); // proverka
					write(f2fd, &version1.new_byte, sizeof(uint8_t)); // proverka
				}
				else{
					int olderr = errno;
                	close(patchfd);
                	close(f1fd);
                	close(f2fd);
                	errno = olderr;
                	errx(11, "Error");	
				}
			}
		}
	}
	else{
		struct version2_t version2;
		while((read(patchfd, &version2, version2_size)) == sizeof(struct version2_t)){
            lseek(f2fd, (version2.offset - 1) * sizeof(uint16_t), SEEK_SET); // proverka
            uint16_t b;
            if ((read(f2fd, &b, sizeof(b))) == sizeof(b)){ // lipsva else
                if(b == version2.original_word){
                    lseek(f2fd, -1 * sizeof(uint16_t), SEEK_CUR); // proverka
                    write(f2fd, &version2.new_word, sizeof(uint16_t)); // proverka
                }
                else{
                    int olderr = errno;
                    close(patchfd);
                    close(f1fd);
                    close(f2fd);
                    errno = olderr;
                    errx(11, "Error");  
				}
			}
	 	}		
	}

	close(patchfd);
	close(f1fd);
	close(f2fd);

	exit(0);
}

// include headers

struct complect_t{
	uint32_t post_offset;
	uint32_t post_len;
	uint8_t pre_offset;
	uint8_t len;
	uint16_t in_offset;
	uint16_t in_len;
	uint64_t suf_offset;
	uint64_t suf_len;
};

int main(int argc, char* argv[]){
	if (argc != 7){
		errx(1, "Invalid number of arguments. Usage: %s affix postfix prefix infix suffix crucifixus", argv[0]);
	}
	char *affix = argv[1];
	char *postfix = argv[2];
	char *prefix = argv[3];
	char *infix = argv[4];
	char *suffix = argv[5];
	char *crucifixus = argv[6];
	// trqbva da otvorq vsichki failove kato posledniq go suzdavam
	int fd1 = open(affix, O_RDONLY);
	if (fd1 == -1){
		err(1, "Could not open file %s", affix);
	}
	int fd2 = open(postfix, O_RDONLY);
    if (fd2 == -1){
		int olderr = errno;
		close(fd1);
		errno = olderr;
        err(2, "Could not open file %s", postfix);
    }
	int fd3 = open(prefix, O_RDONLY);
    if (fd3 == -1){
        int olderr = errno;
        close(fd1);
		close(fd2);
        errno = olderr;
        err(3, "Could not open file %s", prefix);
    }
	int fd4 = open(infix, O_RDONLY);
    if (fd4 == -1){
        int olderr = errno;
        close(fd1);
		close(fd2);
		close(fd3);
        errno = olderr;
        err(4, "Could not open file %s", infix);
    }
	int fd5 = open(suffix, O_RDONLY);
    if (fd5 == -1){
        int olderr = errno;
        close(fd1);
		close(fd2);
		close(fd3);
		close(fd4);
        errno = olderr;
        err(5, "Could not open file %s", suffix);
    }
	int fd6 = open(crucifixus, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd6 == -1){
        int olderr = errno;
        close(fd1);
		close(fd2);
		close(fd3);
		close(fd4);
		close(fd5);
        errno = olderr;
        err(6, "Could not open file %s", crucifixus);
    }
	// namiram count-a ot hedura na vsichki failove i validiram razmerite - pokazvam samo s purviya fail
	int i = 0;
	uint16_t a;
	uint16_t affix_count;
	while(read(fd1, &a, sizeof(a)) == sizeof(a)){
		i++; 
		if(i == 3){
			affix_count = a;
			break;
		}
	}
	lseek(fd1, 16 * sizeof(uint8_t), SEEK_SET);
	struct stat st;
	if(stat(fd1, &st) == -1){
		int olderr = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
		close(fd6);
        errno = olderr;
        err(7, "Could not stat file %s", affix);
	}
	if(((st.st_size - (16 * (sizeof(uint8_t))))%(affix_count*uint16_t)) != 0){
		int olderr = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
		close(fd6);
        errno = olderr;
        err(7, "Invalid size for file %s", affix);
	}
	// cheta affix po komplekti, cheta suotvetniq fail, otivam na nomera, pisha elemntite do length v crucifixus - pokazvam samo za postfix
	struct complect_t complect;
	while((read(affix, &complect, sizeof(complect))) == sizeof(complect)){
		lseek(fd2, (complect.post_offset - 1) * uint32_t, SEEK_SET); // proverka
		uint32_t post;
		uint32_t cnt = 0;
		while(read(fd2, &post, sizeof(post)) == sizeof(post)){
			++cnt;
			write(fd6, &post, sizeof(post)); // proverka
			if(cnt == complect.post_len){
				break;
			}
		}
	}

	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
	close(fd5);
	close(fd6);
	exit(0);
}

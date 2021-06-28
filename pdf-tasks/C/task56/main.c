/* ./main input.lfld output.bin

input.lfld - zapis na potok:
	- v momentite, v koito nqma signal ot seriiniqt port, sa vkarali izkustveno bait 0x55 v potoka - t.e. trqbva da gi mahna;
	- realnite baitove 0x00, 0xFF, 0x55, 0x7D se kodirali posredstvom izkluchvashto-ili s 0x20, kato polucheniqt bait se izprashtal prez potoka, predhojdan ot 0x7D - t.e. trqbva da si vurna realnite baitove i da mahna predhojdashtite 0x7D.

output.bin - trqbva da se generira, sudurja originalnite danni
*/

// include headers

int main(){
	if(argc != 3){
		errx(1, "Invalid number of arguments. Usage: %s input output", argv[0]);
	}
	// pravq pipe, fork-vam, pravq exec cat na input, cheta bait po bait, ako bait e 0x55 - continue, v protiven sluchai - prochitam 0x7D, cheta modificiraniq bait, obrabotvam go da stane kato  originalniq i go zapisvam v output
	int output_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(output_fd == -1){
		err(2, "Could not open file %s", argv[2]);
	}

	int pipefd[2];
	if(pipe(pipefd) == -1){
		int olderr = errno;
		close(output_fd);
		errno = olderr;
		err(3, "Could not create a pipe");
	}
	pid_t child_pid = fork();
	if(child_pid == -1){
		 int olderr = errno;
         close(output_fd);
         errno = olderr;
		 err(4, "Could not fork");
	}
	if(child_pid == 0){
		close(pipefd[0]);
		dup2(pipefd[1], 1); // proverka
		if(execlp("cat", "cat", argv[1], 0) == -1){
			int olderr = errno;
        	close(output_fd);
        	errno = olderr;
			err(5,"Could not exec");
		}
	}
	close(pipefd[1]);
	uint8_t a;
	uint8_t b;
	while(read(pipefd[0],&a, sizeof(a)) == sizeof(a)){
		if(a == 0x55){
			continue;
		}
		else{
			if(a == 0x7D){
				continue;
			}
			else{
				b = a ^ 0x20;
				write(output_fd, &b, sizeof(b)); // proverka	
			}
		}
	}

	wait(NULL);
	close(output_fd);
	exit(0);
}

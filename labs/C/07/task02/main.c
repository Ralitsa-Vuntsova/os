#include <err.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "invalid number of arguments");
	}

	if((execl("/bin/ls", "ls", argv[1], NULL)) == -1){
		err(2, "error");
	}

	exit(0);
}

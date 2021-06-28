#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	while(1){
		if((write(1, "Enter command: ", 15)) != 15){
			err(1, "Error while writing to stdout");
		}

		char buff[1024];
		ssize_t read_size = read(0, &buff, sizeof(buff));
		if(read_size < 0){
			err(2, "Error while reading from stdin");
		}

		buff[read_size - 1] = '\0'; // !!!

		if(strcmp(buff, "exit") == 0){
			break;
		}

		pid_t child = fork();
		if(child == -1){
			err(3, "Could not fork");
		}
		if(child == 0){
			if(execlp(buff, buff, NULL) == -1){
				err(4, "Could not exec command");
				continue;
			}
		}

		wait(NULL);
	}

	exit(0);
}

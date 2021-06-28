#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

int execChild(char* params[]) {
	int pid = fork();
	if(pid < 0) {
		err(1,"Couldn't fork");
	}
	if(pid == 0){
		if(execvp(params[0],params) < 0) {
			err(2, "Error during subcommand execution");
		}
	}
	int status = 0;
	wait(&status);
	return WEXITSTATUS(status);
}

void safe_write(int fd, char* string) {
	if(write(fd, string, strlen(string)) < 0) {
		err(5, "Cannot write in log file");
	}
}

void fileLog(time_t start, time_t end, int status) {
	int fd = open("run.log", O_WRONLY | O_APPEND | O_CREAT, 0666);
	if(fd < 0) {
		err(4, "Cannot open log file");
	}

	char string[100];
	sprintf(string, "%ld ", start);
	safe_write(fd, string);
	sprintf(string, "%ld ", end);
	safe_write(fd, string);
	sprintf(string, "%d", status);
	safe_write(fd, string);
	safe_write(fd, "\n");

	close(fd);
}

bool execution(int limit, char* params[]) {
	time_t startTime = time(NULL);
	int status = execChild(params);
	time_t exitTime = time(NULL);
	fileLog(startTime, exitTime, status);	
	return (exitTime - startTime < limit) && status != 0;
}

int main(int argc, char** argv) {
	if (argc < 3) {
		errx(3,"Usage: %s P Q Q-params", argv[0]);
	}

	if(argv[1] < 1 || argv[1] > 9){
		errx(6, "Invalid argument.");
	}
	int sec = argv[1];

	char* Qargs[100];
	int count = 0;
	for(int i = 2; i < argc; i++) {
		Qargs[count++] = argv[i];
	}
	Qargs[count] = NULL;
	
	int condCount = 0;
	while(condCount < 2) {
		if(execution(sec, Qargs))
			condCount++;
		else condCount = 0;
	}

	return 0;
}

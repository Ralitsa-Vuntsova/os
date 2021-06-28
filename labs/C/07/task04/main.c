#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
	int n = 1000000;

	if(fork() > 0){
		// parent process
		for(int i = 0; i < n; i++){
			printf("father\n");
		}
	}
	else{
		// child process
		for(int i = 0; i < n; i++){
			printf("son\n");
		}
	}

	exit(0);
}

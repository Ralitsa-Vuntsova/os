#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

char* assertCommand(int argc, char* argv[]) {
	if(argc>2) {
		errx(1,"Wrong number of arguments. Usage: %s [cmd]",argv[0]);	
	}
	if(argc==2) {
		if(strlen(argv[1])>4) {
			errx(2,"Error: maximum length of cmd's name is 4");
		}
		return argv[1];
	}
	return "echo";
}

bool isDelimiter(const char c) {
	return c==(char)0x20 || c==(char)0x0A;
}

void exec_cmd(char* cmd,char args[2][5]) {
	char* real_argv[4];
	real_argv[0]=cmd;
	real_argv[3]=(char*)NULL;
	if(args[0][0]=='\0') {
		//No args ==> exit
		return;
	}
	real_argv[1]=args[0];
	if(args[1][0]=='\0') {
		//1 arg only
		real_argv[2]=(char*)NULL;
	} else {
		//2 args
		real_argv[2]=args[1];
	}
	
	const pid_t pid=fork();
	if(pid==-1) {
		err(4,"Cannot fork");
	}
	if(pid==0) {
		if(execvp(cmd,real_argv)==-1) {
			err(5,"Cannot execvp");
		}
	}
	if(wait(NULL)==-1) {
		err(6,"Cannot wait for cmd to finish");
	}
}

int main(int argc, char* argv[]) {
	char* cmd = assertCommand(argc,argv);
	char cmd_args[2][5];
	cmd_args[0][0]='\0';
	cmd_args[1][0]='\0';
	uint8_t row=0,col=0;
	while(1) {
		ssize_t r = read(0, &cmd_args[row][col++],sizeof(char));
		if(r==-1) {
			err(3,"Cannot read from STDIN");
		}
		if(r==0) {
			break;//Input is over
		}
		//At this point, 1 byte was read
		if(!isDelimiter(cmd_args[row][col-1])) {
			if(col==5) {
				errx(4,"Error. Arguments' size can be <=4");
			}
		} else {
			if(row==1) {
				cmd_args[row][col-1]='\0';
				exec_cmd(cmd,cmd_args);
				
				//Next 2 lines are to ensure that no more cmds get called in case the input ends here
				cmd_args[0][0]='\0';
				cmd_args[1][0]='\0';
			
				row=0;
			} else {
				cmd_args[row][col-1]='\0';
				row++;
			}
			col=0;
		}
	}
	//After we break out of the loop(which can only happen when input is over), there still might be unfinished commands
	if(cmd_args[0][0]=='\0') {
		exit(0);//No more cmds
	} 
	exec_cmd(cmd,cmd_args);

	exit(0);
}

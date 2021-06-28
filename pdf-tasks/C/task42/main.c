#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

int charIn(const char c, const char* SET) {//If c is in SET, returns the index of its occurence, otherwise -1
	for(size_t i=0;i<strlen(SET);i++) {
		if(c==SET[i]) {
			return i;
		}
	}
	return -1;
}

void assertEqualSizes(const char* str1, const char* str2) {
	if(strlen(str1)!=strlen(str2)) {
		errx(3,"Error: %s and %s must have the same size!",str1,str2);
	}
}
void optionNone(const char* SET1, const char* SET2) {
	assertEqualSizes(SET1,SET2);
	char c;
	while(1) {
		int rBytes=read(0,&c,sizeof(c));
		if(rBytes==-1) {
			err(2,"Cannot read from STDIN");
		}
		if(rBytes==0) {
			return;//We're done
		}
		int i=charIn(c,SET1);
		if(i!=-1) {
			c=SET2[i];
		}
		write(1,&c,sizeof(c));
	}	
}

void optionD(const char* SET) {
	char c;
	while(1) {
		int rBytes=read(0,&c,sizeof(c));
		if(rBytes==-1) {
			err(2,"Cannot read from STDIN");
		}
		if(rBytes==0) {
			return;//We're done
		}
		if(charIn(c,SET)==-1) {
			write(1,&c,sizeof(c));
		}
	}
}

void optionS(const char* SET) {
	char c;
	bool skipReading=false;
	while(1) {
		if(!skipReading) {
			int rBytes=read(0,&c,sizeof(c));
			if(rBytes==-1) {
				err(2,"Cannot read from STDIN");
			}
			if(rBytes==0) {
				return;
			}
		}
		if(skipReading) {
			skipReading=false;
		}
		write(1,&c,sizeof(c));
		if(charIn(c,SET)!=-1) {
			//Skip all subsequent equal characters
			char next_c;
			while(1) {
				int rBytes=read(0,&next_c,sizeof(next_c));
				if(rBytes==-1) {
					err(2,"Cannot read from STDIN");
				}
				if(rBytes==0) {
					return;//We're done
				}
				if(next_c!=c) {
					skipReading=true;
					c=next_c;
					break;
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {
	if(argc!=3) {
		errx(1,"Wrong usage. Expected 2 args, got %d",argc-1);
	}
	if(strcmp(argv[1],"-s")==0) {
		optionS(argv[2]);
	} else if(strcmp(argv[1],"-d")==0) {
		optionD(argv[2]);
	} else {
		optionNone(argv[1],argv[2]);
	}
		
	exit(0);
}

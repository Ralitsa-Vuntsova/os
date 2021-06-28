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

struct record_t {
	uint32_t uid;
	uint16_t unused1;
	uint16_t unused2;
	uint32_t start_time;
	uint32_t finish_time;
};

struct pair_t {
	uint32_t uid;
	uint32_t sessionLen;
};

const uint8_t recordSize = 3*sizeof(uint32_t) + 2*sizeof(uint16_t);

uint32_t averageSessionLength(const struct record_t* records,size_t records_count) {
	int sum=0;
	for(size_t i=0;i<records_count;i++) {
		sum+=records[i].finish_time-records[i].start_time;
	}
	return sum/records_count;
}

uint32_t dispersion(const struct record_t* records, size_t records_count) {
	uint32_t averageSL=averageSessionLength(records,records_count);
	uint64_t sum=0;
	for(size_t i=0;i<records_count;i++) {
		uint32_t term=records[i].finish_time-records[i].start_time-averageSL;
		sum+=term*term;
	}
	return sum/records_count;
}

void updateUsers(struct pair_t users[2048],size_t* currentCount,struct pair_t user) {
	for(size_t i=0;i<*currentCount;i++) {
		if(users[i].uid==user.uid) {
			if(users[i].sessionLen<user.sessionLen) {
				users[i].sessionLen=user.sessionLen;
			}
			return;
		}
	}
	users[*currentCount]=user;
	(*currentCount)++;
}

void printUser(struct pair_t user) {
	printf("UID: %d. Longest session: %d\n", user.uid, user.sessionLen);
}

int main(int argc, char* argv[]) {
	if(argc!=2) {
		errx(1,"Wrong number of arguments. Usage: %s <file>", argv[0]);
	}
	struct stat st;
	if(stat(argv[1],&st)==-1) {
		err(2,"Cannot stat file %s",argv[1]);
	}
	if(st.st_size%recordSize!=0) {
		errx(3,"Error: input file %s is corrupted",argv[1]);
	}
	int fd=open(argv[1],O_RDONLY);
	if(fd==-1) {
		err(4,"Cannot open file %s for reading", argv[1]);
	}
	struct record_t* records=(struct record_t*)malloc(st.st_size);
	if(read(fd,records,st.st_size)!=st.st_size) {
		const int olderrno=errno;
		close(fd);
		free(records);
		errno=olderrno;
		err(5,"Cannot read records from file %s",argv[1]);
	}
	close(fd);
	
	size_t recordCount=st.st_size/recordSize;
	uint32_t disp=dispersion(records, recordCount);
	size_t uniqueUsersCount=0;
	struct pair_t users[2048];

	struct pair_t currentUser;
	for(size_t i=0;i<recordCount;i++) {
		currentUser.uid=records[i].uid;
		currentUser.sessionLen=records[i].finish_time-records[i].start_time;
		if(currentUser.sessionLen*currentUser.sessionLen>disp) {
			updateUsers(users,&uniqueUsersCount,currentUser);
		}
	}
	free(records);
	for(size_t i=0;i<uniqueUsersCount;i++) {
		printUser(users[i]);
	}

	exit(0);
}


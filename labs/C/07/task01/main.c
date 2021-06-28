#include <err.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	if(execl("/bin/date", "date", (char*)NULL) == -1){
		err(1, "error");
	}

	exit(0);
}

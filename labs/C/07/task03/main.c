#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	if(execlp("sleep", "sleep", "60", NULL) == -1){
		err(1, "error");
	}

	exit(0);
}

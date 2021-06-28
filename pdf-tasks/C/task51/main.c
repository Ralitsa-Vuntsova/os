#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

// cut -d ":" -f 7 /etc/passwd | sort | uniq -c | sort -n
/*
pipe a
fork
in child close a[0], redirect stdout to a[1], exec cut
in parent close a[1]
pipe b
fork
in child close b[0], redirect stdin to a[0] and stdout to b[1], exec sort
in parent close b[1], a[0]
pipe c
fork
in child close c[0], redirect stdin to b[0] and stdout to c[1], exec uniq
in parent close c[1], b[0], redirect stdin to c[0], exec sort
*/

int main(){
	int a[2];

	if(pipe(a) == -1){
		err(1, "Could not create pipe");
	}

	pid_t child1 = fork();
	if(child1 == -1){
		err(2, "Could not fork");
	}
	if(child1 == 0){
		close(a[0]);
		if(dup2(a[1], 1) == -1){
			err(3, "Could not duplicate file descriptor");
		}
		if((execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", NULL)) == -1){
			err(4, "Could not exec command");
		}
	}
	close(a[1]);

	int b[2];
	if(pipe(b) == -1){
		err(5, "Could not create pipe");
	}
	
	pid_t child2 = fork();
	if(child2 == -1){
		err(6, "Could not fork");
	}
	if(child2 == 0){
		if(dup2(a[0],0) == -1){
			err(7, "Could not duplicate file descriptor");
		}
		close(b[0]);
		if(dup2(b[1], 1) == -1){
			err(8, "Could not duplicate file descriptor");
		}
		if(execlp("sort", "sort", NULL) == -1){
			err(9, "Could not exec command");
		}
	}
	close(b[1]);
	close(a[0]);

	int c[2];
	if(pipe(c) == -1){
		err(10, "Could not create pipe");
	}

	pid_t child3 = fork();
	if(child3 == -1){
		err(11, "Could not fork");
	}
	if(child3 == 0){
		if(dup2(b[0], 0) == -1){
			err(12, "Could not duplicate file descriptor");
		}
		close(c[0]);
		if(dup2(c[1], 1) == -1){
			err(13, "Could not duplicate file descriptor");
		}
		if(execlp("uniq", "uniq", "-c", NULL) == -1){
			err(14, "Could not exec command");
		}
	}
	close(c[1]);
	close(b[0]);

	if(dup2(c[0], 0) == -1){
		err(15, "Could not duplicate file descriptor");
	}
	if(execlp("sort", "sort", "-n", NULL) == -1){
		err(16, "Could not exec command");
	}

	wait(NULL);
	exit(0);
}	

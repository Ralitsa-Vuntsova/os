//cat /etc/passwd | sort | uniq

/*
pipeA
fork()
	in child
		close A[0]
		redirect stdout to A[1]
		exec cat
	in parent 
		close A[1]
pipeB
fork()
	in child
		close B[0]
		redirect stdin to A[0] and stdout to B[1]
		exec sort
	in parent 
		close B[1] and A[0]
		redirect stdin to B[0]
		exec uniq
*/

#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    int a[2];
    if(pipe(a) == -1) {
        err(1, "Could not create the first pipe");
    }
    pid_t p1 = fork();
    if(p1 < 0) {
        err(1,"couldn't fork");
    }
    if(p1 == 0) {
        if(dup2(a[1], 1) == -1) {
            err(2, "Could not fix up stdout for cat");
        }
        close(a[0]);
        if(execlp("cat", "cat", "/etc/passwd", (char*) 0) < 0) {
            err(2, "Failed exec for cat");
        }
    }
    close(a[1]);

    int b[2];
    if(pipe(b) == -1) {
        err(3, "Could not create the second pipe");
    }
    pid_t p2 = fork();
    if(p2 < 0) {
        err(3, "coldn't fork");
    }
    if(p2 == 0) {
        if(dup2(a[0], 0) < 0) {
            err(2, "Could not fix up stdin for sort");
        }
        if(dup2(b[1], 1) < 0) {
            err(2, "Could not fix up stdout for sort");
        }
        close(b[0]);
        if(execlp("sort", "sort", (char*)0) < 0) {
            err(4, "Failed exec for sort");
        }
    }

    close(a[0]);
    close(b[1]);
    if(dup2(b[0], 0) == -1) {
        err(2, "Could not fix up stdin for uniq");
    }
    if(execlp("uniq", "uniq", (char*)0) < 0) {
        err(5, "Failed exec for uniq");
    }

    int status;
    if(waitpid(p1, &status, 0) == -1) {
        err(6, "failed wait");
    }
    if(!WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        errx(10, "command failed");
    }
    if(waitpid(p2, &status, 0) == -1) {
        err(6, "failed wait");
    }
    if(!WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        errx(10, "command failed");
    }

    exit(0);
}

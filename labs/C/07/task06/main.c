#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

// indent(1) == '\t'
// indent(2) == "\t\t"
// indent(3) == "\t\t\t"
char * indent(int tabs) {
    char * indentation = (char *) malloc(tabs + 1);

    for (int i = 0; i < tabs; ++i) {
        indentation[i] = '\t';
    }

    indentation[tabs] = '\0';
    return indentation;
}

const int N = 4;

int main() {
    for (int i = 0; i < N; ++i) {
        int pid = fork();
        char * indentation = indent(i);

        if (pid > 0) {
		// parent process
          	printf("%s%d: parent with PID %d\n", indentation, i, getpid()); 
            wait(NULL); // uncomment to order the tree
           	free(indentation);
        } else if (pid == 0) {
		// child process
           	printf("%s%d: child with PID %d\n", indentation, i, getpid());
          	free(indentation);
           	exit(0); // uncomment to create a vine
        }
    }
}

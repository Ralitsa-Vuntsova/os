#include <unistd.h>
#include <sys/types.h>
#include <err.h>

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
cat  =====  grep  =====  sort

pipe(a)
fork()
	in child 1
		close unused file descriptors - a[0]
		redirect stdout to a[1]
		exec(cat)
	in parent
		close(a[1])
pipe(b)
fork()
	in child 2
		close unused file descriptors - b[0]
		redirect stdin to a[0]
		redirect stdout to b[1]
		exec(grep)
	in parent
		close unused file descriptors - a[0], b[1]
		redirect stdin to b[0]
		exec(sort)
*/

int main()
{
	int a[2];
	if (pipe(a) < 0)
	{
		err(1, "Cannot make a pipe");
	}

	const pid_t p = fork();
	if (p < 0)
	{
		err(2, "Cannot fork");
	}

	if (p == 0)
	{
		// first child execs cat and writes in pipe a
		close(a[0]); // close the read end of pipe a
		if (dup2(a[1], 1) < 0) // redirect stdout to pipe a's write end
		{
			err(3, "Cannot make a duplicate");
		}

		if (execlp("cat", "cat", "/etc/passwd", (char*)NULL) < 0)
		{
			err(4, "Cannot execute a command cat");
		}
	}
	close(a[1]); // nobody will ever write in pipe a's write end from now on

	int b[2];
	if (pipe(b) < 0)
	{
		err(5, "Cannot create a pipe");
	}

	const pid_t m = fork();
	if (m < 0)
	{
		err(6, "Cannot fork");
	}

	if (m == 0)
	{
		// child 2
		close(b[0]); // close the read end of b
		if (dup2(a[0], 0) < 0) // redirect pipe a's read end to be stdin
		{
			err(7, "Cannot make a duplicate");
		}

		if (dup2(b[1], 1) < 0) // redirect pipe b's write end to be stdout
		{
			err(8, "Cannot make a duplicate");
		}

		if (execlp("grep", "grep", "42", (char*)NULL) < 0)
		{
			err(9, "Cannot execute command grep");
		}
	}

	// parent
	close(a[0]); // close pipe a, we don't use it at all for sort
	close(b[1]); // close pipe b's write end
	if (dup2(b[0], 0) < 0)
	{
		err(10, "Cannot make a duplicate");
	}

	if (execlp("sort", "sort", "-r", (char*)NULL) < 0)
	{
		err(11, "Cannot execute command sort");
	}

	wait(NULL);
	exit(0);
}

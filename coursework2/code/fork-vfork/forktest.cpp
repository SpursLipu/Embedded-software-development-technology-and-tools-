#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
int main(void)
{
	pid_t pid,cpid;
	int status,bound=100;
	if((pid=fork())==-1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}else if(pid==0){
		puts("This is child process.");
		bound+=200;
		printf("pid=%d,ppid=%d\n",getpid(),getppid());
		printf("bound=%d\n\n",bound);
		exit(9);
	}else{
		 puts("This is parent process.");
                bound+=800;
		printf("pid=%d,ppid=%d\n",getpid(),getppid());
		printf("bound=%d\n\n",bound);
		cpid=wait(&status);
		printf("Child process has finished,cpid=%d\n",cpid);
		if(WIFEXITED(status))
		printf("child process exit code=%d\n",WEXITSTATUS(status));
		else if(WIFSIGNALED(status))
		printf("child process signal code=%d\n",WTERMSIG(status));
		else if(WIFSTOPPED(status))
		printf("child process stop code=%d\n",WSTOPSIG(status));
		else puts("child process exit abnormally.\n");
		exit(EXIT_SUCCESS);
	}		
}

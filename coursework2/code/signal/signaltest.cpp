#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<wait.h>
void handler(int signum)
{
	if(signum==50)
	printf("处理进程的id是：%d\n",getpid());
	else
	printf("error!\n");
}
int main(void)
{
	pid_t pid,cpid;
	int flag,status;
	signal(50,handler);//信号处理
	if((pid=fork())==-1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}else if(pid==0){
		puts("This is child process.");
		printf("pid=%d,ppid=%d\n",getpid(),getppid());
		pause();//进程挂起直至捕捉到信号	
		exit(9);
	}else{
		puts("This is parent process.");
		printf("pid=%d,ppid=%d\n",getpid(),getppid());
		sleep(1);
		flag=kill(pid,50);//向子进程发送信号
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

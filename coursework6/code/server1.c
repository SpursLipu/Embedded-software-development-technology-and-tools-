#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

// 进程回收函数
void recyle(int num)
{
    pid_t pid;
    while( (pid = waitpid(-1, NULL, WNOHANG)) > 0 )
    {
        printf("child died , pid = %d\n", pid);
    }
}

//文件系统数据读取函数
//buf：读取到的字符数据
//mode：读取模式，1为最新一条，2为历史数据
void get_data(char *buf,int mode){
	FILE *fp;
    fp = fopen("/mnt/ext3fs/data1.txt", "r");
    if(fp == NULL) {
        perror("打开文件时发生错误");
        return;
    }

    FILE *fp2;
    fp2 = fopen("/mnt/ext3fs/data2.txt", "r");
    if(fp2 == NULL) {
        perror("打开文件时发生错误");
        return;
    }

	if(mode==1){
	   //fgets (buf, 20, fp);
	   //buf[strlen(buf)-1]='\0';//去除换行
		while(!feof(fp))  
			{
				fgets(buf,sizeof(buf),fp); 
			}
	}	
	else if(mode ==2){
		int flag=0;
		int seq;
		char str[1024];
		char line[10][1024];
		while(!feof(fp))
		{
			fgets(str,sizeof(str),fp);
			if(!feof(fp))
			{
				flag++;
				seq=flag%10;
				if(seq==0)
					strcpy(line[9],str);
				else
				strcpy(line[seq-1],str);
			}
		}
        if(flag<=10)
        {
            for(int i=flag;i>0;i--)
            {
                //printf("%s\n",line[i-1]);
                strcat(buf,line[i-1]);
            }
        }
        else
        {
            seq=flag%10;
            for(int i=seq;i>0;i--)
            {
                //printf("%s\n",line[i-1]);
                strcat(buf,line[i-1]);
            }
            for(int i=10;i>seq;i--)
            {
                //printf("%s\n",line[i-1]);
                strcat(buf,line[i-1]);
            }

        }
		
	}
    else if(mode==3){
	   //fgets (buf, 20, fp);
	   //buf[strlen(buf)-1]='\0';//去除换行
		while(!feof(fp2))  
			{
				fgets(buf,sizeof(buf),fp2); 
			}
	}	
	else if(mode ==4){
		int flag=0;
		int seq;
		char str[1024];
		char line[10][1024];
		while(!feof(fp2))
		{
			fgets(str,sizeof(str),fp2);
			if(!feof(fp2))
			{
				flag++;
				seq=flag%10;
				if(seq==0)
					strcpy(line[9],str);
				else
				strcpy(line[seq-1],str);
			}
		}
        if(flag<=10)
        {
            for(int i=flag;i>0;i--)
            {
                //printf("%s\n",line[i-1]);
                strcat(buf,line[i-1]);
            }
        }
        else
        {
            seq=flag%10;
            for(int i=seq;i>0;i--)
            {
                //printf("%s\n",line[i-1]);
                strcat(buf,line[i-1]);
            }
            for(int i=10;i>seq;i--)
            {
                //printf("%s\n",line[i-1]);
                strcat(buf,line[i-1]);
            }

        }
		
	}
	fclose(fp);
    fclose(fp2);
	return;
}

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        printf("eg: ./a.out port\n");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr);
    int port = atoi(argv[1]);

    // 创建套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    // 初始化服务器 sockaddr_in 
    memset(&serv_addr, 0, serv_len);
    serv_addr.sin_family = AF_INET;                   // 地址族 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 监听本机所有的IP
    serv_addr.sin_port = htons(port);            // 设置端口 
    // 绑定IP和端口
    bind(lfd, (struct sockaddr*)&serv_addr, serv_len );

    // 设置同时监听的最大个数
    listen(lfd, 36);
    printf("Start accept ......\n");

    // 使用信号回收子进程pcb
    struct sigaction act;
    act.sa_handler = recyle;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);

    struct sockaddr_in client_addr;
    socklen_t cli_len = sizeof(client_addr);
    while(1)
    {
        // 父进程接收连接请求
        // accept阻塞的时候被信号中断, 处理信号对应的操作之后
        // 回来之后不阻塞了, 直接返回-1, 这时候 errno==EINTR
        int cfd = accept(lfd, (struct sockaddr*)&client_addr, &cli_len);
        while(cfd == -1 && errno == EINTR)
        {
            cfd = accept(lfd, (struct sockaddr*)&client_addr, &cli_len);
        }
        printf("connect sucessful\n");
        // 创建子进程
        pid_t pid = fork();
        if(pid == 0)
        {
            close(lfd);
            // child process
            // 通信
            char ip[64];
	    char signal_getlatest1[]="1latest\n";//获取最新记录指令，收到的字符串有\n
	    char signal_gethistory1[]="1history\n";//获取历史记录指令
        char signal_getlatest2[]="2latest\n";//获取最新记录指令，收到的字符串有\n
	    char signal_gethistory2[]="2history\n";//获取历史记录指令				
	    char rsp[]="Wrong Signal\n";//收到不正确指令的返回消息
            char rsp_len=strlen(rsp);
            while(1)
            {
                // client ip port
                printf("client IP: %s, port: %d\n", 
                       inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
                       ntohs(client_addr.sin_port));
                char buf[1024];
                int len = read(cfd, buf, sizeof(buf));
                if(len == -1)
                {
                    perror("read error");
                    exit(1);
                }
                else if(len == 0)
                {
                    printf("客户端断开了连接\n");
                    close(cfd);
                    break;
                }
                else
                {
                    printf("recv buf: %s", buf);
                    if(strcmp(buf,signal_getlatest1)==0)
                    {
                        char data[1024];
                        int data_len;
                        get_data(data,1);
                        data_len=strlen(data);
                        write(cfd, data, data_len);
                        printf("latest data1 read successfully! \n");
                    }
                    else if(strcmp(buf,signal_gethistory1)==0){
                        char data[1024];
                        int data_len;
                        get_data(data,2);
                        data_len=strlen(data);
                        write(cfd, data, data_len);
                        printf("history data1 read successfully! \n");
                    }
                    else if(strcmp(buf,signal_getlatest2)==0){
                        char data[1024];
                        int data_len;
                        get_data(data,3);
                        data_len=strlen(data);
                        write(cfd, data, data_len);
                        printf("latest data2 read successfully! \n");
                    }
                    else if(strcmp(buf,signal_gethistory2)==0){
                        char data[1024];
                        int data_len;
                        get_data(data,4);
                        data_len=strlen(data);
                        write(cfd, data, data_len);
                        printf("history data2 read successfully! \n");
                    }
                    else
                        write(cfd,rsp,rsp_len);
              }
            }
            // 干掉子进程
            return 0;

        }
        else if(pid > 0)
        {
            // parent process
            close(cfd);
        }
    }

    close(lfd);
    return 0;
}



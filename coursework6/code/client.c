#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERR_EXIT(msg)\
    do{perror(msg);}while(1)

int main(void)
{
    /**
     *the client implement
     */
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd < 0)  
    {   
        ERR_EXIT('create socket failed!');    
    }   
    /**
     * connect server
     */
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port =htons(5555);
    clientaddr.sin_addr.s_addr = inet_addr("192.168.0.109");//改成服务器IP地址
    if (connect(clientfd,(struct sockaddr*)&clientaddr, sizeof(struct sockaddr)) < 0)
    {   
        ERR_EXIT('failed to connect to server!');
    }   
    char rsp[]="Wrong Signal\n";//收到不正确的指令的返回消息
    char rsp_len=strlen(rsp);
    while(1)
    {   
        char bufw[255]={'\0'};
        while(fgets(bufw, sizeof(bufw), stdin))
        {   
            write(clientfd, bufw, sizeof(bufw));
            char bufr[255]={'\0'};
            int r = read(clientfd, bufr, sizeof(bufr));
	    if(strcmp(bufr,rsp)!=0)
	    {
		printf("Get data successfully:\n");            	
		fputs(bufr,stdout);
 	    }
	    else
            {
		fputs(bufr,stdout);
            }
        }
    }
}


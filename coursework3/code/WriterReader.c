#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define W_sleep 1 //写频率
#define R_sleep 1 //读频率

int ReaderCount=0;//读者计数器
int WriterCount=0;//写者计数器
int SharedData=0;//读者-写着共享数据
pthread_mutex_t mutex_write;//写者互斥锁
pthread_mutex_t mutex_read;//读者互斥锁
pthread_mutex_t mutex_write_count;//写者互斥锁
pthread_mutex_t mutex_read_count;//读者互斥锁
sem_t sem_readercount;//信号量控制只有一个线程可以对读者计数器操作
sem_t sem_writercount;//信号量控制只有一个线程可以对写者计数器操作
//写操作函数
void Write()
{
	int temp=rand();
	printf("写入数据 %d\n",temp);
	SharedData=temp;
}
//读操作函数
void Read()
{
	printf("读到数据 %d\n",SharedData);
}
//读者优先时的写者控制函数
void *writerOfRF(void* in)
{
	while(1)
	{
		pthread_mutex_lock(&mutex_write);
		WriterCount++;
		printf("写线程id %d 进入程序\n",pthread_self());
		printf("当前等待读者数为%d，当前等待写者数为%d\n",ReaderCount,WriterCount);
		Write();
		printf("写线程id %d 退出程序\n",pthread_self());
		WriterCount--;
		pthread_mutex_unlock(&mutex_write);
		sleep(W_sleep);
	}
	pthread_exit((void*)0);
}
//读者优先时的读者控制函数
void *readerOfRF(void* in)
{
	while(1)
	{
		sem_wait(&sem_readercount);
		ReaderCount++;
		printf("读线程id %d 进入程序\n",pthread_self());
		printf("当前等待读者数为%d，当前等待写者数为%d\n",ReaderCount,WriterCount);
		if(ReaderCount == 1)
			pthread_mutex_lock(&mutex_write);
		sem_post(&sem_readercount);

		Read();

		sem_wait(&sem_readercount);
		ReaderCount--;
		printf("读线程id %d 退出程序\n",pthread_self());
		if(ReaderCount == 0)
			pthread_mutex_unlock(&mutex_write);
		sem_post(&sem_readercount);
		sleep(R_sleep);
	}
	pthread_exit((void*)0);
}
//写者优先时的写者控制函数
void *writerOfWF(void* in)
{
	while(1)
	{
		WriterCount++;
		printf("写线程id %d 进入程序\n",pthread_self());
		printf("当前等待读者数为%d，当前等待写者数为%d\n",ReaderCount,WriterCount);

		pthread_mutex_lock(&mutex_write);
		Write();
		pthread_mutex_unlock(&mutex_write);
		
		WriterCount--;
		if(WriterCount == 0)
			pthread_mutex_lock(&mutex_read);
		
		printf("写线程id %d 退出程序\n",pthread_self());
		sleep(W_sleep);
	}
	pthread_exit((void*)0);
}
//写者优先时的读者控制函数
void *readerOfWF(void* in)
{
	while(1)
	{
		sem_wait(&sem_readercount);
		ReaderCount++;
		printf("读线程id %d 进入程序\n",pthread_self());
		printf("当前等待读者数为%d，当前等待写者数为%d\n",ReaderCount,WriterCount);
		sem_post(&sem_readercount);

		pthread_mutex_unlock(&mutex_read);
		Read();

		sem_wait(&sem_readercount);
		ReaderCount--;
		printf("读线程id %d 退出程序\n",pthread_self());
		sem_post(&sem_readercount);
		sleep(R_sleep);
	}
	pthread_exit((void*)0);
}
int main()
{
	pthread_mutex_init(&mutex_write,NULL);
	sem_init(&sem_readercount,0,1);
	sem_init(&sem_writercount,0,1);
	int readernum,writernum;
	printf("请分别输入读者人数和写者人数：");
	scanf("%d%d",&readernum,&writernum);
	int choice;
	printf("读者优先模式输入0，写者优先模式输入1:");
	scanf("%d",&choice);
	pthread_t wid[writernum],rid[readernum];
	int i=0;
	if(choice==0)
	{
		for(i=0;i<writernum;i++)
			pthread_create(&wid[i],NULL,writerOfRF,NULL);
		for(i=0;i<readernum;i++)
			pthread_create(&rid[i],NULL,readerOfRF,NULL);
		sleep(20);
	}
	else if(choice==1)
	{
		for(i=0;i<writernum;i++)
			pthread_create(&wid[i],NULL,writerOfWF,NULL);
		sleep(1);
		for(i=0;i<readernum;i++)
			pthread_create(&rid[i],NULL,readerOfWF,NULL);
		sleep(20);
	}else{printf("没有这个选项！\n");}
	return 0;
}


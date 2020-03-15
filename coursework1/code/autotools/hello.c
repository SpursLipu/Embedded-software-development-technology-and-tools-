#include<stdio.h>
int sum(int a,int b)
{
	        return a+b;
}

int main()
{
	char name[20];
	int a=10 , b=20;
	printf("hello,world!\n");
	printf("please enter your name!");
	scanf("%s",name);
	printf("%s\n",name);
	a=sum(a,b);
	printf("%d\n",a);
}

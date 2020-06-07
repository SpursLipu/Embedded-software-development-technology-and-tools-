#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>

int main(){
int fd1, fd2, retval;
char W_buffer1[26];
char W_buffer2[26];

//产生随机数写入scull
for(int i=0;i<26;i++){
    W_buffer1[i]=rand() % 10+'0';
    W_buffer2[i]=rand() % 10+'0';
}
    puts(W_buffer1);
    puts(W_buffer2);

fd1=open("/dev/myscull1", O_RDWR);
fd2=open("/dev/myscull2", O_RDWR);

retval = ioctl(fd1, 2, 0);
retval = ioctl(fd2, 2, 0);
//printf("io control %d\n", retval);
retval = write(fd1, W_buffer1, 26);
printf("write %d\n", retval);
retval = write(fd2, W_buffer2, 26);
printf("write %d\n", retval);

retval = lseek(fd1, 0, 0);
retval = lseek(fd2, 0, 0);

FILE *fp1;
fp1 = fopen("/mnt/ext3fs/data1.txt", "r");
FILE *fp2;
fp2 = fopen("/mnt/ext3fs/data2.txt", "r");

//一个一个数字读取scull，并写入文件系统
for(int j=0;j<26;j++){
    char R_buffer1[26];
    char R_buffer2[26];

    retval = read(fd1, R_buffer1, 1);
    retval = read(fd2, R_buffer2, 1);
    puts("scull1:");
    puts(R_buffer1);
    puts("scull2:");
    puts(R_buffer2);

    fwrite(R_buffer1,strlen(R_buffer1),1,fp1);
    fwrite(R_buffer2,strlen(R_buffer2),1,fp2);

}


//printf("read %d\n", retval);
close(fd1);
close(fd2);
fclose(fp1);
fclose(fp2);
return 0;
}
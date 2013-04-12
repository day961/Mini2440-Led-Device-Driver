#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(int *argc,char *argv[])
{
	int fd,ret,cmd,arg;
	int val[4]={0,0,0,0},val1=0,n;
	char ch[3];

	fd = open("/dev/ledtest",O_RDWR);
	if(fd<0)
		printf("open error\n");
	else
		printf("open success\n");

	cmd = atoi(argv[1]);
	arg = atoi(argv[2]);

/*	if(ioctl(fd,cmd,arg)<0)
	{
		printf("ioctl fail\n");
	}
*/
//	scanf("%s",ch);
	ch[0]=*argv[1];
	ch[1]=*argv[2];

	printf("apptest %d,%d\n",ch[0],ch[1]);
	
	ret = write(fd,ch,2);
	if(ret < 0)
	{
		printf("error\n");
	}
	ret = read(fd,val,sizeof(val));

	sleep(1);
	printf("Read val = ");
	for(n=0;n<4;n++)
		printf("%2d",val[n]);
	printf("\n");

	close(fd);
	return 0;
}

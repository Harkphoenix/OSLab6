#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/wait.h>
#define max_size 8
typedef struct
{
        long mtype;   // 消息类型，必须为正数
        char mtext[5]; // 发出的消息的数据
} msgbufs;
const key_t key = 75; //建立键为75的消息队列
void server()
{
	int shmflag = IPC_CREAT | 0600;
        int shmid = shmget(key, max_size, shmflag);
	if(shmid == -1)
		puts("创建共享存储消息队列失败");
	char* virtaddr = shmat(shmid, 0, 0);
	if(virtaddr == (void *)(-1))
		puts("映射失败");
	do
	{
		virtaddr[0] = (char)(-1);
		while(virtaddr[0] == (char)(-1)) {}
		puts("server recive data");
	}while(virtaddr[0]!=0);
	int shmend = shmdt(virtaddr);
	if(shmend == -1) puts("server切断映射失败");
	else puts("server切断映射成功");
}
void client()
{
	int i;
	int shflag = IPC_CREAT | 0600;
        int shid = shmget(key, max_size, shflag);
        if(shid == -1)
                puts("创建共享存储消息队列失败");
        char* addr = shmat(shid, 0, 0);
	if(addr == (void *)(-1))
                puts("映射失败");
	for(i = 9; i >= 0; i--)
	{
		while(addr[0] != (char)(-1)){}
		addr[0] = i;
		puts("client send data success");
	}
	int shend = shmdt(addr);
	if(shend == -1) puts("client切断映射失败");
	else puts("client切断映射成功");

}
int main()
{
        int sign;
        while((sign = fork()) == -1);
        if(!sign) server();
        while((sign = fork()) == -1);
        if(!sign) client();
	wait(0);
	wait(0);
        return 0;
}


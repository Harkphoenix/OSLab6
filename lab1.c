#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
typedef struct
{
	long mtype;   // 消息类型，必须为正数
	char mtext[5]; // 发出的消息的数据
} msgbufs;
const key_t key = 75; //建立键为75的消息队列
void server()
{
	msgbufs msgq;
	int msgetflag = IPC_CREAT | 0600;     //如果键值为指定值的消息队列不存在，则创建新的消息队列,并且当前用户具有读写权限
	int msgid = msgget(key, msgetflag);
	if(msgid == -1)
		printf("消息队列创建失败\n");
	do{
		int count = msgrcv(msgid, &msgq, 5, 0, MSG_NOERROR);
		if(count == -1) printf("server recive data error\n");
		else puts("server recive data is success");
	}while(msgq.mtype != 1);
}
void client()
{
	int i;
	msgbufs msgc;
	int msflag = IPC_CREAT | 0600;
	int msid =  msgget(key, msflag);
	for(i = 10; i > 0; i--)
	{
		msgc.mtype = i;
		int mssign = msgsnd(msid, &msgc, 5, 0);
		if(mssign == -1) puts("client send data error");
		else puts("client send data successful");
	}
}

int main()
{
	int sign;
	while((sign = fork()) == -1);
	if(!sign) server();
	while((sign = fork()) == -1);
	if(!sign) client();
	return 0;
}

#ifndef __Protocol_H__  //防止头文件被多次包含而重定义
#define __Protocol_H__

#define PKT_LOGIN 0
#define PKT_LOGOUT 1
#define PKT_PUBLIC_TLK 2
#define PKT_PRIVATE_TLK 3
#define PKT_LOGIN_REPLY 4

#define Fail 99
#define Success 100
#define Error 101

//数据包包头
typedef struct PktHeader
{
	int nPktType;
}PktHeader;

typedef struct LoginPkt
{
	PktHeader header;
	int imgNum;
	char userName[20];
	char password[20];
	
}LoginPkt;

typedef struct LogoutPkt
{
	PktHeader header;
	char userName[20];
};
typedef struct LoginReplyPkt
{
	PktHeader header;
	int onlineUsers;
	int rpy;
	char welcome[512];
}LoginReplyPkt;

typedef struct PublicMsgPkt
{
	PktHeader header;
	int imgNum;
	char fromName[20];
	char message[512];
}PublicMsgPkt;

typedef struct PrivateMsgPkt
{
	PktHeader header;
	char fromName[20];
	char toName[20];
	char message[512];
}PrivateMsgPkt;

//用于遍历用户注册文件
struct user_info{
	char userName[20];
	char password[20];
	char phoneNumber[50];

};

typedef struct userItem{
	char userName[20];
	struct sockaddr addr;
	int msg_Num;
	int imgNum;
}userItem;

typedef struct add_Del
{
	int flag;  //flag=0x100：添加    flag=0x101：删除
	char userName[20];
}add_Del;


#endif
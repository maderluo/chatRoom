#ifndef __Protocol_H__  //��ֹͷ�ļ�����ΰ������ض���
#define __Protocol_H__

#define PKT_LOGIN 0
#define PKT_LOGOUT 1
#define PKT_PUBLIC_TLK 2
#define PKT_PRIVATE_TLK 3
#define PKT_LOGIN_REPLY 4

//���ݰ���ͷ
typedef struct PktHeader
{
	int nPktType;
}PktHeader;

typedef struct userItem
{
	char userName[20];
	struct sockaddr addr;
	int msg_Num;
	int imgNum;
};

typedef struct LoginPkt
{
	PktHeader header;
	int imageNum;//����ѡ��ͷ��
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

#endif
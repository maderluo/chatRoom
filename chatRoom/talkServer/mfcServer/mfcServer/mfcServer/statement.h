#ifndef _STATEMENT_H__
#define _STATEMENT_H__

int checkPassword(char *fileName, LoginPkt pkt);
void dealPkt(const char* revBuf, SOCKET s, struct sockaddr *fromAddr);
void broadcastMsg(SOCKET s, PublicMsgPkt *pkt);
void addUser(char *userName, sockaddr *addr,int imgNum);
void deleteUser(char *userName);
void privateChat(SOCKET s, PrivateMsgPkt *pkt);

#endif
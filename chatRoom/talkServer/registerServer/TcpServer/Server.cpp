#pragma comment(lib,"ws2_32.lib")
#include<stdio.h>
#include<WinSock2.h>

#define Fail -1
#define Success 1
#define Error 0
#define size 1000

struct user_info{
	char UserName[20];
	char Password[20];
	char PhoneNumber[50];
};


int WriteIntoFile(char *FileName, struct user_info* uW);

int main(int argc, char*argv[]){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);

	int port = 5050;
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sListen, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	listen(sListen, 5);
	printf("��ʼ��������\n");

	while (true)
	{
		SOCKET sNewConnection;
		SOCKADDR_IN clientAddr;
		memset(&clientAddr, 0, sizeof(clientAddr));
			int clientAddrLen = sizeof(clientAddr);
		sNewConnection = accept(sListen, (SOCKADDR*)&clientAddr, &clientAddrLen);
		printf("Connect from %s\n", inet_ntoa(clientAddr.sin_addr));
	

			//�������ݣ�
		for (int i = 0; i < 10; i++){
			int SorF;                 //�Ƿ�ɹ�ע��ı�־ Success or Fail
			struct user_info u;
			u.UserName[1024]='\0';
			u.Password[1024]='\0';
			u.PhoneNumber[1024]='\0';
			char message2[1024] = { 0 };
			int recSize = recv(sNewConnection, message2, sizeof(message2), 0);
			sscanf(message2, "%[^,],%[^,],%s\n", u.UserName, u.Password, u.PhoneNumber);
			if (strcmp(message2, "exit") == 0) break;

			printf("�û���:%s\n", u.UserName);
			printf("����:%s\n", u.Password);
			printf("�ֻ���:%s\n", u.PhoneNumber);

			if (strcmp(u.UserName, "") == 0 || strcmp(u.Password, "") == 0 || strcmp(u.PhoneNumber, "") == 0)
			{
				char message1[] = "��ʽ���벻��ȷ������������";
				send(sNewConnection, message1, sizeof(message1), 0);
				continue;
			}

			int flag = 1;
			for (int i = 0; i < strlen(u.Password); i++)
			{
				if (u.PhoneNumber[i] >= '0'&&u.PhoneNumber[i] <= '9')
					continue;
				else{
					flag = 0;
					char message1[] = "��ʽ���벻��ȷ������������";
					send(sNewConnection, message1, sizeof(message1), 0);
					break;
				}
			}
			if (flag == 0) continue;

			SorF = WriteIntoFile("user_info.txt",&u); //����Ϣд���ļ�

			if (SorF == Success/*δ����*/)
			{
				char message2[] = "�û�ע��ɹ�";
				send(sNewConnection, message2, sizeof(message2), 0);
				break;
			}
			else if (SorF == Fail)
			{
				char message2[] = "�û����Ѵ���,���������룺";
				send(sNewConnection, message2, sizeof(message2), 0);
				continue;
			}
			else
			{
				printf("����˳������⣬�û��˻���Ϣ�ļ���ʧ��\n");
				char message2[] = "����˳��ֹ��ϣ�ע��ʧ�ܣ���ȴ�ά��\n";
				send(sNewConnection, message2, sizeof(message2), 0);
				break;
			}

		}
		closesocket(sNewConnection);
	}
	closesocket(sListen);
	WSACleanup();
	return 0;

}

//д��ע����Ϣ������

int WriteIntoFile(char *FileName,struct user_info *uW){
	FILE *fp;
	struct user_info uR[size];
	int info_num = 0;
	
	if ((fp = fopen(FileName, "r+")) == NULL)
		return Error;

	while (!feof(fp)){
		char line[1024];
		if (fgets(line, 1024, fp) == NULL)
			break;
		sscanf(line, "%[^,],%[^,],%s\n", uR[info_num].UserName,uR[info_num].Password,uR[info_num].PhoneNumber);
		if (strcmp(uW->UserName, uR[info_num].UserName) == 0)
			return Fail;
		info_num++;
	}
	fclose(fp);
	fp=fopen(FileName,"a");
	fprintf(fp, "%s,%s,%s\n", uW->UserName, uW->Password, uW->PhoneNumber);
	fclose(fp);
	return Success;
}
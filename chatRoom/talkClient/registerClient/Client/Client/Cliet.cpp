#include<stdio.h>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char*argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons((u_short)atoi(argv[2]));
	connect(sClient, (SOCKADDR*)&serverAddr, sizeof(serverAddr));


	//�������ݣ�
	while (true){
		int x;
		char Register[1024];
		printf("           ***********************************************************\n");
		printf("                                                                      \n");
		printf("                         Welcome to my Register    ^_^                  \n");
		printf("\n");
		printf("                         Made by ��־��  13055824                       \n");
		printf("                                                                      \n");
		printf("           ***********************************************************\n");
		printf("\n");
		printf("\n");
		printf("         |                             �����룺                        |\n");
		printf("         |                             1.ע��;                         |\n");
		printf("         |                             2.�˳�.                         |\n");

		scanf("%d", &x);

		char message[1024] = { 0 };
		int flag = 0;
		switch(x)
		{
		case 1 :
				{
		          printf("�������û��������룬�ֻ���(����Ӣ�Ķ��š�,����������\n");
		          scanf("%s",Register);
		          send(sClient, Register, strlen(Register), 0);
	  //�������ݣ�
		          int recSize = recv(sClient, message, 1024, 0);
		          printf("%s\n", message);
				  if (strcmp(message, "��ʽ���벻��ȷ������������") == 0) break;
			    };

		case 2:
				flag = 1;
				send(sClient, "exit", strlen("exit"), 0);
				break;
			default:
				printf("�����������������\n");
		}
		if (flag == 1) goto lexit;
	}

lexit:
	closesocket(sClient);
	WSACleanup();
	getchar();
	system("pause");
	return 0;
}

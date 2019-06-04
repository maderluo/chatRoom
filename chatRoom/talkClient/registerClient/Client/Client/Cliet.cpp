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


	//发送数据：
	while (true){
		int x;
		char Register[1024];
		printf("           ***********************************************************\n");
		printf("                                                                      \n");
		printf("                         Welcome to my Register    ^_^                  \n");
		printf("\n");
		printf("                         Made by maderluo                       \n");
		printf("                                                                      \n");
		printf("           ***********************************************************\n");
		printf("\n");
		printf("\n");
		printf("         |                             请输入：                        |\n");
		printf("         |                             1.注册;                         |\n");
		printf("         |                             2.退出.                         |\n");

		scanf("%d", &x);

		char message[1024] = { 0 };
		int flag = 0;
		switch(x)
		{
		case 1 :
				{
		          printf("请输入用户名，密码，手机号(请用英文逗号“,”隔开）：\n");
		          scanf("%s",Register);
		          send(sClient, Register, strlen(Register), 0);
	  //接受数据：
		          int recSize = recv(sClient, message, 1024, 0);
		          printf("%s\n", message);
				  if (strcmp(message, "格式输入不正确，请重新输入") == 0) break;
			    };

		case 2:
				flag = 1;
				send(sClient, "exit", strlen("exit"), 0);
				break;
			default:
				printf("输入错误，请重新输入\n");
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

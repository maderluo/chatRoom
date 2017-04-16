
// mfcServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfcServer.h"
#include "mfcServerDlg.h"
#include "afxdialogex.h"
#include "statement.h"
#include <tchar.h>
#include <string.h>
#pragma comment(lib,"Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//���ڴ򿪹ر�ע�����:
STARTUPINFO si = { sizeof si };
PROCESS_INFORMATION pi = { 0 };


#define size 1000   //���ڱ���ע���ļ�

userItem g_UserList[1000];
add_Del _add_Del;
int g_UserNum = 0;


bool g_Stop;

DWORD WINAPI RecvThread(LPVOID lpParam)
{
	CmfcServerDlg* pDlg = (CmfcServerDlg*)lpParam;
	if (pDlg == NULL)
		return -1;
	do
	{
		struct sockaddr_in fromAddr;
		memset(&fromAddr, 0, sizeof(fromAddr));
		int fromLen = sizeof(fromAddr);

		char revBuf[1024] = { 0 };
		int nRet = recvfrom(pDlg->Server, revBuf, sizeof(revBuf), 0, (struct sockaddr*)&fromAddr, &fromLen); //�ӿͻ��˽��յ�¼�˳���Ϣ��������Ϣ
		dealPkt(revBuf, pDlg->Server, (struct sockaddr*)&fromAddr);
		switch (_add_Del.flag)
		{
		case 1:
		{
				  for (int i = 0; i < g_UserNum + 1; i++)
				  {
					  CString str;
					  str = pDlg->m_list.GetItemText(i, 0);
					  if (strcmp(str, _add_Del.userName) == 0)
					  {
						  pDlg->m_list.DeleteItem(i);
						  break;
					  }
				  }
				  char imfoText[1024];
				  strcpy(imfoText, "ϵͳ��Ϣ:");
				  strcat(imfoText, _add_Del.userName);
				  strcat(imfoText, "�˳������ң�");
				  strcat(imfoText, "\r\n");
				  pDlg->m_message.SetSel(0, 0);
				  pDlg->m_message.ReplaceSel(imfoText);
				  _add_Del.flag = 0;
		}; break;
		case 2:
		{
			int count;
			count = pDlg->m_list.GetItemCount();
			pDlg->m_list.InsertItem(count, "");
			pDlg->m_list.SetItemText(count, 0, (LPCTSTR)g_UserList[g_UserNum - 1].userName);
			pDlg->m_list.SetItemText(count, 1, (LPCTSTR)(inet_ntoa(((struct sockaddr_in*)&g_UserList[g_UserNum - 1].addr)->sin_addr)));
			pDlg->m_list.SetItemText(count, 2, "0");
			char imfoText[1024];
			strcpy(imfoText, "ϵͳ��Ϣ:");
			strcat(imfoText, _add_Del.userName);
			strcat(imfoText, "���������ң�");
			strcat(imfoText, "\r\n");
			pDlg->m_message.SetSel(0, 0);
			pDlg->m_message.ReplaceSel(imfoText);
			_add_Del.flag = 0;
		}; break;
		case 3:
		{
			for (int i = 0; i < g_UserNum; i++)
			{
				CString str;
				str = pDlg->m_list.GetItemText(i, 0);
				if (strcmp(str, _add_Del.userName) == 0)
				{
					char str[20];
					itoa(++g_UserList[i].msg_Num, str, 10);
					pDlg->m_list.SetItemText(i, 2, (LPCTSTR)str);

					break;
				}
			}
			_add_Del.flag = 0;
		}; break;
		default:break;
		}
	} while (g_Stop);
	return 0;
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmfcServerDlg �Ի���



CmfcServerDlg::CmfcServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmfcServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_userList, m_list);
	DDX_Control(pDX, IDC_openTalk, m_login);
	DDX_Control(pDX, IDC_openRegister, m_rlogin);
	DDX_Control(pDX, IDC_shutTalk, m_logout);
	DDX_Control(pDX, IDC_shutRegister, m_rlogout);
	DDX_Control(pDX, IDC_message, m_message);
}

BEGIN_MESSAGE_MAP(CmfcServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_shut, &CmfcServerDlg::Onshut)
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP, &CmfcServerDlg::OnHelp)
	ON_COMMAND(ID_about, &CmfcServerDlg::Onabout)
	ON_BN_CLICKED(IDC_openTalk, &CmfcServerDlg::OnBnClickedopentalk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_userList, &CmfcServerDlg::OnLvnItemchangeduserlist)
	ON_BN_CLICKED(IDC_openRegister, &CmfcServerDlg::OnBnClickedopenregister)
	ON_BN_CLICKED(IDC_shutTalk, &CmfcServerDlg::OnBnClickedshuttalk)
	ON_BN_CLICKED(IDC_shutRegister, &CmfcServerDlg::OnBnClickedshutregister)
END_MESSAGE_MAP()


// CmfcServerDlg ��Ϣ�������

BOOL CmfcServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	//��ݼ����ã�
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_list.InsertColumn(0, "�û���", LVCFMT_CENTER, 215);
	m_list.InsertColumn(1, "�û���", LVCFMT_CENTER, 215);
	m_list.InsertColumn(2, "IP��ַ", LVCFMT_CENTER, 215);
	m_list.InsertColumn(3, "��Ϣ��", LVCFMT_CENTER, 215);
	m_list.DeleteColumn(0);  //������ɾ����һ�У���Ϊ��һ�е��ı���������ʾ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CmfcServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmfcServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmfcServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmfcServerDlg::Onshut()
{
	// TODO:  �ڴ���������������
	CmfcServerDlg::OnClose();
	exit(0);
}


void CmfcServerDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�������رշ�����ʱ�������Ͻǵ�X�رգ���
	if (sock_Close == false)
		closesocket(Server);

	CDialogEx::OnClose();
}


BOOL CmfcServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))
		return true;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CmfcServerDlg::OnHelp()
{
	// TODO:  �ڴ���������������
	MessageBox("��ӭʹ�ñ����");
}


void CmfcServerDlg::Onabout()
{
	// TODO:  �ڴ���������������
	MessageBox("��������ڷ������Ĺ���\n��������2����־������\n    ѧ��13055824");
}


void CmfcServerDlg::OnBnClickedopentalk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_Stop = TRUE;
	Server = socket(AF_INET, SOCK_DGRAM, 0);
	if (Server == INVALID_SOCKET)
	{
		printf("socket() error:%d\n", WSAGetLastError());
	}
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons((u_short)5150);

	if (bind(Server, (struct sockaddr*)&local, sizeof(local)) != 0)
	{
		printf("bind() error: %d\n", WSAGetLastError());
		closesocket(Server);
	}
	else
	{
		MessageBox("��������������ɹ�");
		GetDlgItem(IDC_openTalk)->EnableWindow(FALSE);
		GetDlgItem(IDC_shutTalk)->EnableWindow(TRUE);
		DWORD dwThreadID;
		HANDLE hRecvThread = CreateThread(NULL, 0,
			RecvThread, (LPVOID)this, 0, &dwThreadID);
		CloseHandle(hRecvThread);
		return;
	}
}



	int checkPassword(char *fileName, LoginPkt *pkt){
		FILE *fp;
		struct user_info uR[size];
		int info_num = 0;
		if ((fp = fopen(fileName, "r+")) == NULL)
			return Error;
		while (!feof(fp)){
			char line[1024];
			if (fgets(line, sizeof(line), fp) == NULL)
				break;
			sscanf(line, "%[^,],%[^,],%s\n", uR[info_num].userName, uR[info_num].password, uR[info_num].phoneNumber);
			if (strcmp(pkt->userName, uR[info_num].userName) == 0 && strcmp(pkt->password, uR[info_num].password) == 0)
				return Success;
			info_num++;
		}
		return Fail;
		fclose(fp);

	}

	void dealPkt(const char* revBuf, SOCKET s, struct sockaddr *fromAddr)
	{
		//�����ж���ʲô���͵İ�:
		PktHeader *header = (PktHeader*)revBuf;
		if (header->nPktType == PKT_LOGIN)
		{
			LoginPkt *pkt = (LoginPkt*)revBuf;

			//�û��ʺ�������֤�ɹ���
			if (checkPassword("user_info.txt", pkt) == Success)
			{
				//printf("%s������������..\n", pkt->userName);
				LoginReplyPkt reply;

					for (int i = 0; i < g_UserNum; i++)
					{
						if (strcmp(g_UserList[i].userName, pkt->userName) == 0)
						{
							LoginReplyPkt reply;
							reply.header.nPktType = PKT_LOGIN_REPLY;
							reply.rpy = 99;
							sendto(s, (char*)(&reply), 8, 0, fromAddr, sizeof(struct sockaddr));
							return;

						}
					}
						reply.header.nPktType = PKT_LOGIN_REPLY;  //���ذ�������
						reply.rpy = 0;  //0�ɹ����룬-1����ʧ��,101�û�ע���ļ���ʧ
						reply.onlineUsers = g_UserNum;
						char welcome[] = "��ӭ����������";
						strcpy(reply.welcome, welcome);
						addUser(pkt->userName, fromAddr,pkt->imgNum);
						sendto(s, (char*)(&reply),12+ strlen(reply.welcome) + 1, 0, fromAddr, sizeof(sockaddr));

						userItem userPkt = { 0 };
						for (int i = 0; i < g_UserNum-1; i++)
						{
							userPkt.imgNum = g_UserList[i].imgNum;
							strcpy(userPkt.userName, g_UserList[i].userName);
							sendto(s, (char*)(&userPkt), sizeof(userPkt), 0, fromAddr, sizeof(sockaddr));
						}

						PublicMsgPkt msgPkt;
						msgPkt.header.nPktType = PKT_LOGIN;
						msgPkt.imgNum = pkt->imgNum;
						strcpy(msgPkt.fromName, pkt->userName);
						strcpy(msgPkt.message, pkt->userName); 
						strcat(msgPkt.message, "����������!");
						//�㲥������Ϣ:
						broadcastMsg(s, &msgPkt);
					
				}
				
			//�ʺ�������֤ʧ�ܣ�
			else if (checkPassword("user_info.txt", pkt) == Fail)
			{
				LoginReplyPkt reply;
				reply.header.nPktType = PKT_LOGIN_REPLY;
				reply.rpy = -1;
				sendto(s, (char*)(&reply), 8, 0, fromAddr, sizeof(struct sockaddr));
				return;
			}
			//�û�ע���ļ���ʧ��
			else
			{
				LoginReplyPkt reply;
				reply.header.nPktType = PKT_LOGIN_REPLY;
				reply.rpy = 101;
				sendto(s, (char*)(&reply), 8, 0, fromAddr, sizeof(struct sockaddr));
				return;
			}

		}
		else if (header->nPktType == PKT_LOGOUT)
		{
			LogoutPkt *pkt = (LogoutPkt*)revBuf;
			printf("%s�뿪��������..\n", pkt->userName);

			PublicMsgPkt msgPkt;
			msgPkt.header.nPktType = PKT_LOGOUT;
			//strcpy(msgPkt.fromName, "ϵͳ��Ϣ");
			strcpy(msgPkt.fromName, pkt->userName);
			strcpy(msgPkt.message, pkt->userName);
			strcat(msgPkt.message, "�뿪�����ң�");
			broadcastMsg(s, &msgPkt);

			deleteUser(pkt->userName);
		}
		else if (header->nPktType == PKT_PUBLIC_TLK)
		{
			PublicMsgPkt *pkt = (PublicMsgPkt*)revBuf;
			strcpy(_add_Del.userName, pkt->fromName); //���ڼ�¼��Ϣ����
			broadcastMsg(s, pkt);
			_add_Del.flag = 3;
		}
		else if (header->nPktType == PKT_PRIVATE_TLK)
		{
			PrivateMsgPkt *pkt = (PrivateMsgPkt*)revBuf;
			strcpy(_add_Del.userName, pkt->fromName);
			privateChat(s, pkt);
			_add_Del.flag = 3;
		}
		else
		{
			printf("receive an unknow packet..\n");
		}
	}

	void addUser(char *userName, sockaddr *addr,int imgNum){
		strcpy(g_UserList[g_UserNum].userName, userName);
		strcpy(_add_Del.userName, userName);
		_add_Del.flag = 2;
		g_UserList[g_UserNum].addr = *addr;
		g_UserList[g_UserNum].imgNum = imgNum;
		g_UserList[g_UserNum].msg_Num = 0;
		g_UserNum++;
	}

	void broadcastMsg(SOCKET s, PublicMsgPkt *pkt)
	{
		for (int i = 0; i < g_UserNum; i++)
		{
			if (strcmp(g_UserList[i].userName, pkt->fromName)!= 0)
			
			sendto(s, (char*)pkt, sizeof(PublicMsgPkt), 0, &(g_UserList[i].addr), sizeof(g_UserList[i].addr));
		}
	}

	void privateChat(SOCKET s,PrivateMsgPkt *pkt)
	{
		for (int i = 0; i < g_UserNum; i++)
		{
			if (strcmp(g_UserList[i].userName, pkt->toName) == 0)
			{
				PublicMsgPkt msgPkt;
				msgPkt.header.nPktType = PKT_PUBLIC_TLK;
				strcpy(msgPkt.fromName, pkt->fromName);
				strcat(msgPkt.fromName, "����˵");
				strcpy(msgPkt.message, pkt->message);
				sendto(s, (char*)(&msgPkt), sizeof(msgPkt), 0, &(g_UserList[i].addr), sizeof(g_UserList[i].addr));
			}
		}
	}

	void deleteUser(char *userName)
	{
		strcpy(_add_Del.userName, userName);
		_add_Del.flag = 1;
		for (int i = 0; i < g_UserNum; i++)
		{
			if (strcmp(g_UserList[i].userName, userName) == 0)
			{
				g_UserNum--;
				while (i < g_UserNum)
				{
					g_UserList[i] = g_UserList[i + 1];//���ɾ���Ŀ�ȱ
					i++;
				}
			}
		}
		
	}


	void CmfcServerDlg::OnLvnItemchangeduserlist(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		*pResult = 0;
	}


	void CmfcServerDlg::OnBnClickedopenregister()
	{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		CreateProcess(NULL, _T("rServer.exe"), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		MessageBox("�ɹ�����ע�������");
		GetDlgItem(IDC_openRegister)->EnableWindow(FALSE);
		GetDlgItem(IDC_shutRegister)->EnableWindow(TRUE);
		return;
	}


	void CmfcServerDlg::OnBnClickedshuttalk()
	{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		m_logout.EnableWindow(FALSE);   //ֻ����һ��
		m_list.DeleteAllItems();
		GetDlgItem(IDC_message)->SetWindowText(_T(""));
		MessageBox("�ɹ��ر����������");

		g_Stop = false;
		Sleep(200);
		closesocket(Server);
		sock_Close = true;  //��closesocket�ı��

		m_login.EnableWindow(true);
		m_login.UpdateData(true);
	}


	void CmfcServerDlg::OnBnClickedshutregister()
	{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		GetDlgItem(IDC_shutRegister)->EnableWindow(FALSE); //ֻ����һ��
		TerminateProcess(pi.hProcess, 0);
		MessageBox("�ɹ��ر�ע�������");
		GetDlgItem(IDC_openRegister)->EnableWindow(TRUE);
		GetDlgItem(IDC_openRegister)->UpdateData(TRUE);
	}

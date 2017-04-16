
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "Protocol.h"
#include <tchar.h>
#include<stdlib.h>
#pragma comment(lib,"Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool g_Stop;
int curNum;
userItem userList[1000];
LVITEM lvitem;
LVFINDINFO info;

DWORD WINAPI RecvThread(LPVOID lpParam)
{
	CClientDlg* pDlg = (CClientDlg*)lpParam;
	if (pDlg == NULL)
		return -1;
	do
	{
		
		PublicMsgPkt pkt = { 0 };
		int nAddrLen = sizeof(pDlg->serverAddr);
		int nRet = recvfrom(pDlg->Client, (char*)&pkt, sizeof(pkt),
			0, (struct sockaddr*)&(pDlg->serverAddr), &nAddrLen);

		if (nRet < 0)
		{
			pDlg->m_hsyMsg.SetSel(0, 0);
			pDlg->m_hsyMsg.ReplaceSel("系统消息：socket出错\r\n");
		}
		else if (nRet == 0)
		{
			pDlg->m_hsyMsg.SetSel(0, 0);
			pDlg->m_hsyMsg.ReplaceSel("系统消息：服务器已关闭\r\n");
		}
		else
		{
			//有新用户加入时：
			if (pkt.header.nPktType ==PKT_LOGIN)
			{
				strcpy(userList[curNum].userName, pkt.fromName);
				lvitem.mask = LVIF_IMAGE | LVFIF_TEXT;
				lvitem.iItem = curNum;
				lvitem.pszText = pkt.fromName;
				lvitem.iImage = pkt.imgNum;
				pDlg->m_list.InsertItem(&lvitem);
				curNum++;
				strcpy(pkt.fromName, "系统消息");
			}
			else if (pkt.header.nPktType == PKT_LOGOUT)
			{
				info.flags = LVFI_PARTIAL | LVFI_STRING;
				info.psz = pkt.fromName;
				int item;
				item = pDlg->m_list.FindItem(&info);
				if (item != -1)
				{
					pDlg->m_list.DeleteItem(item);
				}
				strcpy(pkt.fromName, "系统消息");
			}
			char szOutText[1024];
			strcpy(szOutText, pkt.fromName);
			strcat(szOutText, ":");
			strcat(szOutText, pkt.message);
			strcat(szOutText, "\r\n");
			pDlg->m_hsyMsg.LineScroll(pDlg->m_hsyMsg.GetLineCount() - 1, 0);
			pDlg->m_hsyMsg.SetSel(0,0);
			pDlg->m_hsyMsg.ReplaceSel(szOutText);
		}

	} while (g_Stop);
	return 0;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void Onabout();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_about, &CAboutDlg::Onabout)
END_MESSAGE_MAP()


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
	, m_IP(_T("127.0.0.1"))
	, m_userName(_T("mader"))
	, m_port(5150)
	, m_password(_T("13055824"))
	, m_sMsg(_T(""))
	, m_check(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_svrIP, m_IP);
	DDX_Text(pDX, IDC_userName, m_userName);
	DDX_Text(pDX, IDC_svrPort, m_port);
	DDX_Text(pDX, IDC_password, m_password);
	DDX_Control(pDX, IDC_hsyMsg, m_hsyMsg);
	DDX_Text(pDX, IDC_sMsg, m_sMsg);
	DDX_Check(pDX, IDC_check, m_check);
	DDX_Control(pDX, IDC_register, m_register);
	DDX_Control(pDX, IDC_enter, m_login);
	DDX_Control(pDX, IDC_exit, m_logout);
	DDX_Control(pDX, IDC_icoList, m_list);
	DDX_Control(pDX, IDC_comboBox, m_comboBox);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_hsyMsg, &CClientDlg::OnEnChangehsymsg)
	ON_BN_CLICKED(IDC_enter, &CClientDlg::OnBnClickedenter)
	ON_BN_CLICKED(IDC_exit, &CClientDlg::OnBnClickedexit)
	ON_BN_CLICKED(IDC_check, &CClientDlg::OnBnClickedcheck)
	ON_BN_CLICKED(IDC_send, &CClientDlg::OnBnClickedsend)
	ON_EN_CHANGE(IDC_svrIP, &CClientDlg::OnEnChangesvrip)
	ON_EN_CHANGE(IDC_userName, &CClientDlg::OnEnChangeusername)
	ON_EN_CHANGE(IDC_svrPort, &CClientDlg::OnEnChangesvrport)
	ON_EN_CHANGE(IDC_password, &CClientDlg::OnEnChangepassword)
	ON_EN_CHANGE(IDC_sMsg, &CClientDlg::OnEnChangesmsg)
	ON_BN_CLICKED(IDC_register, &CClientDlg::OnBnClickedregister)
	ON_COMMAND(ID_shut, &CClientDlg::Onshut)
	ON_COMMAND(ID_HELP, &CClientDlg::OnHelp)
	ON_COMMAND(ID_about, &CClientDlg::Onabout)
	ON_BN_CLICKED(IDC_clearup, &CClientDlg::OnBnClickedclearup)
//	ON_WM_CLOSE()
ON_WM_CLOSE()
ON_NOTIFY(NM_DBLCLK, IDC_icoList, &CClientDlg::OnNMDblclkicolist)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	//初始化图像列表:
	m_imageList1.Create(16, 16, ILC_COLOR16, 6, 6);

	for (int i = 0; i < 6; i++)
	{
		m_imageList1.Add(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1 + i)));
	}

	//为combobox关联图像列表:
	m_comboBox.SetImageList(&m_imageList1);

	//将图像插入组合框中
	for (int i = 0; i < m_imageList1.GetImageCount(); i++)
	{
		COMBOBOXEXITEM cbi = { 0 };
		CString str;
		int nItem;
		cbi.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
		cbi.iItem = i;
		str.Format(_T("%2d"), i + 1);
		cbi.pszText = str.GetBuffer(0);
		cbi.cchTextMax = str.GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;

		//为组合框添加新项目
		nItem = m_comboBox.InsertItem(&cbi);
		ASSERT(nItem == i);
	}

	//设置当前的选择(默认选择）
	m_comboBox.SetCurSel(0);

	//载入头像图标
	HICON myIcon[6];
	for (int i = 0; i < 6; i++)
	{
		myIcon[i] = AfxGetApp()->LoadIconA(IDI_ICON1 + i);
	}

	//创建图像列表 ：
	m_imageList2.Create(32, 32, ILC_COLOR32, 6, 6);
	for (int i = 0; i < 6; i++)
	{
		m_imageList2.Add(myIcon[i]);
	}
	m_list.SetImageList(&m_imageList2, LVSIL_SMALL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnEnChangehsymsg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CClientDlg::OnBnClickedenter()
{
	// TODO:  在此添加控件通知处理程序代码
	//选择头像:
	m_imageNum = m_comboBox.GetCurSel();
	g_Stop = true;
	Client = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr((LPCTSTR)m_IP);
	serverAddr.sin_port = htons(m_port);
	LoginPkt pkt = { 0 };
	pkt.header.nPktType = PKT_LOGIN;
	pkt.imageNum = m_imageNum;
	strcpy(pkt.userName, ((LPCTSTR)m_userName));
	strcpy(pkt.password , ((LPCTSTR)m_password));
	sendto(Client, (char*)(&pkt), sizeof(LoginPkt), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	char revBuff[1024];
	int addrLen = sizeof(serverAddr);
	int revSize = recvfrom(Client, revBuff, sizeof(revBuff), 0, (SOCKADDR*)&serverAddr, &addrLen);
	LoginReplyPkt* reply = (LoginReplyPkt*)revBuff;
	if (reply->rpy == -1){
		MessageBox("用户名已存在或密码错误");
		m_login.EnableWindow(TRUE);
		return;
	}
	else if (reply->rpy == 0){
		MessageBox("成功进入聊天室");
		close_Flag = 1;
		m_login.EnableWindow(FALSE);
		
		MessageBox(reply->welcome);
		m_logout.EnableWindow(TRUE);


		//获取在线用户列表
		curNum = 0;
		for (int i = 0; i < reply->onlineUsers; i++)
		{
			char buff[1024];
			int addrLen = sizeof(serverAddr);
			int rSize = recvfrom(Client, buff, sizeof(buff), 0, (SOCKADDR*)&serverAddr, &addrLen);
			userItem* nUserList = (userItem*)buff;
			strcpy(userList[curNum].userName , nUserList->userName);//写入在线用户列表			
			lvitem.mask = LVIF_IMAGE | LVFIF_TEXT;
			lvitem.iItem = curNum;
			lvitem.pszText = nUserList->userName;
			lvitem.iImage = nUserList->imgNum;
			m_list.InsertItem(&lvitem);
			curNum++;
		}
		

		DWORD dwThreadID;
		HANDLE hRecvThread = CreateThread(NULL, 0,
			RecvThread, (LPVOID)this, 0, &dwThreadID);
		CloseHandle(hRecvThread);
		return;
	}
	else if (reply->rpy == 99){
		MessageBox("请勿重复登录");
		return;
	}
	else if(reply->rpy==101){
		MessageBox("服务端出现问题，请等待维护完毕再用，对此志鹏工作室表示很抱歉");  //rpy==101用户数据库（注册文件）丢失
		return;
	}
}


void CClientDlg::OnBnClickedexit()
{
	// TODO:  在此添加控件通知处理程序代码
	m_logout.EnableWindow(FALSE);
	LogoutPkt pkt = { 0 };
	pkt.header.nPktType = PKT_LOGOUT;
	strcpy(pkt.userName,(LPCTSTR)m_userName);
	sendto(Client, (char*)(&pkt), sizeof(pkt), 0, (struct sockaddr*)&serverAddr,sizeof(serverAddr));
	

	g_Stop = false;
	Sleep(200);
	closesocket(Client);
	close_Flag = 1;  //已closesocket的标记
	Client = INVALID_SOCKET;
	MessageBox("你已成功退出聊天室");
	m_login.EnableWindow(true);
	m_login.UpdateData(true);
	
	//GetDlgItem(IDC_hsyMsg)->SetWindowText(_T(""));
}


void CClientDlg::OnBnClickedcheck()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CClientDlg::OnBnClickedsend()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	if (m_check == FALSE)// 公聊的情况:
	{
		PublicMsgPkt pkt = { 0 };
		pkt.header.nPktType = PKT_PUBLIC_TLK;
		strcpy(pkt.fromName, (LPCTSTR)m_userName);
		strcpy(pkt.message, m_sMsg);
		sendto(Client, (char*)(&pkt), sizeof(pkt), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

		char szOutText[1024];
		strcpy(szOutText, pkt.fromName);
		strcat(szOutText, ":");
		strcat(szOutText, pkt.message);
		strcat(szOutText, "\r\n");
		m_hsyMsg.LineScroll(m_hsyMsg.GetLineCount() - 1, 0);
		m_hsyMsg.SetSel(0, 0);
		m_hsyMsg.ReplaceSel(szOutText);

		m_sMsg.Empty();    //或者GetDlgItem(IDC_sMsg)->SetWindowText(_T(""));
	}
	else
	{
		PrivateMsgPkt pkt = { 0 };
		pkt.header.nPktType = PKT_PRIVATE_TLK;
		strcpy(pkt.fromName, (LPCTSTR)m_userName);
		strcpy(pkt.message, m_sMsg);
		strcpy(pkt.toName, userList[m_curIndex].userName);
		sendto(Client, (char*)(&pkt), sizeof(pkt), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

		char szOutText[1024];
		strcpy(szOutText, "你对");
		strcat(szOutText, pkt.toName);
		strcat(szOutText, "说:");
		strcat(szOutText, pkt.message);
		strcat(szOutText, "\r\n");
		m_hsyMsg.LineScroll(m_hsyMsg.GetLineCount() - 1, 0);
		m_hsyMsg.SetSel(0, 0);
		m_hsyMsg.ReplaceSel(szOutText);

		m_sMsg.Empty();     // GetDlgItem(IDC_sMsg)->SetWindowTextA(_T(""));
	}
	UpdateData(FALSE);
}


void CClientDlg::OnEnChangesvrip()
{
	UpdateData(true);
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CClientDlg::OnEnChangeusername()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CClientDlg::OnEnChangesvrport()
{
	UpdateData(true);
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CClientDlg::OnEnChangepassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CClientDlg::OnEnChangesmsg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	
}


void CClientDlg::OnBnClickedregister()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szBuf[1204];
	_stprintf_s(szBuf, sizeof(szBuf), _T("%s %d"),m_IP,m_port);
	ShellExecute(NULL, _T("open"), _T("rClient.exe"), szBuf, NULL, SW_SHOWNORMAL);
}


void CClientDlg::Onshut()
{
	// TODO:  在此添加命令处理程序代码
	OnClose();
}


void CClientDlg::OnHelp()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("注册请使用端口号5050\n聊天请使用端口号5150\n");
}




void CClientDlg::Onabout()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("本软件由物联网2班罗志鹏制作\n          学号13055824\n           欢迎指正！\n");
}
void CAboutDlg::Onabout()
{

}


void CClientDlg::OnBnClickedclearup()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_hsyMsg)->SetWindowText(_T(""));
}


BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)  //快捷键
{
	// TODO:  在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CClientDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (close_Flag == 1)
	{
		LogoutPkt pkt = { 0 };
		pkt.header.nPktType = PKT_LOGOUT;
		strcpy(pkt.userName, m_userName);
		sendto(Client, (char*)(&pkt), sizeof(LogoutPkt), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
		closesocket(Client);
	}
	CDialogEx::OnClose();
}


void CClientDlg::OnNMDblclkicolist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	m_curIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (m_curIndex == -1)
	{
		AfxMessageBox("还没有选择私聊的用户");
	}
	m_check = true;
	UpdateData(FALSE);
	*pResult = 0;
}

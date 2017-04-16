
// mfcServerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Protocol.h"
#include<winsock.h>
#include "afxwin.h"


// CmfcServerDlg 对话框
class CmfcServerDlg : public CDialogEx
{
	friend DWORD WINAPI RecvThread(LPVOID lpParam);
private:
	SOCKET Server;
	sockaddr_in local;
// 构造
public:
	CmfcServerDlg(CWnd* pParent = NULL);	// 标准构造函数


// 对话框数据
	enum { IDD = IDD_MFCSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool sock_Close = false; //用来判断是否closesocket
	HACCEL hAccel;
	afx_msg void Onshut();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHelp();
	afx_msg void Onabout();
	CListCtrl m_list;
	afx_msg void OnBnClickedopentalk();

	afx_msg void OnLvnItemchangeduserlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedopenregister();
	afx_msg void OnBnClickedshuttalk();
	CButton m_login;
	CButton m_rlogin;
	CButton m_logout;
	CButton m_rlogout;
	CEdit m_message;
	afx_msg void OnBnClickedshutregister();
};

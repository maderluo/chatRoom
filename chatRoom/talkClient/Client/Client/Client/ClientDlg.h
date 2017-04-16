
// ClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <winsock.h>
#include "afxcmn.h"


// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
private:
	friend DWORD WINAPI RecvThread(LPVOID lpParam);
	SOCKET Client;
	struct sockaddr_in serverAddr;


// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

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
	int close_Flag=0; //用来判断是否closesocket
	HACCEL hAccel;
	CString m_IP;
	CString m_userName;
	UINT m_port;
	CString m_password;
	CEdit m_hsyMsg;
	CString m_sMsg;
	afx_msg void OnEnChangehsymsg();
	BOOL m_check;
	afx_msg void OnBnClickedenter();
	afx_msg void OnBnClickedexit();
	afx_msg void OnBnClickedcheck();
	afx_msg void OnBnClickedsend();
	afx_msg void OnEnChangesvrip();
	afx_msg void OnEnChangeusername();
	afx_msg void OnEnChangesvrport();
	afx_msg void OnEnChangepassword();
	afx_msg void OnEnChangesmsg();
	CButton m_register;
	afx_msg void OnBnClickedregister();
	afx_msg void Onshut();
	afx_msg void OnHelp();
	afx_msg void Onabout();
	afx_msg void OnBnClickedclearup();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnClose();
	CButton m_login;
	CButton m_logout;
	afx_msg void OnClose();
	CListCtrl m_list;
	CComboBoxEx m_comboBox;

	int m_curIndex;  //选中的私聊聊友
	int m_imageNum;  //记录comboBox的图像号码
	CImageList m_imageList1;  //用于comboBox
	CImageList m_imageList2;   //用于list control
	afx_msg void OnNMDblclkicolist(NMHDR *pNMHDR, LRESULT *pResult);
};

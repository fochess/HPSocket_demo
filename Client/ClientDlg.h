
// ClientDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "../HpSocket/TcpClient.h"
#include "../HpSocket/Global/helper.h"

// CClientDlg dialog
class CClientDlg : public CDialogEx, public CTcpClientListener
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUserInfoMsg(WPARAM wp, LPARAM lp);
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CEdit m_Content;
	CButton m_Send;
	CListBox m_Info;
	CEdit m_Address;
	CEdit m_Port;
	CButton m_Async;
	CButton m_Start;
	CButton m_Stop;

	BOOL m_bAsyncConn;
	EnAppState m_enState;
	CTcpClient m_Client;
public:
	void SetAppState(EnAppState state);
private:
	virtual EnHandleResult OnSend(IClient* pClient, const BYTE* pData, int iLength);
	virtual EnHandleResult OnReceive(IClient* pClient, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(IClient* pClient);
	virtual EnHandleResult OnError(IClient* pClient, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnConnect(IClient* pClient);
public:
	afx_msg void OnEnChangeEdit1();
};


// ServerDlg.h : header file
//

#pragma once

#include "afxwin.h"
#include "../HpSocket/TcpServer.h"
#include "../HpSocket/Global/helper.h"

// CServerDlg dialog
class CServerDlg : public CDialogEx, public CTcpServerListener
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVER_DIALOG };

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
	afx_msg LRESULT CServerDlg::OnUserInfoMsg(WPARAM wp, LPARAM lp);
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_Info;
	CEdit m_Address;
	CEdit m_ConnID;
	CButton m_DisConn;
	CButton m_Start;
	CButton m_Stop;
	EnAppState m_enState;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit2();

private:
	static const USHORT PORT;
	static const LPCTSTR ADDRESS;
	CString m_strAddress;
	CTcpServer m_Server;

public:
	void SetAppState(EnAppState state);
private:
	virtual EnHandleResult OnPrepareListen(SOCKET soListen);
	virtual EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient);
	virtual EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnReceive(CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(CONNID dwConnID);
	virtual EnHandleResult OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnShutdown();
};

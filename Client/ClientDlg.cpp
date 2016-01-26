
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CClientDlg dialog

#define DEFAULT_CONTENT	_T("text to be sent")
#define DEFAULT_ADDRESS	_T("127.0.0.1")
#define DEFAULT_PORT	_T("5555")

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent),m_Client(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Content);
	DDX_Control(pDX, IDC_BUTTON1, m_Send);
	DDX_Control(pDX, IDC_INFO, m_Info);
	DDX_Control(pDX, IDC_EDIT2, m_Address);
	DDX_Control(pDX, IDC_EDIT3, m_Port);
	DDX_Control(pDX, IDC_CHECK1, m_Async);
	DDX_Control(pDX, IDC_BUTTON2, m_Start);
	DDX_Control(pDX, IDC_BUTTON3, m_Stop);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CClientDlg::OnBnClickedButton3)
	ON_MESSAGE(USER_INFO_MSG, OnUserInfoMsg)
	ON_WM_VKEYTOITEM()
	ON_EN_CHANGE(IDC_EDIT1, &CClientDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Content.SetWindowText(DEFAULT_CONTENT);
	m_Address.SetWindowText(DEFAULT_ADDRESS);
	m_Port.SetWindowText(DEFAULT_PORT);
	m_Async.SetCheck(BST_CHECKED);

	::SetMainWnd(this);
	::SetInfoList(&m_Info);
	SetAppState(ST_STOPPED);

	m_bAsyncConn = FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)
{
	if (
		pMsg->message == WM_KEYDOWN		
		&&(	pMsg->wParam == VK_ESCAPE	 
		||	pMsg->wParam == VK_CANCEL	
		||	pMsg->wParam == VK_RETURN	
		))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CClientDlg::SetAppState(EnAppState state)
{
	m_enState = state;

	if(this->GetSafeHwnd() == nullptr)
		return;

	m_Async.EnableWindow(m_enState == ST_STOPPED);
	m_Start.EnableWindow(m_enState == ST_STOPPED);
	m_Stop.EnableWindow(m_enState == ST_STARTED);
	m_Send.EnableWindow(m_enState == ST_STARTED && m_Content.GetWindowTextLength() > 0);
	m_Address.EnableWindow(m_enState == ST_STOPPED);
	m_Port.EnableWindow(m_enState == ST_STOPPED);
}

//send
void CClientDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;

	CString strContent;
	m_Content.GetWindowText(strContent);

	LPSTR lpszContent = T2A((LPTSTR)(LPCTSTR)strContent);
	int iLen = (int)strlen(lpszContent);

	if(m_Client.Send((LPBYTE)lpszContent, iLen))
		::LogSend(m_Client.GetConnectionID(), strContent);
	else
		::LogSendFail(m_Client.GetConnectionID(), ::GetLastError(), ::GetSocketErrorDesc(SE_DATA_SEND));
}

//start
void CClientDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	SetAppState(ST_STARTING);
	CString strAddress;
	CString strPort;
	m_Address.GetWindowText(strAddress);
	m_Port.GetWindowText(strPort);

	USHORT usPort = (USHORT)_ttoi(strPort);
	m_bAsyncConn = m_Async.GetCheck();
	::LogClientStarting(strAddress,usPort);

	if (m_Client.Start(strAddress,usPort,m_bAsyncConn))
	{
	} 
	else 
	{
		::LogClientStartFail(m_Client.GetLastError(),m_Client.GetLastErrorDesc());
		SetAppState(ST_STOPPED);
	}
}

//stop
void CClientDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	SetAppState(ST_STOPPING);

	if(m_Client.Stop())
		::LogClientStopping(m_Client.GetConnectionID());
	else
		ASSERT(FALSE);
}

int CClientDlg::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex)
{
	if(nKey == 'C')
		pListBox->ResetContent();

	return __super::OnVKeyToItem(nKey, pListBox, nIndex);
}

LRESULT CClientDlg::OnUserInfoMsg(WPARAM wp, LPARAM lp)
{
	info_msg* msg = (info_msg*)wp;

	::LogInfoMsg(msg);

	return 0;
}


void CClientDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	m_Send.EnableWindow(m_enState == ST_STARTED && m_Content.GetWindowTextLength() > 0);
}


EnHandleResult CClientDlg::OnConnect(IClient* pClient)
{
	TCHAR szAddress[40];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	pClient->GetLocalAddress(szAddress, iAddressLen, usPort);

	::PostOnConnect(pClient->GetConnectionID(), szAddress, usPort);
	SetAppState(ST_STARTED);

	return HR_OK;
}

EnHandleResult CClientDlg::OnSend(IClient* pClient, const BYTE* pData, int iLength)
{
	::PostOnSend(pClient->GetConnectionID(), pData, iLength);
	return HR_OK;
}

EnHandleResult CClientDlg::OnReceive(IClient* pClient, const BYTE* pData, int iLength)
{
	::PostOnReceive(pClient->GetConnectionID(), pData, iLength);
	return HR_OK;
}

EnHandleResult CClientDlg::OnClose(IClient* pClient)
{
	::PostOnClose(pClient->GetConnectionID());
	SetAppState(ST_STOPPED);
	return HR_OK;
}

EnHandleResult CClientDlg::OnError(IClient* pClient, EnSocketOperation enOperation, int iErrorCode)
{
	::PostOnError(pClient->GetConnectionID(), enOperation, iErrorCode);
	SetAppState(ST_STOPPED);
	return HR_OK;
}

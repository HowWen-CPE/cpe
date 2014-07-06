// upgrader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "upgrader.h"

#include "MainFrm.h"
#include "upgraderDoc.h"
#include "upgraderView.h"
//#include "ListVwEx.h"	// base class for CRowListView
#include "BitmapDialog.h"
#include "SelectConectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpgraderApp

BEGIN_MESSAGE_MAP(CUpgraderApp, CWinApp)
	//{{AFX_MSG_MAP(CUpgraderApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgraderApp construction

CUpgraderApp::CUpgraderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUpgraderApp object

CUpgraderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUpgraderApp initialization

BOOL CUpgraderApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUpgraderDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CUpgraderView));
	AddDocTemplate(pDocTemplate);

	
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, "Locator");


	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{

		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		MessageBox(NULL,"Only one instance can be run at one time!","Error",MB_OK);
		return FALSE;
	}
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	CString str;
	str.LoadString(AFX_IDS_APP_TITLE);
	m_pMainWnd->SetWindowText(str);
	WSADATA WSAData;
	if (WSAStartup((WORD)((1<<8)|1), (LPWSADATA) &WSAData) != 0)
	{
		CString strMessage;
		strMessage.LoadString(IDS_ERR_WINSOCK);
		AfxMessageBox(strMessage, MB_OK);
		return FALSE;
	}
	if (!Init_Session())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CBitmapDialog/*CDialog*/
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBitmapDialog/*CDialog*/(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog/*CDialog*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBitmapDialog/*CDialog*/)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CUpgraderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CUpgraderApp commands

int CUpgraderApp::ExitInstance() 
{
	/*
	if (m_lpSession)
		snmp_close(m_lpSession);
	*/
	WSACleanup();			
	return CWinApp::ExitInstance();
}

BOOL CUpgraderApp::Init_Session()
{
	/*
	SESSION  session;
	char community[] = "public";
	ZeroMemory((char *)&session, sizeof(SESSION));
	session.peername = "255.255.255.255";
	session.community = (u_char *)community;
	session.community_len = strlen(community);
	session.retries = SNMP_DEFAULT_RETRIES;
	session.timeout = SNMP_DEFAULT_TIMEOUT;
	session.authenticator = NULL;
	snmp_synch_setup(&session);
	m_lpSession = snmp_open(&session);
	if (m_lpSession == NULL)
	{
		CString strbuf;
		strbuf.LoadString(IDS_ERR_SOCK_OPENFAIL);
		AfxMessageBox(strbuf, MB_OK);
		return FALSE;
	}
	*/
	return TRUE;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CBitmapDialog/*CDialog*/::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBitmap(IDB_BG_ABOUT);

	m_nType = CBitmapDialog::BITMAP_STRETCH;
	// force the background to repaint
	Invalidate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
//		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBitmapDialog::OnPaint();
	}
}

// 取得文档指针的全局函数 David 2010.6.2 add
CUpgraderDoc* GetDocument()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CUpgraderView* pView = (CUpgraderView*)pMainFrm->GetActiveView();
	return pView->GetDocument();
}

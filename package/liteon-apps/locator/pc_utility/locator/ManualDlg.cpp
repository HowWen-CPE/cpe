// ManualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "ManualDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualDlg dialog


CManualDlg::CManualDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualDlg)
	m_strPassword = _T("");
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
	m_strIpAddr = _T("");
}


void CManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_ctlIpAddr);
	DDX_Text(pDX, IDC_MANUAL_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_MANUAL_PATH, m_strFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualDlg, CDialog)
	//{{AFX_MSG_MAP(CManualDlg)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, OnBrowseButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualDlg message handlers

BOOL CManualDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CManualDlg::OnOK() 
{
	UpdateData(TRUE);
	u_long  lIp;
	u_char  *ip = (u_char *)&lIp;

	m_ctlIpAddr.GetAddress(ip[0],ip[1],ip[2],ip[3]);
	struct in_addr  in;
	in.s_addr = lIp;
	m_strIpAddr = inet_ntoa(in);

	CDialog::OnOK();
}

void CManualDlg::OnBrowseButton() 
{
	UpdateData(TRUE);  // for password
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, "All Files (*.*)|*.*||", this);
	if (FileDlg.DoModal() !=IDOK)
		return;
	m_strFilePath = FileDlg.GetPathName();
	UpdateData(FALSE);	
}

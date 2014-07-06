// InterfaceSelect.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "InterfaceSelect.h"
#include "upgraderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterfaceSelect dialog


CInterfaceSelect::CInterfaceSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CInterfaceSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInterfaceSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}


void CInterfaceSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterfaceSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterfaceSelect, CDialog)
	//{{AFX_MSG_MAP(CInterfaceSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterfaceSelect message handlers

BOOL CInterfaceSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// bind to a local socket and an interface.
//	(CUpgraderView*)m_pParent->
	CUpgraderView* pUpgraderView = (CUpgraderView*)m_pParent;

	memset(LocalHostIP,NULL,sizeof(LocalHostIP));
	GetLocalIPs (LocalHostIP, sizeof(LocalHostIP));
	int len = strlen(LocalHostIP);
//	if ( IsHexDigit(LocalHostIP[len-1]) != 1 )
		LocalHostIP[len-1] = 0;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

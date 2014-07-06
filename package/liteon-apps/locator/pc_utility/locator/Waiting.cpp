// Waiting.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "Waiting.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaiting dialog


CWaiting::CWaiting(CWnd* pParent /*=NULL*/)
	: CDialog(CWaiting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaiting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Create(IDD,pParent);
}

void CWaiting::Close()
{
	m_ctlAnimate.Close();
	DestroyWindow();
}

void CWaiting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaiting)
	DDX_Control(pDX, IDC_SEARCH_ANIMATE, m_ctlAnimate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaiting, CDialog)
	//{{AFX_MSG_MAP(CWaiting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaiting message handlers

BOOL CWaiting::OnInitDialog() 
{
	CDialog::OnInitDialog();
/*	char path[_MAX_PATH];
	GetCurrentPath(path);
	strcat(path,"search.avi");
	m_ctlAnimate.Open(path); */
	m_ctlAnimate.Open(IDR_SEARCH_AVI);
	m_ctlAnimate.Play(0,-1,-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaiting::PostNcDestroy() 
{
	delete this;	
}

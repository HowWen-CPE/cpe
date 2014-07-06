// Login.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog


CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogin)
	m_sPassword = _T("");
	m_sUserName = _T("");
	//}}AFX_DATA_INIT
//	m_brush.CreateStockObject (HOLLOW_BRUSH); 
}


void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogin)
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_USERNAME, m_sUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogin, CBitmapDialog)
	//{{AFX_MSG_MAP(CLogin)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_DEFAULT_USER, &CLogin::OnStnClickedStaticDefaultUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers

BOOL CLogin::OnInitDialog() 
{
	CBitmapDialog::OnInitDialog();
	
//	if (&m_brush != NULL)
//		m_brush.DeleteObject();

//	m_brush.CreateSolidBrush(RGB(255, 255, 255)); // color white brush 
	// TODO: Add extra initialization here
	((CEdit*)GetDlgItem(IDC_USERNAME))->SetLimitText(32);
	((CEdit*)GetDlgItem(IDC_PASSWORD))->SetLimitText(32);

	// Default User Name & Password
	m_sPassword = _T("");
	m_sUserName = _T("");
	
	UpdateData(false);
	SetBitmap(IDB_LOGIN);

	m_nType = CBitmapDialog::BITMAP_STRETCH;
	// force the background to repaint
	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
		
	// TODO: Return a different brush if the default is not desired
	//if (mBitmap.GetSafeHandle ( ) !=NULL)
	//{
		switch (nCtlColor) 
		{
		case CTLCOLOR_STATIC:
			pDC->SetBkMode (TRANSPARENT); 
			return (HBRUSH) m_brush.m_hObject;
			break;
		default:
			HBRUSH hbr = CBitmapDialog::OnCtlColor (pDC, pWnd, nCtlColor);
			return hbr ; 
			//break; 
		}
}
*/
void CLogin::OnPaint() 
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

void CLogin::OnBnClickedOk()
{
	// 1.更新到内存
	UpdateData(TRUE);

	// 2.关闭对话框
	CBitmapDialog::OnOK();
}

void CLogin::OnStnClickedStaticDefaultUser()
{
	m_sPassword = _T("1234");
	m_sUserName = _T("admin");
	UpdateData(FALSE);

	// TODO: Add your control notification handler code here
}

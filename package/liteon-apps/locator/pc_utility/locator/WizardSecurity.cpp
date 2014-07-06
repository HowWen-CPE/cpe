// WizardSecurity.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSecurity.h"
#include "UpgraderView.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSecurity property page

IMPLEMENT_DYNCREATE(CWizardSecurity, CNGWizardPage/*CPropertyPageEx*/)

CWizardSecurity::CWizardSecurity(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSecurity::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSecurity)
	m_iWzAuth = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_SEC_1,	WIZ97_BITMAP_STRETCH);
}

CWizardSecurity::~CWizardSecurity()
{
}

void CWizardSecurity::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSecurity)
	DDX_Radio(pDX, IDC_RADIO_WZAUTH, m_iWzAuth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSecurity, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSecurity)
	ON_BN_CLICKED(IDC_RADIO_WZAUTH, OnRadioWzauth)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSecurity message handlers

BOOL CWizardSecurity::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	switch(pWizardSheet->m_pWizardLogin->m_iAuth)
	{
	case AUTH_OPEN_SYSTEM:
		if (pWizardSheet->m_pWizardLogin->m_iEncryptionOnOff == OFF)
			m_iWzAuth = DISABLED;
		else
			m_iWzAuth = pWizardSheet->m_pWizardLogin->m_iWepKeyType;
		break;
	case AUTH_SHARED_KEY:
		m_iWzAuth = pWizardSheet->m_pWizardLogin->m_iWepKeyType;
		break;
	case AUTH_AUTO:
		m_iWzAuth = pWizardSheet->m_pWizardLogin->m_iWepKeyType;
		break;
	case AUTH_WPA:
		m_iWzAuth = WPA;
		break;
	case AUTH_WPAPSK:
		m_iWzAuth = WPA_PSK;
		break;
	}
//	m_iWzAuth			= 0;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizardSecurity::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	switch(m_iWzAuth)
	{
	case WPA_PSK:
			pWizardSheet->SetActivePage(PAGE_SEC_WPA_PSK);
		break;
	case WPA:
			pWizardSheet->SetActivePage(PAGE_SEC_WPA);
		break;
	case DISABLED:
		break;
	case WEP64:
	case WEP128:
	case WEP152:
			pWizardSheet->SetActivePage(PAGE_SEC_WEP);
		break;
	}
	return -1;
//	return CPropertyPageEx::OnWizardNext();
}

void CWizardSecurity::OnRadioWzauth() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
}

void CWizardSecurity::OnRadio2() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

void CWizardSecurity::OnRadio3() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

void CWizardSecurity::OnRadio4() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

void CWizardSecurity::OnRadio5() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

LRESULT CWizardSecurity::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	if (pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_AP)
		pWizardSheet->SetActivePage(PAGE_SYS_AP);
	else if (pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_STA0)
		pWizardSheet->SetActivePage(PAGE_SYS_CLIENT_INF);

	return -1;
//	return CPropertyPageEx::OnWizardBack();
}

BOOL CWizardSecurity::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	if (pWizardSheet->m_pWizardSystem->m_iWzOpMode == AP_MODE)
		GetDlgItem(IDC_RADIO1)->EnableWindow(true);
	else if (pWizardSheet->m_pWizardSystem->m_iWzOpMode == CLIENT_MODE)
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(false);
		if ( m_iWzAuth == WPA )
			m_iWzAuth = DISABLED;
		UpdateData(false);
	}

	switch(m_iWzAuth)
	{
	case DISABLED:
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
		break;
	case WPA:
	case WPA_PSK:
	case WEP64:
	case WEP128:
	case WEP152:
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
		break;
	}

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

void CWizardSecurity::OnRadio1() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

BOOL CWizardSecurity::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, true);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CPropertyPageEx::OnWizardFinish();
	else
		return 0;
}

HBRUSH CWizardSecurity::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CNGWizardPage/*CPropertyPageEx*/::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	int CtrlID;
	// TODO: Change any attributes of the DC here
	CtrlID = pWnd->GetDlgCtrlID();

	switch( CtrlID )
	{
		case IDC_STATIC_NOTE_INF:
			pDC->SetTextColor(RGB(0,0,200));
			break;
		case IDC_STATIC_NOTE:
			pDC->SetTextColor(RGB(125,0,0));
			break;
	}

	return hbr;
}

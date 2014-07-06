// WizardSecurityRepeater.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSecurityRepeater.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSecurityRepeater property page

IMPLEMENT_DYNCREATE(CWizardSecurityRepeater, CNGWizardPage/*CPropertyPageEx*/)

CWizardSecurityRepeater::CWizardSecurityRepeater(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSecurityRepeater::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSecurityRepeater)
	m_iWzAuthRepeater = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_SEC_2,	WIZ97_BITMAP_STRETCH);
}

CWizardSecurityRepeater::~CWizardSecurityRepeater()
{
}

void CWizardSecurityRepeater::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSecurityRepeater)
	DDX_Radio(pDX, IDC_RADIO_WZAUTH_REPEATER, m_iWzAuthRepeater);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSecurityRepeater, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSecurityRepeater)
	ON_BN_CLICKED(IDC_RADIO_WZAUTH_REPEATER, OnRadioWzauthRepeater)
	ON_BN_CLICKED(IDC_RADIO2_REPEATER, OnRadio2Repeater)
	ON_BN_CLICKED(IDC_RADIO3_REPEATER, OnRadio3Repeater)
	ON_BN_CLICKED(IDC_RADIO4_REPEATER, OnRadio4Repeater)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSecurityRepeater message handlers

BOOL CWizardSecurityRepeater::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	switch(pWizardSheet->m_pWizardLogin->m_iAuth)
	{
	case AUTH_OPEN_SYSTEM:
		if (pWizardSheet->m_pWizardLogin->m_iEncryptionOnOff == OFF)
			m_iWzAuthRepeater = DISABLED;
		else
			m_iWzAuthRepeater = pWizardSheet->m_pWizardLogin->m_iWepKeyType;
		break;
	case AUTH_SHARED_KEY:
		m_iWzAuthRepeater = pWizardSheet->m_pWizardLogin->m_iWepKeyType;
		break;
		/*scoutlee begin 2004/09/03*/
	case AUTH_AUTO:
		m_iWzAuthRepeater = pWizardSheet->m_pWizardLogin->m_iWepKeyType;
		break;
		/*scoutlee end 2004/09/03*/
	case AUTH_WPA:
	case AUTH_WPAPSK:
		m_iWzAuthRepeater = DISABLED;
		break;
	}
//	m_iWzAuthRepeater			= 0;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardSecurityRepeater::OnRadioWzauthRepeater() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
}

void CWizardSecurityRepeater::OnRadio2Repeater() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

void CWizardSecurityRepeater::OnRadio3Repeater() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

void CWizardSecurityRepeater::OnRadio4Repeater() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
}

LRESULT CWizardSecurityRepeater::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	switch(m_iWzAuthRepeater)
	{
	case DISABLED:
//			pWizardSheet->SetActivePage(PAGE_SUMMARY);
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

LRESULT CWizardSecurityRepeater::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	if (pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_REPEATER)
		pWizardSheet->SetActivePage(PAGE_SYS_REPEATER);
	else if (pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_ADHOC)
		pWizardSheet->SetActivePage(PAGE_SYS_CLIENT_ADHOC);
	return -1;	
//	return CPropertyPageEx::OnWizardBack();
}

BOOL CWizardSecurityRepeater::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	
	switch(m_iWzAuthRepeater)
	{
	case DISABLED:
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
		break;
	case WEP64:
	case WEP128:
	case WEP152:
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
		break;
	}

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

BOOL CWizardSecurityRepeater::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, true);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

// WizardSysClient.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSysClient.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClient property page

IMPLEMENT_DYNCREATE(CWizardSysClient, CNGWizardPage/*CPropertyPageEx*/)

CWizardSysClient::CWizardSysClient(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSysClient::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSysClient)
	m_iWzClientMode = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_NETWORK,	WIZ97_BITMAP_STRETCH);
}

CWizardSysClient::~CWizardSysClient()
{
}

void CWizardSysClient::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSysClient)
	DDX_Radio(pDX, IDC_RADIO_CLIENT_MODE, m_iWzClientMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSysClient, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSysClient)
	ON_BN_CLICKED(IDC_RADIO_CLIENT_MODE, OnRadioClientMode)
	ON_BN_CLICKED(IDC_RADIO2_CLIENT_MODE, OnRadio2ClientMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClient message handlers

void CWizardSysClient::OnRadioClientMode() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	pWizardSheet->m_pWizardSystem->m_iWzOpModeReal = CFG_OP_MODE_STA0;
}

void CWizardSysClient::OnRadio2ClientMode() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	pWizardSheet->m_pWizardSystem->m_iWzOpModeReal = CFG_OP_MODE_ADHOC;
}

LRESULT CWizardSysClient::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	if (m_iWzClientMode == INFRASTRUCTURE)
		pWizardSheet->SetActivePage(PAGE_SYS_CLIENT_INF);
	else
		pWizardSheet->SetActivePage(PAGE_SYS_CLIENT_ADHOC);

	UpdateData(false);

	return -1;
//	return CPropertyPageEx::OnWizardNext();
}

LRESULT CWizardSysClient::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SYS);

	return -1;
//	return CPropertyPageEx::OnWizardBack();
}

BOOL CWizardSysClient::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

BOOL CWizardSysClient::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	switch(pWizardSheet->m_pWizardSystem->m_iWzOpModeReal)
	{
	case CFG_OP_MODE_ADHOC:
		m_iWzClientMode = CLIENT_AD_HOC;
		break;
	case CFG_OP_MODE_STA0:
		m_iWzClientMode = INFRASTRUCTURE;
		break;
		/*Scoutlee Begin  2004/09/16 */
	case MODE_SELECT_108G:
		m_iWzClientMode = W_802_11SUPER_G;
		break;
			
		/*Scoutlee Begin  2004/09/16 */
	}
	
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

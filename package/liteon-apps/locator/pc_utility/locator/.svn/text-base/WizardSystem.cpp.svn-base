// WizardSystem.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSystem.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSystem property page

IMPLEMENT_DYNCREATE(CWizardSystem, CNGWizardPage/*CPropertyPageEx*/)

CWizardSystem::CWizardSystem(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSystem::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSystem)
	m_iWzOpMode = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_OP_MODE,	WIZ97_BITMAP_STRETCH);
}

CWizardSystem::~CWizardSystem()
{
}

void CWizardSystem::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSystem)
	DDX_Radio(pDX, IDC_RADIO_WZAP, m_iWzOpMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSystem, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSystem)
	ON_BN_CLICKED(IDC_RADIO_WZAP, OnRadioWzap)
	ON_BN_CLICKED(IDC_RADIO_WZCLIENT, OnRadioWzclient)
	ON_BN_CLICKED(IDC_RADIO_WZREPEATER, OnRadioWzrepeater)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSystem message handlers

BOOL CWizardSystem::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	
	m_iWzOpModeReal	= pWizardSheet->m_pWizardLogin->m_iOpMode;
	switch(m_iWzOpModeReal)
	{
	case CFG_OP_MODE_STA0:
	case CFG_OP_MODE_ADHOC:
		m_iWzOpMode	= CLIENT_MODE;
		break;
	case CFG_OP_MODE_AP:
		m_iWzOpMode	= AP_MODE;
		break;
	case CFG_OP_MODE_REPEATER:
		m_iWzOpMode	= REAPTER_MODE;
		break;
	}
//	m_szWzSysName = pWizardSheet->m_pWizardLogin->m_cDevName;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizardSystem::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	switch(m_iWzOpMode)
	{
	case AP_MODE:
		pWizardSheet->SetActivePage(PAGE_SYS_AP);
		return -1;
	case CLIENT_MODE:
		pWizardSheet->SetActivePage(PAGE_SYS_CLIENT);
		return -1;
	case REAPTER_MODE:
		pWizardSheet->SetActivePage(PAGE_SYS_REPEATER);
		return -1;
	default:
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardNext();
	}
}

void CWizardSystem::OnRadioWzap() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_iWzOpModeReal = m_iWzOpMode;
}

void CWizardSystem::OnRadioWzclient() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
	m_iWzOpModeReal = m_iWzOpMode;
}

void CWizardSystem::OnRadioWzrepeater() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_iWzOpModeReal = CFG_OP_MODE_REPEATER;
}

BOOL CWizardSystem::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
	
	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

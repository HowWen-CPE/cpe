// WizardSysDevName.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSysDevName.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSysDevName property page

IMPLEMENT_DYNCREATE(CWizardSysDevName, CNGWizardPage/*CPropertyPageEx*/)

CWizardSysDevName::CWizardSysDevName(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSysDevName::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSysDevName)
	m_szWzSysName = _T("");
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_DEVICE_NAME,	WIZ97_BITMAP_STRETCH);
}

CWizardSysDevName::~CWizardSysDevName()
{
}

void CWizardSysDevName::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSysDevName)
	DDX_Text(pDX, IDC_WZSYS_NAME, m_szWzSysName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSysDevName, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSysDevName)
	ON_EN_CHANGE(IDC_WZSYS_NAME, OnChangeWzsysName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSysDevName message handlers

BOOL CWizardSysDevName::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	
	m_szWzSysName = pWizardSheet->m_pWizardLogin->m_cDevName;
	((CEdit*)GetDlgItem(IDC_WZSYS_NAME))->SetLimitText(32);

	UpdateData(false);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWizardSysDevName::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	if (m_szWzSysName.IsEmpty())
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
	
	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

void CWizardSysDevName::OnChangeWzsysName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CNGWizardPage/*CPropertyPageEx*/::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	if (m_szWzSysName.IsEmpty())
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);	
}

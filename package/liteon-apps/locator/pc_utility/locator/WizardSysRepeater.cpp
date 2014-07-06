// WizardSysRepeater.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSysRepeater.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSysRepeater property page

IMPLEMENT_DYNCREATE(CWizardSysRepeater, CNGWizardPage/*CPropertyPageEx*/)

CWizardSysRepeater::CWizardSysRepeater(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSysRepeater::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSysRepeater)
	m_szWzSsidRepeater = _T("");
	m_iWzReapWirelessMode = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WLAN_REP,	WIZ97_BITMAP_STRETCH);
}

CWizardSysRepeater::~CWizardSysRepeater()
{
}

void CWizardSysRepeater::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSysRepeater)
	DDX_Text(pDX, IDC_WZSSID_REPEATER, m_szWzSsidRepeater);
	DDX_CBIndex(pDX, IDC_WZWIRELESS_MODE_REPEATER, m_iWzReapWirelessMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSysRepeater, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSysRepeater)
	ON_EN_CHANGE(IDC_WZSSID_REPEATER, OnChangeWzssidRepeater)
	ON_CBN_SELCHANGE(IDC_WZWIRELESS_MODE_REPEATER, OnSelchangeWzwirelessModeRepeater)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSysRepeater message handlers

BOOL CWizardSysRepeater::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

void CWizardSysRepeater::OnChangeWzssidRepeater() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	if (m_szWzSsidRepeater.IsEmpty())
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	UpdateData(false);
}

BOOL CWizardSysRepeater::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	m_szWzSsidRepeater		= pWizardSheet->m_pWizardLogin->m_cSSID;
	((CEdit*)GetDlgItem(IDC_WZSSID_REPEATER))->SetLimitText(32);

	m_iWzReapSuperG		= pWizardSheet->m_pWizardLogin->m_iSuperG;
	m_iWzReapWirelessModeReal = pWizardSheet->m_pWizardLogin->m_iWirelessMode;
	if ((m_iWzReapSuperG & ABOLT_TURBO_G) && (m_iWzReapSuperG & ABOLT_TURBO_PRIME))
	{	
		m_iWzReapWirelessMode = W_802_11SUPER_G;
		m_iWzReapWirelessModeReal=W_802_11SUPER_G;
	}
	else
	{
		switch(pWizardSheet->m_pWizardLogin->m_iWirelessMode)
		{
		case MODE_SELECT_11B:
			m_iWzReapWirelessMode = W_802_11B;
			m_iWzReapWirelessModeReal=W_802_11B;
			break;
		case MODE_SELECT_11G:
			m_iWzReapWirelessMode = W_802_11G;
			m_iWzReapWirelessModeReal=W_802_11G;
			break;
		/*Scoutlee Begin  2004/09/16 */
		case MODE_SELECT_108G:
			m_iWzReapWirelessMode = W_802_11Static_G;
			m_iWzReapWirelessModeReal=W_802_11Static_G;
			break;
			
		/*Scoutlee Begin  2004/09/16 */
		}
	}
	
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizardSysRepeater::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SYS);
	return -1;
//	return CPropertyPageEx::OnWizardBack();
}

LRESULT CWizardSysRepeater::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class	
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SEC_REPEATER);
	return -1;
//	return CPropertyPageEx::OnWizardNext();
}

BOOL CWizardSysRepeater::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, false);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

void CWizardSysRepeater::OnSelchangeWzwirelessModeRepeater() 
{
	// TODO: Add your control notification handler code here
	int iSuperGFlag = 0;

	UpdateData(true);
	if (m_iWzReapWirelessMode == W_802_11SUPER_G)
		iSuperGFlag = ( ABOLT_SUPERG | ABOLT_TURBO_PRIME );
	else
		iSuperGFlag = ABOLT_TURBO_G;

	m_iWzReapSuperG = iSuperGFlag;

	switch(m_iWzReapWirelessMode)
	{
	case W_802_11B:
		m_iWzReapWirelessModeReal = W_802_11B;
		break;
	case W_802_11G:
		m_iWzReapWirelessModeReal = W_802_11G;
		break;
	case W_802_11Static_G:
		m_iWzReapWirelessModeReal = W_802_11Static_G;
		break;
	case W_802_11SUPER_G:
		m_iWzReapWirelessModeReal = W_802_11SUPER_G;
		break;

	}
	UpdateData(false);	
}

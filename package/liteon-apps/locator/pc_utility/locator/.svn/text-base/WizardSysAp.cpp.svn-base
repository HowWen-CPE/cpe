// WizardSysAp.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSysAp.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSysAp property page

IMPLEMENT_DYNCREATE(CWizardSysAp, CNGWizardPage/*CPropertyPageEx*/)

CWizardSysAp::CWizardSysAp(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSysAp::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSysAp)
	m_szWzSsidAp = _T("");
	m_iWzChannelAp = 0;
	m_iWzApWirelessMode = 0;
	//}}AFX_DATA_INIT
	m_iWzApSuperG	= 0;
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WLAN_AP,	WIZ97_BITMAP_STRETCH);
}

CWizardSysAp::~CWizardSysAp()
{
}

void CWizardSysAp::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSysAp)
	DDX_Text(pDX, IDC_WZSSID_AP, m_szWzSsidAp);
	DDX_CBIndex(pDX, IDC_WZCHANNEL_AP, m_iWzChannelAp);
	DDX_CBIndex(pDX, IDC_COMBO_WZWLAN_MODE, m_iWzApWirelessMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSysAp, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSysAp)
	ON_CBN_SELCHANGE(IDC_WZCHANNEL_AP, OnSelchangeWzchannelAp)
	ON_CBN_SELCHANGE(IDC_COMBO_WZWLAN_MODE, OnSelchangeComboWzwlanMode)
	ON_EN_CHANGE(IDC_WZSSID_AP, OnChangeWzssidAp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSysAp message handlers

BOOL CWizardSysAp::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	m_szWzSsidAp		= pWizardSheet->m_pWizardLogin->m_cSSID;
	m_iWzChannelAp		= pWizardSheet->m_pWizardLogin->m_iChannel;
	preWzChannelAp		= m_iWzChannelAp;
	m_iWzApSuperG		= pWizardSheet->m_pWizardLogin->m_iSuperG;
	m_iWzApWirelessModeReal = pWizardSheet->m_pWizardLogin->m_iWirelessMode;

	((CEdit*)GetDlgItem(IDC_WZSSID_AP))->SetLimitText(32);

	if ((m_iWzApSuperG & ABOLT_TURBO_G) && (m_iWzApSuperG & ABOLT_TURBO_PRIME))
	{
		m_iWzApWirelessMode = W_802_11SUPER_G;
		m_iWzApWirelessModeReal=W_802_11SUPER_G;
	}
	else
	{
		switch(pWizardSheet->m_pWizardLogin->m_iWirelessMode)
		{
		case MODE_SELECT_11B:
			m_iWzApWirelessMode = W_802_11B;
			m_iWzApWirelessModeReal=W_802_11B;
			break;
		case MODE_SELECT_11G:
			m_iWzApWirelessMode = W_802_11G;
			m_iWzApWirelessModeReal=W_802_11G;
			break;
		/*Scoutlee Begin  2004/09/16 */
		case MODE_SELECT_108G:
			m_iWzApWirelessMode = W_802_11Static_G;
			m_iWzApWirelessModeReal=W_802_11Static_G;
			break;
			
		/*Scoutlee Begin  2004/09/16 */
		}
	}

	UpdateData(false);
	OnSelchangeComboWzwlanMode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizardSysAp::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SEC);
	return -1;
	
//	return CPropertyPageEx::OnWizardNext();
}

BOOL CWizardSysAp::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

void CWizardSysAp::OnSelchangeWzchannelAp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	preWzChannelAp		= m_iWzChannelAp;	
}

void CWizardSysAp::OnSelchangeComboWzwlanMode() 
{
	// TODO: Add your control notification handler code here
	int iSuperGFlag = 0;

	UpdateData(true);
	if ((m_iWzApWirelessMode == W_802_11SUPER_G)||(m_iWzApWirelessMode == W_802_11Static_G))
	{
		GetDlgItem(IDC_WZCHANNEL_AP)->EnableWindow(false);
		m_iWzChannelAp = 0;
		if(m_iWzApWirelessMode == W_802_11SUPER_G)
		iSuperGFlag = ( ABOLT_SUPERG | ABOLT_TURBO_PRIME );
	}
	else
	{
		GetDlgItem(IDC_WZCHANNEL_AP)->EnableWindow(true);
		m_iWzChannelAp = preWzChannelAp;
		iSuperGFlag = ABOLT_TURBO_G;
	}
	m_iWzApSuperG = iSuperGFlag;

	switch(m_iWzApWirelessMode)
	{
	case W_802_11B:
		m_iWzApWirelessModeReal = W_802_11B;
		break;
	case W_802_11G:
		m_iWzApWirelessModeReal = W_802_11G;
		break;
	case W_802_11Static_G:
		m_iWzApWirelessModeReal = W_802_11Static_G;
		break;
	case W_802_11SUPER_G:
		m_iWzApWirelessModeReal = W_802_11SUPER_G;
		break;
	}
	UpdateData(false);
}

BOOL CWizardSysAp::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, false);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

void CWizardSysAp::OnChangeWzssidAp() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	if (m_szWzSsidAp.IsEmpty())
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	UpdateData(false);
}

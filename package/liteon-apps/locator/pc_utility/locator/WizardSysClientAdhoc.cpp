// WizardSysClientAdhoc.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSysClientAdhoc.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClientAdhoc property page

IMPLEMENT_DYNCREATE(CWizardSysClientAdhoc, CNGWizardPage/*CPropertyPageEx*/)

CWizardSysClientAdhoc::CWizardSysClientAdhoc(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSysClientAdhoc::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSysClientAdhoc)
	m_szWzSsidClientAdhoc = _T("");
	m_iWzChannelClientAdhoc = 0;
	m_iWzAdhocWirelessMode = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WLAN_AP,	WIZ97_BITMAP_STRETCH);
}

CWizardSysClientAdhoc::~CWizardSysClientAdhoc()
{
}

void CWizardSysClientAdhoc::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSysClientAdhoc)
	DDX_Text(pDX, IDC_WZSSID_CLIENT_ADHOC, m_szWzSsidClientAdhoc);
	DDX_CBIndex(pDX, IDC_WZCHANNEL_CLIENT_ADHOC, m_iWzChannelClientAdhoc);
	DDX_CBIndex(pDX, IDC_COMBO_WZWLAN_MODE_ADHOC, m_iWzAdhocWirelessMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSysClientAdhoc, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSysClientAdhoc)
	ON_EN_CHANGE(IDC_WZSSID_CLIENT_ADHOC, OnChangeWzssidClientAdhoc)
	ON_CBN_SELCHANGE(IDC_WZCHANNEL_CLIENT_ADHOC, OnSelchangeWzchannelClientAdhoc)
	ON_CBN_SELCHANGE(IDC_COMBO_WZWLAN_MODE_ADHOC, OnSelchangeComboWzwlanModeAdhoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClientAdhoc message handlers

LRESULT CWizardSysClientAdhoc::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SYS_CLIENT);

	return -1;
//	return CPropertyPageEx::OnWizardBack();
}

BOOL CWizardSysClientAdhoc::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, false);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

BOOL CWizardSysClientAdhoc::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

void CWizardSysClientAdhoc::OnChangeWzssidClientAdhoc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	if (m_szWzSsidClientAdhoc.IsEmpty())
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	UpdateData(false);
}

BOOL CWizardSysClientAdhoc::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	m_szWzSsidClientAdhoc	= pWizardSheet->m_pWizardLogin->m_cSSID;
	m_iWzChannelClientAdhoc	= pWizardSheet->m_pWizardLogin->m_iChannel;
	preWzChannelClientAdhoc	= m_iWzChannelClientAdhoc;

	m_iWzAdhocSuperG		= pWizardSheet->m_pWizardLogin->m_iSuperG;
	m_iWzAdhocWirelessModeReal = pWizardSheet->m_pWizardLogin->m_iWirelessMode;
	if ((m_iWzAdhocSuperG & ABOLT_TURBO_G) && (m_iWzAdhocSuperG & ABOLT_TURBO_PRIME))
	{
		m_iWzAdhocWirelessMode = W_802_11SUPER_G;
		m_iWzAdhocWirelessModeReal=W_802_11SUPER_G;
	}
	else
	{
		switch(pWizardSheet->m_pWizardLogin->m_iWirelessMode)
		{
		case MODE_SELECT_11B:
			m_iWzAdhocWirelessMode = W_802_11B;
			m_iWzAdhocWirelessModeReal=W_802_11B;
			break;
		case MODE_SELECT_11G:
			m_iWzAdhocWirelessMode = W_802_11G;
			m_iWzAdhocWirelessModeReal=W_802_11G;
			break;
		/*Scoutlee Begin  2004/09/16 */
		case MODE_SELECT_108G:
			m_iWzAdhocWirelessMode = W_802_11Static_G;
			m_iWzAdhocWirelessModeReal=W_802_11Static_G;
			break;
			
		/*Scoutlee Begin  2004/09/16 */
		}
	}

	((CEdit*)GetDlgItem(IDC_WZSSID_CLIENT_ADHOC))->SetLimitText(32);
	UpdateData(false);
	/*scoutlee begin 2004/09/09 for set channel disable*/
	OnSelchangeComboWzwlanModeAdhoc();
	/*scoutlee end  2004/09/09*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardSysClientAdhoc::OnSelchangeWzchannelClientAdhoc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	preWzChannelClientAdhoc	= m_iWzChannelClientAdhoc;	

}

LRESULT CWizardSysClientAdhoc::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SEC_REPEATER);
	return -1;	
//	return CPropertyPageEx::OnWizardNext();
}

void CWizardSysClientAdhoc::OnSelchangeComboWzwlanModeAdhoc() 
{
	// TODO: Add your control notification handler code here
	int iSuperGFlag = 0;

	UpdateData(true);
if ((m_iWzAdhocWirelessMode == W_802_11SUPER_G)||(m_iWzAdhocWirelessMode == W_802_11Static_G))
	{
		GetDlgItem(IDC_WZCHANNEL_CLIENT_ADHOC)->EnableWindow(false);
		m_iWzChannelClientAdhoc = 0;
		if(m_iWzAdhocWirelessMode == W_802_11SUPER_G)
		iSuperGFlag = ( ABOLT_SUPERG | ABOLT_TURBO_PRIME );
	}
	else
	{
		GetDlgItem(IDC_WZCHANNEL_CLIENT_ADHOC)->EnableWindow(true);
		m_iWzChannelClientAdhoc = preWzChannelClientAdhoc;
		iSuperGFlag = ABOLT_TURBO_G;
	}
	m_iWzAdhocSuperG = iSuperGFlag;

	switch(m_iWzAdhocWirelessMode)
	{
	case W_802_11B:
		m_iWzAdhocWirelessModeReal = W_802_11B;
		break;
	case W_802_11G:
		m_iWzAdhocWirelessModeReal = W_802_11G;
		break;
	case W_802_11Static_G:
		m_iWzAdhocWirelessModeReal = W_802_11Static_G;
		break;
	case W_802_11SUPER_G:
		m_iWzAdhocWirelessModeReal = W_802_11SUPER_G;
		break;
	}
	UpdateData(false);
	
}

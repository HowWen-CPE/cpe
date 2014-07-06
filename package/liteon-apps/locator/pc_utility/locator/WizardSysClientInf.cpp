// WizardSysClientInf.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSysClientInf.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClientInf property page

IMPLEMENT_DYNCREATE(CWizardSysClientInf, CNGWizardPage/*CPropertyPageEx*/)

CWizardSysClientInf::CWizardSysClientInf(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardSysClientInf::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardSysClientInf)
	m_szWzSsidClientInf = _T("");
	m_iWzAnySsidEnable = 0;
	m_iWzInfWirelessMode = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WLAN_AP,	WIZ97_BITMAP_STRETCH);
}

CWizardSysClientInf::~CWizardSysClientInf()
{
}

void CWizardSysClientInf::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSysClientInf)
	DDX_Text(pDX, IDC_WZSSID_CLIENT_INF, m_szWzSsidClientInf);
	DDX_Radio(pDX, IDC_RADIO_STATIC_SSID, m_iWzAnySsidEnable);
	DDX_CBIndex(pDX, IDC_WZWIRELESS_MODE_INF, m_iWzInfWirelessMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSysClientInf, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardSysClientInf)
	ON_BN_CLICKED(IDC_RADIO_ANY_SSID, OnRadioAnySsid)
	ON_BN_CLICKED(IDC_RADIO_STATIC_SSID, OnRadioStaticSsid)
	ON_EN_CHANGE(IDC_WZSSID_CLIENT_INF, OnChangeWzssidClientInf)
	ON_CBN_SELCHANGE(IDC_WZWIRELESS_MODE_INF, OnSelchangeWzwirelessModeInf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClientInf message handlers

BOOL CWizardSysClientInf::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	m_szWzSsidClientInf	= pWizardSheet->m_pWizardLogin->m_cSSID;
	preWzSsidClientInf	= m_szWzSsidClientInf;
	((CEdit*)GetDlgItem(IDC_WZSSID_CLIENT_INF))->SetLimitText(32);

	if (m_szWzSsidClientInf.GetLength() == 0)
		m_iWzAnySsidEnable = 1;
	else
		m_iWzAnySsidEnable = 0;
	m_iWzInfSuperG		= pWizardSheet->m_pWizardLogin->m_iSuperG;
	m_iWzInfWirelessModeReal = pWizardSheet->m_pWizardLogin->m_iWirelessMode;
	if ((m_iWzInfSuperG & ABOLT_TURBO_G) && (m_iWzInfSuperG & ABOLT_TURBO_PRIME))
	{
		m_iWzInfWirelessMode = W_802_11SUPER_G;
		m_iWzInfWirelessModeReal=W_802_11SUPER_G;
	}
	else
	{
		switch(pWizardSheet->m_pWizardLogin->m_iWirelessMode)
		{
		case MODE_SELECT_11B:
			m_iWzInfWirelessMode = W_802_11B;
			m_iWzInfWirelessModeReal=W_802_11B;
			break;
		case MODE_SELECT_11G:
			m_iWzInfWirelessMode = W_802_11G;
			m_iWzInfWirelessModeReal=W_802_11G;
			break;
		/*Scoutlee Begin  2004/09/16 */
		case MODE_SELECT_108G:
			m_iWzInfWirelessMode = W_802_11Static_G;
			m_iWzInfWirelessModeReal=W_802_11Static_G;
			break;
			
		/*Scoutlee Begin  2004/09/16 */
		}
	}
	
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardSysClientInf::OnRadioAnySsid() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	

	preWzSsidClientInf = m_szWzSsidClientInf;
	m_szWzSsidClientInf.Empty();
	UpdateData(false);	
	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
	GetDlgItem(IDC_WZSSID_CLIENT_INF)->ShowWindow(false);
	GetDlgItem(IDC_WZSSID_CLIENT_INF)->EnableWindow(false);
	GetDlgItem(IDC_WZSSID_CLIENT_INF)->ShowWindow(true);
}

void CWizardSysClientInf::OnRadioStaticSsid() 
{
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);	
	m_szWzSsidClientInf = preWzSsidClientInf;

	if (m_szWzSsidClientInf.IsEmpty())
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	GetDlgItem(IDC_WZSSID_CLIENT_INF)->EnableWindow(true);

	UpdateData(false);	
}

BOOL CWizardSysClientInf::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	if (m_iWzAnySsidEnable)
		OnRadioAnySsid();

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

LRESULT CWizardSysClientInf::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SYS_CLIENT);

	return -1;
//	return CPropertyPageEx::OnWizardBack();
}

void CWizardSysClientInf::OnChangeWzssidClientInf() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	if (m_szWzSsidClientInf.IsEmpty())
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	UpdateData(false);
}

LRESULT CWizardSysClientInf::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class	
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SEC);
	return -1;
//	return CPropertyPageEx::OnWizardNext();
}

void CWizardSysClientInf::OnSelchangeWzwirelessModeInf() 
{
	// TODO: Add your control notification handler code here
	int iSuperGFlag = 0;

	UpdateData(true);
	switch(m_iWzInfWirelessMode)
	{
	case W_802_11B:
		m_iWzInfWirelessModeReal = W_802_11B;
		iSuperGFlag = ABOLT_TURBO_G;
		break;
	case W_802_11G:
		m_iWzInfWirelessModeReal = W_802_11G;
		iSuperGFlag = ABOLT_TURBO_G;
		break;
	case W_802_11Static_G:
		m_iWzInfWirelessModeReal = W_802_11Static_G;
		iSuperGFlag = ABOLT_TURBO_G;
		break;
	case W_802_11SUPER_G:
		m_iWzInfWirelessModeReal = W_802_11SUPER_G;
		iSuperGFlag = ( ABOLT_SUPERG | ABOLT_TURBO_PRIME );
		break;
	}
	m_iWzInfSuperG = iSuperGFlag;
}

BOOL CWizardSysClientInf::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, false);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

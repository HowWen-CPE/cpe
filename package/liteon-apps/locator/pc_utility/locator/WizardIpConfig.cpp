// WizardIpConfig.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardIpConfig.h"
#include "UpgraderView.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardIpConfig property page

IMPLEMENT_DYNCREATE(CWizardIpConfig, CNGWizardPage/*CPropertyPageEx*/)

CWizardIpConfig::CWizardIpConfig(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage/*CPropertyPageEx*/(CWizardIpConfig::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardIpConfig)
	m_iWzDhcpEnable = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_IP_SETTING,	WIZ97_BITMAP_STRETCH);
}

CWizardIpConfig::~CWizardIpConfig()
{
}

void CWizardIpConfig::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardIpConfig)
	DDX_Control(pDX, IDC_WZSUB_MASK, m_WzCfgSubMask);
	DDX_Control(pDX, IDC_WZIPADDRESS, m_WzCfgIPAddr);
	DDX_Control(pDX, IDC_WZGATEWAY, m_WzCfgGateway);
	DDX_Radio(pDX, IDC_WZRADIO_DHCP_ENABLE, m_iWzDhcpEnable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardIpConfig, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardIpConfig)
	ON_BN_CLICKED(IDC_WZRADIO_DHCP_ENABLE, OnWzradioDhcpEnable)
	ON_BN_CLICKED(IDC_WZRADIO, OnWzradio)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_WZIPADDRESS, OnFieldchangedWzipaddress)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_WZSUB_MASK, OnFieldchangedWzsubMask)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_WZGATEWAY, OnFieldchangedWzgateway)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardIpConfig message handlers

void CWizardIpConfig::OnWzradioDhcpEnable() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	GetDlgItem(IDC_WZIPADDRESS)->EnableWindow(true);
	GetDlgItem(IDC_WZSUB_MASK)->EnableWindow(true);
	GetDlgItem(IDC_WZGATEWAY)->EnableWindow(true);	
}

void CWizardIpConfig::OnWzradio() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	GetDlgItem(IDC_WZIPADDRESS)->EnableWindow(false);
	GetDlgItem(IDC_WZSUB_MASK)->EnableWindow(false);
	GetDlgItem(IDC_WZGATEWAY)->EnableWindow(false);		
}

BOOL CWizardIpConfig::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	memcpy(m_szIPAddr, pWizardSheet->m_pWizardLogin->m_cIPAddr, sizeof(m_szIPAddr));
	memcpy(m_szSubMask, pWizardSheet->m_pWizardLogin->m_cSubMask, sizeof(m_szSubMask));
	memcpy(m_szGateway, pWizardSheet->m_pWizardLogin->m_cGateway, sizeof(m_szGateway));
	m_iWzDhcpEnable = pWizardSheet->m_pWizardLogin->m_iDHCPEnable;

	m_WzCfgIPAddr.SetAddress(m_szIPAddr[0], m_szIPAddr[1], m_szIPAddr[2], m_szIPAddr[3]);
	m_WzCfgSubMask.SetAddress(m_szSubMask[0], m_szSubMask[1], m_szSubMask[2], m_szSubMask[3]);
	if ((m_szGateway[0] != 0) || (m_szGateway[1] != 0) || (m_szGateway[2] != 0) || (m_szGateway[3] != 0))
		m_WzCfgGateway.SetAddress(m_szGateway[0], m_szGateway[1], m_szGateway[2], m_szGateway[3]);
	m_WzCfgIPAddr.SetFieldRange( 0, 1, 223 );
	m_WzCfgGateway.SetFieldRange( 0, 1, 223 );

	UpdateData(false);
	if (m_iWzDhcpEnable)
		OnWzradio();
	else
		OnWzradioDhcpEnable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWizardIpConfig::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);
	
	return CPropertyPageEx::OnSetActive();
}

void CWizardIpConfig::OnFieldchangedWzipaddress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_WzCfgIPAddr.GetAddress(	(unsigned char &)m_szIPAddr[0],
								(unsigned char &)m_szIPAddr[1],
								(unsigned char &)m_szIPAddr[2],
								(unsigned char &)m_szIPAddr[3]);
	
	*pResult = 0;
}

void CWizardIpConfig::OnFieldchangedWzsubMask(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_WzCfgSubMask.GetAddress(	(unsigned char &)m_szSubMask[0],
								(unsigned char &)m_szSubMask[1],
								(unsigned char &)m_szSubMask[2],
								(unsigned char &)m_szSubMask[3]);
	
	*pResult = 0;
}

void CWizardIpConfig::OnFieldchangedWzgateway(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_WzCfgGateway.GetAddress(	(unsigned char &)m_szGateway[0],
								(unsigned char &)m_szGateway[1],
								(unsigned char &)m_szGateway[2],
								(unsigned char &)m_szGateway[3]);
	
	*pResult = 0;
}

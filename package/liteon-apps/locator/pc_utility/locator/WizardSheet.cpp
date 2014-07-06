// WizardSheet.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSheet.h"
#include "UpgraderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardSheet

IMPLEMENT_DYNAMIC(CWizardSheet, CPropertySheetEx)

CWizardSheet::CWizardSheet(CWnd* pParent, UINT iSelectPage,HBITMAP hbmWatermark,HPALETTE hpalWatermark,HBITMAP hbmHeader)
	:CPropertySheetEx(IDS_PROPSHT_CAPTION, pParent, iSelectPage, NULL, NULL, hbmHeader)
{
	m_pWizardLogin				= new CWizardLogin(pParent, this);
	m_pWizardIpConfig			= new CWizardIpConfig(pParent, this);
	m_pWizardSysDevName			= new CWizardSysDevName(pParent, this);
	m_pWizardSystem				= new CWizardSystem(pParent, this);
	m_pWizardSysAp				= new CWizardSysAp(pParent, this);
	m_pWizardSysClient			= new CWizardSysClient(pParent, this);
	m_pWizardSysClientInf		= new CWizardSysClientInf(pParent, this);
	m_pWizardSysClientAdhoc		= new CWizardSysClientAdhoc(pParent, this);
	m_pWizardSysRepeater		= new CWizardSysRepeater(pParent, this);
	m_pWizardSecurity			= new CWizardSecurity(pParent, this);
	m_pWizardSecurityRepeater	= new CWizardSecurityRepeater(pParent, this);
	m_pWizardEncWep				= new CWizardEncWep(pParent, this);
	m_pWizardEncWpapsk			= new CWizardEncWpapsk(pParent, this);
	m_pWizardEncWpa				= new CWizardEncWpa(pParent, this);

	AddPage(m_pWizardLogin);
	AddPage(m_pWizardIpConfig);
	AddPage(m_pWizardSysDevName);
	AddPage(m_pWizardSystem);
	AddPage(m_pWizardSysAp);
	AddPage(m_pWizardSysClient);
	AddPage(m_pWizardSysClientInf);
	AddPage(m_pWizardSysClientAdhoc);
	AddPage(m_pWizardSysRepeater);
	AddPage(m_pWizardSecurity);
	AddPage(m_pWizardSecurityRepeater);
	AddPage(m_pWizardEncWep);
	AddPage(m_pWizardEncWpapsk);
	AddPage(m_pWizardEncWpa);

	m_psh.dwFlags&=(~PSH_HASHELP);
	m_psh.dwFlags|=PSH_WIZARD97|PSH_WIZARDCONTEXTHELP;
	m_psh.dwFlags|=PSH_WIZARDHASFINISH;
}

//CWizardSheet::CPropertySheetEx(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
//	:CPropertySheetEx(pszCaption, pParentWnd, iSelectPage)
//{
//}

CWizardSheet::~CWizardSheet()
{
}


BEGIN_MESSAGE_MAP(CWizardSheet, CPropertySheetEx)
	//{{AFX_MSG_MAP(CWizardSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSheet message handlers

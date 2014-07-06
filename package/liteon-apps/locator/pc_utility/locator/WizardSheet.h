#if !defined(AFX_WIZARDSHEET_H__E5C09AE6_10C9_4754_9D4F_3A0C383B3AD7__INCLUDED_)
#define AFX_WIZARDSHEET_H__E5C09AE6_10C9_4754_9D4F_3A0C383B3AD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSheet.h : header file
//
#if !defined(__GLOBAL_H)
	#include "global.h"
#endif
#include "WizardLogin.h"
#include "WizardIpConfig.h"
#include "WizardSysDevName.h"
#include "WizardSystem.h"
#include "WizardSysAp.h"
#include "WizardSysClient.h"
#include "WizardSysClientInf.h"
#include "WizardSysClientAdhoc.h"
#include "WizardSysRepeater.h"
#include "WizardSecurity.h"
#include "WizardSecurityRepeater.h"
#include "WizardEncWep.h"
#include "WizardEncWpapsk.h"
#include "WizardEncWpa.h"
#include "WizardSummary.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSheet

class CWizardSheet : public CPropertySheetEx
{
	DECLARE_DYNAMIC(CWizardSheet)

// Construction
public:
	char	SerMacAdde[6];
	CWizardSheet(CWnd* pParent, UINT iSelectPage = 0,HBITMAP hbmWatermark=NULL,HPALETTE hpalWatermark=NULL,HBITMAP hbmHeader=NULL);
//	CWizardSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CWizardLogin*				m_pWizardLogin;
	CWizardIpConfig*			m_pWizardIpConfig;
	CWizardSysDevName*			m_pWizardSysDevName;
	CWizardSystem*				m_pWizardSystem;
	CWizardSysAp*				m_pWizardSysAp;
	CWizardSysClient*			m_pWizardSysClient;
	CWizardSysClientInf*		m_pWizardSysClientInf;
	CWizardSysClientAdhoc*		m_pWizardSysClientAdhoc;
	CWizardSysRepeater*			m_pWizardSysRepeater;
	CWizardSecurity*			m_pWizardSecurity;
	CWizardSecurityRepeater*	m_pWizardSecurityRepeater;
	CWizardEncWep*				m_pWizardEncWep;
	CWizardEncWpapsk*			m_pWizardEncWpapsk;
	CWizardEncWpa*				m_pWizardEncWpa;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWizardSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWizardSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSHEET_H__E5C09AE6_10C9_4754_9D4F_3A0C383B3AD7__INCLUDED_)

#if !defined(AFX_WIZARDSYSDEVNAME_H__7523253F_DD8B_43F8_9A08_D5FA22CA7086__INCLUDED_)
#define AFX_WIZARDSYSDEVNAME_H__7523253F_DD8B_43F8_9A08_D5FA22CA7086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSysDevName.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSysDevName dialog

class CWizardSysDevName : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSysDevName)

// Construction
public:
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSysDevName(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSysDevName();

// Dialog Data
	//{{AFX_DATA(CWizardSysDevName)
	enum { IDD = IDD_WIZARD_SYSTEM_DECNAME };
	CString	m_szWzSysName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSysDevName)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardSysDevName)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeWzsysName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSYSDEVNAME_H__7523253F_DD8B_43F8_9A08_D5FA22CA7086__INCLUDED_)

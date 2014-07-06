#if !defined(AFX_WIZARDSYSREPEATER_H__47BC206C_F950_48A7_8334_102B916183F9__INCLUDED_)
#define AFX_WIZARDSYSREPEATER_H__47BC206C_F950_48A7_8334_102B916183F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSysRepeater.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSysRepeater dialog

class CWizardSysRepeater : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSysRepeater)

// Construction
public:
	int m_iWzReapWirelessModeReal;
	int m_iWzReapSuperG;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSysRepeater(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSysRepeater();

// Dialog Data
	//{{AFX_DATA(CWizardSysRepeater)
	enum { IDD = IDD_WIZARD_SYSTEM_REPEATER };
	CString	m_szWzSsidRepeater;
	int		m_iWzReapWirelessMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSysRepeater)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardSysRepeater)
	afx_msg void OnChangeWzssidRepeater();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWzwirelessModeRepeater();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSYSREPEATER_H__47BC206C_F950_48A7_8334_102B916183F9__INCLUDED_)

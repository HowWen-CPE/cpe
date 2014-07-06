#if !defined(AFX_WIZARDSYSAP_H__60E90278_267F_4AB9_80B4_E207D6CF1957__INCLUDED_)
#define AFX_WIZARDSYSAP_H__60E90278_267F_4AB9_80B4_E207D6CF1957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSysAp.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSysAp dialog

class CWizardSysAp : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSysAp)

// Construction
public:
	int m_iWzApWirelessModeReal;
	int preWzChannelAp;
	int m_iWzApSuperG;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSysAp(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSysAp();

// Dialog Data
	//{{AFX_DATA(CWizardSysAp)
	enum { IDD = IDD_WIZARD_SYSTEM_AP };
	CString	m_szWzSsidAp;
	int		m_iWzChannelAp;
	int		m_iWzApWirelessMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSysAp)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardSysAp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWzchannelAp();
	afx_msg void OnSelchangeComboWzwlanMode();
	afx_msg void OnChangeWzssidAp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSYSAP_H__60E90278_267F_4AB9_80B4_E207D6CF1957__INCLUDED_)

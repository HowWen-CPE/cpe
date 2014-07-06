#if !defined(AFX_WIZARDENCWPA_H__69BCCE9B_2DCE_4C86_9605_119918264C7E__INCLUDED_)
#define AFX_WIZARDENCWPA_H__69BCCE9B_2DCE_4C86_9605_119918264C7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardEncWpa.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardEncWpa dialog

class CWizardEncWpa : public CNGWizardPage
{
	DECLARE_DYNCREATE(CWizardEncWpa)

// Construction
public:
	CWnd*	m_pGrandFather;
	CWnd*	m_pParentSheet;
	CWizardEncWpa(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardEncWpa();

// Dialog Data
	//{{AFX_DATA(CWizardEncWpa)
	enum { IDD = IDD_WIZARD_ENC_WPA };
	CIPAddressCtrl	m_ctrlRadiusServer;
	int		m_iWzEncryptionWpa;
	CString	m_szWzKeyWpa;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardEncWpa)
//	public:
//	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardEncWpa)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWzWpaCipher();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDENCWPA_H__69BCCE9B_2DCE_4C86_9605_119918264C7E__INCLUDED_)

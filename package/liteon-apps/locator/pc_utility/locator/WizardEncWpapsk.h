#if !defined(AFX_WIZARDENCWPAPSK_H__3FEC31F9_57D1_433B_BE54_6C068643D606__INCLUDED_)
#define AFX_WIZARDENCWPAPSK_H__3FEC31F9_57D1_433B_BE54_6C068643D606__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardEncWpapsk.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardEncWpapsk dialog

class CWizardEncWpapsk : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardEncWpapsk)

// Construction
public:
	char	m_cWzKeyWpapsk[64];
	CWnd*	m_pGrandFather;
	CWnd*	m_pParentSheet;
	CWizardEncWpapsk(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardEncWpapsk();

// Dialog Data
	//{{AFX_DATA(CWizardEncWpapsk)
	enum { IDD = IDD_WIZARD_ENC_WPAPSK };
	CString	m_szWzKeyWpapsk;
	int		m_iWzEncryption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardEncWpapsk)
	public:
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardEncWpapsk)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboWzencryption();
	afx_msg void OnChangeEditWzkeyWpapsk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDENCWPAPSK_H__3FEC31F9_57D1_433B_BE54_6C068643D606__INCLUDED_)

#if !defined(AFX_WIZARDSECURITY_H__B84FA5E9_C865_4788_8DCD_C88E91540973__INCLUDED_)
#define AFX_WIZARDSECURITY_H__B84FA5E9_C865_4788_8DCD_C88E91540973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSecurity.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"
#include "EditLimit.h"
/////////////////////////////////////////////////////////////////////////////
// CWizardSecurity dialog

class CWizardSecurity : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSecurity)

// Construction
public:
	int m_iWzAuthReal;
//	int m_iWzEncryptionReal;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSecurity(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSecurity();

// Dialog Data
	//{{AFX_DATA(CWizardSecurity)
	enum { IDD = IDD_WIZARD_SECURITY };
	int		m_iWzAuth;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSecurity)
	public:
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardSecurity)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioWzauth();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSECURITY_H__B84FA5E9_C865_4788_8DCD_C88E91540973__INCLUDED_)

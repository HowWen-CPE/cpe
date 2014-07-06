#if !defined(AFX_WIZARDSECURITYREPEATER_H__A3CBC96F_A470_4CC5_ADE2_E02D76D349C1__INCLUDED_)
#define AFX_WIZARDSECURITYREPEATER_H__A3CBC96F_A470_4CC5_ADE2_E02D76D349C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSecurityRepeater.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSecurityRepeater dialog

class CWizardSecurityRepeater : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSecurityRepeater)

// Construction
public:
	int m_iWzAuthRepeaterReal;
//	int m_iWzEncryptionRepeaterReal;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSecurityRepeater(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSecurityRepeater();

// Dialog Data
	//{{AFX_DATA(CWizardSecurityRepeater)
	enum { IDD = IDD_WIZARD_SECURITY_REPEATER };
	int		m_iWzAuthRepeater;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSecurityRepeater)
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
	//{{AFX_MSG(CWizardSecurityRepeater)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioWzauthRepeater();
	afx_msg void OnRadio2Repeater();
	afx_msg void OnRadio3Repeater();
	afx_msg void OnRadio4Repeater();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSECURITYREPEATER_H__A3CBC96F_A470_4CC5_ADE2_E02D76D349C1__INCLUDED_)

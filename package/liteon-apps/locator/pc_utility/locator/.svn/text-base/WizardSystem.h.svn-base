#if !defined(AFX_WIZARDSYSTEM_H__59F74ACF_E5A4_4029_9F07_B1AB71774682__INCLUDED_)
#define AFX_WIZARDSYSTEM_H__59F74ACF_E5A4_4029_9F07_B1AB71774682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSystem.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CWizardSystem dialog

class CWizardSystem : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSystem)

// Construction
public:
	int m_iWzOpModeReal;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSystem(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSystem();

// Dialog Data
	//{{AFX_DATA(CWizardSystem)
	enum { IDD = IDD_WIZARD_SYSTEM };
	int		m_iWzOpMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSystem)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardSystem)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioWzap();
	afx_msg void OnRadioWzclient();
	afx_msg void OnRadioWzrepeater();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSYSTEM_H__59F74ACF_E5A4_4029_9F07_B1AB71774682__INCLUDED_)

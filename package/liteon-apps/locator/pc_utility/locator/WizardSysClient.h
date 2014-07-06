#if !defined(AFX_WIZARDSYSCLIENT_H__252F46D2_A3E5_48E5_88E9_83F54B1F16BE__INCLUDED_)
#define AFX_WIZARDSYSCLIENT_H__252F46D2_A3E5_48E5_88E9_83F54B1F16BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSysClient.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClient dialog

class CWizardSysClient : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSysClient)

// Construction
public:
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSysClient(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSysClient();

// Dialog Data
	//{{AFX_DATA(CWizardSysClient)
	enum { IDD = IDD_WIZARD_SYSTEM_CLIENT };
	int		m_iWzClientMode;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSysClient)
	public:
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardSysClient)
	afx_msg void OnRadioClientMode();
	afx_msg void OnRadio2ClientMode();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSYSCLIENT_H__252F46D2_A3E5_48E5_88E9_83F54B1F16BE__INCLUDED_)

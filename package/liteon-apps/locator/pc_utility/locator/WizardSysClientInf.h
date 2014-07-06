#if !defined(AFX_WIZARDSYSCLIENTINF_H__E7362A3C_EE6D_4578_967B_60C172477AFD__INCLUDED_)
#define AFX_WIZARDSYSCLIENTINF_H__E7362A3C_EE6D_4578_967B_60C172477AFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSysClientInf.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClientInf dialog

class CWizardSysClientInf : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSysClientInf)

// Construction
public:
	CString	preWzSsidClientInf;
	int m_iWzInfWirelessModeReal;
	int m_iWzInfSuperG;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSysClientInf(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSysClientInf();

// Dialog Data
	//{{AFX_DATA(CWizardSysClientInf)
	enum { IDD = IDD_WIZARD_SYSTEM_CLIENTINF };
	CString	m_szWzSsidClientInf;
	int		m_iWzAnySsidEnable;
	int		m_iWzInfWirelessMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSysClientInf)
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
	//{{AFX_MSG(CWizardSysClientInf)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioAnySsid();
	afx_msg void OnRadioStaticSsid();
	afx_msg void OnChangeWzssidClientInf();
	afx_msg void OnSelchangeWzwirelessModeInf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSYSCLIENTINF_H__E7362A3C_EE6D_4578_967B_60C172477AFD__INCLUDED_)

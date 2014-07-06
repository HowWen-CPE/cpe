#if !defined(AFX_WIZARDSYSCLIENTADHOC_H__F10C8C94_94F1_435B_B43B_E03D9008404D__INCLUDED_)
#define AFX_WIZARDSYSCLIENTADHOC_H__F10C8C94_94F1_435B_B43B_E03D9008404D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSysClientAdhoc.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardSysClientAdhoc dialog

class CWizardSysClientAdhoc : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardSysClientAdhoc)

// Construction
public:
	int preWzChannelClientAdhoc;
	int m_iWzAdhocWirelessModeReal;
	int m_iWzAdhocSuperG;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSysClientAdhoc(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardSysClientAdhoc();

// Dialog Data
	//{{AFX_DATA(CWizardSysClientAdhoc)
	enum { IDD = IDD_WIZARD_SYSTEM_CLIENTADHOC };
	CString	m_szWzSsidClientAdhoc;
	int		m_iWzChannelClientAdhoc;
	int		m_iWzAdhocWirelessMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardSysClientAdhoc)
	public:
	virtual LRESULT OnWizardBack();
	virtual BOOL OnWizardFinish();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardSysClientAdhoc)
	afx_msg void OnChangeWzssidClientAdhoc();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWzchannelClientAdhoc();
	afx_msg void OnSelchangeComboWzwlanModeAdhoc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSYSCLIENTADHOC_H__F10C8C94_94F1_435B_B43B_E03D9008404D__INCLUDED_)

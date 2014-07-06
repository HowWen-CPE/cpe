#if !defined(AFX_WIZARDWPA_H__43F69634_7737_421F_8652_8376F8815FD7__INCLUDED_)
#define AFX_WIZARDWPA_H__43F69634_7737_421F_8652_8376F8815FD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardWpa.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"

/////////////////////////////////////////////////////////////////////////////
// CWizardWpa dialog

class CWizardWpa : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardWpa)

// Construction
public:
	char m_cRadiusServer[4];
	int m_iWzEncryptionWpaReal;
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardWpa(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardWpa();

// Dialog Data
	//{{AFX_DATA(CWizardWpa)
	enum { IDD = IDD_WIZARD_ENCWPA };
	CIPAddressCtrl	m_ctrlRadiusServer;
	CString	m_szWzKeyWpa;
	int		m_iWzEncryptionWpa;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardWpa)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardBack();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardWpa)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendcancelWzWpaCipher();
	afx_msg void OnChangeEditWzkeyWpa();
	afx_msg void OnFieldchangedWzAdiusServer(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDWPA_H__43F69634_7737_421F_8652_8376F8815FD7__INCLUDED_)

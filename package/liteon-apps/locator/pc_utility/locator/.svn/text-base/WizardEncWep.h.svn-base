#if !defined(AFX_WIZARDENCWEP_H__6A11E4F8_AF6F_441D_AA03_E74B4E8BB27A__INCLUDED_)
#define AFX_WIZARDENCWEP_H__6A11E4F8_AF6F_441D_AA03_E74B4E8BB27A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardEncWep.h : header file
//
#include "EditLimit.h"
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
/////////////////////////////////////////////////////////////////////////////
// CWizardEncWep dialog

class CWizardEncWep : public CNGWizardPage//CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizardEncWep)

// Construction
public:
	int m_iWzCiperTypeReal;
	void SetDlgItemPosition(int nID, int TopShift, int LeftShift);
	char m_cWzKeyAscii[17];
	char m_cWzKeyHex[33];
	int m_iPrepageWepMode;
	CWnd*	m_pGrandFather;
	CWnd*	m_pParentSheet;
	CWizardEncWep(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardEncWep();

// Dialog Data
	//{{AFX_DATA(CWizardEncWep)
	enum { IDD = IDD_WIZARD_ENC_WEP };
	CString	m_szWzKeyHex;
	CString	m_szWzKeyAscii;
	int		m_iRadioWzKeyType;
	int		m_iWzCiperType;
	//}}AFX_DATA
	CEditLimit	m_cWzHexCtrl;
	CEdit		m_cWzAsciiCtrl;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardEncWep)
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
	//{{AFX_MSG(CWizardEncWep)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioWzkeyHex();
	afx_msg void OnRadioWzkeyAscii();
	afx_msg void OnChangeEditWzkeyHex();
	afx_msg void OnChangeEditWzkeyAscii();
	afx_msg void OnSelchangeCipherType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDENCWEP_H__6A11E4F8_AF6F_441D_AA03_E74B4E8BB27A__INCLUDED_)

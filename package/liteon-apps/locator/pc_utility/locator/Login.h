#if !defined(AFX_LOGIN_H__9B141CD3_8A66_4C73_988F_26B37A439766__INCLUDED_)
#define AFX_LOGIN_H__9B141CD3_8A66_4C73_988F_26B37A439766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//
#include "BitmapDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog

class CLogin : public CBitmapDialog
{
// Construction
public:
	CLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogin)
	enum { IDD = IDD_LOGIN };
	CString	m_sPassword;
	CString	m_sUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

//	CBrush			m_brush;
	// Generated message map functions
	//{{AFX_MSG(CLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnStnClickedStaticDefaultUser();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__9B141CD3_8A66_4C73_988F_26B37A439766__INCLUDED_)

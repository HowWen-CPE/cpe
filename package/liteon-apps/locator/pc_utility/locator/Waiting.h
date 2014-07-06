#if !defined(AFX_WAITING_H__5922AC01_545A_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_WAITING_H__5922AC01_545A_11D2_BC1F_0080C868BB53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Waiting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaiting dialog

class CWaiting : public CDialog
{
// Construction
public:
	CWaiting(CWnd* pParent = NULL);   // standard constructor
	void	Close();
// Dialog Data
	//{{AFX_DATA(CWaiting)
	enum { IDD = IDD_SEARCH_WAIT };
	CAnimateCtrl	m_ctlAnimate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaiting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaiting)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITING_H__5922AC01_545A_11D2_BC1F_0080C868BB53__INCLUDED_)

#if !defined(AFX_WAITRESET_H__2A610239_1774_4E8A_80DF_C24E6DE8EAED__INCLUDED_)
#define AFX_WAITRESET_H__2A610239_1774_4E8A_80DF_C24E6DE8EAED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitReset.h : header file
//
#include "GradientProgCtrl.h"

#define TIMER10 240
#define TIMER11 241
#define MAX_TIME_WAITING      20000 //ms

/////////////////////////////////////////////////////////////////////////////
// CWaitReset dialog

class CWaitReset : public CDialog
{
// Construction
public:
	CWaitReset(ULONG uTime, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaitReset)
	enum { IDD = IDD_WAIT_RESET };
	CGradientProgCtrl	m_ctrlProgress;
	//}}AFX_DATA
private:
	CString m_strPreMac;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitReset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	ULONG m_ulTimeOut;
	ULONG m_uMaxWaitTime;	//等待时间 单位：ms
	// Generated message map functions
	//{{AFX_MSG(CWaitReset)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITRESET_H__2A610239_1774_4E8A_80DF_C24E6DE8EAED__INCLUDED_)

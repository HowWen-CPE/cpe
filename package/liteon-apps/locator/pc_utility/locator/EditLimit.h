#if !defined(AFX_EDITLIMIT_H__A74A3617_18C0_4409_923B_BD6A02C53C0C__INCLUDED_)
#define AFX_EDITLIMIT_H__A74A3617_18C0_4409_923B_BD6A02C53C0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLimit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditLimit window

class CEditLimit : public CEdit
{
// Construction
public:
	CEditLimit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditLimit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditLimit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditLimit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLIMIT_H__A74A3617_18C0_4409_923B_BD6A02C53C0C__INCLUDED_)

#if !defined(AFX_MYLISTCTRL_H__AEADDA97_4533_4C2E_AD1E_9DBCEDD11C4A__INCLUDED_)
#define AFX_MYLISTCTRL_H__AEADDA97_4533_4C2E_AD1E_9DBCEDD11C4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window

class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:
protected:
	int  m_nHighlight;		// Indicate type of selection highlighting
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	enum EHighlight {HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW};
	virtual ~CMyListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int SetHighlightType(EHighlight hilite);
	void RepaintSelectedItems();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__AEADDA97_4533_4C2E_AD1E_9DBCEDD11C4A__INCLUDED_)

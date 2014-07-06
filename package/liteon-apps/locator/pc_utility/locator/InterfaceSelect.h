#if !defined(AFX_INTERFACESELECT_H__CA381D7A_D4DD_4DD7_AF88_E710CA03CDA5__INCLUDED_)
#define AFX_INTERFACESELECT_H__CA381D7A_D4DD_4DD7_AF88_E710CA03CDA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterfaceSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInterfaceSelect dialog

class CInterfaceSelect : public CDialog
{
// Construction
public:
	char LocalHostIP[225];
	CWnd* m_pParent;
	CInterfaceSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInterfaceSelect)
	enum { IDD = IDD_INTERFACE_SEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfaceSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInterfaceSelect)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERFACESELECT_H__CA381D7A_D4DD_4DD7_AF88_E710CA03CDA5__INCLUDED_)

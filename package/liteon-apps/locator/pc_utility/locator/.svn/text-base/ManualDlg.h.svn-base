#if !defined(AFX_MANUALDLG_H__07DAE9E0_5480_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_MANUALDLG_H__07DAE9E0_5480_11D2_BC1F_0080C868BB53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManualDlg dialog

class CManualDlg : public CDialog
{
// Construction
public:
	CManualDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualDlg)
	enum { IDD = IDD_UPGRADE_MANUAL };
	CIPAddressCtrl	m_ctlIpAddr;
	CString	m_strPassword;
	CString	m_strFilePath;
	//}}AFX_DATA
public:
	CString		m_strIpAddr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManualDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowseButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALDLG_H__07DAE9E0_5480_11D2_BC1F_0080C868BB53__INCLUDED_)

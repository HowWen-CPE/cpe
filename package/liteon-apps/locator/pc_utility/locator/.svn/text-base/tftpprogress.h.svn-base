#if !defined(AFX_TFTPPROGESS_H__7B4DF882_538C_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_TFTPPROGESS_H__7B4DF882_538C_11D2_BC1F_0080C868BB53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TftpProgess.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTftpProgress dialog

class CTftpProgress : public CDialog
{
// Construction
public:
	CTftpProgress(CWnd* pParent = NULL);   // standard constructor
	void Close();

// Dialog Data
	//{{AFX_DATA(CTftpProgress)
	enum { IDD = IDD_TFTP_PROGESS };
	CProgressCtrl	m_ctlProgress;
	//}}AFX_DATA

public:
	int ProcessPos;
	int TFTPput(char *host, char *file, char *passwd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTftpProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTftpProgress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TFTPPROGESS_H__7B4DF882_538C_11D2_BC1F_0080C868BB53__INCLUDED_)

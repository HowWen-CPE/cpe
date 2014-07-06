#if !defined(AFX_WIZARDLOGIN_H__0C5903D0_6704_4393_893D_75B60617BCEA__INCLUDED_)
#define AFX_WIZARDLOGIN_H__0C5903D0_6704_4393_893D_75B60617BCEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardLogin.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CWizardLogin dialog

class CWizardLogin : public CNGWizardPage//CPropertyPageEx
{
	DECLARE_DYNCREATE(CWizardLogin)

// Construction
public:
	int m_iSuperG;
	int m_iWirelessMode;
	int m_iOpMode;
	int m_iChannel;
	int m_iAuth;
	int m_iEncryption;
	int m_iEncryptionOnOff;
	int m_iWepKeyType;
	char m_cSSID[33];
	char m_cDevName[33];
	char m_cIPAddr[6];
	char m_cSubMask[4];
	char m_cGateway[4];
	char m_cRadiusServer[16];
	int	m_iDHCPEnable;
	int m_WepInputType;
	ULONG uAuthFail;
	ULONG uDiscover;
	void passRecv();
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardLogin(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardLogin();

// Dialog Data
	//{{AFX_DATA(CWizardLogin)
	enum { IDD = IDD_WIZARD_LOGIN };
	CString	m_szUserName;
	CString	m_szPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardLogin)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeWizardUsername();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDLOGIN_H__0C5903D0_6704_4393_893D_75B60617BCEA__INCLUDED_)

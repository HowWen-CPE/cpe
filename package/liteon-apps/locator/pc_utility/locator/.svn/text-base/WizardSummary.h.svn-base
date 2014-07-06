#if !defined(AFX_WIZARDSUMMARY_H__98346DD6_EFAF_4FF3_93CE_C4032FE407A7__INCLUDED_)
#define AFX_WIZARDSUMMARY_H__98346DD6_EFAF_4FF3_93CE_C4032FE407A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardSummary.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
#include "BitmapDialog.h"

//# define BITMAP_TILE	0 
//# define BITMAP_CENTER	1 

/////////////////////////////////////////////////////////////////////////////
// CWizardSummary dialog

class CWizardSummary : public CBitmapDialog
{
// Construction
public:
//	void SetBitmap (UINT ResID,int Type=BITMAP_TILE);
	int m_iAuthTemp;
	int iCurrentDHCPEnable;
	bool b_IpCahnged;
	bool b_MaskCahnged;
	bool b_GatewayCahnged;
	bool b_DevNameCahnged;
	bool b_OpModeCahnged;
	bool b_SsidCahnged;
	bool b_ChannelCahnged;
	bool b_WlanModeCahnged;
	bool b_AuthCahnged;
	bool b_EncryCahnged;
	int m_iSecurity;
	int m_iKeyStrType;
	int m_iEncryption;
	int m_iAuth;
	bool m_bSecurityEnable;
	int s_iSuperG;
	int s_iWirelessMode;
	int s_iOpMode;
	int s_iChannel;
	char s_szSSID[33];
	char s_szDevName[33];
	char s_szIPAddr[4];
	char s_szSubMask[4];
	char s_szGateway[4];
	char s_szKey1[64];
	CString s_szRadiusServer;
	int	s_iDHCPEnable;
	void Ip2String(CString *outputStr, char* inputIp);
	CWnd* m_pGrandFather;
	CWnd* m_pParentSheet;
	CWizardSummary(CWnd* pGrandFather = NULL, CWnd* pParent = NULL, bool SecurityEnable = false);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizardSummary)
	enum { IDD = IDD_WIZARD_SUMMARY };
	CString	m_szAuthOld;
	CString	m_szChannelOld;
	CString	m_szDeviceNameOld;
	CString	m_szEncryptionOld;
	CString	m_szGatewayOld;
	CString	m_szIpOld;
	CString	m_szSubMaskOld;
	CString	m_szOpModeOld;
	CString	m_szSsidOld;
	CString	m_szWModeOld;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardSummary)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWizardSummary)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//	int		mType;
//	CBrush	mHollowBrush;
//	CBitmap	mBitmap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDSUMMARY_H__98346DD6_EFAF_4FF3_93CE_C4032FE407A7__INCLUDED_)

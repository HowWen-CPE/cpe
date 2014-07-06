#if !defined(AFX_WIZARDIPCONFIG_H__164DB24C_29CC_4864_B46D_3B85B15F5826__INCLUDED_)
#define AFX_WIZARDIPCONFIG_H__164DB24C_29CC_4864_B46D_3B85B15F5826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardIpConfig.h : header file
//
#include "ResizeCtrl.h"
#include "NGWizardPage.h"
//#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CWizardIpConfig dialog

class CWizardIpConfig : public CNGWizardPage/*CPropertyPageEx*/
{
	DECLARE_DYNCREATE(CWizardIpConfig)

// Construction
public:
	char m_szIPAddr[4];
	char m_szSubMask[4];
	char m_szGateway[4];
	CWnd*	m_pGrandFather;
	CWnd*	m_pParentSheet;
	CWizardIpConfig(CWnd* pGrandFather = NULL, CWnd* pParent = NULL);
	~CWizardIpConfig();

// Dialog Data
	//{{AFX_DATA(CWizardIpConfig)
	enum { IDD = IDD_WIZARD_IP_CONFIG };
	CIPAddressCtrl	m_WzCfgSubMask;
	CIPAddressCtrl	m_WzCfgIPAddr;
	CIPAddressCtrl	m_WzCfgGateway;
	int		m_iWzDhcpEnable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardIpConfig)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardIpConfig)
	afx_msg void OnWzradioDhcpEnable();
	afx_msg void OnWzradio();
	virtual BOOL OnInitDialog();
	afx_msg void OnFieldchangedWzipaddress(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldchangedWzsubMask(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldchangedWzgateway(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDIPCONFIG_H__164DB24C_29CC_4864_B46D_3B85B15F5826__INCLUDED_)

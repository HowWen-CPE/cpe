#if !defined(AFX_WIRELESS_H__D2DBCE12_4E46_4928_9398_0E908F1AA684__INCLUDED_)
#define AFX_WIRELESS_H__D2DBCE12_4E46_4928_9398_0E908F1AA684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wireless.h : header file
//
#include "EditLimit.h"
#include "BitmapDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CWireless dialog

class CWireless : public CBitmapDialog
{
// Construction
public:
//	CString m_preSsid;
	int m_preChannel;
//	int m_preOpMode;
//	int m_preSecurity;
//	int m_preAuth;
//	int m_preWirelessMode;
	int m_iWirelessModeReal;
	int m_iSuperG;
	int m_iOpModeReal;
	int m_iAuthReal;
	int m_iEncryptionReal;
	int m_oldChannel;
	void GetLocalIPs(char *localips);
	CWireless(CWnd* pParent = NULL);   // standard constructor
	SOCKET		m_Socket;
	SOCKADDR_IN	saUdpServ, saUdpCli;
	char SerMacAdde[6];
	char LocalMacAddr[6];
	char RadiusServerinwireless[16];
	char m_cRadiusServer[4];
	CString s_szRadiusServer;
	void Ip2String(CString *outputStr, char *inputIp);
// Dialog Data
	//{{AFX_DATA(CWireless)
	enum { IDD = IDD_WLAN_SETTING };
	int		m_iChannel;
	int		m_iEncryption;
	CString	m_sKey;
	CString	m_hKey;
	int		m_iSecurity;
	CString	m_sSsid;
	int		m_KeyHex;
	int		m_iOpMode;
	int		m_iAuth;
	int		m_iWirelessMode;

	CIPAddressCtrl	m_ctrlRadiusServer;
	//}}AFX_DATA
	CEditLimit	m_cHexCtrl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWireless)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddComboString(CComboBox* pCombo, unsigned int IDS_xxx);

	CBrush			m_brush;
	// Generated message map functions
	//{{AFX_MSG(CWireless)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSecurity();
	afx_msg void OnKeyHex();
	afx_msg void OnKeyAscii();
	afx_msg void OnChangeKey();
	afx_msg void OnChangeKey2();
	virtual void OnOK();
	afx_msg void OnChangeSsid();
	afx_msg void OnSelchangeChannel();
	afx_msg void OnDropdownChannel();
	afx_msg void OnSelchangeOpMode();
	afx_msg void OnSelchangeComboAuth();
	afx_msg void OnSelchangeEncryption();
	afx_msg void OnSelchangeWirelessMode();
	afx_msg void OnFieldchangedWzAdiusServer(NMHDR* nmhdr, LRESULT* lresult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIRELESS_H__D2DBCE12_4E46_4928_9398_0E908F1AA684__INCLUDED_)

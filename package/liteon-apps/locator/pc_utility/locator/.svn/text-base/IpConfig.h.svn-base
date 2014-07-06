#if !defined(AFX_IPCONFIG_H__F0A03E83_7786_48D4_8E7D_92C9F342BB68__INCLUDED_)
#define AFX_IPCONFIG_H__F0A03E83_7786_48D4_8E7D_92C9F342BB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IpConfig.h : header file
//
#include "BitmapDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CIpConfig dialog

class CIpConfig : public CBitmapDialog
{
public:
typedef enum
{
	IP_OK = 0,
	IP_MASK_ERR,
	IP_FIELD4_ZERO_ERR,
	IP_FIELD4_FILL_ERR,
	IP_FIELD1_ERR
}CHECK_IP_RESULT;

// Construction
public:
	CWnd* m_pParent;
	int preDhcpEnable;
	UCHAR preGetIP[4];
	UCHAR preGateway[4];
	UCHAR preGetSubMask[4];
	enum CHECK_IP_RESULT CheckValidIPAndMask();
	void ReceiveSetIPData();
	char SerMacAdde[6];
	char LocalMacAddr[6];
	void GetLocalIPs(char *localips);
	UCHAR m_chrGateway[4];
	UCHAR m_oldGateway[4];
	UCHAR m_chrGetSubMask[4];
	UCHAR m_chrGetIP[4];
	UCHAR m_oldGetIP[4];
	SOCKET		m_Socket;
	SOCKADDR_IN	saUdpServ, saUdpCli;
	CIpConfig(CWnd* pParent = NULL);   // standard constructor
	int m_bridge;

// Dialog Data
	//{{AFX_DATA(CIpConfig)
	enum { IDD = IDD_IP_CONFIG };
	CIPAddressCtrl	m_CfgSubMask;
	CIPAddressCtrl	m_CfgGateway;
	CIPAddressCtrl	m_CfgIPAddr;
//	BOOL	m_bDhcpEnable;
	int		m_iDhcpEnable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

CBrush			m_brush;
	// Generated message map functions
	//{{AFX_MSG(CIpConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDhcpenable();
	afx_msg void OnRadioDhcpEnable();
	afx_msg void OnRadioDhcp();
	afx_msg void OnFieldchangedIpaddress(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldchangedGateway(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnIpnFieldchangedSubMask(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCONFIG_H__F0A03E83_7786_48D4_8E7D_92C9F342BB68__INCLUDED_)

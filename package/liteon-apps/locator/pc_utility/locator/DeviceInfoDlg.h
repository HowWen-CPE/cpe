#if !defined(AFX_DEVICEINFODLG_H__07DAE9E2_5480_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_DEVICEINFODLG_H__07DAE9E2_5480_11D2_BC1F_0080C868BB53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceInfoDlg.h : header file
//
#include "BitmapDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDeviceInfoDlg dialog

class CDeviceInfoDlg : public CBitmapDialog
{
// Construction
public:
	CWnd* m_pParent;
	CDeviceInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeviceInfoDlg)
	enum { IDD = IDD_DEVICE_INFO };
	CString	m_strFwVer;
	CString	m_strIpAddr;
	CString	m_strMacAddr;
	CString	m_strModelName;
	CString	m_strChannel;
	CString	m_strDhcp;
	CString	m_strGateway;
	CString	m_strSubnetMask;
	CString	m_strOpMode;
	CString	m_strSecurity;
	CString	m_strSsid;
	CString	m_strWlanMode;
	CString	m_strWlanMacAddr;
	CString	m_strAssociated;
	CString	m_strBssid;
	CString	m_strRssi;
	CString	m_str5GWlanMode;
	CString	m_str5GSsid;
	CString	m_str5GRssi;
	CString	m_str5GBssid;
	CString	m_str5GAssociated;
	CString	m_str5GChannel;
	CString	m_str5GSecurity;
	CString	m_str5GWlanMacAddr;
	CString	m_strWlanOpMode;
	CString	m_str5GWlanOpMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	CBrush			m_brush;
	// Generated message map functions
	//{{AFX_MSG(CDeviceInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedChannelStatic();
public:
	afx_msg void OnStnClickedSecutrityStatic2();
public:
	afx_msg void OnStnClickedSecutrityStatic();
public:
	afx_msg void OnStnClickedRssiStatic();
public:
	afx_msg void OnStnClickedOpmodeStatic();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEINFO_H__07DAE9E2_5480_11D2_BC1F_0080C868BB53__INCLUDED_)

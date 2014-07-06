// upgraderView.h : interface of the CUpgraderView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPGRADERVIEW_H__678EEB8E_5132_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_UPGRADERVIEW_H__678EEB8E_5132_11D2_BC1F_0080C868BB53__INCLUDED_

#define UNICAST
#include "Waiting.h"	// Added by ClassView
#if !defined(__GLOBAL_H)
	#include "global.h"
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#define MAXIMUM_LIST 16

#include "upgraderDoc.h"
#include "ListVwEx.h"	// base class for CRowListView

class CUpgraderView : public CListViewEx
{
protected: // create from serialization only
	CUpgraderView();
	virtual ~CUpgraderView();
	DECLARE_DYNCREATE(CUpgraderView)

// Attributes
public:
	CUpgraderDoc* GetDocument();

// Operations
public:
	CImageList	m_ImageList;
	int			m_nRouter;
	int			m_nSwitch;
	int			m_nGateway;
	int        dialFlag[1];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgraderView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetMiniApInfo(void);
	void GetWizardInfo();
	void GetWirelessInfo();
	void SendAuthentication(UINT configType);
	void CreateWinSocket();
	int  HexDigitConvert(char cChar);
	void SetFactoryDefault();
	LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	void SendConfig();
	
	BOOL ResolveIPs(char *hostname, char *hostips, int len);
	BOOL GetLocalName(char *localname, int len);
	UINT HexToUINT(char* pstr);	
	int  HexStrToBin(char* pInStr,char* pOutBinData,int ConvLen);
	void setLocalMacAddress(int& i); //将本机mac地址设置进去

	void StartDiscovery();
	// 组织探测包的数据 David 2010.9.19
	void GetDiscoveryData();

	// 发送重启命令包 David 2010.6.7
	void SendReboot();
	
	// 显示IP配置的界面 David 2010.6.12 Add
	void ShowIPConfigDlg();
	
	// 显示设备信息的界面 David 2010.6.12 Add
	void ShowDeviceInfoDlg();
	//发送获取host信息包
	void SendHost();

	// 接收包之后的工作 David 2010.6.12 Add
	void LaterAction();

public:
	CWaiting *pWaiting;
	CString m_strPassword;
	CString m_strUserName;
	char m_szLocalHostIP[225];
	SOCKET m_Socket;
	int m_packetLength;
	SOCKADDR_IN	m_saUdpServ;	// 设备端
	SOCKADDR_IN m_saUdpCli;		// 控制端
	char m_szRequest[256];
	char m_szSendDataBuf[256];
	int m_sendDiscoverCount;
	char LocalMacAddr[6];

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void GetDevicesInfo();
	void BindToServer(u_long server);
	void ShowHostSetDlg();
	void LV_AddItem(int nIndex, CDeviceInfo *pItem);
// Generated message map functions
protected:
	//{{AFX_MSG(CUpgraderView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSearch();
	afx_msg void OnUpdateSearch(CCmdUI* pCmdUI);
	afx_msg void OnUpgradeManual();
	afx_msg void OnUpdateUpgradeManual(CCmdUI* pCmdUI);
	afx_msg void OnDeviceInfo();
	afx_msg void OnUpdateDeviceInfo(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateConfig(CCmdUI* pCmdUI);
	afx_msg void OnConfig();
	afx_msg void OnWireless();
	afx_msg void OnUpdateWireless(CCmdUI* pCmdUI);
	afx_msg void OnWizard();
	afx_msg void OnUpdateWizard(CCmdUI* pCmdUI);
	afx_msg void OnWeb();
	afx_msg void OnUpdateWeb(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnChangeSocket(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InterfaceSelect();
	afx_msg void OnToolsSelectconnection();
	afx_msg void OnUpdateToolsSelectconnection(CCmdUI *pCmdUI);
	afx_msg void OnReboot();
	afx_msg void OnUpdateReboot(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in upgraderView.cpp
inline CUpgraderDoc* CUpgraderView::GetDocument()
   { return (CUpgraderDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADERVIEW_H__678EEB8E_5132_11D2_BC1F_0080C868BB53__INCLUDED_)

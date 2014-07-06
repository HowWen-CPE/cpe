// upgraderDoc.h : interface of the CUpgraderDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPGRADERDOC_H__678EEB8C_5132_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_UPGRADERDOC_H__678EEB8C_5132_11D2_BC1F_0080C868BB53__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDeviceInfo
{
public:
	CString		strModelName;
	ipaddr		IpAddr;
	CString		strMacAddr;
	CString		strFwVer;
	CString		strPassword;
};

// 取得所有网卡的信息 David 2010.6.2 add
PIP_ADAPTER_INFO GetAllAdaptersInfo();

class CUpgraderDoc : public CDocument
{
protected: // create from serialization only
	CUpgraderDoc();
	DECLARE_DYNCREATE(CUpgraderDoc)

// Attributes
public:
	CList <CDeviceInfo*, CDeviceInfo*>  m_lstDevices;

	PIP_ADAPTER_INFO	m_pAdapterInfo;		// 网卡信息链表
	PIP_ADAPTER_INFO	m_pDefaultAdapter;	// 用户指定的网卡信息
	std::vector< LCT_DEVICE* > m_arrDevice;	// 设备数组
	LCT_PACKAEGDATA	   m_RecieveData;		// 收到的服务端的回复的数据
	char m_szLocalHostIP[225];
// Operations
public:
	void FreeList();

	// 清除设备信息内存 David 2010.6.11 Add
	void ClearDevice();
	
	// 从socket中把接收的数据转换到本地内存 David 2010.6.11 Add
	int GetDataFromSocket(char* buffer, //[in] 接收socket数据的缓冲的指针
						  CString& strServIP //[in] 从socket中得来的服务端IP
						  );

	// 更新设备信息 David 2010.6.12 Add
	void UpdateDeviceInfoFromRcv();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgraderDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUpgraderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUpgraderDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADERDOC_H__678EEB8C_5132_11D2_BC1F_0080C868BB53__INCLUDED_)

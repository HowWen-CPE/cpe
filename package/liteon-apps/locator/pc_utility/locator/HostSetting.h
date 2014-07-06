#pragma once
#include "afxcmn.h"


// CHostSetting �Ի���

class CHostSetting : public CDialog
{
	DECLARE_DYNAMIC(CHostSetting)

public:
	CHostSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHostSetting();

// �Ի�������
	enum { IDD = IDD_HOST_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
public:
	CString m_hostName;
	CString m_domainName;
	UCHAR m_chrGetIP[4];
	CString m_hostName_old;
	CString m_domainName_old;
	UCHAR m_chrGetIP_old[4];
	CWnd* m_pParent;
	SOCKET		m_Socket;
	SOCKADDR_IN	saUdpServ, saUdpCli;
	CIPAddressCtrl m_ipAddr;
	
	
	char SerMacAdde[6];
	char LocalMacAddr[6];
public:
	void GetLocalIPs(char *localips);
	int checkIP();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
};

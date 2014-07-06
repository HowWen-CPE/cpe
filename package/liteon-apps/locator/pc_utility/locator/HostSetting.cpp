// HostSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "upgrader.h"
#include "HostSetting.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CHostSetting* m_DlgHost;

// CHostSetting 对话框

IMPLEMENT_DYNAMIC(CHostSetting, CDialog)

CHostSetting::CHostSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CHostSetting::IDD, pParent)
	, m_hostName(_T(""))
	, m_domainName(_T(""))
{
	m_DlgHost = this;
	m_pParent = pParent;
}

CHostSetting::~CHostSetting()
{
}

BOOL CHostSetting::OnInitDialog(){
	CDialog::OnInitDialog();

	m_ipAddr.SetAddress(m_chrGetIP[0], m_chrGetIP[1], m_chrGetIP[2], m_chrGetIP[3]);	
	//UpdateData(true);
	Invalidate();
	return true;
}

void CHostSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_hostName);
	DDX_Text(pDX, IDC_EDIT2, m_domainName);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipAddr);
}


BEGIN_MESSAGE_MAP(CHostSetting, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CHostSetting::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CHostSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHostSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CHostSetting 消息处理程序

void CHostSetting::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CHostSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	OnOK();
}

void CHostSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

int CHostSetting::checkIP()
{
	char	compareField;
	bool	zeroStart = false;

	for(int i = 0; i < 4; i ++)
	{
		compareField = m_chrGetIP[i] & 0x80;
		for(int j = 1; j < 8; j ++)
		{
			
			if ((zeroStart == false) && ((compareField & 0x80) == 0))
			{
				zeroStart = true;
			}
			compareField = (m_chrGetIP[i] << j) & 0x80;
			if ((zeroStart == true) && ((compareField & 0x80) != 0))
				return 1;
		}
	}

	if (m_chrGetIP[3] == 0)
		return 1;

	if (m_chrGetIP[3] == 255)
		return 1;

	return 	0;

}
void CHostSetting::OnOK(){
	char	request[128];
	int i = 0;
	int		nResult = 0;
	bool	b_change = false;
	char	chrSendBuffer[64] = "";
	m_ipAddr.GetAddress(m_chrGetIP[0],m_chrGetIP[1],m_chrGetIP[2],m_chrGetIP[3]);
	memset(request, 0, sizeof(request) );


	
	// 设置包
	request[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	request[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;
	//Server Mac Address
	request[i++] = SerMacAdde[0];
	request[i++] = SerMacAdde[1];
	request[i++] = SerMacAdde[2];
	request[i++] = SerMacAdde[3];
	request[i++] = SerMacAdde[4];
	request[i++] = SerMacAdde[5];

	
	request[i++] = LocalMacAddr[0];
	request[i++] = LocalMacAddr[1];
	request[i++] = LocalMacAddr[2];
	request[i++] = LocalMacAddr[3];
	request[i++] = LocalMacAddr[4];
	request[i++] = LocalMacAddr[5];

	request[i++] = (UCP_METHOD_GET_IP >> 8) & 0xff;
	request[i++] = (UCP_METHOD_GET_IP >> 0) & 0xff;

	//Local IP & Port
	CString HostIP;
	char LocalHostIP[225] = "";
	GetLocalIPs (LocalHostIP);
	HostIP = LocalHostIP;
	for (i; i < 14; i++)
	{
		CString StrTemp;

		int pos = HostIP.Find('.',0);
		if ( pos != -1 )
			StrTemp = HostIP.Left(pos);
		else
			StrTemp = HostIP;

		int tt;
		tt = atoi(StrTemp);
		request[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	

    request[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	request[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	request[i++] = 0x01;
	request[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE
	request[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	request[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	request[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	request[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	request[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	request[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	request[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	// Method 这里为设置host
	request[i++] = (UCP_METHOD_SET_HOST >> 8)& 0xff;
	request[i++] = (UCP_METHOD_SET_HOST >> 0)& 0xff;

	char* abc;
	int mindex;
	int j, length;
	if(m_hostName_old != m_hostName || m_domainName_old != m_domainName||
		m_chrGetIP[0] != m_chrGetIP_old[0]||
		m_chrGetIP[1] != m_chrGetIP_old[1]||
		m_chrGetIP[2] != m_chrGetIP_old[2]||
		m_chrGetIP[3] != m_chrGetIP_old[3])
	{
		b_change = true;
		//ip address
		request[i++] = UCP_CODE_IP_ADDR;
		request[i++] = 4;
		request[i++] = m_chrGetIP[0];
		request[i++] = m_chrGetIP[1];
		request[i++] = m_chrGetIP[2];
		request[i++] = m_chrGetIP[3];

		//host name
		request[i++] = UCP_CODE_HOST_NAME;
		length =  m_hostName.GetLength();
		if(length > 255){
			MessageBox("Sorry,the length of host name is more than 255!",MB_OK);
			return;
		}
		request[i++] = (length)&0xff;
		abc = (LPSTR)(LPCTSTR)(m_hostName);
		for(mindex = i, j=0;i < (mindex + length), j< length;j++)
		{
			request[i] = *(abc + j);
			i++;
		}

		//domain name
		request[i++] = UCP_CODE_DOMAIN_NAME;
		length = m_domainName.GetLength();
		if(length > 255){
			MessageBox("Sorry,the length of domain name is more than 255!",MB_OK);
			return;
		}
		request[i++] = (length)&0xff;
		
		abc = (LPSTR)(LPCTSTR)(m_domainName);
		for(mindex = i, j=0; i < (mindex + length), j< length; j++)
		{
			request[i] = *(abc + j);
			i++;
		
		}
	}
	request[i++] = (char)UCP_CODE_END;



	if (b_change)
		nResult = sendto(	m_Socket, request, i, 0, (SOCKADDR *) &saUdpServ,sizeof ( SOCKADDR_IN ));
		
	if( nResult == SOCKET_ERROR ) 
	{
		sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		AfxMessageBox( chrSendBuffer );
		return;
	}
	if (b_change)
		m_pParent->SetTimer(TIMER_OVERTIME,2000,NULL);


	CDialog::OnOK();
}

void CHostSetting::GetLocalIPs(char *localips)
{
	PIP_ADAPTER_INFO	pinfo;
	BYTE				m_data[4096];
	unsigned long		len = 0;

	ZeroMemory(m_data, sizeof(m_data)/sizeof(BYTE));
	pinfo = ( PIP_ADAPTER_INFO )m_data;

	unsigned long nError;

	nError = GetAdaptersInfo( pinfo,&len );
	switch( nError )
	{
		case 0:
			break;
		case ERROR_BUFFER_OVERFLOW:
			nError = GetAdaptersInfo(pinfo, &len);
			if (nError == 0)
				break;
			else 
				MessageBox("Get local IP Error!","Error",MB_OK);
			break;
	}
	
	PIP_ADDR_STRING pAddressList = &(pinfo->IpAddressList);
	_stprintf(localips,"%s",pAddressList->IpAddress.String);

	/*LocalMacAddr[0] = pinfo->Address[0];
	LocalMacAddr[1] = pinfo->Address[1];
	LocalMacAddr[2] = pinfo->Address[2];
	LocalMacAddr[3] = pinfo->Address[3];
	LocalMacAddr[4] = pinfo->Address[4];
	LocalMacAddr[5] = pinfo->Address[5];*/
	

}
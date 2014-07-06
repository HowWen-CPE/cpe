// upgraderDoc.cpp : implementation of the CUpgraderDoc class
//

#include "stdafx.h"
#include "upgrader.h"
#include "upgraderDoc.h"
#include <Iphlpapi.h>
#include "IpConfig.h"
#include <time.h>

#include "upgraderView.h"
#include "TftpProgress.h"
#include "PasswordDlg.h"
#include "ManualDlg.h"
#include "DeviceInfoDlg.h"
#include "Waiting.h"
#include "Login.h"
#include "WaitReset.h"
#include "Wireless.h"
#include "WizardSheet.h"//********
#include "NGWizard.h"
#include "SelectConectionDlg.h"
#include "HostSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpgraderDoc

IMPLEMENT_DYNCREATE(CUpgraderDoc, CDocument)

BEGIN_MESSAGE_MAP(CUpgraderDoc, CDocument)
	//{{AFX_MSG_MAP(CUpgraderDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgraderDoc construction/destruction

CUpgraderDoc::CUpgraderDoc()
{
	m_pDefaultAdapter = NULL;
	m_pAdapterInfo = GetAllAdaptersInfo();
}

CUpgraderDoc::~CUpgraderDoc()
{
	free(m_pAdapterInfo);
	m_pAdapterInfo = NULL;
	FreeList();
	
	// 清除设备信息内存
	ClearDevice();
}

BOOL CUpgraderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUpgraderDoc serialization

void CUpgraderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CUpgraderDoc diagnostics

#ifdef _DEBUG
void CUpgraderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUpgraderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUpgraderDoc commands

void CUpgraderDoc::FreeList()
{
	CDeviceInfo *pItem;

	POSITION pos = m_lstDevices.GetHeadPosition();
	while (pos)
	{
		pItem = m_lstDevices.GetNext(pos);
		delete pItem;
	}
	m_lstDevices.RemoveAll();
}

// 取得所有网卡的信息 David 2010.6.2 add
PIP_ADAPTER_INFO GetAllAdaptersInfo()
{
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ASSERT(NULL != pAdapterInfo);
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// 空间不够则重新分配空间
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR){
			
		return pAdapterInfo;
	}
	else
	{
		free(pAdapterInfo);
		return NULL;
	}
}

// 清除设备信息内存 David 2010.6.11 Add
void CUpgraderDoc::ClearDevice()
{
	for (int i=0; i<m_arrDevice.size(); ++i)
	{
		delete m_arrDevice[i];
		m_arrDevice[i] = NULL;
	}
	m_arrDevice.clear();
}

// 从socket中把接收的数据转换到本地内存 David 2010.6.11 Add
// 缓冲区指针为空返回0
// 不是来自设备端的数据包返回-1
// 与客户端的IP不同返回-2
// 转换分段数据错误-3
// 用户名和密码错误返回-4
// 成功返回1
int CUpgraderDoc::GetDataFromSocket(char* buffer, //[in] 接收socket数据的缓冲的指针
									CString& strServIP)
{
	int nRes = 1;
	ASSERT(NULL != buffer);
	if (NULL == buffer)
		return 0;
	// 清空原内存
	m_RecieveData.ClearData();
	int npBufferIndex = 0;

	


	// 接收成功
	ULONG uClientOper;
	HexToULONG(buffer, 2, npBufferIndex, &uClientOper);

	// 如果不是来自设备端的数据包返回
	if (uClientOper != UAP_CLIENT_OPTION)	
		return -1;

	// 正常接收设备端的数据包
	memset(m_szLocalHostIP, NULL, sizeof(m_szLocalHostIP));
	GetLocalIPs(m_szLocalHostIP, sizeof(m_szLocalHostIP));
	int len = strlen(m_szLocalHostIP);
	if (!IsHexDigit(m_szLocalHostIP[len-1]))
		m_szLocalHostIP[len-1] = 0;
	char szIPEnd[2]= {13, 0};
	len = strcspn(m_szLocalHostIP, szIPEnd);
	m_szLocalHostIP[len] = 0;

	// 客户端IP
	char szIP[20];
	HexToChar(buffer, 4 , npBufferIndex , szIP);
	Ip2String(szIP, szIP);
	if (strcmp(m_szLocalHostIP, szIP) != 0)
		return -1;

	// UDP 端口号
	ULONG		uPort;
	HexToULONG(buffer, 2 , npBufferIndex , &uPort);

	// 服务（设备）端类型
	ULONG Distype;
	HexToULONG(buffer, 2 , npBufferIndex , &Distype);								

	// 服务（设备）端MAC地址
	int	iMACAddrTemp[6];
	for (int j=0; j<6; j++)
	{
		HexToint(buffer, 1 , npBufferIndex , &iMACAddrTemp[j]);
	}
	char chrMacAddrTemp[18];
	memset(chrMacAddrTemp, 0 , sizeof(chrMacAddrTemp));
	_stprintf(chrMacAddrTemp,"%02X:%02X:%02X:%02X:%02X:%02X", 
		iMACAddrTemp[0], iMACAddrTemp[1], iMACAddrTemp[2], 
		iMACAddrTemp[3], iMACAddrTemp[4], iMACAddrTemp[5]);					
	CString	strMACAddress = chrMacAddrTemp;
	for (int j=0; j<6; j++)
		m_RecieveData.m_arrMacAddr[j] = iMACAddrTemp[j];

	// 序列号
	ULONG		uFlagRequest;
	HexToULONG(buffer, 2 , npBufferIndex , &uFlagRequest);

	// 设备类型
	ULONG		uIPConfig;
	HexToULONG(buffer, 2 , npBufferIndex , &uIPConfig);

	// Flag
	ULONG		uType;
	HexToULONG(buffer, 1 , npBufferIndex , &uType);

	// 设备类别
	ULONG		uUCPClass;
	HexToULONG(buffer, 4 , npBufferIndex , &uUCPClass);

	m_RecieveData.m_strServIP = strServIP;
	// method
	ULONG		uMethod;
	HexToULONG(buffer, 2 , npBufferIndex , &uMethod);
	m_RecieveData.m_SendData.m_uMethod = uMethod;

	switch(uMethod)
	{
	case UCP_METHOD_DISCOVER:
		{
			// 读取设备的名称，类型和固件版本
			for (int n = 0; n < 4; n++)
			{
				// 读取长度
				ULONG Flag;
				HexToULONG(buffer, 1 , npBufferIndex , &Flag);
				
				// 读取长度
				int StrLen = (int)buffer[npBufferIndex++];
				char szDeviceTemp[100] = {'\0'};
				switch(Flag)
				{
				case UCP_CODE_DEVICE_NAME:				
					HexToChar(buffer, StrLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_DiscoverData.m_strDeviceName = szDeviceTemp;
					break;
				case UCP_CODE_DEVICE_TYPE:				
					HexToChar(buffer, StrLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_DiscoverData.m_strDeviceType = szDeviceTemp;
					break;
				case UCP_CODE_SOFTWARE_VER:
					HexToChar(buffer, StrLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_DiscoverData.m_strFirmware = szDeviceTemp;
					break;
				case UCP_CODE_HTTP_TYPE:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_DiscoverData.m_nHttpType);
					break;
				}
			}
			// 根据MAC地址查找是否该设备已经存在
			int i=0;
			for(; i<m_arrDevice.size(); i++)
			{
				LCT_DEVICE* pDevice = m_arrDevice[i];
				if (strcmp((LPCTSTR)pDevice->m_strMACAddr, chrMacAddrTemp) == 0)
					break;
			}

			// 不存在则说明探测到新设备
			if (i == m_arrDevice.size())
			{
				// 添加到内存
				LCT_DEVICE* pDevice = new LCT_DEVICE;
				ASSERT(NULL != pDevice);
				m_arrDevice.push_back(pDevice);
				pDevice->m_strName = m_RecieveData.m_SendData.m_DiscoverData.m_strDeviceName;
				pDevice->m_strType = m_RecieveData.m_SendData.m_DiscoverData.m_strDeviceType;
				pDevice->m_strFirmwareVer = m_RecieveData.m_SendData.m_DiscoverData.m_strFirmware;
				pDevice->m_nHTTPSEnable = m_RecieveData.m_SendData.m_DiscoverData.m_nHttpType;
				pDevice->m_strMACAddr = chrMacAddrTemp;
				pDevice->m_strIPAddr = m_RecieveData.m_strServIP;
				for (int j=0; j<6; ++j)
					pDevice->m_arrMac[j] = iMACAddrTemp[j];
			}
			break;
		}		
	case UCP_METHOD_GET_INFO:
		
	
	case UCP_METHOD_GET_IP:
		{
			// 读取标志
			char Flag;
			
			for( HexToChar(buffer, 1 , npBufferIndex , &Flag);
				Flag != (char)UCP_CODE_END;
				HexToChar(buffer, 1 , npBufferIndex , &Flag) )
			{						
				// 读取长度
				char GetLen;
				HexToChar(buffer, 1 , npBufferIndex , &GetLen);

				// 根据长度读取内容
				char szDeviceTemp[100] = {'\0'};	// 读取字符的缓冲区
				BOOL ErrorBreak = FALSE;			// 错误标志
				switch (Flag)
				{
				case UCP_CODE_BRIDGE:
					HexToint(buffer, GetLen , npBufferIndex , &m_RecieveData.bridge);
					
					break;
				case UCP_CODE_DEVICE_NAME:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_DiscoverData.m_strDeviceName = szDeviceTemp;
					m_RecieveData.m_SendData.m_GetinfoData.m_strDeviceName = szDeviceTemp;
					break;
				case UCP_CODE_DEVICE_TYPE:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_DiscoverData.m_strDeviceType = szDeviceTemp;
					break;
				case UCP_CODE_SOFTWARE_VER:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_DiscoverData.m_strFirmware = szDeviceTemp;
					m_RecieveData.m_SendData.m_GetinfoData.m_strFirmware = szDeviceTemp;
					break;										
				case UCP_CODE_USE_DHCP:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nDHCP);
					break;
				case UCP_CODE_GATEWAY_ADDR:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[0]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[1]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[2]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[3]);
					break;
				case UCP_CODE_SUBNET_MASK:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[0]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[1]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[2]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[3]);
					break;
				case UCP_CODE_IP_ADDR:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[0]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[1]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[2]);
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[3]);
					break;
				case UCP_CODE_SSID:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_GetinfoData.m_strSSID = szDeviceTemp;
					break;
				case UCP_CODE_CHANNEL:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nChannel);
					break;
				case UCP_CODE_OPMODE:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nOper);
					break;
				case UCP_CODE_WIRELESS_MODE:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nWirelessMode);
					break;
				case UCP_CODE_SEC_METHOD:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nSecurity);
					break;
				case UCP_CODE_WLAN_MACADDR:					
					HexToChar(buffer, GetLen , npBufferIndex , m_RecieveData.m_SendData.m_GetinfoData.m_szcharWlanMACAddress);					
					break;
				case UCP_CODE_ENCRYPT_ONOFF:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nEncryptionOnOff);
					break;
				case UCP_CODE_SUPER_G:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nSuperG);
					break;
				case UCP_CODE_ASSOCIATED:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_associated);
					break;
			    case UCP_CODE_BSSID:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_GetinfoData.m_Bssid = szDeviceTemp;
					break;
				case UCP_CODE_RSSI:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_GetinfoData.m_Rssi = szDeviceTemp;
					break;
				case UCP_CODE_5GWIRELESS_MODE:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_n5GWirelessMode);
					break;
				case UCP_CODE_5GWLAN_MACADDR:					
					HexToChar(buffer, GetLen , npBufferIndex , m_RecieveData.m_SendData.m_GetinfoData.m_szchar5GWlanMACAddress);					
					break;
				case UCP_CODE_5GSSID:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_GetinfoData.m_str5GSSID = szDeviceTemp;
					break;
				case UCP_CODE_5GRSSI:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_GetinfoData.m_5GRssi = szDeviceTemp;
					break;
				case UCP_CODE_5GBSSID:
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					m_RecieveData.m_SendData.m_GetinfoData.m_5GBssid = szDeviceTemp;
					break;
				case UCP_CODE_5GASSOCIATED:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_5Gassociated);
					break;
				case UCP_CODE_5GCHANNEL:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_n5GChannel);
					break;
				case UCP_CODE_5GSEC_METHOD:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_n5GSecurity);
					break;
				case UCP_CODE_5GENCRYPT_ONOFF:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_n5GEncryptionOnOff);
					break;
				case UCP_CODE_2GRADIO_ONOFF:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nRadioOnOff);
					break;
				case UCP_CODE_5GRADIO_ONOFF:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_n5GRadioOnOff);
					break;
				case UCP_CODE_WLAN_OPMODE:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_nWlanOpmode);
					break;
				case UCP_CODE_5GWLAN_OPMODE:
					HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_GetinfoData.m_n5GWlanOpmode);
					break;
				default:		// 其他的跳过
					HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
					//ErrorBreak = TRUE;
					break;
				}
				// 读取到位置类型的标志发生错误则推出循环
				if (ErrorBreak)
				{
					nRes = -3;
					break;
				}
			}
			break;
		}

	case UCP_METHOD_GET_HOST:{
			char Flag;
			
			for( HexToChar(buffer, 1 , npBufferIndex , &Flag);
				Flag != (char)UCP_CODE_END;
				HexToChar(buffer, 1 , npBufferIndex , &Flag) )
			{						
				// 读取长度
				char GetLen;
				HexToChar(buffer, 1 , npBufferIndex , &GetLen);

				// 根据长度读取内容
				char szDeviceTemp[100] = {'\0'};	// 读取字符的缓冲区
				int szIPTemp[4] = {'\0'};
				switch(Flag)
				{
					case UCP_CODE_HOST_NAME:{
						HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
						m_RecieveData.m_SendData.m_HostData.m_hostName = szDeviceTemp;
						break;
					}
					case UCP_CODE_IP_ADDR:{
						HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_HostData.m_IPAddr[0]);
						HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_HostData.m_IPAddr[1]);
						HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_HostData.m_IPAddr[2]);
						HexToint(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_HostData.m_IPAddr[3]);
						

						break;
					}
					case UCP_CODE_DOMAIN_NAME:{
						HexToChar(buffer, GetLen , npBufferIndex , szDeviceTemp);
						m_RecieveData.m_SendData.m_HostData.m_domainName = szDeviceTemp;
						break;						  
					}		
					default:{
						break;
					}
				}
			}

			break;	 
		}
	case UCP_METHOD_SET_HOST:{
		ULONG ip_set_result;
		HexToULONG(buffer, 1 , npBufferIndex , &ip_set_result);
		m_RecieveData.m_SendData.ip_set_result = ip_set_result;
		break;
    	 }
	case UCP_METHOD_SET_IP:{
		ULONG ip_set_result;
		HexToULONG(buffer, 1 , npBufferIndex , &ip_set_result);
		m_RecieveData.m_SendData.ip_set_result = ip_set_result;
		break;
		}
	case UCP_METHOD_AUTH:
		HexToULONG(buffer, 1 , npBufferIndex , &m_RecieveData.m_SendData.m_AuthData.m_uAuto);
		if (m_RecieveData.m_SendData.m_AuthData.m_uAuto == 1)
		{
			//AfxMessageBox( "User Name or Password Fail!" );
			nRes = -4;
		}
		else
		{
			HexToULONG(buffer, 2 , npBufferIndex , &m_RecieveData.m_SendData.m_AuthData.m_uRequestMethod);
		}
		break;	
	default:
		//ASSERT(FALSE);
		break;
	}

	// 更新设备信息
	UpdateDeviceInfoFromRcv();

	return nRes;
}

// 得到本机IP
BOOL GetLocalIPs(char *localips, int len)
{
	// 初始化结果
	memset (localips, 0, len);

	char locips[256] = "\0";
	//char locname[512] = "\0";
	//if (!GetLocalName(locname, 512) || !ResolveIPs(locname, locips, 256))
	//	return FALSE;

	// 取得默认网卡
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);

	CString strLocalIP;
	//strLocalIP.Format("%s", ); 
	// 根据默认网卡取得IP地址
	strcpy(locips, pDoc->m_pDefaultAdapter->IpAddressList.IpAddress.String);

	int uselen = strlen (locips) + 1;
	if (len < uselen)
		uselen = len;

	// 为结果赋值
	memcpy (localips, locips, uselen);

	return TRUE;
}

// 更新设备信息 David 2010.6.12 Add
void CUpgraderDoc::UpdateDeviceInfoFromRcv()
{
	
}

//获得网卡名字
BOOL GetMacName(char* localips, int len)
{
	// 初始化结果
	memset (localips, 0, len);

	char locips[256] = "\0";
	memset (locips, 0, strlen(locips));
	// 取得默认网卡
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);

	CString strLocalIP;
	//strLocalIP.Format("%s", ); 
	strcpy(locips, pDoc->m_pDefaultAdapter->AdapterName);
	int uselen = strlen (locips) + 1;
	if (len < uselen)
		uselen = len;

	// 为结果赋值
	memcpy (localips, locips, uselen);
	return TRUE;
}
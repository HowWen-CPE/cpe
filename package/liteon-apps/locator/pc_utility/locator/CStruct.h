// 各种数据结构的定义 David 2010.6.11 add

#ifndef __STRUCT_H
#define __STRUCT_H

#pragma pack(push) //保存对齐状态
#pragma pack(1)	  //1字节对齐
extern int g_nID;
// 基类
class CBase
{
public:
	CBase(int nID=0)
	{
		m_nID = nID;
		if (g_nID > nID)
			m_nID = g_nID;			
		else
			g_nID = nID;
		g_nID++;
	}
	virtual ~CBase(){}
public:
	int m_nID;
};

// 设备信息
struct LCT_DEVICE:public CBase
{
public:
	LCT_DEVICE(int nID=0):CBase(nID){};
	~LCT_DEVICE(){};
public:
	CString m_strName;			// 设备名称
	CString m_strType;			// 设备类型
	CString m_strIPAddr;		// IP地址
	CString m_strMACAddr;		// MAC地址
	CString m_strFirmwareVer;	// 固件版本 
	int m_nDHCPEnable;
	int m_arrGateway[4];		
	int m_arrSubMask[4];
	int m_arrMac[6];
	CString m_strSSID;
	CString m_strSecurity;
	CString m_strWirelessMode;
	int m_nHTTPSEnable;
};

// 服务端回应探测包的结构
struct LCT_RCV_DISCOVER
{
public:
	// 清空数据
	void ClearData();
public:
	CString m_strDeviceType;	// 设备类型
	CString m_strDeviceName;	// 设备名称
	CString m_strFirmware;		// 固件版本
	int m_nHttpType;			// Http Type: http or https
};

// 服务端回应getinfo的结构
struct LCT_RCV_GETINFO
{
public:
	// 清空数据
	void ClearData();
public:
	LCT_RCV_GETINFO(){};
	~LCT_RCV_GETINFO(){};
	int m_nDHCP;
	int m_arrGateway[4];
	int m_arrSubmask[4];
	int m_arrIPAddr[4];
	int m_nOper;		// 操作模式
	CString m_strSSID;
	int m_nChannel;
	int m_nWirelessMode;
	int m_nSecurity;		// 安全类型
	int m_nEncryptionOnOff;
	int m_nSuperG;
	CString m_strDeviceName;
	CString m_strFirmware;
	char m_szcharWlanMACAddress[18];
	int m_associated;
	CString m_Bssid;
	CString m_Rssi;
	int m_n5GWirelessMode;
	char m_szchar5GWlanMACAddress[18];
	CString m_str5GSSID;
	CString m_5GRssi;
	CString m_5GBssid;
	int m_5Gassociated;
	int m_n5GChannel;
	int m_n5GSecurity;
	int m_n5GEncryptionOnOff;
	int m_nRadioOnOff;
	int m_n5GRadioOnOff;
	int m_nWlanOpmode;
	int m_n5GWlanOpmode;
};

// 服务端回应登录认证结构
struct LCT_RCV_AUTH
{
public:
	// 清空数据
	void ClearData();
public:
	ULONG m_uAuto;			// 成功则返回0x00，失败返回0x01
	ULONG m_uRequestMethod; // 登录后进行何种操作
};

//服务端回应主机名相关结构
struct LCT_RCV_HOST
{
public:
	//清空数据
	void ClearData();
public:
	CString m_hostName;                //主机名
	CString m_domainName;			   //域名
	int m_IPAddr[4];				   //IP地址
};

// 服务端回应的数据结构
struct LCT_RCV_PACKAEGDATA
{
public:
	// 清空数据
	void ClearData();
public:
	ULONG	m_uMethod;					// 方法                [2 BYTE]
	LCT_RCV_DISCOVER m_DiscoverData;	// 探测包数据
	LCT_RCV_GETINFO m_GetinfoData;		// 取得设备信息数据
	LCT_RCV_HOST m_HostData;            //主机相关信息
	LCT_RCV_AUTH m_AuthData;			// 登录认证数据
	
	int ip_set_result;                  //ip信息设置结果
	

};



// 通用数据包结构
struct LCT_PACKAEGDATA
{
public:
	// 清空数据
	void ClearData();
public:
	ULONG	m_uClientOper;			// 客户端选项 0x0002   [2 BYTE]
	CString m_strClientIP;			// 客户端IP			   [4 BYTE]
	ULONG	m_uPort;				// 客户端端口号 0x5168 [2 BYTE]

	ULONG	m_uServOper;			// 服务端选项 0x0001   [2 BYTE]
	int		m_arrMacAddr[6];		// 服务端MAC地址       [6 BYTE]
	
	ULONG	m_uFlagRequest;			// 包的序列号          [2 BYTE]
	ULONG	m_uDapType;				// 设备类型            [2 BYTE]
	ULONG	m_uFlag;				// Flag 客户端为0x01 服务端为0x00 [1 BYTE]
	ULONG	m_uUCPClass;			// 设备类别            [4 BYTE]
	LCT_RCV_PACKAEGDATA m_SendData;
	CString m_strServIP;			// 服务端IP-这个从套接字得来
	int bridge;
	
};
#pragma pack(pop)//恢复对齐状态
#endif // __STRUCT_H
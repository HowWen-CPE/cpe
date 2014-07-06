// �������ݽṹ�Ķ��� David 2010.6.11 add

#ifndef __STRUCT_H
#define __STRUCT_H

#pragma pack(push) //�������״̬
#pragma pack(1)	  //1�ֽڶ���
extern int g_nID;
// ����
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

// �豸��Ϣ
struct LCT_DEVICE:public CBase
{
public:
	LCT_DEVICE(int nID=0):CBase(nID){};
	~LCT_DEVICE(){};
public:
	CString m_strName;			// �豸����
	CString m_strType;			// �豸����
	CString m_strIPAddr;		// IP��ַ
	CString m_strMACAddr;		// MAC��ַ
	CString m_strFirmwareVer;	// �̼��汾 
	int m_nDHCPEnable;
	int m_arrGateway[4];		
	int m_arrSubMask[4];
	int m_arrMac[6];
	CString m_strSSID;
	CString m_strSecurity;
	CString m_strWirelessMode;
	int m_nHTTPSEnable;
};

// ����˻�Ӧ̽����Ľṹ
struct LCT_RCV_DISCOVER
{
public:
	// �������
	void ClearData();
public:
	CString m_strDeviceType;	// �豸����
	CString m_strDeviceName;	// �豸����
	CString m_strFirmware;		// �̼��汾
	int m_nHttpType;			// Http Type: http or https
};

// ����˻�Ӧgetinfo�Ľṹ
struct LCT_RCV_GETINFO
{
public:
	// �������
	void ClearData();
public:
	LCT_RCV_GETINFO(){};
	~LCT_RCV_GETINFO(){};
	int m_nDHCP;
	int m_arrGateway[4];
	int m_arrSubmask[4];
	int m_arrIPAddr[4];
	int m_nOper;		// ����ģʽ
	CString m_strSSID;
	int m_nChannel;
	int m_nWirelessMode;
	int m_nSecurity;		// ��ȫ����
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

// ����˻�Ӧ��¼��֤�ṹ
struct LCT_RCV_AUTH
{
public:
	// �������
	void ClearData();
public:
	ULONG m_uAuto;			// �ɹ��򷵻�0x00��ʧ�ܷ���0x01
	ULONG m_uRequestMethod; // ��¼����к��ֲ���
};

//����˻�Ӧ��������ؽṹ
struct LCT_RCV_HOST
{
public:
	//�������
	void ClearData();
public:
	CString m_hostName;                //������
	CString m_domainName;			   //����
	int m_IPAddr[4];				   //IP��ַ
};

// ����˻�Ӧ�����ݽṹ
struct LCT_RCV_PACKAEGDATA
{
public:
	// �������
	void ClearData();
public:
	ULONG	m_uMethod;					// ����                [2 BYTE]
	LCT_RCV_DISCOVER m_DiscoverData;	// ̽�������
	LCT_RCV_GETINFO m_GetinfoData;		// ȡ���豸��Ϣ����
	LCT_RCV_HOST m_HostData;            //���������Ϣ
	LCT_RCV_AUTH m_AuthData;			// ��¼��֤����
	
	int ip_set_result;                  //ip��Ϣ���ý��
	

};



// ͨ�����ݰ��ṹ
struct LCT_PACKAEGDATA
{
public:
	// �������
	void ClearData();
public:
	ULONG	m_uClientOper;			// �ͻ���ѡ�� 0x0002   [2 BYTE]
	CString m_strClientIP;			// �ͻ���IP			   [4 BYTE]
	ULONG	m_uPort;				// �ͻ��˶˿ں� 0x5168 [2 BYTE]

	ULONG	m_uServOper;			// �����ѡ�� 0x0001   [2 BYTE]
	int		m_arrMacAddr[6];		// �����MAC��ַ       [6 BYTE]
	
	ULONG	m_uFlagRequest;			// �������к�          [2 BYTE]
	ULONG	m_uDapType;				// �豸����            [2 BYTE]
	ULONG	m_uFlag;				// Flag �ͻ���Ϊ0x01 �����Ϊ0x00 [1 BYTE]
	ULONG	m_uUCPClass;			// �豸���            [4 BYTE]
	LCT_RCV_PACKAEGDATA m_SendData;
	CString m_strServIP;			// �����IP-������׽��ֵ���
	int bridge;
	
};
#pragma pack(pop)//�ָ�����״̬
#endif // __STRUCT_H
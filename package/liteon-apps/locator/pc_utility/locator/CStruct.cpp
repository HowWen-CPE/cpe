#include "StdAfx.h"
#include "CStruct.h"
int g_nID;
// 清空数据
void LCT_PACKAEGDATA::ClearData()
{
	m_uClientOper = 0u;
	m_strClientIP.Empty();
	m_uPort = 0u;
	m_uServOper = 0u;
	memset(m_arrMacAddr, 0, sizeof(m_arrMacAddr));
	m_uFlagRequest = 0u;
	m_uDapType = 0u;
	m_uFlag = 0u;
	m_uUCPClass = 0u;
	m_SendData.ClearData();
	m_strServIP.Empty();
	bridge = 0;
}

// 清空数据
void LCT_RCV_PACKAEGDATA::ClearData()
{
	m_uMethod = 0u;
	m_DiscoverData.ClearData();
	m_GetinfoData.ClearData();
	m_AuthData.ClearData();
	m_HostData.ClearData();
}

// 清空数据
void LCT_RCV_GETINFO::ClearData()
{
	m_nDHCP = 0;
	memset(m_arrGateway, 0, sizeof(m_arrGateway));
	memset(m_arrSubmask, 0, sizeof(m_arrSubmask));
	memset(m_arrIPAddr, 0, sizeof(m_arrIPAddr));
	m_nOper = 0;
	m_strSSID.Empty();
	m_nChannel = 0;
	m_nWirelessMode = 0;
	m_nSecurity = 0;
	m_nEncryptionOnOff = 0;
	m_nSuperG = 0;
	m_strDeviceName.Empty();
	m_strFirmware.Empty();
	memset(m_szcharWlanMACAddress, 0, sizeof(m_szcharWlanMACAddress));
    m_associated=0;
	m_Bssid.Empty();
	m_Rssi.Empty();
	m_n5GWirelessMode = 0;
	memset(m_szchar5GWlanMACAddress, 0, sizeof(m_szchar5GWlanMACAddress));
	m_str5GSSID.Empty();
	m_5GRssi.Empty();
	m_5GBssid.Empty();
	m_5Gassociated = 0;
	m_n5GChannel = 0;
	m_n5GSecurity = 0;
	m_n5GEncryptionOnOff = 0;
	m_nRadioOnOff = 0;
	m_n5GRadioOnOff = 0;
	m_nWlanOpmode = 0;
	m_n5GWlanOpmode = 0;
}

// 清空数据
void LCT_RCV_DISCOVER::ClearData()
{
	m_strDeviceType.Empty();
	m_strDeviceName.Empty();
	m_strFirmware.Empty();
	m_nHttpType = 0;
}

// 清空数据
void LCT_RCV_AUTH::ClearData()
{
	m_uAuto = 0u;
	m_uRequestMethod = 0u;
}

void LCT_RCV_HOST::ClearData()
{
	m_hostName = "";
	m_domainName = "";
	memset(m_IPAddr, NULL, sizeof(m_IPAddr));
}
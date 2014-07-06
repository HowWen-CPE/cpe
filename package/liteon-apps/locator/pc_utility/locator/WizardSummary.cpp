// WizardSummary.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardSummary.h"
//#include "WizardSheet.h"
#include "NGWizard.h"
#include "UpgraderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NO_CHANGE -1
/////////////////////////////////////////////////////////////////////////////
// CWizardSummary dialog


CWizardSummary::CWizardSummary(CWnd* pGrandFather, CWnd* pParent, bool SecurityEnable)
	: CBitmapDialog(CWizardSummary::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizardSummary)
	m_szAuthOld = _T("");
	m_szChannelOld = _T("");
	m_szDeviceNameOld = _T("");
	m_szEncryptionOld = _T("");
	m_szGatewayOld = _T("");
	m_szIpOld = _T("");
	m_szSubMaskOld = _T("");
	m_szOpModeOld = _T("");
	m_szSsidOld = _T("");
	m_szWModeOld = _T("");
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

//	m_iSecurity		= NO_CHANGE;
	m_iKeyStrType	= NO_CHANGE;
	m_iAuth			= NO_CHANGE;
	m_iEncryption	= NO_CHANGE;
	s_iSuperG		= NO_CHANGE;
	s_iWirelessMode	= NO_CHANGE;
	s_iOpMode		= NO_CHANGE;
	s_iChannel		= NO_CHANGE;
	s_iDHCPEnable	= NO_CHANGE;
	s_szSSID[0]		= NO_CHANGE;
	s_szDevName[0]	= NO_CHANGE;
	s_szRadiusServer = "NO_CHANGE";

	b_IpCahnged			= false;
	b_MaskCahnged		= false;
	b_GatewayCahnged	= false;
	b_DevNameCahnged	= false;
	b_OpModeCahnged		= false;
	b_SsidCahnged		= false;
	b_ChannelCahnged	= false;
	b_WlanModeCahnged	= false;
	b_AuthCahnged		= false;
	b_EncryCahnged		= false;

	m_bSecurityEnable = SecurityEnable;

//	mHollowBrush.CreateStockObject (HOLLOW_BRUSH);

//	mBitmap.LoadBitmap (IDB_BG_OP_MODE);
//	mType=BITMAP_TILE;
}


void CWizardSummary::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardSummary)
	DDX_Text(pDX, IDC_EDIT_AUTH_OLD, m_szAuthOld);
	DDX_Text(pDX, IDC_EDIT_CHANNEL_OLD, m_szChannelOld);
	DDX_Text(pDX, IDC_EDIT_DEVNAME_OLD, m_szDeviceNameOld);
	DDX_Text(pDX, IDC_EDIT_ENCRYPT_OLD, m_szEncryptionOld);
	DDX_Text(pDX, IDC_EDIT_GATEWAY_OLD, m_szGatewayOld);
	DDX_Text(pDX, IDC_EDIT_IP_OLD, m_szIpOld);
	DDX_Text(pDX, IDC_EDIT_IPMASK_OLD, m_szSubMaskOld);
	DDX_Text(pDX, IDC_EDIT_OPMODE_OLD, m_szOpModeOld);
	DDX_Text(pDX, IDC_EDIT_SSID_OLD, m_szSsidOld);
	DDX_Text(pDX, IDC_EDIT_WMODE_OLD, m_szWModeOld);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardSummary, CBitmapDialog)
	//{{AFX_MSG_MAP(CWizardSummary)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardSummary message handlers

BOOL CWizardSummary::OnInitDialog() 
{
	CBitmapDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	CString tempStr;
	CString tempSsid;
	int tempInt;
//	int tempInt2;
//	int tempFlag;
	bool bSecturityOn = true;
	char tempChar[22];
	int iPreDHCPEnable;

	// IP, SubnetMask, Gateway, DHCP
	iPreDHCPEnable = pWizardSheet->m_pWizardLogin->m_iDHCPEnable;
	if(iPreDHCPEnable)
	{
		tempStr.LoadString(IDS_DHCP_ENABLE);
		GetDlgItem(IDC_EDIT_IP_OLD)->SetWindowText(tempStr);
		GetDlgItem(IDC_EDIT_IPMASK_OLD)->SetWindowText(tempStr);
		GetDlgItem(IDC_EDIT_GATEWAY_OLD)->SetWindowText(tempStr);		
	}
	else
	{
		// IP Address
		Ip2String(&tempStr, pWizardSheet->m_pWizardLogin->m_cIPAddr);
		GetDlgItem(IDC_EDIT_IP_OLD)->SetWindowText(tempStr);
		// Subnet Mask
		Ip2String(&tempStr, pWizardSheet->m_pWizardLogin->m_cSubMask);
		GetDlgItem(IDC_EDIT_IPMASK_OLD)->SetWindowText(tempStr);
		// Default Gateway
		Ip2String(&tempStr, pWizardSheet->m_pWizardLogin->m_cGateway);
		if (tempStr.Compare("0.0.0.0") == 0)
			tempStr.LoadString(IDS_DISABLED);
		GetDlgItem(IDC_EDIT_GATEWAY_OLD)->SetWindowText(tempStr);		
	}

	// Device Name
	tempStr = pWizardSheet->m_pWizardLogin->m_cDevName;
	GetDlgItem(IDC_EDIT_DEVNAME_OLD)->SetWindowText(tempStr);

	// Operation Mode
	switch(pWizardSheet->m_pWizardLogin->m_iOpMode)
	{
	case CFG_OP_MODE_AP:
		tempStr.LoadString(IDS_OPMODE_AP);
		break;
	case CFG_OP_MODE_STA0:
		tempStr.LoadString(IDS_OPMODE_INF);
		break;
	case CFG_OP_MODE_REPEATER:
		tempStr.LoadString(IDS_OPMODE_REP);
		break;
	case CFG_OP_MODE_ADHOC:
		tempStr.LoadString(IDS_OPMODE_ADHOC);
		break;
	}
	GetDlgItem(IDC_EDIT_OPMODE_OLD)->SetWindowText(tempStr);

	// SSID
	tempStr = pWizardSheet->m_pWizardLogin->m_cSSID;
	if (tempStr.IsEmpty())
		tempStr.LoadString(IDS_ANY_SSID);
	GetDlgItem(IDC_EDIT_SSID_OLD)->SetWindowText(tempStr);

	// Channel
	tempInt = pWizardSheet->m_pWizardLogin->m_iChannel;
	if(tempInt == 0)
		tempStr.LoadString(IDS_CHANNEL_AUTO);
	else
	{
		sprintf(tempChar, "%dMHz (Channel %d)", 2407+5*tempInt, tempInt);
		tempStr = tempChar;
	}
	GetDlgItem(IDC_EDIT_CHANNEL_OLD)->SetWindowText(tempStr);

	// Wireless Mode
	tempInt = pWizardSheet->m_pWizardLogin->m_iSuperG;
	if((tempInt & ABOLT_SUPERG) && (tempInt & ABOLT_TURBO_PRIME))
		tempStr.LoadString(IDS_WMODE_SUPERG);
	else
	{
		switch(pWizardSheet->m_pWizardLogin->m_iWirelessMode)
		{
		case MODE_SELECT_11B:
			tempStr.LoadString(IDS_WMODE_B);
			break;
		case MODE_SELECT_11G:
			tempStr.LoadString(IDS_WMODE_G);
			break;
		case MODE_SELECT_108G:
			tempStr.LoadString(IDS_WMODE_STATICG);
			break;
		}
	}

	GetDlgItem(IDC_EDIT_WMODE_OLD)->SetWindowText(tempStr);

	// Security
	switch(pWizardSheet->m_pWizardLogin->m_iAuth)
	{
		case AUTH_OPEN_SYSTEM:
			switch(pWizardSheet->m_pWizardLogin->m_iEncryptionOnOff)
			{
			case OFF:
				tempStr.LoadString(IDS_DISABLED);
				bSecturityOn = false;
				GetDlgItem(IDC_EDIT_ENCRYPT_OLD)->ShowWindow(false);
				break;
			case ON:
				switch(pWizardSheet->m_pWizardLogin->m_iWepKeyType)
				{
				case WEP64:
					tempStr.LoadString(IDS_AUTH_WEP64);
					break;
				case WEP128:
					tempStr.LoadString(IDS_AUTH_WEP128);
					break;
				case WEP152:
					tempStr.LoadString(IDS_AUTH_WEP152);
					break;
				}
				break;
			}
			break;
		case AUTH_SHARED_KEY:
        case AUTH_AUTO: /*scout 2004/08/23*/
			switch(pWizardSheet->m_pWizardLogin->m_iWepKeyType)
			{
			case WEP64:
				tempStr.LoadString(IDS_AUTH_WEP64);
				break;
			case WEP128:
				tempStr.LoadString(IDS_AUTH_WEP128);
				break;
			case WEP152:
				tempStr.LoadString(IDS_AUTH_WEP152);
				break;
			}
			break;
		case AUTH_WPA:
			tempStr.LoadString(IDS_AUTH_WPA);
			break;
		case AUTH_WPAPSK:
			tempStr.LoadString(IDS_AUTH_WPA_PSK);
			break;
		
	}		
	GetDlgItem(IDC_EDIT_AUTH_OLD)->SetWindowText(tempStr);

	if (bSecturityOn)
	{
		// Cipher type or Authentication
		switch(pWizardSheet->m_pWizardLogin->m_iEncryption)
		{
		case ENCRYPTION_DISABLED:
			GetDlgItem(IDC_EDIT_ENCRYPT_OLD)->ShowWindow(false);
			break;
		case ENCRYPTION_WEP:
			switch(pWizardSheet->m_pWizardLogin->m_iAuth)
			{
			case AUTH_OPEN_SYSTEM:
				tempStr.LoadString(IDS_OPEN_SYSTEM);
				break;
			case AUTH_SHARED_KEY:
				tempStr.LoadString(IDS_SHARED_KEY);
				break;
	/*scout 2004/08/23*/
            case AUTH_AUTO:
				tempStr.LoadString(IDS_AUTO);
				break;

			}
			break;
		case ENCRYPTION_AES_CCM:
			tempStr.LoadString(IDS_ENCRYPTION_AES);
			break;
		case ENCRYPTION_TKIP:
			tempStr.LoadString(IDS_ENCRYPTION_TKIP);
			break;
	
	
		}
	GetDlgItem(IDC_EDIT_ENCRYPT_OLD)->SetWindowText(tempStr);
	}

	UpdateData(true);

//=======================================================================


	// IP, SubnetMask, Gateway, DHCP
	iCurrentDHCPEnable = pWizardSheet->m_pWizardIpConfig->m_iWzDhcpEnable;

	if(iCurrentDHCPEnable != iPreDHCPEnable)
	{
		s_iDHCPEnable = iCurrentDHCPEnable;
		if (iCurrentDHCPEnable == 1)
		{
			tempStr.LoadString(IDS_DHCP_ENABLE);
			if( tempStr.Compare( m_szIpOld ) != 0 )
			{
				b_IpCahnged = true;
				b_MaskCahnged = true;
				b_GatewayCahnged = true;
				s_iDHCPEnable = iCurrentDHCPEnable;
			}
			GetDlgItem(IDC_EDIT_IP_NEW)->SetWindowText(tempStr);
			GetDlgItem(IDC_EDIT_IPMASK_NEW)->SetWindowText(tempStr);
			GetDlgItem(IDC_EDIT_GATEWAY_NEW)->SetWindowText(tempStr);
		}
		else
		{
			// IP Address
			b_IpCahnged = true;
			Ip2String(&tempStr, pWizardSheet->m_pWizardIpConfig->m_szIPAddr);
			memcpy(s_szIPAddr, pWizardSheet->m_pWizardIpConfig->m_szIPAddr, sizeof(s_szIPAddr));
			GetDlgItem(IDC_EDIT_IP_NEW)->SetWindowText(tempStr);

			// Subnet Mask
			Ip2String(&tempStr, pWizardSheet->m_pWizardIpConfig->m_szSubMask);
			b_MaskCahnged = true;
			memcpy(s_szSubMask, pWizardSheet->m_pWizardIpConfig->m_szSubMask, sizeof(s_szSubMask));
			GetDlgItem(IDC_EDIT_IPMASK_NEW)->SetWindowText(tempStr);

			// Default Gateway
			Ip2String(&tempStr, pWizardSheet->m_pWizardIpConfig->m_szGateway);
			b_GatewayCahnged = true;
			if (tempStr.Compare("0.0.0.0") == 0)
				tempStr.LoadString(IDS_DISABLED);
			memcpy(s_szGateway, pWizardSheet->m_pWizardIpConfig->m_szGateway, sizeof(s_szGateway));
			GetDlgItem(IDC_EDIT_GATEWAY_NEW)->SetWindowText(tempStr);		
		}
	}/*scoutlee  2004/08/23*/
	else if(iCurrentDHCPEnable == 1){
    	tempStr.LoadString(IDS_DHCP_ENABLE);
	    GetDlgItem(IDC_EDIT_IP_NEW)->SetWindowText(tempStr);
	   	GetDlgItem(IDC_EDIT_IPMASK_NEW)->SetWindowText(tempStr);
		GetDlgItem(IDC_EDIT_GATEWAY_NEW)->SetWindowText(tempStr);
	}
	else if (iCurrentDHCPEnable == 0)
	{
		// IP Address
		Ip2String(&tempStr, pWizardSheet->m_pWizardIpConfig->m_szIPAddr);
		if( tempStr.Compare( m_szIpOld ) != 0 )
		{
			b_IpCahnged = true;
			memcpy(s_szIPAddr, pWizardSheet->m_pWizardIpConfig->m_szIPAddr, sizeof(s_szIPAddr));
		}
		GetDlgItem(IDC_EDIT_IP_NEW)->SetWindowText(tempStr);

		// Subnet Mask
		Ip2String(&tempStr, pWizardSheet->m_pWizardIpConfig->m_szSubMask);
		if( tempStr.Compare( m_szSubMaskOld ) != 0 )
		{
			b_MaskCahnged = true;
			memcpy(s_szSubMask, pWizardSheet->m_pWizardIpConfig->m_szSubMask, sizeof(s_szSubMask));
		}
		GetDlgItem(IDC_EDIT_IPMASK_NEW)->SetWindowText(tempStr);

		// Default Gateway
		Ip2String(&tempStr, pWizardSheet->m_pWizardIpConfig->m_szGateway);
		if (tempStr.Compare("0.0.0.0") == 0)
			tempStr.LoadString(IDS_DISABLED);
		if( tempStr.Compare( m_szGatewayOld ) != 0 )
		{
			b_GatewayCahnged = true;
			memcpy(s_szGateway, pWizardSheet->m_pWizardIpConfig->m_szGateway, sizeof(s_szGateway));
		}
		GetDlgItem(IDC_EDIT_GATEWAY_NEW)->SetWindowText(tempStr);		
	}

	// Device Name
	tempStr = pWizardSheet->m_pWizardSysDevName->m_szWzSysName;
	if( tempStr.Compare( m_szDeviceNameOld ) != 0 )
	{
		b_DevNameCahnged = true;
//		sprintf(s_szDevName, tempStr);
	}
	sprintf(s_szDevName, tempStr);
	GetDlgItem(IDC_EDIT_DEVNAME_NEW)->SetWindowText(tempStr);

	// Operation Mode
	tempInt = pWizardSheet->m_pWizardSystem->m_iWzOpModeReal;
	switch(tempInt)
	{
	case CFG_OP_MODE_AP:
		tempStr.LoadString(IDS_OPMODE_AP);
		break;
	case CFG_OP_MODE_STA0:
		tempStr.LoadString(IDS_OPMODE_INF);
		break;
	case CFG_OP_MODE_REPEATER:
		tempStr.LoadString(IDS_OPMODE_REP);
		break;
	case CFG_OP_MODE_ADHOC:
		tempStr.LoadString(IDS_OPMODE_ADHOC);
		break;
	}
	if( tempStr.Compare( m_szOpModeOld ) != 0 )
	{
		b_OpModeCahnged = true;
		s_iOpMode = tempInt;
	}
	GetDlgItem(IDC_EDIT_OPMODE_NEW)->SetWindowText(tempStr);

	// SSID
	switch(pWizardSheet->m_pWizardSystem->m_iWzOpModeReal)
	{
	case CFG_OP_MODE_AP:
		tempStr = tempSsid = pWizardSheet->m_pWizardSysAp->m_szWzSsidAp;
		break;
	case CFG_OP_MODE_STA0:
		tempSsid = pWizardSheet->m_pWizardSysClientInf->m_szWzSsidClientInf;
		if( tempSsid.IsEmpty() )// Any SSID
		{
			tempStr.LoadString(IDS_ANY_SSID);
			// Set Open System and Disable Encryption
			m_iAuth = AUTH_OPEN_SYSTEM;
			m_iEncryption = ENCRYPTION_DISABLED;
		}
		else
			tempStr = tempSsid;
		break;
	case CFG_OP_MODE_REPEATER:
		tempStr = tempSsid = pWizardSheet->m_pWizardSysRepeater->m_szWzSsidRepeater;
		break;
	case CFG_OP_MODE_ADHOC:
		tempStr = tempSsid = pWizardSheet->m_pWizardSysClientAdhoc->m_szWzSsidClientAdhoc;
		break;
	}
	if( tempSsid.Compare(pWizardSheet->m_pWizardLogin->m_cSSID) !=0 )
	{
		b_SsidCahnged = true;
		sprintf(s_szSSID, tempSsid);
	}
	GetDlgItem(IDC_EDIT_SSID_NEW)->SetWindowText(tempStr);

	// Wireless Mode
	switch(pWizardSheet->m_pWizardSystem->m_iWzOpModeReal)
	{
	case CFG_OP_MODE_AP:
		tempInt = pWizardSheet->m_pWizardSysAp->m_iWzApWirelessModeReal;
		break;
	case CFG_OP_MODE_STA0:
		tempInt = pWizardSheet->m_pWizardSysClientInf->m_iWzInfWirelessModeReal;
		break;
	case CFG_OP_MODE_REPEATER:
		tempInt = pWizardSheet->m_pWizardSysRepeater->m_iWzReapWirelessModeReal;
		break;
	case CFG_OP_MODE_ADHOC:
		tempInt = pWizardSheet->m_pWizardSysClientAdhoc->m_iWzAdhocWirelessModeReal;
		break;
	}
	switch(tempInt)
	{
	case W_802_11B:
		tempStr.LoadString(IDS_WMODE_B);
		break;
	case W_802_11G:
		tempStr.LoadString(IDS_WMODE_G);
		break;
	case W_802_11Static_G:
		tempStr.LoadString(IDS_WMODE_STATICG);
		break;
	case W_802_11SUPER_G:
		tempStr.LoadString(IDS_WMODE_SUPERG);
		break;

	}
	if (tempStr.Compare(m_szWModeOld) != 0)
	{
		b_WlanModeCahnged = true;
		s_iWirelessMode = tempInt;
	}
	GetDlgItem(IDC_EDIT_WMODE_NEW)->SetWindowText(tempStr);

	// Channel
	switch(pWizardSheet->m_pWizardSystem->m_iWzOpModeReal)
	{
	case CFG_OP_MODE_AP:
		tempInt = pWizardSheet->m_pWizardSysAp->m_iWzChannelAp;
		if (tempInt)
		{
			sprintf(tempChar, "%dMHz (Channel %d)", 2407+5*tempInt, tempInt);
			tempStr = tempChar;
		}
		else
			tempStr.LoadString(IDS_CHANNEL_AUTO);
		break;
	case CFG_OP_MODE_STA0:
		tempInt = 0;
		tempStr.LoadString(IDS_CHANNEL_AUTO);
		break;
	case CFG_OP_MODE_REPEATER:
		tempInt = 0;
		tempStr.LoadString(IDS_CHANNEL_AUTO);
		break;
	case CFG_OP_MODE_ADHOC:
		tempInt = pWizardSheet->m_pWizardSysClientAdhoc->m_iWzChannelClientAdhoc;
		if (tempInt)
		{
			sprintf(tempChar, "%dMHz (Channel %d)", 2407+5*tempInt, tempInt);
			tempStr = tempChar;
		}
		else
			tempStr.LoadString(IDS_CHANNEL_AUTO);
		break;
	}
	if (tempStr.Compare(m_szChannelOld) != 0)
	{
		b_ChannelCahnged = true;
		s_iChannel = tempInt;
	}
	GetDlgItem(IDC_EDIT_CHANNEL_NEW)->SetWindowText(tempStr);

//	if (m_bSecurityEnable)
//	{
		// Authentication
		if ((pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_REPEATER) ||
			(pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_ADHOC))
		{
			m_iSecurity = pWizardSheet->m_pWizardSecurityRepeater->m_iWzAuthRepeater;
			switch(m_iSecurity)
			{
			case DISABLED:
				tempStr.LoadString(IDS_DISABLED);
				if (tempStr.Compare(m_szAuthOld) != 0)
					b_AuthCahnged = true;
				break;
			case WEP64:
				tempStr.LoadString(IDS_AUTH_WEP64);
				if (tempStr.Compare(m_szAuthOld) != 0)
					b_AuthCahnged = true;
				break;
			case WEP128:
				tempStr.LoadString(IDS_AUTH_WEP128);
				if (tempStr.Compare(m_szAuthOld) != 0)
					b_AuthCahnged = true;
				break;
			case WEP152:
				tempStr.LoadString(IDS_AUTH_WEP152);
				if (tempStr.Compare(m_szAuthOld) != 0)
					b_AuthCahnged = true;
				break;
			}
			m_iAuthTemp = AUTH_OPEN_SYSTEM;
			if (b_AuthCahnged)
				m_iAuth = AUTH_OPEN_SYSTEM;
		}		
		else
		{
			m_iSecurity = pWizardSheet->m_pWizardSecurity->m_iWzAuth;
			switch(m_iSecurity)
			{
			case DISABLED:
				tempStr.LoadString(IDS_DISABLED);
				if (tempStr.Compare(m_szAuthOld) != 0)
				{
					b_AuthCahnged = true;
					m_iAuth = AUTH_OPEN_SYSTEM;
				}
				m_iAuthTemp = AUTH_OPEN_SYSTEM;
				break;
			case WEP64:
				tempStr.LoadString(IDS_AUTH_WEP64);
				if (tempStr.Compare(m_szAuthOld) != 0)
				{
					b_AuthCahnged = true;
//					m_iAuth = pWizardSheet->m_pWizardEncWep->m_iWzCiperType;
				}
				m_iAuthTemp = pWizardSheet->m_pWizardEncWep->m_iWzCiperTypeReal;
				break;
			case WEP128:
				tempStr.LoadString(IDS_AUTH_WEP128);
				if (tempStr.Compare(m_szAuthOld) != 0)
				{
					b_AuthCahnged = true;
//					m_iAuth = pWizardSheet->m_pWizardEncWep->m_iWzCiperType;
				}
				m_iAuthTemp = pWizardSheet->m_pWizardEncWep->m_iWzCiperTypeReal;
				break;
			case WEP152:
				tempStr.LoadString(IDS_AUTH_WEP152);
				if (tempStr.Compare(m_szAuthOld) != 0)
				{
					b_AuthCahnged = true;
//					m_iAuth = pWizardSheet->m_pWizardEncWep->m_iWzCiperType;
				}
				m_iAuthTemp = pWizardSheet->m_pWizardEncWep->m_iWzCiperTypeReal;
				break;
			case WPA_PSK:
				tempStr.LoadString(IDS_AUTH_WPA_PSK);
				if (tempStr.Compare(m_szAuthOld) != 0)
				{
					b_AuthCahnged = true;
					m_iAuth = AUTH_WPAPSK;
				}
				m_iAuthTemp = AUTH_WPAPSK;
				break;
			case WPA:
				tempStr.LoadString(IDS_AUTH_WPA);
				if (tempStr.Compare(m_szAuthOld) != 0)
				{
					b_AuthCahnged = true;
					m_iAuth = AUTH_WPA;
				}
				m_iAuthTemp = AUTH_WPA;
				break;
			}
		}
		GetDlgItem(IDC_EDIT_AUTH_NEW)->SetWindowText(tempStr);

		// Encryption
		switch(m_iSecurity)
		{
		case DISABLED:
			m_iEncryption = ENCRYPTION_DISABLED;
//			tempStr.LoadString(IDS_DISABLED);
			break;
		case WEP64:
			m_iEncryption = ENCRYPTION_WEP;
//			tempStr.LoadString(IDS_AUTHENTIACTION);
//			GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(tempStr);
//			GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(true);
			/*scout 2004/08/23*/
			if (m_iAuthTemp == AUTH_OPEN_SYSTEM)
				tempStr.LoadString(IDS_OPEN_SYSTEM);
			else if(m_iAuthTemp == AUTH_SHARED_KEY)
				tempStr.LoadString(IDS_SHARED_KEY);
			else
                tempStr.LoadString(IDS_AUTO);
			if (tempStr.Compare(m_szEncryptionOld) != 0)
			{
				b_EncryCahnged = true;
				m_iAuth = m_iAuthTemp;
			}
			GetDlgItem(IDC_EDIT_ENCRYPT_NEW)->ShowWindow(true);
			break;
		case WEP128:
			m_iEncryption = ENCRYPTION_WEP;
//			tempStr.LoadString(IDS_AUTHENTIACTION);
//			GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(tempStr);
//			GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(true);
			/*scout 2004/08/23*/
			if (m_iAuthTemp == AUTH_OPEN_SYSTEM)
				tempStr.LoadString(IDS_OPEN_SYSTEM);
			else if(m_iAuthTemp == AUTH_SHARED_KEY)
				tempStr.LoadString(IDS_SHARED_KEY);
			else
                tempStr.LoadString(IDS_AUTO);
			if (tempStr.Compare(m_szEncryptionOld) != 0)
			{
				b_EncryCahnged = true;
				m_iAuth = m_iAuthTemp;
			}
			GetDlgItem(IDC_EDIT_ENCRYPT_NEW)->ShowWindow(true);
			break;
		case WEP152:
			m_iEncryption = ENCRYPTION_WEP;
//			tempStr.LoadString(IDS_AUTHENTIACTION);
//			GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(tempStr);
//			GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(true);

			/*scout 2004/08/23*/
			if (m_iAuthTemp == AUTH_OPEN_SYSTEM)
				tempStr.LoadString(IDS_OPEN_SYSTEM);
			else if(m_iAuthTemp == AUTH_SHARED_KEY)
				tempStr.LoadString(IDS_SHARED_KEY);
			else
                tempStr.LoadString(IDS_AUTO);
			if (tempStr.Compare(m_szEncryptionOld) != 0)
			{
				b_EncryCahnged = true;
				m_iAuth = m_iAuthTemp;
			}
			GetDlgItem(IDC_EDIT_ENCRYPT_NEW)->ShowWindow(true);
			break;
		case WPA_PSK:
//			tempStr.LoadString(IDS_ENCRYPTION);
//			GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(tempStr);
//			GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(true);
			tempInt = pWizardSheet->m_pWizardEncWpapsk->m_iWzEncryption;
			if (tempInt == TKIP)
			{
				tempStr.LoadString(IDS_ENCRYPTION_TKIP);
				m_iEncryption = ENCRYPTION_TKIP;
			}
			else
			{
				tempStr.LoadString(IDS_ENCRYPTION_AES);
				m_iEncryption = ENCRYPTION_AES_CCM;
			}
			if (tempStr.Compare(m_szEncryptionOld) != 0)
				b_EncryCahnged = true;
			GetDlgItem(IDC_EDIT_ENCRYPT_NEW)->ShowWindow(true);
			break;
		case WPA:
			tempStr.LoadString(IDS_ENCRYPTION);
//			GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(tempStr);
//			GetDlgItem(IDC_STATIC_TYPE)->ShowWindow(true);
			tempInt = pWizardSheet->m_pWizardEncWpa->m_iWzEncryptionWpa;
			if (tempInt == TKIP)
			{
				tempStr.LoadString(IDS_ENCRYPTION_TKIP);
				m_iEncryption = ENCRYPTION_TKIP;
			}
			else
			{
				tempStr.LoadString(IDS_ENCRYPTION_AES);
				m_iEncryption = ENCRYPTION_AES_CCM;
			}
			if (tempStr.Compare(m_szEncryptionOld) != 0)
				b_EncryCahnged = true;
			GetDlgItem(IDC_EDIT_ENCRYPT_NEW)->ShowWindow(true);
			break;
		}
		GetDlgItem(IDC_EDIT_ENCRYPT_NEW)->SetWindowText(tempStr);

	if (m_bSecurityEnable)
	{
		// Key
		switch(m_iAuthTemp)
		{
		case AUTH_WPA:
			m_iKeyStrType = TYPE_WPA;
			strcpy(s_szKey1, pWizardSheet->m_pWizardEncWpa->m_szWzKeyWpa);
			break;
		case AUTH_OPEN_SYSTEM:
			if (m_iSecurity == DISABLED)
				break;
		case AUTH_SHARED_KEY:
        case AUTH_AUTO: /*scout 2004/08/23*/
			m_iKeyStrType = pWizardSheet->m_pWizardEncWep->m_iRadioWzKeyType;
			if( m_iKeyStrType == TYPE_HEX )
			{
				strcpy(s_szKey1, pWizardSheet->m_pWizardEncWep->m_cWzKeyHex);
			}
			else
			{
				strcpy(s_szKey1, pWizardSheet->m_pWizardEncWep->m_cWzKeyAscii);
			}
			break;
		case AUTH_WPAPSK:
			m_iKeyStrType = TYPE_WPAPSK;
			strcpy(s_szKey1, pWizardSheet->m_pWizardEncWpapsk->m_cWzKeyWpapsk);
			break;
		}

		// Radius Server
		if (m_iAuthTemp == AUTH_WPA)
			Ip2String(&s_szRadiusServer, pWizardSheet->m_pWizardEncWpa->m_cRadiusServer);
	}

//	if ((m_iAuth == AUTH_OPEN_SYSTEM) && (m_iEncryption == DISABLED))
//		m_iEncryption = NO_CHANGE;

	SetBitmap(IDB_BG_CONFIRMATION);

	m_nType = CBitmapDialog::BITMAP_STRETCH;
	// force the background to repaint
	Invalidate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardSummary::Ip2String(CString *outputStr, char *inputIp)
{
	char address[4],
		 output[17];

	memset(output, 0, sizeof(output));
	memcpy(address, inputIp, sizeof(address));

	sprintf(output, "%u.%u.%u.%u", 
			(unsigned char) address[0],
			(unsigned char) address[1],
			(unsigned char) address[2],
			(unsigned char) address[3]);
	*outputStr = output;
}

void CWizardSummary::OnOK() 
{

	// TODO: Add extra validation here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	CUpgraderView* pGrandFather = (CUpgraderView*)m_pGrandFather;
	char request[256];
	char		chrSendBuffer[64] = "";
	int			nResult;
	int			i = 0;
	int			length;
	int			count;

	memset(request, 0, sizeof(request) );
	memset(chrSendBuffer, 0, sizeof(chrSendBuffer) );

	//UCP_METHOD_DISCOVER
	request[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	request[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

	//Server Mac Address
	request[i++] = pWizardSheet->SerMacAdde[0];
	request[i++] = pWizardSheet->SerMacAdde[1];
	request[i++] = pWizardSheet->SerMacAdde[2];
	request[i++] = pWizardSheet->SerMacAdde[3];
	request[i++] = pWizardSheet->SerMacAdde[4];
	request[i++] = pWizardSheet->SerMacAdde[5];

	request[i++] = (UCP_METHOD_GET_IP >> 8) & 0xff;
	request[i++] = (UCP_METHOD_GET_IP >> 0) & 0xff;

	//Local IP & Port
	CString HostIP;
	char LocalHostIP[225] = "";
	GetLocalIPs (LocalHostIP, sizeof(LocalHostIP));
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


	//UCP_METHOD_SET_WIZARD
	request[i++] = (char)((UCP_METHOD_SET_WIZARD >> 8)&0xff);
	request[i++] = (char)((UCP_METHOD_SET_WIZARD >> 0)&0xff);

	if (s_iDHCPEnable != NO_CHANGE)
	{
		//Set DHCP
		request[i++] = UCP_CODE_USE_DHCP;
		request[i++] = 1;
		request[i++] = s_iDHCPEnable;
	}

	if ((b_IpCahnged) && (iCurrentDHCPEnable == 0))
	{
		//Set IP
		request[i++] = UCP_CODE_IP_ADDR;
		request[i++] = 4;
		request[i++] = s_szIPAddr[0];
		request[i++] = s_szIPAddr[1];
		request[i++] = s_szIPAddr[2];
		request[i++] = s_szIPAddr[3];
	}

	if ((b_MaskCahnged) && (iCurrentDHCPEnable == 0))
	{
		//Set Mask
		request[i++] = UCP_CODE_SUBNET_MASK;
		request[i++] = 4;
		request[i++] = s_szSubMask[0];
		request[i++] = s_szSubMask[1];
		request[i++] = s_szSubMask[2];
		request[i++] = s_szSubMask[3];
	}

	if ((b_GatewayCahnged) && (iCurrentDHCPEnable == 0))
	{
		//Set Gateway
		request[i++] = UCP_CODE_GATEWAY_ADDR;
		request[i++] = 4;
		request[i++] = s_szGateway[0];
		request[i++] = s_szGateway[1];
		request[i++] = s_szGateway[2];
		request[i++] = s_szGateway[3];
	}

//	if (s_szDevName[0] != NO_CHANGE)
//	{
		//Set Device Name
		length = strlen(s_szDevName);
		request[i++] = UCP_CODE_DEVICE_NAME;
		request[i++] = length;
		for ( count=0; count < length; i++, count++ )
			request[i] = s_szDevName[count];
//	}

	if (s_iOpMode != NO_CHANGE)
	{
		//Set Operation Mode
		request[i++] = UCP_CODE_OPMODE;
		request[i++] = 1;
		request[i++] = s_iOpMode;
	}
	
	if (s_szSSID[0] != NO_CHANGE)
	{
		//Set SSID
		length = strlen(s_szSSID);
		request[i++] = UCP_CODE_SSID;
		request[i++] = length;
		for ( count=0; count < length; i++, count++ )
			request[i] = s_szSSID[count];
	}

	if (s_iChannel != NO_CHANGE)
	{
		//Set Channel
		request[i++] = UCP_CODE_CHANNEL;
		request[i++] = 1;
		request[i++] = s_iChannel;
	}

	if (s_iWirelessMode != NO_CHANGE)
	{
		//Set Super G & Wireless Mode
		request[i++] = UCP_CODE_WIRELESS_MODE;
		request[i++] = 1;
		request[i++] = s_iWirelessMode;
	}

	if (m_iAuth != NO_CHANGE)
	{
		//Set Authenetication
		request[i++] = UCP_CODE_SEC_METHOD;
		request[i++] = 1;
		request[i++] = m_iAuth;
	}

	if (m_iEncryption != NO_CHANGE)
	{
		//Set Encryption
		request[i++] = UCP_CODE_ENCRYPTION;
		request[i++] = 1;
		request[i++] = m_iEncryption;
	}

	//Set Key
	if (m_iKeyStrType != NO_CHANGE)
	{
		length = strlen(s_szKey1);
		switch(m_iKeyStrType)
		{
		case TYPE_WPAPSK:
			request[i++] = UCP_CODE_WPA_PSK;
			request[i++] = length;
			break;
		case TYPE_HEX:
		case TYPE_ASCII:
			switch(m_iSecurity)
			{
			case WEP64:
				request[i++] = UCP_CODE_WEPKEY64;
				break;
			case WEP128:
				request[i++] = UCP_CODE_WEPKEY128;
				break;
			case WEP152:
				request[i++] = UCP_CODE_WEPKEY152;
				break;
			}
			request[i++] = m_iKeyStrType;
			break;
		case TYPE_WPA:
			request[i++] = UCP_CODE_RADIUS_SECRET;
			request[i++] = length;
			break;
		}
		for ( count=0; count < length; i++, count++ )
			request[i] = s_szKey1[count];
	}

	// Set Radius Server
	if ((m_iAuthTemp == AUTH_WPA) &&
		(s_szRadiusServer.Compare(pWizardSheet->m_pWizardLogin->m_cRadiusServer) != 0))
	{
		char tempStr[17];
		memset(tempStr, 0, sizeof(tempStr));
		strcpy(tempStr, s_szRadiusServer);
		length = s_szRadiusServer.GetLength();	
		request[i++] = UCP_CODE_RADIUS_SERVER;
		request[i++] = length;
		for ( count=0; count < length; i++, count++ )
			request[i] = tempStr[count];
	}

	if (i == 27)
	{
		CBitmapDialog::OnOK();
		return;
	}
	
	request[i++] = (char)UCP_CODE_END;

	nResult = sendto( pGrandFather->m_Socket, request, i, 0, (SOCKADDR *) &pGrandFather->m_saUdpServ,sizeof ( SOCKADDR_IN ));

	if( nResult == SOCKET_ERROR ) 
	{
			sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
			AfxMessageBox( chrSendBuffer );
			return;
	}
	pGrandFather->SetTimer(TIMER_OVERTIME,2000,NULL);

	CBitmapDialog::OnOK();
}
/*
HBRUSH CWizardSummary::OnCtlColor (CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	int CtrlID;
	bool b_changed = false;
	// TODO: Change any attributes of the DC here
	CtrlID = pWnd->GetDlgCtrlID();
	switch( CtrlID )
	{
		case IDC_EDIT_IP_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_IPMASK_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_GATEWAY_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_DEVNAME_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_OPMODE_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_SSID_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_CHANNEL_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_WMODE_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_AUTH_NEW:
			b_changed = true;
			break;
		case IDC_EDIT_ENCRYPT_NEW:
			b_changed = true;
			break;
	}
	if (b_changed)
		pDC->SetTextColor(RGB(255,0,0));

	return CWizardSummary::OnCtlColor(pDC, pWnd, nCtlColor);
}
*/

void CWizardSummary::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
//		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		dc.SetTextColor(oldColor);

		CBitmapDialog::OnPaint();
	}
}

HBRUSH CWizardSummary::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBitmapDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int CtrlID;
	// TODO: Change any attributes of the DC here
	CtrlID = pWnd->GetDlgCtrlID();

	switch( CtrlID )
	{
		case IDC_EDIT_IP_NEW:
			if (b_IpCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_IPMASK_NEW:
			if (b_MaskCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_GATEWAY_NEW:
			if (b_GatewayCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_DEVNAME_NEW:
			if (b_DevNameCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_OPMODE_NEW:
			if (b_OpModeCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_SSID_NEW:
			if (b_SsidCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_CHANNEL_NEW:
			if (b_ChannelCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_WMODE_NEW:
			if (b_WlanModeCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_AUTH_NEW:
			if (b_AuthCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
		case IDC_EDIT_ENCRYPT_NEW:
			if (b_EncryCahnged)
				pDC->SetTextColor(RGB(0,0,255));
			else
				pDC->SetTextColor(RGB(0,0,125));
			break;
			//////
		case IDC_EDIT_IP_OLD:
		case IDC_EDIT_IPMASK_OLD:
		case IDC_EDIT_GATEWAY_OLD:
		case IDC_EDIT_DEVNAME_OLD:
		case IDC_EDIT_OPMODE_OLD:
		case IDC_EDIT_SSID_OLD:
		case IDC_EDIT_CHANNEL_OLD:
		case IDC_EDIT_WMODE_OLD:
		case IDC_EDIT_AUTH_OLD:
		case IDC_EDIT_ENCRYPT_OLD:
			pDC->SetTextColor(RGB(0,0,125));
			break;
	}

	return hbr;
}

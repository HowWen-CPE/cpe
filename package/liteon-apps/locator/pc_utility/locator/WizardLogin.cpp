// WizardLogin.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardLogin.h"
#include "UpgraderView.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardLogin property page

IMPLEMENT_DYNCREATE(CWizardLogin, CNGWizardPage/*CPropertyPageEx*/)

CWizardLogin::CWizardLogin(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardLogin::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardLogin)
	m_szUserName = _T("");
	m_szPassword = _T("");
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;
	m_iEncryption	= 0;
	m_iWepKeyType	= 0;

	SetWatermark(IDB_BG_LOGIN,	WIZ97_BITMAP_STRETCH);

}

CWizardLogin::~CWizardLogin()
{
}

void CWizardLogin::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardLogin)
	DDX_Text(pDX, IDC_WIZARD_PASSWORD, m_szPassword);
	DDX_Text(pDX, IDC_WIZARD_USERNAME, m_szUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardLogin, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardLogin)
	ON_EN_CHANGE(IDC_WIZARD_USERNAME, OnChangeWizardUsername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardLogin message handlers

LRESULT CWizardLogin::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	CUpgraderView*	pUpgraderView;
//	CWizardSheet*	pParentSheet;
	CNGWizard*		pParentSheet;
	bool			bAuthPass = FALSE;
//	struct			sockaddr_in from;
//	int				len = sizeof( struct sockaddr_in);
	SYSTEMTIME		st;
	int				sysMinute,
					sysSecond;

	pUpgraderView	= (CUpgraderView*)m_pGrandFather;
//	pParentSheet	= (CWizardSheet*)m_pParentSheet;
	pParentSheet	= (CNGWizard*)m_pParentSheet;

	UpdateData(true);
	pUpgraderView->m_strUserName = m_szUserName;
	pUpgraderView->m_strPassword = m_szPassword;
	pUpgraderView->SendAuthentication(UCP_METHOD_GET_WIZARD);
	
	GetSystemTime(&st);
	sysMinute = st.wMinute;
	sysSecond = st.wSecond;

	while(1)
	{
		uDiscover = 0;
		passRecv();
		switch(uDiscover)
		{
		case UCP_METHOD_AUTH:
			pUpgraderView->KillTimer(TIMER_OVERTIME);
			if (uAuthFail)
			{
				AfxMessageBox( "User Name or Password Fail!" );
				return -1;
			}
			else
			{
				pUpgraderView->GetWizardInfo();
				pUpgraderView->SetTimer(TIMER_OVERTIME,2000,NULL);

				//return CPropertyPageEx::OnWizardNext();
			}
			break;
		case UCP_METHOD_GET_WIZARD:
			pUpgraderView->KillTimer(TIMER_OVERTIME);
//			addPropertySheetPage();

			return CNGWizardPage/*CPropertyPageEx*/::OnWizardNext();
		}
		GetSystemTime(&st);
		if (st.wSecond - sysSecond >= 0)
		{
			if (st.wSecond - sysSecond > 2)
				break;
		}
		else
		{
			if ((st.wSecond + 60) - sysSecond > 2)
				break;
		}
	}	
	return -1;
}

BOOL CWizardLogin::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_szPassword = _T("");
	m_szUserName = _T("Admin");
	
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardLogin::OnChangeWizardUsername() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData(true);
}

void CWizardLogin::passRecv()
{
/*
	CUpgraderView*	pUpgraderView;
	bool		bAuthPass = FALSE;
	struct		sockaddr_in from;
	int			len = sizeof( struct sockaddr_in);
	const int	MSG_CHUNK = 512;
	int			status = 0,
				iMACAddrTemp[6],
				iIp[4];
	char		buffer[MSG_CHUNK];
	char		GetTemp,
				GetLen;
	int			index = 0;
	ULONG		uGetIP,
				uPort,
				uFlagRequest,
				uIPConfig,
				uType,
				uUCPClass;

	pUpgraderView = (CUpgraderView*)m_pGrandFather;
	pUpgraderView->m_npBufferIndex = 0;
	memset (buffer, 0, sizeof(buffer));
	memset(m_cRadiusServer, 0, sizeof(m_cRadiusServer));
	status = recvfrom (pUpgraderView->m_Socket, buffer, MSG_CHUNK, 0, (struct sockaddr*)&from, &len);
	if (status > 0 )
	{
		HexToULONG(buffer, 2 , pUpgraderView->m_npBufferIndex , &uGetIP);
		if ( uGetIP == UCP_METHOD_GET_IP)
		{
			HexToint(buffer, 4 , pUpgraderView->m_npBufferIndex , iIp);
			HexToULONG(buffer, 2 , pUpgraderView->m_npBufferIndex , &uPort);
				ULONG Distype;
				HexToULONG(buffer, 2 , pUpgraderView->m_npBufferIndex , &Distype);								
				for (int j = 0; j < 6; j++)
			{
				HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &iMACAddrTemp[j]);
			}
//				char	chrMacAddrTemp[18];				

//				memset(chrMacAddrTemp, 0 , sizeof(chrMacAddrTemp));				

//				_stprintf(chrMacAddrTemp,"%02x.%02x.%02x.%02x.%02x.%02x", 
//						iMACAddrTemp[0], iMACAddrTemp[1], iMACAddrTemp[2], 
//						iMACAddrTemp[3], iMACAddrTemp[4], iMACAddrTemp[5]);							

//				strMACAddress.Format(chrMacAddrTemp);
				
			HexToULONG(buffer, 2 , pUpgraderView->m_npBufferIndex , &uFlagRequest);
			HexToULONG(buffer, 2 , pUpgraderView->m_npBufferIndex , &uIPConfig);
			HexToULONG(buffer, 1 , pUpgraderView->m_npBufferIndex , &uType);
			HexToULONG(buffer, 4 , pUpgraderView->m_npBufferIndex , &uUCPClass);
			HexToULONG(buffer, 2 , pUpgraderView->m_npBufferIndex , &uDiscover);

			if (uDiscover == UCP_METHOD_AUTH)
				HexToULONG(buffer, 1 , pUpgraderView->m_npBufferIndex , &uAuthFail);
			else if (uDiscover == UCP_METHOD_GET_WIZARD)
			{
				for( HexToChar(buffer, 1 , pUpgraderView->m_npBufferIndex , &GetTemp);
				     GetTemp != (char)UCP_CODE_END;
				     HexToChar(buffer, 1 , pUpgraderView->m_npBufferIndex , &GetTemp) )
				{						
					HexToChar(buffer, 1 , pUpgraderView->m_npBufferIndex , &GetLen);
					switch(GetTemp)
					{
					case UCP_CODE_USE_DHCP:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iDHCPEnable);
						break;
					case UCP_CODE_GATEWAY_ADDR:
						HexToChar(buffer, 4 , pUpgraderView->m_npBufferIndex , m_cGateway);
						break;
					case UCP_CODE_SUBNET_MASK:
						HexToChar(buffer, 4 , pUpgraderView->m_npBufferIndex , m_cSubMask);
						break;
					case UCP_CODE_IP_ADDR:
						HexToChar(buffer, 4 , pUpgraderView->m_npBufferIndex , m_cIPAddr);
						break;
					case UCP_CODE_DEVICE_NAME:
						memset(m_cDevName, 0, sizeof(m_cDevName));
						HexToChar(buffer, GetLen , pUpgraderView->m_npBufferIndex , m_cDevName);
						break;
					case UCP_CODE_SSID:
						memset(m_cSSID, 0, sizeof(m_cSSID));
						HexToChar(buffer, GetLen , pUpgraderView->m_npBufferIndex , m_cSSID);
						break;
					case UCP_CODE_CHANNEL:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iChannel);
						if (GetLen == 0)
							m_iChannel = 0;
						break;
					case UCP_CODE_OPMODE:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iOpMode);
						break;
					case UCP_CODE_WIRELESS_MODE:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iWirelessMode);
						break;
					case UCP_CODE_SUPER_G:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iSuperG);
						break;
					case UCP_CODE_SEC_METHOD:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iAuth);
						break;
					case UCP_CODE_ENCRYPTION:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iEncryption);
						break;
					case UCP_CODE_RADIUS_SERVER:
						HexToChar(buffer, GetLen , pUpgraderView->m_npBufferIndex , m_cRadiusServer);
						break;
					case UCP_CODE_WEP_KEY_TYPE:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iWepKeyType);
						break;
					case UCP_CODE_ENCRYPT_ONOFF:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_iEncryptionOnOff);
						break;
					case UCP_CODE_WEP_InputType:
						HexToint(buffer, 1 , pUpgraderView->m_npBufferIndex , &m_WepInputType);
						break;
					default:
						AfxMessageBox("Code Error");
						return;
					}
				}
			}
		}
	}
*/
}

BOOL CWizardLogin::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetWizardButtons(PSWIZB_NEXT|PSWIZB_DISABLEDFINISH);


	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

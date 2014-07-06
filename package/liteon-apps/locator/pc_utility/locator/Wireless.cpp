// Wireless.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "Wireless.h"
#include "global.h"
#include <Iphlpapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWireless dialog


CWireless::CWireless(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CWireless::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWireless)
	m_iChannel = 0;
	m_iEncryption = 0;
	m_sKey = _T("");
	m_hKey = _T("");
	m_iSecurity = 0;
	m_sSsid = _T("");
	m_KeyHex = 0;
	m_iOpMode = 0;
	m_iAuth = 0;
	m_iWirelessMode = 0;

	//}}AFX_DATA_INIT
//	m_brush.CreateStockObject (HOLLOW_BRUSH); 

}

void CWireless::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWireless)
	DDX_CBIndex(pDX, IDC_CHANNEL, m_iChannel);
	DDX_CBIndex(pDX, IDC_ENCRYPTION, m_iEncryption);
	DDX_Text(pDX, IDC_KEY, m_sKey);
	DDX_Text(pDX, IDC_KEY2, m_hKey);
	DDX_CBIndex(pDX, IDC_SECURITY, m_iSecurity);
	DDX_Text(pDX, IDC_SSID, m_sSsid);
	DDV_MaxChars(pDX, m_sSsid, 32);
	DDX_Radio(pDX, IDC_KEY_HEX, m_KeyHex);
	DDX_CBIndex(pDX, IDC_OP_MODE, m_iOpMode);
	DDX_CBIndex(pDX, IDC_COMBO_AUTH, m_iAuth);
	DDX_CBIndex(pDX, IDC_WIRELESS_MODE, m_iWirelessMode);
	DDX_Control(pDX, IDC_ADIUS_SERVER, m_ctrlRadiusServer);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_KEY2, m_cHexCtrl);
}

BEGIN_MESSAGE_MAP(CWireless, CBitmapDialog)
	//{{AFX_MSG_MAP(CWireless)
	ON_CBN_SELCHANGE(IDC_SECURITY, OnSelchangeSecurity)
	ON_BN_CLICKED(IDC_KEY_HEX, OnKeyHex)
	ON_BN_CLICKED(IDC_KEY_ASCII, OnKeyAscii)
	ON_EN_CHANGE(IDC_KEY, OnChangeKey)
	ON_EN_CHANGE(IDC_KEY2, OnChangeKey2)
	ON_EN_CHANGE(IDC_SSID, OnChangeSsid)
	ON_CBN_SELCHANGE(IDC_CHANNEL, OnSelchangeChannel)
	ON_CBN_DROPDOWN(IDC_CHANNEL, OnDropdownChannel)
	ON_CBN_SELCHANGE(IDC_OP_MODE, OnSelchangeOpMode)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTH, OnSelchangeComboAuth)
	ON_CBN_SELCHANGE(IDC_ENCRYPTION, OnSelchangeEncryption)
	ON_CBN_SELCHANGE(IDC_WIRELESS_MODE, OnSelchangeWirelessMode)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_ADIUS_SERVER, OnFieldchangedWzAdiusServer)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWireless message handlers

BOOL CWireless::OnInitDialog() 
{
	CBitmapDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
    /*Scoutlee Begin 2004/09/07*/

    
	CString strRadiusServer;

//	memcpy(strRadiusServer, pWizardSheet->m_pWizardLogin->m_cRadiusServer, sizeof(strRadiusServer));
	strRadiusServer = RadiusServerinwireless;
	s_szRadiusServer=RadiusServerinwireless;
	
	if (strRadiusServer.IsEmpty() == false)
	{
		for(int i = 0; i < 4; i++)
		{
			CString StrTemp;

			int pos = strRadiusServer.Find('.',0);
			if ( pos != -1 )
				StrTemp = strRadiusServer.Left(pos);
			else
				StrTemp = strRadiusServer;

			int tt;
			tt = atoi(StrTemp);
			m_cRadiusServer[i] = (char)tt;
			strRadiusServer.Delete(0,pos+1);
		}
		m_ctrlRadiusServer.SetAddress(m_cRadiusServer[0],
									  m_cRadiusServer[1],
									  m_cRadiusServer[2],
									  m_cRadiusServer[3]);
	}


//	if (&m_brush != NULL)
//		m_brush.DeleteObject();

//	m_brush.CreateSolidBrush(RGB(255, 255, 255)); // color white brush 

//	m_oldChannel = m_iChannel;
	GetDlgItem(IDOK)->EnableWindow(true);

	switch(m_iOpModeReal)
	{
	case CFG_OP_MODE_AP:
		m_iOpMode = SEL_AP_MODE;
		break;
	case CFG_OP_MODE_STA0:
		m_iOpMode = SEL_INF;
		break;
	case CFG_OP_MODE_ADHOC:
		m_iOpMode = SEL_ADHOC;
		break;
	case CFG_OP_MODE_REPEATER:
		m_iOpMode = SEL_REPEATER;
		break;
	}
//	m_preOpMode = m_iOpMode;

	if ((m_iSuperG & ABOLT_TURBO_G) && (m_iSuperG & ABOLT_TURBO_PRIME))
		m_iWirelessMode = W_802_11SUPER_G;
	else
	{
		switch(m_iWirelessModeReal)
		{
		case MODE_SELECT_11B:
			m_iWirelessMode = W_802_11B;
			break;
		case MODE_SELECT_11G:
			m_iWirelessMode = W_802_11G;
			break;
		/*Scoutlee Begin  2004/09/16 */
		case MODE_SELECT_108G:
			m_iWirelessMode = W_802_11Static_G;
			break;
			
		/*Scoutlee Begin  2004/09/16 */
		}
	}
	UpdateData(false);

	((CComboBox*)GetDlgItem(IDC_CHANNEL))->SetCurSel(m_iChannel);
	((CComboBox*)GetDlgItem(IDC_SECURITY))->SetCurSel(0);
	
	if (m_iOpMode == 1)
		GetDlgItem(IDC_CHANNEL)->EnableWindow(FALSE);

	OnSelchangeOpMode();

	m_preChannel		= m_iChannel;
//	m_preSecurity		= m_iSecurity;
//	m_preSsid			= m_sSsid;
//	m_preWirelessMode	= m_iWirelessMode;
	SetBitmap(IDB_WIRELESS);

	m_nType = CBitmapDialog::BITMAP_STRETCH;
	// force the background to repaint
	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWireless::OnSelchangeSecurity() 
{
	// TODO: Add your control notification handler code here
 	CString m_str, m_keystr;
	CString tempStr;
	CRect rectFrame;
	int tempInt;
    
	GetDlgItem(IDOK)->EnableWindow(false);
	GetDlgItem(IDC_STATIC_KEYINPUT)->GetClientRect(&rectFrame);

	m_iSecurity = ((CComboBox*)GetDlgItem(IDC_SECURITY))->GetCurSel();

	switch(m_iSecurity)
	{
	case DISABLED:
		GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(false);
		GetDlgItem(IDC_COMBO_AUTH)->ShowWindow(false);
		GetDlgItem(IDC_ENCRYPTION)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEY1)->ShowWindow(false);
		GetDlgItem(IDC_KEY)->ShowWindow(false);
		GetDlgItem(IDC_KEY2)->ShowWindow(false);
		GetDlgItem(IDC_KEY_HEX)->ShowWindow(false);
		GetDlgItem(IDC_KEY_ASCII)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->ShowWindow(false);		
		GetDlgItem(IDC_ADIUS_SERVER)->ShowWindow(false);	

		//--------------------------------------
		m_iEncryptionReal = ENCRYPTION_DISABLED;
		m_iAuthReal = AUTH_OPEN_SYSTEM;
		break;
	case WPA_PSK:
		m_sKey="";
		UpdateData(false);
		tempStr.LoadString(IDS_ENCRYPTION);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->SetWindowText(tempStr);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(true);

		tempStr.LoadString(IDS_TEXT_KEY_STR_PRESHARED);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEYINPUT)->SetWindowText(tempStr);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(true);
		GetDlgItem(IDC_COMBO_AUTH)->ShowWindow(false);
		GetDlgItem(IDC_ENCRYPTION)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_KEY1)->ShowWindow(true);
		GetDlgItem(IDC_KEY_HEX)->ShowWindow(false);
		GetDlgItem(IDC_KEY_ASCII)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->ShowWindow(false);
		GetDlgItem(IDC_ADIUS_SERVER)->ShowWindow(false);
		m_keystr.LoadString(IDS_TEXT_KEY_PSK);
//		rectFrame.left = 135;
//		rectFrame.top = 255;
//		rectFrame.bottom = rectFrame.top + 20;
//		rectFrame.right = rectFrame.left + 260;
//		GetDlgItem(IDC_STATIC_KEYINPUT)->MoveWindow(rectFrame);
//		m_KeyHex = 1;
//		UpdateData(false);
		//--------------------------------------
		tempInt = ((CComboBox*)GetDlgItem(IDC_ENCRYPTION))->GetCurSel();
		if (tempInt == 0)
			m_iEncryptionReal = ENCRYPTION_TKIP;
		else
			m_iEncryptionReal = ENCRYPTION_AES_CCM;
		m_iAuthReal = AUTH_WPAPSK;
		OnKeyAscii();
		break;
	case WPA:
		m_sKey="";
		UpdateData(false);
		tempStr.LoadString(IDS_ENCRYPTION);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->SetWindowText(tempStr);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(true);

		tempStr.LoadString(IDS_TEXT_KEY_WPA_IP);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->SetWindowText(tempStr);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->ShowWindow(true);
		
		tempStr.LoadString(IDS_TEXT_KEY_WPA_Radius);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEYINPUT)->SetWindowText(tempStr);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(true);


		GetDlgItem(IDC_ADIUS_SERVER)->ShowWindow(true);
		GetDlgItem(IDC_COMBO_AUTH)->ShowWindow(false);
		GetDlgItem(IDC_ENCRYPTION)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_KEY1)->ShowWindow(false);
		GetDlgItem(IDC_KEY_HEX)->ShowWindow(false);
		GetDlgItem(IDC_KEY_ASCII)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(true);
		m_keystr.LoadString(IDS_TEXT_KEY_WPA);

		//--------------------------------------
		tempInt = ((CComboBox*)GetDlgItem(IDC_ENCRYPTION))->GetCurSel();
		if (tempInt == 0)
			m_iEncryptionReal = ENCRYPTION_TKIP;
		else
			m_iEncryptionReal = ENCRYPTION_AES_CCM;
		m_iAuthReal = AUTH_WPA;
		OnKeyAscii();
		break;
	default:
		tempStr.LoadString(IDS_TEXT_KEY_WEP_INPUT);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->SetWindowText(tempStr);
		GetDlgItem(IDC_STATIC_KEY_TYPE)->ShowWindow(true);

		tempStr.LoadString(IDS_AUTHENTIACTION);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->SetWindowText(tempStr);
		GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(true);
		if ( (m_iOpMode != SEL_ADHOC) && (m_iOpMode != SEL_REPEATER) )
		{
			GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(true);
			GetDlgItem(IDC_COMBO_AUTH)->ShowWindow(true);
		}
		else
		{
			GetDlgItem(IDC_STATIC_ENCRYPTION)->ShowWindow(false);
			GetDlgItem(IDC_COMBO_AUTH)->ShowWindow(false);
		}
			/*scout 2004/08/23 if APmode delet Auto  selection */
            if  (m_iOpMode == SEL_AP_MODE){
				((CComboBox*)GetDlgItem(IDC_COMBO_AUTH))->DeleteString( 2 ); 
				m_iAuth=0;
				UpdateData(false);
				
            }else{
				int nIndex = 0;
				if ((nIndex=((CComboBox*)GetDlgItem(IDC_COMBO_AUTH))->FindString(nIndex, "Auto")) == CB_ERR){
					((CComboBox*)GetDlgItem(IDC_COMBO_AUTH))->AddString("Auto");
				}
			}
		GetDlgItem(IDC_ENCRYPTION)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEY1)->ShowWindow(true);
		GetDlgItem(IDC_KEY)->ShowWindow(true);
		GetDlgItem(IDC_KEY_HEX)->ShowWindow(true);
		GetDlgItem(IDC_KEY_ASCII)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(true);
		GetDlgItem(IDC_ADIUS_SERVER)->ShowWindow(false);
		m_keystr.LoadString(IDS_TEXT_KEY_WEP);
//		rectFrame.left = 253;
//		rectFrame.top = 255;
//		rectFrame.bottom = rectFrame.top + 20;
//		rectFrame.right = rectFrame.left + 225;
//		GetDlgItem(IDC_STATIC_KEYINPUT)->MoveWindow(rectFrame);
		//--------------------------------------
		tempInt = ((CComboBox*)GetDlgItem(IDC_OP_MODE))->GetCurSel();
		if ( (tempInt == SEL_ADHOC) || (tempInt == SEL_REPEATER) )
			m_iAuthReal = AUTH_OPEN_SYSTEM;
		else
		{
			/*scout add 2004/08/23*/
			if ( ((CComboBox*)GetDlgItem(IDC_COMBO_AUTH))->GetCurSel() == 0 )
				m_iAuthReal = AUTH_OPEN_SYSTEM;
			else if ( ((CComboBox*)GetDlgItem(IDC_COMBO_AUTH))->GetCurSel() == 1 )
				m_iAuthReal = AUTH_OPEN_SYSTEM;
			else
				m_iAuthReal = AUTH_AUTO;
		}
		m_iEncryptionReal = ENCRYPTION_WEP;
		break;
	}

	if ((m_iSecurity != DISABLED)&&(m_iSecurity!=WPA)&&(m_iSecurity!=WPA_PSK))
	{
		if ( m_KeyHex == 0)
			OnKeyHex();
		else
			OnKeyAscii();
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(true);
		OnKeyAscii();
	}
//	GetDlgItem(IDC_STATIC_KEYINPUT)->SetWindowText(m_str);
	GetDlgItem(IDC_STATIC_KEY1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_KEY1)->SetWindowText(m_keystr);
	GetDlgItem(IDC_STATIC_KEY1)->ShowWindow(true);
}

void CWireless::OnKeyHex() 
{
	// TODO: Add your control notification handler code here
 	CString m_str;
	int len;

	UpdateData(TRUE);

//	m_sKey.Empty();

	switch(m_iSecurity)
	{
	case WEP64:
		len = 10;
		m_str.LoadString(IDS_TEXT_KEY_HEX64);
		break;
	case WEP128:
		len = 26;
		m_str.LoadString(IDS_TEXT_KEY_HEX128);
		break;
	case WEP152:
		len = 32;
		m_str.LoadString(IDS_TEXT_KEY_HEX152);
		break;
	}
	if ((m_iSecurity != WPA_PSK)&&(m_iSecurity != WPA)){
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEYINPUT)->SetWindowText(m_str);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(true);
	}
	((CEdit*)GetDlgItem(IDC_KEY2))->SetLimitText(len);
	if ( m_hKey.GetLength() > len)
	{
		m_hKey = m_hKey.Left(len);
		SetDlgItemText(IDC_KEY2, m_hKey);
		GetDlgItem(IDOK)->EnableWindow(true);
	}
	else if (m_hKey.GetLength() == len)
		GetDlgItem(IDOK)->EnableWindow(true);
	else
			GetDlgItem(IDOK)->EnableWindow(false);
	if (m_iSecurity != DISABLED)
		((CEdit*)GetDlgItem(IDC_KEY2))->ShowWindow(true);
	else
		((CEdit*)GetDlgItem(IDC_KEY2))->ShowWindow(false);

	((CEdit*)GetDlgItem(IDC_KEY))->ShowWindow(false);

	UpdateData(FALSE);
}

void CWireless::OnKeyAscii() 
{
	// TODO: Add your control notification handler code here
 	CString m_str;
	int len;

	UpdateData(TRUE);

//	m_sKey.Empty();

	switch(m_iSecurity)
	{
	case WEP64:
		len = 5;
		m_str.LoadString(IDS_TEXT_KEY_STRING64);
		break;
	case WEP128:
		len = 13;
		m_str.LoadString(IDS_TEXT_KEY_STRING128);
		break;
	case WEP152:
		len = 16;
		m_str.LoadString(IDS_TEXT_KEY_STRING152);
		break;
	case WPA_PSK:
		len = 64;
		m_str.LoadString(IDS_TEXT_KEY_STR_PRESHARED);
		break;
	case WPA:
		len = 64;
		m_str.LoadString(IDS_TEXT_KEY_WPA_Radius);
		break;
	}
	if ((m_iSecurity != WPA_PSK)&&(m_iSecurity != WPA)){
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_KEYINPUT)->SetWindowText(m_str);
		GetDlgItem(IDC_STATIC_KEYINPUT)->ShowWindow(true);
	}
	((CEdit*)GetDlgItem(IDC_KEY))->SetLimitText(len);

	if (m_iSecurity == WPA_PSK)
	{
		if ( (m_sKey.GetLength() <8) || (m_sKey.GetLength() > 63) )
			GetDlgItem(IDOK)->EnableWindow(false);
		else
			GetDlgItem(IDOK)->EnableWindow(true);
	}else if(m_iSecurity == WPA){
		if (m_sKey.GetLength() > 64) 
			GetDlgItem(IDOK)->EnableWindow(false);
		else
			GetDlgItem(IDOK)->EnableWindow(true);
	}	
	else
	{
		if(m_iSecurity != DISABLED){
			if ( m_sKey.GetLength() > len)
			{
				m_sKey = m_sKey.Left(len);
				SetDlgItemText(IDC_KEY, m_sKey);
				GetDlgItem(IDOK)->EnableWindow(true);
			}
			else if (m_sKey.GetLength() == len)
				GetDlgItem(IDOK)->EnableWindow(true);
			else
				GetDlgItem(IDOK)->EnableWindow(false);
		}
	}
	((CEdit*)GetDlgItem(IDC_KEY2))->ShowWindow(false);
	if (m_iSecurity != DISABLED)
		((CEdit*)GetDlgItem(IDC_KEY))->ShowWindow(true);
	else
		((CEdit*)GetDlgItem(IDC_KEY))->ShowWindow(false);

	UpdateData(FALSE);
}

void CWireless::OnChangeKey() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int length;

	GetDlgItemText(IDC_KEY, m_sKey);
	length = m_sKey.GetLength();

	switch(m_iSecurity)
	{
	case WPA_PSK:
		if ((length > 7) && (length < 64))
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	case WPA:
		if (length < 65)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	case DISABLED:
		GetDlgItem(IDOK)->EnableWindow(true);
		break;
	case WEP64:
		if (length == 5)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	case WEP128:
		if (length == 13)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	case WEP152:
		if (length == 16)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	}
}

void CWireless::OnChangeKey2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int length;

	GetDlgItemText(IDC_KEY2, m_hKey);
	length = m_hKey.GetLength();

	switch(m_iSecurity)
	{
	case WPA_PSK:
	case WPA:
		break;
	case DISABLED:
		GetDlgItem(IDOK)->EnableWindow(true);
		break;
	case WEP64:
		if (length == 10)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	case WEP128:
		if (length == 26)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	case WEP152:
		if (length == 32)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
		break;
	}
	
}

void CWireless::OnOK() 
{
	// TODO: Add extra validation here
	CString		keyTemp;
	char		request[256];
	char		chrSendBuffer[64] = "";
	char		tempString[64];
	int			nResult;
	int			i = 0,
				count,
				length;

//	if ((m_preChannel != m_iChannel) ||
//		(m_preSecurity != m_iSecurity) ||
//		(m_preOpMode != m_iOpMode) ||
//		(m_preSsid.Compare( m_sSsid ) != 0))
//	{
	memset(request, 0, sizeof(request) );
	memset(chrSendBuffer, 0, sizeof(chrSendBuffer) );

	//UCP_METHOD_DISCOVER
	request[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	request[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

	//Server Mac Address
	request[i++] = SerMacAdde[0];
	request[i++] = SerMacAdde[1];
	request[i++] = SerMacAdde[2];
	request[i++] = SerMacAdde[3];
	request[i++] = SerMacAdde[4];
	request[i++] = SerMacAdde[5];

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

	//UCP_METHOD_SET_WLAN
	request[i++] = (char)((UCP_METHOD_SET_WLAN >> 8)&0xff);
	request[i++] = (char)((UCP_METHOD_SET_WLAN >> 0)&0xff);

	//Set Operation Mode
	request[i++] = UCP_CODE_OPMODE;
	request[i++] = 1;
	request[i++] = m_iOpModeReal;
	
	//Set SSID
	memset(tempString, 0, sizeof(tempString));
	length = m_sSsid.GetLength();
	lstrcpy(tempString, m_sSsid);

	request[i++] = UCP_CODE_SSID;
	request[i++] = length;
	for ( count=0; count < length; i++, count++ )
		request[i] = tempString[count];

	//Set Channel
//	if (m_oldChannel != m_iChannel)
//	{
	request[i++] = UCP_CODE_CHANNEL;
	request[i++] = 1;
	request[i++] = m_iChannel;
//	}

	//Set Super G & Wireless Mode
	request[i++] = UCP_CODE_WIRELESS_MODE;
	request[i++] = 1;
	request[i++] = m_iWirelessModeReal;
//	if (m_iSuperG != ( ABOLT_SUPERG | ABOLT_TURBO_PRIME ))
	
	//Set Authenetication
	request[i++] = UCP_CODE_SEC_METHOD;
	request[i++] = 1;
	request[i++] = m_iAuthReal;

	//Set Encryption
	request[i++] = UCP_CODE_ENCRYPTION;
	request[i++] = 1;
//	if (m_iSecurity == 4)//WPA-PSK
//		request[i++] = m_iEncryption;
//	else
//		request[i++] = 2;
	request[i++] = m_iEncryptionReal;

	//Set Key
	if (m_KeyHex == 0)
		keyTemp = m_hKey;
	else
		keyTemp = m_sKey;
	/*scoutlee begin 2004/09/07*/
	if((m_iSecurity==4)||(m_iSecurity==5)) //WPA_PSK && WPA
		keyTemp = m_sKey;
	/*scoutlee end 2004/09/07*/
	length = keyTemp.GetLength();

	if (m_iSecurity)
	{
		switch(m_iSecurity)
		{
		case 1://WEP_64
			request[i++] = UCP_CODE_WEPKEY64;
			request[i++] = m_KeyHex;
			break;
		case 2://WEP_128
			request[i++] = UCP_CODE_WEPKEY128;
			request[i++] = m_KeyHex;
			break;
		case 3://WEP_152
			request[i++] = UCP_CODE_WEPKEY152;
			request[i++] = m_KeyHex;
			break;
		case 4://WPA_PSK
			request[i++] = UCP_CODE_WPA_PSK;
			request[i++] = length;
			break;
		case 5:
			request[i++] = UCP_CODE_RADIUS_SECRET;
			request[i++] = length;
			break;
		}

		memset(tempString, 0, sizeof(tempString));
		lstrcpy(tempString, keyTemp);

		for ( count=0; count < length; i++, count++ )
			request[i] = tempString[count];
	}
	/*Scoutlee Begin 2004/09/07*/
	// Set Radius Server
	if ((m_iSecurity==5) && 
			(s_szRadiusServer.Compare(RadiusServerinwireless) != 0))
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
	/*Scoutlee end 2004/09/07*/
	//End of code
	request[i++] = (char)UCP_CODE_END;

	nResult = sendto(	m_Socket, request, i, 0, (SOCKADDR *) &saUdpServ,sizeof ( SOCKADDR_IN ));

	if( nResult == SOCKET_ERROR ) 
	{
			sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
			AfxMessageBox( chrSendBuffer );
			return;
	}
	CBitmapDialog::OnOK();
//	}
//	else
//		CDialog::OnCancel();
}

void CWireless::GetLocalIPs(char *localips)
{
	PIP_ADAPTER_INFO	pinfo;
	BYTE				m_data[4096];
	unsigned long		len = 0;

	ZeroMemory( m_data,4096 );
	pinfo = ( PIP_ADAPTER_INFO )m_data;

	unsigned long nError;

	nError = GetAdaptersInfo( pinfo,&len );
	switch( nError ) {
		case 0:
			break;
		case ERROR_BUFFER_OVERFLOW:
			nError = GetAdaptersInfo( pinfo,&len );
			if( nError == 0 )	break;
			else MessageBox("Get local IP Error!","Error",MB_OK);
			break;
	}

	PIP_ADDR_STRING pAddressList = &(pinfo->IpAddressList);

	_stprintf(localips,"%s",pAddressList->IpAddress.String);

	LocalMacAddr[0] = pinfo->Address[0];
	LocalMacAddr[1] = pinfo->Address[1];
	LocalMacAddr[2] = pinfo->Address[2];
	LocalMacAddr[3] = pinfo->Address[3];
	LocalMacAddr[4] = pinfo->Address[4];
	LocalMacAddr[5] = pinfo->Address[5];
}

void CWireless::OnChangeSsid() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_SSID, m_sSsid);
	
}
void CWireless::OnFieldchangedWzAdiusServer(NMHDR* nmhdr, LRESULT* lresult) 
{
	// TODO: Add your control notification handler code here

	m_ctrlRadiusServer.GetAddress(  (unsigned char &)m_cRadiusServer[0],
									(unsigned char &)m_cRadiusServer[1],
									(unsigned char &)m_cRadiusServer[2],
									(unsigned char &)m_cRadiusServer[3]);
		Ip2String(&s_szRadiusServer, m_cRadiusServer);
	
}
void CWireless::OnSelchangeChannel() 
{
	// TODO: Add your control notification handler code here
	m_iChannel = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
}

void CWireless::OnDropdownChannel() 
{
	// TODO: Add your control notification handler code here
//	m_oldChannel = m_iChannel;
}

void CWireless::OnSelchangeOpMode() 
{
	// TODO: Add your control notification handler code here
	m_iOpMode = ((CComboBox*)GetDlgItem(IDC_OP_MODE))->GetCurSel();

	switch(m_iOpMode)
	{
	case SEL_AP_MODE:
		m_iChannel = m_preChannel;
		UpdateData(false);
		if ( m_iWirelessMode != W_802_11SUPER_G )
			GetDlgItem(IDC_CHANNEL)->EnableWindow(true);
		else
			GetDlgItem(IDC_CHANNEL)->EnableWindow(false);

		m_iOpModeReal = CFG_OP_MODE_AP;
//		GetDlgItem(IDC_WIRELESS_MODE)->EnableWindow(true);

		AddComboString( (CComboBox*)GetDlgItem(IDC_SECURITY), IDS_SECURITY_WPA);
		UpdateData(false);
		break;
	case SEL_INF:
		m_preChannel = m_iChannel;
		m_iChannel = 0;
		UpdateData(false);
		GetDlgItem(IDC_CHANNEL)->EnableWindow(false);

		m_iOpModeReal = CFG_OP_MODE_STA0;
//		GetDlgItem(IDC_WIRELESS_MODE)->EnableWindow(true);
		AddComboString( (CComboBox*)GetDlgItem(IDC_SECURITY), IDS_SECURITY_WPA_PSK	);
		UpdateData(false);
		break;
	case SEL_ADHOC:
		m_iChannel = m_preChannel;
		UpdateData(false);
		if ( m_iWirelessMode != W_802_11SUPER_G )
			GetDlgItem(IDC_CHANNEL)->EnableWindow(true);
		else
			GetDlgItem(IDC_CHANNEL)->EnableWindow(false);

		m_iOpModeReal = CFG_OP_MODE_ADHOC;
//		((CComboBox*)GetDlgItem(IDC_WIRELESS_MODE))->SetCurSel(0);
//		OnSelchangeWirelessMode();
//		GetDlgItem(IDC_WIRELESS_MODE)->EnableWindow(false);
		AddComboString( (CComboBox*)GetDlgItem(IDC_SECURITY), IDS_SECURITY);
		if (m_iSecurity > 3)
			((CComboBox*)GetDlgItem(IDC_SECURITY))->SetCurSel(0);
		UpdateData(false);
		break;
	case SEL_REPEATER:
		m_iChannel = 0;
		UpdateData(false);

		GetDlgItem(IDC_CHANNEL)->EnableWindow(false);
		m_iOpModeReal = CFG_OP_MODE_REPEATER;
//		GetDlgItem(IDC_WIRELESS_MODE)->EnableWindow(true);
		AddComboString( (CComboBox*)GetDlgItem(IDC_SECURITY), IDS_SECURITY);
		if (m_iSecurity > 3)
			((CComboBox*)GetDlgItem(IDC_SECURITY))->SetCurSel(0);
		UpdateData(false);
		break;
	}
	OnSelchangeWirelessMode();
	OnSelchangeSecurity();
}

/*
HBRUSH CWireless::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired

	switch (nCtlColor) 
		{
		case CTLCOLOR_STATIC:
			pDC->SetBkMode (TRANSPARENT); 
			return (HBRUSH) m_brush.m_hObject;
			break;
		default:
			HBRUSH hbr = CBitmapDialog::OnCtlColor (pDC, pWnd, nCtlColor);
			return hbr ; 
			break; 
		}
		
		return hbr ; 
}
*/
void CWireless::OnSelchangeComboAuth() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
/*scout add 2004/08/23*/
	if (m_iAuth == 0)
		m_iAuthReal = AUTH_OPEN_SYSTEM;
	else if (m_iAuth == 1)
		m_iAuthReal = AUTH_SHARED_KEY;
	else 
		m_iAuthReal = AUTH_AUTO;
}

void CWireless::OnSelchangeEncryption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if (m_iEncryption == 0)
		m_iEncryptionReal = ENCRYPTION_TKIP;
	else //if (m_iEncryption == 1)
		m_iEncryptionReal = ENCRYPTION_AES_CCM;
}

void CWireless::OnSelchangeWirelessMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	switch(m_iWirelessMode)
	{
	case W_802_11B:
//		m_iChannel = m_oldChannel;
		UpdateData(false);
		m_iWirelessModeReal = W_802_11B;
	//	m_iSuperG &= ~(ABOLT_TURBO_PRIME|ABOLT_FRIENDLY_TURBO);
		if ( (m_iOpMode != SEL_REPEATER) && (m_iOpMode != SEL_INF) )
			GetDlgItem(IDC_CHANNEL)->EnableWindow(true);
		break;
	case W_802_11G:
//		m_iChannel = m_oldChannel;
		UpdateData(false);
		m_iWirelessModeReal = W_802_11G;
	//	m_iSuperG &= ~(ABOLT_TURBO_PRIME|ABOLT_FRIENDLY_TURBO);
		if ( (m_iOpMode != SEL_REPEATER) && (m_iOpMode != SEL_INF) )
			GetDlgItem(IDC_CHANNEL)->EnableWindow(true);
		break;
	case W_802_11Static_G:    //108g static
//		m_oldChannel = m_iChannel;
		m_iChannel = 0;
		UpdateData(false);
		m_iWirelessModeReal = W_802_11Static_G;
	//	m_iSuperG &= ~(ABOLT_TURBO_PRIME|ABOLT_FRIENDLY_TURBO);
		GetDlgItem(IDC_CHANNEL)->EnableWindow(false);
		break;
	case W_802_11SUPER_G:     //108g dynamic  
//		m_oldChannel = m_iChannel;
		m_iChannel = 0;
		UpdateData(false);
		m_iWirelessModeReal = W_802_11SUPER_G;
	//	m_iSuperG &= ~(ABOLT_FRIENDLY_TURBO);
		GetDlgItem(IDC_CHANNEL)->EnableWindow(false);
		break;
	}
}

void CWireless::AddComboString(CComboBox *pCombo, unsigned int IDS_xxx)
{
    CString m_str;
    m_str.LoadString(IDS_xxx);

    CString strTemp = m_str;
    CString strAdder;
    int nEnd;

    pCombo->ResetContent();
    while ((nEnd = strTemp.Find('\n')) > 0)
    {
        strAdder = strTemp.Left(nEnd);
        strTemp = strTemp.Mid(nEnd+1);
        pCombo->AddString(strAdder);
    }
}

void CWireless::OnPaint() 
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
		CBitmapDialog::OnPaint();
	}
}
void CWireless::Ip2String(CString *outputStr, char *inputIp)
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
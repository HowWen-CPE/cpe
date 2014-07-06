// IpConfig.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "IpConfig.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIpConfig* m_DlgIPConfig;

bool flag = false;
/////////////////////////////////////////////////////////////////////////////
// CIpConfig dialog


CIpConfig::CIpConfig(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CIpConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIpConfig)
//	m_bDhcpEnable = FALSE;
	m_iDhcpEnable = 0;
	//}}AFX_DATA_INIT
	m_DlgIPConfig = this;
//	m_brush.CreateStockObject (HOLLOW_BRUSH);
	m_pParent = pParent;
}


void CIpConfig::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpConfig)
	DDX_Control(pDX, IDC_SUB_MASK, m_CfgSubMask);
	DDX_Control(pDX, IDC_GATEWAY, m_CfgGateway);
	DDX_Control(pDX, IDC_IPADDRESS, m_CfgIPAddr);
//	DDX_Check(pDX, IDC_DHCPENABLE, m_bDhcpEnable);
	DDX_Radio(pDX, IDC_RADIO_DHCP_ENABLE, m_iDhcpEnable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIpConfig, CBitmapDialog)
	//{{AFX_MSG_MAP(CIpConfig)
	ON_BN_CLICKED(IDC_DHCPENABLE, OnDhcpenable)
	ON_BN_CLICKED(IDC_RADIO_DHCP_ENABLE, OnRadioDhcpEnable)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioDhcp)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, OnFieldchangedIpaddress)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_GATEWAY, OnFieldchangedGateway)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CIpConfig::OnBnClickedOk)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SUB_MASK, &CIpConfig::OnIpnFieldchangedSubMask)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpConfig message handlers

BOOL CIpConfig::OnInitDialog() 
{
	CBitmapDialog::OnInitDialog();
	// TODO: Add extra initialization here
	bool display;
	int i;

//	if (&m_brush != NULL)
//		m_brush.DeleteObject();

//	m_brush.CreateSolidBrush(RGB(255, 255, 255)); // color white brush
	for (i = 0; i < 4; i++)
	{
		m_oldGateway[i] = m_chrGateway[i];
		m_oldGetIP[i] = m_chrGetIP[i];
	}
	
	m_CfgIPAddr.SetAddress(m_chrGetIP[0],m_chrGetIP[1],m_chrGetIP[2],m_chrGetIP[3]);
	m_CfgSubMask.SetAddress(m_chrGetSubMask[0],m_chrGetSubMask[1],m_chrGetSubMask[2],m_chrGetSubMask[3]);
	if ((m_chrGateway[0] != 0) || (m_chrGateway[1] != 0) || (m_chrGateway[2] != 0) || (m_chrGateway[3] != 0))
		m_CfgGateway.SetAddress(m_chrGateway[0],m_chrGateway[1],m_chrGateway[2],m_chrGateway[3]);
	m_CfgIPAddr.SetFieldRange( 0, 1, 223 );
	m_CfgGateway.SetFieldRange( 0, 1, 223 );

	for (i = 0; i < 4; i++)
	{
		preGateway[i] = m_chrGateway[i];
		preGetIP[i] = m_chrGetIP[i];
		preGetSubMask[i] = m_chrGetSubMask[i];
	}
	/*
	memset(m_chrGetIP,NULL,4);
	memset(m_chrGetSubMask,NULL,4);
	memset(m_chrGateway,NULL,4);
	m_CfgGateway.GetAddress(m_chrGetIP[0],m_chrGetIP[1],m_chrGetIP[2],m_chrGetIP[3]);
	m_CfgGateway.SetAddress(m_chrGetIP[0],m_chrGetIP[1],m_chrGetIP[2],m_chrGetIP[3]);
	*/
	
	if (m_iDhcpEnable == 1)
		display = FALSE;
	else
		display = TRUE;
	preDhcpEnable = m_iDhcpEnable;
	
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(display);
	GetDlgItem(IDC_SUB_MASK)->EnableWindow(display);
	GetDlgItem(IDC_GATEWAY)->EnableWindow(display);

	SetBitmap(IDB_IP_SETTING);

	//happiness add 20110510
	//如果是router，则将动态获取ip给灰掉,将gateway灰掉
	if(m_bridge== 0x02){
		GetDlgItem(IDC_RADIO3)->EnableWindow(false);
		GetDlgItem(IDC_GATEWAY)->EnableWindow(false);
	}
	//-----------------------------------------
	//happiness add 20110525
	//如果ip为全零，则都灰掉
	//if(m_chrGetIP[0]==0&&m_chrGetIP[1]==0&&m_chrGetIP[2]==0&&m_chrGetIP[3]==0){
	//	GetDlgItem(IDC_RADIO3)->EnableWindow(false);
	//	GetDlgItem(IDC_RADIO_DHCP_ENABLE)->EnableWindow(false);
	//}

	m_nType = CBitmapDialog::BITMAP_STRETCH;
	// force the background to repaint
	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIpConfig::OnOK() 
{
	// TODO: Add extra validation here
	char	request[128];
	char	chrSendBuffer[64] = "";
	int		nResult = 0;
	int		i = 0;
	bool	b_change = false;
	CString	errorMsg;

	m_CfgIPAddr.GetAddress(m_chrGetIP[0],m_chrGetIP[1],m_chrGetIP[2],m_chrGetIP[3]);
	m_CfgSubMask.GetAddress(m_chrGetSubMask[0],m_chrGetSubMask[1],m_chrGetSubMask[2],m_chrGetSubMask[3]);
	m_CfgGateway.GetAddress(m_chrGateway[0],m_chrGateway[1],m_chrGateway[2],m_chrGateway[3]);

	switch (CheckValidIPAndMask())
	{
	case IP_MASK_ERR:
		errorMsg.LoadString(IDS_ERROR_MASK);
		AfxMessageBox(errorMsg);
		return;
	case IP_FIELD4_ZERO_ERR:
		errorMsg.LoadString(IDS_ERR_IPFIELD4_ZERO);
		AfxMessageBox(errorMsg);
		return;
	case IP_FIELD4_FILL_ERR:
		errorMsg.LoadString(IDS_ERR_IPFIELD4_FILL);
		AfxMessageBox(errorMsg);
		return;
	case IP_FIELD1_ERR:
		errorMsg.LoadString(IDS_ERR_IPFIELD4_FILL);
		AfxMessageBox(errorMsg);
		return;
	case IP_OK:
		break;
	}
	
	memset(request, 0, sizeof(request) );
	memset(chrSendBuffer, 0, sizeof(chrSendBuffer) );

	// 设置包0x0001
	request[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	request[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

	//Server Mac Address
	request[i++] = SerMacAdde[0];
	request[i++] = SerMacAdde[1];
	request[i++] = SerMacAdde[2];
	request[i++] = SerMacAdde[3];
	request[i++] = SerMacAdde[4];
	request[i++] = SerMacAdde[5];

	//本地mac地址
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

	//Get Change IP & Mask & Gateway
	m_CfgIPAddr.GetAddress(m_chrGetIP[0],m_chrGetIP[1],m_chrGetIP[2],m_chrGetIP[3]);
	m_CfgSubMask.GetAddress(m_chrGetSubMask[0],m_chrGetSubMask[1],m_chrGetSubMask[2],m_chrGetSubMask[3]);
	m_CfgGateway.GetAddress(m_chrGateway[0],m_chrGateway[1],m_chrGateway[2],m_chrGateway[3]);


	//UCP_METHOD_SET_IP
	request[i++] = (char)((UCP_METHOD_SET_IP >> 8)&0xff);
	request[i++] = (char)((UCP_METHOD_SET_IP >> 0)&0xff);


	//Set DHCP
	if (preDhcpEnable != m_iDhcpEnable)
	{
		request[i++] = UCP_CODE_USE_DHCP;
		request[i++] = 1;
		request[i++] = m_iDhcpEnable;
		b_change = true;
	}

	if (!m_iDhcpEnable)
	{
		//Set IP
		if ((preGetIP[0] != m_chrGetIP[0])
			|| (preGetIP[1] != m_chrGetIP[1])
			|| (preGetIP[2] != m_chrGetIP[2])
			|| (preGetIP[3] != m_chrGetIP[3])
			|| (preDhcpEnable != m_iDhcpEnable))
		{
			/*request[i++] = UCP_CODE_IP_ADDR;
			request[i++] = 4;
			request[i++] = m_chrGetIP[0];
			request[i++] = m_chrGetIP[1];
			request[i++] = m_chrGetIP[2];
			request[i++] = m_chrGetIP[3];*/
			b_change = true;
		}

		//Set Mask
		if ((preGetSubMask[0] != m_chrGetSubMask[0])
			|| (preGetSubMask[1] != m_chrGetSubMask[1])
			|| (preGetSubMask[2] != m_chrGetSubMask[2])
			|| (preGetSubMask[3] != m_chrGetSubMask[3])
			||  preDhcpEnable != m_iDhcpEnable)
		{
			/*request[i++] = UCP_CODE_SUBNET_MASK;
			request[i++] = 4;
			request[i++] = m_chrGetSubMask[0];
			request[i++] = m_chrGetSubMask[1];
			request[i++] = m_chrGetSubMask[2];
			request[i++] = m_chrGetSubMask[3];*/
			b_change = true;
		}

		//Set Gateway
		if ((preGateway[0] != m_chrGateway[0])
			|| (preGateway[1] != m_chrGateway[1])
			|| (preGateway[2] != m_chrGateway[2])
			|| (preGateway[3] != m_chrGateway[3])
			|| (preDhcpEnable != m_iDhcpEnable))
		{
			/*request[i++] = UCP_CODE_GATEWAY_ADDR;
			request[i++] = 4;
			request[i++] = m_chrGateway[0];
			request[i++] = m_chrGateway[1];
			request[i++] = m_chrGateway[2];
			request[i++] = m_chrGateway[3];*/
			b_change = true;
		}
	}
	if(b_change){
		request[i++] = UCP_CODE_USE_DHCP;
		request[i++] = 1;
		request[i++] = m_iDhcpEnable;
		request[i++] = UCP_CODE_IP_ADDR;
		request[i++] = 4;
		request[i++] = m_chrGetIP[0];
		request[i++] = m_chrGetIP[1];
		request[i++] = m_chrGetIP[2];
		request[i++] = m_chrGetIP[3];
		request[i++] = UCP_CODE_SUBNET_MASK;
		request[i++] = 4;
		request[i++] = m_chrGetSubMask[0];
		request[i++] = m_chrGetSubMask[1];
		request[i++] = m_chrGetSubMask[2];
		request[i++] = m_chrGetSubMask[3];
		if(m_bridge== 0x01){
			request[i++] = UCP_CODE_GATEWAY_ADDR;
			request[i++] = 4;
			request[i++] = m_chrGateway[0];
			request[i++] = m_chrGateway[1];
			request[i++] = m_chrGateway[2];
			request[i++] = m_chrGateway[3];
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

	CBitmapDialog::OnOK();
}

void CIpConfig::GetLocalIPs(char *localips)
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

	LocalMacAddr[0] = pinfo->Address[0];
	LocalMacAddr[1] = pinfo->Address[1];
	LocalMacAddr[2] = pinfo->Address[2];
	LocalMacAddr[3] = pinfo->Address[3];
	LocalMacAddr[4] = pinfo->Address[4];
	LocalMacAddr[5] = pinfo->Address[5];

}

void CIpConfig::ReceiveSetIPData()
{
#if 0
	int			iErrorCount = 0;
	char		buffer[512];
	int			status;
	struct		sockaddr_in from;
	int			len = sizeof( struct sockaddr_in);
//	ULONG		uGetIP;
//	ULONG		uPort;
//	ULONG		uGetTemp;
//	ULONG		uUCPClass;
	int			DeviceCount = 0;
	//char		chrMACAddr[20];
//	char		chrIPaddr[20];
	ULONG		uSetip,uHi,uLo;

	while (true)
	{
		memset (buffer, 0, sizeof(buffer));
		
		status = recvfrom (m_Socket, buffer, 512, 0, (struct sockaddr*)&from,&len);

		uHi = (ULONG)buffer[status - 2];
		uLo = (ULONG)buffer[status - 1];

		uSetip = (uHi << 8) | uLo;

		if ( uSetip == UCP_METHOD_SET_IP)
			return;
		else if ( status == SOCKET_ERROR)
		{
			if ( iErrorCount > 100000)
				return;

			iErrorCount++;
		}
	}
#endif
}

void CIpConfig::OnDhcpenable() 
{
	// TODO: Add your control notification handler code here
/*	bool display;

	m_bDhcpEnable = ((CButton*)GetDlgItem(IDC_DHCPENABLE))->GetCheck();
	if (m_bDhcpEnable)
		display = FALSE;
	else
		display = TRUE;
	
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(display);
	GetDlgItem(IDC_SUB_MASK)->EnableWindow(display);
	GetDlgItem(IDC_GATEWAY)->EnableWindow(display);
*/
}

void CIpConfig::OnRadioDhcpEnable() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	GetDlgItem(IDC_IPADDRESS)->EnableWindow(true);
	GetDlgItem(IDC_SUB_MASK)->EnableWindow(true);
	if(m_bridge== 0x01){
		GetDlgItem(IDC_GATEWAY)->EnableWindow(true);
	}
}

void CIpConfig::OnRadioDhcp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
	GetDlgItem(IDC_SUB_MASK)->EnableWindow(false);
	GetDlgItem(IDC_GATEWAY)->EnableWindow(false);	
}
/*
HBRUSH CIpConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
		switch (nCtlColor) 
		{
		case CTLCOLOR_BTN:
		case CTLCOLOR_STATIC:
			pDC->SetBkMode (TRANSPARENT); 
			return (HBRUSH) m_brush.m_hObject;
			break;
		default:
			HBRUSH hbr = CBitmapDialog::OnCtlColor (pDC, pWnd, nCtlColor);
			return hbr ; 
			break; 
		}
}
*/
enum CIpConfig::CHECK_IP_RESULT CIpConfig::CheckValidIPAndMask()
{
	char	compareField;
	bool	zeroStart = false;

	for(int i = 0; i < 4; i ++)
	{
		compareField = m_chrGetSubMask[i] & 0x80;
		for(int j = 1; j < 8; j ++)
		{
			
			if ((zeroStart == false) && ((compareField & 0x80) == 0))
			{
				zeroStart = true;
			}
			compareField = (m_chrGetSubMask[i] << j) & 0x80;
			if ((zeroStart == true) && ((compareField & 0x80) != 0))
				return IP_MASK_ERR;
		}
	}

	if (m_chrGetIP[3] == 0)
		return IP_FIELD4_ZERO_ERR;

	if (m_chrGetIP[3] == 255)
		return IP_FIELD4_FILL_ERR;

	return 	IP_OK;
}

void CIpConfig::OnFieldchangedIpaddress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString errorMsg;
	char	errStr[72];

	UpdateData(true);

	m_CfgIPAddr.GetAddress(m_chrGetIP[0],m_chrGetIP[1],m_chrGetIP[2],m_chrGetIP[3]);

	if ((m_oldGetIP[0] == m_chrGetIP[0]) &&
		(m_oldGetIP[1] == m_chrGetIP[1]) &&
		(m_oldGetIP[2] == m_chrGetIP[2]) &&
		(m_oldGetIP[3] == m_chrGetIP[3]))
		return;

	if ((m_chrGetIP[0] < 1) || (m_chrGetIP[0] > 223))
	{
		errorMsg.LoadString(IDS_ERR_IPFIELD1);
		sprintf(errStr, errorMsg, m_chrGetIP[0]);
		if (flag == false)
		{
			flag = true;
			AfxMessageBox(errStr);
		}
	}
	flag = false;

	UpdateData(false);

	for (int i = 0; i < 4; i++)
		m_oldGetIP[i] = m_chrGetIP[i];

	*pResult = 0;
}

void CIpConfig::OnFieldchangedGateway(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
/*
	CString errorMsg;
	char	errStr[72];

	UpdateData(true);

	m_CfgGateway.GetAddress(m_chrGateway[0],m_chrGateway[1],m_chrGateway[2],m_chrGateway[3]);

	if ((m_oldGateway[0] == m_chrGateway[0]) &&
		(m_oldGateway[1] == m_chrGateway[1]) &&
		(m_oldGateway[2] == m_chrGateway[2]) &&
		(m_oldGateway[3] == m_chrGateway[3]))
		return;

	if ((m_chrGateway[0] < 1) || (m_chrGateway[0] > 223))
	{
		errorMsg.LoadString(IDS_ERR_IPFIELD1);
		sprintf(errStr, errorMsg, m_chrGateway[0]);
		if (flag == false)
		{
			flag = true;
			AfxMessageBox(errStr);
		}
	}
	flag = false;

	UpdateData(false);

	for (int i = 0; i < 4; i++)
		m_oldGateway[i] = m_chrGateway[i];
*/

	*pResult = 0;
}

void CIpConfig::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBitmapDialog::OnCancel();
}

void CIpConfig::OnPaint() 
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

void CIpConfig::OnBnClickedOk()
{
	OnOK();
}

void CIpConfig::OnIpnFieldchangedSubMask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

// DeviceInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "DeviceInfoDlg.h"
#include "upgraderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceInfoDlg dialog


CDeviceInfoDlg::CDeviceInfoDlg(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CDeviceInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceInfoDlg)
	m_strFwVer = _T("");
	m_strIpAddr = _T("");
	m_strMacAddr = _T("");
	m_strModelName = _T("");
	m_strChannel = _T("");
	m_strDhcp = _T("");
	m_strGateway = _T("");
	m_strSubnetMask = _T("");
	m_strOpMode = _T("");
	m_strSecurity = _T("");
	m_strSsid = _T("");
	m_strWlanMode = _T("");
	m_strWlanMacAddr = _T("");
	m_strBssid = _T("--");
    m_strRssi = _T("--");
	m_strAssociated = _T("--");
	m_str5GWlanMode = _T("");
	m_str5GSsid = _T("");
	m_str5GRssi = _T("--");
	m_str5GBssid = _T("--");
	m_str5GAssociated = _T("--");
	m_str5GChannel = _T("");
	m_str5GSecurity = _T("");
	m_str5GWlanMacAddr = _T("");
	m_strWlanOpMode = _T("");
	m_str5GWlanOpMode = _T("");
	//}}AFX_DATA_INIT
	m_pParent = pParent;
//	m_brush.CreateStockObject (HOLLOW_BRUSH);
}


void CDeviceInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceInfoDlg)
	DDX_Text(pDX, IDC_FWVER_STATIC, m_strFwVer);
	DDX_Text(pDX, IDC_IPADDR_STATIC, m_strIpAddr);
	DDX_Text(pDX, IDC_MACADDR_STATIC, m_strMacAddr);
	DDX_Text(pDX, IDC_MODEL_STATIC, m_strModelName);
	DDX_Text(pDX, IDC_CHANNEL_STATIC, m_strChannel);
	DDX_Text(pDX, IDC_DHCP_STATIC, m_strDhcp);
	DDX_Text(pDX, IDC_GATEWAY_STATIC, m_strGateway);
	DDX_Text(pDX, IDC_MASK_STATIC, m_strSubnetMask);
	DDX_Text(pDX, IDC_OPMODE_STATIC, m_strOpMode);
	DDX_Text(pDX, IDC_SECUTRITY_STATIC, m_strSecurity);
	DDX_Text(pDX, IDC_SSID_STATIC, m_strSsid);
	DDX_Text(pDX, IDC_WLANMODE_STATIC, m_strWlanMode);
	DDX_Text(pDX, IDC_WLAN_MACADDR_STATIC, m_strWlanMacAddr);
	DDX_Text(pDX, IDC_BSSID_STATIC, m_strBssid);
	DDX_Text(pDX, IDC_RSSI_STATIC, m_strRssi);
	DDX_Text(pDX, IDC_CONNECTSTATUS_STATIC, m_strAssociated);
	DDX_Text(pDX, IDC_5GWLANMODE_STATIC, m_str5GWlanMode);
	DDX_Text(pDX, IDC_5GSSID_STATIC, m_str5GSsid);
	DDX_Text(pDX, IDC_5GRSSI_STATIC, m_str5GRssi);
	DDX_Text(pDX, IDC_5GBSSID_STATIC, m_str5GBssid);
	DDX_Text(pDX, IDC_5GCONNECTSTATUS_STATIC, m_str5GAssociated);
	DDX_Text(pDX, IDC_5GCHANNEL_STATIC, m_str5GChannel);
	DDX_Text(pDX, IDC_5GSECUTRITY_STATIC, m_str5GSecurity);
	DDX_Text(pDX, IDC_5GWLAN_MACADDR_STATIC, m_str5GWlanMacAddr);
	DDX_Text(pDX, IDC_WLANOPMODE_STATIC, m_strWlanOpMode);
	DDX_Text(pDX, IDC_5GWLANOPMODE_STATIC, m_str5GWlanOpMode);
   
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceInfoDlg, CBitmapDialog)
	//{{AFX_MSG_MAP(CDeviceInfoDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_CHANNEL_STATIC, &CDeviceInfoDlg::OnStnClickedChannelStatic)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceInfoDlg message handlers

BOOL CDeviceInfoDlg::OnInitDialog() 
{
	CBitmapDialog::OnInitDialog();
	CUpgraderView* m_pUpgraderView = (CUpgraderView*)m_pParent;

	UpdateData(FALSE);

	SetBitmap(IDB_BG_INFO);

	m_nType = CBitmapDialog::BITMAP_STRETCH;
	// force the background to repaint
	Invalidate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
HBRUSH CDeviceInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBitmapDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
}
*/

void CDeviceInfoDlg::OnPaint() 
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

void CDeviceInfoDlg::OnStnClickedChannelStatic()
{
	// TODO: Add your control notification handler code here
}

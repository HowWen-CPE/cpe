// upgraderView.cpp : implementation of the CUpgraderView class
//

#include "stdafx.h"
#include "upgrader.h"
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

#include <winsock.h>
//#include "ListVwEx.h"	// base class for CRowListView
//#include "MyListCtrl.h"
/*Scoutlee begin 2004/9/2 04:58と*/
#define APCFG_AUTH_OPEN_SYSTEM          0
#define APCFG_AUTH_SHARED_KEY           1
#define APCFG_AUTH_WPA                  2
#define APCFG_AUTH_WPAPSK               3
#define APCFG_AUTH_AUTO                 4

/*Scoutlee end 2004/9/2 04:58と*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define LIST_MAX_ITEMLEN	    255
#define MAX_HOSTNAME_LAN		255

oid BootStatus[] = {1,3,6,1,4,1,3093,3,1,4,0};
oid sysDescr[] = {1,3,6,1,2,1,1,1,0};
oid FWVersion[] = {1,3,6,1,4,1,3093,1,3,0};
oid ifPhysAddr[] = {1,3,6,1,2,1,2,2,1,6,1};
oid Password[] = {1,3,6,1,4,1,3093,3,1,3,0};

static BOOL ext_bRun = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CUpgraderView

IMPLEMENT_DYNCREATE(CUpgraderView, CListViewEx)

BEGIN_MESSAGE_MAP(CUpgraderView, CListViewEx)
	//{{AFX_MSG_MAP(CUpgraderView)
	ON_WM_CREATE()
	ON_COMMAND(ID_SEARCH, OnSearch)
	ON_UPDATE_COMMAND_UI(ID_SEARCH, OnUpdateSearch)
	ON_COMMAND(ID_UPGRADE_MANUAL, OnUpgradeManual)
	ON_UPDATE_COMMAND_UI(ID_UPGRADE_MANUAL, OnUpdateUpgradeManual)
	ON_COMMAND(ID_DEVICE_INFO, OnDeviceInfo)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_INFO, OnUpdateDeviceInfo)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_CONFIG, OnUpdateConfig)
	ON_COMMAND(ID_CONFIG, OnConfig)
	ON_COMMAND(ID_WEB, OnWeb)
	ON_UPDATE_COMMAND_UI(ID_WEB, OnUpdateWeb)
	ON_WM_TIMER()
	ON_COMMAND(ID_WIRELESS, OnWireless)
	ON_UPDATE_COMMAND_UI(ID_WIRELESS, OnUpdateWireless)
	ON_COMMAND(ID_WIZARD, OnWizard)
	ON_UPDATE_COMMAND_UI(ID_WIZARD, OnUpdateWizard)
	ON_COMMAND(ID_TOOLS_SELECTCONNECTION, OnToolsSelectconnection)
	//}}AFX_MSG_MAP	
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SELECTCONNECTION, &CUpgraderView::OnUpdateToolsSelectconnection)
	ON_MESSAGE(WM_CHANGE_SOCKET, &CUpgraderView::OnChangeSocket)
	ON_COMMAND(ID_REBOOT, &CUpgraderView::OnReboot)
	ON_UPDATE_COMMAND_UI(ID_REBOOT, &CUpgraderView::OnUpdateReboot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgraderView construction/destruction

CUpgraderView::CUpgraderView()
{
	// TODO: add construction code here
	dialFlag[0] = 1;
}

CUpgraderView::~CUpgraderView()
{
}

BOOL CUpgraderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_SHOWSELALWAYS;
	cs.style |= LVS_REPORT;
//	cs.style |= LVS_OWNERDRAWFIXED;
//	cs.style |= LVS_EDITLABELS;
	return CListViewEx::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CUpgraderView drawing

void CUpgraderView::OnDraw(CDC* pDC)
{
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

void CUpgraderView::OnInitialUpdate()
{
	CListViewEx::OnInitialUpdate();
	CRect rect;
	GetClientRect(&rect);
	m_sendDiscoverCount = 0;
	
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();

	ctlList.InsertColumn(0,"Device Name", LVCFMT_LEFT, (rect.right*9)/40 , -1 );
	ctlList.InsertColumn(1,"Device Type", LVCFMT_LEFT, (rect.right*8)/40 , -1 );
	ctlList.InsertColumn(2,"LAN IP Address", LVCFMT_LEFT, (rect.right*7)/40 , -1 );
	ctlList.InsertColumn(3,"Ethernet MAC Address", LVCFMT_LEFT, (rect.right*8)/40 , -1 );	
	ctlList.InsertColumn(4,"Firmware Version", LVCFMT_LEFT, (rect.right*8)/40 , -1 );	

	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	pWaiting = NULL;

	// 取得网卡个数
	int nAdapterCount = 0;
	PIP_ADAPTER_INFO pAdapter = pDoc->m_pAdapterInfo;
	while (NULL != pAdapter)
	{
		nAdapterCount++;
		pAdapter = pAdapter->Next;
	}
	
	// 根据网卡的数量让用户选择一个默认网卡
	switch (nAdapterCount)
	{
	case 1:
		pDoc->m_pDefaultAdapter = pDoc->m_pAdapterInfo;
		break;
	case 0:
		MessageBox("There is not any adapter!", "Error", MB_OK|MB_ICONERROR);
		break;
	default:
		CSelectConectionDlg dlg;
		if (dlg.DoModal() != IDOK)
		{
			// 默认选择第一个
			pDoc->m_pDefaultAdapter = pDoc->m_pAdapterInfo;			
			//MessageBox("Please select a default adapter", "warning", MB_OK|MB_ICONWARNING);
		}
		break;
	}
	ASSERT(NULL != pDoc->m_pDefaultAdapter);
	
	InterfaceSelect();
	CreateWinSocket();
	StartDiscovery();
	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CUpgraderView diagnostics

#ifdef _DEBUG
void CUpgraderView::AssertValid() const
{
	CListViewEx::AssertValid();
}

void CUpgraderView::Dump(CDumpContext& dc) const
{
	CListViewEx::Dump(dc);
}

CUpgraderDoc* CUpgraderView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUpgraderDoc)));
	return (CUpgraderDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUpgraderView message handlers

int CUpgraderView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= LVS_ICON |LVS_NOLABELWRAP|LVS_AUTOARRANGE|LVS_SINGLESEL|LVS_REPORT;

	if (CListViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;
//	CMyListCtrl& ListCtrl = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ListCtrl = (CListCtrl&) GetListCtrl();

	m_ImageList.Create(47,47,
			       ILC_COLORDDB|ILC_MASK, // list does include masks
			       1,
			       0);// list won't grow
	CBitmap Bitmap;
	Bitmap.LoadBitmap (IDB_ROUTER); 
	m_nRouter = m_ImageList.Add(&Bitmap,(COLORREF)RGB(0,255,0)); // Add to Image List
	Bitmap.DeleteObject(); // Unload the bitmap from CBitmap

	Bitmap.LoadBitmap (IDB_SWITCH); 
	m_nSwitch = m_ImageList.Add(&Bitmap,(COLORREF)RGB(0,255,0)); // Add to Image List
	Bitmap.DeleteObject(); // Unload the bitmap from CBitmap

	Bitmap.LoadBitmap (IDB_GATEWAY); // Load multiple Page Bitmap
	m_nGateway = m_ImageList.Add(&Bitmap,(COLORREF)RGB(0,255,0)); // Add to Image List
	Bitmap.DeleteObject(); // Unload the bitmap from CBitmap
		
	ListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);

	return 0;
}

void CUpgraderView::OnSearch() 
{
	HCURSOR hOldCur = BeginWait();
	
	if(pWaiting == NULL){
		pWaiting = new CWaiting;
		// 查找2000 ms
		SetTimer(TIMER4,2000,NULL);
	}
	
	StartDiscovery();
}

void CUpgraderView::OnUpdateSearch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);	
}

void CUpgraderView::LV_AddItem(int nIndex, CDeviceInfo *pItem)
{
	LV_ITEM			lvI;  //list view item structure

	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
    lvI.state = INDEXTOOVERLAYMASK(1)|ILD_IMAGE;
    lvI.stateMask = LVIS_OVERLAYMASK;
	lvI.iItem = nIndex;	
	lvI.iSubItem = 0;
	lvI.lParam = (LPARAM)pItem;
	lvI.iImage = m_nGateway;
	struct in_addr  in;
	in.s_addr = pItem->IpAddr.sin_addr.s_addr;
	CString strIpAddr = inet_ntoa(in);
	lvI.pszText = (LPTSTR)(LPCTSTR)strIpAddr;
	lvI.cchTextMax = LIST_MAX_ITEMLEN;

//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	ctlList.InsertItem(&lvI);
}

// Factory default Davidgong 2010.5.27
void CUpgraderView::OnUpgradeManual() 
{	
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		if(dialFlag[0]==1){
		dialFlag[0]=0;
		CLogin	dlg;				
		if (dlg.DoModal() == IDOK)
		{
			m_strUserName = dlg.m_sUserName;
			m_strPassword = dlg.m_sPassword;
			SendAuthentication(UCP_METHOD_RESET);
			
		}
		dialFlag[0]=1;
	}
	}

//	SetFactoryDefault(LocalIP);
}

void CUpgraderView::OnUpdateUpgradeManual(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(TRUE);
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUpgraderView::OnUpdateWireless(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(TRUE);
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUpgraderView::OnUpdateWizard(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(TRUE);
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	//if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
	//	pCmdUI->Enable(TRUE);
	//else
		pCmdUI->Enable(FALSE);
}

void CUpgraderView::OnDeviceInfo() 
{
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetNextItem(-1, LVNI_SELECTED);
	//CDeviceInfo *pItem = (CDeviceInfo *)ctlList.GetItemData(index);
//	CDeviceInfoDlg  InfoDlg;
//	InfoDlg.m_strModelName = DeviceNameList[index];
//	InfoDlg.m_strIpAddr =  IPAddressList[index];
//	InfoDlg.m_strMacAddr = MacAddressList[index];
//	InfoDlg.m_strFwVer = FirmwareVerList[index];
//	InfoDlg.DoModal();
	BindToServer(0x11111111);
	GetMiniApInfo();
}

void CUpgraderView::OnUpdateDeviceInfo(CCmdUI* pCmdUI) 
{
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);	
#ifdef UNICAST
	//happiness add
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(index);
		/*int o = m_saUdpServ.sin_addr.s_addr&inet_addr("255.255.255.0");
		int p = inet_addr("192.168.1.10")&inet_addr("255.255.255.0");
		int q = inet_addr("255.255.255.255")&inet_addr("255.255.255.0");*/
		if((pDevice->m_strIPAddr == "0.0.0.0")){
#endif
			m_saUdpServ.sin_addr.s_addr = htonl(0xffffffff);
#ifdef UNICAST
		}else{
			m_saUdpServ.sin_addr.s_addr = inet_addr(pDevice->m_strIPAddr);
			if(((inet_addr(m_szLocalHostIP)&inet_addr("255.255.255.0")) !=
				((m_saUdpServ.sin_addr.s_addr)&inet_addr("255.255.255.0")))){
				m_saUdpServ.sin_addr.s_addr = htonl(0xffffffff);
			}
			
			//m_saUdpServ.sin_addr.s_addr = htonl(0xffffffff);
		}
		//m_saUdpServ.sin_addr.s_addr = htonl(0xffffffff);
			
	}
#endif

}

void CUpgraderView::OnRButtonDown(UINT nFlags, CPoint point) 
{
/*
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.HitTest(point);
	if (index != -1)
	{
		//int pos = ctlList.GetSelectionMark();
		//CDeviceInfo *pItem = (CDeviceInfo *)ctlList.GetItemData(index);
		CDeviceInfoDlg  InfoDlg;
		InfoDlg.m_strModelName = m_szDeviceNameList[index];
		InfoDlg.m_strIpAddr =  m_szIPAddressList[index];
		InfoDlg.m_strMacAddr = m_szMacAddressList[index];
		InfoDlg.m_strFwVer = m_szFirmwareVerList[index];
		InfoDlg.DoModal();
		//GetMiniApInfo();
	}
*/
	CListViewEx::OnRButtonDown(nFlags, point);
}

void CUpgraderView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.HitTest(point);
	if (index != -1)
		PostMessage(WM_COMMAND, ID_UPGRADE_AUTO, 0);

	GetMiniApInfo();

	CListViewEx::OnLButtonDblClk(nFlags, point);
}

void CUpgraderView::OnSize(UINT nType, int cx, int cy) 
{
	CListViewEx::OnSize(nType, cx, cy);
	if ( GetListCtrl().GetSafeHwnd() )
	{
//		GetListCtrl().SetColumnWidth(0,cx/5);
//		GetListCtrl().SetColumnWidth(1,(cx/5)*2);
//		GetListCtrl().SetColumnWidth(2,cx/5);
//		GetListCtrl().SetColumnWidth(3,cx/5);
	}	
}

// 组织探测包的数据 David 2010.9.19
void CUpgraderView::GetDiscoveryData()
{
	// 探测包为0x0100，目标MAC为全零
	int i;
	for (i = 0; i < 8; i++ )// Broadcast packet.
	{
		if ( i == 0 )
			m_szSendDataBuf[i] = 0x01;
		else
			m_szSendDataBuf[i] = 0x00;
	}

	CUpgraderDoc* pDoc = GetDocument();
	//BYTE macAddress[MAX_ADAPTER_ADDRESS_LENGTH];
	int k = 0;
	//for(k;k< MAX_ADAPTER_ADDRESS_LENGTH; k++){
	//	macAddress[k] = pDoc->m_pDefaultAdapter->Address[k];

	//}
	for(k = 0; k < 6;k ++){
		m_szSendDataBuf[i++] = pDoc->m_pDefaultAdapter->Address[k];
	}
	//setLocalMacAddress(i);
	/*m_szSendDataBuf[i++] = 0x00;
	m_szSendDataBuf[i++] = 0x23;
	m_szSendDataBuf[i++] = 0xcd;
	m_szSendDataBuf[i++] = 0x9c;
	m_szSendDataBuf[i++] = 0xe7;
	m_szSendDataBuf[i++] = 0x0a;*/
	

	//客户端选项0x0002
	m_szSendDataBuf[i++] = (UAP_CLIENT_OPTION >> 8 ) & 0xff;
	m_szSendDataBuf[i++] = (UAP_CLIENT_OPTION >> 0 ) & 0xff;

	// 源IP
	CString strHostIP = m_szLocalHostIP;
	for (i; i < 14; i++)
	{
		CString StrTemp;

		int pos = strHostIP.Find('.',0);
		if ( pos != -1 )
			StrTemp = strHostIP.Left(pos);
		else
			StrTemp = strHostIP;

		int tt = atoi(StrTemp);
		m_szSendDataBuf[i] = (char)tt;
		strHostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szSendDataBuf[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szSendDataBuf[i++] = (UDP_UDAP_PORT >> 0)&0xff;
	//happiness add 2011.05.21
   
	/*CUpgraderDoc* pDoc = GetDocument();
	BYTE macAddress[MAX_ADAPTER_ADDRESS_LENGTH];
	int k = 0;
	for(k;k< MAX_ADAPTER_ADDRESS_LENGTH; k++){
		macAddress[k] = pDoc->m_pDefaultAdapter->Address[k];

	}
	for(k = 0; k < 6;k ++){
	m_szSendDataBuf[i++] = macAddress[k];
	}*/


	//SequenceNum 序列号
	m_szSendDataBuf[i++] = 0x01;
	m_szSendDataBuf[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE //设备类型
	m_szSendDataBuf[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szSendDataBuf[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szSendDataBuf[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	m_szSendDataBuf[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szSendDataBuf[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szSendDataBuf[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szSendDataBuf[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	//UCP_METHOD_DISCOVER
	m_szSendDataBuf[i++] = (UCP_METHOD_DISCOVER >> 8)&0xff;
	m_szSendDataBuf[i++] = (UCP_METHOD_DISCOVER >> 0)&0xff;

	//Send Data
	m_packetLength = i;
}
void CUpgraderView::StartDiscovery()
{
	// 清空内存
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	pDoc->ClearDevice();
	memset(m_szSendDataBuf,0,sizeof(m_szSendDataBuf));

	// 清空界面
	GetListCtrl().DeleteAllItems();

	// 组织探测包的数据 David 2010.9.19
	GetDiscoveryData();
	//BindToServer(0xc0a8020a);
	m_saUdpServ.sin_addr.s_addr = htonl(0xffffffff);
	BindToServer(0xc0a8020a);
	int nResult = sendto( m_Socket, m_szSendDataBuf, m_packetLength, 0, (SOCKADDR *) &m_saUdpServ,sizeof ( SOCKADDR_IN ));
	m_sendDiscoverCount++;
	if( nResult == SOCKET_ERROR ) 
	{
		//KillTimer(TIMER1);
		m_sendDiscoverCount = 0;
		char errBuf[40];
		sprintf( errBuf, "sendto() fail! Error Code: %05d", nResult );
		//AfxMessageBox( errBuf );
		return;
	}
	
	//Re-send
	SetTimer(TIMER1,500,NULL);
}	

// 接收设备端数据包 David 2010.6.3
void CUpgraderView::GetDevicesInfo()
{
	if (m_Socket == INVALID_SOCKET)
		return;

	// 接收设备端数据包
	const int	MSG_CHUNK = 1024;
	char		buffer[MSG_CHUNK] = "\0";
	sockaddr_in soctFrom;	// 设备端的IP地址
	memset(&soctFrom, NULL, sizeof(sockaddr));
	int	nFromLen = sizeof(sockaddr_in);	// 数据包长度	
	int status = recvfrom(m_Socket, buffer, MSG_CHUNK, 0, (sockaddr*)&soctFrom, &nFromLen);
	CString strServIP = inet_ntoa(soctFrom.sin_addr);
	char szIP[20];
	memset(szIP,0,sizeof(szIP));
	//m_npBufferIndex = 0;
	char szBufferBin[512] = "\0";
	
	// 接收失败返回
	if (status <= 0)
		return;	
	
	// 接收成功，将转换到本地内存
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	int nRes = pDoc->GetDataFromSocket(buffer, strServIP);
	switch (nRes)
	{
	case 0:		// 缓冲区指针为空
	case -1:	// 不是来自设备端的数据包
	case -2:	// 与客户端的IP不同
		return;
	case -3:	// 转换分段数据错误
		KillTimer(TIMER_OVERTIME);
		MessageBox("Data To Doc Error");

		return;
	case -4:	// 用户名和密码错误
		KillTimer(TIMER_OVERTIME);
		AfxMessageBox( "User Name or Password Fail!" );
		return;
	}
	
	// socket数据转换到本地内存后，继续后续的任务
	KillTimer(TIMER_OVERTIME);
	LaterAction();
}

int CUpgraderView::HexStrToBin(char *pInStr, char *pOutBinData, int ConvLen)
{
	bool	exitFlag = 0;					// Exit parsing
	bool	stateTrigger = 0;				// Find data to convert
	bool	lastHex = 0;					// Last input character is Hex?
	int		bytesCount = 0;					// How many Hex character have been find
	int		HexCoupleCount = 0;				// How many hex already convert
	char	TempStr[3];						// TempStr for convert Hex to byte
	char	*pch1;							// pointer
	char	*pch2;							// pointer
	int		i;

	// Set all output buffer to 0.
	pch1 = pOutBinData;
	for(i=0; i<ConvLen; i++) {
		*pch1++ = 0;						// Clear pOutBinData
	}

	pch1 = pInStr;
	pch2 = pOutBinData;

	// Generate the output binary string.
	while(1)								// Parsing input string
	{
		switch(*pch1) {
		  case NULL:						// Termainal string
			exitFlag = 1;
			if(bytesCount > 0)
				stateTrigger = 1;			// *** Need parsing the number.
			break;

		  case 0x20:						// SPACE
		  case 0x09:						// TAB
 			if(lastHex)
				stateTrigger = 1;			// *** Need parsing the number.
			break;

		  default:
			if(!IsHexDigit(*pch1)) {		// Not hexdecimal character
				exitFlag = 1;
				break;
			}
			if(HexCoupleCount >= ConvLen){	// Too much input.
				exitFlag = 1;
				break;
			}
			lastHex = 1;
			bytesCount++;
			if(bytesCount == 2)
				stateTrigger = 1;			// *** Need parsing the number.
			break;
		} // End of switch(*pch1)

		pch1++;
		// Generate binary output
		if(stateTrigger) {
			pch1 = pch1 - 2;
			strncpy(TempStr,pch1,bytesCount);
			TempStr[bytesCount] = NULL;		// Set the NULL Terminated.

			*pch2++ = (char)HexToUINT(TempStr);
			HexCoupleCount++;
			pch1 = pch1 + 2;
			stateTrigger = 0;
			bytesCount = 0;
			lastHex = 0;
		} // End of if(stateTrigger)

		if(exitFlag)
			break;

	} // End of while(*pch != NULL)

	return 1;								// Convert OK!!
}

UINT CUpgraderView::HexToUINT(char *pstr)
{
	UINT uResult = 0;
	
	while(*pstr)
	{
		if(!IsHexDigit(*pstr)) {
			return 0xFFFFFFFF;
		}

		uResult = uResult * 16;
		if(*pstr > '9')
			uResult = uResult + (char)(*pstr & 0xF) + 9;
		else
			uResult = uResult + (char)(*pstr & 0xF);
		pstr++;
	}
	return uResult;
}

// 取得主机名称 david 2010.6.1
BOOL CUpgraderView::GetLocalName(char *localname, int len)
{
	// 1.初始化结果
	memset(localname, 0, len);

	// 2.取得主机名
	char locname[512] = "\0";
	int status = gethostname (locname, 512);
	if (status != 0)
		return FALSE;

	// 3.为结果赋值
	int uselen = strlen (locname) + 1;
	if (uselen > len) 
		uselen = len;
	memcpy (localname, locname, uselen);

	return TRUE;
}

BOOL CUpgraderView::ResolveIPs(char *hostname, char *hostips, int len)
{
	hostent *phe = gethostbyname (hostname);
	ASSERT(NULL != phe);
	if (phe == NULL || phe->h_addr_list[0] == NULL)
		return FALSE;

	char allips[256] = "\0";
	char null_ip[256] = "\0";
	int i=0;
	for (i=0; i<256; i++)
	{
		allips[i] = 0;
		null_ip[i] = 0;
	}

	int count = 0;
	while (phe->h_addr_list[count] != NULL && count * 15 < 240)
	{
		char oneip[15] = "";
		for (i = 0; i < 15; i++)
			oneip[i] = 0;

		sprintf(oneip, "%u.%u.%u.%u", 
			(unsigned char) phe->h_addr_list[count][0],
			(unsigned char) phe->h_addr_list[count][1],
			(unsigned char) phe->h_addr_list[count][2],
			(unsigned char) phe->h_addr_list[count][3]);

		for (i = (count * 15); i < (count * 15 + 15); i++)
			null_ip[i] = oneip[i % 15];

		count++;
	}

	int lEnd = 0;
	for (i=0; i<15; i++)
	{
		if (null_ip[i*15] == 0)
			break;
		for (int j=0; j<15; j++)
			allips[lEnd+j] = null_ip[i*15 + j];

		lEnd = strlen(allips);
		allips[lEnd] = 13;
		lEnd ++;
	}

	for (i=0; i<len; i++)
		hostips[i] = 0;

	int uselen = strlen(allips);
	if (uselen > len)
		uselen = len;

	for (i = 0; i < uselen; i++)
		hostips[i] = allips[i];
	hostips[uselen] = 0;

	return TRUE;
}

void CUpgraderView::OnUpdateConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

// ipsetting Davidgong 2010.5.27
void CUpgraderView::OnConfig() 
{	
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		if(dialFlag[0]==1){
			dialFlag[0]=0;
		CLogin	dlg;
		if (dlg.DoModal() == IDOK)
		{
			
			m_strUserName = dlg.m_sUserName;
			m_strPassword = dlg.m_sPassword;
			SendAuthentication(UCP_METHOD_GET_IP);
		}
		dialFlag[0]=1;
	}
	}
	
}

// 发包取得IP
void CUpgraderView::SendConfig()
{
	char	chrSendBuffer[128];
	memset(chrSendBuffer, 0, sizeof(chrSendBuffer));
	int		i = 0;

	//UCP_METHOD_DISCOVER
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();//.GetSelectedCount();//.HitTest(point);
	if (index != -1)
	{
		LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(index);
		for ( int j = 0; j < 6; j++ )	
			//m_szRequest[i++] = (char)iMACAddr[index][j];
			m_szRequest[i++] = (char)pDevice->m_arrMac[j];
	}

	setLocalMacAddress(i);
	// 客户端选项0x0002
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 8 ) & 0xff;
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 0 ) & 0xff;

	// 源IP
	CString HostIP = m_szLocalHostIP;
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
		m_szRequest[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	m_szRequest[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	// Method 这里为获得IP
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 0 ) & 0xff;

	int nResult = sendto(m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof (SOCKADDR_IN));
	if( nResult == SOCKET_ERROR ) 
	{
		//sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		sprintf( chrSendBuffer, "UDP packet sendto error, please check network connection, and click again");
		AfxMessageBox( chrSendBuffer );
		return;
	}
	
}

// link to iexplore Davidgong 2010.5.28
void CUpgraderView::OnWeb() 
{
	
	// 1.定义命令行参数和IP地址
	TCHAR szkey[MAX_PATH*2];
	CString strIpaddress;
	int nHTTPSEnable;
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);

	// 2.根据注册表查找浏览器的路径，合成命令行
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();	
	if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
	{
		int pos = ctlList.GetSelectionMark();
		LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(pos);
		nHTTPSEnable = pDevice->m_nHTTPSEnable;
		strIpaddress = pDevice->m_strIPAddr;
		if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), szkey) == ERROR_SUCCESS)
		{
			lstrcat(szkey, _T("\\shell\\open\\command"));
			if (ERROR_SUCCESS == GetRegKey(HKEY_CLASSES_ROOT,szkey, szkey)) 
			{
				TCHAR *pos;
				pos = _tcsstr(szkey, _T("\"%1\""));
				// No quotes found
				if (pos == NULL)
				{                     
					pos = _tcsstr(szkey, _T("%1"));      // Check for %1, without quotes 
					if (pos == NULL)                   // No parameter at all...
						pos = szkey+lstrlen(szkey)-1;
					else
						*pos = '\0';                   // Remove the parameter
				}
				else
					*pos = '\0';                       // Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, strIpaddress);
			}
		}
	}

	// 3.启动浏览器
	CString strIP;
	//Added By Andy Yu in 20140227: Http or Https
	if (nHTTPSEnable == 1)
	{
		strIP.Format("https:\\\\%s", strIpaddress);
	}
	else
	{
		strIP.Format("http:\\\\%s", strIpaddress);
	}
	ShellExecute(NULL, NULL, strIP, NULL, NULL, SW_SHOW);
}

void CUpgraderView::OnUpdateWeb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

//发送获取host包以获取主机名，域名，ip地址
void CUpgraderView::SendHost()
{
	char	chrSendBuffer[128];
	memset(chrSendBuffer, 0, sizeof(chrSendBuffer));
	int		i = 0;

	//UCP_METHOD_DISCOVER
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();//.GetSelectedCount();//.HitTest(point);
	if (index != -1)
	{
		LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(index);
		for ( int j = 0; j < 6; j++ )	
			//m_szRequest[i++] = (char)iMACAddr[index][j];
			m_szRequest[i++] = (char)pDevice->m_arrMac[j];
	}
	setLocalMacAddress(i);

	// 客户端选项0x0002
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 8 ) & 0xff;
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 0 ) & 0xff;

	// 源IP
	CString HostIP = m_szLocalHostIP;
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
		m_szRequest[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	m_szRequest[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	// Method 这里为获得IP
	m_szRequest[i++] = (UCP_METHOD_GET_HOST >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_GET_HOST >> 0 ) & 0xff;

	int nResult = sendto(m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof (SOCKADDR_IN));
	if( nResult == SOCKET_ERROR ) 
	{
		//sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		sprintf( chrSendBuffer, "UDP packet sendto error, please check network connection, and click again");
		AfxMessageBox( chrSendBuffer );
		return;
	}
	

}

LONG CUpgraderView::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS)
	{
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}

// 发送恢复出厂设置包
void CUpgraderView::SetFactoryDefault()
{
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int MacPos = ctlList.GetSelectionMark();
	if ( MacPos == -1 )
	{
		MessageBox("You have to select one device to set default","Select warning",MB_ICONWARNING | MB_OK);
		return;
	}
	memset(m_szRequest, 0, sizeof(m_szRequest));
	int	i = 0;
	//Reset packet 设置包0x0001
	m_szRequest[i++] = 0x00;
	m_szRequest[i++] = 0x01;

	//Server MAC
	int		j = 0;
	CString strMac;
	strMac.Format("%02X:%02X:%02X:%02X:%02X:%02X",
		pDoc->m_arrDevice[MacPos]->m_arrMac[0],
		pDoc->m_arrDevice[MacPos]->m_arrMac[1],
		pDoc->m_arrDevice[MacPos]->m_arrMac[2],
		pDoc->m_arrDevice[MacPos]->m_arrMac[3],
		pDoc->m_arrDevice[MacPos]->m_arrMac[4],
		pDoc->m_arrDevice[MacPos]->m_arrMac[5]
		);
	char szMac[18] = {'\0'};
	strcpy(szMac, LPCTSTR(strMac));
	for (i, j; i < 8; i++,j++)
	{
		char chrTemp,chrTemp1;
		ULONG Mac,MacHi,MacLo;
		chrTemp  = szMac[j];
		chrTemp1 = szMac[j + 1];
		MacHi = HexDigitConvert(chrTemp);
		MacLo = HexDigitConvert(chrTemp1);
		Mac = ( MacHi << 4 ) | MacLo;
		m_szRequest[i] = (char)Mac;
		j+=2;
	}

	setLocalMacAddress(i);

	// 客户端选项 0x0002
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 8 ) & 0xff;
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 0 ) & 0xff;

	// 控制端IP
	CString HostIP = m_szLocalHostIP;
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
		m_szRequest[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	// 设备类型 0xC001
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP 设备类别
	m_szRequest[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	//UCP_METHOD_RESET
	m_szRequest[i++] = (UCP_METHOD_RESET >> 8)&0xff;
	m_szRequest[i++] = (UCP_METHOD_RESET >> 0)&0xff;

	//Send Data
	int nResult = sendto(	m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof ( SOCKADDR_IN ));
	if( nResult == SOCKET_ERROR ) 
	{
		char	szErrMsgBuffer[64];
		//sprintf( szErrMsgBuffer, "sendto() fail! Error Code: %05d", nResult );
		sprintf( szErrMsgBuffer, "UDP packet sendto error, please check network connection, and click again");
		AfxMessageBox( szErrMsgBuffer );
		return;
	}
	
}

int CUpgraderView::HexDigitConvert(char cChar)
{
	int value;

	if(cChar >= '0' && cChar <= '9')
	{
		value = cChar;
		value = value - 48;
	}
	else if(cChar >= 'A' && cChar <= 'F')
	{
		value = cChar;
		value = value - 65 + 10;
	}
	else if (cChar >= 'a' && cChar <= 'f')
	{
		value = cChar;
		value = value - 97 + 10;
	}
	return value;
}

void CUpgraderView::OnTimer(UINT nIDEvent) 
{	
	// TODO: Add your message handler code here and/or call default
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);	

	switch(nIDEvent)
	{
		case TIMER1:
		case TIMER2:			
			if (m_sendDiscoverCount < 3 && pDoc->m_arrDevice.size() == 0)
			{
				int nResult = sendto( m_Socket, m_szSendDataBuf, m_packetLength, 0, (SOCKADDR *) &m_saUdpServ,sizeof ( SOCKADDR_IN ));
				m_sendDiscoverCount++;
				if( nResult == SOCKET_ERROR ) 
				{
					//KillTimer(TIMER1);
					m_sendDiscoverCount = 0;
					char errBuf[40];
					memset(errBuf, 0, sizeof(errBuf));
					//sprintf( errBuf, "sendto() fail! Error Code: %05d", nResult );
					sprintf( errBuf, "UDP packet sendto error, please check network connection, and click again");
					AfxMessageBox( errBuf );
					return;
				}
				
				if (nIDEvent == TIMER1)
				{
					SetTimer(TIMER2,500,NULL);
					KillTimer(TIMER1);
				}
				else
				{
					SetTimer(TIMER1,500,NULL);
					KillTimer(TIMER2);
				}
				
			}
			else
			{
				m_sendDiscoverCount = 0;
				if (nIDEvent == TIMER1)
					KillTimer(TIMER1);
				else if (nIDEvent == TIMER2)
					KillTimer(TIMER2);
			}
			break;
		case TIMER_OVERTIME:
			if(!ext_bRun)
			{
				ext_bRun = TRUE;
				if (MessageBox( "No response error.\nPlease rescan or reconfig again","error",MB_OK ) == IDOK)
				{
					ext_bRun = FALSE;
				}
			}
			break;
		case TIMER4:
			pWaiting->Close();
			pWaiting = NULL;
			KillTimer(TIMER4);
			break;
	}
	
	CListViewEx::OnTimer(nIDEvent);
}

BOOL CUpgraderView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CListViewEx::PreTranslateMessage(pMsg);
}

LRESULT CUpgraderView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case WM_ACTIVATEAPP:
		WSAGETSELECTEVENT(lParam);
		GetDevicesInfo();
		break;
	default:
		break;
	}
	return CListViewEx::WindowProc(message, wParam, lParam);
}

// 创建套接字 David 2010.6.1
void CUpgraderView::CreateWinSocket()
{
 	WSADATA		wsaData;
	char		szErrMsgBuffer[64];

	int nResult = WSAStartup( MAKEWORD(1,1), &wsaData );
	if (nResult != 0) 
	{
		sprintf( szErrMsgBuffer, "WSAStartup() fail! Error Code: %05d", nResult );
		AfxMessageBox( szErrMsgBuffer );
		return;
	}

	// 取得选择的网卡的IP
	//memset(m_szLocalHostIP, NULL, sizeof(m_szLocalHostIP));
	//GetMacName(m_szLocalHostIP, sizeof(m_szLocalHostIP));
	//struct ifreq if_ppp0;
    //strncpy(if_ppp0.ifr_name, *m_szLocalHostIP, IFNAMSIZ);

	memset(m_szLocalHostIP, NULL, sizeof(m_szLocalHostIP));
	GetLocalIPs(m_szLocalHostIP, sizeof(m_szLocalHostIP));
	
	int len = strlen(m_szLocalHostIP);
	if ( !IsHexDigit(m_szLocalHostIP[len-1]))
		m_szLocalHostIP[len-1] = 0;

	m_Socket = socket( AF_INET, SOCK_DGRAM, 0 );

	// bind to a local socket and an interface.
    m_saUdpCli.sin_family		= AF_INET;
    m_saUdpCli.sin_addr.s_addr	= inet_addr(m_szLocalHostIP);//htonl( INADDR_ANY );
    m_saUdpCli.sin_port			= htons( UDP_UDAP_PORT );

	nResult = bind( m_Socket, (SOCKADDR *)&m_saUdpCli, sizeof(SOCKADDR_IN) );
	if( nResult == SOCKET_ERROR ) 
	{
	nResult = WSAGetLastError();
	sprintf( szErrMsgBuffer, "bind() fail! Error Code: %05d", nResult );
	AfxMessageBox( szErrMsgBuffer );
	return;
	}

	if (WSAAsyncSelect(m_Socket, this->GetSafeHwnd(), WM_ACTIVATEAPP, FD_READ) != 0)
		return;
	
	
	//BindToServer(0xffffffff);
	

	/*in_addr add;
	add.s_addr = inet_addr(m_szLocalHostIP);
	nResult = setsockopt(	m_Socket, 
		IPPROTO_IP,
		IP_MULTICAST_IF,
		(char*)&add,
		sizeof(in_addr)
		);*/

	/*if( nResult == SOCKET_ERROR )
	{
		nResult = WSAGetLastError();
		sprintf( szErrMsgBuffer, "setsockopt() fail! Error Code: %05d", nResult );
		AfxMessageBox( szErrMsgBuffer );
		return;
	}*/

	
}

void CUpgraderView::BindToServer(u_long serverIp)
{

	int nResult;
	
// Fill an IP address structure, to send an IP broadcast. The 
    // packet will be broadcasted to the specified port.
    m_saUdpServ.sin_family		= AF_INET;
    //m_saUdpServ.sin_addr.s_addr	= htonl( INADDR_BROADCAST );
	//m_saUdpServ.sin_addr.s_addr	= htonl( serverIp );
	//m_saUdpServ.sin_addr.s_addr	= htonl( 0xc0a80050 );
    m_saUdpServ.sin_port		= htons( UDP_UDAP_PORT );

	

	// set a socket option
	BOOL bBroadcast = TRUE;
	nResult = setsockopt(	m_Socket, 
		SOL_SOCKET,
		SO_BROADCAST,
		(char*)&bBroadcast,
		sizeof( BOOL )
		);

}

void CUpgraderView::SendAuthentication(UINT configType)
{
	char	chrSendBuffer[128];
	int		nResult;
	int		i = 0;

	memset(chrSendBuffer, 0, sizeof(chrSendBuffer));

	//UCP_METHOD_DISCOVER 设置包为0x0001
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

	// 目标端MAC
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(index);
		for ( int j = 0; j < 6; j++ )	
			//m_szRequest[i++] = (char)iMACAddr[index][j];
			m_szRequest[i++] = (char)pDevice->m_arrMac[j];
	}

	setLocalMacAddress(i);

	//UAP_CLIENT_OPTION 客户端选项0x0002
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 8 ) & 0xff;
	m_szRequest[i++] = (UAP_CLIENT_OPTION >> 0 ) & 0xff;

	// 控制端IP
	CString HostIP = m_szLocalHostIP;
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
		m_szRequest[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	// 设备类型 0xC001
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST 0x01
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP 设备类别 00010001
	m_szRequest[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	// METHOD 这里是认证包 UCP_METHOD_AUTH
	m_szRequest[i++] = 0x00;
	m_szRequest[i++] = 0x05;

	// OperatorType 登录后进行何种操作
	m_szRequest[i++] = 0x00;
	m_szRequest[i++] = (char)configType;

	//Auth User Name
	m_szRequest[i++] = UCP_CODE_AUTH_NAME;
	m_szRequest[i++] = (char)m_strUserName.GetLength();
	sprintf(&m_szRequest[i++],(LPCTSTR) m_strUserName);
	i = i + m_strUserName.GetLength() - 1;

	//Auth Password
	m_szRequest[i++] = UCP_CODE_AUTH_PASSWORD;
	m_szRequest[i++] = (char)m_strPassword.GetLength();
	sprintf(&m_szRequest[i++],(LPCTSTR) m_strPassword);
	i = i + m_strPassword.GetLength() - 1;
	
	nResult = sendto(	m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof ( SOCKADDR_IN ));

	if( nResult == SOCKET_ERROR ) 
	{
		//sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		sprintf( chrSendBuffer, "UDP packet sendto error, please check network connection, and click again");
		AfxMessageBox( chrSendBuffer );
		return;
	}
	
	SetTimer(TIMER_OVERTIME, 2000, NULL);
}

// wireless setting Davidgong 2010.5.27
void CUpgraderView::OnWireless() 
{	
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		
		CLogin	dlg;				
		if (dlg.DoModal() == IDOK)
		{
			m_strUserName = dlg.m_sUserName;
			m_strPassword = dlg.m_sPassword;
			SendAuthentication(UCP_METHOD_GET_WLAN);
			
		}
	}
	
}

void CUpgraderView::OnWizard() 
{
/*
	CBitmap bmpheader;
	bmpheader.LoadBitmap(IDB_HEADER);
	CNGWizard dlg(IDS_WIZARD_TITLE,this,0);
	
//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		for ( int i = 0; i < 6; i++ )
			dlg.SerMacAdde[i] = (char)iMACAddr[index][i];

		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			SetTimer(TIMER_OVERTIME,2000,NULL);
		}
	}
*/
}

void CUpgraderView::GetWirelessInfo()
{
/*
	char	chrSendBuffer[128];
//	BOOL	bBroadcast;
	int		nResult;
	int		i = 0;

	memset(chrSendBuffer, 0, sizeof(chrSendBuffer));

	//UCP_METHOD_DISCOVER
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		for ( int j = 0; j < 6; j++ )	
			m_szRequest[i++] = (char)iMACAddr[index][j];
	}

	//UCP_METHOD_GET_IP
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 0 ) & 0xff;

	CString HostIP = m_szLocalHostIP;
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
		m_szRequest[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	m_szRequest[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	//UCP_METHOD_GET_WLAN
	m_szRequest[i++] = 0x00;
	m_szRequest[i++] = 0x06;

	nResult = sendto( m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof ( SOCKADDR_IN ));

	if( nResult == SOCKET_ERROR ) 
	{
		sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		AfxMessageBox( chrSendBuffer );
		return;
	}
*/
}

void CUpgraderView::GetWizardInfo()
{
/*
	char	chrSendBuffer[128];
	int		nResult;
	int		i = 0;

	memset(chrSendBuffer, 0, sizeof(chrSendBuffer));
	memset(m_szRequest, 0, sizeof(m_szRequest));

	//UCP_METHOD_DISCOVER
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		for ( int j = 0; j < 6; j++ )	
			m_szRequest[i++] = (char)iMACAddr[index][j];
	}

	//UCP_METHOD_GET_IP
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 0 ) & 0xff;

	CString HostIP = m_szLocalHostIP;
	for (i; i < 14; i++)
	{
		CString StrTemp;

		int pos = HostIP.Find('.',0);
		if ( pos != -1 )
			StrTemp = HostIP.Left(pos);
		else
			StrTemp = HostIP;

		int tt = atoi(StrTemp);
		m_szRequest[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	m_szRequest[i++] = (char)(UAP_CLASS_UCP >> 24);
	m_szRequest[i++] = (char)(UAP_CLASS_UCP >> 16);
	m_szRequest[i++] = (char)(UAP_CLASS_UCP >> 8);
	m_szRequest[i++] = (char)(UAP_CLASS_UCP >> 0);

	//UCP_METHOD_GET_WIZARD
	m_szRequest[i++] = (char)UCP_METHOD_GET_WIZARD >> 8;
	m_szRequest[i++] = (char)UCP_METHOD_GET_WIZARD;

	nResult = sendto( m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof ( SOCKADDR_IN ));
	if( nResult == SOCKET_ERROR ) 
	{
		sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		AfxMessageBox( chrSendBuffer );
		return;
	*/
}

void CUpgraderView::GetMiniApInfo()
{
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	char	chrSendBuffer[128];
	int		nResult;
	int		i = 0;

	memset(chrSendBuffer, 0, sizeof(chrSendBuffer));

	//UCP_METHOD_DISCOVER
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(index);
		for ( int j = 0; j < 6; j++ )	
			//m_szRequest[i++] = (char)iMACAddr[index][j];
			m_szRequest[i++] = (char)pDevice->m_arrMac[j];
	}
	setLocalMacAddress(i);

	//UCP_METHOD_GET_IP
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 0 ) & 0xff;

	CString strHostIP = m_szLocalHostIP;
	for (i; i < 14; i++)
	{
		CString StrTemp;
		int pos = strHostIP.Find('.',0);
		if ( pos != -1 )
			StrTemp = strHostIP.Left(pos);
		else
			StrTemp = strHostIP;

		int tt = atoi(StrTemp);
		m_szRequest[i] = (char)tt;
		strHostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	m_szRequest[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	//UCP_METHOD_GET_INFO
	m_szRequest[i++] = (UCP_METHOD_GET_INFO >> 8 ) & 0xff;
	m_szRequest[i++] = UCP_METHOD_GET_INFO & 0xff;

	nResult = sendto( m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof ( SOCKADDR_IN ));
	//*dialFlag = 1;
	if( nResult == SOCKET_ERROR ) 
	{
		//sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		sprintf( chrSendBuffer, "UDP packet sendto error, please check network connection, and click again");
		AfxMessageBox( chrSendBuffer );
		return;
	}
	
	SetTimer(TIMER_OVERTIME, 2000, NULL);
}

void CUpgraderView::InterfaceSelect()
{
	//memset(m_szLocalHostIP,NULL,sizeof(m_szLocalHostIP));
	//GetLocalIPs (m_szLocalHostIP, sizeof(m_szLocalHostIP));
	//int len = strlen(m_szLocalHostIP);
	//if ( IsHexDigit(m_szLocalHostIP[len-1]) != 1 )
	//	m_szLocalHostIP[len-1]
	//
	//len = strcspn(m_szLocalHostIP, szIPEnd);
	//m_szLocalHostIP[len] = 0;
	//
	//HexToChar(buffer, 4 , m_npBufferIndex , ip);
	//Ip2String(ip, ip);
}

// 选择网卡连接 david 2010.6.1
void CUpgraderView::OnToolsSelectconnection()
{
	CSelectConectionDlg dlg;

	// 选择后重新建立socket
	if (IDOK == dlg.DoModal())
	{
		closesocket(m_Socket);
		WSACleanup();
		CreateWinSocket();
		StartDiscovery();
	}
}

void CUpgraderView::OnUpdateToolsSelectconnection(CCmdUI *pCmdUI)
{
	//CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	//if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
	//	pCmdUI->Enable(TRUE);
	//else
	//	pCmdUI->Enable(FALSE);
}

LRESULT CUpgraderView::OnChangeSocket(WPARAM, LPARAM)
{
	return 0;
}

// 重启设备
void CUpgraderView::OnReboot()
{
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();
	if (index != -1)
	{
		
		CLogin	dlg;
		if (dlg.DoModal() == IDOK)
		{
			
			m_strUserName = dlg.m_sUserName;
			m_strPassword = dlg.m_sPassword;
			SendAuthentication(UCP_METHOD_REBOOT);
		}
	}
	
}

void CUpgraderView::OnUpdateReboot(CCmdUI *pCmdUI)
{
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	if (ctlList.GetNextItem(-1, LVNI_SELECTED) != -1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

// 发送重启命令包 David 2010.6.7
void CUpgraderView::SendReboot()
{
	char	chrSendBuffer[128];
	int		nResult;
	int		i = 0;

	memset(chrSendBuffer, 0, sizeof(chrSendBuffer));

	// 设置包
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_DISCOVER >> 0 ) & 0xff;

	//	CMyListCtrl& ctlList = (CMyListCtrl&) GetListCtrl();
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	int index = ctlList.GetSelectionMark();//.GetSelectedCount();//.HitTest(point);
	if (index != -1)
	{
		LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(index);
		for ( int j = 0; j < 6; j++ )	
			//m_szRequest[i++] = (char)iMACAddr[index][j];
			m_szRequest[i++] = (char)pDevice->m_arrMac[j];
	}
	setLocalMacAddress(i);

	//UCP_METHOD_GET_IP
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_GET_IP >> 0 ) & 0xff;

	CString HostIP = m_szLocalHostIP;
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
		m_szRequest[i] = (char)tt;
		HostIP.Delete(0,pos+1);
	}

	//UDP Port Number
	m_szRequest[i++] = (UDP_UDAP_PORT >> 8)&0xff;
	m_szRequest[i++] = (UDP_UDAP_PORT >> 0)&0xff;

	//SequenceNum
	m_szRequest[i++] = 0x01;
	m_szRequest[i++] = 0x00;

	//UDAP_TYPE_IPCONFIGURE
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 8)&0xff);
	m_szRequest[i++] = (char)((UDAP_TYPE_IPCONFIGURE >> 0)&0xff);

	//UAP_FLAG_REQUEST
	m_szRequest[i++] = UAP_FLAG_REQUEST;

	//UAP_CLASS_UCP
	m_szRequest[i++] = (UAP_CLASS_UCP >> 24)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 16)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 8)&0xff;
	m_szRequest[i++] = (UAP_CLASS_UCP >> 0)&0xff;

	// Method 这里为重启
	m_szRequest[i++] = (UCP_METHOD_REBOOT >> 8 ) & 0xff;
	m_szRequest[i++] = (UCP_METHOD_REBOOT >> 0 ) & 0xff;

	nResult = sendto(m_Socket, m_szRequest, i, 0, (SOCKADDR *) &m_saUdpServ,sizeof (SOCKADDR_IN));

	if( nResult == SOCKET_ERROR ) 
	{
		//sprintf( chrSendBuffer, "sendto() fail! Error Code: %05d", nResult );
		sprintf( chrSendBuffer, "UDP packet sendto error, please check network connection, and click again");
		AfxMessageBox( chrSendBuffer );
		return;
	}
	
}

// 显示IP配置的界面 David 2010.6.12 Add
void CUpgraderView::ShowIPConfigDlg()
{
	CIpConfig	dlgIPConfig(this);		// IP配置对话框

	dlgIPConfig.m_bridge = 0;
	memset(dlgIPConfig.m_chrGetIP,NULL,4);
	memset(dlgIPConfig.m_chrGetSubMask,NULL,4);
	memset(dlgIPConfig.m_chrGateway,NULL,4);
	memset(dlgIPConfig.LocalMacAddr, NULL, 6);
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	dlgIPConfig.m_bridge = pDoc->m_RecieveData.bridge;
	
	for (int n = 0; n < 4; n++ )
	{
		dlgIPConfig.m_chrGetIP[n] = (char)pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[n];
		dlgIPConfig.m_chrGetSubMask[n] = (char)pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[n];
		dlgIPConfig.m_chrGateway[n] = (char)pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[n];
	}

	for ( int j = 0; j < 6; j++ )
		dlgIPConfig.SerMacAdde[j] = (char)pDoc->m_RecieveData.m_arrMacAddr[j];

	for(int k = 0; k < 6;k ++){
		dlgIPConfig.LocalMacAddr[k] = pDoc->m_pDefaultAdapter->Address[k];
	}
 

	dlgIPConfig.m_Socket = m_Socket;
	dlgIPConfig.saUdpServ = m_saUdpServ;
	dlgIPConfig.m_iDhcpEnable = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nDHCP;
	
	if(dialFlag[0]==1){
		dialFlag[0]=0;
		dlgIPConfig.DoModal();
		dialFlag[0]=1;
	}
	
}

// 显示设备信息的界面 David 2010.6.12 Add
void CUpgraderView::ShowDeviceInfoDlg()
{
	CDeviceInfoDlg	dlgInfo;			// 设备信息对话框
	char charTemp[33];
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	// Device Name
	dlgInfo.m_strModelName = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_strDeviceName;

	// Ethernet MAC Address
	dlgInfo.m_strMacAddr.Format("%02X:%02X:%02X:%02X:%02X:%02X", 
								pDoc->m_RecieveData.m_arrMacAddr[0],
								pDoc->m_RecieveData.m_arrMacAddr[1],
								pDoc->m_RecieveData.m_arrMacAddr[2],
								pDoc->m_RecieveData.m_arrMacAddr[3],
								pDoc->m_RecieveData.m_arrMacAddr[4],
								pDoc->m_RecieveData.m_arrMacAddr[5]);

	// Wireless MAC Address
	dlgInfo.m_strWlanMacAddr = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_szcharWlanMACAddress;

	// Firmware Version
	dlgInfo.m_strFwVer = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_strFirmware;

	// IP Address
	sprintf(charTemp,"%d.%d.%d.%d", 
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[0], 
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[1],
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[2], 
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrIPAddr[3]);
	dlgInfo.m_strIpAddr = charTemp;

	// Subnet Mask
	sprintf(charTemp,"%d.%d.%d.%d", 
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[0],
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[1],
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[2], 
		pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrSubmask[3]);
	dlgInfo.m_strSubnetMask = charTemp;

	// Default Gateway 如果设置为空，则界面全部显示为空
	if ((CFG_OP_MODE_AP==pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nOper)
		|| (CFG_OP_MODE_STA0==pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nOper)
		|| (CFG_OP_MODE_STA1==pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nOper))
	{
		if ((pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[0]==0) 
			&& (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[1]==0) 
			&& (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[2]==0) 
			&& (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[3]==0))
			sprintf(charTemp,"");
		else
			sprintf(charTemp,"%d.%d.%d.%d", 
			pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[0],
			pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[1],
			pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[2],
			pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_arrGateway[3]);
	}
	else
		sprintf(charTemp,"--");
	dlgInfo.m_strGateway = charTemp;

	// DHCP Client
	if ((CFG_OP_MODE_AP==pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nOper)
		|| (CFG_OP_MODE_STA0==pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nOper)
		|| (CFG_OP_MODE_STA1==pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nOper))
	{
		if (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nDHCP == 1)
			sprintf(charTemp,"Enabled");
		else
			sprintf(charTemp,"Disabled");
	}
	else
			sprintf(charTemp,"--");
	dlgInfo.m_strDhcp = charTemp;

	// Operation Mode
	switch(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nOper)
	{
		case CFG_OP_MODE_NORMAL:
			sprintf(charTemp,"Router ");
			break;
		case CFG_OP_MODE_AP:
			sprintf(charTemp,"Access Point ");
			break;
		case CFG_OP_MODE_WISP0:
			sprintf(charTemp,"WISP ");
			break;
		case CFG_OP_MODE_STA0:
			sprintf(charTemp,"Wireless Client ");
			break;
		case CFG_OP_MODE_WDSSTA0:
			sprintf(charTemp,"WDS Client");
			break;
		case CFG_OP_MODE_WISP1:
			sprintf(charTemp,"WISP ");
			break;
		case CFG_OP_MODE_STA1:
			sprintf(charTemp,"Wireless Client ");
			break;
		case CFG_OP_MODE_WDSSTA1:
			sprintf(charTemp,"WDS Client ");
			break;
		case CFG_OP_MODE_UR:
			sprintf(charTemp,"Wireless Bridge ");
			break;
		case CFG_OP_MODE_REPEATER:
			sprintf(charTemp,"Repeater ");
			break;
		case CFG_OP_MODE_ADHOC:
			sprintf(charTemp,"ADHOC ");
			break;
		default:
			ASSERT(FALSE);
			break;
	}
	dlgInfo.m_strOpMode = charTemp;

	// Wireless Mode
	if ((pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nSuperG & ABOLT_TURBO_G) 
		&& (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nSuperG & ABOLT_TURBO_PRIME))
		sprintf(charTemp,"2.4GHz 802.11 Super G dynamic");
	else
	{
		switch(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nWirelessMode)
		{
			case MODE_SELECT_11A:
				sprintf(charTemp,"802.11a");
				break;
			case MODE_SELECT_11AN:
				sprintf(charTemp,"802.11a/n");
				break;
			case MODE_SELECT_11N5G:
				sprintf(charTemp,"802.11n");
				break;
			case MODE_SELECT_11AC:
				sprintf(charTemp,"802.11ac");
				break;
			case MODE_SELECT_11BG:
				sprintf(charTemp,"802.11b/g");
				break;
			case MODE_SELECT_11BGN:
				sprintf(charTemp,"802.11b/g/n");
				break;
			case MODE_SELECT_11B:
				sprintf(charTemp,"802.11b");
				break;
			case MODE_SELECT_11G:
				sprintf(charTemp,"802.11g");
				break;
			case MODE_SELECT_11N:
				sprintf(charTemp,"802.11n");
				break;
			default:
				sprintf(charTemp,"--");
				break;
		}
	}
	dlgInfo.m_strWlanMode = charTemp;
	
	//Associated
	if(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_associated==1)
	{
       sprintf(charTemp,"Associated");
	   dlgInfo.m_strAssociated = charTemp;
    }
	else if (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_associated==2)
	{
	   sprintf(charTemp,"Unkown");
	   dlgInfo.m_strAssociated = charTemp;
	}
	else if (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_associated==3)
	{
	   sprintf(charTemp,"--");
	   dlgInfo.m_strAssociated = charTemp;
	}
	else
	{
      sprintf(charTemp,"Disassociated");
	  dlgInfo.m_strAssociated = charTemp;
	}

	//RSSI
	dlgInfo.m_strRssi = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_Rssi;

	//BSSID
	if (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_Bssid == "00:00:00:00:00:00)")
	{
		sprintf(charTemp,"");
		dlgInfo.m_strBssid = charTemp;
	}
	else
	{
		dlgInfo.m_strBssid = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_Bssid;
	}
	
	// SSID
	dlgInfo.m_strSsid = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_strSSID;

	// Channel / Frequency
	if (200 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nChannel)
	{
		sprintf(charTemp, "--");
	}
	else if (0 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nChannel)
	{
		sprintf(charTemp, "Unkown");
	}
	else
	{
		sprintf(charTemp, "Channel %d/ %dMHz",pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nChannel, 
		2407+(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nChannel*5));
	}
	dlgInfo.m_strChannel = charTemp;

	// Security
	switch(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nSecurity)
	{
	case AUTH_OPEN_SYSTEM:
		if(!pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nEncryptionOnOff)
			sprintf(charTemp, "Open System");
		else 
			sprintf(charTemp, "Open System");
		break;
	case AUTH_SHARED_KEY:
		sprintf(charTemp, "WEP Shared Key");
		break;
	case AUTH_WPA:
		sprintf(charTemp, "WPA-802.1x");
		break;
	case AUTH_WPA2:
		sprintf(charTemp, "WPA2-802.1x");
		break;
	case AUTH_WPA2MIX:
		sprintf(charTemp, "WPA2-Mixed-802.1x");
		break;
	case AUTH_WPAPSK:
		sprintf(charTemp, "WPA-PSK");
		break;
	case AUTH_WPA2PSK:
		sprintf(charTemp, "WPA2-PSK");
		break;
	case AUTH_WPA2MIXPSK:
		sprintf(charTemp, "WPA2-Mixed-PSK");
		break;
	case AUTH_AUTO:
		sprintf(charTemp, "WEP Auto");
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	dlgInfo.m_strSecurity = charTemp;

   // 5G Wireless Mode
	switch(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GWirelessMode)
	{
		case MODE_SELECT_11A:
			sprintf(charTemp,"802.11a");
			break;
		case MODE_SELECT_11AN:
			sprintf(charTemp,"802.11a/n");
			break;
		case MODE_SELECT_11N5G:
			sprintf(charTemp,"802.11n");
			break;
		case MODE_SELECT_11AC:
			sprintf(charTemp,"802.11ac");
			break;
		case MODE_SELECT_11BG:
			sprintf(charTemp,"802.11b/g");
			break;
		case MODE_SELECT_11BGN:
			sprintf(charTemp,"802.11b/g/n");
			break;
		case MODE_SELECT_11B:
			sprintf(charTemp,"802.11b");
			break;
		case MODE_SELECT_11G:
			sprintf(charTemp,"802.11g");
			break;
		case MODE_SELECT_11N:
			sprintf(charTemp,"802.11n");
			break;
		default:
			sprintf(charTemp,"--");
			break;
	}
	dlgInfo.m_str5GWlanMode = charTemp;
	
	//5G Wireless MAC Address
	dlgInfo.m_str5GWlanMacAddr = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_szchar5GWlanMACAddress;

	//5G SSID
	dlgInfo.m_str5GSsid = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_str5GSSID;

	//5G RSSI
	dlgInfo.m_str5GRssi = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_5GRssi;

	//5G BSSID
	if (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_5GBssid == "00:00:00:00:00:00)")
	{
		sprintf(charTemp,"");
		dlgInfo.m_str5GBssid = charTemp;
	}
	else
	{
		dlgInfo.m_str5GBssid = pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_5GBssid;
	}

	//5G Associated
	if(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_5Gassociated==1)
	{
       sprintf(charTemp,"Associated");
	   dlgInfo.m_str5GAssociated = charTemp;
    }
	else if (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_5Gassociated==2)
	{
	   sprintf(charTemp,"Unkown");
	   dlgInfo.m_str5GAssociated = charTemp;
	}
	else if (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_5Gassociated==3)
	{
	   sprintf(charTemp,"--");
	   dlgInfo.m_str5GAssociated = charTemp;
	}
	else
	{
      sprintf(charTemp,"Disassociated");
	  dlgInfo.m_str5GAssociated = charTemp;
	}

	//5G Channel / Frequency
	if (200 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GChannel)
	{
		sprintf(charTemp, "--");
	}
	else if (0 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GChannel)
	{
		sprintf(charTemp, "Unkown");
	}
	else
	{
		switch (pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GChannel)
		{
			case 0:
				sprintf(charTemp, "Unkown");
				break;
			case 36:
				sprintf(charTemp, "Channel 36/ 5180MHz");
				break;
			case 40:
				sprintf(charTemp, "Channel 40/ 5200MHz");
				break;
			case 44:
				sprintf(charTemp, "Channel 44/ 5220MHz");
				break;
			case 48:
				sprintf(charTemp, "Channel 48/ 5240MHz");
				break;
			case 52:
				sprintf(charTemp, "Channel 52/ 5260MHz");
				break;
			case 56:
				sprintf(charTemp, "Channel 56/ 5280MHz");
				break;
			case 60:
				sprintf(charTemp, "Channel 60/ 5300MHz");
				break;
			case 64:
				sprintf(charTemp, "Channel 64/ 5320MHz");
				break;
			case 100:
				sprintf(charTemp, "Channel 100/ 5500MHz");
				break;
			case 104:
				sprintf(charTemp, "Channel 104/ 5520MHz");
				break;
			case 108:
				sprintf(charTemp, "Channel 108/ 5540MHz");
				break;
			case 112:
				sprintf(charTemp, "Channel 112/ 5560MHz");
				break;
			case 116:
				sprintf(charTemp, "Channel 116/ 5580MHz");
				break;
			case 120:
				sprintf(charTemp, "Channel 120/ 5600MHz");
				break;
			case 124:
				sprintf(charTemp, "Channel 124/ 5620MHz");
				break;
			case 128:
				sprintf(charTemp, "Channel 128/ 5640MHz");
				break;
			case 132:
				sprintf(charTemp, "Channel 132/ 5660MHz");
				break;
			case 136:
				sprintf(charTemp, "Channel 136/ 5680MHz");
				break;
			case 140:
				sprintf(charTemp, "Channel 140/ 5700MHz");
				break;
			case 149:
				sprintf(charTemp, "Channel 149/ 5745MHz");
				break;
			case 153:
				sprintf(charTemp, "Channel 153/ 5765MHz");
				break;
			case 157:
				sprintf(charTemp, "Channel 157/ 5785MHz");
				break;
			case 161:
				sprintf(charTemp, "Channel 161/ 5805MHz");
				break;
			case 165:
				sprintf(charTemp, "Channel 165/ 5825MHz");
				break;
			default:
				sprintf(charTemp, "--");
				break;
		}
	}
	dlgInfo.m_str5GChannel = charTemp;

	// Security
	switch(pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GSecurity)
	{
	case AUTH_OPEN_SYSTEM:
		if(!pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GEncryptionOnOff)
			sprintf(charTemp, "Open System");
		else 
			sprintf(charTemp, "Open System");
		break;
	case AUTH_SHARED_KEY:
		sprintf(charTemp, "WEP Shared Key");
		break;
	case AUTH_WPA:
		sprintf(charTemp, "WPA-802.1x");
		break;
	case AUTH_WPA2:
		sprintf(charTemp, "WPA2-802.1x");
		break;
	case AUTH_WPA2MIX:
		sprintf(charTemp, "WPA2-Mixed-802.1x");
		break;
	case AUTH_WPAPSK:
		sprintf(charTemp, "WPA-PSK");
		break;
	case AUTH_WPA2PSK:
		sprintf(charTemp, "WPA2-PSK");
		break;
	case AUTH_WPA2MIXPSK:
		sprintf(charTemp, "WPA2-Mixed-PSK");
		break;
	case AUTH_AUTO:
		sprintf(charTemp, "WEP Auto");
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	dlgInfo.m_str5GSecurity = charTemp;

	// Associated RSSI BSSID
	if (dlgInfo.m_strAssociated == "Disassociated") {
		sprintf(charTemp, "");
		dlgInfo.m_strRssi = charTemp;
		dlgInfo.m_strBssid = charTemp;
	}

	// 5G Associated RSSI BSSID
	if (dlgInfo.m_str5GAssociated == "Disassociated") {
		sprintf(charTemp, "");
		dlgInfo.m_str5GRssi = charTemp;
		dlgInfo.m_str5GBssid = charTemp;
	}

	//2.4G WLAN OP Mode
	if (1 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nWlanOpmode) {
		sprintf(charTemp, "Wireless Client Mode");
	}
	else
	{
		sprintf(charTemp, "Access Point Mode");
	}
	dlgInfo.m_strWlanOpMode = charTemp;

	//5G WLAN OP Mode
	if (1 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GWlanOpmode) {
		sprintf(charTemp, "Wireless Client Mode");
	}
	else
	{
		sprintf(charTemp, "Access Point Mode");
	}
	dlgInfo.m_str5GWlanOpMode = charTemp;
	
	//2.4G RadioStatus
	if (0 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_nRadioOnOff) {
		sprintf(charTemp, "--");
		dlgInfo.m_strWlanMode = charTemp;
		dlgInfo.m_strSsid = charTemp;
		dlgInfo.m_strRssi = charTemp;
		dlgInfo.m_strBssid = charTemp;
		dlgInfo.m_strAssociated = charTemp;
		dlgInfo.m_strChannel = charTemp;
		dlgInfo.m_strSecurity = charTemp;
		dlgInfo.m_strWlanMacAddr = charTemp;
		dlgInfo.m_strWlanOpMode = charTemp;
	}

	//5G RadioStatus
	if (0 == pDoc->m_RecieveData.m_SendData.m_GetinfoData.m_n5GRadioOnOff) {
		sprintf(charTemp, "--");
		dlgInfo.m_str5GWlanMode = charTemp;
		dlgInfo.m_str5GSsid = charTemp;
		dlgInfo.m_str5GRssi = charTemp;
		dlgInfo.m_str5GBssid = charTemp;
		dlgInfo.m_str5GAssociated = charTemp;
		dlgInfo.m_str5GChannel = charTemp;
		dlgInfo.m_str5GSecurity = charTemp;
		dlgInfo.m_str5GWlanMacAddr = charTemp;
		dlgInfo.m_str5GWlanOpMode = charTemp;
	}
	
	if(dialFlag[0]==1){
		dialFlag[0]=0;
	dlgInfo.DoModal();
	dialFlag[0]=1;
	}
}

//将本机mac地址设置进去
void CUpgraderView::setLocalMacAddress(int& i){
	CUpgraderDoc* pDoc = GetDocument();
#ifdef UNICAST
	int k = 0;
	/*int j = inet_addr(m_szLocalHostIP)&inet_addr("255.255.255.0");
	int l = (m_saUdpServ.sin_addr.s_addr)&inet_addr("255.255.255.0");
	int m = inet_addr("192.168.115.80")&inet_addr("255.255.255.0");
	int n = inet_addr("192.168.115.74")&inet_addr("255.255.255.0");
	int p = inet_addr("255.255.255.255")&inet_addr("255.255.255.0");*/
	if((inet_addr(m_szLocalHostIP)&inet_addr("255.255.255.0")) !=
		((m_saUdpServ.sin_addr.s_addr)&inet_addr("255.255.255.0"))){
			//if peer ip is 0 or different net ,broadcast
		for(k = 0; k < 6;k ++){
			m_szRequest[i++] = 0xFF;
		
			LocalMacAddr[k] = 0xFF;
		}
	}else{
		//unicast
		for(k = 0; k < 6;k ++){
			m_szRequest[i++] = pDoc->m_pDefaultAdapter->Address[k];
			
			LocalMacAddr[k] = pDoc->m_pDefaultAdapter->Address[k];
			
		}
	}
#else
	int k = 0;
	for(k = 0; k < 6;k ++){
		m_szRequest[i++] = 0xFF;
		
		LocalMacAddr[k] = 0xFF;
		
	}
#endif
	

}

//显示主机信息对话框
void CUpgraderView::ShowHostSetDlg()
{
 
 CHostSetting dlg(this);
 dlg.m_domainName = "";
 dlg.m_hostName = "";
 memset(dlg.m_chrGetIP, NULL, 4);

 
 CUpgraderDoc* pDoc = GetDocument();
 ASSERT(NULL != pDoc);
 dlg.m_domainName_old = dlg.m_domainName = pDoc->m_RecieveData.m_SendData.m_HostData.m_domainName;
 dlg.m_hostName_old = dlg.m_hostName = pDoc->m_RecieveData.m_SendData.m_HostData.m_hostName;
 for(int n = 0; n < 4; n++)
 {
	 dlg.m_chrGetIP_old[n] = dlg.m_chrGetIP[n] = pDoc->m_RecieveData.m_SendData.m_HostData.m_IPAddr[n];
	 
 }
 for ( int j = 0; j < 6; j++ )
		dlg.SerMacAdde[j] = (char)pDoc->m_RecieveData.m_arrMacAddr[j];

	
 for(int k = 0; k < 6;k ++){
		dlg.LocalMacAddr[k] = LocalMacAddr[k];
 }

//---------------------------------------
/*avoid the compiling difference between debug version and release version
 especially when showing host ip
 TODO:find out the difference of compling result between debug and release even at condition of the same source code
 */
 int j = 0;
 j+=3;
//--------------------------------------
 
 dlg.saUdpServ = m_saUdpServ;
 dlg.m_Socket = m_Socket;
 if(dialFlag[0]==1){
	 dialFlag[0]=0;
	 dlg.DoModal();
	 dialFlag[0]=1;
 }


}
// 接收包之后的工作 David 2010.6.12 Add
void CUpgraderView::LaterAction()
{
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	KillTimer(TIMER_OVERTIME);
	switch (pDoc->m_RecieveData.m_SendData.m_uMethod)
	{
	case UCP_METHOD_DISCOVER:
		{
			// 检查界面和内存
			CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
			// 清除界面
			ctlList.DeleteAllItems();
			// 更新内存到界面
			for (int i=0; i<pDoc->m_arrDevice.size(); ++i)
			{
				LCT_DEVICE* pDevice = pDoc->m_arrDevice[i];
				LV_ITEM		lvitem;
				lvitem.mask = LVIF_TEXT;
				lvitem.iSubItem = 0;
				lvitem.pszText = _T("");
				lvitem.iItem = i;
				ctlList.InsertItem(&lvitem);

				ctlList.SetItemText(i, 0, pDevice->m_strName);
				ctlList.SetItemText(i, 1, pDevice->m_strType);
				if (pDevice->m_strIPAddr == "0.0.0.0")
					ctlList.SetItemText(i, 2, "No IP Address");
				else
					ctlList.SetItemText(i, 2, pDevice->m_strIPAddr);
				ctlList.SetItemText(i, 3, pDevice->m_strMACAddr);
				ctlList.SetItemText(i, 4, pDevice->m_strFirmwareVer);
				ctlList.SetItemData(i, (DWORD)pDevice);
			}
			// 更新界面已有设备的信息					
			break;
		}		
 
	case UCP_METHOD_GET_IP:	
		ShowIPConfigDlg();			
		break;
	case  UCP_METHOD_GET_HOST:{
		ShowHostSetDlg();					  
		break;
		}
	case UCP_METHOD_GET_INFO:
		ShowDeviceInfoDlg();
		break;
	case UCP_METHOD_SET_WIZARD:
	case UCP_METHOD_SET_WLAN:
	case UCP_METHOD_SET_IP:	
		{
			//设定是否成功的返回包
		int result = pDoc->m_RecieveData.m_SendData.ip_set_result;
		if(result == 0x14){
			//set success
			CWaitReset	dlgw(40000);
			dlgw.DoModal();
			EndWaitCursor();	// Remove the hourglass cursor.
		}else if(result== 0x15){
			//set fail
			MessageBox("Set failure!", "Error", MB_OK);
		}else if(result == 0x16){
			//no need set
			MessageBox("Successfully set !",MB_OK);
		}
		break;
		}
	case UCP_METHOD_RESET:
		{
			KillTimer(TIMER_OVERTIME);
			BeginWaitCursor();	// Display the hourglass cursor.
			CWaitReset	dlgw(50000);
			dlgw.DoModal();
			EndWaitCursor();	// Remove the hourglass cursor.
			StartDiscovery();
			break;
		}				
	case UCP_METHOD_REBOOT:
		{
			KillTimer(TIMER_OVERTIME);
			BeginWaitCursor();	// Display the hourglass cursor.
			CWaitReset	dlgw(50000);
			dlgw.DoModal();
			EndWaitCursor();	// Remove the hourglass cursor.
			StartDiscovery();
			break;
		}
	case UCP_METHOD_SET_HOST:{
		int result = pDoc->m_RecieveData.m_SendData.ip_set_result;
		if(result == 0x14|| result == 0x16){
			MessageBox("Successfully set !",MB_OK);
		}else if(result == 0x15){
			MessageBox("Set failure!", "Error", MB_OK);
		}
		break;
		}
	case UCP_METHOD_AUTH:		//登录认证回应包
		{
			KillTimer(TIMER_OVERTIME);
			switch(pDoc->m_RecieveData.m_SendData.m_AuthData.m_uRequestMethod)
			{
			case UCP_METHOD_RESET:
				SetFactoryDefault();
				SetTimer(TIMER_OVERTIME, 2000, NULL);
				break;
			case UCP_METHOD_GET_IP:
				SendConfig();
				//SetTimer(TIMER_OVERTIME, 2000, NULL);
				break;
			case UCP_METHOD_GET_WLAN:
				GetWirelessInfo();
				SetTimer(TIMER_OVERTIME, 2000, NULL);
				break;
			case UCP_METHOD_REBOOT:
				SendReboot();
				SetTimer(TIMER_OVERTIME, 2000, NULL);
				break;
			case UCP_METHOD_GET_HOST:
				SendHost();
				break;
			default:
				//ASSERT(FALSE);
				break;
			}
			break;
		}
	}
}

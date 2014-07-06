// WaitReset.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WaitReset.h"
#include "mainfrm.h"
#include "upgraderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitReset dialog


CWaitReset::CWaitReset(ULONG uTime, CWnd* pParent /*=NULL*/)
	: CDialog(CWaitReset::IDD, pParent)
{
	m_uMaxWaitTime = uTime;
	//{{AFX_DATA_INIT(CWaitReset)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ulTimeOut = 0;
}


void CWaitReset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitReset)
	DDX_Control(pDX, IDC_PROGRESS_WAITING, m_ctrlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitReset, CDialog)
	//{{AFX_MSG_MAP(CWaitReset)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitReset message handlers

void CWaitReset::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER10:
		RestoreWaitCursor();        // Restore the hourglass cursor.
		m_ctrlProgress.SetPos(m_ulTimeOut * 100 / m_uMaxWaitTime);
		m_ulTimeOut += 100;
		if(m_ulTimeOut >= m_uMaxWaitTime)
		{
			OnOK();
		}
		break;
	case TIMER11:		// 探测设备端的IP
		{
			// 1.发送探测包
			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			CUpgraderView* pView = (CUpgraderView*)pMainFrm->GetActiveView();
			pView->GetDiscoveryData();
			int nResult = sendto( pView->m_Socket, pView->m_szSendDataBuf, pView->m_packetLength, 0, (SOCKADDR *) &(pView->m_saUdpServ),sizeof ( SOCKADDR_IN ));
			if( nResult == SOCKET_ERROR )
			{
				break;
			}
			

			// 2.接收设备端的回复
			const int	MSG_CHUNK = 512;
			char		buffer[MSG_CHUNK] = "\0";
			sockaddr_in soctFrom;	// 设备端的IP地址
			memset(&soctFrom, NULL, sizeof(sockaddr));
			int	nFromLen = sizeof(sockaddr_in);	// 数据包长度	
			int status = recvfrom(pView->m_Socket, buffer, MSG_CHUNK, 0, (sockaddr*)&soctFrom, &nFromLen);
			int nCount = 1;
			CString strServIP = inet_ntoa(soctFrom.sin_addr);
			// 取得设置之前的设备的MAC地址
			CString strPreMAC;
			CListCtrl& ctlList = (CListCtrl&) pView->GetListCtrl();
			int index = ctlList.GetSelectionMark();//.GetSelectedCount();//.HitTest(point);
			//ASSERT(-1 != index);
			if (index != -1)
			{
				LCT_DEVICE* pDevice = (LCT_DEVICE*)ctlList.GetItemData(index);
				strPreMAC.Format("%02X:%02X:%02X:%02X:%02X:%02X",
					pDevice->m_arrMac[0],
					pDevice->m_arrMac[1],
					pDevice->m_arrMac[2],
					pDevice->m_arrMac[3],
					pDevice->m_arrMac[4],
					pDevice->m_arrMac[5]);
				//for ( int j = 0; j < 6; j++ )	
				//	strPreMAC = strPreMAC+(char)pDevice->m_arrMac[j];
			}
			if (!strPreMAC.IsEmpty())
				m_strPreMac = strPreMAC;
			// 取得设备端发来的数据包中的MAC地址
			int	iMACAddrTemp[6];
			int npBufferIndex = 10;
			for (int j=0; j<6; j++)
			{
				HexToint(buffer, 1 , npBufferIndex , &iMACAddrTemp[j]);
			}
			char chrMacAddrTemp[18];
			memset(chrMacAddrTemp, 0 , sizeof(chrMacAddrTemp));
			_stprintf(chrMacAddrTemp,"%02X:%02X:%02X:%02X:%02X:%02X", 
				iMACAddrTemp[0], iMACAddrTemp[1], iMACAddrTemp[2], 
				iMACAddrTemp[3], iMACAddrTemp[4], iMACAddrTemp[5]);					
			CString	strRcvMAC = chrMacAddrTemp;

			// 如果接收了5次仍然没有收到设备端的回复就放弃
			while ( m_strPreMac!=strRcvMAC && nCount<=5 )
			{				
				Sleep(10);
				memset(buffer, 0, MSG_CHUNK);
				memset(&soctFrom, NULL, sizeof(sockaddr));
				status = recvfrom(pView->m_Socket, buffer, MSG_CHUNK, 0, (sockaddr*)&soctFrom, &nFromLen);
				nCount++;
				
				// 接收失败
				if (status <= 0)
					continue;

				// 如果不是设备端发来的数据包就继续接收
				if (buffer[1]!=0x02)
					continue;
				
				// 取得设备端发来的数据包中的MAC地址				
				npBufferIndex = 10;
				for (int j=0; j<6; j++)
				{
					HexToint(buffer, 1 , npBufferIndex , &iMACAddrTemp[j]);
				}
				
				memset(chrMacAddrTemp, 0 , sizeof(chrMacAddrTemp));
				_stprintf(chrMacAddrTemp,"%02X:%02X:%02X:%02X:%02X:%02X", 
					iMACAddrTemp[0], iMACAddrTemp[1], iMACAddrTemp[2], 
					iMACAddrTemp[3], iMACAddrTemp[4], iMACAddrTemp[5]);					
				strRcvMAC = chrMacAddrTemp;

				strServIP = inet_ntoa(soctFrom.sin_addr);
								
			}

			// 3.若回复包中的IP不为0.0.0.0，则说明设备端设置完成，关闭进度条
			if (strServIP != "0.0.0.0"
				&& m_strPreMac==strRcvMAC)
			{
				m_ulTimeOut = m_uMaxWaitTime;
				m_ctrlProgress.SetPos(m_ulTimeOut * 100 / m_uMaxWaitTime);
				Sleep(10);
				if(m_ulTimeOut >= m_uMaxWaitTime)
				{
					OnOK();
				}
			}
			break;
		}		
	}
   
	CDialog::OnTimer(nIDEvent);
}

void CWaitReset::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    KillTimer(TIMER10);
	KillTimer(TIMER11);
	
}

BOOL CWaitReset::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetTimer(TIMER10 , 100 , NULL);
	
    m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetStartColor(RGB(100,100,255));

	// 每隔两秒发送一个探测包
	SetTimer(TIMER11 , 1000 , NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

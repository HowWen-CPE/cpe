// SelectConectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "SelectConectionDlg.h"
#include "upgraderDoc.h"
#include "MainFrm.h"
#include "upgraderView.h"

// CSelectConectionDlg dialog

IMPLEMENT_DYNAMIC(CSelectConectionDlg, CDialog)

// list的列下标
enum ADAPTER_COL
{
	ADT_DIS = 0,
	ADT_IP
};

CSelectConectionDlg::CSelectConectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectConectionDlg::IDD, pParent)
{
}

CSelectConectionDlg::~CSelectConectionDlg()
{
	
}

void CSelectConectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONNECTION, m_listConnection);
}


BEGIN_MESSAGE_MAP(CSelectConectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectConectionDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONNECTION, OnLvnItemchangedLcConfig)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONNECTION, &CSelectConectionDlg::OnNMDblclkListConnection)
END_MESSAGE_MAP()


// CSelectConectionDlg message handlers

BOOL CSelectConectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 初始化listctrl控件
	InitCtrl();

	// 更新数据到界面
	MemToInterface();

	// 已选择的网卡高亮显示
	SetListState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}

// 初始化listctrl控件
void CSelectConectionDlg::InitCtrl()
{
	m_listConnection.SetExtendedStyle(LVS_EX_FLATSB
									 |LVS_EX_FULLROWSELECT
									 //|LVS_SINGLESEL
									);
	m_listConnection.DeleteAllItems();
	CRect rect;
	GetClientRect(&rect);
	ScreenToClient(rect);
	m_listConnection.InsertColumn(ADT_DIS, "Adapter Description",  LVCFMT_LEFT, (int)(rect.Width()*0.6), -1);
	m_listConnection.InsertColumn(ADT_IP, "IP",  LVCFMT_LEFT, (int)(rect.Width()*0.4), -1);

	// 装载图标
	m_image.DeleteImageList();
	m_image.Create(16,16, TRUE|ILC_COLOR16, 1, 1);
	m_listConnection.SetImageList(&m_image, LVSIL_SMALL); 
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_ADAPTER);
	m_image.Add(hIcon);
}

// 更新数据到界面
BOOL CSelectConectionDlg::MemToInterface()
{
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);

	// 重新探测网卡信息
	free(pDoc->m_pAdapterInfo);
	pDoc->m_pAdapterInfo = GetAllAdaptersInfo();

	// 循环网卡信息链表，插入网卡信息
	PIP_ADAPTER_INFO	pAdapter = NULL;
	pAdapter = pDoc->m_pAdapterInfo;
	while (NULL != pAdapter)
	{
		// 插入网卡描述列
		int nIndex = m_listConnection.GetItemCount();		
		LV_ITEM   lvitemAdd = {0};
		lvitemAdd.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		lvitemAdd.iItem = nIndex ;
		lvitemAdd.iSubItem = 0;
		lvitemAdd.pszText = pAdapter->Description;
		lvitemAdd.iImage = 0;	//图标的下标
		lvitemAdd.lParam = (LPARAM)pAdapter;		
		if (m_listConnection.InsertItem(&lvitemAdd) == -1)
			break;
		m_listConnection.SetItemData(nIndex, (DWORD)pAdapter);

		// 插入网卡IP列
		CString strIP;
		IP_ADDR_STRING* lpIP = &(pAdapter->IpAddressList);
		while (lpIP != NULL)
		{
			strIP = strIP + lpIP->IpAddress.String + ",";
			lpIP = lpIP->Next;
		}
		strIP.TrimRight(',');
		m_listConnection.SetItemText(nIndex, ADT_IP, strIP);

		// 指向下一个网卡
		pAdapter = pAdapter->Next;
	}

	UpdateData(FALSE);
	return TRUE;
}

//更新界面数据到内存 David 2010.6.2 add
BOOL CSelectConectionDlg::InterfaceToMem()
{
	// 取得选中行
	int nSelCount = m_listConnection.GetSelectedCount();
	if (nSelCount != 1)
		return FALSE;

	// 取得绑定的指针,设置为默认的网卡
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	pDoc->m_pDefaultAdapter = (PIP_ADAPTER_INFO)m_listConnection.GetItemData(m_listConnection.GetSelectionMark());
	ASSERT(NULL != pDoc->m_pDefaultAdapter);

	return TRUE;
}

void CSelectConectionDlg::OnBnClickedOk()
{
	// 界面更新到内存
	if (!InterfaceToMem())
	{
		MessageBox("Only one adapter can be selected!", "Error",MB_OK);
		return;
	}
	
	OnOK();
}

void CSelectConectionDlg::OnLvnItemchangedLcConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码  
	if(m_listConnection.GetCheck(pNMLV->iItem))  
		UnCheckOtherItem(m_listConnection, pNMLV->iItem);
	*pResult = 0;
}

// 选中行之外的行unchecked
void CSelectConectionDlg::UnCheckOtherItem(CListCtrl& listCtrl, int index)
{
	for (int i=0;i<listCtrl.GetItemCount();++i)
	{
		if(i == index)
			continue;
		listCtrl.SetCheck(i,FALSE);		
	}
}
void CSelectConectionDlg::OnNMDblclkListConnection(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	OnBnClickedOk();
	*pResult = 0;
}

// 已选择的网卡高亮显示
void CSelectConectionDlg::SetListState()
{
	// 1.判定是否已经选定了网卡
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	if (NULL == pDoc->m_pDefaultAdapter)
		return;

	// 2.查找界面上的数据
	for (int i=0; i<m_listConnection.GetItemCount(); ++i)
	{
		// 如果绑定的数据和已经选择的默认网卡相同就高亮显示
		if (pDoc->m_pDefaultAdapter == (PIP_ADAPTER_INFO)m_listConnection.GetItemData(i))
		{
			//m_listConnection.ModifyStyle(0, LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL);//一直选中某行且只能单行选中
			m_listConnection.SetItemState(i, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
			m_listConnection.EnsureVisible(i, FALSE);
		}
	}

	// 3.刷新界面
	m_listConnection.Invalidate();
}
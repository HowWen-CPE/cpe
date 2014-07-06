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

// list�����±�
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

	// ��ʼ��listctrl�ؼ�
	InitCtrl();

	// �������ݵ�����
	MemToInterface();

	// ��ѡ�������������ʾ
	SetListState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}

// ��ʼ��listctrl�ؼ�
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

	// װ��ͼ��
	m_image.DeleteImageList();
	m_image.Create(16,16, TRUE|ILC_COLOR16, 1, 1);
	m_listConnection.SetImageList(&m_image, LVSIL_SMALL); 
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_ADAPTER);
	m_image.Add(hIcon);
}

// �������ݵ�����
BOOL CSelectConectionDlg::MemToInterface()
{
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);

	// ����̽��������Ϣ
	free(pDoc->m_pAdapterInfo);
	pDoc->m_pAdapterInfo = GetAllAdaptersInfo();

	// ѭ��������Ϣ��������������Ϣ
	PIP_ADAPTER_INFO	pAdapter = NULL;
	pAdapter = pDoc->m_pAdapterInfo;
	while (NULL != pAdapter)
	{
		// ��������������
		int nIndex = m_listConnection.GetItemCount();		
		LV_ITEM   lvitemAdd = {0};
		lvitemAdd.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		lvitemAdd.iItem = nIndex ;
		lvitemAdd.iSubItem = 0;
		lvitemAdd.pszText = pAdapter->Description;
		lvitemAdd.iImage = 0;	//ͼ����±�
		lvitemAdd.lParam = (LPARAM)pAdapter;		
		if (m_listConnection.InsertItem(&lvitemAdd) == -1)
			break;
		m_listConnection.SetItemData(nIndex, (DWORD)pAdapter);

		// ��������IP��
		CString strIP;
		IP_ADDR_STRING* lpIP = &(pAdapter->IpAddressList);
		while (lpIP != NULL)
		{
			strIP = strIP + lpIP->IpAddress.String + ",";
			lpIP = lpIP->Next;
		}
		strIP.TrimRight(',');
		m_listConnection.SetItemText(nIndex, ADT_IP, strIP);

		// ָ����һ������
		pAdapter = pAdapter->Next;
	}

	UpdateData(FALSE);
	return TRUE;
}

//���½������ݵ��ڴ� David 2010.6.2 add
BOOL CSelectConectionDlg::InterfaceToMem()
{
	// ȡ��ѡ����
	int nSelCount = m_listConnection.GetSelectedCount();
	if (nSelCount != 1)
		return FALSE;

	// ȡ�ð󶨵�ָ��,����ΪĬ�ϵ�����
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	pDoc->m_pDefaultAdapter = (PIP_ADAPTER_INFO)m_listConnection.GetItemData(m_listConnection.GetSelectionMark());
	ASSERT(NULL != pDoc->m_pDefaultAdapter);

	return TRUE;
}

void CSelectConectionDlg::OnBnClickedOk()
{
	// ������µ��ڴ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������  
	if(m_listConnection.GetCheck(pNMLV->iItem))  
		UnCheckOtherItem(m_listConnection, pNMLV->iItem);
	*pResult = 0;
}

// ѡ����֮�����unchecked
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

// ��ѡ�������������ʾ
void CSelectConectionDlg::SetListState()
{
	// 1.�ж��Ƿ��Ѿ�ѡ��������
	CUpgraderDoc* pDoc = GetDocument();
	ASSERT(NULL != pDoc);
	if (NULL == pDoc->m_pDefaultAdapter)
		return;

	// 2.���ҽ����ϵ�����
	for (int i=0; i<m_listConnection.GetItemCount(); ++i)
	{
		// ����󶨵����ݺ��Ѿ�ѡ���Ĭ��������ͬ�͸�����ʾ
		if (pDoc->m_pDefaultAdapter == (PIP_ADAPTER_INFO)m_listConnection.GetItemData(i))
		{
			//m_listConnection.ModifyStyle(0, LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL);//һֱѡ��ĳ����ֻ�ܵ���ѡ��
			m_listConnection.SetItemState(i, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
			m_listConnection.EnsureVisible(i, FALSE);
		}
	}

	// 3.ˢ�½���
	m_listConnection.Invalidate();
}
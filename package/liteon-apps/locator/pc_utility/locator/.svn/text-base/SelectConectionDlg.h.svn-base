#pragma once
#include "afxcmn.h"


// CSelectConectionDlg dialog

class CSelectConectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectConectionDlg)

public:
	CSelectConectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectConectionDlg();

	// 更新数据到界面 David 2010.6.2 add
	BOOL MemToInterface();
	//更新界面数据到内存 David 2010.6.2 add
	BOOL InterfaceToMem();
	// 已选择的网卡高亮显示 David 2010.6.10 add
	void SetListState();

// Dialog Data
	enum { IDD = IDD_DIALOG_SEL_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// 初始化listctrl控件
	void InitCtrl();
	// 选中行之外的行unchecked
	void UnCheckOtherItem(CListCtrl& listCtrl, int index);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listConnection;
	CImageList m_image;

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedLcConfig(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkListConnection(NMHDR *pNMHDR, LRESULT *pResult);
};

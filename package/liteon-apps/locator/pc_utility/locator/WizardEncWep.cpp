// WizardEncWep.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardEncWep.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*Scoutlee begin 2004/9/2 04:58¤U¤È*/
#define APCFG_AUTH_OPEN_SYSTEM          0
#define APCFG_AUTH_SHARED_KEY           1
#define APCFG_AUTH_WPA                  2
#define APCFG_AUTH_WPAPSK               3
#define APCFG_AUTH_AUTO                 4

/*Scoutlee end 2004/9/2 04:58¤U¤È*/
/////////////////////////////////////////////////////////////////////////////
// CWizardEncWep property page

IMPLEMENT_DYNCREATE(CWizardEncWep, CNGWizardPage/*CPropertyPageEx*/)

CWizardEncWep::CWizardEncWep(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardEncWep::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardEncWep)
	m_szWzKeyHex = _T("");
	m_szWzKeyAscii = _T("");
	m_iRadioWzKeyType = 0;
	m_iWzCiperType = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WEP,	WIZ97_BITMAP_STRETCH);
}

CWizardEncWep::~CWizardEncWep()
{
}

void CWizardEncWep::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardEncWep)
	DDX_Text(pDX, IDC_EDIT_WZKEY_HEX, m_szWzKeyHex);
	DDX_Text(pDX, IDC_EDIT_WZKEY_ASCII, m_szWzKeyAscii);
	DDX_Radio(pDX, IDC_RADIO_WZKEY_HEX, m_iRadioWzKeyType);
	DDX_CBIndex(pDX, IDC_CIPHER_TYPE, m_iWzCiperType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_WZKEY_HEX, m_cWzHexCtrl);
	DDX_Control(pDX, IDC_EDIT_WZKEY_ASCII, m_cWzAsciiCtrl);
}


BEGIN_MESSAGE_MAP(CWizardEncWep, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardEncWep)
	ON_BN_CLICKED(IDC_RADIO_WZKEY_HEX, OnRadioWzkeyHex)
	ON_BN_CLICKED(IDC_RADIO_WZKEY_ASCII, OnRadioWzkeyAscii)
	ON_EN_CHANGE(IDC_EDIT_WZKEY_HEX, OnChangeEditWzkeyHex)
	ON_EN_CHANGE(IDC_EDIT_WZKEY_ASCII, OnChangeEditWzkeyAscii)
	ON_CBN_SELCHANGE(IDC_CIPHER_TYPE, OnSelchangeCipherType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardEncWep message handlers

BOOL CWizardEncWep::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	// TODO: Add extra initialization here
	/*scoutlee begin 2004/09/06*/
    CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	switch(pWizardSheet->m_pWizardLogin->m_iAuth){
							case  APCFG_AUTH_SHARED_KEY:
							      m_iWzCiperType =1;
						              break;
						        case  APCFG_AUTH_OPEN_SYSTEM:
						             m_iWzCiperType =0;
						              break;
						        case  APCFG_AUTH_AUTO:
						              m_iWzCiperType =2;
						              break;
								default:
  						              break;
	}
	switch(pWizardSheet->m_pWizardLogin->m_WepInputType){
								case  0:
								m_iRadioWzKeyType=0;
						              break;
						        case  1:
								m_iRadioWzKeyType=1;
						      
								default:
  						              break;
	}



	UpdateData(false);
	/*scoutlee end 2004/09/06*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardEncWep::OnRadioWzkeyHex() 
{
	// TODO: Add your control notification handler code here
 	CString m_str;

	UpdateData(true);

	switch(m_iPrepageWepMode)
	{
	case WEP64:
		m_str.LoadString(IDS_TEXT_KEY_HEX64);
		break;
	case WEP128:
		m_str.LoadString(IDS_TEXT_KEY_HEX128);
		break;
	case WEP152:
		m_str.LoadString(IDS_TEXT_KEY_HEX152);
		break;
	}
	GetDlgItem(IDC_STATIC_INPUT)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_INPUT)->SetWindowText(m_str);
	GetDlgItem(IDC_STATIC_INPUT)->ShowWindow(true);

	GetDlgItem(IDC_EDIT_WZKEY_ASCII)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_WZKEY_HEX)->ShowWindow(true);

	UpdateData(false);

	OnChangeEditWzkeyHex();
}

void CWizardEncWep::OnRadioWzkeyAscii() 
{
	// TODO: Add your control notification handler code here
 	CString m_str;

	UpdateData(true);

	switch(m_iPrepageWepMode)
	{
	case WEP64:
		m_str.LoadString(IDS_TEXT_KEY_STRING64);
		break;
	case WEP128:
		m_str.LoadString(IDS_TEXT_KEY_STRING128);
		break;
	case WEP152:
		m_str.LoadString(IDS_TEXT_KEY_STRING152);
		break;
	}
	GetDlgItem(IDC_STATIC_INPUT)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_INPUT)->SetWindowText(m_str);
	GetDlgItem(IDC_STATIC_INPUT)->ShowWindow(true);

	GetDlgItem(IDC_EDIT_WZKEY_HEX)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_WZKEY_ASCII)->ShowWindow(true);

	UpdateData(false);

	OnChangeEditWzkeyAscii();
}

BOOL CWizardEncWep::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	int len;

	if ((pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_REPEATER) ||
		(pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_ADHOC))
	{
		m_iWzCiperType = AUTH_OPEN_SYSTEM;
		UpdateData(false);
		GetDlgItem(IDC_CIPHER_TYPE)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_AUTH_WEP)->ShowWindow(false);
		m_iPrepageWepMode = pWizardSheet->m_pWizardSecurityRepeater->m_iWzAuthRepeater;
//		SetDlgItemPosition(IDC_STATIC_INPUT_TYPE, -10, 0);
	}
	else
	{
		GetDlgItem(IDC_CIPHER_TYPE)->ShowWindow(true);
		/*scout 2004/08/23 if APmode delet Auto  selection */
       	if (pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_AP){
		    ((CComboBox*)GetDlgItem(IDC_CIPHER_TYPE))->DeleteString( 2 ); 
		 }else{
	    	int nIndex = 0;
			/*scout if user click back button and change to client 
			  detemine whether the auto is exist ,is not and add to combo list
			*/
            if ((nIndex=((CComboBox*)GetDlgItem(IDC_CIPHER_TYPE))->FindString(nIndex, "Auto")) == CB_ERR)
			{
               ((CComboBox*)GetDlgItem(IDC_CIPHER_TYPE))->AddString("Auto");

			}

		 }
		GetDlgItem(IDC_STATIC_AUTH_WEP)->ShowWindow(true);
		m_iPrepageWepMode = pWizardSheet->m_pWizardSecurity->m_iWzAuth;
	}

	switch(m_iPrepageWepMode)
	{
	case WEP64:
		len = 5;
		break;
	case WEP128:
		len = 13;
		break;
	case WEP152:
		len = 16;
		break;
	}
	((CEdit*)GetDlgItem(IDC_EDIT_WZKEY_HEX))->SetLimitText(len*2);
	((CEdit*)GetDlgItem(IDC_EDIT_WZKEY_ASCII))->SetLimitText(len);

	if(m_iRadioWzKeyType == 0)
	{
		OnRadioWzkeyHex();
		if (m_szWzKeyHex.GetLength() == len*2)
			pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
		else if (m_szWzKeyHex.GetLength() > len*2)
		{
			m_szWzKeyHex = m_szWzKeyHex.Left(len*2);
			pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
		}
		else
			pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	}
	else
	{
		OnRadioWzkeyAscii();
		if (m_szWzKeyAscii.GetLength() == len)
			pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
		else if (m_szWzKeyAscii.GetLength() > len)
		{
			m_szWzKeyAscii = m_szWzKeyAscii.Left(len);
			pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
		}
		else
			pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	}

	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

LRESULT CWizardEncWep::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	if ((pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_REPEATER) ||//Reapter
		(pWizardSheet->m_pWizardSystem->m_iWzOpModeReal == CFG_OP_MODE_ADHOC))
		pWizardSheet->SetActivePage(PAGE_SEC_REPEATER);
	else
		pWizardSheet->SetActivePage(PAGE_SEC);

	return -1;	
//	return CPropertyPageEx::OnWizardBack();
}

BOOL CWizardEncWep::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, true);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

void CWizardEncWep::OnChangeEditWzkeyHex() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	int len;
	
	UpdateData(true);

	switch(m_iPrepageWepMode)
	{
	case WEP64:
		len = 10;
		break;
	case WEP128:
		len = 26;
		break;
	case WEP152:
		len = 32;
		break;
	}
	if (m_szWzKeyHex.GetLength() == len)
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);

	sprintf(m_cWzKeyHex, m_szWzKeyHex);
}

void CWizardEncWep::OnChangeEditWzkeyAscii() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	int len;
	
	UpdateData(true);

	switch(m_iPrepageWepMode)
	{
	case WEP64:
		len = 5;
		break;
	case WEP128:
		len = 13;
		break;
	case WEP152:
		len = 16;
		break;
	}
	if (m_szWzKeyAscii.GetLength() == len)
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);	

	sprintf(m_cWzKeyAscii, m_szWzKeyAscii);
}

void CWizardEncWep::OnSelchangeCipherType() 
{
	// TODO: Add your control notification handler code here
    /*scout 2004/08/23 if select auto change m_iWzCiperType=4
	  because AUTH_AUTO=4
	*/
	UpdateData(true);	
	if(m_iWzCiperType==2)
		m_iWzCiperTypeReal = AUTH_AUTO;
	else
		m_iWzCiperTypeReal = m_iWzCiperType;
	
}

void CWizardEncWep::SetDlgItemPosition(int nID, int TopShift, int LeftShift)
{
//	CRect rectFrame;
//	CSize shift;
//	int Hight,Length;
//	GetDlgItem(nID)->GetClientRect(rectFrame);

//	Hight = rectFrame.bottom - rectFrame.top;
//	Length = rectFrame.right - rectFrame.left;
//	shift.cx = rectFrame.left - LeftShift;
//	shift.cy = rectFrame.top - TopShift;

//	rectFrame -= shift;

//	rectFrame.top = rectFrame.top + TopShift;
//	rectFrame.bottom = rectFrame.top + Hight+2;
//	rectFrame.left = rectFrame.left + LeftShift;
//	rectFrame.right = rectFrame.left + Length;//+10;
//	ScreenToClient(rectFrame);
//	GetDlgItem(nID)->MoveWindow(rectFrame);
}

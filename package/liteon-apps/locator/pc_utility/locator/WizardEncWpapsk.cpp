// WizardEncWpapsk.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardEncWpapsk.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardEncWpapsk property page

IMPLEMENT_DYNCREATE(CWizardEncWpapsk, CNGWizardPage/*CPropertyPageEx*/)

CWizardEncWpapsk::CWizardEncWpapsk(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardEncWpapsk::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardEncWpapsk)
	m_szWzKeyWpapsk = _T("");
	m_iWzEncryption = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WPA_PSK,	WIZ97_BITMAP_STRETCH);
}

CWizardEncWpapsk::~CWizardEncWpapsk()
{
}

void CWizardEncWpapsk::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardEncWpapsk)
	DDX_Text(pDX, IDC_EDIT_WZKEY_WPAPSK, m_szWzKeyWpapsk);
	DDX_CBIndex(pDX, IDC_COMBO_WZENCRYPTION, m_iWzEncryption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardEncWpapsk, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardEncWpapsk)
	ON_CBN_SELCHANGE(IDC_COMBO_WZENCRYPTION, OnSelchangeComboWzencryption)
	ON_EN_CHANGE(IDC_EDIT_WZKEY_WPAPSK, OnChangeEditWzkeyWpapsk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardEncWpapsk message handlers

BOOL CWizardEncWpapsk::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CEdit*)GetDlgItem(IDC_EDIT_WZKEY_WPAPSK))->SetLimitText(63);
 /*scoutlee begin 2004/09/06*/
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	switch(pWizardSheet->m_pWizardLogin->m_iEncryption)
	{
	case ENCRYPTION_TKIP:
		m_iWzEncryption = TKIP;
		break;
	case ENCRYPTION_AES_CCM:
		m_iWzEncryption = AES;
		break;
	case ENCRYPTION_DISABLED:
	case ENCRYPTION_WEP:
	case ENCRYPTION_AES_OCB:
	case ENCRYPTION_CKIP:
	case ENCRYPTION_AUTO:
		m_iWzEncryption = TKIP;
		break;
	}
		UpdateData(false);

	/*scoutlee end 2004/09/06*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizardEncWpapsk::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWizardSheet* pWizardSheet = (CWizardSheet*)m_pParentSheet;
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SEC);
	
	return -1;
//	return CPropertyPageEx::OnWizardBack();
}

BOOL CWizardEncWpapsk::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnChangeEditWzkeyWpapsk();
	
	return CPropertyPageEx::OnSetActive();
}

BOOL CWizardEncWpapsk::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, true);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

void CWizardEncWpapsk::OnSelchangeComboWzencryption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
}

void CWizardEncWpapsk::OnChangeEditWzkeyWpapsk() 
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

	len = m_szWzKeyWpapsk.GetLength();

	if ( (len < 8) || (len > 63))
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);

	sprintf( m_cWzKeyWpapsk, m_szWzKeyWpapsk);
}

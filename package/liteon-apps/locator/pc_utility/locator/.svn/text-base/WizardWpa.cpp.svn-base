// WizardWpa.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardWpa.h"
//#include "WizardSheet.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardWpa property page

IMPLEMENT_DYNCREATE(CWizardWpa, CNGWizardPage/*CPropertyPageEx*/)

CWizardWpa::CWizardWpa(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardWpa::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardWpa)
	m_szWzKeyWpa = _T("");
	m_iWzEncryptionWpa = 0;
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WPA,	WIZ97_BITMAP_STRETCH);
}

CWizardWpa::~CWizardWpa()
{
}

void CWizardWpa::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage/*CPropertyPageEx*/::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardWpa)
	DDX_Control(pDX, IDC_WZ_ADIUS_SERVER, m_ctrlRadiusServer);
	DDX_Text(pDX, IDC_EDIT_WZKEY_WPA, m_szWzKeyWpa);
	DDV_MaxChars(pDX, m_szWzKeyWpa, 64);
	DDX_CBIndex(pDX, IDC_WZ_WPA_CIPHER, m_iWzEncryptionWpa);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardWpa, CNGWizardPage/*CPropertyPageEx*/)
	//{{AFX_MSG_MAP(CWizardWpa)
	ON_CBN_SELENDCANCEL(IDC_WZ_WPA_CIPHER, OnSelendcancelWzWpaCipher)
	ON_EN_CHANGE(IDC_EDIT_WZKEY_WPA, OnChangeEditWzkeyWpa)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_WZ_ADIUS_SERVER, OnFieldchangedWzAdiusServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardWpa message handlers

BOOL CWizardWpa::OnInitDialog() 
{
	CNGWizardPage/*CPropertyPageEx*/::OnInitDialog();
	
	// TODO: Add extra initialization here
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	CString strRadiusServer;

	strRadiusServer = pWizardSheet->m_pWizardLogin->m_cRadiusServer;
	
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

	switch(pWizardSheet->m_pWizardLogin->m_iEncryption)
	{
	case ENCRYPTION_TKIP:
		m_iWzEncryptionWpa = TKIP;
		break;
	case ENCRYPTION_AES_CCM:
		m_iWzEncryptionWpa = AES;
		break;
	case ENCRYPTION_DISABLED:
	case ENCRYPTION_WEP:
	case ENCRYPTION_AES_OCB:
	case ENCRYPTION_CKIP:
	case ENCRYPTION_AUTO:
		m_iWzEncryptionWpa = TKIP;
		break;
	}

	UpdateData(false);
	
	((CEdit*)GetDlgItem(IDC_EDIT_WZKEY_WPA))->SetLimitText(64);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWizardWpa::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	if ( m_szWzKeyWpa.IsEmpty() || m_ctrlRadiusServer.IsBlank() )
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
	
	return CNGWizardPage/*CPropertyPageEx*/::OnSetActive();
}

void CWizardWpa::OnSelendcancelWzWpaCipher() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CWizardWpa::OnChangeEditWzkeyWpa() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CNGWizardPage/*CPropertyPageEx*/::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	if ( m_szWzKeyWpa.IsEmpty() || m_ctrlRadiusServer.IsBlank() )
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);	
}

void CWizardWpa::OnFieldchangedWzAdiusServer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	UpdateData(true);

	m_ctrlRadiusServer.GetAddress(  (unsigned char &)m_cRadiusServer[0],
									(unsigned char &)m_cRadiusServer[1],
									(unsigned char &)m_cRadiusServer[2],
									(unsigned char &)m_cRadiusServer[3]);

	if ( m_szWzKeyWpa.IsEmpty() || m_ctrlRadiusServer.IsBlank() )
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		pWizardSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH|PSWIZB_DISABLEDFINISH);
	
	*pResult = 0;
}

LRESULT CWizardWpa::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;

	pWizardSheet->SetActivePage(PAGE_SEC);
	
	return -1;	
//	return CNGWizardPage/*CPropertyPageEx*/::OnWizardBack();
}

BOOL CWizardWpa::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizardSummary*	m_pWizardSummary;

	m_pWizardSummary = new CWizardSummary(m_pGrandFather, m_pParentSheet, true);
	if ( m_pWizardSummary->DoModal() == IDOK )
		return CNGWizardPage/*CPropertyPageEx*/::OnWizardFinish();
	else
		return 0;
}

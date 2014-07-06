// WizardEncWpa.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "WizardEncWpa.h"
#include "NGWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardEncWpa property page

IMPLEMENT_DYNCREATE(CWizardEncWpa, CNGWizardPage)

CWizardEncWpa::CWizardEncWpa(CWnd* pGrandFather, CWnd* pParent) : CNGWizardPage(CWizardEncWpa::IDD, 0, NULL, NULL)
{
	//{{AFX_DATA_INIT(CWizardEncWpa)
	m_iWzEncryptionWpa = 0;
	m_szWzKeyWpa = _T("");
	//}}AFX_DATA_INIT
	m_pGrandFather	= pGrandFather;
	m_pParentSheet	= pParent;

	SetWatermark(IDB_BG_WEP,	WIZ97_BITMAP_STRETCH);
}

CWizardEncWpa::~CWizardEncWpa()
{
}

void CWizardEncWpa::DoDataExchange(CDataExchange* pDX)
{
	CNGWizardPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardEncWpa)
	DDX_Control(pDX, IDC_WZ_ADIUS_SERVER, m_ctrlRadiusServer);
	DDX_CBIndex(pDX, IDC_WZ_WPA_CIPHER, m_iWzEncryptionWpa);
	DDX_Text(pDX, IDC_EDIT_WZKEY_WPA, m_szWzKeyWpa);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardEncWpa, CNGWizardPage)
	//{{AFX_MSG_MAP(CWizardEncWpa)
	ON_CBN_SELCHANGE(IDC_WZ_WPA_CIPHER, OnSelchangeWzWpaCipher)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardEncWpa message handlers

BOOL CWizardEncWpa::OnInitDialog() 
{
	CNGWizardPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;
	CString strRadiusServer;
	char m_cRadiusServer[4];

//	memcpy(strRadiusServer, pWizardSheet->m_pWizardLogin->m_cRadiusServer, sizeof(strRadiusServer));
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

void CWizardEncWpa::OnSelchangeWzWpaCipher() 
{
	// TODO: Add your control notification handler code here
	
}
/*
BOOL CWizardEncWep::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CNGWizard* pWizardSheet = (CNGWizard*)m_pParentSheet;


	return CNGWizardPage::OnSetActive();
}
*/

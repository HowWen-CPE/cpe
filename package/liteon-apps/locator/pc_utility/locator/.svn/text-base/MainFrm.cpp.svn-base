// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "upgrader.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT ToolIndicators[] =
{
	ID_SEARCH,           // status line indicator
//	ID_WIZARD,
//	ID_CONFIG,
//	ID_WIRELESS,
	ID_UPGRADE_MANUAL,
//	ID_SEPARATOR,
	ID_DEVICE_INFO,
//	ID_SEPARATOR,
	ID_WEB,
	ID_REBOOT
};

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	CImageList img;
	CString str;

	if (!m_wndReBar.Create(this))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// set up toolbar properties
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(50, 100);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	img.Create(IDB_TOOLBAR, 24, 0, RGB(0, 255, 0));
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();
	img.Create(IDB_TOOLBAR, 24, 0, RGB(0, 255, 0));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
	
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndToolBar.SetButtons(ToolIndicators, sizeof(ToolIndicators)/sizeof(UINT));

	// set up each toolbar button
//	m_wndToolBar.SetButtonInfo(0, ID_SEARCH, TBSTYLE_BUTTON, 0);
	str.LoadString(IDS_SEARCH);
	m_wndToolBar.SetButtonText(0, str);
//	m_wndToolBar.SetButtonInfo(1, ID_UPGRADE_AUTO, TBSTYLE_BUTTON, 1);
	//str.LoadString(ID_WIZARD);
	//m_wndToolBar.SetButtonText(1, str);

	//str.LoadString(ID_CONFIG);
	//m_wndToolBar.SetButtonText(1, str);

	//str.LoadString(ID_WIRELESS);	
	//m_wndToolBar.SetButtonText(3, str);
//	m_wndToolBar.SetButtonInfo(3, ID_UPGRADE_MANUAL, TBSTYLE_BUTTON, 3);

	str.LoadString(IDS_UPGRADE_MANUAL);
	m_wndToolBar.SetButtonText(1, str);

//	m_wndToolBar.SetButtonInfo(4, ID_SEPARATOR, TBSTYLE_SEP, 4);
//	m_wndToolBar.SetButtonInfo(5, ID_DEVICE_INFO, TBSTYLE_BUTTON, 5);

	str.LoadString(IDS_DEVICE_INFO);
	m_wndToolBar.SetButtonText(2, str);

	str.LoadString(IDS_CONN_WEB);
	m_wndToolBar.SetButtonText(3, str);
	
	str.LoadString(IDS_REBOOT);
	m_wndToolBar.SetButtonText(4, str);
	
	CRect rectToolBar;

	// set up toolbar button sizes
	m_wndToolBar.GetItemRect(0, &rectToolBar);
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(24,20));

	m_wndReBar.AddBar(&m_wndToolBar);

	// set up min/max sizes and ideal sizes for pieces of the rebar
	REBARBANDINFO rbbi;

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 6;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);
	rbbi.cxMinChild = 0;

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.x = (GetSystemMetrics(SM_CXSCREEN) - 640) / 2;
	cs.y = (GetSystemMetrics(SM_CYSCREEN) - 320) / 2;
	cs.cx = 640;
	cs.cy = 320; 
	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CFrameWnd::OnEraseBkgnd(pDC);
}

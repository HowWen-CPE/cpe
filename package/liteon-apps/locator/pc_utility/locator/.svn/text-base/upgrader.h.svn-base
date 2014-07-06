// upgrader.h : main header file for the UPGRADER application
//

#if !defined(AFX_UPGRADER_H__678EEB86_5132_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_UPGRADER_H__678EEB86_5132_11D2_BC1F_0080C868BB53__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "snmpapi.h"
#include "CStruct.h"
/////////////////////////////////////////////////////////////////////////////
// CUpgraderApp:
// See upgrader.cpp for the implementation of this class
//
class CUpgraderDoc;
class CUpgraderApp : public CWinApp
{
public:
	CUpgraderApp();

public:
	BOOL Init_Session();
	LPSESSION   m_lpSession;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgraderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUpgraderApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CUpgraderApp theApp;

// 取得文档指针的全局函数
CUpgraderDoc* GetDocument();
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADER_H__678EEB86_5132_11D2_BC1F_0080C868BB53__INCLUDED_)

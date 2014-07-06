// EditLimit.cpp : implementation file
//

#include "stdafx.h"
#include "upgrader.h"
#include "EditLimit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditLimit

CEditLimit::CEditLimit()
{
}

CEditLimit::~CEditLimit()
{
}


BEGIN_MESSAGE_MAP(CEditLimit, CEdit)
	//{{AFX_MSG_MAP(CEditLimit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLimit message handlers

void CEditLimit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
    UCHAR ucBackSpace = 0x8;
    if( (nChar >= 'a' && nChar <= 'f') || (nChar >= '0' && nChar <= '9') ||
        (nChar >= 'A' && nChar <= 'F') || nChar == ucBackSpace){
        ;
    } else {
        return;
    }
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

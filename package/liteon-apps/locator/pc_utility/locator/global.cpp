#include "stdafx.h"
#include "Upgrader.h"
#include "global.h"

HCURSOR BeginWait()
{
	HCURSOR hOldCur;
	hOldCur = ::SetCursor( AfxGetApp()->LoadStandardCursor( IDC_WAIT ));
	return (hOldCur);
}

void EndWait(HCURSOR hOldCur)
{
	::SetCursor( hOldCur );
}

void ErrorMsg(UINT uMsgID, LPCTSTR lpszArg)
{
	CString strMessage;
	TCHAR   szMessage[_MAX_PATH];

	strMessage.LoadString(uMsgID);
	wsprintf(szMessage, strMessage, lpszArg);
	AfxMessageBox(szMessage, MB_OK|MB_ICONSTOP);
}

void GetCurrentPath(LPTSTR Path)
{
	GetModuleFileName(AfxGetInstanceHandle(), Path, _MAX_PATH);
    *(strrchr(Path, '\\' ) + 1) = 0;
}
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__678EEB88_5132_11D2_BC1F_0080C868BB53__INCLUDED_)
#define AFX_STDAFX_H__678EEB88_5132_11D2_BC1F_0080C868BB53__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxtempl.h>
#include <winsock.h>
#include <vector>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#pragma warning(disable: 4996 4018)
#include <Iphlpapi.h>
#include "global.h"
// É¾³ýÖ¸ÕëµÄºê David 2010.6.2 add
#define DELETEP(p) if (NULL != p){delete p; p=NULL;}
#define WM_CHANGE_SOCKET WM_USER+100

void HexToint(char *pBuffer, int len, int& iIndex, int* pOut);
void HexToULONG(char *pBuffer, int Len, int& iIndex, ULONG *pOut);
void HexToChar(char *pBuffer, int Len, int& iIndex, char *pOut);
void Ip2String(char *outputStr, char *inputIp);
BOOL IsHexDigit(char cChar);
BOOL GetLocalIPs(char *localips, int len);
BOOL GetMacName(char* localips, int len);
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__678EEB88_5132_11D2_BC1F_0080C868BB53__INCLUDED_)

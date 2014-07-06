// GradientProgCtrl.h: interface for the CGradientProgCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADIENTPROGCTRL_H__78B5CFCB_83B0_4000_8B66_C408227180ED__INCLUDED_)
#define AFX_GRADIENTPROGCTRL_H__78B5CFCB_83B0_4000_8B66_C408227180ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemDC.h"


class CGradientProgCtrl : public CProgressCtrl  
{
public:
	CGradientProgCtrl();

// Attributes
public:
// Attributes
	
	void SetRange(int nLower, int nUpper);
	int SetPos(int nPos);
	int SetStep(int nStep);
	int StepIt(void);

// Operations
public:
	
	// Set Functions
	void SetTextColor(COLORREF color)	{m_clrText = color;}
	void SetBkColor(COLORREF color)		 {m_clrBkGround = color;}
	void SetStartColor(COLORREF color)	{m_clrStart = color;}
	void SetEndColor(COLORREF color)	{m_clrEnd = color;}

	// Show the percent caption
	void ShowPercent(BOOL bShowPercent = TRUE)	{m_bShowPercent = bShowPercent;}
	
	// Get Functions
	COLORREF GetTextColor(void)	{return m_clrText;}
	COLORREF GetBkColor(void)		 {return m_clrBkGround;}
	COLORREF GetStartColor(void)	{return m_clrStart;}
	COLORREF GetEndColor(void)	{return m_clrEnd;}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGradientProgCtrl();

	// Generated message map functions
protected:
	void DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth);	
	int m_nLower, m_nUpper, m_nStep, m_nCurrentPosition;
	COLORREF	m_clrStart, m_clrEnd, m_clrBkGround, m_clrText;
	BOOL m_bShowPercent;
	//{{AFX_MSG(CGradientProgressCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRADIENTPROGCTRL_H__78B5CFCB_83B0_4000_8B66_C408227180ED__INCLUDED_)

// PC_Simple.h : main header file for the PC_SIMPLE application
//

#if !defined(AFX_PC_SIMPLE_H__8F27B7AE_EC97_4270_876A_2FA4FF750325__INCLUDED_)
#define AFX_PC_SIMPLE_H__8F27B7AE_EC97_4270_876A_2FA4FF750325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPC_SimpleApp:
// See PC_Simple.cpp for the implementation of this class
//

class CPC_SimpleApp : public CWinApp
{
public:
	CPC_SimpleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPC_SimpleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPC_SimpleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PC_SIMPLE_H__8F27B7AE_EC97_4270_876A_2FA4FF750325__INCLUDED_)

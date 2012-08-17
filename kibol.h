// kibol.h : main header file for the KIBOL application
//

#if !defined(AFX_KIBOL_H__101B2104_C7DF_4911_AE03_7498F46A93A0__INCLUDED_)
#define AFX_KIBOL_H__101B2104_C7DF_4911_AE03_7498F46A93A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKibolApp:
// See kibol.cpp for the implementation of this class
//

class CKibolApp : public CWinApp
{
public:
	CKibolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKibolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKibolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KIBOL_H__101B2104_C7DF_4911_AE03_7498F46A93A0__INCLUDED_)

// kibol.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "kibol.h"
#include "kibolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKibolApp

BEGIN_MESSAGE_MAP(CKibolApp, CWinApp)
	//{{AFX_MSG_MAP(CKibolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKibolApp construction

CKibolApp::CKibolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKibolApp object

CKibolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKibolApp initialization

BOOL CKibolApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CKibolDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//DEL BOOL CKibolApp::UstawDane()
//DEL {
//DEL 	/*
//DEL 	int x, y, i;
//DEL  
//DEL   pozycja_x := n div 2;
//DEL   pozycja_y := m div 2;
//DEL 
//DEL 
//DEL 
//DEL   for xx:=0 to n do
//DEL    for yy:=0 to m do
//DEL    begin
//DEL     wierzcholek[xx,yy].deg:=0;
//DEL     wierzcholek[1,yy].deg:=5;
//DEL     wierzcholek[n-1,yy].deg:=5;
//DEL     wierzcholek[xx,1].deg:=5;
//DEL     wierzcholek[xx,m-1].deg:=5;
//DEL     wierzcholek[0,yy].deg:=8;
//DEL     wierzcholek[n,yy].deg:=8;
//DEL     wierzcholek[xx,0].deg:=8;
//DEL     wierzcholek[xx,m].deg:=8;
//DEL 
//DEL     i:=0;
//DEL     repeat
//DEL      wierzcholek[xx,m-i].waga:=i;
//DEL      i:=i+1;
//DEL     until i>m;
//DEL    end;
//DEL    wierzcholek[(n div 2)-1,1].deg:=3;
//DEL    wierzcholek[(n div 2)-1,m-1].deg:=3;
//DEL    wierzcholek[(n div 2)+1,1].deg:=3;
//DEL    wierzcholek[(n div 2)+1,m-1].deg:=3;
//DEL    wierzcholek[(n div 2),0].deg:=0;
//DEL    wierzcholek[(n div 2),1].deg:=0;
//DEL    wierzcholek[(n div 2),m-1].deg:=0;
//DEL    wierzcholek[(n div 2),m].deg:=0;
//DEL    wierzcholek[1,1].deg:=7;
//DEL    wierzcholek[1,m-1].deg:=7;
//DEL    wierzcholek[n-1,1].deg:=7;
//DEL    wierzcholek[n-1,m-1].deg:=7;
//DEL    wierzcholek[n-1,m-1].deg:=7;
//DEL    wierzcholek[1,0].deg:=8;
//DEL    wierzcholek[1,m].deg:=8;
//DEL    wierzcholek[n,1].deg:=8;
//DEL    wierzcholek[n,m-1].deg:=8;
//DEL 
//DEL    wierzcholek[1,m-2].waga:=3;
//DEL    wierzcholek[n-1,m-2].waga:=3;
//DEL    */
//DEL 	return(true);
//DEL }

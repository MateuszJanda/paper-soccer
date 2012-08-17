// kibolDlg.cpp : implementation file
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
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKibolDlg dialog

CKibolDlg::CKibolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKibolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKibolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32


//## pamietac o zmianie trybu na 0, teraz testowany jest tryb H vs H
	tryb_gry = 0;
	koryguj = 2;

	kolor = RGB(255, 0, 0);
	bk_kolor = RGB(192, 192, 192);
	przesun.x = 30;
	przesun.y = 60;
	//offset.x = 3;
	//offset.y = 41;
	offset.x = 0;
	offset.y = 0;
	rozm = 30;
	win_akt.x = 3*rozm + przesun.x + offset.x;
	win_akt.y = 4*rozm + przesun.y + offset.y;

	marker_check = 1;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKibolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKibolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKibolDlg, CDialog)
	//{{AFX_MSG_MAP(CKibolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_GRA_MARKERY, OnGraMarkery)
	ON_COMMAND(ID_TRYB_HUMVSHUM, OnTrybHumvshum)
	ON_COMMAND(ID_TRYB_HUMVSKOMP, OnTrybHumvskomp)
	ON_COMMAND(ID_TRYB_KOMPVSHUM, OnTrybKompvshum)
	ON_COMMAND(ID_GRA_KONIEC, OnGraKoniec)
	ON_COMMAND(ID_GRA_NOWA, OnGraNowa)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKibolDlg message handlers

BOOL CKibolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKibolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKibolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	Zamazuj();
	WinRysujBoisko();
	Marker();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKibolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKibolDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt(point);
	CString zwrot;
	
	
	switch(tryb_gry) {
	case 0:
		zwrot = MessageBox("Nie wybrano zadnego trybu!","Kibol",MB_OK | MB_ICONEXCLAMATION);
		break;
	case 1:
		TrybHvsH(pt.x + offset.x, pt.y + offset.y);
		break;
	case 2:
		TrybKvsH(pt.x + offset.x, pt.y + offset.y);
		break;
	case 3:
		TrybKvsH(pt.x + offset.x, pt.y + offset.y);
		break;
// Tryb sieciowy
//	case 4:		
	}
	
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CKibolDlg::WinRysuj(int x, int y)
{
/*
	CWindowDC pdc(this);
	
	pdc.MoveTo(win_akt.x, win_akt.y);
	pdc.LineTo(x, y);
	win_akt.x = x;
	win_akt.y = y;
*/
	CClientDC pdc(this);
	//CPen lSolidPen(PS_SOLID, 1, RGB(255,0,0) );
	CPen lSolidPen(PS_SOLID, 1, kolor );


	//pobranie obszaru rysowania
	CRect lObszar;
	GetClientRect(lObszar);
	//lObszar.NormalizeRect();

	CPen *lOldPen;

	lOldPen = pdc.SelectObject(&lSolidPen);

	//rysowanie linii
	pdc.MoveTo(win_akt.x, win_akt.y);
	pdc.LineTo(x, y);
	win_akt.x = x;
	win_akt.y = y;

	//wybranie oryginalnego piora
	pdc.SelectObject(lOldPen);

}

void CKibolDlg::WinRysujBoisko()
{
	//CWindowDC pdc(this);
	CClientDC pdc(this);
	
	pdc.MoveTo( przesun.x + offset.x, przesun.y + offset.y );
	pdc.LineTo( przesun.x + offset.x, przesun.y +(boisko.height - 2)*rozm + offset.y );
	pdc.LineTo( przesun.x + 2*rozm + offset.x, przesun.y + (boisko.height -2)*rozm + offset.y );
	pdc.LineTo( przesun.x + 2*rozm + offset.x, przesun.y + (boisko.height -2 +1)*rozm + offset.y );
	pdc.LineTo( przesun.x + (2+2)*rozm + offset.x, przesun.y + (boisko.height -2 +1)*rozm + offset.y );
	pdc.LineTo( przesun.x + (2+2)*rozm + offset.x, przesun.y + (boisko.height -2)*rozm + offset.y );
	pdc.LineTo( przesun.x + (2+2+2)*rozm + offset.x, przesun.y + (boisko.height -2)*rozm + offset.y );
	pdc.LineTo( przesun.x + (2+2+2)*rozm + offset.x, przesun.y + offset.y );
	pdc.LineTo( przesun.x + (2+2)*rozm + offset.x, przesun.y + offset.y );
	pdc.LineTo( przesun.x + (2+2)*rozm + offset.x, przesun.y + (-1)*rozm + offset.y );
	pdc.LineTo( przesun.x + 2*rozm + offset.x, przesun.y + (-1)*rozm + offset.y );
	pdc.LineTo( przesun.x + 2*rozm + offset.x, przesun.y + offset.y );
	pdc.LineTo( przesun.x + offset.x, przesun.y + offset.y );
}

int CKibolDlg::TrybHvsH(int win_x, int win_y)
{
	CPunkt win_pos;
	CPunkt pkt;
	CString zwrot;
	int stan;

	win_pos.x = win_x;
	win_pos.y = win_y;

	
//## TEST
/*
	stan = KonwersjaWin(&win_pos, &pkt);

	if(stan != 0) 
	{
		WinRysuj(win_pos.x, win_pos.y);
		win_akt.x = win_pos.x;
		win_akt.y = win_pos.y;
	}
*/
//## KONIEC TESTU

	if( KonwersjaWin(&win_pos, &pkt) )
	{
		stan = boisko.RuchHum(pkt);
		if(stan != 3) 
		{
			WinRysuj(win_pos.x, win_pos.y);
			win_akt = win_pos;
		}

		switch(stan){
		case 0:
			zwrot = MessageBox("KONIEC GRY!","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
			tryb_gry = 0;
			break;
		case 1:
			ZmienKolor();
			zwrot = MessageBox("Koniec ruchow zawodnika.","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
			break;
//		case 2:
//			zwrot = MessageBox("Grasz dalej.","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
//			break;
		}	
	}

	return(0);
}


/* 0 =	konwersja nie udala sie
 * 1 =	konwersja udala sie
 */
int CKibolDlg::KonwersjaWin(CPunkt *win_pos, CPunkt *pkt)
{
	int marg = 6;

	// lewy dolny
	if( ((*win_pos).x > (win_akt.x - rozm - marg)) &&
		((*win_pos).x < (win_akt.x - rozm + marg)) &&
		((*win_pos).y > (win_akt.y + rozm - marg)) &&
		((*win_pos).y < (win_akt.y + rozm + marg)) )
	{
		(*win_pos).x = win_akt.x - rozm;
		(*win_pos).y = win_akt.y + rozm;
		(*pkt).x = boisko.akt.x -1;
		(*pkt).y = boisko.akt.y -1;
		return(1);
	}

	// lewy
	if( ((*win_pos).x > (win_akt.x - rozm - marg)) &&
		((*win_pos).x < (win_akt.x - rozm + marg)) &&
		((*win_pos).y > (win_akt.y - marg)) &&
		((*win_pos).y < (win_akt.y + marg)) )
	{
		(*win_pos).x = win_akt.x - rozm;
		(*win_pos).y = win_akt.y;
		(*pkt).x = boisko.akt.x -1;
		(*pkt).y = boisko.akt.y;
		return(1);
	}

	// lewy gorny
	if( ((*win_pos).x > (win_akt.x - rozm - marg)) &&
		((*win_pos).x < (win_akt.x - rozm + marg)) &&
		((*win_pos).y > (win_akt.y - rozm - marg)) &&
		((*win_pos).y < (win_akt.y - rozm + marg)) )
	{
		(*win_pos).x = win_akt.x - rozm;
		(*win_pos).y = win_akt.y - rozm;
		(*pkt).x = boisko.akt.x -1;
		(*pkt).y = boisko.akt.y +1;
		return(1);
	}

	// gorny
	if( ((*win_pos).x > (win_akt.x - marg)) &&
		((*win_pos).x < (win_akt.x + marg)) &&
		((*win_pos).y > (win_akt.y - rozm - marg)) &&
		((*win_pos).y < (win_akt.y - rozm + marg)) )
	{
		(*win_pos).x = win_akt.x;
		(*win_pos).y = win_akt.y - rozm;
		(*pkt).x = boisko.akt.x;
		(*pkt).y = boisko.akt.y +1;
		return(1);
	}

	// prawy gorny
	if( ((*win_pos).x > (win_akt.x + rozm - marg)) &&
		((*win_pos).x < (win_akt.x + rozm + marg)) &&
		((*win_pos).y > (win_akt.y - rozm - marg)) &&
		((*win_pos).y < (win_akt.y - rozm + marg)) )
	{
		(*win_pos).x = win_akt.x + rozm;
		(*win_pos).y = win_akt.y - rozm;
		(*pkt).x = boisko.akt.x +1;
		(*pkt).y = boisko.akt.y +1;
		return(1);
	}

	// prawy
	if( ((*win_pos).x > (win_akt.x + rozm - marg)) &&
		((*win_pos).x < (win_akt.x + rozm + marg)) &&
		((*win_pos).y > (win_akt.y - marg)) &&
		((*win_pos).y < (win_akt.y + marg)) )
	{
		(*win_pos).x = win_akt.x + rozm;
		(*win_pos).y = win_akt.y;
		(*pkt).x = boisko.akt.x +1;
		(*pkt).y = boisko.akt.y;
		return(1);
	}

	// prawy dolny
	if( ((*win_pos).x > (win_akt.x + rozm - marg)) &&
		((*win_pos).x < (win_akt.x + rozm + marg)) &&
		((*win_pos).y > (win_akt.y + rozm - marg)) &&
		((*win_pos).y < (win_akt.y + rozm + marg)) )
	{
		(*win_pos).x = win_akt.x + rozm;
		(*win_pos).y = win_akt.y + rozm;
		(*pkt).x = boisko.akt.x +1;
		(*pkt).y = boisko.akt.y -1;
		return(1);
	}

	// dolny
	if( ((*win_pos).x > (win_akt.x - marg)) &&
		((*win_pos).x < (win_akt.x + marg)) &&
		((*win_pos).y > (win_akt.y + rozm - marg)) &&
		((*win_pos).y < (win_akt.y + rozm + marg)) )
	{
		(*win_pos).x = win_akt.x;
		(*win_pos).y = win_akt.y + rozm;
		(*pkt).x = boisko.akt.x;
		(*pkt).y = boisko.akt.y -1;
		return(1);
	}

	return(0);
}

int CKibolDlg::TrybKvsH(int win_x, int win_y)
{	
	CPunkt win_pos;
	CPunkt pkt;
	CString zwrot;
	int stan;

	win_pos.x = win_x;
	win_pos.y = win_y;

	if((tryb_gry == 2) && KonwersjaWin(&win_pos, &pkt) )
	{
		stan = boisko.RuchHum(pkt);
		if(stan != 3) 
		{
			WinRysuj(win_pos.x, win_pos.y);
		}

		switch(stan){
		case 0:
//			zwrot = MessageBox("KONIEC GRY!","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
			tryb_gry = 0;
			break;
		case 1:
//			zwrot = MessageBox("Koniec ruchow. Teraz komputer.","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
			koryguj = boisko.KorektaHvsK( koryguj );
			ZmienKolor();
			tryb_gry = 3;
			break;
		}	
	}


	if(tryb_gry == 3)
	{
		CPunkt top;
		CStos *stos_komp, *stos_temp;
		stos_komp = new CStos;

		stan = boisko.RuchKomp(stos_komp);
//		zwrot = MessageBox("Czy cos wygenerowalo?.","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
		
		while(stos_komp != NULL)
		{
			stos_temp = stos_komp;
//			stos_komp = boisko.ZeStosu(stos_komp, &pkt, &top);
			KonwersjaKon(&(*stos_temp).pkt, &win_pos);
			WinRysuj(win_pos.x, win_pos.y);
	
			boisko.DodajSasiada(boisko.akt, (*stos_temp).pkt); 
			boisko.punkt_info[boisko.akt.x][boisko.akt.y].deg++;
			boisko.punkt_info[(*stos_temp).pkt.x][(*stos_temp).pkt.y].deg++;
			boisko.akt = (*stos_temp).pkt;
			stos_komp = (*stos_temp).prev;
			delete stos_temp;
		}


		switch(stan){
		case 0:
//			zwrot = MessageBox("KONIEC GRY!","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
			tryb_gry = 0;
			break;
		case 1:
//			zwrot = MessageBox("Koniec drogi. Teraz czlowiek.","Tryb Hum_vs_Hum",MB_OK | MB_ICONEXCLAMATION);
			ZmienKolor();
			tryb_gry = 2;
			break;
		}	
	}

	return(0);
}

int CKibolDlg::KonwersjaKon(CPunkt *pkt, CPunkt *win_pos)
{
	if( (*pkt).x == (boisko.akt.x +1) ) (*win_pos).x = win_akt.x + rozm;
	if( (*pkt).x == (boisko.akt.x -1) ) (*win_pos).x = win_akt.x - rozm;
	if( (*pkt).x == boisko.akt.x ) (*win_pos).x = win_akt.x;

	if( (*pkt).y == (boisko.akt.y +1) )
	{
		(*win_pos).y = win_akt.y - rozm;
		return(1);
	}
	
	if( (*pkt).y == (boisko.akt.y -1) )
	{
		(*win_pos).y = win_akt.y + rozm;
		return(1);
	}

	if( (*pkt).y == boisko.akt.y )
	{
		(*win_pos).y = win_akt.y;
		return(1);
	}

	return(0);
}

int CKibolDlg::Marker()
{
	CClientDC pdc(this);
	int x, y;

	COLORREF marker_kolor;	
	//CWindowDC pdc(this);

	if(marker_check == 1) marker_kolor = RGB(0, 0, 0);
	else marker_kolor = bk_kolor;

	CPen pioro(PS_SOLID, 1, marker_kolor);
	//CPen pioro(PS_SOLID, 1, bk_kolor);
	CPen *p_pioro;
	p_pioro = pdc.SelectObject(&pioro);

	for(x =(przesun.x + offset.x);  x<=(6*rozm + przesun.x + offset.x); x+=rozm)
		for(y=(przesun.y + offset.y); y<=(8*rozm + przesun.y + offset.y); y+=rozm)
		{
			pdc.MoveTo(x,y);
			pdc.LineTo(x +1,y +1);
		}

	pdc.SelectObject(p_pioro);
	WinRysujBoisko();

	return(0);
}

COLORREF CKibolDlg::ZmienKolor()
{
	if(kolor == RGB(255, 0, 0)) kolor = RGB(0, 0, 255);
	else kolor = RGB(255, 0, 0);
	return(kolor);
}

void CKibolDlg::OnGraMarkery() 
{

	if( marker_check == 0) marker_check = 1;
	else marker_check = 0;

	Marker();
}

void CKibolDlg::OnTrybHumvshum() 
{
	Zamazuj();
	WinRysujBoisko();
	Marker();

	boisko.akt.x = 4;
	boisko.akt.y = 5;
	boisko.UstawDane();
	win_akt.x = 3*rozm + przesun.x + offset.x;
	win_akt.y = 4*rozm + przesun.y + offset.y;

	tryb_gry = 1;
}

void CKibolDlg::OnTrybHumvskomp() 
{
	Zamazuj();
	WinRysujBoisko();
	Marker();

	boisko.akt.x = 4;
	boisko.akt.y = 5;
	boisko.UstawDane();
	win_akt.x = 3*rozm + przesun.x + offset.x;
	win_akt.y = 4*rozm + przesun.y + offset.y;

	tryb_gry = 2;
	koryguj = 0;
}

void CKibolDlg::OnTrybKompvshum() 
{
	Zamazuj();
	WinRysujBoisko();
	Marker();

	boisko.akt.x = 4;
	boisko.akt.y = 5;
	boisko.UstawDane();
	win_akt.x = 3*rozm + przesun.x + offset.x;
	win_akt.y = 4*rozm + przesun.y + offset.y;

	tryb_gry = 3;

	TrybKvsH(win_akt.x, win_akt.y);
}

void CKibolDlg::OnGraKoniec() 
{
	OnOK();
}

int CKibolDlg::Zamazuj()
{
	CClientDC pdc(this);
	CPen pioro(PS_SOLID, 1, bk_kolor );
	CBrush pedzel(bk_kolor);
	CPen *p_pioro;
	CBrush *p_pedzel;

	p_pedzel = pdc.SelectObject(&pedzel);
	p_pioro = pdc.SelectObject(&pioro);

	pdc.SetBkMode( OPAQUE );
	pdc.Rectangle(przesun.x, przesun.y, przesun.x + 6*rozm, przesun.y + 8*rozm);
	pdc.Rectangle(przesun.x + 2*rozm, przesun.y - rozm, przesun.x + 4*rozm, przesun.y);
	pdc.Rectangle(przesun.x + 2*rozm, przesun.y + 8*rozm, przesun.x + 4*rozm, przesun.y + 9*rozm);

	pdc.SelectObject(p_pedzel);
	pdc.SelectObject(p_pioro);

	return(0);
}

void CKibolDlg::OnGraNowa() 
{
	Zamazuj();
	WinRysujBoisko();
	Marker();

	boisko.akt.x = 4;
	boisko.akt.y = 5;
	boisko.UstawDane();
	win_akt.x = 3*rozm + przesun.x + offset.x;
	win_akt.y = 4*rozm + przesun.y + offset.y;

	tryb_gry = 0;
}

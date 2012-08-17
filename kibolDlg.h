// kibolDlg.h : header file
//

#if !defined(AFX_KIBOLDLG_H__C33D6898_7569_45F4_8A45_66AE6574FA01__INCLUDED_)
#define AFX_KIBOLDLG_H__C33D6898_7569_45F4_8A45_66AE6574FA01__INCLUDED_

#include "kibol.h"	// Added by ClassView
#include "KibolM.h"
#include "Punkt.h"
#include "Stos.h"	// Added by ClassView


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKibolDlg dialog

class CKibolDlg : public CDialog
{
// Construction
public:
	int KonwersjaKon(CPunkt *pkt, CPunkt *win_pos);
	int TrybKvsH(int win_x, int win_y);
	int tryb_gry;
	CPunkt przesun;
	int rozm;
	int KonwersjaWin(CPunkt *win_pos, CPunkt *pkt);
	int TrybHvsH(int win_x, int win_y);
	CKibolM boisko;
	CPunkt offset;
	void WinRysujBoisko();
	void WinRysuj(int x, int y);
	CPunkt win_akt;
	CKibolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKibolDlg)
	enum { IDD = IDD_KIBOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKibolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKibolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KIBOLDLG_H__C33D6898_7569_45F4_8A45_66AE6574FA01__INCLUDED_)

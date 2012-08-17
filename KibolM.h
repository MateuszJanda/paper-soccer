// JEZU UFAM TOBIE...
//
// KibolM.h: interface for the CKibolM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KIBOLM_H__09055707_E912_4AC4_A11A_C914B5F72FC8__INCLUDED_)
#define AFX_KIBOLM_H__09055707_E912_4AC4_A11A_C914B5F72FC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Punkt.h"
#include "Stos.h"
#include "Drogi.h"
#include "WierzchGrafu.h"

class CKibolM  
{
public:
	int KorektaHvsK(int krok);
	CDrogi * LosujRozw(CDrogi *drogi);
	CDrogi * KasujDrogi(CDrogi *droga);
	CDrogi *DodajDroge(CDrogi *droga, CStos *stos);
	CStos * KasujStos(CStos *stos);
	int RuchKomp(CStos *stos);
	int RuchHum(CPunkt pkt);
	CPunkt GenerujPozycje(CPunkt baza_gen, CPunkt prev_gen);
	CStos * KopiujStos(CStos *stos);
	CStos * OdwrocStos(CStos *stos);
	CStos * ZeStosu(CStos *stos, CPunkt *removed, CPunkt *top);
	CStos * NaStos(CStos *stos, CPunkt punkt);
	BOOL SprSasiada(CPunkt punkt_1, CPunkt punkt_2);
	BOOL OdejmijSasiada(CPunkt punkt_1, CPunkt punkt_2);
	BOOL NrSasiada(CPunkt punkt_1, CPunkt punkt_2, int *nr_sasiad_1, int *nr_sasiad_2);
	BOOL DodajSasiada(CPunkt punkt_1, CPunkt punkt_2);
	CWierzchGrafu punkt_info[9][11];
//	int stan_gry;
//	CStos stos_hum;
//	CStos stos_komp;
	CPunkt akt;
//	int tryb_gry;
	int lista_sasiedztw[9][11][8];
	BOOL UstawDane();
	CKibolM();
	int height;
	int width;
	virtual ~CKibolM();
};

#endif // !defined(AFX_KIBOLM_H__09055707_E912_4AC4_A11A_C914B5F72FC8__INCLUDED_)

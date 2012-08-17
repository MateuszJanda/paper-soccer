// KibolM.cpp: implementation of the CKibolM class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include "stdafx.h"
#include "kibol.h"
#include "KibolM.h"
#include "Punkt.h"
#include "Stos.h"
#include "WierzchGrafu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_L 0
#define L_L 1
#define G_L 2
#define G_G 3
#define G_P 4
#define P_P 5
#define D_P 6
#define D_D 7 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKibolM::CKibolM()
{
	width = 8;
	height = 10;

	akt.x = 4;
	akt.y = 5;

	srand( time(NULL) );
	UstawDane();
}

CKibolM::~CKibolM()
{

}

BOOL CKibolM::UstawDane()
{
	int x, y, i;
	int width_div = (int)(width / 2);

// USTAWIANIE WAG
	for(x = 0; x<=width; x++)
		for(y =0; y<=height; y++)
			punkt_info[x][height - y].waga = y;

	punkt_info[1][height -2].waga = 3;
	punkt_info[width -1][height -2].waga = 3;

// USTAWIANIE STOPNI
	for(x = 0; x<=width; x++)
		for(y = 0; y<=height; y++)
		{
			punkt_info[x][y].deg = 0;
			punkt_info[1][y].deg = 5;
			punkt_info[width -1][y].deg = 5;
		    punkt_info[x][1].deg = 5;
		    punkt_info[x][height -1].deg = 5;
		    punkt_info[0][y].deg = 8;
			punkt_info[width][y].deg = 8;
			punkt_info[x][0].deg = 8;
		    punkt_info[x][height].deg = 8;
		}


	punkt_info[width_div -1][1].deg = 3;
	punkt_info[width_div -1][height-1].deg = 3;
	punkt_info[width_div +1][1].deg = 3;
	punkt_info[width_div +1][height-1].deg = 3;
	punkt_info[width_div][0].deg = 0;
	punkt_info[width_div][1].deg = 0;
	punkt_info[width_div][height-1].deg = 0;
	punkt_info[width_div][height].deg = 0;
	punkt_info[1][1].deg = 7;
	punkt_info[1][height-1].deg = 7;
	punkt_info[width-1][1].deg = 7;
	punkt_info[width-1][height -1].deg = 7;
	punkt_info[width-1][height -1].deg = 7;
	punkt_info[1][0].deg = 8;
	punkt_info[1][height].deg = 8;
	punkt_info[width][1].deg = 8;
	punkt_info[width][height -1].deg = 8;

// USTAWIANIE LISTY SASIEDZTW

	// na poczatku zerowanie calej tablicy sasiedztw
	for(x = 0; x<=width; x++)
		for(y = 0; y<=height; y++)
			for(i = 0; i<8; i++)
				lista_sasiedztw[x][y][i] = 0;

	for(x = 0; x<=width; x++)
		for(y = 0; y<=height; y++)
		{
			// USTAWIANIE DOLNEGO LEWEGO SASIADA
			// ustawianie w pionie
			lista_sasiedztw[1][y][D_L]			= 1;		//w ld
			lista_sasiedztw[width][y][D_L]		= 1;
			// ustawianie w poziomie
			lista_sasiedztw[x][1][D_L]			= 1;
			lista_sasiedztw[x][height][D_L]		= 1;

			lista_sasiedztw[1][y][L_L]			= 1;       //w lewo
			lista_sasiedztw[width][y][L_L]		= 1;
			lista_sasiedztw[x][1][L_L]			= 1;
			lista_sasiedztw[x][height -1][L_L]	= 1;

			lista_sasiedztw[1][y][G_L]			= 1;       //w lg
			lista_sasiedztw[width][y][G_L]		= 1;
			lista_sasiedztw[x][0][G_L]			= 1;
			lista_sasiedztw[x][height -1][G_L]	= 1;

			lista_sasiedztw[1][y][G_G]			= 1;       //gora
			lista_sasiedztw[width -1][y][G_G]	= 1;
			lista_sasiedztw[x][0][G_G]			= 1;
			lista_sasiedztw[x][height -1][G_G]	= 1;
			
			lista_sasiedztw[0][y][G_P]			= 1;        //pg
			lista_sasiedztw[width -1][y][G_P]	= 1;
			lista_sasiedztw[x][0][G_P]			= 1;
			lista_sasiedztw[x][height -1][G_P]	= 1;

			lista_sasiedztw[0][y][P_P]			= 1;       //w prawo
			lista_sasiedztw[width-1][y][P_P]	= 1;
			lista_sasiedztw[x][1][P_P]			= 1;
			lista_sasiedztw[x][height -1][P_P]	= 1;
			
			lista_sasiedztw[0][y][D_P]			= 1;        //pd
			lista_sasiedztw[width-1][y][D_P]	= 1;
			lista_sasiedztw[x][1][D_P]			= 1;
			lista_sasiedztw[x][height][D_P]		= 1;
	
			lista_sasiedztw[1][y][D_D]			= 1;      //w dol
			lista_sasiedztw[width -1][y][D_D]	= 1;
			lista_sasiedztw[x][1][D_D]			= 1;
			lista_sasiedztw[x][height][D_D]		= 1;
		}
	
	// DOLNA BRAMKA
//	lista_sasiedztw[width_div][1][D_L]			= 0;
//	lista_sasiedztw[width_div][1][D_P]			= 0;

	lista_sasiedztw[width_div -1][1][P_P]		= 0;
	lista_sasiedztw[width_div -1][1][D_P]		= 0;

	lista_sasiedztw[width_div +1][1][D_L]		= 0;
	lista_sasiedztw[width_div +1][1][L_L]		= 0;

	// GORNA BRAMKA
//	lista_sasiedztw[width_div][height -1][G_P]	= 0;
//	lista_sasiedztw[width_div][height -1][G_L]	= 0;

	lista_sasiedztw[width_div -1][height -1][G_P] = 0;
	lista_sasiedztw[width_div -1][height -1][P_P] = 0;
	 
	lista_sasiedztw[width_div +1][height -1][L_L] = 0;
	lista_sasiedztw[width_div +1][height -1][G_L] = 0;
	

	for(y = 0; y<=height; y++)
		for(i = 0; i<=7; i++)
			lista_sasiedztw[width_div][y][i] = 0;
			
	return(true);
}

BOOL CKibolM::DodajSasiada(CPunkt punkt_1, CPunkt punkt_2)
{
	int nr_sasiad_1, nr_sasiad_2;


	NrSasiada(punkt_1, punkt_2, &nr_sasiad_1, &nr_sasiad_2);

	lista_sasiedztw[punkt_1.x][punkt_1.y][nr_sasiad_1] = 1;
	lista_sasiedztw[punkt_2.x][punkt_2.y][nr_sasiad_2] = 1;
	return(0);
}

BOOL CKibolM::NrSasiada(CPunkt punkt_1, CPunkt punkt_2, int *nr_sasiad_1, int *nr_sasiad_2)
{
	if( (punkt_2.x < punkt_1.x) && (punkt_2.y < punkt_1.y) )
	{
		*nr_sasiad_1 = 0;
		*nr_sasiad_2 = 4;
		return(0);
	}

	if( (punkt_2.x < punkt_1.x) && (punkt_2.y == punkt_1.y) )
	{
		*nr_sasiad_1 = 1;
		*nr_sasiad_2 = 5;
		return(0);
	}

	if( (punkt_2.x < punkt_1.x) && (punkt_2.y > punkt_1.y) )
	{
		*nr_sasiad_1 = 2;
		*nr_sasiad_2 = 6;
		return(0);
	}

	if( (punkt_2.x == punkt_1.x) && (punkt_2.y > punkt_1.y) )
	{
		*nr_sasiad_1 = 3;
		*nr_sasiad_2 = 7;
		return(0);
	}

	if( (punkt_2.x > punkt_1.x) && (punkt_2.y > punkt_1.y) )
	{
		*nr_sasiad_1 = 4;
		*nr_sasiad_2 = 0;
		return(0);
	}

	if( (punkt_2.x > punkt_1.x) && (punkt_2.y == punkt_1.y) )
	{
		*nr_sasiad_1 = 5;
		*nr_sasiad_2 = 1;
		return(0);
	}

	if( (punkt_2.x > punkt_1.x) && (punkt_2.y < punkt_1.y) )
	{
		*nr_sasiad_1 = 6;
		*nr_sasiad_2 = 2;
		return(0);
	}

	if( (punkt_2.x == punkt_1.x) && (punkt_2.y < punkt_1.y) )
	{
		*nr_sasiad_1 = 7;
		*nr_sasiad_2 = 3;
		return(0);
	}
	return(1);
}

BOOL CKibolM::OdejmijSasiada(CPunkt punkt_1, CPunkt punkt_2)
{
	int nr_sasiad_1, nr_sasiad_2;

	NrSasiada(punkt_1, punkt_2, &nr_sasiad_1, &nr_sasiad_2);

	lista_sasiedztw[punkt_1.x][punkt_1.y][nr_sasiad_1] = 0;
	lista_sasiedztw[punkt_2.x][punkt_2.y][nr_sasiad_2] = 0;
	return(0);
}

// 1 = sasiad znaleziony
// 0 = sasiad nie znaleziony
//
BOOL CKibolM::SprSasiada(CPunkt punkt_1, CPunkt punkt_2)
{
	int nr_sasiad_1, nr_sasiad_2;

 
	NrSasiada(punkt_1, punkt_2, &nr_sasiad_1, &nr_sasiad_2);

	//sprawdzic czy && ma pelna skutecznosc, chodzi o wypelnienie 
	//tablicy w UstawDane()
	if( (lista_sasiedztw[punkt_1.x][punkt_1.y][nr_sasiad_1] == 1) &&
		(lista_sasiedztw[punkt_2.x][punkt_2.y][nr_sasiad_2] == 1) )
		return(1);
	else return(0);
}

CStos *CKibolM::NaStos(CStos *stos, CPunkt punkt)
{
	CStos *temp;
	
	temp = new CStos;

	if(stos == NULL) (*temp).dl = 1;
	else (*temp).dl = (*stos).dl +1;

	(*temp).pkt.x = punkt.x;
	(*temp).pkt.y = punkt.y;
	(*temp).prev = stos;
//	stos = temp;
	
	return(temp);
}

CStos* CKibolM::ZeStosu(CStos *stos, CPunkt *removed, CPunkt *top)
{
	CStos *temp;

	temp = stos;
	(*removed).x = (*stos).pkt.x;
	(*removed).y = (*stos).pkt.y;
	stos = (*temp).prev;
	delete temp;

	if(stos == NULL) *top = akt;
	else *top = (*stos).pkt;

	return(stos);
}

CStos *CKibolM::OdwrocStos(CStos *stos)
{
	CPunkt removed, top;
	CStos *temp;

	temp = stos;
	stos = NULL;
	do {
		temp = ZeStosu(temp, &removed, &top);
		stos = NaStos(stos, removed);
	} while(temp != NULL);

	return(stos);
}

CStos * CKibolM::KopiujStos(CStos *stos)
{
	CStos *temp;
	CStos *nowy_stos;
	
//	nowy_stos = new CStos;
	nowy_stos = NULL;

	temp = stos;

	do {
		nowy_stos = NaStos(nowy_stos, (*temp).pkt );
		temp = (*temp).prev;
	} while(temp != NULL);

	nowy_stos = OdwrocStos(nowy_stos);

	return(nowy_stos);
}

CPunkt CKibolM::GenerujPozycje(CPunkt baza_gen, CPunkt prev_gen)
{
	CPunkt new_wygen;

	//0
	if(( prev_gen.x == -1 ) || ( prev_gen.y == -1 ))
	{
		new_wygen.x = baza_gen.x -1;
		new_wygen.y = baza_gen.y -1;
		return(new_wygen);
	}

	//1
	if( (prev_gen.x == baza_gen.x -1) && (prev_gen.y == baza_gen.y -1) )
	{
		new_wygen.x = baza_gen.x -1;
		new_wygen.y = baza_gen.y;
		return(new_wygen);
	}

	//2
	if( (prev_gen.x == baza_gen.x -1) && (prev_gen.y == baza_gen.y) )
	{
		new_wygen.x = baza_gen.x -1;
		new_wygen.y = baza_gen.y +1;
		return(new_wygen);
	}

	//3
	if( (prev_gen.x == baza_gen.x -1) && (prev_gen.y == baza_gen.y +1) )
	{
		new_wygen.x = baza_gen.x;
		new_wygen.y = baza_gen.y +1;
		return(new_wygen);
	}

	//4
	if( (prev_gen.x == baza_gen.x) && (prev_gen.y == baza_gen.y +1) )
	{
		new_wygen.x = baza_gen.x +1;
		new_wygen.y = baza_gen.y +1;
		return(new_wygen);
	}

	//5
	if( (prev_gen.x == baza_gen.x +1) && (prev_gen.y == baza_gen.y +1) )
	{
		new_wygen.x = baza_gen.x +1;
		new_wygen.y = baza_gen.y;
		return(new_wygen);
	}

	//6
	if( (prev_gen.x == baza_gen.x +1) && (prev_gen.y == baza_gen.y) )
	{
		new_wygen.x = baza_gen.x +1;
		new_wygen.y = baza_gen.y -1;
		return(new_wygen);
	}

	//7
	if( (prev_gen.x == baza_gen.x +1) && (prev_gen.y == baza_gen.y -1) )
	{
		new_wygen.x = baza_gen.x;
		new_wygen.y = baza_gen.y -1;
		return(new_wygen);
	}

	//gdy nie mozna wiecej generowac
	if( (prev_gen.x == baza_gen.x) && (prev_gen.y == baza_gen.y -1) )
	{
		new_wygen.x = -1;
		new_wygen.y = -1;
	}
	return(new_wygen);
}


/*	0 = koniec gry
 *	1 = wykonany ruch jest dozwolony, koniec ruchow czlowieka
 *	2 =	wykonany ruch jest dozwolony, czowiek gra dalej
 *	3 =	ruch niedozwolony
 */
int CKibolM::RuchHum(CPunkt pkt)
{	
	CPunkt temp_pkt;
	temp_pkt = pkt;

	if ( (SprSasiada(akt, temp_pkt) == false) &&
		 (punkt_info[pkt.x][pkt.y].deg < 7) )
	{	
		DodajSasiada(akt, temp_pkt);
		punkt_info[akt.x][akt.y].deg++;
		punkt_info[temp_pkt.x][temp_pkt.y].deg++;
		akt = temp_pkt;

		if( (punkt_info[akt.x][akt.y].waga == 0) ||
			(punkt_info[akt.x][akt.y].waga == height) ) return(0);

		if( punkt_info[akt.x][akt.y].deg == 1 ) return(1);

		return(2);
	}

	return(3);
}


/*	0 = koniec gry
 *	1 = wykonany ruch jest dozwolony, koniec ruchow komputera
 */
int CKibolM::RuchKomp(CStos *stos)
{
	CPunkt bazowy, poprzed, nowy;
    CStos *stos_gen; //, *stos_temp;	
	CDrogi *rozwiaz;
	int koniec_gener = 0;

	
	bazowy = akt;
	stos_gen = NULL;
	rozwiaz = NULL;

	do {
// ta czes petli zajmuje sie generowaniem i wybieraniem kolejnych wierzcholkow
// - zajmuje sie budowaniem stosu ( dorgi )
		do {
			nowy = GenerujPozycje(bazowy, poprzed);

// jezli droga jest wciaz budowna to uzupelniane sa dane o wierzcholku poniewaz
// moze sie zdarzyc ze w wyniku budowania drogi program utworzy cykl
			if( (nowy.x != -1) && (nowy.y != -1) )
			{
				stos_gen = NaStos(stos_gen, nowy);

// budowanie dorgi zostaje wstrzymane jezeli wierzcholke ktory chcielismy
// dodac juz istnieje na liscie sasiedztw lyb gdy nie da sie z niego
// generowac nastepnych wierzcholkow poniewaz jego stopien jest >= 7
				if( SprSasiada(bazowy, nowy) == false  && 
					punkt_info[nowy.x][nowy.y].deg < 7 )
				{
					DodajSasiada(bazowy, nowy);
					punkt_info[bazowy.x][bazowy.y].deg++;
					punkt_info[nowy.x][nowy.y].deg++;
					bazowy = nowy;
					poprzed.x = -1;
					poprzed.y = -1;
				} 
				else stos_gen = ZeStosu(stos_gen, &poprzed, &bazowy);
			}
		} while( ((nowy.x != -1) && (nowy.y != -1)) &&
			(punkt_info[nowy.x][nowy.y].deg > 1) &&
			(punkt_info[bazowy.x][bazowy.y].waga != 0) );

//++++++++++++++++++++
// ta czes petli REPEAT zajmuje sie zdejmowaniem elementu ze stosu ( cofnieciem
// sie z drogi o jeden element ), a takze wyborem najlepszej z drog


// do anlizy wymagana jest jakakolwiek nie pusta droga, zawsze na poczatku
// bedzie to droga pierwsza      
		if(rozwiaz == NULL)
		{
			rozwiaz = DodajDroge(rozwiaz, stos_gen);
		}

// anlizy i wybory najlepszej, czyli drogri ktorej ostatni wierzcholek ma jak
// ma najmnijsza wage, a sama dorga jest jak najdluzsza
		if( (nowy.x != -1) && (nowy.y != -1) )
		{
// poszukiwanie najlepszej wagi

//## narazie beda wyszukiwane dogi o najlepszych wagach bez 
//	uwzgledniania dlugosci drogi, sprawdzic w PRANIU
			if( punkt_info[(*stos_gen).pkt.x][(*stos_gen).pkt.y].waga ==
				punkt_info[(*rozwiaz).stos->pkt.x][(*rozwiaz).stos->pkt.y].waga)
			{	
//##				if( (*stos_gen).dl > (*rozwiaz).stos->dl) 
//##					rozwiaz = KasujDrogi(rozwiaz);
				rozwiaz = DodajDroge(rozwiaz, stos_gen);
			}

			if( punkt_info[(*stos_gen).pkt.x][(*stos_gen).pkt.y].waga <
				punkt_info[(*rozwiaz).stos->pkt.x][(*rozwiaz).stos->pkt.y].waga)
			{
				rozwiaz = KasujDrogi(rozwiaz);
				rozwiaz = DodajDroge(rozwiaz, stos_gen);
			}
		}

// Zdejmowanie elementu ze stosu i odpowiedznia modyfikacja danych
		if( stos_gen != NULL )
		{
			stos_gen = ZeStosu(stos_gen, &poprzed, &bazowy);		
			OdejmijSasiada(bazowy, poprzed);
			punkt_info[bazowy.x][bazowy.y].deg--;
			punkt_info[poprzed.x][poprzed.y].deg--;
		}

//## te 2 warunki powinny byc wystarczajace ale nalezy przetesotwc
		if( (stos_gen == NULL) &&
			(nowy.x == -1) && (nowy.y == -1) )
			if( (bazowy.x == akt.x) && (bazowy.y == akt.y) &&
				(poprzed.x == bazowy.x) && (poprzed.y == bazowy.y -1) )
				koniec_gener = 1;

	} while( koniec_gener == 0 &&
		punkt_info[bazowy.x][bazowy.y].waga != 0);

// likwidowanie TYMCZASOWO stosu gdy zakonczono generacje 
	KasujStos(stos_gen);
	rozwiaz = LosujRozw(rozwiaz);
	stos_gen = KopiujStos((*rozwiaz).stos);
	rozwiaz = KasujDrogi(rozwiaz);


// odwrocenie stosu jest konieczne do tego aby narysowac dorge na boisku
// poniewaz na szczycie znajduje sie ostatni element drogi a rysowanie musimy
// zaczac od pierwszego
	stos_gen = OdwrocStos(stos_gen);


// odpowiednia modyfikacja listy_sasiedztw oraz stopni wierzcholkow
/*
	stos_temp = stos_gen;
	do {
		DodajSasiada(akt, (*stos_temp).pkt); 
		punkt_info[akt.x][akt.y].deg++;
		punkt_info[(*stos_temp).pkt.x][(*stos_temp).pkt.y].deg++;
		akt = (*stos_temp).pkt;
		stos_temp = (*stos_temp).prev;
	} while( stos_temp != NULL);
*/
	*stos = *stos_gen;
//## 
	delete stos_gen;
//##
	if(punkt_info[akt.x][akt.y].waga == 0) return(0);
	else return(1);
}

CStos * CKibolM::KasujStos(CStos *stos)
{
	CPunkt removed, top;

	while( stos != NULL)
		stos = ZeStosu(stos, &removed, &top);
	return(stos);
}

CDrogi *CKibolM::DodajDroge(CDrogi *droga, CStos *stos)
{
	CDrogi *temp;

	temp = new CDrogi;

	if(droga == NULL) (*temp).ilosc = 1;
	else (*temp).ilosc = (*droga).ilosc +1;

	(*temp).stos = KopiujStos(stos);
	(*temp).prev_rozw = droga;

//	droga = temp;
	return(temp);
}

CDrogi * CKibolM::KasujDrogi(CDrogi *droga)
{
	CDrogi *temp;

	while(droga != NULL)
	{
		temp = droga;
		KasujStos( (*droga).stos );
		droga = (*temp).prev_rozw;
		delete temp;
	}
	return(droga);
}

CDrogi *CKibolM::LosujRozw(CDrogi *droga)
{
	CDrogi *temp;
	int wynik;
	
	//wynik = 1 + (int)( (*droga).ilosc * rand() / RAND_MAX +1.0);
	wynik = (int)( ((*droga).ilosc -1) * rand() / RAND_MAX);

	while(wynik--)
	{
		temp = droga;
		droga = (*temp).prev_rozw;
		delete temp;
	}

	return(droga);
}
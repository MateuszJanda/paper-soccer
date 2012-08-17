// Drogi.h: interface for the CDrogi class.
//
//////////////////////////////////////////////////////////////////////
#include "Stos.h"

#if !defined(AFX_DROGI_H__DE6B21B6_6BAA_437D_AFBF_85E4BA5F2D30__INCLUDED_)
#define AFX_DROGI_H__DE6B21B6_6BAA_437D_AFBF_85E4BA5F2D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrogi  
{
public:
//#	CDrogi();
	int ilosc;
	CDrogi *prev_rozw;
	CStos *stos;
};

#endif // !defined(AFX_DROGI_H__DE6B21B6_6BAA_437D_AFBF_85E4BA5F2D30__INCLUDED_)

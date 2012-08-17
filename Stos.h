// Stos.h: interface for the CStos class.
//
//////////////////////////////////////////////////////////////////////
#include "Punkt.h"

#if !defined(AFX_STOS_H__68F6FAE2_C437_46F9_B618_37BAAE1A9C64__INCLUDED_)
#define AFX_STOS_H__68F6FAE2_C437_46F9_B618_37BAAE1A9C64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStos  
{
public:
	CStos();
	int dl;
	CPunkt pkt;
	CStos *prev;
};

#endif // !defined(AFX_STOS_H__68F6FAE2_C437_46F9_B618_37BAAE1A9C64__INCLUDED_)

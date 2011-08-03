
#ifndef CATTRACTOR_H
#define CATTRACTOR_H

#include "../CBaseObject.h"

class CAttractor : public CBaseObject
{
	int m_nElemType ;	// tells us what element this attractor is

public:
	CAttractor(void) ;
	~CAttractor(void) ;

	//	Accessor
	int GetElemType(void) { return m_nElemType ; } ;

	//	mutators
	void SetElemType(int type) { m_nElemType = type ; } ;
}


#endif
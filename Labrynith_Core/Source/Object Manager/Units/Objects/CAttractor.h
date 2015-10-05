#ifndef CATTRACTOR_H
#define CATTRACTOR_H

#include "../CBaseObject.h"

class CAttractor : public CBaseObject
{
private:
	int m_nElemType ;
public:
	CAttractor(void) ;
	CAttractor(int elemType) ;

	//	mutators
	void SetElemType(int type) { m_nElemType = type ; } ;

	//	accessors
	int GetElemType(void) { return m_nElemType ; } ;
} ;

#endif
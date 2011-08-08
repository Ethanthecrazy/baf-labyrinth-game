#ifndef CELECTRICGENERATOR_H
#define CELECTRICGENERATOR_H

#include "../CBaseObject.h"

class CElectricGenerator: public CBaseObject
{
private:
	float m_nElectricUpdateTimer ;
public:
	CElectricGenerator() ;
	~CElectricGenerator() ;

	void Update(float fDT ) ;

	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
} ;

#endif
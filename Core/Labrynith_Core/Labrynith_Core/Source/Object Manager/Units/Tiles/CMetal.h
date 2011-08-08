
#ifndef CMETAL_H
#define CMETAL_H

#include "../CBaseObject.h"

class CMetal : public CBaseObject
{
private:
	bool m_bPowered ;
	float m_nElectricUpdateTimer ;
public:
	CMetal() ;
	~CMetal() ;

	void Update( float fDT ) ;

	bool GetIsElectrified( void ) { return m_bPowered ; } ;
	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
	void SetPowered( bool powered ) { m_bPowered = powered ; } ;
} ;

#endif
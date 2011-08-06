#ifndef COILCAN_H
#define COILCAN_H

#include "../CBaseObject.h"

class COilCan : public CBaseObject
{
private:
	int m_nCharges ;
	
public:
	COilCan( void ) ;
	
	void Render( int CameraPosX, int CameraPosY );

	void UseObject( CBaseObject* user ) ;
	
	void SetCharges( int charges ) { m_nCharges = charges ; } ;
	
	int GetCharges( void ) { return m_nCharges ; } ;
	
} ;


#endif
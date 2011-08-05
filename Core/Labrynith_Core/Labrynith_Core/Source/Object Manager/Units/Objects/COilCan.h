#ifndef COILCAN_H
#define COILCAN_H

#include "../CBaseObject.h"

class COilCan : public CBaseObject
{
private:
	int m_nCharges ;
	float m_fLifeDuration;
	bool m_bOnFire ;
public:
	COilCan( void ) ;

	void Update(float fDT) ;
	
	void Render( int CameraPosX, int CameraPosY );

	void UseObject( CBaseObject* user ) ;
	
	void SetCharges( int charges ) { m_nCharges = charges ; } ;
	void SetLifeDuration( int life ) { m_fLifeDuration = life ; } ;
	void SetOnFire( bool onFire ) { m_bOnFire = onFire ; } ; 

	int GetCharges( void ) { return m_nCharges ; } ;
	float GetLifeDuration( void ) { return m_fLifeDuration ; } ;
	bool GetOnFire( void ) { return m_bOnFire ; } ;

	
} ;


#endif
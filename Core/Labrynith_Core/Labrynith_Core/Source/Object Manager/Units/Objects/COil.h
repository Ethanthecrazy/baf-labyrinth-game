#ifndef COIL_H
#define COIL_H

#include "../CBaseObject.h"

class COil : public CBaseObject
{
private:
	int m_nAnimID ;
	int m_nAnimImageID ;
	float m_fLifeDuration ;
	float m_fFireTimer ;
	bool m_bOnFire ;
public:
	COil( void ) ;

	void Update(float fDT) ;

	void Render( int CameraPosX , int CameraPosY ) ;

	void SetLifeDuration( int life ) { m_fLifeDuration = life ; } ;
	void SetFireTimer( int timer ) { m_fFireTimer = timer ; } ;
	void SetOnFire( bool onFire ) ;

	float GetLifeDuration( void ) { return m_fLifeDuration ; } ;
	float GetFireTimer( void ) { return m_fFireTimer ; } ;
	bool GetOnFire( void ) { return m_bOnFire ; } ;
} ;

#endif
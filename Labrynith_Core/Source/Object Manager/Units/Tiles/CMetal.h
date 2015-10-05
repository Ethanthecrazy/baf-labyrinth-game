
#ifndef CMETAL_H
#define CMETAL_H

#include "../CBaseObject.h"
#include "../../../Messaging/MEventSystem.h"

class CMetal : public CBaseObject , public IListener
{
private:
	int m_nAnimID ;
	int m_nAnimImageID ;
	bool m_bPowered ;
	bool m_bGolemPowered ;
	float m_nElectricUpdateTimer ;
public:
	CMetal() ;
	~CMetal() ;

	void Update( float fDT ) ;
	void Render( int CameraPosX, int CameraPosY );

	void HandleEvent( Event* _toHandle );

	bool GetIsElectrified( void ) { return m_bPowered ; } ;
	bool GetIsGolemPowered( void ) { return m_bGolemPowered ; } ;
	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
	void SetPowered( bool powered ) ;
	void SetGolemPowered( bool powered ) { m_bGolemPowered = powered ; } ;
} ;

#endif
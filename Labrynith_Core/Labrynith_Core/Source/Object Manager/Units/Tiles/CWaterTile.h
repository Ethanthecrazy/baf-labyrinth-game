#ifndef CWATERTILE
#define CWATERTILE

#include "../CBaseObject.h"
#include "../../../Messaging/MEventSystem.h"

class CWaterTile : public CBaseObject , public IListener
{
	int m_nAnimID ;
	int m_nAnimImageID ;
	bool m_bPowered ;
	float m_nElectricUpdateTimer ;
	int m_nWaterImageID;
	int m_nIceImageID;
	bool m_bIsFrozen;
	bool m_bGolemPowered ;
public:
	CWaterTile();
	CWaterTile(bool bIsFrozen);
	~CWaterTile();

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);

	void HandleEvent( Event* _toHandle );

	//accessors
	bool IsFrozen();
	bool GetIsElectrified( void ) { return m_bPowered ; } ;
	bool GetIsGolemPowered( void ) { return m_bGolemPowered ; } ;
	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	//mutators
	void SetIsFrozen(const bool bIsFrozen);
	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
	void SetPowered( bool powered ) ;
	void SetGolemPowered( bool powered ) { m_bGolemPowered = powered ; } ;
};
#endif
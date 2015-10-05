#ifndef CGOLEM_IRON
#define CGOLEM_IRON

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Iron : public CBaseGolem , public IListener
{
private:
	int m_nAnimID ;
	int m_nAnimImageID ;
	bool m_bPowered ;
	bool m_bGolemPowered ;
	float m_nElectricUpdateTimer ;
	void IronGolemSetup();
public:
	CGolem_Iron(void);
	CGolem_Iron(CBaseGolem* pGolem);
	~CGolem_Iron(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
	void SetFlag_MovementState( int newFlag );

	bool GetIsElectrified( void ) { return m_bPowered ; } ;
	bool GetIsGolemPowered( void ) { return m_bGolemPowered ; } ;
	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
	void SetPowered( bool powered ) ;
	void SetGolemPowered( bool powered ) { m_bGolemPowered = powered ; } ;
};
#endif
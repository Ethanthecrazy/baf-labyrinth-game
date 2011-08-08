#ifndef CGOLEM_IRON
#define CGOLEM_IRON

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Iron : public CBaseGolem , public IListener
{
private:
	bool m_bPowered ;
	float m_nElectricUpdateTimer ;
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

	bool GetIsElectrified( void ) { return m_bPowered ; } ;
	int GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
	void SetPowered( bool powered ) { m_bPowered = powered ; } ;
};
#endif
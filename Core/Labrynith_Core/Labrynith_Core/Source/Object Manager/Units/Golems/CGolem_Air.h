#ifndef CGOLEM_AIR
#define CGOLEM_AIR

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Air : public CBaseGolem , public IListener
{

public:
	CGolem_Air(void);
	CGolem_Air(CBaseGolem* pGolem);
	~CGolem_Air(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
};
#endif
#ifndef CGOLEM_WATER
#define CGOLEM_WATER

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Water : public CBaseGolem , public IListener
{

public:
	CGolem_Water(void);
	CGolem_Water(CBaseGolem* pGolem);
	~CGolem_Water(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
};
#endif
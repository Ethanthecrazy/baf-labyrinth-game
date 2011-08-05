#ifndef CGOLEM_LAVA
#define CGOLEM_LAVA

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Lava : public CBaseGolem , public IListener
{

public:
	CGolem_Lava(void);
	CGolem_Lava(CBaseGolem* pGolem);
	~CGolem_Lava(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
};
#endif
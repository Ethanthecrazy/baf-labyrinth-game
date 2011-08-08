#ifndef CGOLEM_FIRE
#define CGOLEM_FIRE

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Fire : public CBaseGolem , public IListener
{

public:
	CGolem_Fire(void);
	CGolem_Fire(CBaseGolem* pGolem);
	~CGolem_Fire(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	void ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
};
#endif
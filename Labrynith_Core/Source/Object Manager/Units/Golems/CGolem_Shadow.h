#ifndef CGOLEM_SHADOW
#define CGOLEM_SHADOW

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Shadow : public CBaseGolem , public IListener
{

public:
	CGolem_Shadow(void);
	CGolem_Shadow(CBaseGolem* pGolem);
	~CGolem_Shadow(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
	void DrainLight( int _range );
};
#endif
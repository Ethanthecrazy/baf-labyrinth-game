#ifndef CGOLEM_IRON
#define CGOLEM_IRON

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Iron : public CBaseGolem , public IListener
{

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
};
#endif
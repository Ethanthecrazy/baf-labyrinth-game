#ifndef CGOLEM_LIGHT
#define CGOLEM_LIGHT

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Light : public CBaseGolem , public IListener
{

public:
	CGolem_Light(void);
	CGolem_Light(CBaseGolem* pGolem);
	~CGolem_Light(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
};
#endif
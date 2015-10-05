#ifndef CGOLEM_EARTH
#define CGOLEM_EARTH

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Earth : public CBaseGolem , public IListener
{

	void EarthGolemSetup();
public:
	CGolem_Earth(void);
	CGolem_Earth(CBaseGolem* pGolem);
	~CGolem_Earth(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
};
#endif

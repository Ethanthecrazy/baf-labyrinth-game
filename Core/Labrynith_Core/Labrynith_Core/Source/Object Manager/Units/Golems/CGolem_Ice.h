#ifndef CGOLEM_ICE
#define CGOLEM_ICE

#include "../CBaseGolem.h"
#include "../../../Messaging/IListener.h"

class CGolem_Ice : public CBaseGolem , public IListener
{

	void IceGolemSetup();
public:
	CGolem_Ice(void);
	CGolem_Ice(CBaseGolem* pGolem);
	~CGolem_Ice(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void UpdateAI();
	void HandleEvent( Event* _toHandle );
};
#endif
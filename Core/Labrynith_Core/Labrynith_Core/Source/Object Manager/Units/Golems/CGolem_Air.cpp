#include "CGolem_Air.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"

CGolem_Air::CGolem_Air(void)
{
	//basic Air golem setup
	CBaseGolem::CBaseGolem();
	SetMoveType(RAND_MOVE);
	SetGolemType(AIR_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/AirGolem.png" ));
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Air::CGolem_Air(CBaseGolem* pGolem)
{
	//basic Air golem setup
	CGolem_Air::CGolem_Air();
}
CGolem_Air::~CGolem_Air(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
}

void CGolem_Air::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();
}
void CGolem_Air::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
}
bool CGolem_Air::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Air Golem specific Collisions
	return false;
}
bool CGolem_Air::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Air Golem specific Collisions
	return false;
}
void CGolem_Air::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Air::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Air Golem responds to
	if( _toHandle->GetEventID() == "ATTRACTORREMOVED" )
	{
		//start moving randomly again
		SetMoveType(RAND_MOVE);
	}
}
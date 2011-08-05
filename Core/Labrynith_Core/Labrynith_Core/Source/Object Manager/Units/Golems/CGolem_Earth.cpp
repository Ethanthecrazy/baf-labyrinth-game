#include "CGolem_Earth.h"
#include <iostream>
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"

CGolem_Earth::CGolem_Earth(void)
{
	//basic earth golem setup
	CBaseGolem::CBaseGolem();
	SetGolemType(EARTH_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/StoneGolem.png" ));
	cout << "Earth Golem class made" << endl;
}
CGolem_Earth::CGolem_Earth(CBaseGolem* pGolem)
{
	//basic earth golem setup
	CGolem_Earth::CGolem_Earth();
}
CGolem_Earth::~CGolem_Earth(void)
{
	CBaseGolem::~CBaseGolem();
}

void CGolem_Earth::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();
}
void CGolem_Earth::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
}
bool CGolem_Earth::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Earth Golem specific Collisions
	return false;
}
bool CGolem_Earth::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Earth Golem specific Collisions
	return false;
}
void CGolem_Earth::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Earth::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
}

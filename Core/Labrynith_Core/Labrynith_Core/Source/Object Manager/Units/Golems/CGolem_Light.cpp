#include "CGolem_Light.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../MObjectManager.h"
#include "../CBaseObject.h"

CGolem_Light::CGolem_Light(void)
{
	//basic Light golem setup
	CBaseGolem::CBaseGolem();
	SetGolemType(LIGHT_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/LightGolem.png" ));
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Light::CGolem_Light(CBaseGolem* pGolem)
{
	//basic Light golem setup
	CGolem_Light::CGolem_Light();
}
CGolem_Light::~CGolem_Light(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
}

void CGolem_Light::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();
}
void CGolem_Light::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);

	MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber )
		.GetFlake( OBJECT_LIGHT ).SetInfoAtIndex(GetIndexPosX(), GetIndexPosY(), rand() % 15 + 240 );
}
bool CGolem_Light::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Light Golem specific Collisions
	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			
		}
		break;

	case OBJECT_ENTITY:
		{
			//Entities cannot walk-thro other entities
			if(!nCanHandleCollision)
				return true;

			CBaseEntity* temp = (CBaseEntity*)pBase;
			if(temp->GetType() == ENT_GOLEM)
			{
				CBaseGolem* temp = (CBaseGolem*)pBase;
			}
			return true;
		}
		break;
	};
	return false;
}
bool CGolem_Light::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Light Golem specific Collisions
	return false;
}
void CGolem_Light::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Light::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Light Golem responds to
}
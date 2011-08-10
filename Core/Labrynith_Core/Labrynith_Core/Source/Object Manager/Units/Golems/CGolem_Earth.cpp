#include "CGolem_Earth.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../MObjectManager.h"
#include "../CBaseObject.h"


void CGolem_Earth::EarthGolemSetup()
{
	SetGolemType(EARTH_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/StoneGolem.png" ));
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Earth::CGolem_Earth(void)
{
	CBaseGolem::CBaseGolem();
	//basic earth golem setup
	EarthGolemSetup();
}
CGolem_Earth::CGolem_Earth(CBaseGolem* pGolem)
{
	CBaseGolem::CBaseGolem();
	//basic earth golem setup
	EarthGolemSetup();
}
CGolem_Earth::~CGolem_Earth(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
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
	if(!pBase || pBase == this || !nCanHandleCollision)
		return false;

	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
	{
		if( pBase->GetType() != OBJ_DOOR || pBase->GetType() != OBJ_BUTTON || pBase->GetType() != OBJ_SPAWNER || pBase->GetType() != OBJ_EXIT && nCanHandleCollision )
		{
			if( !nCanHandleCollision )
				return Collided;

			switch( GetFlag_DirectionToMove() )
			{
			case FLAG_MOVE_LEFT:
				if( pBase->GetFlag_MovementState() != FLAG_MOVESTATE_MOVING)
				{
					MObjectManager::GetInstance()->MoveEntLeft( pBase->m_nIdentificationNumber ) ;
					
				}
				break;
			case FLAG_MOVE_UP:
				if( pBase->GetFlag_MovementState() != FLAG_MOVESTATE_MOVING)
				{
					MObjectManager::GetInstance()->MoveEntUp( pBase->m_nIdentificationNumber ) ;
				}
				break;
			case FLAG_MOVE_RIGHT:
				if( pBase->GetFlag_MovementState() != FLAG_MOVESTATE_MOVING)
				{
					MObjectManager::GetInstance()->MoveEntRight( pBase->m_nIdentificationNumber ) ;
				}
				break;
			case FLAG_MOVE_DOWN:
				if( pBase->GetFlag_MovementState() != FLAG_MOVESTATE_MOVING)
				{
					MObjectManager::GetInstance()->MoveEntDown( pBase->m_nIdentificationNumber ) ;
				}
				break;
			}
		}
		return Collided;
	}

	//Do Earth Golem specific Collisions
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
				switch(temp->GetGolemType())
				{
				case FIRE_GOLEM:
					{
						if(nCanHandleCollision)
						{
							//turn me into an Lava Golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(this, LAVA_GOLEM));
							//Get rid of the Fire golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(temp->m_nIdentificationNumber));
						}						
					}
					break;
				};
			}
			return true;
		}
		break;
	};
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
	//Events only the Earth Golem responds to
}

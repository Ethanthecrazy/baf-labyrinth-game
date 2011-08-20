#include "CGolem_Earth.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../MObjectManager.h"
#include "../Tiles/CWaterTile.h"
#include "../CBaseObject.h"
#include "../Objects/CSteamPuff.h"


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
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
	{
		if( pBase->GetType() != OBJ_DOOR && pBase->GetType() != OBJ_BUTTON && pBase->GetType() != OBJ_SPAWNER && pBase->GetType() != OBJ_EXIT && nCanHandleCollision )
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
	case OBJECT_TILE:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			if( temp->GetType() == OBJ_WATER )
			{
				if(((CWaterTile*)temp)->IsFrozen())
				{
					return false;
				}
				return true;
			}
		}
		break;

	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			
		}
		break;

	case OBJECT_ENTITY:
		{
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
							int tileid = MObjectManager::GetInstance()->FindLayer(temp->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(temp->GetIndexPosX(), temp->GetIndexPosY());

							temp->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);
							int* newID = new int;						
							//turn me into an Lava Golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(temp, LAVA_GOLEM, newID));
							//Get rid of the Fire golem							
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveGolemCombined(this->m_nIdentificationNumber, newID));

							CSteamPuff* toAdd = new CSteamPuff();
							toAdd->MakeFire();
							toAdd->SetPosX( ( ( GetPosX() ) + ( temp->GetPosX() ) ) / 2);
							toAdd->SetPosY( ( ( GetPosY() ) + ( temp->GetPosY() ) ) / 2);
							toAdd->SetIndexPosX( GetIndexPosX() );
							toAdd->SetIndexPosY( GetIndexPosY() );
							MObjectManager::GetInstance()->AddUnit( toAdd, MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetLayerID() );
						}	
						//Trick the AI into thinking it can walk tho
						//this entity, however when it actully tries to
						//it wont be able to walk past it
						return nCanHandleCollision;
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

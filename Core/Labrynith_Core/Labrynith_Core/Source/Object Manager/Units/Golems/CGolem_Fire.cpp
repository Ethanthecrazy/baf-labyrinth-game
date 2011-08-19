#include "CGolem_Fire.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../MObjectManager.h"
#include "../../../Animation Manager/CAnimationManager.h"
#include "../Objects/COil.h"
#include "../CPlayer.h"
#include "../Tiles/CWaterTile.h"
#include "../Objects/CAttractor.h"
#include "../Objects/CSteamPuff.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/COptionsState.h"

CGolem_Fire::CGolem_Fire(void)
{
	//basic Fire golem setup
	CBaseGolem::CBaseGolem();
	SetGolemType(FIRE_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/FireGolem.png" ));
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Fire::CGolem_Fire(CBaseGolem* pGolem)
{
	//basic Fire golem setup
	CGolem_Fire::CGolem_Fire();
}
CGolem_Fire::~CGolem_Fire(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
}

void CGolem_Fire::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();
	CheckIceTile();
	// check surrounding objects to see if they can catch on fire
	for( int i = -1 ; i <= 1 ; ++i )
	{
		for( int u = -1 ; u <= 1 ; ++u )
		{
			if( ( i == -1 && u != 0 ) || ( i == 1 && u != 0 ) || ( u == -1 && i != 0 ) || ( u == 1 && i != 0 ) )
					continue ;
			int item = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( this->GetIndexPosX() + i , this->GetIndexPosY() + u ) ;
			IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
			if( object )
			{
				if( object->GetType() == OBJ_OIL )
				{
					((COil*)object)->SetOnFire(true) ;
				}
			}
			int entityID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( this->GetIndexPosX() + i , this->GetIndexPosY() + u ) ;
			IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
			if( !entity )
				continue;
			if( entity->GetType() == ENT_PLAYER )
			{
				((CPlayer*)entity)->SetLives( ((CPlayer*)entity)->GetLives() - 1 ) ;
			}
		}
	}

}
void CGolem_Fire::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);

	MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber )
		.GetFlake( OBJECT_LIGHT ).SetInfoAtIndex(GetIndexPosX(), GetIndexPosY(), rand() % 15 + 240 );
}
bool CGolem_Fire::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Fire Golem specific Collisions
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

	case OBJECT_ENTITY:
		{
			CBaseEntity* temp = (CBaseEntity*)pBase;
			if(temp->GetType() == ENT_GOLEM)
			{
				CBaseGolem* temp = (CBaseGolem*)pBase;
				switch(temp->GetGolemType())
				{
				case EARTH_GOLEM:
					{
						if(nCanHandleCollision)
						{
							int tileid = MObjectManager::GetInstance()->FindLayer(temp->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(temp->GetIndexPosX(), temp->GetIndexPosY());

							temp->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);
							int* newID = new int;
							//turn me into an Lava Golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(temp, LAVA_GOLEM, newID));
							//Get rid of the Earth golem		
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveGolemCombined(this->m_nIdentificationNumber, newID));						
						}	
						//Trick the AI into thinking it can walk tho
						//this entity, however when it actully tries to
						//it wont be able to walk past it
						return nCanHandleCollision;
					}
					break;

				case WATER_GOLEM:
					{
						if(nCanHandleCollision)
						{
							//Check exit collision before removing the objects
							int tileid = MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(this->GetIndexPosX(), this->GetIndexPosY());

							this->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);


							tileid = 0;
							tileid = MObjectManager::GetInstance()->FindLayer(temp->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(temp->GetIndexPosX(), temp->GetIndexPosY());

							temp->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);

							//Get rid of the Water golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(temp->m_nIdentificationNumber));
							//Get rid of this the Fire Golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(this->m_nIdentificationNumber));

							CSteamPuff* toAdd = new CSteamPuff();

							toAdd->SetPosX( ( ( GetPosX() ) + ( temp->GetPosX() ) ) / 2 - 64 );
							toAdd->SetPosY( ( ( GetPosY() ) + ( temp->GetPosY() ) ) / 2 );
							toAdd->SetIndexPosX( GetIndexPosX() );
							toAdd->SetIndexPosY( GetIndexPosY() );

							MObjectManager::GetInstance()->AddUnit( toAdd, MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetLayerID() );
						}	
						//It thinks it can walk thro the golem
						return false;
					}
					break;

				case ICE_GOLEM:
					{
						if(nCanHandleCollision)
						{
							int tileid = MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(this->GetIndexPosX(), this->GetIndexPosY());

							this->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);
							//Change the Ice Golem into a Water Golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(temp, WATER_GOLEM));
							//Get rid of this the Fire Golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(this->m_nIdentificationNumber));
							

							CSteamPuff* toAdd = new CSteamPuff();

							toAdd->SetPosX( ( ( GetPosX() ) + ( temp->GetPosX() ) ) / 2 - 64 );
							toAdd->SetPosY( ( ( GetPosY() ) + ( temp->GetPosY() ) ) / 2 - 16 );
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
void CGolem_Fire::ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this || this->GetLayerLocation() != pBase->GetLayerLocation())
		return;

	CBaseGolem::ExitCollision(pBase, nCanHandleCollision);

	switch(pBase->m_nUnitType)
	{
	case OBJECT_TILE:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			if( temp->GetType() == OBJ_WATER )
			{
				if(((CWaterTile*)temp)->IsFrozen())
				{
					if(nCanHandleCollision)
					{
						//if its ice, melt it
						((CWaterTile*)temp)->SetIsFrozen(false);
					}
				}
				else
				{
					if(nCanHandleCollision)
					{
						//if its water, get rid of this the Fire Golem
						MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(this->m_nIdentificationNumber));
					}
				}
			}
		}
		break;
	};
}
bool CGolem_Fire::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Fire Golem specific Collisions
	return false;
}
bool CGolem_Fire::CanInteract(IUnitInterface* pBase)
{
	if(!pBase)
		return false;

	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			return false;
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
				case ICE_GOLEM:
					{
						return false;
					}
					break;
				};
			}
		}
		break;
	};
	return false;
}
void CGolem_Fire::CheckIceTile()
{
	//If our target has been cleared
	if(!HasTarget())
	{		
		//if we are on an ice tile, turn it into water
		MObjectManager* OM = MObjectManager::GetInstance();
		int tile = OM->FindLayer(this->m_nIdentificationNumber).GetFlake(OBJECT_TILE)
			.GetInfoAtIndex(this->GetIndexPosX(), this->GetIndexPosY());
		IUnitInterface* unit = OM->GetUnit(tile);
		CBaseObject* obj;
		//if the object isnt valid leave
		if(!unit)
			return;

		if(unit->m_nUnitType == OBJECT_TILE)
		{
			obj = (CBaseObject*)unit;
			if(obj->GetType() == OBJ_WATER)
			{
				((CWaterTile*)obj)->SetIsFrozen(false);
				//and call exitcollision to get rid of the fire golem
				this->ExitCollision(obj, true);
			}
		}
	}
}
void CGolem_Fire::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Fire::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Fire Golem responds to
}

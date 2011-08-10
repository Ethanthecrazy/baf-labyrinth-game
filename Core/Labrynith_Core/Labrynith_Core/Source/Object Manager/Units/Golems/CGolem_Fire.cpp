#include "CGolem_Fire.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../MObjectManager.h"
#include "../Objects/COil.h"
#include "../CPlayer.h"
#include "../Tiles/CWaterTile.h"

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
			//Entities cannot walk-thro other entities
			if(!nCanHandleCollision)
				return true;

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
							//turn me into an Lava Golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(this, LAVA_GOLEM));
							//Get rid of the Earth golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(temp->m_nIdentificationNumber));
						}						
					}
					break;

				case WATER_GOLEM:
					{
						if(nCanHandleCollision)
						{
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
						}						
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
void CGolem_Fire::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Fire::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Fire Golem responds to
}

#include "CGolem_Ice.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../Tiles/CWaterTile.h"
#include "../../../AI Handler/CAI_Handler.h"
#include "../../MObjectManager.h"
#include "../Objects/CSteamPuff.h"

void CGolem_Ice::IceGolemSetup()
{
	//basic Ice golem setup
	SetGolemType(ICE_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IceGolem.png" ));
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Ice::CGolem_Ice(void)
{
	CBaseGolem::CBaseGolem();
	//basic Ice golem setup
	IceGolemSetup();
}
CGolem_Ice::CGolem_Ice(CBaseGolem* pGolem)
{
	CBaseGolem::CBaseGolem();
	//basic Ice golem setup
	IceGolemSetup();
	//copy its positions
	this->SetIndexPosX(pGolem->GetIndexPosX());
	this->SetIndexPosY(pGolem->GetIndexPosY());
	this->SetPosX(pGolem->GetPosX());
	this->SetPosY(pGolem->GetPosY());
	this->SetLastPosX(pGolem->GetLastPosX());
	this->SetLastPosY(pGolem->GetLastPosY());
	//copy its states
	this->SetFlag_DirectionToMove(pGolem->GetFlag_DirectionToMove());
	this->SetFlag_MovementState(pGolem->GetFlag_MovementState());
}
CGolem_Ice::~CGolem_Ice(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
}

void CGolem_Ice::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();
}
void CGolem_Ice::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
}
bool CGolem_Ice::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this || this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Ice Golem specific Collisions
	switch(pBase->m_nUnitType)
	{
	case OBJECT_TILE:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			if( temp->GetType() == OBJ_WATER )
			{
				if(!((CWaterTile*)temp)->IsFrozen())
				{
					if(nCanHandleCollision)
					{
					//if its water, freeze it
					((CWaterTile*)temp)->SetIsFrozen(true);
					}
				}
				return false;
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
				case FIRE_GOLEM:
					{
						if(nCanHandleCollision)
						{
							int tileid = MObjectManager::GetInstance()->FindLayer(temp->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(temp->GetIndexPosX(), temp->GetIndexPosY());

							temp->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);
							//turn me into a water golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(this, WATER_GOLEM));
							//Get rid of the Fire Golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(temp->m_nIdentificationNumber));

						CSteamPuff* toAdd = new CSteamPuff();

							toAdd->SetPosX( ( ( GetPosX() ) + ( temp->GetPosX() ) ) / 2 - 64 );
							toAdd->SetPosY( ( ( GetPosY() ) + ( temp->GetPosY() ) ) / 2 - 16 );
							toAdd->SetIndexPosX( GetIndexPosX() );
							toAdd->SetIndexPosY( GetIndexPosY() );

							MObjectManager::GetInstance()->AddUnit( toAdd, MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetLayerID() );
						

						}
					}
					break;

				case WATER_GOLEM:
					{
						if(nCanHandleCollision)
						{
						//turn the Water golem into an Ice Golem
						MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(temp, ICE_GOLEM));
						}
					}
					break;

				case LAVA_GOLEM:
					{
						if(nCanHandleCollision)
						{
							int tileid = MObjectManager::GetInstance()->FindLayer(temp->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(temp->GetIndexPosX(), temp->GetIndexPosY());

							temp->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);
							//turn the Lava golem into an Iron Golem
							int* newID = new int;
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(temp, IRON_GOLEM, newID));
							//Get rid of this the Ice Golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveGolemCombined(this->m_nIdentificationNumber, newID));

						CSteamPuff* toAdd = new CSteamPuff();

							toAdd->SetPosX( ( ( GetPosX() ) + ( temp->GetPosX() ) ) / 2 - 64 );
							toAdd->SetPosY( ( ( GetPosY() ) + ( temp->GetPosY() ) ) / 2 - 16 );
							toAdd->SetIndexPosX( GetIndexPosX() );
							toAdd->SetIndexPosY( GetIndexPosY() );

							MObjectManager::GetInstance()->AddUnit( toAdd, MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetLayerID() );
						
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
bool CGolem_Ice::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Ice Golem specific Collisions
	return false;
}
void CGolem_Ice::UpdateAI()
{
	CBaseGolem::UpdateAI();
	CAI_Handler::GetInstance()->CheckCollisionRange(this, 1);
}
void CGolem_Ice::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Ice Golem responds to
}
bool CGolem_Ice::CanInteract(IUnitInterface* pBase)
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
				case FIRE_GOLEM:
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

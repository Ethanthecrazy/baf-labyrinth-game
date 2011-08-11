#include "CGolem_Water.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../Tiles/CWaterTile.h"
#include "../../MObjectManager.h"
#include "../Tiles/CMetal.h"
#include "../Tiles/CElectricButton.h"
#include "CGolem_Iron.h"
#include "../../../Animation Manager/CAnimationManager.h"

void CGolem_Water::WaterGolemSetup()
{
	SetGolemType(WATER_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/WaterGolem.png" ));
	SetElectricUpdateTimer( .5f ) ;
	SetPowered( false ) ;
	m_nAnimID = CAnimationManager::GetInstance()->GetID( "Electricity" ) ;
	m_nAnimImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/electricity.png") ;
	CAnimationManager::GetInstance()->SetAnimTexture( m_nAnimID , m_nAnimImageID ) ;
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
	MEventSystem::GetInstance()->RegisterClient("CIRCUTBROKEN" , this ) ;
}
CGolem_Water::CGolem_Water(void)
{
	CBaseGolem::CBaseGolem();
	//basic Water golem setup
	WaterGolemSetup();
}
CGolem_Water::CGolem_Water(CBaseGolem* pGolem)
{
	CBaseGolem::CBaseGolem();
	//basic Water golem setup
	WaterGolemSetup();
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
CGolem_Water::~CGolem_Water(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
	MEventSystem::GetInstance()->UnregisterClient("CIRCUTBROKEN" , this ) ;
}

void CGolem_Water::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();

	if( GetFlag_MovementState() == FLAG_MOVESTATE_ATDESTINATION )
	{
		//SetPowered( false ) ;
		// check to see if the golem is a connection
		//MEventSystem::GetInstance()->SendEvent( "CIRCUTBROKEN" ) ;
	}
		
	if( GetIsElectrified() )
	{
		CAnimationManager::GetInstance()->UpdateAnimation( fDT , m_nAnimID ) ;
		SetElectricUpdateTimer( GetElectricUpdateTimer() - fDT ) ;
	}
}
void CGolem_Water::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
	if( GetIsElectrified() )
		CAnimationManager::GetInstance()->Draw(m_nAnimID, (int)(GetPosX() - CameraPosX), (int)(GetPosY() - CameraPosY),
		0.2f, 0.2f, 0 , 0 , 0 , 0xffffffff ) ;
}
bool CGolem_Water::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Water Golem specific Collisions
	switch(pBase->m_nUnitType)
	{
	case OBJECT_TILE:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			if( temp->GetType() == OBJ_WATER )
			{
				if(((CWaterTile*)temp)->IsFrozen())
				{
					//turn me into an ice golem
					MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(this, ICE_GOLEM));
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
							int tileid = MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(this->GetIndexPosX(), this->GetIndexPosY());

							this->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);


							tileid = 0;
							tileid = MObjectManager::GetInstance()->FindLayer(temp->m_nIdentificationNumber)
												.GetFlake(OBJECT_TILE).GetInfoAtIndex(temp->GetIndexPosX(), temp->GetIndexPosY());

							temp->ExitCollision(MObjectManager::GetInstance()->GetUnit(tileid), nCanHandleCollision);

							//get rid of the fire golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(temp->m_nIdentificationNumber));
							//get rid of this the water golem
							MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(this->m_nIdentificationNumber));
						}
					}
					break;

				case ICE_GOLEM:
					{
						if(nCanHandleCollision)
						{
							//turn me into an Ice Golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(this, ICE_GOLEM));
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

							//turn me into an Iron Golem
							MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(this, IRON_GOLEM));
							//Get rid of the Lava golem
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
bool CGolem_Water::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Water Golem specific Collisions
	return false;
}
void CGolem_Water::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Water::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Water Golem responds to
	if( _toHandle->GetEventID() == "CIRCUTBROKEN" )
	{
		SetPowered( false ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
}

void CGolem_Water::SetPowered( bool powered )
{
	if( GetIsElectrified() == false && powered == true )
	{
		CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
	else if( GetIsElectrified() == true && powered == false )
	{
		CAnimationManager::GetInstance()->StopAnimation( m_nAnimID ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
	m_bPowered = powered ;
}

 void CGolem_Water::SetFlag_MovementState( int newFlag )
 {
	 if( newFlag == FLAG_MOVESTATE_ATDESTINATION )
		MEventSystem::GetInstance()->SendEvent( "CIRCUTBROKEN" ) ;

	 CBaseGolem::SetFlag_MovementState( newFlag ) ;
 }
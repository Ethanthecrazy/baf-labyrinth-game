#include "CGolem_Iron.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../MObjectManager.h"
#include "../CBaseObject.h"
#include "../Tiles/CMetal.h"
#include "../Tiles/CElectricButton.h"
#include "CGolem_Water.h"
#include "../../../Animation Manager/CAnimationManager.h"
#include "../Tiles/CWaterTile.h"

void CGolem_Iron::IronGolemSetup()
{
	SetGolemType(IRON_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IronGolem.png" ));
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
CGolem_Iron::CGolem_Iron(void)
{
	CBaseGolem::CBaseGolem();
	//basic Iron golem setup
	IronGolemSetup();

}
CGolem_Iron::CGolem_Iron(CBaseGolem* pGolem)
{
	CGolem_Iron::CGolem_Iron();
	//basic Iron golem setup
	IronGolemSetup();
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
CGolem_Iron::~CGolem_Iron(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
	MEventSystem::GetInstance()->UnregisterClient("CIRCUTBROKEN" , this ) ;
}

void CGolem_Iron::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();

	if( GetFlag_MovementState() == FLAG_MOVESTATE_MOVING && GetIsElectrified() == true )
	{
		SetPowered( false ) ;
		// check to see if the golem is a connection
		//MEventSystem::GetInstance()->SendEvent( "CIRCUTBROKEN" ) ;
	}

	if( GetIsElectrified() )
	{
		CAnimationManager::GetInstance()->UpdateAnimation( fDT , m_nAnimID ) ;
		SetElectricUpdateTimer( GetElectricUpdateTimer() - fDT ) ;
	}
}
void CGolem_Iron::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
	if( GetIsElectrified() )
		CAnimationManager::GetInstance()->Draw(m_nAnimID , GetPosX() - CameraPosX , GetPosY() - CameraPosY , .2 , .2 , 0 , 0 , 0 , 0xffffffff ) ;
}
bool CGolem_Iron::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Iron Golem specific Collisions
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
bool CGolem_Iron::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Iron Golem specific Collisions
	return false;
}
void CGolem_Iron::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Iron::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Iron Golem responds to
	if( _toHandle->GetEventID() == "CIRCUTBROKEN" )
	{
		SetPowered( false ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
}

void CGolem_Iron::SetPowered( bool powered )
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

void CGolem_Iron::SetFlag_MovementState( int newFlag )
{
	 if( newFlag == FLAG_MOVESTATE_ATDESTINATION )
		MEventSystem::GetInstance()->SendEvent( "CIRCUTBROKEN" ) ;

	 CBaseGolem::SetFlag_MovementState( newFlag ) ;
}
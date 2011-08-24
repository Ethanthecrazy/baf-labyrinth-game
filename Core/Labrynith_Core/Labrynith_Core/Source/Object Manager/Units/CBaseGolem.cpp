#include "../MObjectManager.h"
#include "../../AI Handler/CAI_Handler.h"
#include "CBaseGolem.h"
#include "Objects\CAttractor.h"
#include "../../GameStates/CGamePlayState.h"
#include "../../GameStates/COptionsState.h"
#include "../../Animation Manager/CAnimationManager.h"
#include "../../Wrappers/CSGD_FModManager.h"
#include "../../Messaging/IListener.h"
#include "Tiles\CButton.h"
#include "Tiles\CDoor.h"
#include "Tiles\CElectricButton.h"

CBaseGolem::CBaseGolem(void)
{
	CBaseEntity::CBaseEntity();
	fCollectedTime = 0.0f;
	m_nType = ENT_GOLEM;
	ClearTarget();
	SetGolemType(-1);
	SetMoveType(TARGET_MOVE);
	m_nEatSoundID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/creature_snarl1.mp3" ) ;
	m_nStepSoundID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/footstepsGolem.mp3" ) ;
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(m_nEatSoundID, true);
	Opt->AdjustSound(m_nStepSoundID, true);
}
CBaseGolem::~CBaseGolem(void)
{
	//MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED" , this ) ;
	CBaseEntity::~CBaseEntity();
}

void CBaseGolem::Update(float fDT)
{
	CBaseEntity::Update(fDT);

	/*if( GetFlag_prev_MovementState() == FLAG_MOVESTATE_MOVING)
	{		
		if(this->GetFlag_MovementState() == FLAG_MOVESTATE_ATDESTINATION)
		{
		int objectID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( GetIndexPosX() , GetIndexPosY() ) ;
		IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(objectID)) ;
		
		CheckCollision( object , true );
		}
	}*/

	if(( GetDistanceLeft() <= 32 && GetDistanceLeft() >= 30 ) || (GetDistanceLeft() <= 16 && GetDistanceLeft() >= 14 ) )
		CSGD_FModManager::GetInstance()->PlaySound2D( m_nStepSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber) ;

	//UpdateAI();
}
void CBaseGolem::Render( int CameraPosX, int CameraPosY )
{
	CBaseEntity::Render(CameraPosX, CameraPosY);
}
bool CBaseGolem::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			if(pBase->GetType() == OBJ_ATTRACTOR )
			{
				if( ((CAttractor*)pBase)->GetElemType() != this->GetGolemType() )
						return true;

				if(nCanHandleCollision)
				{
					int cameraX = 0 , cameraY = 0 ;
					CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
					int tileXPos = (int)((pBase->GetPosX() + cameraX) / TILE_WIDTH) ;
					int tileYPos = (int)((pBase->GetPosY() + cameraY) / TILE_HEIGHT) ;

					int ObjectID = pBase->m_nIdentificationNumber;
					//int ObjectID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
					MEventSystem::GetInstance()->SendEvent( "ATTRACTORREMOVED" , MObjectManager::GetInstance()->GetUnit( ObjectID ) ) ;
					MObjectManager::GetInstance()->RemoveUnit( ObjectID ) ;
					MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).SetInfoAtIndex( tileXPos , tileYPos , 0 ) ;
					
					
					MObjectManager::GetInstance()->GetUnit( ObjectID )->SetVelY( 0 ) ;
					MObjectManager::GetInstance()->GetUnit( ObjectID )->SetVelX( 0 ) ;
					MObjectManager::GetInstance()->GetUnit( ObjectID )->SetDistanceLeft( 0 ) ;
					MObjectManager::GetInstance()->GetUnit( ObjectID )->SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );

					switch( GetFlag_DirectionToMove() )
					{
					case FLAG_MOVE_LEFT:
						SetAnimID( CAnimationManager::GetInstance()->GetID("EatLeft") ) ;
						break;
					case FLAG_MOVE_UP:
						SetAnimID( CAnimationManager::GetInstance()->GetID("EatUp") ) ;
						break;
					case FLAG_MOVE_RIGHT:
						SetAnimID( CAnimationManager::GetInstance()->GetID("EatRight") ) ;
						break;
					case FLAG_MOVE_DOWN:
						SetAnimID( CAnimationManager::GetInstance()->GetID("EatDown") ) ;
						break;
					}
					CAnimationManager::GetInstance()->PlayAnimation( GetCurrentAnimID() ) ;
					CSGD_FModManager::GetInstance()->PlaySound2D( m_nEatSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber ) ;
					
					//this->SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );
					MEventSystem::GetInstance()->SendEvent( "spawner.spawn" );
				}
				return false;
			}			
			else if( pBase->GetType() == OBJ_OIL )
				return pBase->CheckCollision(this, nCanHandleCollision);	
			else if( pBase->GetType() == OBJ_LIGHTORB || pBase->GetType() == OBJ_POWERGLOVES || pBase->GetType() == OBJ_OILCAN )
				return true;
		}
		break;

	case OBJECT_ENTITY:
		{
			return CheckEntCollision((CBaseEntity*)pBase);
		}
		break;

		case OBJECT_TILE:
		{
			if(pBase->GetType() == OBJ_ELECTRICBUTTON)
			{				
				((CElectricButton*)pBase)->CheckCollision(this, nCanHandleCollision);
				return false;
			}
			else if( pBase->GetType() == OBJ_DOOR )
			{
				return !((CDoor*)pBase)->GetIsOpen();
			}
			else if(pBase->GetType() == OBJ_EXIT)
			{
				return true;
			}
			else if( pBase->GetType() == OBJ_PIT )
				return false;		
			else if( pBase->GetType() == OBJ_RAMP )
				return false;
			else if(pBase->GetType() == OBJ_ELECTRICGENERATOR)
				return true;
		}
		break;
	};
	return false;
}
void CBaseGolem::ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if( !pBase || pBase == this || pBase->GetLayerLocation() != this->GetLayerLocation() )
		return;

	switch(pBase->m_nUnitType)
	{
	case OBJECT_TILE:
		{			
			if( pBase->GetType() == OBJ_BUTTON && nCanHandleCollision )
			{
				((CButton*)pBase)->CheckCollision(this);
			}
			else if(pBase->GetType() == OBJ_ELECTRICBUTTON)
			{				
				((CElectricButton*)pBase)->CheckCollision(this, nCanHandleCollision);
			}
		}
		break;
	};
}
bool CBaseGolem::CheckTileCollision(int TileID)
{
	switch(TileID)
	{
	//nothing is there
	case -1:
		{
			return true;
		}
		break;
	case 0:
		{
			return true;
		}
	};
	return false;
}
void CBaseGolem::UpdateAI()
{
	switch(GetMoveType())
	{
	case RAND_MOVE:
		{
			CAI_Handler::GetInstance()->RandomMove(this);
		}
		break;
	case TARGET_MOVE:
		{
			CAI_Handler::GetInstance()->MoveToPos(this);
		}
		break;
	}
}
bool CBaseGolem::CanInteract(IUnitInterface* pBase)
{
	return false;
}

int CBaseGolem::GetGolemType() const
{
	return m_nGolemType;
}
int CBaseGolem::GetMoveType() const
{
	return m_nMovementType;
}
void CBaseGolem::SetGolemType(const int nGolemType)
{
	m_nGolemType = nGolemType; 
}
void CBaseGolem::SetMoveType(const int nMovementType)
{
	m_nMovementType = nMovementType;

}

void CBaseGolem::HandleEvent( Event* _toHandle )
{
	//include all the events all the golems respond to
	if( _toHandle->GetEventID() == "ATTRACTORPLACED" )
	{
		CAttractor* placedAttr = (CAttractor*)_toHandle->GetParam() ;
		if( placedAttr->GetElemType() == this->GetGolemType() )
		{								
			if(this->GetLayerLocation() != placedAttr->GetLayerLocation())
				return;

			//start moving toward our target
			SetTargetPos( placedAttr->GetIndexPosX() , placedAttr->GetIndexPosY() ) ;
			SetMoveType(TARGET_MOVE);
		}
	}
	else if( _toHandle->GetEventID() == "ATTRACTORREMOVED" )
	{
		CAttractor* attr = (CAttractor*)_toHandle->GetParam() ;
		if( !attr )
			return;
		
		// ATTRACTOR REMOVED ON DIFFERENT LEVEL?
		if( GetTargetPosX() == attr->GetIndexPosX() && GetTargetPosY() == attr->GetIndexPosY() )
		{
			ClearTarget();
		}
	}
}

bool CBaseGolem::CheckEntCollision(CBaseEntity* pEntity)
{
	if(pEntity->GetType() == OBJ_ATTRACTOR )
	{
		int cameraX = 0 , cameraY = 0 ;
		CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
		int tileXPos = (int)((pEntity->GetPosX() + cameraX) / TILE_WIDTH) ;
		int tileYPos = (int)((pEntity->GetPosY() + cameraY) / TILE_HEIGHT) ;

		int ObjectID = pEntity->m_nIdentificationNumber ;
		//int ObjectID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
		MEventSystem::GetInstance()->SendEvent( "ATTRACTORREMOVED" , MObjectManager::GetInstance()->GetUnit( ObjectID ) ) ;
		MObjectManager::GetInstance()->RemoveUnit( ObjectID ) ;
		MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).SetInfoAtIndex( tileXPos , tileYPos , 0 ) ;


		switch( GetFlag_DirectionToMove() )
		{
		case FLAG_MOVE_LEFT:
			SetAnimID( CAnimationManager::GetInstance()->GetID("EatLeft") ) ;
			break;
		case FLAG_MOVE_UP:
			SetAnimID( CAnimationManager::GetInstance()->GetID("EatUp") ) ;
			break;
		case FLAG_MOVE_RIGHT:
			SetAnimID( CAnimationManager::GetInstance()->GetID("EatRight") ) ;
			break;
		case FLAG_MOVE_DOWN:
			SetAnimID( CAnimationManager::GetInstance()->GetID("EatDown") ) ;
			break;
		}
		CAnimationManager::GetInstance()->PlayAnimation( GetCurrentAnimID() ) ;
		CSGD_FModManager::GetInstance()->PlaySound2D( m_nEatSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber ) ;
		MEventSystem::GetInstance()->SendEvent( "spawner.spawn" );

		return true;
	}

	return false;
}

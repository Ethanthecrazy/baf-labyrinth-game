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

	if(( GetDistanceLeft() <= 32 && GetDistanceLeft() >= 30 ) || (GetDistanceLeft() <= 16 && GetDistanceLeft() >= 14 ) )
		CSGD_FModManager::GetInstance()->PlaySoundA( GetStepSoundID() ) ;

	fCollectedTime += fDT;
	if(fCollectedTime > 1.0f)
	{
		
		fCollectedTime = 0.0f;
	}
	UpdateAI();
}
void CBaseGolem::Render( int CameraPosX, int CameraPosY )
{
	CBaseEntity::Render(CameraPosX, CameraPosY);
}
bool CBaseGolem::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;
	
	if(this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			if(pBase->GetType() == OBJ_ATTRACTOR )
			{
				if(nCanHandleCollision)
				{
					if( ((CAttractor*)pBase)->GetElemType() != this->GetGolemType() )
						return true ;

					
					
					int cameraX = 0 , cameraY = 0 ;
					CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
					int tileXPos = (int)((pBase->GetPosX() + cameraX) / 32.0f) ;
					int tileYPos = (int)((pBase->GetPosY() + cameraY) / 32.0f) ;

					int ObjectID = pBase->m_nIdentificationNumber ;
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
					CSGD_FModManager::GetInstance()->PlaySoundA( m_nEatSoundID ) ;
					MEventSystem::GetInstance()->SendEvent( "spawner.spawn" );
				}
				return false;
			}			
			else if( pBase->GetType() == OBJ_SPAWNER )
				return false;
			else if( pBase->GetType() == OBJ_OIL )
				return pBase->CheckCollision(this, nCanHandleCollision);	
		}
		break;
	case OBJECT_ENTITY:
		{
			return CheckEntCollision((CBaseEntity*)pBase);
		}
		break;

		case OBJECT_TILE:
		{
			if( pBase->GetType() == OBJ_BUTTON || pBase->GetType() == OBJ_ELECTRICBUTTON )
			{
				if(nCanHandleCollision)
				{
					((CButton*)pBase)->CheckCollision(this);
					return false;
				}
			}
			else if( pBase->GetType() == OBJ_DOOR )
			{
				if( ((CDoor*)pBase)->GetIsOpen() )
					return false;
				else
					return true;
			}
			else if(pBase->GetType() == OBJ_EXIT)
			{
				return true;
			}
			else if( pBase->GetType() == OBJ_PIT )
				return pBase->CheckCollision(this, nCanHandleCollision);		
			else if( pBase->GetType() == OBJ_RAMP )
				return pBase->CheckCollision(this, nCanHandleCollision);
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
			if( pBase->GetType() == OBJ_BUTTON || pBase->GetType() == OBJ_ELECTRICBUTTON )
			{
				if(nCanHandleCollision)
				{
					((CButton*)pBase)->CheckCollision(this);
				}
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
			return ;
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
		int tileXPos = (int)((pEntity->GetPosX() + cameraX) / 32.0f) ;
		int tileYPos = (int)((pEntity->GetPosY() + cameraY) / 32.0f) ;

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
		CSGD_FModManager::GetInstance()->PlaySoundA( m_nEatSoundID ) ;
		MEventSystem::GetInstance()->SendEvent( "spawner.spawn" );

		return true;
	}

	return false;
}

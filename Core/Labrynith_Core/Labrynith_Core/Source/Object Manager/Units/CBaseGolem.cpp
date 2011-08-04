#include "../MObjectManager.h"
#include "../../AI Handler/CAI_Handler.h"
#include "CBaseGolem.h"
#include "Objects\CAttractor.h"
#include "../../GameStates/CGamePlayState.h"
#include "../../Animation Manager/CAnimationManager.h"
#include "../../Wrappers/CSGD_FModManager.h"
CBaseGolem::CBaseGolem(void)
{
	CBaseEntity::CBaseEntity();
	fCollectedTime = 0.0f;
	m_nType = ENT_GOLEM;
	ClearTarget();
	ClearNewTarget();
	SetGolemType(-1);
	SetMoveType(RAND_MOVE);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED" , this ) ;
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED" , this ) ;
	m_nEatSoundID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/creature_snarl1.mp3" ) ;
}
CBaseGolem::~CBaseGolem(void)
{
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED" , this ) ;
	CBaseEntity::~CBaseEntity();
}

void CBaseGolem::Update(float fDT)
{
	CBaseEntity::Update(fDT);
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
//bool CBaseGolem::CheckCollision(IUnitInterface* pBase)
//{
//	switch(pBase->m_nUnitType)
//	{
//	case OBJECT_TILE:
//		{
//			return false;
//		}
//		break;
//	case OBJECT_OBJECT:
//		{
//			return true;
//		}
//		break;
//	case OBJECT_ENTITY:
//		{
//			return true;
//		}
//		break;
//	}
//	return false;
//}
void CBaseGolem::UpdateAI()
{
	//CAI_Handler::GetInstance()->RandomMove(this);
	CAI_Handler::GetInstance()->MoveToPos(this);
}
void CBaseGolem::ClearTarget()
{
	SetTargetPos(-1, -1);
}
bool CBaseGolem::HasTarget()
{
	if(GetTargetPosX() < 0 ||
		GetTargetPosY() < 0)
		return false;

	return true;
}

int CBaseGolem::GetTargetPosX() const
{
	return m_nTargetX;
}
int CBaseGolem::GetTargetPosY() const
{
	return m_nTargetY;
}
int CBaseGolem::GetGolemType() const
{
	return m_nGolemType;
}
void CBaseGolem::SetGolemType(const int nGolemType)
{
	m_nGolemType = nGolemType; 
}
void CBaseGolem::SetTargetPosX(const int nTargetX)
{
	if(nTargetX < -1)
		return;

	m_nTargetX = nTargetX;
}
void CBaseGolem::SetTargetPosY(const int nTargetY)
{
	if(nTargetY < -1)
		return;

	m_nTargetY = nTargetY;
}
void CBaseGolem::SetTargetPos(const int nTargetX, const int nTargetY)
{
	SetTargetPosX(nTargetX);
	SetTargetPosY(nTargetY);
}
void CBaseGolem::SetMoveType(const int nMovementType)
{
	m_nMovementType = nMovementType;

}
void CBaseGolem::SetNewTargetPosX(const int nTargetX)
{
	if(nTargetX < -1)
		return;

	m_nNewTargetX = nTargetX;
}
void CBaseGolem::SetNewTargetPosY(const int nTargetY)
{
	if(nTargetY < -1)
		return;

	m_nNewTargetY = nTargetY;
}
void CBaseGolem::SetNewTargetPos(const int nTargetX, const int nTargetY)
{
	SetNewTargetPosX(nTargetX);
	SetNewTargetPosY(nTargetY);
}
void CBaseGolem::ClearNewTarget()
{
	SetNewTargetPos(-1, -1);
}
bool CBaseGolem::HasNewTarget()
{
	if(m_nNewTargetX < 0 ||
	   m_nNewTargetY < 0)
		return false;

	return true;
}

void CBaseGolem::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "ATTRACTORPLACED" )
	{
		CAttractor* placedAttr = (CAttractor*)_toHandle->GetParam() ;
		if( placedAttr->GetElemType() == this->GetGolemType() )
		{
			SetTargetPos( placedAttr->GetIndexPosX() , placedAttr->GetIndexPosY() ) ;
		}
	}
	else if( _toHandle->GetEventID() == "ATTRACTORREMOVED" )
	{
		CAttractor* attr = (CAttractor*)_toHandle->GetParam() ;
		if( !attr )
			return ;
		if( GetTargetPosX() == attr->GetIndexPosX() && GetTargetPosY() == attr->GetIndexPosY() )
		{
			ClearTarget() ;
		}
	}
}

bool CBaseGolem::CheckCollision(IUnitInterface* pBase)
{
	if(!pBase)
		return false;

	//if we collide with an object

	if(pBase->GetType() == ENT_ATTRACTOR )
	{
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

		return false;
	}
	return true;
}

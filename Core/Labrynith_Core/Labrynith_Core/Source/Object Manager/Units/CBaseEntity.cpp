#include "CBaseEntity.h"
#include "../../Wrappers/CSGD_TextureManager.h"
#include "../MObjectManager.h"
#include "../../Animation Manager/CAnimationManager.h"

CBaseEntity::CBaseEntity()
{
	m_nIdentificationNumber = 0 ;

	// position
	 SetPosX(0);
	 SetPosY(0);

	// velocity
	 SetVelX(0);
	 SetVelY(0);

	 m_nUnitType = OBJECT_ENTITY;

	 SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );

	 m_nImageID = -1;
	 SetAnimID(-1);
	 m_uiRefCount = 1;
	 SetPlayAnimWhileStill(false);
}

CBaseEntity::~CBaseEntity(void)
{

}

void CBaseEntity::Update(float fDT)
{	
	//MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).SetInfoAtIndex( GetIndexPosX(), GetIndexPosY(),  rand() % 15 + 240 );
	
	//If the animation isnt playing, play it
	if(!CAnimationManager::GetInstance()->IsAnimationPlaying(GetCurrentAnimID()))
		CAnimationManager::GetInstance()->PlayAnimation(GetCurrentAnimID());

	CAnimationManager::GetInstance()->UpdateAnimation(fDT, GetCurrentAnimID());

	static float temptimer = 0.0f;
	temptimer += fDT;
	if(temptimer > 5.0f)
	{
		temptimer = 0.0f;
	}

	if( GetFlag_MovementState() == FLAG_MOVESTATE_MOVING )
	{
		SetDistanceLeft( GetDistanceLeft() - 125 * fDT );
		if( GetDistanceLeft() > 0 )
		{
			switch ( GetFlag_DirectionToMove() )
			{
			case FLAG_MOVE_RIGHT:
				SetPosX( GetPosX() + 125 * fDT );
				break;
			case FLAG_MOVE_LEFT:
				SetPosX( GetPosX() - 125 * fDT );
				break;
			case FLAG_MOVE_UP:
				SetPosY( GetPosY() - 125 * fDT );				
				break;
			case FLAG_MOVE_DOWN:
				SetPosY( GetPosY() + 125 * fDT );
				break;
			}
		}
		else
		{
			switch ( GetFlag_DirectionToMove() )
			{
			case FLAG_MOVE_RIGHT:
				SetPosX( GetLastPosX() + 32 );
				break;
			case FLAG_MOVE_LEFT:
				SetPosX( GetLastPosX() - 32 );
				break;
			case FLAG_MOVE_UP:
				SetPosY( GetLastPosY() - 32 );
				break;
			case FLAG_MOVE_DOWN:
				SetPosY( GetLastPosY() + 32 );
				break;
			}

			SetLastPosX( GetPosX() );
			SetLastPosY( GetPosY() );

			MObjectManager::GetInstance()->FindFlake( this->m_nIdentificationNumber ).FinishMovingEnt( this );
			SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );
		}
	}
	else
	{
		SetLastPosX( GetPosX() );
		SetLastPosY( GetPosY() );
		
		if(!PlayAnimWhileStill)
		{
			//When we are not moving stop the animation at the
			//first frame(which is idle) 
			CAnimationManager::GetInstance()->StopAnimationAtFrame(GetCurrentAnimID(), 0);
		}
		
	}
}

void CBaseEntity::Render( int CameraPosX, int CameraPosY )
{
	if( m_nImageID > -1 )
	{
		int DrawPositionX = (int)GetPosX() - CameraPosX;
		int DrawPositionY = (int)GetPosY() - CameraPosY;

		if(GetCurrentAnimID() <= -1)
		{
		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, DrawPositionX, DrawPositionY,
			1.0f,
			1.0f,
			0,
			0.0f,
			0.0f,
			0.0f,
			D3DCOLOR_ARGB( MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( GetIndexPosX(), GetIndexPosY() ), 255, 255, 255) );	
		}
		else
		{
			CAnimationManager::GetInstance()->Draw(GetCurrentAnimID(), DrawPositionX,
				DrawPositionY,
				1.0f,
				1.0f,
				0,
				0.0f,
				0.0f,
				D3DCOLOR_ARGB( MObjectManager::GetInstance()->
				FindLayer( m_nIdentificationNumber ).
				GetFlake( OBJECT_LIGHT ).
				GetInfoAtIndex( GetIndexPosX(), GetIndexPosY() ), 255, 255, 255) );
		}

	}
}

bool CBaseEntity::CheckCollision(IUnitInterface* pBase)
{
	return true;
}

void CBaseEntity::AddRef(void)
{
	++m_uiRefCount;
}

void CBaseEntity::Release(void)
{
	--m_uiRefCount;

	if (m_uiRefCount == 0)
		delete this;
}

void CBaseEntity::LoadEntMoveAnimIDs()
{
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("entity-movedown"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("entity-moveleft"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("entity-moveright"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("entity-moveup"));
	//down animation by default
	SetAnimID(m_vMovementAnimIDs[0]);
}

void CBaseEntity::SetAnimID(const int nAnimID)
{
	m_nCurrAnimID = nAnimID;
	//update animations texture to render with
	CAnimationManager::GetInstance()->SetAnimTexture(GetCurrentAnimID(), m_nImageID);
}
void CBaseEntity::SetImageID(const int nImageID)
{
	m_nImageID = nImageID;
	//update animations texture to render with
	CAnimationManager::GetInstance()->SetAnimTexture(GetCurrentAnimID(), m_nImageID);
}

void CBaseEntity::SetFlag_DirectionToMove( int newFlag )
{  
	//if it is the same direction no use doing work
	if( IUnitInterface::GetFlag_DirectionToMove() == newFlag )
		return;

	IUnitInterface::SetFlag_DirectionToMove(newFlag);
	
	//Change animation when direction changes
	switch(newFlag)
	{
	case FLAG_MOVE_RIGHT:
		{
			if(GetCurrentAnimID() > -1)
					SetAnimID(m_vMovementAnimIDs[2]);
			break;
		}
	case FLAG_MOVE_LEFT:
		{
			if(GetCurrentAnimID() > -1)
					SetAnimID(m_vMovementAnimIDs[1]);
			break;
		}
	case FLAG_MOVE_UP:
		{
			if(GetCurrentAnimID() > -1)
					SetAnimID(m_vMovementAnimIDs[3]);
			break;
		}
	case FLAG_MOVE_DOWN:
		{
			if(GetCurrentAnimID() > -1)
					SetAnimID(m_vMovementAnimIDs[0]);
			break;
		}
	}
}

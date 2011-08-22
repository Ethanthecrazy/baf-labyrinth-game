#include "CBaseEntity.h"
#include "../../Wrappers/CSGD_TextureManager.h"
#include "../MObjectManager.h"
#include "../../Animation Manager/CAnimationManager.h"
#include "../../AI Handler/CAI_Handler.h"
#include "../../GameStates/CGamePlayState.h"
#include "CBaseGolem.h"
#include "CPlayer.h"
#include "CBaseObject.h"
#include "../../Wrappers/CSGD_FModManager.h"
#include "../../GameStates/CSaveSlotState.h"
#include "../../CGame.h"
#include "../../GameStates/CLoadLevelState.h"
#include "Tiles\CDoor.h"
#include "Tiles\CButton.h"

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
	 SetAI_ID(-1);
	 m_uiRefCount = 1;
	 SetPlayAnimWhileStill(false);
	 LoadEntMoveAnimIDs();
}
CBaseEntity::~CBaseEntity(void)
{
	int tempthing = 0;
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
		int speed = 200;

		SetDistanceLeft( GetDistanceLeft() - speed * fDT );
		if( GetDistanceLeft() > 0 )
		{
			switch ( GetFlag_DirectionToMove() )
			{
			case FLAG_MOVE_RIGHT:
				SetPosX( GetPosX() + speed * fDT );
				break;
			case FLAG_MOVE_LEFT:
				SetPosX( GetPosX() - speed * fDT );
				break;
			case FLAG_MOVE_UP:
				SetPosY( GetPosY() - speed * fDT );				
				break;
			case FLAG_MOVE_DOWN:
				SetPosY( GetPosY() + speed * fDT );
				break;
			}
		}
		else
		{
			switch ( GetFlag_DirectionToMove() )
			{
			case FLAG_MOVE_RIGHT:
				SetPosX( GetLastPosX() + TILE_WIDTH );
				break;
			case FLAG_MOVE_LEFT:
				SetPosX( GetLastPosX() - TILE_WIDTH );
				break;
			case FLAG_MOVE_UP:
				SetPosY( GetLastPosY() - TILE_HEIGHT );
				break;
			case FLAG_MOVE_DOWN:
				SetPosY( GetLastPosY() + TILE_HEIGHT );
				break;
			}

			SetLastPosX( GetPosX() );
			SetLastPosY( GetPosY() );

			MObjectManager::GetInstance()->FindFlake( this->m_nIdentificationNumber ).FinishMovingEnt( this );
			SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );
			CheckAtDestinationCollision();
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

void CBaseEntity::CheckAtDestinationCollision()
{
	//Check to see if we are colliding with a tile
	int tileID = MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber)
		.GetFlake(OBJECT_TILE).GetInfoAtIndex(this->GetIndexPosX(), this->GetIndexPosY());

	CBaseObject* pBase = (CBaseObject*)(MObjectManager::GetInstance()->GetUnit(tileID));

	if(!pBase || this->GetLayerLocation() != pBase->GetLayerLocation())
		return;

	if( pBase->m_nIdentificationNumber > 0 )
	{
		if(pBase->GetType() == OBJ_EXIT)
		{
			CGamePlayState* pGamePlay = CGamePlayState::GetInstance();
			//BUG- check if the level is valid
			pGamePlay->SetCurrentLevel(pGamePlay->GetCurrentLevel() + 1);
			//Save the new current level we are on
			CSaveSlotState::GetInstance()->Save();
			//Load the next level
			CSGD_FModManager::GetInstance()->PlaySoundA(((CPlayer*)this)->GetLvlCompSoundID());
			CGame::GetInstance()->PushState(CLoadLevelState::GetInstance());
		}
		else if( pBase->GetType() == OBJ_PIT)
		{
			pBase->CheckCollision(this, true);		
		}
		else if( pBase->GetType() == OBJ_RAMP)
		{
			pBase->CheckCollision(this, true);	
		}
		else if( pBase->GetType() == OBJ_HELPER)
		{
			pBase->CheckCollision(this, true);	
		}
		else if( pBase->GetType() == OBJ_LIGHTEFFECTED)
		{
			pBase->CheckCollision(this, true);	
		}
		else if( pBase->GetType() == OBJ_DOOR)
		{
			((CDoor*)pBase)->CheckCollision(this);
		}
		else if( pBase->GetType() == OBJ_BUTTON )
		{
			((CButton*)pBase)->CheckCollision(this);
		}
	}
}
void CBaseEntity::Render( int CameraPosX, int CameraPosY )
{
	if( m_nImageID > -1 )
	{
		RECT camRect;
		camRect.top = (long)CameraPosY;
		camRect.left = (long)CameraPosX;
		camRect.right = (long)camRect.left + 1024;
		camRect.bottom = (long)camRect.top + 768;
		
		
		RECT objRect;
		objRect.top = (long)GetPosY();
		objRect.left = (long)GetPosX();
		objRect.bottom = (long)objRect.top + TILE_HEIGHT;
		objRect.right = (long)objRect.left + TILE_WIDTH;

		RECT out;
		if(!IntersectRect(&out, &camRect, &objRect) && CGamePlayState::GetInstance()->GetRenderCulling())
			return;

		int lightamount = MObjectManager::GetInstance()->GetLayer( this->GetLayerLocation() ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( GetIndexPosX(), GetIndexPosY() );
		

		int DrawPositionX = (int)GetPosX() - CameraPosX;
		int DrawPositionY = (int)GetPosY() - CameraPosY;

		if( m_nUnitType == OBJECT_ENTITY )
		{
			if( (((CBaseEntity*)(this)))->GetType() == ENT_GOLEM )
			{
				if( (((CBaseGolem*)(this)))->GetGolemType() == SHADOW_GOLEM )
				{
					lightamount = 255;
				}
			}
		}

		if( m_nUnitType == OBJECT_ENTITY )
		{
			if( (((CBaseEntity*)(this)))->GetType() == ENT_PLAYER )
			{
				if( lightamount < 32 )
					lightamount = 32;
			}
		}

		if(lightamount == 0 && CGamePlayState::GetInstance()->GetRenderCulling())
			return;

		if(GetCurrentAnimID() <= -1)
		{
			CSGD_TextureManager::GetInstance()->Draw( m_nImageID, DrawPositionX, DrawPositionY,
				1.0f,
				1.0f,
				0,
				0.0f,
				0.0f,
				0.0f,
				D3DCOLOR_ARGB( 255, lightamount, lightamount, lightamount) );	
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
				D3DCOLOR_ARGB( 255, lightamount, lightamount, lightamount) );
		}

	}
}
bool CBaseEntity::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if( pBase == this || pBase == NULL )
		return false ;

	return true;
}
void CBaseEntity::ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{

}
bool CBaseEntity::CheckTileCollision(int TileID)
{
	return false;
}
bool CBaseEntity::CanInteract(IUnitInterface* pBase)
{
	return false;
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
void CBaseEntity::ClearTarget()
{
	CAI_Handler::GetInstance()->ClearTargets(GetAI_ID());
	SetTargetPos(-1, -1);
}
bool CBaseEntity::HasTarget()
{
	if(GetTargetPosX() < 0 ||
		GetTargetPosY() < 0)
		return false;

	return true;
}
void CBaseEntity::AddRef(void)
{
	++m_uiRefCount;
}
void CBaseEntity::Release(void)
{
	--m_uiRefCount;

	if (m_uiRefCount <= 1)
		MMessageSystem::GetInstance()->SendMsg(new msgDeleteMe(this));
}

//accessors
int CBaseEntity::GetTargetPosX() const
{
	return m_nTargetX;
}
int CBaseEntity::GetTargetPosY() const
{
	return m_nTargetY;
}
int CBaseEntity::GetAI_ID() const
{
	return m_nAI_ID;
}
//mutators
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
void CBaseEntity::SetAI_ID(const int nAI_ID)
{
	m_nAI_ID = nAI_ID;
}
void CBaseEntity::SetTargetPosX(const int nTargetX)
{
	if(nTargetX < -1)
		return;

	m_nTargetX = nTargetX;
}
void CBaseEntity::SetTargetPosY(const int nTargetY)
{
	if(nTargetY < -1)
		return;

	m_nTargetY = nTargetY;
}
void CBaseEntity::SetTargetPos(const int nTargetX, const int nTargetY)
{
	SetTargetPosX(nTargetX);
	SetTargetPosY(nTargetY);
	CAI_Handler::GetInstance()->SetPrimaryTarget(this, GetTargetPosX(), GetTargetPosY());
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

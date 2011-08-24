#include "CPlayer.h"	
#include "../MObjectManager.h"
#include "../../AI Handler/CAI_Handler.h"
#include "../../GameStates/CGamePlayState.h"
#include "../../GameStates/CSaveSlotState.h"
#include "Tiles\CDoor.h"
#include "Tiles\CButton.h"
#include "Tiles\CWaterTile.h"
#include "../../GameStates/CLoadLevelState.h"
#include "../../GameStates/COptionsState.h"
#include "../../CGame.h"
#include "CBaseObject.h"
#include <iostream>
#include "../../Wrappers/CSGD_DirectInput.h"
#include "../../Wrappers/CSGD_Direct3D.h"
#include "../../Wrappers/CSGD_FModManager.h"
#include "../../Wrappers/CSGD_TextureManager.h"
#include "../../Animation Manager/CAnimationManager.h"
#include "Tiles\CMetal.h"
#include "Tiles\CElectricButton.h"

CPlayer::CPlayer(void)
{
	CBaseEntity::CBaseEntity();
	SetAnimState(ANIM_MOVING);
	LoadEntMoveAnimIDs();
	m_fInvincilibilityTimer = 0.0f;
	m_fRegenTimer = 0.0f;
	m_nType = ENT_PLAYER;
	SetHeldItem(NULL);
	SetEquippedItem(NULL);	
	HeartImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/heart.png" );
	//SetLives(4);	
	m_nLives = 4;
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	m_nPickUpSoundID = FM->LoadSound("resource/Sounds/pick-up.mp3" );
	m_nPutDownSoundID = FM->LoadSound("resource/Sounds/put-down.mp3" );
	m_nLvCompSoundID = FM->LoadSound("resource/Sounds/completeLevel.wav");
	HurtSoundID = FM->LoadSound("resource/Sounds/hurt.mp3");
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(m_nPickUpSoundID, true);
	Opt->AdjustSound(m_nPutDownSoundID, true);
	Opt->AdjustSound(m_nLvCompSoundID, true);
	Opt->AdjustSound(HurtSoundID, true);	
}
CPlayer::~CPlayer(void)
{
	CBaseEntity::~CBaseEntity();
	if(m_pHeldItem)
		delete m_pHeldItem;

	if(m_pEquippedItem)
		delete m_pEquippedItem;
}
void CPlayer::Update(float fDT)
{
	CBaseEntity::Update(fDT);
	UpdateInvincibilityTime(fDT);
	UpdateAnimState(fDT);
	Input();

	if( GetLives() < 4 )
		m_fRegenTimer += fDT;
	else
		m_fRegenTimer = 0.0f;

	if( m_fRegenTimer > 10.0f )
	{
		m_fRegenTimer = 0.0f;
		SetLives( GetLives() + 1 );
	}
}
void CPlayer::Render( int CameraPosX, int CameraPosY )
{	
	CSGD_TextureManager* TM = CSGD_TextureManager::GetInstance();

	//Player Lives

	int l;

	for( l = 0; l < GetLives(); l++)
	{
		TM->Draw(HeartImageID,(l * TILE_WIDTH) + 30, 40);
	}

	if( GetLives() < 4 )
	{
		RECT heart = {};
		heart.right = 32;
		heart.bottom = 32;
		heart.top = 32 - 32 * m_fRegenTimer / 10.0f;

		TM->Draw(HeartImageID,(l * TILE_WIDTH) + 30, 40 + heart.top, 1.0f, 1.0f, &heart, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( int( m_fRegenTimer / 10.0f * 255 ), 255, 255, 255  ));
	}

	if( GetHeldItem() )
	{
		if( GetHeldItem()->GetType() == OBJ_LIGHTORB )
		{
			MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber )
				.GetFlake( OBJECT_LIGHT ).SetInfoAtIndex(GetIndexPosX(), GetIndexPosY(), rand() % 15 + 200 );
		}
	}

	if( GetEquippedItem() )
	{
		if( GetEquippedItem()->GetType() == OBJ_LIGHTORB )
		{
			MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber )
				.GetFlake( OBJECT_LIGHT ).SetInfoAtIndex(GetIndexPosX(), GetIndexPosY(), rand() % 15 + 200 );
		}
	}

	RenderAnimState(CameraPosX, CameraPosY);

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( GetHeldItem() != NULL )
	{
		DrawMouseRange(1, GetHeldItem(), 255, 255, 255);
	}
	else if( GetEquippedItem() != NULL )
	{
		if( GetEquippedItem()->GetType() == OBJ_OILCAN )
		{
			DrawMouseRange(1, GetEquippedItem(), 255, 0, 0);
		}
	}

	//char temp[64];

	//sprintf( temp, "%f", m_fRegenTimer ); 

	//CSGD_Direct3D::GetInstance()->DrawTextA( temp, GetPosX() - CameraPosX, GetPosY() - CameraPosY );


}
void CPlayer::RenderAnimState( int CameraPosX, int CameraPosY )
{
	int state = GetAnimState();
	if(state == ANIM_MOVING)
	{
		CBaseEntity::Render(CameraPosX, CameraPosY);
		return;
	}

	int id = m_vMovementAnimIDs[GetStateAnimID()];
	CAnimationManager* AM = CAnimationManager::GetInstance();
	int DrawPositionX = (int)GetPosX() - CameraPosX;
	int DrawPositionY = (int)GetPosY() - CameraPosY;
	AM->Draw(id, DrawPositionX, DrawPositionY, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		D3DCOLOR_ARGB( 255, 255, 255, 255) );
}
void CPlayer::UpdateAnimState(float fDT)
{
	if(GetAnimState() == ANIM_MOVING)
		return;

	int animindex = GetStateAnimID();
	if(animindex > (int)m_vMovementAnimIDs.size() || animindex == -1)
		return;

	int id = m_vMovementAnimIDs[animindex];
	CAnimationManager* AM = CAnimationManager::GetInstance();
	AM->UpdateAnimation(fDT, id);

	if(!AM->IsAnimationPlaying(id))
	{
		//animation is done set back to norm
		SetAnimState(ANIM_MOVING);
	}
}
void CPlayer::DrawMouseRange(int nRange, CBaseObject* item, unsigned char red,
	unsigned char green, unsigned char blue)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	int cameraX = 0, cameraY = 0;
	CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
	int tileXPos = (int)((pDI->MouseGetPosX() + cameraX) / TILE_WIDTH) ;
	int tileYPos = (int)((pDI->MouseGetPosY() + cameraY) / TILE_HEIGHT) ;

	if( tileXPos >= GetIndexPosX() - nRange && tileXPos <= GetIndexPosX() + nRange && tileYPos >= GetIndexPosY() - nRange && tileYPos <= GetIndexPosY() + nRange )
	{
		int objectID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
		int entityID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( tileXPos , tileYPos ) ;
		int tileID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( tileXPos , tileYPos ) ;
		IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(objectID)) ;
		IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
		IUnitInterface* tile = (MObjectManager::GetInstance()->GetUnit(tileID)) ;
		if( !item->CheckCollision( object , false ) && !item->CheckCollision( entity , false ) && !item->CheckCollision( tile , false ) && tileID != 0 )
		{
			int xPos1 = tileXPos * TILE_WIDTH - cameraX ;
			int yPos1 = tileYPos * TILE_HEIGHT - cameraY ;
			CSGD_Direct3D::GetInstance()->DrawLine( xPos1 , yPos1 , xPos1 + TILE_HEIGHT , yPos1, red, green, blue);
			CSGD_Direct3D::GetInstance()->DrawLine( xPos1 + TILE_WIDTH , yPos1 , xPos1 + TILE_WIDTH , yPos1 + TILE_HEIGHT, red, green, blue);
			CSGD_Direct3D::GetInstance()->DrawLine( xPos1 + TILE_WIDTH , yPos1 + TILE_HEIGHT , xPos1 , yPos1 + TILE_HEIGHT, red, green, blue);
			CSGD_Direct3D::GetInstance()->DrawLine( xPos1 , yPos1 + TILE_HEIGHT , xPos1 , yPos1, red, green, blue);
		}
	}
}
void CPlayer::Input()
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CAI_Handler* AI = CAI_Handler::GetInstance();
	CAnimationManager* AM = CAnimationManager::GetInstance();

	if( pDI->KeyPressed( DIK_E ) || pDI->JoystickButtonPressed(1))
	{
		SwitchItems();
	}

	if( GetFlag_MovementState() == FLAG_MOVESTATE_ATDESTINATION )
	{
		if( pDI->KeyDown( DIK_W ) || 
			pDI->JoystickGetLStickDirDown(DIR_UP, 0))
		{
			AI->CardinalMove(this, FLAG_MOVE_UP);
			SetAnimState(ANIM_MOVING);
			return;
		}
		else if( pDI->KeyDown( DIK_S ) ||
			pDI->JoystickGetLStickDirDown(DIR_DOWN, 0))
		{
			AI->CardinalMove(this, FLAG_MOVE_DOWN);
			SetAnimState(ANIM_MOVING);
			return;
		}
		else if( pDI->KeyDown( DIK_A ) ||
			pDI->JoystickGetLStickDirDown(DIR_LEFT, 0))
		{
			AI->CardinalMove(this, FLAG_MOVE_LEFT);
			SetAnimState(ANIM_MOVING);
			return;
		}
		else if( pDI->KeyDown( DIK_D ) ||
			pDI->JoystickGetLStickDirDown(DIR_RIGHT, 0) )
		{
			AI->CardinalMove(this, FLAG_MOVE_RIGHT);
			SetAnimState(ANIM_MOVING);
			return;
		}

		if( pDI->MouseButtonPressed( 0 ) ||
			pDI->JoystickButtonPressed(0))
		{
			int cameraX = 0 , cameraY = 0 ;
			CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
			int tileXPos = (int)((pDI->MouseGetPosX() + cameraX) / TILE_WIDTH) ;
			int tileYPos = (int)((pDI->MouseGetPosY() + cameraY) / TILE_HEIGHT) ;
			cout << " mouse clicked at ( " << tileXPos << " , " << tileYPos << " )\n" ;

			if( GetHeldItem() == NULL )
				return ;

			if( tileXPos >= GetIndexPosX() - 1 && tileXPos <= GetIndexPosX() + 1 && tileYPos >= GetIndexPosY() - 1 && tileYPos <= GetIndexPosY() + 1 )
			{
				int item = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
				int entityID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( tileXPos , tileYPos ) ;
				int tileID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( tileXPos , tileYPos ) ;
				IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
				IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
				IUnitInterface* tile = (MObjectManager::GetInstance()->GetUnit(tileID)) ;
				if( !GetHeldItem()->CheckCollision( object , false ) && !GetHeldItem()->CheckCollision( entity , false ) && !GetHeldItem()->CheckCollision( tile , false ) && tileID != 0 )
				{					
					MEventSystem::GetInstance()->SendEvent( "ATTRACTORPLACED" , GetHeldItem() ) ;
					MMessageSystem::GetInstance()->SendMsg( new msgPlaceObject(tileXPos , tileYPos ) ) ;
					//Call objects exitcollision
					int tileID = MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).
						GetFlake( OBJECT_TILE ).GetInfoAtIndex(tileXPos , tileYPos);
					IUnitInterface* obj = (MObjectManager::GetInstance()->GetUnit(tileID));
					GetHeldItem()->ExitCollision(obj, true);
				}
			}

		}
		if( pDI->MouseButtonPressed( 1 ) ||
			pDI->JoystickButtonPressed(1))
		{
			if( GetEquippedItem() )
			{
				switch(GetEquippedItem()->GetType())
				{
				case OBJ_POWERGLOVES:
					{
						SetAnimState(ANIM_THROW);
						AM->SetAnimTexture(m_vMovementAnimIDs[GetStateAnimID()], m_nImageID);
						AM->PlayAnimation(m_vMovementAnimIDs[GetStateAnimID()]);						
						break;
					}
				};
				GetEquippedItem()->UseObject( (CBaseObject*)this ) ;
			}
		}
	}
}
bool CPlayer::CanInteract(IUnitInterface* pBase)
{	

	return false;
}
bool CPlayer::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//if we collide with an object
	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			if( temp->GetType() == OBJ_ATTRACTOR || temp->GetType() == OBJ_POWERGLOVES || temp->GetType() == OBJ_OILCAN || temp->GetType() == OBJ_LIGHTORB)
			{
				//if we can hold the object we collided with...		
				//allow the player to hold it unless 
				//the player is already holding onto something
				if( temp->GetType() == OBJ_ATTRACTOR && GetHeldItem() != NULL )
				{
					return true;
				}
				else if( ( temp->GetType() == OBJ_POWERGLOVES || temp->GetType() == OBJ_OILCAN || temp->GetType() == OBJ_LIGHTORB) && 
					GetEquippedItem() != NULL && GetHeldItem() != NULL )
				{
					return true;
				}
				else
				{
					pBase->SetVelY( 0 ) ;
					pBase->SetVelX( 0 ) ;
					pBase->SetDistanceLeft( 0 ) ;
					pBase->SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );

					MMessageSystem::GetInstance()->SendMsg( new msgPickUpObject( (CBaseObject*)pBase ) ) ;	
					return false;				
				}
				return true;
			}					
			else if( temp->GetType() == OBJ_OIL )
				return temp->CheckCollision(this, nCanHandleCollision);			

			return true;
		}
		break;

	case OBJECT_ENTITY:
		{
			return true;
		}
		break;

	case OBJECT_TILE:
		{
			/*if( pBase->GetType() == OBJ_BUTTON && nCanHandleCollision )
			{
			((CButton*)pBase)->CheckCollision(this);
			return false;
			}*/
			if(pBase->GetType() == OBJ_ELECTRICBUTTON)
			{				
				((CElectricButton*)pBase)->CheckCollision(this, nCanHandleCollision);
				return false;
			}
			else if( pBase->GetType() == OBJ_DOOR )
			{
				//if we can hold the object we collided with...		
				//allow the player to hold it unless 
				//the player is already holding onto something
				if( ((CDoor*)pBase)->GetIsOpen() )
					return false;
				else
					return true;
			}
			else if(pBase->GetType() == OBJ_WATER)
			{
				//if the tile is frozen we can walk past it
				return !((CWaterTile*)pBase)->IsFrozen();
			}
			else if(pBase->GetType() == OBJ_ELECTRICGENERATOR)
				return true;
		}
		break;
	};

	return false;
}
void CPlayer::ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if( !pBase || !nCanHandleCollision || pBase->GetLayerLocation() != this->GetLayerLocation() )
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
	};
}
bool CPlayer::CheckTileCollision(int TileID)
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
	case 1:
		{

		}
	};
	return false;
}
void CPlayer::UpdateInvincibilityTime(float fDT)
{
	//if we're not invincible no use updating the timer
	if(!IsInvincible())
		return;

	m_fInvincilibilityTimer += fDT;
	if(m_fInvincilibilityTimer > INVTIME)
	{
		SetInvincilibity(false);
	}
}
void CPlayer::LoadEntMoveAnimIDs()
{
	m_vMovementAnimIDs.clear();
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-movedown"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-moveleft"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-moveright"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-moveup"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-place-down"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-place-left"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-place-right"));
	m_vMovementAnimIDs.push_back(CAnimationManager::GetInstance()->GetID("player-place-up"));
	//down animation by default
	SetAnimID(m_vMovementAnimIDs[0]);
}
//accessors
int CPlayer::GetLives() const
{
	return m_nLives;
}
bool CPlayer::IsInvincible() const
{
	return m_bIsInvincible;
}
CBaseObject* CPlayer::GetHeldItem() const
{
	return m_pHeldItem;
}
int CPlayer::GetStateAnimID()
{
	int dir = IUnitInterface::GetFlag_DirectionToMove();
	int animstate = GetAnimState();

	switch(dir)
	{
	case FLAG_MOVE_RIGHT:
		{
			return (animstate * 4) + 2;
		}
	case FLAG_MOVE_LEFT:
		{
			return (animstate * 4) + 1;
		}
	case FLAG_MOVE_UP:
		{
			return (animstate * 4) + 3;
		}
	case FLAG_MOVE_DOWN:
		{
			return (animstate * 4);
		}
	}
	return -1;
}
//mutators
bool CPlayer::SetLives(const int nLives)
{
	if(nLives < 0)
		return false;

	if(IsInvincible() && nLives < m_nLives)
		return false;

	//if the player loses a life(s) they are
	//temporarily invincible
	if(nLives < m_nLives)
	{
		SetInvincilibity(true);

		if(HurtSoundID > -1)
		CSGD_FModManager::GetInstance()->PlaySoundA(HurtSoundID);
	}

	m_nLives = nLives;

	

	if(GetLives() <= 0)
		CGamePlayState::GetInstance()->KillPlayer();
	return true;
}
void CPlayer::SetInvincilibity(const bool bIsInvincible)
{
	if(!bIsInvincible)
		m_fInvincilibilityTimer = 0.0f;

	m_bIsInvincible = bIsInvincible;
}
void CPlayer::SetHeldItem(CBaseObject* const pHeldItem)
{
	m_pHeldItem = pHeldItem;
}
void CPlayer::SwitchItems(void)
{
	if( GetHeldItem() != NULL )
		if( GetHeldItem()->GetType() == OBJ_ATTRACTOR )
		{
			return ;
		}
		CBaseObject* temp = GetHeldItem() ;
		SetHeldItem(GetEquippedItem()) ;
		SetEquippedItem(temp) ;
}
void CPlayer::SetAnimState(int nAnimState)
{
	m_nAnimState = nAnimState;
}

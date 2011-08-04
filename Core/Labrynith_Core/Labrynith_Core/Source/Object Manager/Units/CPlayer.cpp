#include "CPlayer.h"	
#include "../MObjectManager.h"
#include "../../AI Handler/CAI_Handler.h"
#include "../../Wrappers/CSGD_DirectInput.h"
#include "../../GameStates/CGamePlayState.h"
#include "Tiles\CDoor.h"
#include "Tiles\CButton.h"
#include "../../GameStates/CLoadLevelState.h"
#include "../../CGame.h"
#include "CBaseObject.h"
#include <iostream>
#include "../../Wrappers/CSGD_Direct3D.h"

CPlayer::CPlayer(void)
{
	CBaseEntity::CBaseEntity();
	m_pHeldItem = NULL;
	SetLives(4);
	m_nType = ENT_PLAYER;
}
CPlayer::~CPlayer(void)
{
	CBaseEntity::~CBaseEntity();
	if(m_pHeldItem)
		delete m_pHeldItem;
}
void CPlayer::Update(float fDT)
{
	CBaseEntity::Update(fDT);
	Input();
}
void CPlayer::Render( int CameraPosX, int CameraPosY )
{
	MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber )
		.GetFlake( OBJECT_LIGHT ).SetInfoAtIndex(GetIndexPosX(), GetIndexPosY(), rand() % 15 + 240 );
	CBaseEntity::Render(CameraPosX, CameraPosY);

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( GetHeldItem() == NULL )
		return ;

	int cameraX = 0 , cameraY = 0 ;
	CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
	int tileXPos = (int)((pDI->MouseGetPosX() + cameraX) / 32.0f) ;
	int tileYPos = (int)((pDI->MouseGetPosY() + cameraY) / 32.0f) ;

	if( MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) == 0 && MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( tileXPos , tileYPos ) == 0 && MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( tileXPos , tileYPos ) == 1 )
	if( tileXPos >= GetIndexPosX() - 1 && tileXPos <= GetIndexPosX() + 1 && tileYPos >= GetIndexPosY() - 1 && tileYPos <= GetIndexPosY() + 1 )
	{
		int xPos1 = tileXPos * 32 - cameraX ;
		int yPos1 = tileYPos * 32 - cameraY ;
		CSGD_Direct3D::GetInstance()->DrawLine( xPos1 , yPos1 , xPos1 + 32 , yPos1 ) ;
		CSGD_Direct3D::GetInstance()->DrawLine( xPos1 + 32 , yPos1 , xPos1 + 32 , yPos1 + 32 ) ;
		CSGD_Direct3D::GetInstance()->DrawLine( xPos1 + 32 , yPos1 + 32 , xPos1 , yPos1 + 32 ) ;
		CSGD_Direct3D::GetInstance()->DrawLine( xPos1 , yPos1 + 32 , xPos1 , yPos1 ) ;
	}

}
void CPlayer::Input()
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CAI_Handler* AI = CAI_Handler::GetInstance();

	if( GetFlag_MovementState() == FLAG_MOVESTATE_ATDESTINATION )
		{

			if( pDI->KeyDown( DIK_UP ) )
			{
				//MObjectManager::GetInstance()->MoveEntUp( m_nIdentificationNumber );
				//AI->CheckCollisions(this, GetIndexPosX(), GetIndexPosY(), true);
				AI->CardinalMove(this, FLAG_MOVE_UP);
				return;
			}

			if( pDI->KeyDown( DIK_DOWN ) )
			{
				//MObjectManager::GetInstance()->MoveEntDown( m_nIdentificationNumber );
				//AI->CheckCollisions(this, GetIndexPosX(), GetIndexPosY(), true);
				AI->CardinalMove(this, FLAG_MOVE_DOWN);
				return;
			}

			if( pDI->KeyDown( DIK_LEFT ) )
			{
				//MObjectManager::GetInstance()->MoveEntLeft( m_nIdentificationNumber );
				//AI->CheckCollisions(this, GetIndexPosX(), GetIndexPosY(), true);
				AI->CardinalMove(this, FLAG_MOVE_LEFT);
				return;
			}

			if( pDI->KeyDown( DIK_RIGHT ) )
			{
				//MObjectManager::GetInstance()->MoveEntRight( m_nIdentificationNumber );
				//AI->CheckCollisions(this, GetIndexPosX(), GetIndexPosY(), true);
				AI->CardinalMove(this, FLAG_MOVE_RIGHT);
				return;
			}

			if( pDI->MouseButtonPressed( 0 ) )
			{
				int cameraX = 0 , cameraY = 0 ;
				CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
				int tileXPos = (int)((pDI->MouseGetPosX() + cameraX) / 32.0f) ;
				int tileYPos = (int)((pDI->MouseGetPosY() + cameraY) / 32.0f) ;
				cout << " mouse clicked at ( " << tileXPos << " , " << tileYPos << " )\n" ;

				if( GetHeldItem() == NULL )
					return ;

				if( tileXPos >= GetIndexPosX() - 1 && tileXPos <= GetIndexPosX() + 1 && tileYPos >= GetIndexPosY() - 1 && tileYPos <= GetIndexPosY() + 1 )
					MMessageSystem::GetInstance()->SendMsg( new msgPlaceObject(tileXPos , tileYPos ) ) ;

			}

		}
}
bool CPlayer::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this)
		return false;

	//if we collide with an object
	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			if( temp->GetType() == ENT_ATTRACTOR )
			{
				//if we can hold the object we collided with...		
				//allow the player to hold it unless 
				//the player is already holding onto something
				if( GetHeldItem() != NULL )
					return true ;
				else
				{
					MMessageSystem::GetInstance()->SendMsg( new msgPickUpObject( (CBaseObject*)pBase ) ) ;
				}
				return true;
			}

			if( temp->GetType() == OBJ_DOOR )
			{
				//if we can hold the object we collided with...		
				//allow the player to hold it unless 
				//the player is already holding onto something
				if( ((CDoor*)temp)->GetIsOpen() )
					return false;
				else
					return true;
			}

			if( temp->GetType() == OBJ_SPAWNER )
				return false;

			if( temp->GetType() == OBJ_BUTTON )
			{
				((CButton*)temp)->CheckCollision(this);
				return false;
			}
			if(temp->GetType() == OBJ_EXIT)
			{
				CGamePlayState* pGamePlay = CGamePlayState::GetInstance();
				//BUG- check if the level is valid
				pGamePlay->SetCurrentLevel(pGamePlay->GetCurrentLevel() + 1);
				//pGamePlay->SetCurrentLevel(1);
				//Load the next level
				CGame::GetInstance()->PushState(CLoadLevelState::GetInstance());
				return false;
			}
			return true;
		}
		break;

	case OBJECT_ENTITY:
		{
			return true;
		}
		break;
	};

	return false;
	//return pBase->CheckCollision(this);
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
	};
	return false;
}
//accessors
int CPlayer::GetLives() const
{
	return m_nLives;
}
IUnitInterface* CPlayer::GetHeldItem() const
{
	return m_pHeldItem;
}
//mutators
void CPlayer::SetLives(const int nLives)
{
	if(nLives >= 0)
		m_nLives = nLives;
}
void CPlayer::SetHeldItem(IUnitInterface* const pHeldItem)
{
	m_pHeldItem = pHeldItem;
}

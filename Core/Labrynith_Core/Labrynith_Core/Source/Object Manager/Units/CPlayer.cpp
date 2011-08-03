#include "CPlayer.h"	
#include "../MObjectManager.h"
#include "../../Wrappers/CSGD_DirectInput.h"
#include "../../GameStates/CGamePlayState.h"
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

	if( GetFlag_MovementState() == FLAG_MOVESTATE_ATDESTINATION )
		{

			if( pDI->KeyDown( DIK_UP ) )
			{
				MObjectManager::GetInstance()->MoveEntUp( m_nIdentificationNumber );
				return;
			}

			if( pDI->KeyDown( DIK_DOWN ) )
			{
				MObjectManager::GetInstance()->MoveEntDown( m_nIdentificationNumber );
				return;
			}

			if( pDI->KeyDown( DIK_LEFT ) )
			{
				MObjectManager::GetInstance()->MoveEntLeft( m_nIdentificationNumber );
				return;
			}

			if( pDI->KeyDown( DIK_RIGHT ) )
			{
				MObjectManager::GetInstance()->MoveEntRight( m_nIdentificationNumber );
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
bool CPlayer::CheckCollision(IUnitInterface* pBase)
{
	if(!pBase)
		return false;

	//if we collide with an object

	if(pBase->GetType() == OBJECT_OBJECT)
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
	}


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

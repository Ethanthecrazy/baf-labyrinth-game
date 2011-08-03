#include "CPlayer.h"	
#include "../MObjectManager.h"
#include "../../Wrappers/CSGD_DirectInput.h"

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

		}
}
bool CPlayer::CheckCollision(IUnitInterface* pBase)
{
	if(!pBase)
		return false;

	//if we collide with an object
	if(pBase->m_nUnitType == OBJECT_OBJECT)
	{
		//if we can hold the object we collided with...
		//allow the player to hold it unless 
		//the player is already holding onto something
		return true;
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
void CPlayer::SetHeldItem(const IUnitInterface* pHeldItem)
{
	if(pHeldItem)
	{
		//add the item we have back into the object manager
		//'switch' the item to hold with the one sent in
	}
	else
	{
		//the item to hold is null, aka the player is dropping it
		//add the item we have back into the object manager
	}
}

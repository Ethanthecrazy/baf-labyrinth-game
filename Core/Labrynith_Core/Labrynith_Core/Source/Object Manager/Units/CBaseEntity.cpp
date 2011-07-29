#include "CBaseEntity.h"
#include "../../Wrappers/CSGD_TextureManager.h"
#include "../MObjectManager.h"

CBaseEntity::CBaseEntity()
{
	// position
	 SetPosX(0);
	 SetPosY(0);

	// velocity
	 SetVelX(0);
	 SetVelY(0);

	 m_nUnitType = OBJECT_ENTITY;

	 SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );

	 m_nImageID = -1;

	 m_uiRefCount = 1;
}

CBaseEntity::~CBaseEntity(void)
{

}

void CBaseEntity::Update(float fDT)
{
	//MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).SetInfoAtIndex( GetIndexPosX(), GetIndexPosY(), rand() % 15 + 240 );

	if( GetFlag_MovementState() == FLAG_MOVESTATE_MOVING )
	{

		SetDistanceLeft( GetDistanceLeft() - 125 * fDT );

		if( GetDistanceLeft() >= 0 )
		{

			switch ( GetFlag_DirectionToMove() )
			{

			case FLAG_MOVE_RIGHT:
				SetPosX( GetPosX() + 125 * fDT );
				//MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).SetInfoAtIndex( GetIndexPosX() - 1, GetIndexPosY(), rand() % 15 + 240 );
				break;
			case FLAG_MOVE_LEFT:
				SetPosX( GetPosX() - 125 * fDT );
				//MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).SetInfoAtIndex( GetIndexPosX() + 1, GetIndexPosY(), rand() % 15 + 240 );
				break;
			case FLAG_MOVE_UP:
				SetPosY( GetPosY() - 125 * fDT );
				//MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).SetInfoAtIndex( GetIndexPosX() + 1, GetIndexPosY(), rand() % 15 + 240 );
				break;
			case FLAG_MOVE_DOWN:
				SetPosY( GetPosY() + 125 * fDT );
				//MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).SetInfoAtIndex( GetIndexPosX() - 1, GetIndexPosY(), rand() % 15 + 240 );
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

			int itemCollision = MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetValueInFlakeAtIndex( OBJECT_OBJECT, GetIndexPosX(), GetIndexPosY() );
			
			if( itemCollision > 0 )
				MObjectManager::GetInstance()->RemoveUnit( itemCollision );

			MObjectManager::GetInstance()->FindFlake( this->m_nIdentificationNumber ).FinishMovingEnt( this );

			SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );
		}
	}
	else
	{
		SetLastPosX( GetPosX() );
		SetLastPosY( GetPosY() );
	}

	

}

void CBaseEntity::Render( int CameraPosX, int CameraPosY )
{
	if( m_nImageID > -1 )
	{
		int DrawPositionX = (int)GetPosX() - CameraPosX;
		int DrawPositionY = (int)GetPosY() - CameraPosY;

		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, DrawPositionX, DrawPositionY,
			1.0f,
			1.0f,
			0,
			0.0f,
			0.0f,
			0.0f,
			D3DCOLOR_ARGB( MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( GetIndexPosX(), GetIndexPosY() ), 255, 255, 255) );	

	}
}

bool CBaseEntity::CheckCollision(IUnitInterface* pBase)
{
	return false;
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
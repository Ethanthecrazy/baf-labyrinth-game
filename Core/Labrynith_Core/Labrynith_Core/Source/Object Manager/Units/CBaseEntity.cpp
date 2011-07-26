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
	if( GetFlag_MovementState() == FLAG_MOVESTATE_MOVING )
	{

		SetDistanceLeft( GetDistanceLeft() - 150 * fDT );

		if( GetDistanceLeft() >= 0 )
		{

			switch ( GetFlag_DirectionToMove() )
			{

			case FLAG_MOVE_RIGHT:
				SetPosX( GetPosX() + 150 * fDT );
				break;
			case FLAG_MOVE_LEFT:
				SetPosX( GetPosX() - 150 * fDT );
				break;
			case FLAG_MOVE_UP:
				SetPosY( GetPosY() - 150 * fDT );
				break;
			case FLAG_MOVE_DOWN:
				SetPosY( GetPosY() + 150 * fDT );
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

		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, DrawPositionX, DrawPositionY );
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
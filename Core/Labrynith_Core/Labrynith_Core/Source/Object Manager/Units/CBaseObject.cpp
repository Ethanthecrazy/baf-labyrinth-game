#include "CBaseObject.h"


#include "../../Wrappers/CSGD_TextureManager.h"
#include "../MObjectManager.h"

CBaseObject::CBaseObject()
{
	// position
	 SetPosX(0);
	 SetPosY(0);

	// velocity
	 SetVelX(0);
	 SetVelY(0);

	 m_nUnitType = OBJECT_OBJECT;

	 m_nImageID = -1;

	 m_uiRefCount = 1;

	 m_nIdentificationNumber = 0 ;
}

CBaseObject::~CBaseObject(void)
{

}

void CBaseObject::Update(float fDT)
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

void CBaseObject::Render( int CameraPosX, int CameraPosY )
{
	if( m_nImageID > -1 )
	{
		int DrawPositionX = (int)GetPosX() - CameraPosX;
		int DrawPositionY = (int)GetPosY() - CameraPosY;

		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, DrawPositionX, DrawPositionY );
	}
}

bool CBaseObject::CheckCollision(IUnitInterface* pBase)
{
	return true;
}

void CBaseObject::AddRef(void)
{
	++m_uiRefCount;
}

void CBaseObject::Release(void)
{
	--m_uiRefCount;

	if (m_uiRefCount == 0)
		delete this;
}
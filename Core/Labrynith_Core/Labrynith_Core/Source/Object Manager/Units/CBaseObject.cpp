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

	 this->SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION ) ;
	 

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
			int xDirection = 0 ;
			int yDirection = 0 ;

			switch ( GetFlag_DirectionToMove() )
			{

			case FLAG_MOVE_RIGHT:
				SetPosX( GetLastPosX() + 32 );
				xDirection = 1 ;
				break;
			case FLAG_MOVE_LEFT:
				SetPosX( GetLastPosX() - 32 );
				xDirection = -1 ;
				break;
			case FLAG_MOVE_UP:
				SetPosY( GetLastPosY() - 32 );
				yDirection = -1 ;
				break;
			case FLAG_MOVE_DOWN:
				SetPosY( GetLastPosY() + 32 );
				yDirection = 1 ;
				break;
			}

			MObjectManager::GetInstance()->FindFlake(m_nIdentificationNumber).SetInfoAtIndex( GetIndexPosX() , GetIndexPosY() , 0 ) ;
			SetIndexPosX( GetPosX() / 32 ) ;
			SetIndexPosY( GetPosY() / 32 ) ;
			if( (GetVelX() > 0 || GetVelY() > 0) && this->GetType() == OBJ_ATTRACTOR )
				MEventSystem::GetInstance()->SendEvent( "ATTRACTORPLACED" , this ) ;
			
			MObjectManager::GetInstance()->FindFlake(m_nIdentificationNumber).SetInfoAtIndex( GetIndexPosX() , GetIndexPosY() , m_nIdentificationNumber ) ;

			SetLastPosX( GetPosX() );
			SetLastPosY( GetPosY() );

			MObjectManager::GetInstance()->FindFlake( this->m_nIdentificationNumber ).FinishMovingEnt( this );

			if( GetVelX() != 0 || GetVelY() != 0 )
			{
				if( int collUnit = MObjectManager::GetInstance()->GetLayer( 1 ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( GetIndexPosX() + xDirection , GetIndexPosY() + yDirection ) )
				{
					if( CheckCollision( MObjectManager::GetInstance()->GetUnit( collUnit ) , true ) )
					{
						SetVelY( 0 ) ;
						SetVelX( 0 ) ;
						SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION ) ;
						SetDistanceLeft( 0 ) ;
					}
				}
				else if( int collTile = MObjectManager::GetInstance()->GetLayer( 1 ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( GetIndexPosX() + xDirection , GetIndexPosY() + yDirection ) != 1 )
				{
					SetVelY( 0 ) ;
					SetVelX( 0 ) ;
					SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION ) ;
					SetDistanceLeft( 0 ) ;
				}
				else if( GetIndexPosY() - 1 < 0 || GetIndexPosY() + 1 > MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber).GetLayerHeight() - 1 || GetIndexPosX() - 1 < 0 || GetIndexPosX() + 1 > MObjectManager::GetInstance()->FindLayer(m_nIdentificationNumber).GetLayerWidth() - 1 )
				{
					SetVelY( 0 ) ;
					SetVelX( 0 ) ;
					SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION ) ;
					SetDistanceLeft( 0 ) ;
				}

			}

			if( GetVelX() != 0 )
			{

				SetVelX( GetVelX() - 1 ) ;
				if( GetVelX() == 0 )
				{
					SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION ) ;
					SetDistanceLeft( 0 ) ;
				}
				else
					SetDistanceLeft( 32.0f ) ;
			}
			if( GetVelY() != 0 )
			{

				SetVelY( GetVelY() - 1 ) ;
				if( GetVelY() == 0 )
				{
					SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION ) ;
					SetDistanceLeft( 0 ) ;
				}
				else
					SetDistanceLeft( 32.0f ) ;
			}

			
			//SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );
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
		
		CSGD_TextureManager::GetInstance()->Draw( m_nImageID,
		DrawPositionX,
		DrawPositionY,
		1.0f,
		1.0f,
		0,
		0.0f,
		0.0f,
		0.0f,
		D3DCOLOR_ARGB( MObjectManager::GetInstance()->GetLayer( 1 ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( GetIndexPosX(), GetIndexPosY() ), 255, 255, 255) );	

	}
}

void CBaseObject::UseObject( CBaseObject* user )
{
	return ;
}

bool CBaseObject::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if( pBase == this )
		return false;

	return true;
}
void CBaseObject::ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{

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

#include "CBaseObject.h"


#include "../../Wrappers/CSGD_TextureManager.h"
#include "../MObjectManager.h"
#include "../../GameStates/CGamePlayState.h"
#include "Tiles\CWaterTile.h"

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

	 m_nIdentificationNumber = 0;
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
			SetIndexPosX( (int)(GetPosX() / 32) ) ;
			SetIndexPosY( (int)(GetPosY() / 32) ) ;
			if( (GetVelX() > 0 || GetVelY() > 0) && this->GetType() == OBJ_ATTRACTOR )
				MEventSystem::GetInstance()->SendEvent( "ATTRACTORPLACED" , this ) ;
			
			MObjectManager::GetInstance()->FindFlake(m_nIdentificationNumber).SetInfoAtIndex( GetIndexPosX() , GetIndexPosY() , m_nIdentificationNumber ) ;

			SetLastPosX( GetPosX() );
			SetLastPosY( GetPosY() );

			MObjectManager::GetInstance()->FindFlake( this->m_nIdentificationNumber ).FinishMovingEnt( this );

			if( GetVelX() != 0 || GetVelY() != 0 )
			{
				int item = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( GetIndexPosX() + xDirection , GetIndexPosY() + yDirection ) ;
				int entityID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( GetIndexPosX() + xDirection , GetIndexPosY() + yDirection ) ;
				int tileID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( GetIndexPosX() + xDirection , GetIndexPosY() + yDirection ) ;
				IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
				IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
				IUnitInterface* tile = (MObjectManager::GetInstance()->GetUnit(tileID)) ;
				if( CheckCollision( object , true ) || CheckCollision( entity , true ) || CheckCollision( tile , true ) || tileID == 0 )
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
		RECT camRect;
		camRect.top = (long)CameraPosY;
		camRect.left = (long)CameraPosX;
		camRect.bottom = (long)camRect.top + 600;
		camRect.right = (long)camRect.left + 800;
		
		RECT objRect;
		objRect.top = (long)GetPosY();
		objRect.left = (long)GetPosX();
		objRect.bottom = (long)objRect.top + 32;
		objRect.right = (long)objRect.left + 32;

		RECT out;
		if(!IntersectRect(&out, &camRect, &objRect))
			return;

		int lightamount = MObjectManager::GetInstance()->GetLayer( this->GetLayerLocation() ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( GetIndexPosX(), GetIndexPosY() );
		if(lightamount == 0)
			return;

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
		D3DCOLOR_ARGB( 255, lightamount, lightamount, lightamount) );	

	}
}

void CBaseObject::UseObject( CBaseObject* user )
{
	return ;
}

bool CBaseObject::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//if we collide with an object
	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase; // this is hilarious, who did this? - Nathan
			if( temp->GetType() == OBJ_ATTRACTOR || temp->GetType() == OBJ_POWERGLOVES || temp->GetType() == OBJ_OILCAN || temp->GetType() == OBJ_OIL || temp->GetType() == OBJ_LIGHTORB || temp->GetType() == OBJ_DOOR || temp->GetType() == OBJ_RAMP || temp->GetType() == OBJ_ELECTRICGENERATOR || temp->GetType() == OBJ_EXIT )
			{
				return true;
			}
		}
		break;

	case OBJECT_ENTITY:
		{
			return true;
		}
		break;
	case OBJECT_TILE:
		{				
			if(pBase->GetType() == OBJ_WATER)
				if( !((CWaterTile*)pBase)->IsFrozen() && this->GetType() != OBJ_OILCAN )
					return true;

			if( pBase->GetType() == OBJ_DOOR || pBase->GetType() == OBJ_EXIT || pBase->GetType() == OBJ_RAMP || pBase->GetType() == OBJ_PIT || pBase->GetType() == OBJ_ELECTRICGENERATOR)
				return true ;
		}
		break ;
	};

	return false;
}
void CBaseObject::ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{

}
bool CBaseObject::CanInteract(IUnitInterface* pBase)
{
	return false;
}

void CBaseObject::AddRef(void)
{
	++m_uiRefCount;
}

void CBaseObject::Release(void)
{
	--m_uiRefCount;

	if (m_uiRefCount <= 1)
		MMessageSystem::GetInstance()->SendMsg(new msgDeleteMe(this));
}

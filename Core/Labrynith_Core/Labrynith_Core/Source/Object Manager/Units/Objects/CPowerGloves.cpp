#include "CPowerGloves.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../CPlayer.h"
#include "../../MObjectManager.h"

CPowerGloves::CPowerGloves()
{
	m_nType = OBJ_POWERGLOVES;
}

void CPowerGloves::UseObject( CBaseObject* user )
{
	CPlayer* player = (CPlayer*)user ;

	if( player->GetHeldItem() == NULL )
		return;

	int xDirection = 0 ;
	int yDirection = 0 ;
	
	switch( player->GetFlag_DirectionToMove() )
	{
	case FLAG_MOVE_UP:
		{
			player->GetHeldItem()->SetVelY( 3 ) ;
			player->GetHeldItem()->SetFlag_DirectionToMove( FLAG_MOVE_UP ) ;
			player->GetHeldItem()->SetFlag_MovementState( FLAG_MOVESTATE_MOVING ) ;
			yDirection--;
		}
		break ;
	case FLAG_MOVE_RIGHT:
		{
			player->GetHeldItem()->SetVelX( 3 ) ;
			player->GetHeldItem()->SetFlag_DirectionToMove( FLAG_MOVE_RIGHT ) ;
			player->GetHeldItem()->SetFlag_MovementState( FLAG_MOVESTATE_MOVING ) ;
			xDirection++;
		}
		break;
	case FLAG_MOVE_DOWN:
		{
			player->GetHeldItem()->SetVelY( 3 ) ;
			player->GetHeldItem()->SetFlag_DirectionToMove( FLAG_MOVE_DOWN ) ;
			player->GetHeldItem()->SetFlag_MovementState( FLAG_MOVESTATE_MOVING ) ;
			yDirection++;
		}
		break ;
	case FLAG_MOVE_LEFT:
		{
			player->GetHeldItem()->SetVelX( 3 ) ;
			player->GetHeldItem()->SetFlag_DirectionToMove( FLAG_MOVE_LEFT ) ;
			player->GetHeldItem()->SetFlag_MovementState( FLAG_MOVESTATE_MOVING ) ;
			xDirection--;
		}
		break ;
	}
	//GetHeldItem()->SetDistanceLeft( TILE_WIDTH ) ;
	
	int item = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( player->GetIndexPosX() + xDirection , player->GetIndexPosY() + yDirection ) ;
	int entityID = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( player->GetIndexPosX() + xDirection , player->GetIndexPosY() + yDirection ) ;
	int tileID = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( player->GetIndexPosX() + xDirection , player->GetIndexPosY() + yDirection ) ;
	IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
	IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
	IUnitInterface* tile = (MObjectManager::GetInstance()->GetUnit(tileID)) ;
	if( player->GetHeldItem()->CheckCollision( object , false ) || player->GetHeldItem()->CheckCollision( entity , false ) || player->GetHeldItem()->CheckCollision( tile , false ) || tileID == 0 )
	{
		player->GetHeldItem()->SetVelX( 0 );
		player->GetHeldItem()->SetFlag_DirectionToMove( FLAG_MOVE_LEFT );
		player->GetHeldItem()->SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );
	}
	else
	{
		player->GetHeldItem()->SetLastPosX( player->GetPosX() ) ;
		player->GetHeldItem()->SetLastPosY( player->GetPosY() ) ;
		MMessageSystem::GetInstance()->SendMsg( new msgPlaceObject(player->GetIndexPosX() + xDirection , player->GetIndexPosY() + yDirection ) ) ;
	}
}
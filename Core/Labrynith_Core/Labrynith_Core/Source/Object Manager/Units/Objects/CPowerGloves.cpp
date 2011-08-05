#include "CPowerGloves.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../CPlayer.h"


void CPowerGloves::UseObject( CBaseObject* user )
{
	CPlayer* player = (CPlayer*)user ;

	if( player->GetHeldItem() == NULL )
								return ;

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
							//GetHeldItem()->SetDistanceLeft( 32.0f ) ;
							player->GetHeldItem()->SetLastPosX( player->GetPosX() ) ;
							player->GetHeldItem()->SetLastPosY( player->GetPosY() ) ;
							MMessageSystem::GetInstance()->SendMsg( new msgPlaceObject(player->GetIndexPosX() + xDirection , player->GetIndexPosY() + yDirection ) ) ;
}
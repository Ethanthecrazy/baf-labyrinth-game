#include "COilCan.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../CPlayer.h"
#include "COil.h"
#include "../../MObjectManager.h"
#include "../../../Wrappers/CSGD_FModManager.h"

COilCan::COilCan( void )
{
	CBaseObject::CBaseObject() ;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/OilBarrel.png") ;
	m_nOilSoundID = CSGD_FModManager::GetInstance()->LoadSound( "resource/Sounds/OilDrop.mp3" ) ;
	SetCharges( 5 ) ;
}

void COilCan::UseObject( CBaseObject* user )
{
	CPlayer* player = (CPlayer*)user ;

	if( GetCharges() > 0 )
	{
		int xDirection = 0 ;
		int yDirection = 0 ;
		
		switch( player->GetFlag_DirectionToMove() )
		{
		case FLAG_MOVE_UP:
			{
				yDirection--;
			}
			break ;
		case FLAG_MOVE_RIGHT:
			{
				xDirection++;
			}
			break;
		case FLAG_MOVE_DOWN:
			{
				yDirection++;
			}
			break ;
		case FLAG_MOVE_LEFT:
			{
				xDirection--;
			}
			break ;
		}

		int item = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( player->GetIndexPosX() + xDirection , player->GetIndexPosY() + yDirection ) ;
		IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
		if( object )
			return ;

		COil* oil = new COil() ;
		oil->SetIndexPosX( player->GetIndexPosX() + xDirection ) ;
		oil->SetIndexPosY( player->GetIndexPosY() + yDirection ) ;
		oil->SetPosX( oil->GetIndexPosX() * 32 ) ;
		oil->SetPosY( oil->GetIndexPosY() * 32 ) ;
		MObjectManager::GetInstance()->AddUnitIndexed( oil , MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetLayerID() ) ;

		this->SetCharges( GetCharges() - 1 ) ;

		CSGD_FModManager::GetInstance()->PlaySoundA( m_nOilSoundID ) ;
	}
}

void COilCan::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render( CameraPosX , CameraPosY ) ;


	// display the number of charges left
}
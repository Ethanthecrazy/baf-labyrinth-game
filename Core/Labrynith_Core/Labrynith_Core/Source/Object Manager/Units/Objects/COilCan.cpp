#include "COilCan.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../CPlayer.h"
#include "COil.h"
#include "../../MObjectManager.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/CGamePlayState.h"
#include "../../../Wrappers/CSGD_DirectInput.h"
#include "../../../GameStates/COptionsState.h"

COilCan::COilCan( void )
{
	CBaseObject::CBaseObject() ;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/OilBarrel.png") ;
	m_nOilSoundID = CSGD_FModManager::GetInstance()->LoadSound( "resource/Sounds/OilDrop.mp3" ) ;
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(m_nOilSoundID, true);
	SetCharges( 5 ) ;
}

void COilCan::UseObject( CBaseObject* user )
{
	CPlayer* player = (CPlayer*)user ;

	if( GetCharges() > 0 )
	{
		CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
		
		int cameraX = 0 , cameraY = 0 ;
		CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
		int tileXPos = (int)((pDI->MouseGetPosX() + cameraX) / TILE_WIDTH );
		int tileYPos = (int)((pDI->MouseGetPosY() + cameraY) / TILE_HEIGHT) ;

		if( tileXPos >= player->GetIndexPosX() - 1 && tileXPos <= player->GetIndexPosX() + 1 && tileYPos >= player->GetIndexPosY() - 1 && tileYPos <= player->GetIndexPosY() + 1 )
		{
			int item = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			int entityID = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			int tileID = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
			IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
			IUnitInterface* tile = (MObjectManager::GetInstance()->GetUnit(tileID)) ;
			if( !this->CheckCollision( object , false ) && !this->CheckCollision( entity , false ) && !this->CheckCollision( tile , false ) && tileID != 0 )
			{
				COil* oil = new COil() ;
				oil->SetIndexPosX( tileXPos ) ;
				oil->SetIndexPosY( tileYPos ) ;
				oil->SetPosX( (float)(oil->GetIndexPosX() * TILE_WIDTH) );
				oil->SetPosY( (float)(oil->GetIndexPosY() * TILE_HEIGHT) );
				MObjectManager::GetInstance()->AddUnitIndexed( oil , 
				MObjectManager::GetInstance()->FindLayer(CGamePlayState::GetInstance()->testVaribale).GetLayerID() ) ;

				this->SetCharges( GetCharges() - 1 ) ;

				CSGD_FModManager::GetInstance()->PlaySoundA( m_nOilSoundID ) ;
			}
		}
	}
}

void COilCan::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render( CameraPosX , CameraPosY ) ;
	// display the number of charges left
}
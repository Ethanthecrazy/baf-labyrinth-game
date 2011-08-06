#include "COil.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../MObjectManager.h"
#include "../../../Animation Manager/CAnimationManager.h"
#include "../CPlayer.h"

COil::COil( void )
{
	CBaseObject::CBaseObject() ;
	m_nType = OBJ_OIL ;
	SetLifeDuration( 120.0f ) ;
	SetFireTimer( 30.0f ) ;
	SetOnFire( false ) ;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/Oil.png") ;
	m_nAnimID = CAnimationManager::GetInstance()->GetID( "fire" ) ;
	m_nAnimImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/fire.png") ;
	CAnimationManager::GetInstance()->SetAnimTexture( m_nAnimID , m_nAnimImageID ) ;
}

void COil::Update(float fDT)
{
	CBaseObject::Update(fDT) ;

	if( GetOnFire() )
	{

		CAnimationManager::GetInstance()->UpdateAnimation( fDT , m_nAnimID ) ;
		// set other objects on fire
		SetFireTimer( GetFireTimer() - fDT ) ;
		if( GetFireTimer() <= 0 )
		{
			// check surrounding objects to see if they can catch on fire
			for( int i = -1 ; i <= 1 ; ++i )
			{
				for( int u = -1 ; u <= 1 ; ++u )
				{
					int item = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( this->GetIndexPosX() + i , this->GetIndexPosY() + u ) ;
					IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
					if( object )
					{
						if( object->GetType() == OBJ_OIL )
						{
							((COil*)object)->SetOnFire(true) ;
						}
					}
					int entityID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( this->GetIndexPosX() + i , this->GetIndexPosY() + u ) ;
					IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
					if( !entity )
						continue;
					if( entity->GetType() == ENT_PLAYER )
					{
						((CPlayer*)entity)->SetLives( ((CPlayer*)entity)->GetLives() - 1 ) ;
					}
				}
			}
		}


		SetLifeDuration( GetLifeDuration() - fDT ) ;
		if( GetLifeDuration() <= 0 )
		{
			// delete oil, and fire on this tile
			MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).SetInfoAtIndex( this->GetIndexPosX() , this->GetIndexPosY() , 0 ) ;
			MObjectManager::GetInstance()->RemoveUnit( this->m_nIdentificationNumber ) ;
			delete this ;
		}
	}
}

void COil::Render( int CameraPosX , int CameraPosY )
{
	CBaseObject::Render( CameraPosX , CameraPosY ) ;
	if( GetOnFire() )
		CAnimationManager::GetInstance()->Draw(m_nAnimID , GetPosX() - CameraPosX , GetPosY() - CameraPosY , .2 , .2 , 0 , 0 , 0 , 0xffffffff ) ;
}


void COil::SetOnFire( bool onFire )
{ 
	if( GetOnFire() == false && onFire == true )
		CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
	m_bOnFire = onFire ; 
}
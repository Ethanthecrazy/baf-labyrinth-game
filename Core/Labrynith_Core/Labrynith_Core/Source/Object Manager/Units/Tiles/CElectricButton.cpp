#include "CElectricButton.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../MObjectManager.h"
#include "../CPlayer.h"
#include "../Golems/CGolem_Water.h"
#include "../Golems/CGolem_Iron.h"
#include "CElectricButton.h"
#include "CMetal.h"

CElectricButton::CElectricButton(string nLink)
{
	CButton::CButton( nLink ) ;
	m_nType = OBJ_ELECTRICBUTTON ;
	SetPowered( false ) ;
	SetElectricUpdateTimer( 10.0f ) ;
}
CElectricButton::~CElectricButton(void)
{
	CButton::~CButton() ;
}

void CElectricButton::Update( float fDT )
{
		CBaseObject::Update(fDT) ;
	
	SetElectricUpdateTimer( GetElectricUpdateTimer() - fDT ) ;
	if( GetElectricUpdateTimer() <= 0 )
	{
		// check surrounding objects to see if they can catch on fire
		for( int i = -1 ; i <= 1 ; ++i )
		{
			for( int u = -1 ; u <= 1 ; ++u )
			{
				if( ( i == -1 && u != 0 ) || ( i == 1 && u != 0 ) || ( u == -1 && i != 0 ) || ( u == 1 && i != 0 ) )
					continue ;
				int item = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( this->GetIndexPosX() + i , this->GetIndexPosY() + u ) ;
				IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
				if( object )
				{
					if( object->GetType() == OBJ_METAL )
					{
						((CMetal*)object)->SetPowered(true) ;
					}
					else if( object->GetType() == OBJ_ELECTRICBUTTON )
					{
						((CElectricButton*)object)->SetPowered(true) ;
					}
					else if( object->GetType() == ENT_GOLEM )
					{
						if( ((CBaseGolem*)object)->GetGolemType() == WATER_GOLEM )
						{
							((CGolem_Water*)object)->SetPowered(true) ;
						}
						else if( ((CBaseGolem*)object)->GetGolemType() == IRON_GOLEM )
						{
							((CGolem_Iron*)object)->SetPowered(true) ;
						}
					}
				}
			}
		}
		int entityID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( this->GetIndexPosX(), this->GetIndexPosY() ) ;
		IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
		if( entity )
		{
			if( entity->GetType() == ENT_PLAYER )
			{
				((CPlayer*)entity)->SetLives( ((CPlayer*)entity)->GetLives() - 1 ) ;
			}
		}
	}
}

bool CElectricButton::CheckCollision(IUnitInterface* pBase)
{
	if( pBase == this )
		return false;
	
	SetPressed(!GetIsPressed()) ;

	if(GetIsPressed())
	{
		MEventSystem::GetInstance()->SendEvent("Button.Pressed", (void*)GetLink().c_str());
		CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/pressed.mp3"));
	}
	else
	{
		MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)GetLink().c_str());
		CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/un-pressed.mp3"));
	}

	return false;
}


void CElectricButton::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "Button.Pressed" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  GetLink())
			SetPressed( true ) ;
	}

	if( _toHandle->GetEventID() == "Button.Unpress" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  GetLink())
			SetPressed( false ) ;
	}
}
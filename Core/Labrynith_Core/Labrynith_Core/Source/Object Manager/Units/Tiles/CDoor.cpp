#include "CDoor.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Wrappers/CSGD_FModManager.h"

CDoor::CDoor(string nLink)
{
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_DOOR;
	m_bIsOpen = false;
	m_nLink = nLink;
	MEventSystem::GetInstance()->RegisterClient("Button.Pressed", this);
	MEventSystem::GetInstance()->RegisterClient("Button.Unpress", this);
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Closed.png" ));
}

CDoor::~CDoor(void)
{

}

void CDoor::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "Button.Pressed" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
		{
			m_bIsOpen = true;
			this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Open.png" ));
			CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/Door_Slide.wav"));
		}
	}

	if( _toHandle->GetEventID() == "Button.Unpress" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
		{
			m_bIsOpen = false;
			this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Closed.png" ));
			CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/Door_Slide.wav"));
		}
	}
}

bool CDoor::CheckCollision(IUnitInterface* pBase)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	if(m_bIsOpen)
	{
		printf("Door is open!\n");
		return false;
	}
	else
	{
		printf("Door is closed, cant go through\n");
		return true;
	}
}


void CDoor::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render(CameraPosX, CameraPosY);	
}
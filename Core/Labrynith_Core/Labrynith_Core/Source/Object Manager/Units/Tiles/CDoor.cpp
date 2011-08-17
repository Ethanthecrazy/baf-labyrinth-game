#include "CDoor.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/COptionsState.h"
#include "../../../GameStates/CGamePlayState.h"

CDoor::CDoor(string nLink)
{
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_DOOR;
	m_bIsOpen = false;
	m_nLink = nLink;
	MEventSystem::GetInstance()->RegisterClient("Button.Pressed", this);
	MEventSystem::GetInstance()->RegisterClient("Button.Unpress", this);
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Closed.png" ));
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	OpenSoundID = FM->LoadSound("resource/Sounds/Door_Slide.wav");
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(OpenSoundID, true);
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
			CSGD_FModManager::GetInstance()->PlaySound2D(OpenSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);
		}
	}

	if( _toHandle->GetEventID() == "Button.Unpress" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
		{
			m_bIsOpen = false;
			this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Closed.png" ));
			CSGD_FModManager::GetInstance()->PlaySound2D(OpenSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);
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
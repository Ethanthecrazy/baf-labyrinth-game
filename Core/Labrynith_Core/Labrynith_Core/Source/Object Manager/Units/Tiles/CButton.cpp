#include "CButton.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/COptionsState.h"
#include "../../../GameStates/CGamePlayState.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Wrappers/CSGD_TextureManager.h"

CButton::CButton(string nLink)
{
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_BUTTON;
	m_bLooksPressed = false;
	m_bIsPressed = false;
	m_nLink = nLink;

	MEventSystem::GetInstance()->RegisterClient("Button.Pressed", this);
	MEventSystem::GetInstance()->RegisterClient("Button.Unpress", this);
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	OpenSoundID = FM->LoadSound("resource/Sounds/pressed.mp3");
	CloseSoundID = FM->LoadSound("resource/Sounds/un-pressed.mp3");
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(OpenSoundID, true);
	Opt->AdjustSound(CloseSoundID, true);

	m_nIMG_Up = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/singleTile.png" );
	m_nIMG_Down = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/buttonDown.png" );

}
CButton::~CButton(void)
{	
	MEventSystem::GetInstance()->UnregisterClient("Button.Pressed", this);
	MEventSystem::GetInstance()->UnregisterClient("Button.Unpress", this);
}

bool CButton::CheckCollision(IUnitInterface* pBase)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	m_bIsPressed = !m_bIsPressed;
	m_bLooksPressed = !m_bLooksPressed;

	if(m_bIsPressed)
	{
		MEventSystem::GetInstance()->SendEvent("Button.Pressed", (void*)m_nLink.c_str());
	}
	else
	{
		MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)m_nLink.c_str());
	}

	if(m_bLooksPressed)
	{
		CSGD_FModManager::GetInstance()->PlaySound2D(OpenSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);
	}
	else
	{
		CSGD_FModManager::GetInstance()->PlaySound2D(OpenSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);
	}

	//printf("Steped on button\n");
	return false;
}

void CButton::Update(float fDT)
{	
	//MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)m_nLink.c_str());

	if( m_bLooksPressed )
		m_nImageID = m_nIMG_Down;
	else
		m_nImageID = m_nIMG_Up;
}


void CButton::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render(CameraPosX, CameraPosY);	
}


void CButton::HandleEvent( Event* _toHandle )
{	
	if( _toHandle->GetEventID() == "Button.Pressed" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
			m_bIsPressed = true;
	}

	if( _toHandle->GetEventID() == "Button.Unpress" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
			m_bIsPressed = false;
	}
}
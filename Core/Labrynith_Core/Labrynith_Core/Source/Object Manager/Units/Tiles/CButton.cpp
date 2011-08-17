#include "CButton.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/COptionsState.h"
#include "../../../Messaging/MEventSystem.h"

CButton::CButton(string nLink)
{
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_BUTTON;
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

	if(m_bIsPressed)
	{
		MEventSystem::GetInstance()->SendEvent("Button.Pressed", (void*)m_nLink.c_str());
		//CSGD_FModManager::GetInstance()->PlaySoundA(OpenSoundID);
	}
	else
	{
		MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)m_nLink.c_str());
		//CSGD_FModManager::GetInstance()->PlaySoundA(CloseSoundID);
	}

	//printf("Steped on button\n");
	return false;
}

void CButton::Update(float fDT)
{	
	//MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)m_nLink.c_str());
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
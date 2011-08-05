#include "CButton.h"
#include "../../../Wrappers/CSGD_FModManager.h"

#include "../../../Messaging/MEventSystem.h"

CButton::CButton(string nLink)
{
	m_nUnitType = OBJECT_BUTTON;
	m_nType = OBJ_BUTTON;
	m_bIsPressed = false;
	m_nLink = nLink;
}
CButton::~CButton(void)
{

}

bool CButton::CheckCollision(IUnitInterface* pBase)
{
	if( pBase == this )
		return false;

	m_bIsPressed = !m_bIsPressed;

	if(m_bIsPressed)
	{
		MEventSystem::GetInstance()->SendEvent("Button.Pressed", (void*)m_nLink.c_str());
		CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/pressed.mp3"));
	}
	else
	{
		MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)m_nLink.c_str());
		CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/un-pressed.mp3"));
	}

	//printf("Steped on button\n");
	return false;
}

void CButton::Update(float fDT)
{	
	//MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)m_nLink.c_str());
}
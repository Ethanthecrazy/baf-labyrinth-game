#include "CButton.h"

#include "../../../Messaging/MEventSystem.h"

CButton::CButton(string nLink)
{
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

	MEventSystem::GetInstance()->SendEvent("Button.Pressed", (void*)m_nLink.c_str());
	//printf("Steped on button\n");
	return false;
}

void CButton::Update(float fDT)
{	
	MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)m_nLink.c_str());
}
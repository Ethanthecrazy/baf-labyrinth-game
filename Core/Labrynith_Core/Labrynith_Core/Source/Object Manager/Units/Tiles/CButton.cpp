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
	MEventSystem::GetInstance()->SendEvent("Button.Pressed", (void*)m_nLink.c_str());
	printf("Steped on button\n");
	return false;
}
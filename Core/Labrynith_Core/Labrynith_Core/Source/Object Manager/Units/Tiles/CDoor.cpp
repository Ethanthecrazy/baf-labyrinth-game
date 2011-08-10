#include "CDoor.h"
#include "../../../Messaging/MEventSystem.h"

CDoor::CDoor(string nLink)
{
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_DOOR;
	m_bIsOpen = false;
	m_nLink = nLink;
	MEventSystem::GetInstance()->RegisterClient("Button.Pressed", this);
	MEventSystem::GetInstance()->RegisterClient("Button.Unpress", this);
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
			m_bIsOpen = true;
	}

	if( _toHandle->GetEventID() == "Button.Unpress" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
			m_bIsOpen = false;
	}
}

bool CDoor::CheckCollision(IUnitInterface* pBase)
{
	if( pBase == this )
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
	if(!m_bIsOpen)
		CBaseObject::Render(CameraPosX, CameraPosY);
	
}
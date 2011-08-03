#include "CDoor.h"


CDoor::CDoor(string nLink)
{
	m_bIsOpen = false;
	m_nLink = nLink;
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
}

bool CDoor::CheckCollision(IUnitInterface* pBase)
{
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
		CBaseEntity::Render(CameraPosX, CameraPosY);
	
}
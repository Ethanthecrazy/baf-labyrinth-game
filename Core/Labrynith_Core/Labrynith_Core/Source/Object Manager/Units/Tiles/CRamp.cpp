#include "CRamp.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"

CRamp::CRamp(string nLink) : m_nLink(nLink)
{	
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Exit.png" ));
	this->m_nType = OBJ_RAMP;
	
	MEventSystem::GetInstance()->RegisterClient("Ramp.Up", this);
	MEventSystem::GetInstance()->RegisterClient("Ramp.Down", this);
}

bool CRamp::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	return false;
}

void CRamp::HandleEvent( Event* _toHandle )
{	
	if( _toHandle->GetEventID() == "Ramp.Up" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
		{

		}
	}
	
	if( _toHandle->GetEventID() == "Ramp.Down" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
		{

		}
	}
}
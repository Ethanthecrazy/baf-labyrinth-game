#include "CRamp.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"

CRamp::CRamp(int nLink) : m_nLink(nLink)
{	
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Exit.png" ));
	this->m_nType = OBJ_RAMP;
}

bool CRamp::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	return false;
}

void CRamp::HandleEvent( Event* _toHandle )
{	
	if( _toHandle->GetEventID() == "Button.Pressed" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		//if( tmp ==  m_nLink)
			//m_bIsPressed = true;
	}
}
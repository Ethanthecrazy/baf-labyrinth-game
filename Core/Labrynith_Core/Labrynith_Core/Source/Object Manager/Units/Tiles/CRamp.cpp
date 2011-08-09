#include "CRamp.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../MObjectManager.h"

CRamp::CRamp(string direction)
{	
	this->m_nType = OBJ_RAMP;
	if(direction == "up")
	{
		Up = true;
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Stairs_Up.png" ));
	}
	else
	{
		Up = false;
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Stairs_Down.png" ));
	}
}

bool CRamp::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/moveFloor.wav"));

	if(Up)
	{
		MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
			MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetLayerID() - 1) );
	}
	else
	{
		MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
			MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetLayerID() + 1) );
	}

	return false;
}
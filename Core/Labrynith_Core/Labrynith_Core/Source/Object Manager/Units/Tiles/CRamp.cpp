#include "CRamp.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../MObjectManager.h"

CRamp::CRamp(string direction)
{	
	this->m_nType = OBJ_RAMP;
	m_nUnitType = OBJECT_TILE;
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
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/moveFloor.wav"));

	if(Up)
	{
		MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
			this->GetLayerLocation() - 1) );
	}
	else
	{
		MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
			this->GetLayerLocation() + 1) );
	}

	return false;
}
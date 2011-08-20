#include "CRamp.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../MObjectManager.h"
#include "../../../GameStates/COptionsState.h"
#include "../../../GameStates/CGamePlayState.h"
#include "../CBaseEntity.h"
#include "../CPlayer.h"

CRamp::CRamp(string direction, int nFloorlevel)// : floorlevel(nFloorlevel)
{	
	floorlevel = nFloorlevel;
	this->m_nType = OBJ_RAMP;
	m_nUnitType = OBJECT_TILE;
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	m_nMoveFloorSoundID = FM->LoadSound("resource/Sounds/moveFloor.wav");
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(m_nMoveFloorSoundID, true);
	if(direction == "up")
	{
		Up = true;
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Stairs_Up.png" ));
	}
	else if(direction == "down")
	{
		Up = false;
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Stairs_Down.png" ));
	}
}

bool CRamp::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this || !nCanHandleCollision || this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	CSGD_FModManager::GetInstance()->PlaySound2D(m_nMoveFloorSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);

	if(Up)
	{
		if(floorlevel == -1)
		{
			MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
				this->GetLayerLocation() - 1) );
		}
		else
		{
			MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
			floorlevel) );
		}
		MMessageSystem::GetInstance()->ProcessMessages();
	}
	else
	{
		if(floorlevel == -1)
		{
			MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
				this->GetLayerLocation() + 1) );
		}
		else
		{			
			MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
				floorlevel) );
		}
		MMessageSystem::GetInstance()->ProcessMessages();
	}

	return false;
}
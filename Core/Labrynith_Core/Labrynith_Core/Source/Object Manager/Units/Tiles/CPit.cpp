#include "CPit.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../MObjectManager.h"
#include "../CPlayer.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../../GameStates/CGamePlayState.h"
#include "../CBaseEntity.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/COptionsState.h"

CPit::CPit()
{
	m_nUnitType = OBJECT_TILE;
	this->m_nType = OBJ_PIT;
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	HurtSoundID = FM->LoadSound("resource/Sounds/hurt.mp3");
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(HurtSoundID, true);
}

bool CPit::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//IUnitInterface* temp = new CBaseEntity();
	//pBase->m_nIdentificationNumber = MObjectManager::GetInstance()->MoveUnit(pBase, this->GetLayerLocation() + 1 );
	//MObjectManager::GetInstance()->RemoveUnit( pBase->m_nIdentificationNumber );
	
	if( this->GetLayerLocation() != CGamePlayState::GetInstance()->GetNumLevelFloors() )
	{		
		CSGD_FModManager::GetInstance()->PlaySoundA(HurtSoundID);

		MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
			this->GetLayerLocation() + 1) );
	}
	else
	{
		// kill the entity
		if(pBase->GetType() == ENT_PLAYER)
			CGamePlayState::GetInstance()->KillPlayer();
		else
			MMessageSystem::GetInstance()->SendMsg( new msgRemoveUnit(pBase->m_nIdentificationNumber) );
	}

	return false;
}

void CPit::Render( int CameraPosX, int CameraPosY )
{	
	if( this->GetLayerLocation() != CGamePlayState::GetInstance()->GetNumLevelFloors() )		
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pitbelow.png" ));
	else
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pit.png" ));

	CBaseObject::Render(CameraPosX, CameraPosY);
}
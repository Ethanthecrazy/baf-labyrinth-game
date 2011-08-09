#include "CPit.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../MObjectManager.h"
#include "../CPlayer.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../../GameStates/CGamePlayState.h"
#include "../CBaseEntity.h"
#include "../../../Wrappers/CSGD_FModManager.h"

CPit::CPit()
{
	this->m_nType = OBJ_PIT;
}

bool CPit::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this || !nCanHandleCollision)
		return false;

	//IUnitInterface* temp = new CBaseEntity();
	//pBase->m_nIdentificationNumber = MObjectManager::GetInstance()->MoveUnit(pBase, MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetLayerID() + 1 );
	//MObjectManager::GetInstance()->RemoveUnit( pBase->m_nIdentificationNumber );
	
	if( MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetLayerID() != CGamePlayState::GetInstance()->GetNumLevelFloors() )
	{		
		CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/hurt.mp3"));

		MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
			MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetLayerID() + 1) );
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
	if( MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetLayerID() != CGamePlayState::GetInstance()->GetNumLevelFloors() )		
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pitbelow.png" ));
	else
		this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pit.png" ));

	CBaseObject::Render(CameraPosX, CameraPosY);
}
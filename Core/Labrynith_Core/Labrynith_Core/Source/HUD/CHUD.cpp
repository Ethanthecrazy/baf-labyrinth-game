#include "CHUD.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../Object Manager/MObjectManager.h"
#include "../Object Manager/Units/CPlayer.h"
#include "../Object Manager/Units/CBaseObject.h"
#include "../GameStates/CGamePlayState.h"
#include <iostream>
CHUD::CHUD()
{
	pPlayer = NULL;
	CSGD_TextureManager* TM = CSGD_TextureManager::GetInstance();
	HeartImageID = TM->LoadTexture( "resource/heart.png" );
	InvSlotImageID = TM->LoadTexture( "resource/InvSlot.png" );
}
CHUD::~CHUD()
{

}
void CHUD::Render()
{
	pPlayer = (CPlayer*)MObjectManager::GetInstance()->GetUnit( CGamePlayState::GetInstance()->testVaribale );
	
	if(pPlayer == NULL)
	{
		return;
	}
	
	CSGD_TextureManager* TM = CSGD_TextureManager::GetInstance();

	//Player HeldItem
	CBaseObject* pHeldItem = pPlayer->GetHeldItem();
	TM->Draw(InvSlotImageID, 770 - 30, 636 - 40);
	if(pHeldItem)
	{
		TM->Draw(pHeldItem->m_nImageID, 770 - 30, 636 - 40);
	}
	//Player EquippedItem
	CBaseObject* pEquipItem = pPlayer->GetEquippedItem();
	TM->Draw(InvSlotImageID, 834 - 30, 636 - 40);
	if(pEquipItem)
	{
		TM->Draw(pEquipItem->m_nImageID, 834 - 30, 636 - 40);
	}
	
}
CHUD* CHUD::GetInstance()
{
	static CHUD instance;
	return &instance;
}
void CHUD::SetPlayer(const int nID)
{
	IUnitInterface* obj = MObjectManager::GetInstance()->GetUnit(nID);
	//if this is a player object
	//BUG - need a way to determine if this obj
	// is the player or not
	if(obj->m_nUnitType == OBJECT_ENTITY)
	{
		if(obj->GetType() == ENT_PLAYER)
			pPlayer = (CPlayer*)obj;
	}
}
void CHUD::SetPlayer(CPlayer* player)
{
	pPlayer = player;
}
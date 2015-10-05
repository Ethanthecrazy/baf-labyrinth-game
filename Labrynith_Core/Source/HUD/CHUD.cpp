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
	m_nIMG_Hand = TM->LoadTexture( "resource/helditem.png" );
	m_nIMG_Armor = TM->LoadTexture( "resource/equippeditem.png" );;
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
	TM->Draw(InvSlotImageID, 670 - 30, 536 - 40);
	if(pHeldItem)
	{
		TM->Draw(pHeldItem->m_nImageID, 670 - 30, 536 - 40);
	}
	TM->Draw( m_nIMG_Hand, 670 - 30 - 10, 536 - 40 + 48 );
	//Player EquippedItem
	CBaseObject* pEquipItem = pPlayer->GetEquippedItem();
	TM->Draw(InvSlotImageID, 734 - 30, 536 - 40);
	if(pEquipItem)
	{
		TM->Draw(pEquipItem->m_nImageID, 734 - 30, 536 - 40);
	}
	TM->Draw( m_nIMG_Armor, 734 - 30 - 10, 536 - 40 + 48 );
	
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
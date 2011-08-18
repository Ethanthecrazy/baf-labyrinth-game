#include "CHUD.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../Object Manager/MObjectManager.h"
#include "../Object Manager/Units/CPlayer.h"
#include "../Object Manager/Units/CBaseObject.h"

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
	if(pPlayer == NULL)
		return;

	CSGD_TextureManager* TM = CSGD_TextureManager::GetInstance();

	//Player Lives
	for(int i = 0; i < pPlayer->GetLives(); i++)
	{
		TM->Draw(HeartImageID,(i * TILE_WIDTH) + 30, 40);
	}

	//Player HeldItem
	CBaseObject* pHeldItem = pPlayer->GetHeldItem();
	TM->Draw(InvSlotImageID, 670 - 30, 536 - 40);
	if(pHeldItem)
	{
		TM->Draw(pHeldItem->m_nImageID, 686 - 30, 552 - 40);
	}
	//Player EquippedItem
	CBaseObject* pEquipItem = pPlayer->GetEquippedItem();
	TM->Draw(InvSlotImageID, 734 - 30, 536 - 40);
	if(pEquipItem)
	{
		TM->Draw(pEquipItem->m_nImageID, 750 - 30, 552 - 40);
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
	if(obj->GetType() == OBJECT_ENTITY)
	{
		pPlayer = (CPlayer*)obj;
	}
}
void CHUD::SetPlayer(const CPlayer* player)
{
	pPlayer = ((CPlayer*)(player));
}
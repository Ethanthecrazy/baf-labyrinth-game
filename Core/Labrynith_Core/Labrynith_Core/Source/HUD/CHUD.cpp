#include "CHUD.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../Object Manager/MObjectManager.h"
#include "../Object Manager/Units/CPlayer.h"
#include "../Object Manager/Units/CBaseObject.h"

CHUD::CHUD()
{
	pPlayer = NULL;
	HeartImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/heart.png" );
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
		TM->Draw(HeartImageID,(i * 32), 0);
	}

	//Player HeldItem
	CBaseObject* pHeldItem = pPlayer->GetHeldItem();
	if(pHeldItem)
	{
		TM->Draw(pHeldItem->m_nImageID, 700, 564);
	}
	//Player EquippedItem
	CBaseObject* pEquipItem = pPlayer->GetEquippedItem();
	if(pEquipItem)
	{
		TM->Draw(pEquipItem->m_nImageID, 750, 564);
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
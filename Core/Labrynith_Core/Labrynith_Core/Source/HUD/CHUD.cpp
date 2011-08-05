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

	//Player Lives
	for(int i = 0; i < pPlayer->GetLives(); i++)
	{
		CSGD_TextureManager::GetInstance()->Draw(HeartImageID,(i * 32), 0);
	}

	//Player HeldItem
	if(pPlayer->GetHeldItem())
		pPlayer->GetHeldItem()->Render(568, 368);
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
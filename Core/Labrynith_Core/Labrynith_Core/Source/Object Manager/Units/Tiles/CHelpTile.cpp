#include "CHelpTile.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../CGame.h"
#include "../../../GameStates/CHelpState.h"

CHelpTile::CHelpTile(string sHelpText)
{
	m_sHelpText = sHelpText;
	m_nUnitType = OBJECT_TILE;
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/HelpTile.png" ));
	this->m_nType = OBJ_HELPER;
}

bool CHelpTile::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	if(!pBase || pBase == this || !nCanHandleCollision || this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;
	
	if(pBase->GetType() == ENT_PLAYER)
	{
		CGame::GetInstance()->PushState( CHelpState::GetInstance(m_sHelpText) );
	}

	return false;
}
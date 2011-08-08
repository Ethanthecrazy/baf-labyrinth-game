#include "CPit.h"
#include "../../../Wrappers/CSGD_TextureManager.h"

CPit::CPit()
{
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Exit.png" ));
	this->m_nType = OBJ_PIT;
}

bool CPit::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	return false;
}
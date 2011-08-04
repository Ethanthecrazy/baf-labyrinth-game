#include "CExit.h"
#include "../../../Wrappers/CSGD_TextureManager.h"

CExit::CExit()
{
	CBaseObject::CBaseObject();
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Exit.png" ));
	this->m_nType = OBJ_EXIT;
}
CExit::~CExit()
{
	CBaseObject::~CBaseObject();
}

void CExit::Update(float fDT)
{

}
void CExit::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render(CameraPosX, CameraPosY);
}
bool CExit::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	return false;
}
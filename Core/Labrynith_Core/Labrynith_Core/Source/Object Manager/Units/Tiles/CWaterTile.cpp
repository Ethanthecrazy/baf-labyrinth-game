#include "CWaterTile.h"
#include "../../../Wrappers/CSGD_TextureManager.h"

CWaterTile::CWaterTile()
{
	CBaseObject::CBaseObject();
	this->m_nIceImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/iceTile.png" ));
	this->m_nWaterImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/waterTile.png" ));
	this->m_nType = OBJ_WATER;
	SetIsFrozen(false);
}
CWaterTile::CWaterTile(bool bIsFrozen)
{
	CBaseObject::CBaseObject();
	this->m_nIceImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/iceTile.png" ));
	this->m_nWaterImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/waterTile.png" ));
	this->m_nType = OBJ_WATER;
	SetIsFrozen(bIsFrozen);
}
CWaterTile::~CWaterTile()
{

}

void CWaterTile::Update(float fDT)
{

}
void CWaterTile::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render(CameraPosX, CameraPosY);
}
bool CWaterTile::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	return false;
}
//accessors
bool CWaterTile::IsFrozen()
{
	return m_bIsFrozen;
}
//mutators
void CWaterTile::SetIsFrozen(const bool bIsFrozen)
{
	m_bIsFrozen = bIsFrozen;
	if(IsFrozen())
		this->m_nImageID = m_nIceImageID;
	else
		this->m_nImageID = m_nWaterImageID;
}

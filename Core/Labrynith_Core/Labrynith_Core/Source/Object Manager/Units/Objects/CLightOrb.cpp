#include "CLightOrb.h"
#include "../../MObjectManager.h"
#include "../../../Wrappers/CSGD_TextureManager.h"

CLightOrb::CLightOrb(void)
{
	m_nType = OBJ_LIGHTORB;
	m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/LightOrb.png" ));
}

void CLightOrb::Update(float fDT)
{
	CBaseObject::Update(fDT);
	
	MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber )
		.GetFlake( OBJECT_LIGHT ).SetInfoAtIndex(GetIndexPosX(), GetIndexPosY(), rand() % 15 + 150 );
}
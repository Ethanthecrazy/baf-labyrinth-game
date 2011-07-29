#include "CBaseGolem.h"

CBaseGolem::CBaseGolem(void)
{
	CBaseEntity::CBaseEntity();
}
CBaseGolem::~CBaseGolem(void)
{
	CBaseEntity::~CBaseEntity();
}

void CBaseGolem::Update(float fDT)
{
	CBaseEntity::Update(fDT);
}
void CBaseGolem::Render( int CameraPosX, int CameraPosY )
{
	CBaseEntity::Render(CameraPosX, CameraPosY);
}
void CBaseGolem::UpdateAI()
{

}

int CBaseGolem::GetGolemType() const
{
	return m_nGolemType;
}
void CBaseGolem::SetGolemType(const int nGolemType)
{
	m_nGolemType = nGolemType; 
}
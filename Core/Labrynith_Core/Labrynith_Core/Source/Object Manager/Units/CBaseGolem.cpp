#include "../MObjectManager.h"
#include "CBaseGolem.h"

CBaseGolem::CBaseGolem(void)
{
	CBaseEntity::CBaseEntity();
	fCollectedTime = 0.0f;
}
CBaseGolem::~CBaseGolem(void)
{
	CBaseEntity::~CBaseEntity();
}

void CBaseGolem::Update(float fDT)
{
	CBaseEntity::Update(fDT);
	fCollectedTime += fDT;
	if(fCollectedTime >= 2.0f)
	{
		MObjectManager* OM = MObjectManager::GetInstance();
		//random movement
		switch(rand() % 4)
		{
		case 0:
			{
				//MoveDown
				OM->MoveEntDown( this->m_nIdentificationNumber );
			}
			break;

		case 1:
			{
				//MoveLeft
				OM->MoveEntLeft( this->m_nIdentificationNumber );
			}
			break;

		case 2:
			{
				//MoveRight
				OM->MoveEntRight( this->m_nIdentificationNumber );
			}
			break;

		case 3:
			{
				//MoveUp
				OM->MoveEntUp( this->m_nIdentificationNumber );
			}
			break;
		}
		fCollectedTime = 0.0f;
	}
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
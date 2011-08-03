#include "../MObjectManager.h"
#include "../../AI Handler/CAI_Handler.h"
#include "CBaseGolem.h"

CBaseGolem::CBaseGolem(void)
{
	CBaseEntity::CBaseEntity();
	fCollectedTime = 0.0f;
	m_nType = ENT_GOLEM;
	ClearTarget();
}
CBaseGolem::~CBaseGolem(void)
{
	CBaseEntity::~CBaseEntity();
}

void CBaseGolem::Update(float fDT)
{
	CBaseEntity::Update(fDT);
	fCollectedTime += fDT;
	if(fCollectedTime > 1.0f)
	{
		
		fCollectedTime = 0.0f;
	}
	UpdateAI();
}
void CBaseGolem::Render( int CameraPosX, int CameraPosY )
{
	CBaseEntity::Render(CameraPosX, CameraPosY);
}
void CBaseGolem::UpdateAI()
{
	//CAI_Handler::GetInstance()->RandomMove(this);
	CAI_Handler::GetInstance()->MoveToPos(this);
}
void CBaseGolem::ClearTarget()
{
	SetTargetPos(-1, -1);
}
bool CBaseGolem::HasTarget()
{
	if(GetTargetPosX() < 0 ||
		GetTargetPosY() < 0)
		return false;

	return true;
}

int CBaseGolem::GetGolemType() const
{
	return m_nGolemType;
}
int CBaseGolem::GetTargetPosX() const
{
	return m_nTargetX;
}
int CBaseGolem::GetTargetPosY() const
{
	return m_nTargetY;
}
void CBaseGolem::SetGolemType(const int nGolemType)
{
	m_nGolemType = nGolemType; 
}
void CBaseGolem::SetTargetPosX(const int nTargetX)
{
	if(nTargetX < -1)
		return;

	m_nTargetX = nTargetX;
}
void CBaseGolem::SetTargetPosY(const int nTargetY)
{
	if(nTargetY < -1)
		return;

	m_nTargetY = nTargetY;
}
void CBaseGolem::SetTargetPos(const int nTargetX, const int nTargetY)
{
	SetTargetPosX(nTargetX);
	SetTargetPosY(nTargetY);
}
void CBaseGolem::SetMoveType(const int nMovementType)
{
	m_nMovementType = nMovementType;
}
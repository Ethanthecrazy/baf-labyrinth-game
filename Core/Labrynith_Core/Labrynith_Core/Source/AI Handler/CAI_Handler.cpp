#include "CAI_Handler.h"
#include "../Object Manager/MObjectManager.h"
#include "../Object Manager/Units/CBaseGolem.h"

//singleton
CAI_Handler::CAI_Handler()
{

}
CAI_Handler::~CAI_Handler()
{

}
//helpers
	
CAI_Handler* CAI_Handler::GetInstance()
{
	static CAI_Handler instance;
	return &instance;
}
void CAI_Handler::MoveToPos(CBaseGolem* pGolem)
{
	//let the golem finish its current movement, if it has one
	if(pGolem->GetFlag_MovementState() == FLAG_MOVESTATE_MOVING)
		return;

	//if we dont have a target, leave
	if(!pGolem->HasTarget())
		return;

	//if we are at our destination, theres no point in doing work
	if(pGolem->GetIndexPosX() == pGolem->GetTargetPosX()
	   && pGolem->GetIndexPosY() == pGolem->GetTargetPosY())
	{
		//we've reached our destination
		pGolem->ClearTarget();
		return;
	}

	MObjectManager* OM = MObjectManager::GetInstance();

	//Vertical Movement
	if(pGolem->GetIndexPosY() > pGolem->GetTargetPosY())
	{
		//we are below our target, move up
		OM->MoveEntUp( pGolem->m_nIdentificationNumber );
		return;
	}
	else if(pGolem->GetIndexPosY() < pGolem->GetTargetPosY())
	{
		//we are above our target, move down
		OM->MoveEntDown( pGolem->m_nIdentificationNumber );
		return;
	}

	//Horizontal Movement
	if(pGolem->GetIndexPosX() > pGolem->GetTargetPosX())
	{
		//the target is to the left
		OM->MoveEntLeft( pGolem->m_nIdentificationNumber );
		return;
	}
	else if(pGolem->GetIndexPosX() < pGolem->GetTargetPosX())
	{
		//the target is to the right
		OM->MoveEntRight( pGolem->m_nIdentificationNumber );
		return;
	}
}
void CAI_Handler::RandomMove(CBaseGolem* pGolem)
{
	//let the golem finish its current movement, if it has one
	if(pGolem->GetFlag_MovementState() == FLAG_MOVESTATE_MOVING)
		return;

	MObjectManager* OM = MObjectManager::GetInstance();
	//random movement
	switch(rand() % 4)
	{
	case 0:
		{
			//MoveDown
			OM->MoveEntDown( pGolem->m_nIdentificationNumber );
		}
		break;

	case 1:
		{
			//MoveLeft
			OM->MoveEntLeft( pGolem->m_nIdentificationNumber );
		}
		break;

	case 2:
		{
			//MoveRight
			OM->MoveEntRight( pGolem->m_nIdentificationNumber );
		}
		break;

	case 3:
		{
			//MoveUp
			OM->MoveEntUp( pGolem->m_nIdentificationNumber );
		}
		break;
	}
}
//excutes the ai process
void CAI_Handler::ExecuteAI(CBaseGolem* pGolem)
{

}
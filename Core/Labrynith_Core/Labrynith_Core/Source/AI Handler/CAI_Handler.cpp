#include "CAI_Handler.h"
#include <iostream>
#include "../Object Manager/MObjectManager.h"

#include "../Object Manager/Units/CBaseGolem.h"

//singleton
CAI_Handler::CAI_Handler()
{

}
CAI_Handler::~CAI_Handler()
{
	m_vEntities.clear();
}
//helpers
bool CAI_Handler::HorizontalMove(const CBaseEntity* pEntity, const int nTargetX)
{
	//see where the target is from our position...
	if(((CBaseEntity*)(pEntity))->GetIndexPosX() > nTargetX)
	{
		//Move Left
		return CardinalMove(pEntity, FLAG_MOVE_LEFT);
	}
	else if(((CBaseEntity*)(pEntity))->GetIndexPosX() < nTargetX)
	{
		//Move Right
		return CardinalMove(pEntity, FLAG_MOVE_RIGHT);
	}
	return false;
}
bool CAI_Handler::VerticalMove(const CBaseEntity* pEntity, const int nTargetY)
{
	//see where the target is from our position...
	if(((CBaseEntity*)(pEntity))->GetIndexPosY() > nTargetY)
	{
		//Move Up
		return CardinalMove(pEntity, FLAG_MOVE_UP);
	}
	else if(((CBaseEntity*)(pEntity))->GetIndexPosY() < nTargetY)
	{
		//Move Down
		return CardinalMove(pEntity, FLAG_MOVE_DOWN);
	}
	return false;
}	
int CAI_Handler::GetVertDirectionToTarget(int nPosY, tTarget target)
{
	if(nPosY < target.m_nY)
	{
		//target is to the below us
		return DIRDOWN;
	}
	else if(nPosY > target.m_nY)
	{
		//target is to the above us
		return DIRUP;
	}
	//our target is neither above or below us
	//it must be to the left, right, or we are at the target
	return -1;
}
int CAI_Handler::GetHorzDirectionToTarget(int nPosX, tTarget target)
{
	if(nPosX > target.m_nX)
	{
		//target is to the left of us
		return DIRLEFT;
	}
	else if(nPosX < target.m_nX)
	{
		//target is to the right of us
		return DIRRIGHT;
	}
	//our target is neither to the left or the right of us
	//it must be above, below, or we are at the target
	return -1;
}

CAI_Handler* CAI_Handler::GetInstance()
{
	static CAI_Handler instance;
	return &instance;
}
int CAI_Handler::FindEntityInList(const CBaseEntity* pEntity)
{
	//Check to see if this enitity is in the list
	int EntID = ((CBaseEntity*)(pEntity))->m_nIdentificationNumber;
	unsigned int i;
	for(i = 0; i < m_vEntities.size(); i++)
	{
		if(m_vEntities[i]->m_nEntityID == EntID)
			return i;
	}
	//we didnt find the entity
	return -1;
}
void CAI_Handler::SetPrimaryTarget(const CBaseEntity* pEntity, int nX, int nY)
{
	//Check to see if this enitity is in the list
	int i = FindEntityInList(pEntity);
	//if we didnt find the entity..
	if(i == -1)
	{
		//Assign the entity an ID
		//its size before we add a new one is its id
		((CBaseEntity*)(pEntity))->SetAI_ID(m_vEntities.size());
		//add it to the list, including its primary target
		int EntID = ((CBaseEntity*)(pEntity))->m_nIdentificationNumber;
		tTarget target = tTarget(nX, nY, true);
		tEntityInfo* entityinfo = new tEntityInfo(EntID, target);
		m_vEntities.push_back(entityinfo);
		return;
	}
	//if we make it here we found the entity
	//we are setting a new primary target, clear all the temp targets
	m_vEntities[i]->m_vTargets.clear();
	//add the new primary target
	tTarget target = tTarget(nX, nY, true);
	m_vEntities[i]->m_vTargets.push_back(target);
	//reset the current target we are looking at
	m_vEntities[i]->nCurrTarget = 0;
}
void CAI_Handler::ClearTargets(int nAI_ID)
{
	if(nAI_ID < 0)
		return;

	if(nAI_ID > (int)m_vEntities.size())
		return;

	m_vEntities[nAI_ID]->m_vTargets.clear();
}
void CAI_Handler::ClearEntityList()
{
	m_vEntities.clear();
}
bool CAI_Handler::CheckCollisions(const CBaseEntity* pEntity, const int nX, 
		const int nY, bool nCanHandleCollision)
{
	if(!pEntity)
		return true;

	MObjectManager* OM = MObjectManager::GetInstance();
	
	//Dont allow the Entitys to move out of range
	//Right
	if( nX > OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerWidth() - 1 )
	{
		cout << "AI:Right Movement Denied" << endl;
		return true;
	}
	//Left
	if(nX < 0)
	{
		cout << "AI:Left Movement Denied" << endl;
		return true;
	}
	//Up
	if(nY < 0)
	{
		cout << "AI:Up Movement Denied" << endl;
		return true;
	}
	//Down
	if( nY > OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerHeight() - 1 )
	{
		cout << "AI:Down Movement Denied" << endl;
		return true;
	}

	//Its possible for an object and entity and tile to occupy the same position!
	//Check to see if we are colliding with a Tile
	int TileID = OM->FindLayer(pEntity->m_nIdentificationNumber)
		 .GetFlake(OBJECT_TILE).GetInfoAtIndex(nX, nY);

	//if we cannot pass that tile leave
	bool Collided = ((CBaseEntity*)(pEntity))->CheckTileCollision(TileID);
	if(Collided)
	{
		return Collided;
	}

	//Check to see if we are colliding with an object
	int objectID = OM->FindLayer(pEntity->m_nIdentificationNumber)
		.GetFlake(OBJECT_OBJECT).GetInfoAtIndex(nX, nY);
	if( objectID > 0 )
	{
		cout << "AI:Collided With Object " << objectID << "\n";
		Collided = ((CBaseEntity*)(pEntity))->CheckCollision(OM->GetUnit(objectID), nCanHandleCollision);
		//Let the Entity handle its object collision
		if(Collided)
		{
			return Collided;
		}
	}

	//Check to see if we are colliding with an entity
	int EntityID = OM->FindFlake(pEntity->m_nIdentificationNumber)
		.GetInfoAtIndex(nX, nY);

	//we cannot collide with ourselves
	if( EntityID > 0 && OM->GetUnit(EntityID) != pEntity )
	{
		cout << "AI:Collided With Entity " << EntityID << "\n";
		//Let the Entity handle its collision
		Collided = ((CBaseEntity*)(pEntity))->CheckCollision(OM->GetUnit(EntityID), nCanHandleCollision);
		if(Collided)
		{
			return Collided;
		}
	}

	//Check to see if we are colliding with a button
	int buttonID = OM->FindLayer(pEntity->m_nIdentificationNumber)
		.GetFlake(OBJECT_BUTTON).GetInfoAtIndex(nX, nY);
	if( buttonID > 0 )
	{
		cout << "AI:Collided With Button " << buttonID << "\n";
		Collided = ((CBaseEntity*)(pEntity))->CheckCollision(OM->GetUnit(buttonID), nCanHandleCollision);
		//Let the Entity handle its object collision
		if(Collided)
		{
			return Collided;
		}
	}

	//we are not colliding with anything
	return false;
}
int CAI_Handler::GetDirectionToTarget(const CBaseEntity* pEntity)
{
	int index = ((CBaseEntity*)(pEntity))->GetAI_ID();
	int nPosX = ((CBaseEntity*)(pEntity))->GetIndexPosX();
	int nPosY = ((CBaseEntity*)(pEntity))->GetIndexPosY();
	tTarget currTarget = m_vEntities[index]->GetCurrentTarget();
	int nDirection = ((CBaseEntity*)(pEntity))->GetFlag_DirectionToMove();
	bool isHorizontal;
	switch(nDirection)
	{
	case DIRUP:
		{
			isHorizontal = false;
		}
		break;

	case DIRDOWN:
		{
			isHorizontal = false;
		}
		break;

	case DIRLEFT:
		{
			isHorizontal = true;
		}
		break;

	case DIRRIGHT:
		{
			isHorizontal = true;
		}
		break;

	default:
		{
			//Leave if those directions arent hit
			//direction is invalid
			return -2;
		}
		break;
	};
	if(isHorizontal)
	{
		//horizontal check
		nDirection = GetHorzDirectionToTarget(nPosX, currTarget);
		if(nDirection == -1)
		{
			return GetVertDirectionToTarget(nPosY, currTarget);
		}
		return nDirection;
	}
	else
	{
		//vertical check
		nDirection = GetVertDirectionToTarget(nPosY, currTarget);
		if(nDirection == -1)
		{
			return GetHorzDirectionToTarget(nPosX, currTarget);
		}
		return nDirection;
	}
}
int CAI_Handler::CheckWallDistance(const CBaseEntity* pEntity, int nPosX,
	                               int nPosY, const int nDirection)
{
	int nDistance = 0;
	int index = ((CBaseEntity*)(pEntity))->GetAI_ID();
	MObjectManager* OM = MObjectManager::GetInstance();

	switch(nDirection)
	{
	case FLAG_MOVE_UP:
		{
			//check up wall distance
			while(CheckCollisions(pEntity, nPosX, nPosY--, false))
			{
				//if we check out of bounds leave
				if(nPosY <= 0)
					return -1;

				++nDistance;
			}
			return nDistance;
		}
		break;

	case FLAG_MOVE_DOWN:
		{
			//check down wall distance
			while(CheckCollisions(pEntity, nPosX, nPosY++, false))
			{
				//if we check out of bounds leave
				if(nPosY >= OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerHeight())
					return -1;

				++nDistance;
			}
			return nDistance;
		}
		break;

	case FLAG_MOVE_LEFT:
		{
			//check left wall distance first
			while(CheckCollisions(pEntity, nPosX--, nPosY, false))
			{
				//if we check out of bounds leave
				if(nPosX <= 0)
					return -1;

				++nDistance;
			}
			return nDistance;
		}
		break;

	case FLAG_MOVE_RIGHT:
		{
			//check right wall distance first
			while(CheckCollisions(pEntity, nPosX++, nPosY, false))
			{
				//if we check out of bounds leave
				if(nPosX >= OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerWidth())
					return -1;

				++nDistance;
			}
			return nDistance;
		}
		break;

	default:
		{
			//Leave if those directions arent hit
			//direction is invalid
			return -1;
		}
		break;
	};
}
void CAI_Handler::DoExitCollision(const CBaseEntity* pEntity, bool nCanHandleCollision)
{
	MObjectManager* OM = MObjectManager::GetInstance();
	int nX = ((CBaseEntity*)(pEntity))->GetIndexPosX(); 
	int nY = ((CBaseEntity*)(pEntity))->GetIndexPosY();

	//Check to see if we are colliding with an object
	int objectID = OM->FindLayer(pEntity->m_nIdentificationNumber)
		.GetFlake(OBJECT_OBJECT).GetInfoAtIndex(nX, nY);
	if( objectID > 0 )
	{
		//Let the Entity handle its collision
		((CBaseEntity*)(pEntity))->ExitCollision(OM->GetUnit(objectID), nCanHandleCollision);
	}

	//Check to see if we are colliding with an entity
	int EntityID = OM->FindFlake(pEntity->m_nIdentificationNumber)
		.GetInfoAtIndex(nX, nY);

	//we cannot collide with ourselves
	if( EntityID > 0 && OM->GetUnit(EntityID) != pEntity )
	{
		//Let the Entity handle its collision
		((CBaseEntity*)(pEntity))->ExitCollision(OM->GetUnit(EntityID), nCanHandleCollision);
	}

	//Check to see if we are colliding with a button
	int buttonID = OM->FindLayer(pEntity->m_nIdentificationNumber)
		.GetFlake(OBJECT_BUTTON).GetInfoAtIndex(nX, nY);
	if( buttonID > 0 )
	{
		//Let the Entity handle its object collision
		((CBaseEntity*)(pEntity))->ExitCollision(OM->GetUnit(buttonID), nCanHandleCollision);
	}
}
int CAI_Handler::CheckPath(const CBaseEntity* pEntity, const int nDirection,
	tTarget& target)
{
	int StartDirection = nDirection;
	bool isHorizontal;
	int index = ((CBaseEntity*)(pEntity))->GetAI_ID();
	int offsetX = 0, offsetY = 0;
	int nX = ((CBaseEntity*)(pEntity))->GetIndexPosX(); 
	int nY = ((CBaseEntity*)(pEntity))->GetIndexPosY();
	int nDistance;

	switch(nDirection)
	{
	case DIRUP:
		{
			offsetY -= 1;
			isHorizontal = 0;
		}
		break;

	case DIRDOWN:
		{
			offsetY += 1;
			isHorizontal = 0;
		}
		break;

	case DIRLEFT:
		{
		    offsetX -= 1;
			isHorizontal = 1;
		}
		break;

	case DIRRIGHT:
		{
		    offsetX += 1;
			isHorizontal = 1;
		}
		break;

	default:
		{
			//Leave if those directions arent hit
			//direction is invalid
			return false;
		}
		break;
	};
}
bool CAI_Handler::CardinalMove(const CBaseEntity* pEntity, const int nDirection)
{
	//let the Entity finish its current movement, if it has one
	if(((CBaseEntity*)(pEntity))->GetFlag_MovementState() == FLAG_MOVESTATE_MOVING)
		return false;

	MObjectManager* OM = MObjectManager::GetInstance();

	//determine which direction we are looking at
	int nX = ((CBaseEntity*)(pEntity))->GetIndexPosX(); 
	int nY = ((CBaseEntity*)(pEntity))->GetIndexPosY();
	switch(nDirection)
	{
	case FLAG_MOVE_UP:
		{
			nY -= 1;
		}
		break;

	case FLAG_MOVE_DOWN:
		{
			nY += 1;
		}
		break;

	case FLAG_MOVE_LEFT:
		{
		    nX -= 1;
		}
		break;

	case FLAG_MOVE_RIGHT:
		{
		    nX += 1;
		}
		break;

	default:
		{
			//Leave if those directions arent hit
			//direction is invalid
			return false;
		}
		break;
	};
	//Allow us to face in the correct direction
	//regardless of collision
	((CBaseEntity*)(pEntity))->SetFlag_DirectionToMove(nDirection);
	//Check to see if we are colliding with anything
	bool isColliding = CheckCollisions(pEntity, nX, nY, true);
	if(!isColliding)
	{
		// POSSIBLE BUG
		//Check the object we are standing on
		//only if we are able to move
		//CheckCollisions(pEntity, ((CBaseEntity*)pEntity)->GetIndexPosX(), ((CBaseEntity*)pEntity)->GetIndexPosY(), true);
		DoExitCollision(pEntity, true);
		//we can move if we're not colliding with something
		cout << "AI:" << ((CBaseEntity*)(pEntity))->GetIndexPosX() 
			<< ", " << ((CBaseEntity*)(pEntity))->GetIndexPosY() << endl;
		//Let ObjectManager know where we are
		OM->FindFlake(pEntity->m_nIdentificationNumber).SetInfoAtIndex(nX, nY, pEntity->m_nIdentificationNumber);
		((CBaseEntity*)(pEntity))->SetFlag_MovementState( FLAG_MOVESTATE_MOVING );
		//Change Entity index as we are now moving
		((CBaseEntity*)(pEntity))->SetIndexPosX(nX);
		((CBaseEntity*)(pEntity))->SetIndexPosY(nY);
		((CBaseEntity*)(pEntity))->SetDistanceLeft( 32.0f );
	}
	return !isColliding;
}
void CAI_Handler::MoveToPos(const CBaseEntity* pEntity)
{
	//let the Entity finish its current movement, if it has one
	if(((CBaseEntity*)(pEntity))->GetFlag_MovementState() == FLAG_MOVESTATE_MOVING)
		return;

	//if we dont have a target, leave
	if(!((CBaseEntity*)(pEntity))->HasTarget())
		return;

	//if we are at our destination, theres no point in doing work
	if(((CBaseEntity*)(pEntity))->GetIndexPosX() == pEntity->GetTargetPosX()
	   && ((CBaseEntity*)(pEntity))->GetIndexPosY() == pEntity->GetTargetPosY())
	{
		//we've reached our destination
		((CBaseEntity*)(pEntity))->ClearTarget();
		return;
	}

	int index = ((CBaseEntity*)(pEntity))->GetAI_ID();
	if(m_vEntities[index]->HasCurrentTarget())
	{
		tTarget currTarget = m_vEntities[index]->GetCurrentTarget();
		int nPosX = ((CBaseEntity*)(pEntity))->GetIndexPosX();
		int nPosY = ((CBaseEntity*)(pEntity))->GetIndexPosY();

		//Horizontal Movement
		if(HorizontalMove(pEntity, currTarget.m_nX))
			return;

		//Vertical Movement
		//if movement fails try a different direction
		if(VerticalMove(pEntity, currTarget.m_nY))
			return;

		//Get direction to our target
		int nDirToTarget = GetDirectionToTarget(pEntity);
		//if we are aiming for a temp target
		//and if we hit that target
		if(m_vEntities[index]->nCurrTarget > 0
			&& nDirToTarget == -1)
		{
			//We hit our target get rid of it
			m_vEntities[index]->m_vTargets.erase(m_vEntities[index]->m_vTargets.begin()
				+ m_vEntities[index]->nCurrTarget);
			//start aiming for our prevous target
			m_vEntities[index]->nCurrTarget -= 1;
			return;
		}

		if(nDirToTarget == -2)
		{
			//something happened leave
			return;
		}
		//if the above fails we hit a wall
		//we need to get a new/temp target in order to reach our primary one
		GetNewTarget(pEntity, GetDirectionToTarget(pEntity));

	}
}
void CAI_Handler::RandomMove(const CBaseEntity* pEntity)
{
	//let the Entity finish its current movement, if it has one
	if(((CBaseEntity*)(pEntity))->GetFlag_MovementState() == FLAG_MOVESTATE_MOVING)
		return;

	MObjectManager* OM = MObjectManager::GetInstance();
	//random movement
	switch(rand() % 5)
	{
	case FLAG_MOVE_DOWN:
		{
			CardinalMove(pEntity, FLAG_MOVE_DOWN);
		}
		break;

	case FLAG_MOVE_LEFT:
		{
			CardinalMove(pEntity, FLAG_MOVE_LEFT);
		}
		break;

	case FLAG_MOVE_RIGHT:
		{
			CardinalMove(pEntity, FLAG_MOVE_RIGHT);
		}
		break;

	case FLAG_MOVE_UP:
		{
			CardinalMove(pEntity, FLAG_MOVE_UP);
		}
		break;
	}
}
bool CAI_Handler::GetNewTarget(const CBaseEntity* pEntity, const int nDirection)
{
	MObjectManager* OM = MObjectManager::GetInstance();
	int index = ((CBaseEntity*)(pEntity))->GetAI_ID();
	int offsetX = 0, offsetY = 0;
	int nX = ((CBaseEntity*)(pEntity))->GetIndexPosX(); 
	int nY = ((CBaseEntity*)(pEntity))->GetIndexPosY();
	int nDistanceDown, nDistanceUp;
	int nDistanceRight, nDistanceLeft;
	int isHorizontal;
	//get the direction
	switch(nDirection)
	{
	case DIRUP:
		{
			offsetY -= 1;
			isHorizontal = 0;
		}
		break;

	case DIRDOWN:
		{
			offsetY += 1;
			isHorizontal = 0;
		}
		break;

	case DIRLEFT:
		{
		    offsetX -= 1;
			isHorizontal = 1;
		}
		break;

	case DIRRIGHT:
		{
		    offsetX += 1;
			isHorizontal = 1;
		}
		break;

	default:
		{
			//Leave if those directions arent hit
			//direction is invalid
			return false;
		}
		break;
	};

	
	if(isHorizontal == 1)
	{	
		//check down wall distance first
		nDistanceDown = CheckWallDistance(pEntity, nX + offsetX, nY + offsetY, FLAG_MOVE_DOWN);
		//check up wall distance now
		nDistanceUp = CheckWallDistance(pEntity, nX + offsetX, nY + offsetY, FLAG_MOVE_UP);
		//if there is no wall our direction is off
		if(nDistanceDown == 0 &&
			nDistanceUp == 0)
		{
			return false;//GetNewTarget(pEntity, ((CBaseEntity*)(pEntity))->GetFlag_DirectionToMove());
		}
		//if both directions are out of bounds, leave
		else if(nDistanceDown == -1 &&
			nDistanceUp == -1)
		{
			return false;
		}

		//If the distance is -1, something happened so go the
		//other direction instead
		if(nDistanceUp == -1)
		{
			//new Down target
			tTarget newtarget = tTarget(nX + offsetX, 
				((CBaseEntity*)(pEntity))->GetIndexPosY() + nDistanceDown, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}
		else if(nDistanceDown == -1)
		{
			//new Up target
			tTarget newtarget = tTarget(nX + offsetX, 
				((CBaseEntity*)(pEntity))->GetIndexPosY() - nDistanceUp, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}

		//Entitys favor down movement so up distance has to be smaller
		//for a Entity to move up
		if((nDistanceDown <= nDistanceUp && nDistanceDown > -1))
		{
			//new Down target
			tTarget newtarget = tTarget(nX + offsetX, 
				((CBaseEntity*)(pEntity))->GetIndexPosY() + nDistanceDown, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}
		else if((nDistanceDown > nDistanceUp && nDistanceUp > -1))
		{
			//new Up target
			tTarget newtarget = tTarget(nX + offsetX, 
				((CBaseEntity*)(pEntity))->GetIndexPosY() - nDistanceUp, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}
		return false;
	}
	else
	{
		//check right wall distance first
		nDistanceRight = CheckWallDistance(pEntity, nX + offsetX, nY + offsetY, FLAG_MOVE_RIGHT);
		//check left wall distance now
		nDistanceLeft = CheckWallDistance(pEntity, nX + offsetX, nY + offsetY, FLAG_MOVE_LEFT);

		//if there is no wall our direction is off
		if(nDistanceRight == 0 &&
			nDistanceLeft == 0)
		{
			return false;//GetNewTarget(pEntity, ((CBaseEntity*)(pEntity))->GetFlag_DirectionToMove());
		}
		//if both directions are out of bounds, leave
		else if(nDistanceRight == -1 &&
			nDistanceLeft == -1)
		{
			return false;
		}

		//If the distance is -1, something happened so go the
		//other direction instead
		if(nDistanceRight == -1)
		{
			//new Left Target
			tTarget newtarget = tTarget(((CBaseEntity*)(pEntity))->GetIndexPosX() - nDistanceLeft, 
				nY + offsetY, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}
		else if(nDistanceLeft == -1)
		{
			//new Right target
			tTarget newtarget = tTarget(((CBaseEntity*)(pEntity))->GetIndexPosX() + nDistanceRight, 
				nY + offsetY, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}

		//Entitys favor right movement so left distance has to be smaller
		//for an Entity to move left
		if((nDistanceRight <= nDistanceLeft))
		{
			//new Right target
			tTarget newtarget = tTarget(((CBaseEntity*)(pEntity))->GetIndexPosX() + nDistanceRight, 
				nY + offsetY, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}
		else if((nDistanceRight > nDistanceLeft))
		{
			//new Left Target
			tTarget newtarget = tTarget(((CBaseEntity*)(pEntity))->GetIndexPosX() - nDistanceLeft, 
				nY + offsetY, false);
			m_vEntities[index]->AddNewTarget(newtarget);
			return true;
		}
		return false;
	}
}

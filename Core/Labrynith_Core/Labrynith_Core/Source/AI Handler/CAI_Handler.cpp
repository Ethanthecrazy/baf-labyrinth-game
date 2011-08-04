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

}
//helpers
bool CAI_Handler::HorizontalMove(const CBaseGolem* pGolem, const int nTargetX)
{
	//see where the target is from our position...
	if(((CBaseGolem*)(pGolem))->GetIndexPosX() > nTargetX)
	{
		//Move Left
		return CardinalMove(pGolem, FLAG_MOVE_LEFT);
	}
	else if(((CBaseGolem*)(pGolem))->GetIndexPosX() < nTargetX)
	{
		//Move Right
		return CardinalMove(pGolem, FLAG_MOVE_RIGHT);
	}
	return false;
}
bool CAI_Handler::VerticalMove(const CBaseGolem* pGolem, const int nTargetY)
{
	//see where the target is from our position...
	if(((CBaseGolem*)(pGolem))->GetIndexPosY() > nTargetY)
	{
		//Move Up
		return CardinalMove(pGolem, FLAG_MOVE_UP);
	}
	else if(((CBaseGolem*)(pGolem))->GetIndexPosY() < nTargetY)
	{
		//Move Down
		return CardinalMove(pGolem, FLAG_MOVE_DOWN);
	}
	return false;
}	

CAI_Handler* CAI_Handler::GetInstance()
{
	static CAI_Handler instance;
	return &instance;
}
bool CAI_Handler::CheckCollisions(const CBaseGolem* pGolem, const int nX, 
		const int nY, bool nCanHandleCollision)
{
	MObjectManager* OM = MObjectManager::GetInstance();
	
	//Dont allow the golems to move out of range
	//Right
	if( nX > OM->FindLayer(pGolem->m_nIdentificationNumber).GetLayerWidth() - 1 )
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
	if( nY > OM->FindLayer(pGolem->m_nIdentificationNumber).GetLayerHeight() - 1 )
	{
		cout << "AI:Down Movement Denied" << endl;
		return true;
	}

	//Its possible for an object/entity and tile to occupy the same position!
	//Check to see if we are colliding with a Tile
	int TileID = OM->FindLayer(pGolem->m_nIdentificationNumber)
		 .GetFlake(OBJECT_TILE).GetInfoAtIndex(nX, nY);
	//BUG - 0 is temp code
	// 
	if(TileID == 0)
	{
		//cout << "AI:Collided With Tile " << "\n";
		if(nCanHandleCollision)
		{
			//BUG - Let the golem handle its tile collision
			//BUG - Only return if the golem cannot pass that tile
			//(which should be handled by the golem)
			return true;//((CBaseGolem*)(pGolem))->CheckCollision(OM->GetUnit(TileID));
		}
		else
			return true;
	}

	//Check to see if we are colliding with an object
	int objectID = OM->FindLayer(pGolem->m_nIdentificationNumber)
		.GetFlake(OBJECT_OBJECT).GetInfoAtIndex(nX, nY);
	if( objectID > 0 )
	{
		cout << "AI:Collided With Object " << objectID << "\n";
		if(nCanHandleCollision)
		{
			//Let the golem handle its object collision
			return ((CBaseGolem*)(pGolem))->CheckCollision(OM->GetUnit(objectID));
		}
		else
			return true;
	}

	//Check to see if we are colliding with an entity
	int EntityID = OM->FindFlake(pGolem->m_nIdentificationNumber)
		.GetInfoAtIndex(nX, nY);
	if( EntityID > 0 )
	{
		cout << "AI:Collided With Entity " << EntityID << "\n";
		if(nCanHandleCollision)
		{
			//Let the golem handle its object collision
			return ((CBaseGolem*)(pGolem))->CheckCollision(OM->GetUnit(EntityID));
		}
		else
			return true;
	}

	//we are not colliding with anything
	return false;
}
bool CAI_Handler::CardinalMove(const CBaseGolem* pGolem, const int nDirection)
{
	MObjectManager* OM = MObjectManager::GetInstance();
	//determine which direction we are looking at
	int nX = ((CBaseGolem*)(pGolem))->GetIndexPosX(); 
	int nY = ((CBaseGolem*)(pGolem))->GetIndexPosY();
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
	((CBaseGolem*)(pGolem))->SetFlag_DirectionToMove(nDirection);
	//Check to see if we are colliding with anything
	bool isColliding = CheckCollisions(pGolem, nX, nY, true);
	if(!isColliding)
	{
		//we can move if we're not colliding with something
		cout << "AI:" << ((CBaseGolem*)(pGolem))->GetIndexPosX() 
			<< ", " << ((CBaseGolem*)(pGolem))->GetIndexPosY() << endl;
		//Let ObjectManager know where we are
		OM->FindFlake(pGolem->m_nIdentificationNumber).SetInfoAtIndex(nX, nY, pGolem->m_nIdentificationNumber);
		((CBaseGolem*)(pGolem))->SetFlag_MovementState( FLAG_MOVESTATE_MOVING );
		//Change Golem index as we are now moving
		((CBaseGolem*)(pGolem))->SetIndexPosX(nX);
		((CBaseGolem*)(pGolem))->SetIndexPosY(nY);
		((CBaseGolem*)(pGolem))->SetDistanceLeft( 32.0f );
	}
	return !isColliding;
}
void CAI_Handler::MoveToPos(const CBaseGolem* pGolem)
{
	//let the golem finish its current movement, if it has one
	if(((CBaseGolem*)(pGolem))->GetFlag_MovementState() == FLAG_MOVESTATE_MOVING)
		return;

	//if we dont have a target, leave
	if(!((CBaseGolem*)(pGolem))->HasTarget())
		return;

	//if the target is on a tile/object that cannot 
	//be passed target is invalid
	if(CheckCollisions(pGolem, pGolem->GetTargetPosX(),
		pGolem->GetTargetPosY(), false))
	{
		((CBaseGolem*)(pGolem))->ClearTarget();
		return;
	}

	//if we are at our destination, theres no point in doing work
	if(((CBaseGolem*)(pGolem))->GetIndexPosX() == pGolem->GetTargetPosX()
	   && ((CBaseGolem*)(pGolem))->GetIndexPosY() == pGolem->GetTargetPosY())
	{
		//we've reached our destination
		((CBaseGolem*)(pGolem))->ClearTarget();
		return;
	}

	//if we have a new/temp target, we must
	//get to that target first
	if(((CBaseGolem*)(pGolem))->HasNewTarget())
	{
		//if the new target is on a tile/object that cannot 
		//be passed
		if(CheckCollisions(pGolem, pGolem->m_nNewTargetX,
			pGolem->m_nNewTargetY, false))
		{
			((CBaseGolem*)(pGolem))->ClearNewTarget();
			return;
		}

		//Horizontal Movement
		if(HorizontalMove(pGolem, pGolem->m_nNewTargetX))
			return;

		//Vertical Movement
		if(VerticalMove(pGolem, pGolem->m_nNewTargetY))
			return;

		//if we hit this target we may continue toward
		//our real target
		if(((CBaseGolem*)(pGolem))->GetIndexPosX() == pGolem->m_nNewTargetX
	     && ((CBaseGolem*)(pGolem))->GetIndexPosY() == pGolem->m_nNewTargetY)
		{
			((CBaseGolem*)(pGolem))->ClearNewTarget();
			return;
		}

		return;
	}

	MObjectManager* OM = MObjectManager::GetInstance();

	//Horizontal Movement
	//if movement fails try a different direction
	if(HorizontalMove(pGolem, pGolem->GetTargetPosX()))
		return;

	//Vertical Movement
	//if all directions fail we hit a wall
	//we need to navigate around it
	if(VerticalMove(pGolem, pGolem->GetTargetPosY()))
		return;

	//Get our new/temp target in order to reach our primary one
	//we need to get the direction the target is from the golem
	switch(((CBaseGolem*)(pGolem))->GetFlag_DirectionToMove())
	{
		case FLAG_MOVE_DOWN:
		{ 
			GetNewTarget(pGolem, FLAG_MOVE_DOWN);
		}
		break;

		case FLAG_MOVE_LEFT:
		{
			GetNewTarget(pGolem, FLAG_MOVE_LEFT);
		}
		break;

		case FLAG_MOVE_RIGHT:
		{
			GetNewTarget(pGolem, FLAG_MOVE_RIGHT);
		}
		break;

		case FLAG_MOVE_UP:
		{
			GetNewTarget(pGolem, FLAG_MOVE_UP);
		}
		break;
	}
}
void CAI_Handler::RandomMove(const CBaseGolem* pGolem)
{
	//let the golem finish its current movement, if it has one
	if(((CBaseGolem*)(pGolem))->GetFlag_MovementState() == FLAG_MOVESTATE_MOVING)
		return;

	MObjectManager* OM = MObjectManager::GetInstance();
	//random movement
	switch(rand() % 5)
	{
	case FLAG_MOVE_DOWN:
		{
			CardinalMove(pGolem, FLAG_MOVE_DOWN);
		}
		break;

	case FLAG_MOVE_LEFT:
		{
			CardinalMove(pGolem, FLAG_MOVE_LEFT);
		}
		break;

	case FLAG_MOVE_RIGHT:
		{
			CardinalMove(pGolem, FLAG_MOVE_RIGHT);
		}
		break;

	case FLAG_MOVE_UP:
		{
			CardinalMove(pGolem, FLAG_MOVE_UP);
		}
		break;
	}
}
void CAI_Handler::GetNewTarget(const CBaseGolem* pGolem, const int nDirection)
{
	MObjectManager* OM = MObjectManager::GetInstance();
	int offsetX = 0, offsetY = 0;
	bool isHorizontal;
	//get the direction
	switch(nDirection)
	{
	case FLAG_MOVE_UP:
		{
			offsetY -= 1;
			isHorizontal = false;
		}
		break;

	case FLAG_MOVE_DOWN:
		{
			offsetY += 1;
			isHorizontal = false;
		}
		break;

	case FLAG_MOVE_LEFT:
		{
		    offsetX -= 1;
			isHorizontal = true;
		}
		break;

	case FLAG_MOVE_RIGHT:
		{
		    offsetX += 1;
			isHorizontal = true;
		}
		break;
	};

	int nX = ((CBaseGolem*)(pGolem))->GetIndexPosX(); 
	int nY = ((CBaseGolem*)(pGolem))->GetIndexPosY();
	int distanceX = 0, distanceY = 0;
	if(isHorizontal)
	{
		//check down wall distance first
		while(CheckCollisions(pGolem, nX + offsetX, nY++, false) &&
			nY < OM->FindLayer(pGolem->m_nIdentificationNumber).GetLayerHeight())
		{
			++distanceX;
		}
		//check up wall distance now
		nY = ((CBaseGolem*)(pGolem))->GetIndexPosY();
		while(CheckCollisions(pGolem, nX + offsetX, nY--, false) &&
			nY > 0)
		{
			++distanceY;
		}
		//if we check past range the target cannot be reached
		if((((CBaseGolem*)(pGolem))->GetIndexPosY() + distanceX >= 
			OM->FindLayer(pGolem->m_nIdentificationNumber).GetLayerHeight() - 1) // right check
			|| (((CBaseGolem*)(pGolem))->GetIndexPosY() - distanceY <= 0))  //left check
		{
			((CBaseGolem*)(pGolem))->ClearTarget();
		}
		//golems favor down movement so up distance has to be smaller
		//for a golem to move up
		if(distanceX < distanceY)
		{
			//new Down target
			((CBaseGolem*)(pGolem))->SetNewTargetPos(nX + offsetX,
				((CBaseGolem*)(pGolem))->GetIndexPosY() + distanceX);
		}
		else
		{
			//new Up target
			((CBaseGolem*)(pGolem))->SetNewTargetPos(nX + offsetX,
				((CBaseGolem*)(pGolem))->GetIndexPosY() - distanceY);
		}
	}
	else
	{
		//check right wall distance first
		while(CheckCollisions(pGolem, nX++, nY + offsetY, false) &&
			nX < OM->FindLayer(pGolem->m_nIdentificationNumber).GetLayerWidth())
		{
			++distanceX;
		}
		//check left wall distance now
		int nX = ((CBaseGolem*)(pGolem))->GetIndexPosX();
		while(CheckCollisions(pGolem, nX--, nY + offsetY, false) &&
			nX > 0)
		{
			++distanceY;
		}
		//if we check past range the target cannot be reached
		if((((CBaseGolem*)(pGolem))->GetIndexPosX() + distanceX >= 
			OM->FindLayer(pGolem->m_nIdentificationNumber).GetLayerWidth() - 1) // right check
			|| (((CBaseGolem*)(pGolem))->GetIndexPosX() - distanceY <= 0))  //left check
		{
			((CBaseGolem*)(pGolem))->ClearTarget();
		}
		//golems favor right movement so left distance has to be smaller
		//for a golem to move left
		if(distanceX < distanceY)
		{
			//new Right target
			((CBaseGolem*)(pGolem))->SetNewTargetPos(((CBaseGolem*)(pGolem))->GetIndexPosX() + distanceX,
				nY + offsetY);
		}
		else
		{
			//new Left Target
			((CBaseGolem*)(pGolem))->SetNewTargetPos(((CBaseGolem*)(pGolem))->GetIndexPosX() - distanceY,
				nY + offsetY);
		}
	}
}

#include "CAI_Handler.h"
#include <iostream>
#include "../Object Manager/MObjectManager.h"
#
#include "../Object Manager/Units/CBaseGolem.h"

//singleton
CAI_Handler::CAI_Handler()
{

}
CAI_Handler::~CAI_Handler()
{

}
//helpers
bool CAI_Handler::HorizontalMove(const CBaseEntity* pEntity, const int nTargetX)
{
	//BUG equal to is missing
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
	//BUG equal to is missing
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

CAI_Handler* CAI_Handler::GetInstance()
{
	static CAI_Handler instance;
	return &instance;
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

	//Check to see if we are colliding with an entity
	int EntityID = OM->FindFlake(pEntity->m_nIdentificationNumber)
		.GetInfoAtIndex(nX, nY);

	//we cannot collide with ourselves
	if(OM->GetUnit(EntityID) == pEntity)
		return false;

	if( EntityID > 0 )
	{
		cout << "AI:Collided With Entity " << EntityID << "\n";
		//Let the Entity handle its collision
		return ((CBaseEntity*)(pEntity))->CheckCollision(OM->GetUnit(EntityID), nCanHandleCollision);
	}

	//we are not colliding with anything
	return false;
}
bool CAI_Handler::CardinalMove(const CBaseEntity* pEntity, const int nDirection)
{
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
		CheckCollisions(pEntity, ((CBaseEntity*)pEntity)->GetIndexPosX(), ((CBaseEntity*)pEntity)->GetIndexPosY(), true);

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

	//if the target is on a tile/object that cannot 
	//be passed target is invalid
	if(CheckCollisions(pEntity, pEntity->GetTargetPosX(),
		pEntity->GetTargetPosY(), false))
	{
		((CBaseEntity*)(pEntity))->ClearTarget();
		return;
	}

	//if we are at our destination, theres no point in doing work
	if(((CBaseEntity*)(pEntity))->GetIndexPosX() == pEntity->GetTargetPosX()
	   && ((CBaseEntity*)(pEntity))->GetIndexPosY() == pEntity->GetTargetPosY())
	{
		//we've reached our destination
		((CBaseEntity*)(pEntity))->ClearTarget();
		return;
	}

	//if we have a new/temp target, we must
	//get to that target first
	if(((CBaseEntity*)(pEntity))->HasNewTarget())
	{
		//if the new target is on a tile/object that cannot 
		//be passed
		if(CheckCollisions(pEntity, pEntity->m_nNewTargetX,
			pEntity->m_nNewTargetY, false))
		{
			((CBaseEntity*)(pEntity))->ClearNewTarget();
			return;
		}

		//Horizontal Movement
		if(HorizontalMove(pEntity, pEntity->m_nNewTargetX))
			return;

		//Vertical Movement
		if(VerticalMove(pEntity, pEntity->m_nNewTargetY))
			return;

		//if we hit this target we may continue toward
		//our real target
		if(((CBaseEntity*)(pEntity))->GetIndexPosX() == pEntity->m_nNewTargetX
	     && ((CBaseEntity*)(pEntity))->GetIndexPosY() == pEntity->m_nNewTargetY)
		{
			((CBaseEntity*)(pEntity))->ClearNewTarget();
			return;
		}

		return;
	}

	MObjectManager* OM = MObjectManager::GetInstance();

	//Horizontal Movement
	//if movement fails try a different direction
	if(HorizontalMove(pEntity, pEntity->GetTargetPosX()))
		return;

	//Vertical Movement
	//if all directions fail we hit a wall
	//we need to navigate around it
	if(VerticalMove(pEntity, pEntity->GetTargetPosY()))
		return;

	//Get our new/temp target in order to reach our primary one
	//we need to get the direction the target is from the Entity
	switch(((CBaseEntity*)(pEntity))->GetFlag_DirectionToMove())
	{
		case FLAG_MOVE_DOWN:
		{ 
			GetNewTarget(pEntity, FLAG_MOVE_DOWN);
		}
		break;

		case FLAG_MOVE_LEFT:
		{
			GetNewTarget(pEntity, FLAG_MOVE_LEFT);
		}
		break;

		case FLAG_MOVE_RIGHT:
		{
			GetNewTarget(pEntity, FLAG_MOVE_RIGHT);
		}
		break;

		case FLAG_MOVE_UP:
		{
			GetNewTarget(pEntity, FLAG_MOVE_UP);
		}
		break;
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
void CAI_Handler::GetNewTarget(const CBaseEntity* pEntity, const int nDirection)
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

	int nX = ((CBaseEntity*)(pEntity))->GetIndexPosX(); 
	int nY = ((CBaseEntity*)(pEntity))->GetIndexPosY();
	int distanceX = 0, distanceY = 0;
	if(isHorizontal)
	{
		//check down wall distance first
		while(CheckCollisions(pEntity, nX + offsetX, nY++, false) &&
			nY < OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerHeight())
		{
			++distanceX;
		}
		//check up wall distance now
		nY = ((CBaseEntity*)(pEntity))->GetIndexPosY();
		while(CheckCollisions(pEntity, nX + offsetX, nY--, false) &&
			nY > 0)
		{
			++distanceY;
		}
		//if we check past range the target cannot be reached
		if((((CBaseEntity*)(pEntity))->GetIndexPosY() + distanceX >= 
			OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerHeight() - 1) // right check
			|| (((CBaseEntity*)(pEntity))->GetIndexPosY() - distanceY <= 0))  //left check
		{
			((CBaseEntity*)(pEntity))->ClearTarget();
		}
		//Entitys favor down movement so up distance has to be smaller
		//for a Entity to move up
		if(distanceX < distanceY)
		{
			//new Down target
			((CBaseEntity*)(pEntity))->SetNewTargetPos(nX + offsetX,
				((CBaseEntity*)(pEntity))->GetIndexPosY() + distanceX);
		}
		else
		{
			//new Up target
			((CBaseEntity*)(pEntity))->SetNewTargetPos(nX + offsetX,
				((CBaseEntity*)(pEntity))->GetIndexPosY() - distanceY);
		}
	}
	else
	{
		//check right wall distance first
		while(CheckCollisions(pEntity, nX++, nY + offsetY, false) &&
			nX < OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerWidth())
		{
			++distanceX;
		}
		//check left wall distance now
		int nX = ((CBaseEntity*)(pEntity))->GetIndexPosX();
		while(CheckCollisions(pEntity, nX--, nY + offsetY, false) &&
			nX > 0)
		{
			++distanceY;
		}
		//if we check past range the target cannot be reached
		if((((CBaseEntity*)(pEntity))->GetIndexPosX() + distanceX >= 
			OM->FindLayer(pEntity->m_nIdentificationNumber).GetLayerWidth() - 1) // right check
			|| (((CBaseEntity*)(pEntity))->GetIndexPosX() - distanceY <= 0))  //left check
		{
			((CBaseEntity*)(pEntity))->ClearTarget();
		}
		//Entitys favor right movement so left distance has to be smaller
		//for a Entity to move left
		if(distanceX < distanceY)
		{
			//new Right target
			((CBaseEntity*)(pEntity))->SetNewTargetPos(((CBaseEntity*)(pEntity))->GetIndexPosX() + distanceX,
				nY + offsetY);
		}
		else
		{
			//new Left Target
			((CBaseEntity*)(pEntity))->SetNewTargetPos(((CBaseEntity*)(pEntity))->GetIndexPosX() - distanceY,
				nY + offsetY);
		}
	}
}

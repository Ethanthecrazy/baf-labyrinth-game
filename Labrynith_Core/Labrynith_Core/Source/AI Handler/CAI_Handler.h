#ifndef CAI_HANDLER
#define CAI_HANDLER

#include <vector>
class IUnitInterface;
class CBaseEntity;
class CBaseGolem;

enum DIRECTIONS{DIRUP = 1, DIRDOWN, DIRLEFT, DIRRIGHT};

class CAI_Handler
{
	//structs
	//used to store targets
	struct tTarget
	{
		int m_nY;
		int m_nX;
		bool bIsPrimary;
		tTarget()
		{
			m_nX = -1;
			m_nY = -1;
			bIsPrimary = false;
		}
		tTarget(int X, int Y, bool isPrimary)
		{
			m_nX = X;
			m_nY = Y;
			bIsPrimary = isPrimary;
		}
		bool operator==(tTarget target)
		{
			if(m_nY == target.m_nY
			&& m_nX == target.m_nX
			&& bIsPrimary == target.bIsPrimary)
			{
				return true;
			}
			return false;
		}
	};
	//store entity info for pathfinding
	struct tEntityInfo
	{
		//all the targets our entity must get to
		//in order to reach its primary one
		std::vector<tTarget> m_vTargets;
		int m_nEntityID, nCurrTarget;
		tEntityInfo()
		{
			m_nEntityID = 0;
			nCurrTarget = 0;
		}
		tEntityInfo(int nID, tTarget target)
		{
			m_nEntityID = nID;
			nCurrTarget = 0;
			m_vTargets.push_back(target);
		}
		bool HasCurrentTarget()
		{
		if(m_vTargets[nCurrTarget].m_nX < 0 ||
			m_vTargets[nCurrTarget].m_nY < 0)
			return false;

		return true;
		}
		tTarget GetCurrentTarget()
		{
			return m_vTargets[nCurrTarget];
		}
		void AddNewTarget(tTarget target)
		{
			//start going toward the new target
			m_vTargets.push_back(target);
			nCurrTarget += 1;
		}
	};

	//data members
	//list of entities
	std::vector<tEntityInfo*> m_vEntities; 

	//singleton
	CAI_Handler();
	CAI_Handler(const CAI_Handler&) { }
	~CAI_Handler();
	//helpers
	int FindEntityInList(const CBaseEntity* pEntity);
	bool HorizontalMove(const CBaseEntity* pEntity, const int nTargetX);
	bool VerticalMove(const CBaseEntity* pEntity, const int nTargetY);
	int GetVertDirectionToTarget(int nPosY, tTarget target);
	int GetHorzDirectionToTarget(int nPosX, tTarget target);
	int GetDirectionToTarget(const CBaseEntity* pEntity);
    bool GetNewTarget(const CBaseEntity* pEntity, const int nDirection);
	void SetNewTarget(const CBaseEntity* pEntity);	
	int CheckWallDistance(const CBaseEntity* pEntity, int nPosX, 
		                  int nPosY, const int nDirection);
	bool CheckPathDistance(const CBaseEntity* pEntity, const int nDirection, int& nDistance,
						  int nWallDist);
	bool CheckPath(const CBaseEntity* pEntity, const int nDirection,
			      int nNumChecks, tTarget& startPos);
public:

	static CAI_Handler* GetInstance();
	//Checks to see if an object is in that position
	//bool- determines weather to allow the Entity to handle
	// its own collisions
	bool CheckCollisions(const IUnitInterface* pEntity, const int nX, 
		const int nY, bool nCanHandleCollision);
	//called when an entity is no longer colliding with an object
	void DoExitCollision(const IUnitInterface* pEntity, bool nCanHandleCollision);
	//checks for collisions with an entity in the specified range
	void CheckCollisionRange(const IUnitInterface* pEntity, const unsigned int nRange);
	//moves in the 4 cardinal dircetions
	bool CardinalMove(const CBaseEntity* pEntity, const int nDirection);
	//used to move toward a certain position(target)
	void MoveToPos(const CBaseEntity* pEntity);
	//moves randomly around the area
	void RandomMove(const CBaseEntity* pEntity);
	//Sets the main target that object is after
	void SetPrimaryTarget(const CBaseEntity* pEntity, int nX, int nY);
	//Clears all primary and temp targets that entity may have
	void ClearTargets(int nAI_ID);
	//clears the entire list of entities
	void ClearEntityList();
};
#endif
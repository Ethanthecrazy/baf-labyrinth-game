#ifndef CAI_HANDLER
#define CAI_HANDLER

#include <vector>
class CBaseEntity;

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
	int CheckPathDistance(const CBaseEntity* pEntity, const int nDirection);
	bool CheckPath(const CBaseEntity* pEntity, const int nDirection,
			      const int StartDirection, tTarget& startPos, bool useStartDir);
public:

	static CAI_Handler* GetInstance();
	//Checks to see if an object is in that position
	//bool- determines weather to allow the Entity to handle
	// its own collisions
	bool CheckCollisions(const CBaseEntity* pEntity, const int nX, 
		const int nY, bool nCanHandleCollision);
	void DoExitCollision(const CBaseEntity* pEntity, bool nCanHandleCollision);
	bool CardinalMove(const CBaseEntity* pEntity, const int nDirection);
	void MoveToPos(const CBaseEntity* pEntity);
	void RandomMove(const CBaseEntity* pEntity);
	void SetPrimaryTarget(const CBaseEntity* pEntity, int nX, int nY);
	void ClearTargets(int nAI_ID);
	void ClearEntityList();
};
#endif
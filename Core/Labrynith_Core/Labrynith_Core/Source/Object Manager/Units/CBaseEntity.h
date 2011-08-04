#ifndef CBaseEntity_h_
#define CBaseEntity_h_

#include "../IUnitInterface.h"
#include <vector>

class CAI_Handler;
class CBaseEntity : public IUnitInterface
{
	unsigned int m_uiRefCount;
	int m_nCurrAnimID;
	std::vector<int> m_vMovementAnimIDs;
	bool PlayAnimWhileStill;
	//target to walk to, -1 is no target
	int m_nTargetX, m_nTargetY;
	//this is for the AI Handler class
	int m_nNewTargetX, m_nNewTargetY;
protected:
	int m_nType;
	//these are also for ai dont touch!
	int GetNewTargetPosX() const;
	int GetNewTargetPosY() const;
	void SetNewTargetPosX(const int nTargetX);
	void SetNewTargetPosY(const int nTargetY);
	void SetNewTargetPos(const int nTargetX, const int nTargetY);
	void ClearNewTarget();
	bool HasNewTarget();
	friend class CAI_Handler;
public:
	int m_nImageID;

	CBaseEntity(void);
	virtual ~CBaseEntity(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	//this function handles object/entity collisions
	virtual bool CheckCollision(IUnitInterface* pBase);
	//this function determines if we can walk thro certain tiles
	virtual bool CheckTileCollision(int TileID);
	//loads the animations for basic movement
	void LoadEntMoveAnimIDs();
	void ClearTarget();
	bool HasTarget();
	void AddRef(void);
	void Release(void);

	//accessors
	int GetType(void) { return m_nType; }
	int GetCurrentAnimID(void) const { return m_nCurrAnimID; }
	int GetTargetPosX() const;
	int GetTargetPosY() const;
	//mutators
	void SetAnimID(const int nAnimID);
	void SetImageID(const int nImageID);
	void SetPlayAnimWhileStill(const bool bCanAnim)
	{PlayAnimWhileStill = bCanAnim;}
	void SetTargetPosX(const int nTargetX);
	void SetTargetPosY(const int nTargetY);
	void SetTargetPos(const int nTargetX, const int nTargetY);
	void SetFlag_DirectionToMove( int newFlag );	
};

#endif // CBase_h__
#ifndef CBaseEntity_h_
#define CBaseEntity_h_

#include "../IUnitInterface.h"
#include <vector>

class CAI_Handler;
enum AI_TYPE{AI_LOW = 0, AI_MED, AI_HIGH};
class CBaseEntity : public IUnitInterface
{
	unsigned int m_uiRefCount;
	int m_nCurrAnimID;
	std::vector<int> m_vMovementAnimIDs;
	bool PlayAnimWhileStill;
	//target to walk to, -1 is no target
	int m_nTargetX, m_nTargetY;
	int m_nAI_ID;
	int nAI_Type;
	void SetTargetPosX(const int nTargetX);
	void SetTargetPosY(const int nTargetY);
protected:
	int m_nType;
	friend class CAI_Handler;
public:
	int m_nImageID;

	CBaseEntity(void);
	virtual ~CBaseEntity(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	//this function handles object/entity collisions
	virtual bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	//this function is called when we are no longer colliding with an object
	//the object we are "leaving" collision with is sent
	virtual void ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision);
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
	int GetAI_ID() const;
	//mutators
	void SetAnimID(const int nAnimID);
	void SetImageID(const int nImageID);
	void SetAI_ID(const int nAI_ID);
	void SetPlayAnimWhileStill(const bool bCanAnim)
	{PlayAnimWhileStill = bCanAnim;}
	void SetTargetPos(const int nTargetX, const int nTargetY);
	void SetFlag_DirectionToMove( int newFlag );	
};

#endif // CBase_h__
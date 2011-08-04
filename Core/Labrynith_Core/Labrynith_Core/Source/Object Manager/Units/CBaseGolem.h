#ifndef CBASEGOLEM_H_
#define CBASEGOLEM_H_

#include "CBaseEntity.h"
#include "../../Messaging/IListener.h"

enum MOVETYPES {NO_MOVE, RAND_MOVE, TARGET_MOVE,
				NUM_MOVE_TYPES};


class CAI_Handler;
class CBaseGolem : public CBaseEntity , public IListener
{
	//target to walk to, -1 is no target
	int m_nTargetX, m_nTargetY;
	//this is for the AI Handler class
	int m_nNewTargetX, m_nNewTargetY;
	int m_nGolemType;
	int m_nMovementType;
	int m_nEatSoundID;
	float fCollectedTime;
	friend class CAI_Handler;
protected:
	void SetNewTargetPosX(const int nTargetX);
	void SetNewTargetPosY(const int nTargetY);
	void SetNewTargetPos(const int nTargetX, const int nTargetY);
	void ClearNewTarget();
	bool HasNewTarget();
public:
	CBaseGolem(void);
	virtual ~CBaseGolem(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	//virtual bool CheckCollision(IUnitInterface* pBase);
	bool CheckCollision(IUnitInterface* pBase);
	virtual void UpdateAI();
	void ClearTarget();
	bool HasTarget();

	//accessors
	int GetTargetPosX() const;
	int GetTargetPosY() const;
	int GetGolemType() const;
	int GetEatSoundID() { return m_nEatSoundID ; }
	//mutators
	void SetGolemType(const int nGolemType);
	void SetTargetPosX(const int nTargetX);
	void SetTargetPosY(const int nTargetY);
	void SetTargetPos(const int nTargetX, const int nTargetY);
	void SetMoveType(const int nMovementType);

	void HandleEvent( Event* _toHandle ) ;
};
#endif
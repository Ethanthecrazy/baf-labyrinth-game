#ifndef CBASEGOLEM_H_
#define CBASEGOLEM_H_

#include "CBaseEntity.h"
#include "../../Messaging/IListener.h"

enum MOVETYPES {NO_MOVE, RAND_MOVE, TARGET_MOVE,
				NUM_MOVE_TYPES};

class CBaseGolem : public CBaseEntity , public IListener
{
	//target to walk to, -1 is no target
	int m_nTargetX, m_nTargetY;
	int m_nGolemType;
	int m_nMovementType;
	int m_nEatSoundID;
	float fCollectedTime;
public:
	CBaseGolem(void);
	virtual ~CBaseGolem(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase);
	virtual void UpdateAI();
	void ClearTarget();
	bool HasTarget();

	int GetGolemType() const;
	int GetTargetPosX() const;
	int GetTargetPosY() const;
	int GetEatSoundID() { return m_nEatSoundID ; }
	void SetGolemType(const int nGolemType);
	void SetTargetPosX(const int nTargetX);
	void SetTargetPosY(const int nTargetY);
	void SetTargetPos(const int nTargetX, const int nTargetY);
	void SetMoveType(const int nMovementType);

	void HandleEvent( Event* _toHandle ) ;
};
#endif
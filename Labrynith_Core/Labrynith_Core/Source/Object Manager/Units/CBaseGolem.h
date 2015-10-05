#ifndef CBASEGOLEM_H_
#define CBASEGOLEM_H_

#include "CBaseEntity.h"
class Event;

enum MOVETYPES {NO_MOVE, RAND_MOVE, TARGET_MOVE,
				NUM_MOVE_TYPES};

class CBaseGolem : public CBaseEntity
{
	int m_nGolemType;
	int m_nMovementType;
	int m_nEatSoundID;
	int m_nStepSoundID;
	float fCollectedTime;
	bool CheckEntCollision(CBaseEntity* pEntity);

	float LastDistance;

protected:
	
public:
	CBaseGolem(void);
	virtual ~CBaseGolem(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	virtual bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	virtual void ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	virtual bool CheckTileCollision(int TileID);
	virtual void UpdateAI();
	virtual bool CanInteract(IUnitInterface* pBase);

	//accessors
	int GetGolemType() const;
	int GetMoveType() const;
	int GetEatSoundID() { return m_nEatSoundID ; }
	int GetStepSoundID() { return m_nStepSoundID ; }
	//mutators
	void SetGolemType(const int nGolemType);
	virtual void SetMoveType(const int nMovementType);
	void SetStepSoundID( int ID ) { m_nStepSoundID = ID ; }

	virtual void HandleEvent( Event* _toHandle ) ;
};
#endif
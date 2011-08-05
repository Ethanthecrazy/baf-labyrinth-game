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
	float fCollectedTime;
	bool CheckEntCollision(CBaseEntity* pEntity);
protected:
	
public:
	CBaseGolem(void);
	virtual ~CBaseGolem(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	virtual bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	//bool CheckCollision(IUnitInterface* pBase);
	virtual bool CheckTileCollision(int TileID);
	virtual void UpdateAI();

	//accessors
	int GetGolemType() const;
	int GetMoveType() const;
	int GetEatSoundID() { return m_nEatSoundID ; }
	//mutators
	void SetGolemType(const int nGolemType);
	void SetMoveType(const int nMovementType);

	virtual void HandleEvent( Event* _toHandle ) ;
};
#endif
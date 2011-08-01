#ifndef CPLAYER
#define CPLAYER

#include "CBaseEntity.h"

class CPlayer : public CBaseEntity
{
	int m_nLives;
	//Item the player is holding
	//this must not be in the object manager when this happens
	IUnitInterface *m_pHeldItem;
public:
	CPlayer(void);
	virtual ~CPlayer(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	void Input();
	virtual bool CheckCollision(IUnitInterface* pBase);
	//accessors
	int GetLives() const;
	IUnitInterface* GetHeldItem() const;
	//mutators
	void SetLives(const int nLives);
	void SetHeldItem(const IUnitInterface* pHeldItem);
};
#endif
#ifndef CPLAYER
#define CPLAYER

#include "CBaseEntity.h"

class CBaseObject;

class CPlayer : public CBaseEntity
{
	int m_nLives;
	//Item the player is holding
	//this must not be in the object manager when this happens
	IUnitInterface *m_pHeldItem;
public:
	CPlayer(void);
	//BUG - this constructor will change as development continues
	CPlayer(float fPosX, float fPosY, int nLives, CBaseObject* pHeldItem);
	virtual ~CPlayer(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	void Input();
	virtual bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	virtual bool CheckTileCollision(int TileID);
	//accessors
	int GetLives() const;
	IUnitInterface* GetHeldItem() const;
	//mutators
	void SetLives(const int nLives);
	void SetHeldItem(IUnitInterface* const pHeldItem);
};
#endif
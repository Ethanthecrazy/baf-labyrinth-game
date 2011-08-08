#ifndef CPLAYER
#define CPLAYER

#include "CBaseEntity.h"

class CBaseObject;

class CPlayer : public CBaseEntity
{
	int m_nLives;
	int m_nPickUpSoundID;
	int m_nPutDownSoundID;
	//Item the player is holding
	//this must not be in the object manager when this happens
	CBaseObject *m_pHeldItem;
	CBaseObject *m_pEquippedItem;
public:
	CPlayer(void);
	//BUG - this constructor will change as development continues
	CPlayer(float fPosX, float fPosY, int nLives, CBaseObject* pHeldItem);
	virtual ~CPlayer(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	void Input();
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	void ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CheckTileCollision(int TileID);
	void SwitchItems(void); // switches held and equipped items if it is possible
	void ActivateEquipped(void);	//	activates the equipped item
	//accessors
	int GetLives() const;
	int GetPickUpSoundID() { return m_nPickUpSoundID ; } ;
	int GetPutDownSoundID() { return m_nPutDownSoundID ; } ;
	CBaseObject* GetHeldItem() const;
	CBaseObject* GetEquippedItem() { return m_pEquippedItem ; } ;
	
	//mutators
	void SetLives(const int nLives);
	void SetPickUpSoundID(int ID ) { m_nPickUpSoundID = ID ; } ;
	void SetPutDownSoundID(int ID) { m_nPutDownSoundID = ID ; } ;
	void SetHeldItem(CBaseObject* const pHeldItem);
	void SetEquippedItem( CBaseObject* const pItem ) { m_pEquippedItem = pItem ; } ;
};
#endif
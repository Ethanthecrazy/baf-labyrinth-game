#ifndef CPLAYER
#define CPLAYER

#include "CBaseEntity.h"

//how long the player is invincible
#define INVTIME 3.0f

class CBaseObject;

class CPlayer : public CBaseEntity
{
	int m_nLives;
	bool m_bIsInvincible;
	float m_fInvincilibilityTimer;
	int m_nPickUpSoundID;
	int m_nPutDownSoundID;
	int m_nLvCompSoundID;
	//Item the player is holding
	//this must not be in the object manager when this happens
	CBaseObject *m_pHeldItem;
	CBaseObject *m_pEquippedItem;

	void UpdateInvincibilityTime(float fDT);
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
	bool CanInteract(IUnitInterface* pBase);
	bool CheckTileCollision(int TileID);
	void SwitchItems(void); // switches held and equipped items if it is possible
	void ActivateEquipped(void);	//	activates the equipped item
	void LoadEntMoveAnimIDs();

	//accessors
	int GetLives() const;
	bool IsInvincible() const;
	int GetPickUpSoundID() { return m_nPickUpSoundID ; } ;
	int GetPutDownSoundID() { return m_nPutDownSoundID ; } ;
	int GetLvlCompSoundID() { return m_nLvCompSoundID ; } ;
	CBaseObject* GetHeldItem() const;
	CBaseObject* GetEquippedItem() { return m_pEquippedItem ; } ;
	
	//mutators
	void SetLives(const int nLives);
	void SetInvincilibity(const bool bIsInvincible);
	void SetPickUpSoundID(int ID ) { m_nPickUpSoundID = ID ; } ;
	void SetPutDownSoundID(int ID) { m_nPutDownSoundID = ID ; } ;
	void SetHeldItem(CBaseObject* const pHeldItem);
	void SetEquippedItem( CBaseObject* const pItem ) { m_pEquippedItem = pItem ; } ;
};
#endif
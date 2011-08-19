#ifndef CSAVESLOTSTATE_H
#define CSAVESLOTSTATE_H

#include "IGameState.h"
#include "../CBitFont.h"

#define MAXSLOTS 3

class CPlayer;

class CSaveSlotState : public IGameState
{
	CBitFont MetalText;
	//current player data loaded
	CPlayer* m_pLoadedPlayer;
	//current level loaded
	int m_nCurrLoadedLevel;
	int m_nLoadedLevels[MAXSLOTS];
	//which slot the player is saving/loading to
	int m_nCurrSaveSlot;
	int m_nIndex;

	int m_nSoundID;
	int m_nImageID;

	//Enums
	enum SLOTOPTIONS{SELECT, SLOTDELETE, BACK, NUMSLOTOPTIONS};

	CSaveSlotState();
	CSaveSlotState(const CSaveSlotState&) { }
	CSaveSlotState& operator=(const CSaveSlotState&) { }
	~CSaveSlotState();

	void SetSaveSlot(const int nSlot);
	//deletes game data
	void Delete();
	//Loads game data
	void Load(int nSaveSlot);
public:

	static CSaveSlotState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);
	void EnterCommand(void);
	//Saves game data
	void Save();

	//accessors
	int GetCurrLevel() const;
	//mutators
	void SetCurrLevel(const int nLevel);
};
#endif
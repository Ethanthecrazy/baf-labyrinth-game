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
	//which slot the player is saving/loading to
	int m_nCurrSaveSlot;
	int m_nIndex;

	//Enums
	enum SLOTOPTIONS{SELECT, SLOTDELETE, NUMSLOTOPTIONS};

	CSaveSlotState();
	CSaveSlotState(const CSaveSlotState&) { }
	CSaveSlotState& operator=(const CSaveSlotState&) { }
	~CSaveSlotState();

	void SetSaveSlot(const int nSlot);
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
	//Loads game data
	void Load();
	//deletes game data
	void Delete();

	//accessors
	int GetCurrLevel() const;
	//mutators
	void SetCurrLevel(const int nLevel);
};
#endif
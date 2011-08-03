#ifndef CSAVESLOTSTATE_H
#define CSAVESLOTSTATE_H

#define MAXSLOTS 3

#include "IGameState.h"

class CPlayer;

class CSaveSlotState : public IGameState
{
	//current data loaded
	//current player data loaded
	CPlayer* m_pLoadedPlayer;
	//current level loaded
	int m_nCurrLoadedLevel;
	//which slot the player is saving/loading to
	int m_nCurrSaveSlot;
	//Saving only occurs at the end of a level
	//bool isLoading;

	// default constructor
	CSaveSlotState();
	// copy constructor
	CSaveSlotState(const CSaveSlotState&) { }
	// assignment operator
	CSaveSlotState& operator=(const CSaveSlotState&) { }
	// destructor
	~CSaveSlotState();
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
};
#endif
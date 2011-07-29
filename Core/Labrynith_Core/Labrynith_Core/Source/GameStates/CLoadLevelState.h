#ifndef CLOADLEVELSTATE_H_
#define CLOADLEVELSTATE_H_

#include "IGameState.h"

class CLoadLevelState : public IGameState
{
	// default constructor
	CLoadLevelState();
	// copy constructor
	CLoadLevelState(const CLoadLevelState&) { }
	// assignment operator
	CLoadLevelState& operator=(const CLoadLevelState&) { }
	// destructor
	~CLoadLevelState();

public:

	static CLoadLevelState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);

	bool LoadLevel(int _level);

	void EnterCommand(void);
};


#endif
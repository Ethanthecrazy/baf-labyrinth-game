#ifndef CGamePlayState_h__
#define CGamePlayState_h__

#include "IGameState.h"

class CGamePlayState : public IGameState
{
	// default constructor
	CGamePlayState();
	// copy constructor
	CGamePlayState(const CGamePlayState&) { }
	// assignment operator
	CGamePlayState& operator=(const CGamePlayState&) { }
	// destructor
	~CGamePlayState();

public:

	static CGamePlayState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);
};

#endif // CGamePlayState_h__

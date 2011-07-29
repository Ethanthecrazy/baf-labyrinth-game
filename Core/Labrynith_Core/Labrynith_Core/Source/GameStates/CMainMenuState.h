
#ifndef CMainMenuState_h__
#define CMainMenuState_h__

#include "IGameState.h"

#include "../CBitFont.h"

enum MenuItems{ PLAY, OPTIONS, HOWTOPLAY, HIGHSCORES, CREDITS, EXIT, NUM_MENUITEMS };

class CMainMenuState : public IGameState
{
	CBitFont MetalText;

	// default constructor
	CMainMenuState();
	// copy constructor
	CMainMenuState(const CMainMenuState&) { }
	// assignment operator
	CMainMenuState& operator=(const CMainMenuState&) { }
	// destructor
	~CMainMenuState();

public:

	static CMainMenuState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);

	void EnterCommand(void);
};

#endif // CMainMenuState_h__
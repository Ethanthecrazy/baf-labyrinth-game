
#ifndef CMainMenuState_h__
#define CMainMenuState_h__

#include "IGameState.h"

#include "../CBitFont.h"

//HowToPlay State
//Controls will be displayed in-game + tutorial level
//HighScore State
//no highscores
enum MenuItems{ PLAY, OPTIONS, CREDITS, EXIT, NUM_MENUITEMS };

class CMainMenuState : public IGameState
{
	//Data Members
	CBitFont MetalText;
	int m_nIndex;
	//Asset ids:
	int		m_nImageID;
	int		m_nSoundID;
	
	// default constructor
	CMainMenuState();
	// copy constructor
	CMainMenuState(const CMainMenuState&) { }
	// assignment operator
	CMainMenuState& operator=(const CMainMenuState&) { }
	// destructor
	~CMainMenuState();

	bool SetMenuIndex(const int nIndex);
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
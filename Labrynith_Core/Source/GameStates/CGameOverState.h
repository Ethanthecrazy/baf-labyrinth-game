#ifndef CGAMEOVERSTATE_H_
#define CGAMEOVERSTATE_H_

#include "IGameState.h"

#include "../CBitFont.h"

//HowToPlay State
//Controls will be displayed in-game + tutorial level
//HighScore State
//no highscores
enum { GAMEOVER_RETRY, GAMEOVER_EXIT, GAMEOVER_NUM_MENUITEMS };

class CGameOverState : public IGameState
{
	//Data Members
	CBitFont MetalText;
	int m_nIndex;
	int m_nSoundID;
	
	// default constructor
	CGameOverState();
	// copy constructor
	CGameOverState(const CGameOverState&) { }
	// assignment operator
	CGameOverState& operator=(const CGameOverState&) { }
	// destructor
	~CGameOverState();

	bool SetMenuIndex(const int nIndex);
public:

	static CGameOverState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);

	void EnterCommand(void);
};

#endif // CGameOverState.h
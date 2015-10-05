#ifndef Pause_H_
#define Pause_H_

#include "IGameState.h"
#include "..\CBitFont.h"

class CPauseState : public IGameState
{
private:
	//Data Members
	CBitFont MetalText;
	int m_nIndex;
	//Asset ids:
	int		m_nImageID;
	int		m_nSoundID;
	//Enums
	enum PAUSEOPTIONS{RESUME, RESTART, EXIT, NUMOPTIONS};

	CPauseState(void);
	CPauseState (const CPauseState &);
	CPauseState & operator=(const CPauseState &);
	~CPauseState (void);
public:
	static CPauseState* GetInstance(void);

	virtual void Enter(void);
	virtual bool Input(void);
	virtual void Update(float fElapsedTime);
	virtual void Render(void);
	virtual void Exit(void);
	void EnterCommand(void);
};

#endif
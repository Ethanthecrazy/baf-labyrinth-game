#ifndef CHELPSTATE_H_
#define CHELPSTATE_H_

#include "IGameState.h"
#include "..\CBitFont.h"

#include <string>
using std::string;

class CHelpState : public IGameState
{
private:
	//Data Members
	CBitFont MetalText;

	int		m_nSoundID;
	static string helptext;

	CHelpState(void);
	CHelpState (const CHelpState &);
	CHelpState & operator=(const CHelpState &);
	~CHelpState (void);
public:
	static CHelpState* GetInstance(string text);

	virtual void Enter(void);
	virtual bool Input(void);
	virtual void Update(float fElapsedTime);
	virtual void Render(void);
	virtual void Exit(void);
	void EnterCommand(void);
};

#endif
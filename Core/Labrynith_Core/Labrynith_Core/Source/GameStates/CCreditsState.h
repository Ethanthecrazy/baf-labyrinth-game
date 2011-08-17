#ifndef CCREDITSSTATE_H_
#define CCREDITSSTATE_H_

#include "IGameState.h"
#include "../CBitFont.h"

#include <string>
#include <vector>


struct CreditLine
{
	std::string line;

	int posX;
	int posY;

	float size;
};

class CCreditsState : public IGameState
{
	// default constructor
	CCreditsState();
	// copy constructor
	CCreditsState(const CCreditsState&) { }
	// assignment operator
	CCreditsState& operator=(const CCreditsState&) { }
	// destructor
	~CCreditsState();
	
	CBitFont MetalText;
	int m_fYposition;
	int scrollSpeed;

	std::vector<CreditLine> Credits;

public:

	static CCreditsState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);

	void EnterCommand(void);
};


#endif  // CCreditsState
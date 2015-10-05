#ifndef CDATAMANAGERSTATE_H
#define CDATAMANAGERSTATE_H

#include "IGameState.h"

class CDataManagerState : public IGameState
{
	// default constructor
	CDataManagerState();
	// copy constructor
	CDataManagerState(const CDataManagerState&) { }
	// assignment operator
	CDataManagerState& operator=(const CDataManagerState&) { }
	// destructor
	~CDataManagerState();

public:

	static CDataManagerState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);

	void EnterCommand(void);
};


#endif

#ifndef IGameState_h__
#define IGameState_h__

class IGameState
{

public:
	virtual void Enter(void) = 0; // Enters the Game State
	virtual bool Input(void) = 0; // Input
	virtual void Update(float fDT) = 0; // Update
	virtual void Render(void) = 0; // Draw
	virtual void Exit(void) = 0; // Leaves the Game State
	virtual ~IGameState() = 0 {};	// destructor
	virtual void EnterCommand() = 0;

};

#endif // IGameState_h__
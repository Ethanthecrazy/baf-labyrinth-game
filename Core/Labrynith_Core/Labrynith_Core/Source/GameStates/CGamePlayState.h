#ifndef CGamePlayState_h__
#define CGamePlayState_h__

#include "IGameState.h"
#include "../Object Manager/MObjectManager.h"
#include "../Messaging/MMessageSystem.h"

class CGamePlayState : public IGameState
{
	int m_nMouseID;

	// default constructor
	CGamePlayState();
	// copy constructor
	CGamePlayState(const CGamePlayState&) { }
	// assignment operator
	CGamePlayState& operator=(const CGamePlayState&) { }
	// destructor
	~CGamePlayState();

	float timestep;
	int m_nCurrLevel;
	int currFloor;
public:

	static int testVaribale;

	static CGamePlayState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);

	void GetCamera( int& X , int& Y ) ;

	void EnterCommand(void);

	int GetCurrentLevel(void) {return m_nCurrLevel;}
	void SetCurrentLevel(const int _level) {m_nCurrLevel = _level;}

	static void MessageProc(CBaseMessage* _message);
};

#endif // CGamePlayState_h__

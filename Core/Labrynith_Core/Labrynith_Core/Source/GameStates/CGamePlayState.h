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
	int numLevelFloors;
	bool m_bRenderCulling;
	bool m_bIsPaused;
public:

	static int testVaribale;
	static CGamePlayState* GetInstance();

	void Enter(void);
	bool Input(void);
	void Update(float fDT);
	void Render(void);
	void Exit(void);
	void EnterCommand(void);

	static void MessageProc(CBaseMessage* _message);
	static void KillPlayer(void);

	//accessors
	void GetCamera( int& X , int& Y );
	int GetNumLevelFloors(void) { return numLevelFloors; }
	bool GetRenderCulling(void) { return m_bRenderCulling; }
	bool IsPaused() const;
	int GetCurrentLevel(void) {return m_nCurrLevel;}
	//mutators
	void SetCurrentLevel(const int _level) {m_nCurrLevel = _level;}
	void SetNumLevelFloors(int nNumFloors) { numLevelFloors = nNumFloors; }
	void SetPaused(const bool bIsPaused);
	
};

#endif // CGamePlayState_h__

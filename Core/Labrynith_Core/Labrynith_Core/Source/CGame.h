#ifndef CGame_h__
#define CGame_h__

#include <Windows.h>

#include <vector>
#include <string>
using std::string;
using namespace std;

#include "Wrappers\CSGD_Direct3D.h"
#include "Wrappers\CSGD_DirectInput.h"
#include "Wrappers\CSGD_TextureManager.h"
#include "UTimer.h"

class IGameState;

class CGame
{
	vector<IGameState*> m_pGameStates;
	bool bFullscreen;

	float m_fElapsedTime;
	float m_fGameTime;

	int m_nScreenWidth;
	int m_nScreenHeight;

	UTimer TheTimer;

	// default constructor
	CGame();
	// copy constructor
	CGame(const CGame&);
	// assignment operator
	CGame& operator=(const CGame&);
	// destructor
	~CGame();

	// utility functions
	bool Input();
	void Update();
	void Render();
	void EnterCommand();

public:

	static CGame* GetInstance();
	void ChangeState(IGameState* pNextState);

	void Initialize(HWND hWnd, HINSTANCE hInstance, 
		int nScreenWidth, int nScreenHeight, bool bIsWindowed);

	bool Main();
	void Shutdown();

	float GetElapsedTime() {return m_fElapsedTime; }
	void SetElapsedTime(float t) { m_fElapsedTime = t; }
	
	int GetScreenWidth() {return m_nScreenWidth;  }
	int GetScreenHeight() {return m_nScreenHeight; }
	
	void ClearAllStates( void );
	void PopState(void);
	void PushState(IGameState* pNextState);

};


#endif // CGame_h__
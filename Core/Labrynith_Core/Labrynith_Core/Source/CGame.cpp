
#include "CGame.h"
#include <string>
#include <iostream>
#include <time.h>
using std::string;

#include "GameStates\CMainMenuState.h"
#include "Animation Manager\CAnimationManager.h"
#include "Wrappers\CSGD_FModManager.h"

// default constructor
CGame::CGame()
{
	bFullscreen = false;
}

// destructor
CGame::~CGame() { }

// singleton accessor
CGame* CGame::GetInstance()
{
	static CGame instance;
	return &instance;
}

// initialization
void CGame::Initialize(HWND hWnd, HINSTANCE hInstance, 
	int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{

	srand( unsigned int( time( 0 ) ) );
	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;

	// initialize singletons
	CSGD_Direct3D::GetInstance()->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, 
		bIsWindowed, true);

	CSGD_TextureManager::GetInstance()->InitTextureManager(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(),
		CSGD_Direct3D::GetInstance()->GetSprite());

	CAnimationManager::GetInstance()->LoadAnimation("resource/Animation Files/entity-movement.xml", true);
	CAnimationManager::GetInstance()->LoadAnimation("resource/Animation Files/fire.xml", true );	
	CAnimationManager::GetInstance()->LoadAnimation("resource/Animation Files/entity-Eating.xml", true );
	CAnimationManager::GetInstance()->LoadAnimation("resource/Animation Files/Electricity.xml" , true ) ;

	CSGD_DirectInput::GetInstance()->InitDirectInput(hWnd, hInstance, DI_KEYBOARD | DI_MOUSE, DI_MOUSE);

	CSGD_FModManager::GetInstance()->InitFModManager( hWnd ) ;

	
	ChangeState( CMainMenuState::GetInstance() );

	TheTimer.Reset();
	TheTimer.Start();
}

// execution
bool CGame::Main()
{
	m_fElapsedTime = (float)TheTimer.GetElapsedTime();
	
	TheTimer.Reset();

	if (Input() == false)
		return false;

	Update();
	Render();

	return true;
}

bool CGame::Input()
{
	CSGD_DirectInput::GetInstance()->ReadDevices();

	if( CSGD_DirectInput::GetInstance()->KeyPressed( DIK_GRAVE ) )
	{
		m_pGameStates[m_pGameStates.size() - 1]->EnterCommand();
	}

	static bool Is_Fullscreen = false;

	if( m_pGameStates.size() )
		return m_pGameStates[m_pGameStates.size() - 1]->Input();
	else
		return true;
}

void CGame::Update()
{
	// update
	if( m_pGameStates.size() )
		m_pGameStates[m_pGameStates.size() - 1]->Update(m_fElapsedTime);
}

void CGame::Render()
{
	CSGD_Direct3D::GetInstance()->Clear(0, 0, 0);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();

	for( unsigned int i = 0; i < m_pGameStates.size(); ++i )
	{
		m_pGameStates[i]->Render();
	}

	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->Present();

	CSGD_FModManager::GetInstance()->Update() ;
}

void CGame::ClearAllStates( void )
{
	for( unsigned int i = 0; i < m_pGameStates.size(); ++i )
	{
		m_pGameStates[i]->Exit();
	}

	m_pGameStates.clear();

}

void CGame::ChangeState(IGameState* pNextState)
{
	ClearAllStates();

	m_pGameStates.push_back( pNextState );

	if (m_pGameStates[0])
		m_pGameStates[0]->Enter();
}

// cleanup
void CGame::Shutdown()
{
	ChangeState(NULL);

	CSGD_DirectInput::GetInstance()->ShutdownDirectInput();

	CSGD_TextureManager::GetInstance()->ShutdownTextureManager();

	CSGD_Direct3D::GetInstance()->ShutdownDirect3D();

	CAnimationManager::GetInstance()->DeleteInstance();

	CSGD_FModManager::GetInstance()->ShutdownFModManager() ;
}

void CGame::PopState(void)
{
	m_pGameStates[m_pGameStates.size() - 1]->Exit();
	m_pGameStates.pop_back();

}

void CGame::PushState(IGameState* pNextState)
{
	m_pGameStates.push_back( pNextState );
	m_pGameStates[m_pGameStates.size() - 1]->Enter();

}

bool CGame::IsFullScreen() const
{
	return bFullscreen;
}
void CGame::SetFullScreen(const bool bFullScreen)
{
	//if they are the same leave, no
	//use in doing work
	if(bFullscreen == bFullScreen)
		return;

	CSGD_Direct3D::GetInstance()->ChangeDisplayParam(CSGD_Direct3D::GetInstance()->GetPresentParams()->BackBufferWidth,
			CSGD_Direct3D::GetInstance()->GetPresentParams()->BackBufferHeight, (bFullScreen) ? false : true);
	bFullscreen = bFullScreen;
}

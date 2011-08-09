
#include "CGameOverState.h"
#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CLoadLevelState.h"

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../CGame.h"

#include <iostream>


// default constructor
CGameOverState::CGameOverState() 
{
	m_nIndex = 0;
}

// destructor
CGameOverState::~CGameOverState() 
{

}

CGameOverState* CGameOverState::GetInstance()
{
	static CGameOverState instance;
	return &instance;
}

void CGameOverState::Enter(void)
{
	cout << "GameOver\n";

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ',
		64,
		64,
		10 );
}

bool CGameOverState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	
	
	//Enter
	if(pDI->KeyPressed(DIK_RETURN))
	{
		switch(m_nIndex)
		{
		case GAMEOVER_RETRY:
			CGame::GetInstance()->PushState( CLoadLevelState::GetInstance() );
			pGame->PopState() ;
			break;

		case GAMEOVER_EXIT:
			CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
			break;
		};
	}

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP))
	{
		SetMenuIndex(--m_nIndex);
		//Play a sound
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN))
	{
		SetMenuIndex(++m_nIndex);
		//Play a sound
	}

	return true;
}

void CGameOverState::Update(float fDT)
{

}

void CGameOverState::Render(void)
{
	
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	MetalText.Print( "GameOver", 100, 100, 0.6f );
	MetalText.Print( "->", 0, 230 + (m_nIndex * 30), 0.5f );
	MetalText.Print( "Retry", 110, 230, 0.5f );
	MetalText.Print( "Quit", 110, 260, 0.5f );
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
}

void CGameOverState::Exit(void)
{
	cout << "left GameOver state -> ";
}

void CGameOverState::EnterCommand(void)
{
	while( true )
	{
		cout << "Command>";
		string command;
		cin >> command;

		if( command == "help" )
		{
			cout << "\n==============================\n";
			cout << "Command\t\tDescription\n\n";

			cout << "help\t\tthis list\n";
			cout << "return\t\tcontinues the game\n";
			
			cout << endl;

		}
		else if( command == "return" )
		{	
			break;
		}
		else
		{
			cout << "Unknown command '" << command <<"' - type 'help' for command list.\n";
		}
	}
}

bool CGameOverState::SetMenuIndex(const int nIndex)
{
	//if index is too low
	if(nIndex < 0 )
	{
		m_nIndex = (GAMEOVER_NUM_MENUITEMS - 1);
		return false;
	}

	//if index is too high
	if(nIndex >= GAMEOVER_NUM_MENUITEMS)
	{
		m_nIndex = 0;
		return false;
	}

	m_nIndex = nIndex;
	return true;
}

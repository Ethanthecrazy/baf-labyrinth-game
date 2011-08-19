
#include "CGameOverState.h"
#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CLoadLevelState.h"
#include "COptionsState.h"

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../Wrappers/CSGD_FModManager.h"
#include "../CGame.h"

#include <iostream>


// default constructor
CGameOverState::CGameOverState() 
{
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();

	m_nIndex = 0;
	m_nSoundID = FM->LoadSound("resource/Sounds/Clic1.wav");
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
	COptionsState* Opt = COptionsState::GetInstance();

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
	Opt->AdjustSound(m_nSoundID, true);
}

bool CGameOverState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	
	//Enter
	if(pDI->KeyPressed(DIK_RETURN) ||
		pDI->JoystickButtonPressed(0))
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
	if(pDI->KeyPressed(DIK_UP) ||
		pDI->JoystickGetLStickDirPressed(DIR_UP, 0))
	{
		SetMenuIndex(--m_nIndex);
		FM->PlaySoundA(m_nSoundID);
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN) ||
		pDI->JoystickGetLStickDirPressed(DIR_DOWN, 0))
	{
		SetMenuIndex(++m_nIndex);
		FM->PlaySoundA(m_nSoundID);
	}

	return true;
}

void CGameOverState::Update(float fDT)
{

}

void CGameOverState::Render(void)
{
	MetalText.Print( "GameOver", 300, 200, 0.6f );
	MetalText.Print( "->", 200, 330 + (m_nIndex * 30), 0.5f );
	MetalText.Print( "Retry", 310, 330, 0.5f );
	MetalText.Print( "Quit", 310, 360, 0.5f );
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
}

void CGameOverState::Exit(void)
{
	cout << "GameOver -> ";
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

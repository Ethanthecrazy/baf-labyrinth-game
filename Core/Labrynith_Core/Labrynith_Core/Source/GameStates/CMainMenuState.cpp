
#include "CMainMenuState.h"
#include "CGamePlayState.h"
#include "CSaveSlotState.h"
#include "COptionsState.h"
#include "CCreditsState.h"

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../Wrappers/CSGD_FModManager.h"
#include "COptionsState.h"
#include "../CGame.h"

#include <iostream>


// default constructor
CMainMenuState::CMainMenuState() 
{
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	COptionsState* Opt = COptionsState::GetInstance();

	m_nIndex = 0;
	m_nImageID = -1;
	m_nSoundID = FM->LoadSound("resource/Sounds/Clic1.wav");	
	Opt->AdjustSound(m_nSoundID, true);
}

// destructor
CMainMenuState::~CMainMenuState() 
{

}

CMainMenuState* CMainMenuState::GetInstance()
{
	static CMainMenuState instance;
	return &instance;
}

void CMainMenuState::Enter(void)
{
	cout << "MainMenu\n";

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
}

bool CMainMenuState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();

	//Enter
	if(pDI->KeyPressed(DIK_RETURN))
	{
		switch(m_nIndex)
		{
		case PLAY:
			//Change to GamePlayState
			pGame->ChangeState( CSaveSlotState::GetInstance() ) ;
			break;

		case LOAD:
			//Choose a different slot
			pGame->ChangeState( CSaveSlotState::GetInstance() );
			break;

		case OPTIONS:
			//Change to Options State
			pGame->ChangeState( COptionsState::GetInstance() );
			break;
			
		case CREDITS:
			//Change to credits
			pGame->ChangeState( CCreditsState::GetInstance() );
			break;

		case EXIT:
			pGame->ClearAllStates();
			return false;
			break;
		};
	}

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP))
	{
		SetMenuIndex(--m_nIndex);
		FM->PlaySoundA(m_nSoundID);
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN))
	{
		SetMenuIndex(++m_nIndex);
		FM->PlaySoundA(m_nSoundID);
	}

	return true;
}

void CMainMenuState::Update(float fDT)
{

}

void CMainMenuState::Render(void)
{
	MetalText.Print( "Main Menu", 100, 100, 0.6f );
	MetalText.Print( "->", 0, 230 + (m_nIndex * 30), 0.5f );
	MetalText.Print( "Play", 110, 230, 0.5f );
	MetalText.Print( "Load", 110, 260, 0.5f );
	MetalText.Print( "Options", 110, 290, 0.5f );
	MetalText.Print( "Credits", 110, 320, 0.5f );
	MetalText.Print( "Exit", 110, 350, 0.5f );
}

void CMainMenuState::Exit(void)
{
	cout << "MainMenu -> ";
}

void CMainMenuState::EnterCommand(void)
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

bool CMainMenuState::SetMenuIndex(const int nIndex)
{
	//if index is too low
	if(nIndex < 0 )
	{
		m_nIndex = (NUM_MENUITEMS - 1);
		return false;
	}

	//if index is too high
	if(nIndex >= NUM_MENUITEMS)
	{
		m_nIndex = 0;
		return false;
	}

	m_nIndex = nIndex;
	return true;
}

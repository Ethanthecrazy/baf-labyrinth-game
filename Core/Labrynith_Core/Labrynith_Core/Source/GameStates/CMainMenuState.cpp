
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

	m_nIndex = 0;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/darkCave.png");
	m_nSoundID = FM->LoadSound("resource/Sounds/Clic1.wav");	
	SongID = -1;
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
	COptionsState* Opt = COptionsState::GetInstance();

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
	
	if(SongID == -1)
		SongID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/Music/menu.mp3", FMOD_LOOP_NORMAL);

	Opt->AdjustSound(SongID, false);

	if( !CSGD_FModManager::GetInstance()->IsSoundPlaying(SongID) )
		CSGD_FModManager::GetInstance()->PlaySoundA( SongID );

	Opt->AdjustSound(m_nSoundID, true);
}

bool CMainMenuState::Input(void)
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
		case PLAY:
			//Change to GamePlayState
			pGame->ChangeState( CSaveSlotState::GetInstance() ) ;
			break;

		case OPTIONS:
			//Change to Options State
			pGame->ChangeState( COptionsState::GetInstance() );
			break;
			
		case CREDITS:
			//Change to credits
			CSGD_FModManager::GetInstance()->StopSound(SongID);
			SongID = -1;
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

	//Arcade controls
	if( pDI->JoystickGetLStickDirPressed(DIR_UP, 0) )
	{
		SetMenuIndex(--m_nIndex);
		FM->PlaySoundA(m_nSoundID);
	}
	else if( pDI->JoystickGetLStickDirPressed(DIR_DOWN, 0) )
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
	CSGD_TextureManager::GetInstance()->Draw(m_nImageID, 0 , 0);
	MetalText.Print( "Main Menu", 450, 200, 0.6f );
	MetalText.Print( "->", 430, 330 + (m_nIndex * 30), 0.5f );
	MetalText.Print( "Play", 460, 330, 0.5f );
	MetalText.Print( "Options", 460, 360, 0.5f );
	MetalText.Print( "Credits", 460, 390, 0.5f );
	MetalText.Print( "Exit", 460, 420, 0.5f );
	MetalText.Print( "Labyrinthium", 150, 50, 0.7f );
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


#include "CMainMenuState.h"
#include "CGamePlayState.h"

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../CGame.h"

#include <iostream>


// default constructor
CMainMenuState::CMainMenuState() 
{
}

// destructor
CMainMenuState::~CMainMenuState() { }

CMainMenuState* CMainMenuState::GetInstance()
{
	static CMainMenuState instance;
	return &instance;
}

void CMainMenuState::Enter(void)
{
	cout << "MainMenu\n";

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ',
		64,
		64,
		10 );
}

bool CMainMenuState::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	
	
	if( pDI->KeyPressed( DIK_ESCAPE ) )
		return false;

	if( pDI->KeyPressed( DIK_RETURN ) )
		CGame::GetInstance()->ChangeState( CGamePlayState::GetInstance() );

	return true;
}

void CMainMenuState::Update(float fDT)
{

}

void CMainMenuState::Render(void)
{
	
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	MetalText.Print( "Main Menu", 100, 100, 0.6f );

	MetalText.Print( "New", 110, 230, 0.5f );
	MetalText.Print( "Load", 110, 260, 0.5f );
	MetalText.Print( "Options", 110, 290, 0.5f );
	MetalText.Print( "Exit", 110, 320, 0.5f );

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
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
#include "CCreditsState.h"
#include "CMainMenuState.h"

#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../CGame.h"
#include "../Wrappers/CSGD_FModManager.h"
#include "COptionsState.h"

#include <iostream>
#include <fstream>

// default constructor
CCreditsState::CCreditsState() 
{
	scrollSpeed = 120;
	soundid = -1;
}

// destructor
CCreditsState::~CCreditsState() 
{
	Credits.clear();
}

CCreditsState* CCreditsState::GetInstance()
{
	static CCreditsState instance;
	return &instance;
}

void CCreditsState::Enter(void)
{
	Credits.clear();
	soundid = -1;
	cout << "Credits\n";
	m_fYposition = -512;

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );

	soundid = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/Music/credits.mp3", FMOD_LOOP_NORMAL);
	COptionsState::GetInstance()->AdjustSound(soundid, false);
	CSGD_FModManager::GetInstance()->PlaySoundA( soundid );

	ifstream fin;
	fin.open("resource/credits.txt");

	if (fin.is_open())
	{
		while ( fin.good() )
		{
			CreditLine credit;
			getline(fin, credit.line);
			if(credit.line == "")
				getline(fin, credit.line);
			fin >> credit.posX >> credit.posY >> credit.size;
			
			Credits.push_back(credit);
		}

		fin.close();
	}
	else 
		cout << "Unable to open file"; 
}

bool CCreditsState::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	
	
	//Enter
	if(pDI->KeyPressed(DIK_ESCAPE) || pDI->KeyPressed(DIK_RETURN))
	{
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
	}

	if(pDI->KeyDown(DIK_SPACE))
	{
		scrollSpeed = 350;
	}
	else
		scrollSpeed = 120;

	return true;
}

void CCreditsState::Update(float fDT)
{
	m_fYposition += fDT * scrollSpeed;
}

void CCreditsState::Render(void)
{	
	float lastthing = Credits[Credits.size()-1].posY - m_fYposition;
	
	for(unsigned int i = 0; i < Credits.size(); ++i)
	{
		MetalText.Print((char*)Credits[i].line.c_str(), Credits[i].posX, Credits[i].posY - m_fYposition, Credits[i].size);
	}

	/*MetalText.Print("Credits", 250, 500 - m_fYposition, headersSize);	
	
	MetalText.Print("Executive Producer", 50, 800 - m_fYposition, headersSize);
	MetalText.Print("-John O'Leske", 300, 900 - m_fYposition, normalSize);
	
	MetalText.Print("Associate Producers", 50, 1300 - m_fYposition, headersSize);
	MetalText.Print("-Robert Martinez", 300, 1400 - m_fYposition, normalSize);
	MetalText.Print("-Sean Hathaway", 300, 1450 - m_fYposition, normalSize);
	MetalText.Print("-Shawn Paris", 300, 1500 - m_fYposition, normalSize);
	
	MetalText.Print("Programmers", 50, 1800 - m_fYposition, headersSize);
	MetalText.Print("-Ethan Pendergraft", 300, 1900 - m_fYposition, normalSize);
	MetalText.Print("-Jon \"Alfonso\" Taylor", 300, 1950 - m_fYposition, normalSize);
	MetalText.Print("-Rob Taylor", 300, 2000 - m_fYposition, normalSize);
	MetalText.Print("-Nathan Ream", 300, 2050 - m_fYposition, normalSize);
	
	MetalText.Print("Artists", 50, 2300 - m_fYposition, headersSize);
	MetalText.Print("-Chris Maher", 300, 2400 - m_fYposition, normalSize);
	MetalText.Print("-Ethan Pendergraft", 300, 2450 - m_fYposition, normalSize);
	MetalText.Print("-Google", 300, 2500 - m_fYposition, normalSize);
	
	MetalText.Print("A Special Thanks to", 10, 2800 - m_fYposition, headersSize);
	MetalText.Print("-Jordan Russell", 300, 2900 - m_fYposition, normalSize);
	MetalText.Print("-For the Installer", 300, lastthing, normalSize);*/

	if( lastthing < -50 )
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
}

void CCreditsState::Exit(void)
{
	cout << "Credits -> ";
	CSGD_FModManager::GetInstance()->StopSound(soundid);
}

void CCreditsState::EnterCommand(void)
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

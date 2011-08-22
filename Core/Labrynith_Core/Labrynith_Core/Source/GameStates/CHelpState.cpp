#include "CHelpState.h"
#include "COptionsState.h"
#include "..\CGame.h"
#include "..\Wrappers\CSGD_TextureManager.h"
#include "..\Wrappers\CSGD_DirectInput.h"
#include "..\Wrappers\CSGD_FModManager.h"
#include "CGamePlayState.h"
#include "../Messaging/MMessageSystem.h"
#include <iostream>
string CHelpState::helptext = "";

CHelpState::CHelpState(void)
{
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();

	m_nSoundID = FM->LoadSound("resource/Sounds/Clic1.wav");	
	
}

CHelpState::~CHelpState(void)
{

}

void CHelpState::Enter(void)
{
	cout << "HelpState\n";
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	COptionsState* Opt = COptionsState::GetInstance();

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
	Opt->AdjustSound(m_nSoundID, true);

	CSGD_FModManager::GetInstance()->PlaySoundA(m_nSoundID);
}
//Input
bool CHelpState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	
	//Enter
	if(pDI->GetBufferedDIKCodeEx() ||
		pDI->JoystickCheckBufferedButtonsEx() != -1)
	{
		pGame->PopState();
	}

	return true;
}
void CHelpState::Update(float fElapsedTime)
{

}
void CHelpState::Render(void)
{
	MetalText.Print(helptext, 300, 150, 0.5f);

	MetalText.Print("Press any key to continue", 300, 635, 0.5f);
}
CHelpState* CHelpState::GetInstance(string text)
{
	helptext = "";
	for(unsigned int i = 0; i < text.size(); ++i)
	{
		if(text[i] == '\\')
		{
			if(i != text.size() - 1)
			{
				if(text[i+1] == 'n')
				{					
					++i;
					helptext += '\n';
					continue;
				}
			}
		}

		helptext += text[i];
	}
	static CHelpState state;
	return &state;
}
void CHelpState::Exit(void)
{

	if( CGamePlayState::GetInstance()->m_nCurrLevel == 4 )
		MMessageSystem::GetInstance()->SendMsg( new msgEndGame() );
	
	cout << "HelpState -> ";
}
void CHelpState::EnterCommand(void)
{

}
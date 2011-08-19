#include "CPauseState.h"
#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CLoadLevelState.h"
#include "COptionsState.h"
#include "..\CGame.h"
#include "..\Wrappers\CSGD_Direct3D.h"
#include "..\Wrappers\CSGD_TextureManager.h"
#include "..\Wrappers\CSGD_DirectInput.h"
#include "..\Wrappers\CSGD_FModManager.h"

CPauseState::CPauseState(void)
{
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();

	m_nIndex = 0;
	m_nImageID = -1;
	m_nSoundID = FM->LoadSound("resource/Sounds/Clic1.wav");	
	
}

CPauseState::~CPauseState(void)
{

}

void CPauseState::Enter(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	COptionsState* Opt = COptionsState::GetInstance();

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
	Opt->AdjustSound(m_nSoundID, true);
}
//Input
bool CPauseState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP) ||
		pDI->JoystickGetLStickDirPressed(DIR_UP, 0))
	{
		FM->PlaySoundA(m_nSoundID);
		if(m_nIndex != 0)
			m_nIndex -= 1;
		else
			m_nIndex = NUMOPTIONS - 1;
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN) ||
		pDI->JoystickGetLStickDirPressed(DIR_DOWN, 0))
	{
		FM->PlaySoundA(m_nSoundID);
		if(m_nIndex != (NUMOPTIONS - 1))
			m_nIndex += 1;
		else
			m_nIndex = 0;
	}

	//Enter
	if(pDI->KeyPressed(DIK_RETURN) ||
		pDI->JoystickButtonPressed(0))
	{
		CGamePlayState* pGameState = CGamePlayState::GetInstance();
		switch(m_nIndex)
		{
		case RESUME:
			{
				pGameState->SetPaused(false);
				pGame->PopState();
				break;
			}
		case RESTART:
			{
				pGame->PopState();
				pGame->PushState(CLoadLevelState::GetInstance());
				break;
			}
		case EXIT:
			{
				//un-pause the game - for when user returns 
				pGameState->SetPaused(false);
				pGame->ChangeState(CMainMenuState::GetInstance());
				break;
			}
		};
	}
	return true;
}
void CPauseState::Update(float fElapsedTime)
{

}
void CPauseState::Render(void)
{
	//Get access to singletons
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	MetalText.Print("Paused", 320, 150);
	MetalText.Print("Resume", 400, 365, 0.6f);
	MetalText.Print("Restart Level", 400, 400, 0.6f);
	MetalText.Print("Exit", 400, 435, 0.6f);
	MetalText.Print("->", 300, 365 + (m_nIndex * 35), 0.6f);
}
CPauseState* CPauseState::GetInstance(void)
{
	static CPauseState state;
	return &state;
}
void CPauseState::Exit(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	//pTM->UnloadTexture(m_nImageID);
}
void CPauseState::EnterCommand(void)
{

}
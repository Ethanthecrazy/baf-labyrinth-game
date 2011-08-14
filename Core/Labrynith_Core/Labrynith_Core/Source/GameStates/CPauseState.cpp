#include "CPauseState.h"
#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CLoadLevelState.h"
#include "..\CGame.h"
#include "..\Wrappers\CSGD_Direct3D.h"
#include "..\Wrappers\CSGD_TextureManager.h"
#include "..\Wrappers\CSGD_DirectInput.h"

CPauseState::CPauseState(void)
{
	m_nIndex = 0;
	m_nImageID = -1;
}

CPauseState::~CPauseState(void)
{

}

void CPauseState::Enter(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.txt" );
}
//Input
bool CPauseState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP))
	{
		if(m_nIndex != 0)
			m_nIndex -= 1;
		else
			m_nIndex = NUMOPTIONS - 1;
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN))
	{
		if(m_nIndex != (NUMOPTIONS - 1))
			m_nIndex += 1;
		else
			m_nIndex = 0;
	}

	//Enter
	if(pDI->KeyPressed(DIK_RETURN))
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

	MetalText.Print("Paused", 220, 50, 1.1f);
	MetalText.Print("Resume", 300, 265);
	MetalText.Print("Restart Level", 300, 300);
	MetalText.Print("Exit", 300, 335);
	MetalText.Print("->", 200, 265 + (m_nIndex * 35));
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
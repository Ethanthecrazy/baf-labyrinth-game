#include "CGamePlayState.h"
#include "CMainMenuState.h"

#include <string>

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"

#include "../CGame.h"

using namespace std;

CGamePlayState::CGamePlayState()
{
	
}

// destructor
CGamePlayState::~CGamePlayState() { }

CGamePlayState* CGamePlayState::GetInstance()
{
	static CGamePlayState instance;
	return &instance;
}

void CGamePlayState::Enter(void)
{

}

bool CGamePlayState::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	
	
	if( pDI->KeyPressed( DIK_ESCAPE ) )
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );

	return true;
}

void CGamePlayState::Update(float fDT)
{

	

}

void CGamePlayState::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	pD3D->DrawTextA( "Gameplay State", 100, 100 );
}

void CGamePlayState::Exit(void)
{
	
}
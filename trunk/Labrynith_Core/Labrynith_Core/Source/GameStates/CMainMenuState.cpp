
#include "CMainMenuState.h"
#include "CGamePlayState.h"

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../CGame.h"


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

	pD3D->DrawTextA( "Main Menu State", 100, 100 );

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
}

void CMainMenuState::Exit(void)
{
}
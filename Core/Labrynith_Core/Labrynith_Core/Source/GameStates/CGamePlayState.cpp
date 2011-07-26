#include "CGamePlayState.h"
#include "CMainMenuState.h"

#include <string>

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"

#include "../CGame.h"
#include "../Object Manager/Units/CBaseEntity.h"
#include "../Object Manager/Units/CBaseObject.h"

#include <iostream>

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
	cout << "GamePlay\n";

	MObjectManager::GetInstance()->ResizeLayer( 1, 10, 10 );

	IUnitInterface* temp = new CBaseEntity();
	((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pokeball.png" );
	
	((CBaseEntity*)(temp))->SetIndexPosX( 0 );
	((CBaseEntity*)(temp))->SetIndexPosY( 0 );

	testVaribale = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
}

bool CGamePlayState::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	

	if( pDI->KeyPressed( DIK_ESCAPE ) )
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );

	CBaseEntity* player = (CBaseEntity*)MObjectManager::GetInstance()->GetUnit( testVaribale );

	if( player )
	{
		if( player->GetFlag_MovementState() == FLAG_MOVESTATE_ATDESTINATION )
		{

			if( pDI->KeyDown( DIK_UP ) )
			{
				MObjectManager::GetInstance()->MoveEntUp( player->m_nIdentificationNumber );
				return true;
			}

			if( pDI->KeyDown( DIK_DOWN ) )
			{
				MObjectManager::GetInstance()->MoveEntDown( player->m_nIdentificationNumber );
				return true;
			}

			if( pDI->KeyDown( DIK_LEFT ) )
			{
				MObjectManager::GetInstance()->MoveEntLeft( player->m_nIdentificationNumber );
				return true;
			}

			if( pDI->KeyDown( DIK_RIGHT ) )
			{
				MObjectManager::GetInstance()->MoveEntRight( player->m_nIdentificationNumber );
				return true;
			}

		}

		return true;

	}

	return true;
}

void CGamePlayState::Update(float fDT)
{
	MObjectManager::GetInstance()->Update( fDT );
}

void CGamePlayState::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	CBaseEntity* player = (CBaseEntity*)MObjectManager::GetInstance()->GetUnit( testVaribale );

	if( player )
	{
		MObjectManager::GetInstance()->Render( (int)player->GetPosX() - 800/2 + 16, (int)player->GetPosY() - 600/2 + 16 );
	}
	else
	{
		MObjectManager::GetInstance()->Render( 0, 0 );
	}

	pD3D->DrawTextA( "Gameplay State", 100, 100 );
}

void CGamePlayState::Exit(void)
{
	MObjectManager::GetInstance()->RemoveUnit( testVaribale ); 
	MObjectManager::GetInstance()->RemoveAllUnits();
	cout << "GamePlay -> ";

	//MObjectManager::GetInstance()->DeleteInstance();
}

void CGamePlayState::EnterCommand(void)
{
	while( true )
	{
		cout << "Command>";
		string command;
		cin >> command;

		if( command == "addentity" )
		{	
			cout << "Add Entity\n";
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			IUnitInterface* temp = new CBaseEntity();
			((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pokeball.png" );
			((CBaseEntity*)(temp))->SetIndexPosX( PosX );
			((CBaseEntity*)(temp))->SetIndexPosY( PosY );
			((CBaseEntity*)(temp))->SetPosX( (float)PosX * 32.0f );
			((CBaseEntity*)(temp))->SetPosY( (float)PosY * 32.0f );
			MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );

		}
		else if( command == "addobject" )
		{	
			cout << "Add Object\n";
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			IUnitInterface* temp = new CBaseObject();
			((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/heart.png" );
			((CBaseEntity*)(temp))->SetIndexPosX( PosX );
			((CBaseEntity*)(temp))->SetIndexPosY( PosY );
			((CBaseEntity*)(temp))->SetPosX( (float)PosX * 32.0f );
			((CBaseEntity*)(temp))->SetPosY( (float)PosY * 32.0f );
			MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );

		}
		else if( command == "remove" )
		{	
			cout << "Remove Object\n";
			cout << "\tID:";
			int toRemove;
			cin >> toRemove;

			MObjectManager::GetInstance()->RemoveUnit( toRemove );
		}
		else if( command == "help" )
		{
			cout << "\n==============================\n";
			cout << "Command\t\tDescription\n\n";

			cout << "addentity\t\tadds an entity\n";
			cout << "addobject\t\tadds an object\n";
			cout << "remove\t\tremoves an object\n";
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
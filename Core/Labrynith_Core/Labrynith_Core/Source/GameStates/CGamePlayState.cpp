#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CLoadLevelState.h"

#include <string>

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"

#include "../CGame.h"
#include "../Object Manager/Units/CBaseEntity.h"
#include "../Object Manager/Units/CBaseObject.h"
#include "../Messaging/MEventSystem.h"

#include <iostream>

using namespace std;

CGamePlayState::CGamePlayState()
{
	m_nCurrLevel = 0;
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

	MMessageSystem::GetInstance()->InitMessageSystem( CGamePlayState::MessageProc );
	
	CGame::GetInstance()->PushState( CLoadLevelState::GetInstance() );

	IUnitInterface* temp = new CBaseEntity();
	//((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pokeball.png" );
	((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/$rob.png" );
	//Load basic movement animations
	((CBaseEntity*)(temp))->LoadEntMoveAnimIDs();
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
		if( pDI->KeyDown( DIK_SPACE ) )
			MObjectManager::GetInstance()->FindLayer( testVaribale ).GetFlake( OBJECT_LIGHT ).SetInfoAtIndex( player->GetIndexPosX(), player->GetIndexPosY(), rand() % 15 + 240 );
				
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
	timestep = fDT;

	MEventSystem::GetInstance()->ProcessEvents();
	MMessageSystem::GetInstance()->ProcessMessages();
	MObjectManager::GetInstance()->Update( fDT );
}

void CGamePlayState::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	CBaseEntity* player = (CBaseEntity*)MObjectManager::GetInstance()->GetUnit( testVaribale );

	if( player )
	{
		int cameraX = (int)player->GetPosX() - 800/2 + 16;
		int cameraY = (int)player->GetPosY() - 600/2 + 16;

		if( cameraX + 800 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * 32 )
			cameraX = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * 32 - 800;

		if( cameraY + 600 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * 32 )
			cameraY = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * 32 - 600;

		if( cameraX < 0 )
			cameraX = 0;

		if( cameraY < 0 )
			cameraY = 0;

		MObjectManager::GetInstance()->Render( cameraX, cameraY );
	}
	else
	{
		MObjectManager::GetInstance()->Render( 0, 0 );
	}

	pD3D->DrawTextA( "Gameplay State", 100, 100 );

	char temp[64];

	sprintf( temp, "%f", timestep ); 

	CSGD_Direct3D::GetInstance()->DrawTextA( temp, 100, 126 );

	sprintf( temp, "%f", 1.0f / timestep ); 

	CSGD_Direct3D::GetInstance()->DrawTextA( temp, 100, 164 );
}

void CGamePlayState::Exit(void)
{
	MObjectManager::GetInstance()->RemoveUnit( testVaribale ); 
	MObjectManager::GetInstance()->RemoveAllUnits();
	MMessageSystem::GetInstance()->ShutdownMessageSystem();
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

		if( command == "setlight" )
		{	
			cout << "Set Light\n";
			cout << "\tValue:";
			int _value;
			cin >> _value;
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			MObjectManager::GetInstance()->FindLayer( testVaribale ).SetValueInFlakeAtIndex( _value, OBJECT_LIGHT, PosX, PosY );
		}
		else if( command == "addentity" )
		{	
			cout << "Add Entity\n";
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			MMessageSystem::GetInstance()->SendMsg( new msgCreateEntity( PosX, PosY ) ); 

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

			MEventSystem::GetInstance()->SendEvent( "Add.Object", temp );

		}
		else if( command == "settile" )
		{	
			cout << "Set Tile\n";
			cout << "\tValue:";
			int _value;
			cin >> _value;
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			MObjectManager::GetInstance()->FindLayer( testVaribale ).SetValueInFlakeAtIndex( _value, OBJECT_TILE, PosX, PosY );
		}
		else if( command == "scramble" )
		{	
			cout << "Set Tile\n";
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			for( int i = 0; i < PosX; ++i )
			{
				for( int n = 0; n < PosY; ++n )
				{
					MObjectManager::GetInstance()->FindLayer( testVaribale ).SetValueInFlakeAtIndex( rand() % 3, OBJECT_TILE, i, n );
				}
			}

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

void CGamePlayState::MessageProc( CBaseMessage* _message )
{
	
	switch(_message->GetMsgID())
	{
	case MSG_CREATE_ENTITY:
		{
			msgCreateEntity* NewMessage = (msgCreateEntity*)_message;

			IUnitInterface* temp = new CBaseEntity();
			((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/$rob.png" );
			//Load basic movement animations
			((CBaseEntity*)(temp))->LoadEntMoveAnimIDs();
			((CBaseEntity*)(temp))->SetPlayAnimWhileStill(true);
			((CBaseEntity*)(temp))->SetIndexPosX( NewMessage->GetX() );
			((CBaseEntity*)(temp))->SetIndexPosY( NewMessage->GetY() );
			((CBaseEntity*)(temp))->SetPosX( (float)NewMessage->GetX() * 32.0f );
			((CBaseEntity*)(temp))->SetPosY( (float)NewMessage->GetY() * 32.0f );
			MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
		}
		break;

	case MSG_REMOVE_UNIT:
		{
			msgRemoveUnit* NewMessage = (msgRemoveUnit*)_message;
			MObjectManager::GetInstance()->RemoveUnit( NewMessage->GetTarget() );
		}
		break;

	case MSG_TRANSFER_LIGHT:
		{
			msgTransferLight* NewMessage = (msgTransferLight*)_message;
			MFlake* FlakeScope = (MFlake*)NewMessage->GetFlake(); 

			//FlakeScope->SetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY(), FlakeScope->GetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY() ) - NewMessage->GetTransferValue() );
			FlakeScope->SetInfoAtIndex( NewMessage->GetTwoX(), NewMessage->GetTwoY(), NewMessage->GetTransferValue() );

		}
		break;
	}

}
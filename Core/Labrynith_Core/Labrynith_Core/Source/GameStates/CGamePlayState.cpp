#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CLoadLevelState.h"

#include <string>

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"

#include "../CGame.h"
#include "../Object Manager/Units/CBaseEntity.h"
#include "../Object Manager/Units/Objects/CAttractor.h"
#include "../Object Manager/Units/CPlayer.h"
#include "../Object Manager/Units/CBaseGolem.h"
#include "../Messaging/MEventSystem.h"
#include "../HUD/CHUD.h"


#include <iostream>

using namespace std;

int CGamePlayState::testVaribale = -1;

CGamePlayState::CGamePlayState()
{
	m_nCurrLevel = 1;
	testVaribale = -1;
	m_nMouseID = -1 ;
}

// destructor
CGamePlayState::~CGamePlayState() 
{

}

CGamePlayState* CGamePlayState::GetInstance()
{
	static CGamePlayState instance;
	return &instance;
}

void CGamePlayState::Enter(void)
{
	m_nMouseID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource\\mouse.png" ) ;

	cout << "GamePlay\n";

	MMessageSystem::GetInstance()->InitMessageSystem( CGamePlayState::MessageProc );
	
	CGame::GetInstance()->PushState( CLoadLevelState::GetInstance() );
	
}

bool CGamePlayState::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	

	if( pDI->KeyPressed( DIK_ESCAPE ) )
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );

	//BUG - temp code added for AI testing
	/*if( pDI->MouseButtonPressed( 0 ) )
	{
		IUnitInterface* golem = MObjectManager::GetInstance()->GetUnit(120001);
		int cameraX = 0 , cameraY = 0 ;
				CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
		int tileXPos = (int)((pDI->MouseGetPosX() + cameraX) / 32.0f) ;
		int tileYPos = (int)((pDI->MouseGetPosY() + cameraY) / 32.0f) ;
		((CBaseGolem*)(golem))->SetTargetPos(tileXPos, tileYPos);
	}*/

	return true;
}

void CGamePlayState::Update(float fDT)
{
	timestep = fDT;

	MEventSystem::GetInstance()->ProcessEvents();
	MMessageSystem::GetInstance()->ProcessMessages();
	MObjectManager::GetInstance()->Update( fDT );
}

void CGamePlayState::GetCamera( int& X , int& Y )
{
	CBaseEntity* player = (CBaseEntity*)MObjectManager::GetInstance()->GetUnit( testVaribale );

	if( player )
	{
		X = (int)player->GetPosX() - 800/2 + 16;
		Y = (int)player->GetPosY() - 600/2 + 16;

		if( X + 800 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * 32 )
			X = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * 32 - 800;
		
		if( Y + 600 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * 32 )
			Y = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * 32 - 600;

		if( X < 0 )
			X = 0;

		if( Y < 0 )
			Y = 0;

	}
	return ;
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
	//Draw the HUD
	CHUD::GetInstance()->Render();
	//pD3D->DrawTextA( "Gameplay State", 100, 100 );

	int mouseX = CSGD_DirectInput::GetInstance()->MouseGetPosX() ;
	int mouseY = CSGD_DirectInput::GetInstance()->MouseGetPosY() ;

	CSGD_TextureManager::GetInstance()->Draw( m_nMouseID , mouseX - 8 , mouseY - 2 ) ;

	/*char temp[64];

	sprintf( temp, "%f", timestep ); 

	CSGD_Direct3D::GetInstance()->DrawTextA( temp, 100, 126 );

	sprintf( temp, "%f", 1.0f / timestep ); 

	CSGD_Direct3D::GetInstance()->DrawTextA( temp, 100, 164 );*/
}

void CGamePlayState::Exit(void)
{
	//MObjectManager::GetInstance()->RemoveUnit( testVaribale ); 
	MObjectManager::GetInstance()->RemoveAllUnits();
	MMessageSystem::GetInstance()->ShutdownMessageSystem();
	cout << "GamePlay -> ";
	testVaribale = -1;

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
		else if( command == "setgolemtarget")
		{
			cout << "Set Target\n";
			cout << "\tID:";
			int golemID;
			cin >> golemID;
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			//get the object corresponding to the id
			IUnitInterface* golem = MObjectManager::GetInstance()->GetUnit(golemID);
			if(golem)
			{		
				if(golem->m_nUnitType == OBJECT_ENTITY)
				{
					if(((CBaseEntity*)(golem))->GetType() == ENT_GOLEM)
					{
						//we have a golem
						((CBaseGolem*)(golem))->SetTargetPos(PosX, PosY);
					}
					else
					{
						cout << "Entity is not a golem" << endl;
					}
				}
				else
				{
					cout << "Object is not an entity!" << endl;
				}
			}
			else
			{
				cout << golemID << " is invalid!" << endl;
			}
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
			((CBaseObject*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/heart.png" );
			((CBaseObject*)(temp))->SetIndexPosX( PosX );
			((CBaseObject*)(temp))->SetIndexPosY( PosY );
			((CBaseObject*)(temp))->SetPosX( (float)PosX * 32.0f );
			((CBaseObject*)(temp))->SetPosY( (float)PosY * 32.0f );

			//MEventSystem::GetInstance()->SendEvent( "Add.Object", temp );

			MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );

		}
		else if( command == "addattractorfire" )
		{
			cout << "Add fire attractor\n";
			cout << "\tX:";
			int PosX;
			cin >> PosX;
			cout << "\tY:";
			int PosY;
			cin >> PosY;

			CAttractor* temp = new CAttractor(FIRE_GOLEM) ;
			temp->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/heart.png" );
			temp->SetIndexPosX( PosX ) ;
			temp->SetIndexPosY( PosY ) ;
			temp->SetPosX( (float)PosX * 32.0f ) ;
			temp->SetPosY( (float)PosY * 32.0f ) ;

			MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
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
			((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IceGolem.png" );
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

	case MSG_CREATE_PLAYER:
		{
			//we only need one player object
			if(testVaribale != -1)
				return;

			msgCreatePlayer* NewMessage = (msgCreatePlayer*)_message;

			IUnitInterface* temp = new CPlayer();
			((CPlayer*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/MainCharacter.png" ));
			//Load basic movement animations
			((CPlayer*)(temp))->LoadEntMoveAnimIDs();
			((CPlayer*)(temp))->SetPlayAnimWhileStill(false);
			((CPlayer*)(temp))->SetIndexPosX( NewMessage->GetX() );
			((CPlayer*)(temp))->SetIndexPosY( NewMessage->GetY() );
			((CPlayer*)(temp))->SetPosX( (float)NewMessage->GetX() * 32.0f );
			((CPlayer*)(temp))->SetPosY( (float)NewMessage->GetY() * 32.0f );
			//set-up the HUD so it renders player info
			CHUD::GetInstance()->SetPlayer(((CPlayer*)(temp)));
			testVaribale = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
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
	
	case MSG_PLACE_OBJECT:
		{
			msgPlaceObject* NewMessage = (msgPlaceObject*)_message;
			int tileXPos = NewMessage->GetTileXPos() ;
			int tileYPos = NewMessage->GetTileYPos() ;
			CPlayer* player = (CPlayer*)MObjectManager::GetInstance()->GetUnit( testVaribale );

			if( !player->GetHeldItem() )	//	break if the player isnt holding an item
				break;

			// place object in that tile
			int checkObject = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			if( checkObject == 0 ) //	if tile is empty
			{
				IUnitInterface* object = player->GetHeldItem() ;
				object->SetIndexPosX( tileXPos ) ;
				object->SetIndexPosY( tileYPos ) ;
				int PlacedID = MObjectManager::GetInstance()->AddUnitIndexed( player->GetHeldItem() , 1 ) ;
				MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).SetInfoAtIndex( tileXPos , tileYPos , PlacedID ) ;
				player->GetHeldItem()->SetPosX( tileXPos * 32 ) ;
				player->GetHeldItem()->SetPosY( tileYPos * 32 ) ;
				player->SetHeldItem(NULL);

				if( object->GetType() == ENT_ATTRACTOR )
				{
					MEventSystem::GetInstance()->SendEvent( "ATTRACTORPLACED" , object ) ;
				}
			}
		}
		break;

	case MSG_PICKUP_OBJECT:
		{
			msgPickUpObject* NewMessage = (msgPickUpObject*)_message;
			CBaseObject* pBase = NewMessage->GetMsgObject() ;
			CPlayer* player = (CPlayer*)MObjectManager::GetInstance()->GetUnit( testVaribale );

			int cameraX = 0 , cameraY = 0 ;
			CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
			int tileXPos = (int)((pBase->GetPosX() + cameraX) / 32.0f) ;
			int tileYPos = (int)((pBase->GetPosY() + cameraY) / 32.0f) ;

			int ObjectID = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			MObjectManager::GetInstance()->RemoveUnit( ObjectID ) ;
			MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).SetInfoAtIndex( tileXPos , tileYPos , 0 ) ;
			player->SetHeldItem(pBase);
			//pBase->SetIndexPosX(-1);
			//pBase->SetIndexPosY(-1);
			MEventSystem::GetInstance()->SendEvent( "ATTRACTORREMOVED" , pBase ) ;
		}
		break;
	}

}

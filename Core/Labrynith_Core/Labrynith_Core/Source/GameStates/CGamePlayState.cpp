#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CLoadLevelState.h"
#include "CGameOverState.h"
#include "CPauseState.h"
#include "COptionsState.h"
#include "CCreditsState.h"

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
#include "../Wrappers/CSGD_FModManager.h"

#include "../Object Manager/Units/Golems/CGolem_Earth.h"
#include "../Object Manager/Units/Golems/CGolem_Fire.h"
#include "../Object Manager/Units/Golems/CGolem_Water.h"
#include "../Object Manager/Units/Golems/CGolem_Ice.h"
#include "../Object Manager/Units/Golems/CGolem_Air.h"
#include "../Object Manager/Units/Golems/CGolem_Lava.h"
#include "../Object Manager/Units/Golems/CGolem_Iron.h"
#include "../Object Manager/Units/Golems/CGolem_Light.h"
#include "../Object Manager/Units/Golems/CGolem_Shadow.h"
#include "../Object Manager/Units/Objects/CSpawner.h"

#include <iostream>

using namespace std;

CGamePlayState::CGamePlayState()
{
	m_nCurrLevel = 5;
	testVaribale = -1;
	m_nMouseID = -1 ;
	currFloor = 1;
	numLevelFloors = 1;
	m_bRenderCulling = true;
	SetPaused(false);

	cameraX = 0;
	cameraY = 0;
	PlayGameSongID = -1;

	m_bIsPaused = false;

	m_bDrawMouse = true;
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
	
	if(PlayGameSongID == -1)
	{
		int decision = rand() % 3;

		switch (decision)
		{
		case 0:
			PlayGameSongID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/Music/chapter1.mp3", FMOD_LOOP_NORMAL);
			break;
		case 1:
			PlayGameSongID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/Music/chapter2.mp3", FMOD_LOOP_NORMAL);
			break;
		case 2:
			PlayGameSongID = CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/Music/chapter3.mp3", FMOD_LOOP_NORMAL);
			break;
		}
	}

	COptionsState::GetInstance()->AdjustSound(PlayGameSongID, false);

	if( !CSGD_FModManager::GetInstance()->IsSoundPlaying(PlayGameSongID) )
		CSGD_FModManager::GetInstance()->PlaySoundA( PlayGameSongID );


	MMessageSystem::GetInstance()->InitMessageSystem( CGamePlayState::MessageProc );
	
	CGame::GetInstance()->PushState( CLoadLevelState::GetInstance() );

	m_nIMG_Black = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/black.png" ); 
	m_bIsOver = false;
	m_fCountdown = 0.0f;

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
	
	cout << "Leaving CGamePlayStates'Enter\n";
}

bool CGamePlayState::Input(void)
{
	if(IsPaused())
		return true;

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	

	if( pDI->KeyPressed( DIK_ESCAPE )||
		pDI->JoystickButtonPressed(2) )
	{
		//m_bIsPaused = true;
		m_bDrawMouse = false;
		CGame::GetInstance()->PushState( CPauseState::GetInstance() );
	}
	
	if( pDI->KeyPressed( DIK_T ) )
		m_bRenderCulling = !m_bRenderCulling;

	//BUG - temp code added for AI testing
	//if( pDI->MouseButtonPressed( 0 ) )
	//{
	//	/*IUnitInterface* golem = MObjectManager::GetInstance()->GetUnit(120003);
	//	int cameraX = 0 , cameraY = 0 ;
	//			CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);
	//	int tileXPos = (int)((pDI->MouseGetPosX() + cameraX) / TILE_WIDTH.0f) ;
	//	int tileYPos = (int)((pDI->MouseGetPosY() + cameraY) / TILE_HEIGHT) ;
	//	((CBaseGolem*)(golem))->SetTargetPos(tileXPos, tileYPos);*/
	//}

	return true;
}

void CGamePlayState::Update(float fDT)
{

	if( m_bIsOver )
		m_fCountdown += fDT;
	else
		MObjectManager::GetInstance()->Update( fDT );

	if(IsPaused())
		return;

	timestep = fDT;
	MEventSystem::GetInstance()->ProcessEvents();
	MMessageSystem::GetInstance()->ProcessMessages();

	int cameraX = 0 , cameraY = 0 ;
	CGamePlayState::GetInstance()->GetCamera(cameraX , cameraY);

	if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < 0 )
		CSGD_DirectInput::GetInstance()->MouseSetPosX( 0 ) ;
	else if( CSGD_DirectInput::GetInstance()->MouseGetPosX() > CGame::GetInstance()->GetScreenWidth() - 20 )
		CSGD_DirectInput::GetInstance()->MouseSetPosX( CGame::GetInstance()->GetScreenWidth() - 20 ) ;
	if( CSGD_DirectInput::GetInstance()->MouseGetPosY() < 0 )
		CSGD_DirectInput::GetInstance()->MouseSetPosY( 0 ) ;
	else if( CSGD_DirectInput::GetInstance()->MouseGetPosY() > CGame::GetInstance()->GetScreenHeight() - 20 )
		CSGD_DirectInput::GetInstance()->MouseSetPosY( CGame::GetInstance()->GetScreenHeight() - 20 ) ;

	if( m_fCountdown >= 15.0f )
		CGame::GetInstance()->ChangeState( CCreditsState::GetInstance() );

	m_bDrawMouse = true;

}

void CGamePlayState::GetCamera( int& X , int& Y )
{
	CBaseEntity* player = (CBaseEntity*)MObjectManager::GetInstance()->GetUnit( testVaribale );

	if( player )
	{
		X = (int)player->GetPosX() - 800/2 + 32;
		Y = (int)player->GetPosY() - 600/2 + 32;

		if( X + 800 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * TILE_WIDTH )
			X = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * TILE_WIDTH - 800;
		
		if( Y + 600 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * TILE_HEIGHT )
			Y = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * TILE_HEIGHT - 600;

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

	CBaseEntity* player = nullptr;
	if( testVaribale > 0 )
	{
		player = (CBaseEntity*)MObjectManager::GetInstance()->GetUnit( testVaribale );
	}
	else
	{		
		cout << "No player to render\n";
	}

	if( player )
	{
		cameraX = (int)player->GetPosX() - 800/2 + 32;
		cameraY = (int)player->GetPosY() - 600/2 + 32;

		if( cameraX + 800 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * TILE_WIDTH )
			cameraX = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerWidth() * TILE_WIDTH - 800;

		if( cameraY + 600 > MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * TILE_HEIGHT )
			cameraY = MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerHeight() * TILE_HEIGHT - 600;

		if( cameraX < 0 )
			cameraX = 0;

		if( cameraY < 0 )
			cameraY = 0;

		MObjectManager::GetInstance()->Render( cameraX, cameraY, MObjectManager::GetInstance()->FindLayer( testVaribale ).GetLayerID() );
	}
	else
	{
		MObjectManager::GetInstance()->Render( 0, 0, 0 );
	}


	//Draw the HUD
	CHUD::GetInstance()->Render();
	//pD3D->DrawTextA( "Gameplay State", 100, 100 );

	int mouseX = CSGD_DirectInput::GetInstance()->MouseGetPosX() ;
	int mouseY = CSGD_DirectInput::GetInstance()->MouseGetPosY() ;

	if( m_bDrawMouse )
		CSGD_TextureManager::GetInstance()->Draw( m_nMouseID , mouseX - 8 , mouseY - 2 ) ;
	
	MMessageSystem::GetInstance()->ProcessMessages();

	//char temp[64];

	//sprintf_s( temp, "%f", timestep ); 
	//sprintf( temp, "%f", timestep ); 

	//CSGD_Direct3D::GetInstance()->DrawTextA( temp, 100, 126 );

	//sprintf( temp, "%f", 1.0f / timestep ); 

	//CSGD_Direct3D::GetInstance()->DrawTextA( temp, 100, 164 );

	if( m_bIsOver )
	{
		int fade = int( 255 * ( m_fCountdown / 10.0f ) ); 

		if( fade > 255 )
			fade = 255;

		CSGD_TextureManager::GetInstance()->Draw( m_nIMG_Black, 0, 0, 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( fade, 255, 255, 255 ) ); 

		MetalText.Print( "But Now You Must Escape . . .", 800 / 2 - 256, 600 / 2, 0.5f );
	}
}

void CGamePlayState::Exit(void)
{
	MMessageSystem::GetInstance()->ShutdownMessageSystem();
	MEventSystem::GetInstance()->ShutdownEventSystem();
	cout << "GamePlay -> ";
	testVaribale = -1;	
	CSGD_FModManager::GetInstance()->StopSound(PlayGameSongID);
	PlayGameSongID = -1;
}

void CGamePlayState::KillPlayer(void)
{
	CGame::GetInstance()->PushState( CGameOverState::GetInstance() );
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
			((CBaseObject*)(temp))->SetPosX( (float)PosX * TILE_WIDTH );
			((CBaseObject*)(temp))->SetPosY( (float)PosY * TILE_HEIGHT );

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
			temp->SetPosX( (float)PosX * TILE_WIDTH ) ;
			temp->SetPosY( (float)PosY * TILE_HEIGHT ) ;

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

			CBaseEntity* temp = new CBaseEntity();
			((CBaseEntity*)(temp))->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IceGolem.png" );
			((CBaseEntity*)(temp))->SetPlayAnimWhileStill(true);
			((CBaseEntity*)(temp))->SetIndexPosX( NewMessage->GetX() );
			((CBaseEntity*)(temp))->SetIndexPosY( NewMessage->GetY() );
			((CBaseEntity*)(temp))->SetPosX( (float)NewMessage->GetX() * TILE_WIDTH );
			((CBaseEntity*)(temp))->SetPosY( (float)NewMessage->GetY() * TILE_HEIGHT );
			MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
		}
		break;

	case MSG_CREATE_PLAYER:
		{
			//we only need one player object
			//if(testVaribale != -1)
				//return;

			msgCreatePlayer* NewMessage = (msgCreatePlayer*)_message;

			CPlayer* temp = new CPlayer();
			temp->SetLayerLocation(NewMessage->GetZ());
			((CPlayer*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Player.png" ));
			((CPlayer*)(temp))->SetPlayAnimWhileStill(false);
			((CPlayer*)(temp))->SetIndexPosX( NewMessage->GetX() );
			((CPlayer*)(temp))->SetIndexPosY( NewMessage->GetY() );
			((CPlayer*)(temp))->SetPosX( (float)NewMessage->GetX() * TILE_WIDTH );
			((CPlayer*)(temp))->SetPosY( (float)NewMessage->GetY() * TILE_HEIGHT );
			//set-up the HUD so it renders player info
			CHUD::GetInstance()->SetPlayer(temp);
			CGamePlayState::GetInstance()->testVaribale = MObjectManager::GetInstance()->AddUnitIndexed( temp, NewMessage->GetZ() );
			//CHUD::GetInstance()->SetPlayer(testVaribale);
		}
		break;

	case MSG_REMOVE_UNIT:
		{
			MObjectManager* OM = MObjectManager::GetInstance();
			msgRemoveUnit* NewMessage = (msgRemoveUnit*)_message;
			OM->RemoveUnit( NewMessage->GetTarget() );
			MEventSystem::GetInstance()->SendEvent("spawner.spawn");
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

	case MSG_DRAIN_LIGHT:
		{
			msgDrainLight* NewMessage = (msgDrainLight*)_message;
			MFlake* FlakeScope = (MFlake*)NewMessage->GetFlake(); 

			//FlakeScope->SetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY(), FlakeScope->GetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY() ) - NewMessage->GetTransferValue() );
			FlakeScope->SetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY(), 
				FlakeScope->GetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY() ) - NewMessage->GetTransferValue() );

			if( FlakeScope->GetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY() ) < 0 )
				FlakeScope->SetInfoAtIndex( NewMessage->GetOneX(), NewMessage->GetOneY(), 0 );
		}
		break;
	
	case MSG_PLACE_OBJECT:
		{
			msgPlaceObject* NewMessage = (msgPlaceObject*)_message;
			int tileXPos = NewMessage->GetTileXPos() ;
			int tileYPos = NewMessage->GetTileYPos() ;
			CPlayer* player = (CPlayer*)MObjectManager::GetInstance()->GetUnit( CGamePlayState::GetInstance()->testVaribale );

			if( !player->GetHeldItem() )	//	break if the player isnt holding an item
				break;

			// place object in that tile
			//int checkObject = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			//if( checkObject == 0 ) //	if there is NOT an object there
			//{
			   //checkObject = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			   //if( checkObject != 0 ) // if there IS an entity there
				  // break;
			   //
			   //checkObject = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( tileXPos , tileYPos ) ;
			   //if( checkObject == 0 ) // if the tile is NOT empty
				  // break;


				
				IUnitInterface* object = player->GetHeldItem() ;
				object->SetIndexPosX( tileXPos ) ;
				object->SetIndexPosY( tileYPos ) ;
				int PlacedID = MObjectManager::GetInstance()->AddUnitIndexed( player->GetHeldItem(),
					MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetLayerID());

				player->GetHeldItem()->SetPosX( (float)(tileXPos * TILE_WIDTH) );
				player->GetHeldItem()->SetPosY( (float)(tileYPos * TILE_HEIGHT) );
				player->GetHeldItem()->Release();
				player->SetHeldItem(NULL);

				if( object->GetType() == OBJ_ATTRACTOR )
				{
					MEventSystem::GetInstance()->SendEvent( "ATTRACTORPLACED" , object ) ;
				}
				CSGD_FModManager::GetInstance()->PlaySoundA(player->GetPutDownSoundID()) ;
			//}
		}
		break;

	case MSG_PICKUP_OBJECT:
		{
			msgPickUpObject* NewMessage = (msgPickUpObject*)_message;
			CBaseObject* pBase = NewMessage->GetMsgObject() ;
			CPlayer* player = (CPlayer*)MObjectManager::GetInstance()->GetUnit( CGamePlayState::GetInstance()->testVaribale );

			if( pBase->GetType() == OBJ_POWERGLOVES || pBase->GetType() == OBJ_OILCAN )
			{
				if( player->GetEquippedItem() == NULL )
				{
					player->SetEquippedItem(pBase) ;
					pBase->AddRef();
				}
				else if( player->GetHeldItem() == NULL )
				{
					player->SetHeldItem(pBase) ;
					pBase->AddRef();
				}
				else
					break;
			}
			else
			{
				if( player->GetHeldItem() == NULL )
				{
					player->SetHeldItem(pBase);
					pBase->AddRef();
				}
				else if( player->GetEquippedItem() == NULL && pBase->GetType() == OBJ_LIGHTORB )
				{
					player->SetEquippedItem(pBase) ;
					pBase->AddRef();
				}
				else
					break;
			}
			int ObjectID = MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( pBase->GetIndexPosX() , pBase->GetIndexPosY() ) ;
			MObjectManager::GetInstance()->RemoveUnit( ObjectID ) ;
			//MObjectManager::GetInstance()->FindLayer( player->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).SetInfoAtIndex( pBase->GetIndexPosX() , pBase->GetIndexPosY() , 0 ) ;
			

			//pBase->SetIndexPosX(-1);
			//pBase->SetIndexPosY(-1);
			if( pBase->GetType() == OBJ_ATTRACTOR )
			{
				MEventSystem::GetInstance()->SendEvent( "ATTRACTORREMOVED" , pBase ) ;
			}
			CSGD_FModManager::GetInstance()->PlaySoundA(player->GetPickUpSoundID()) ;
		}
		break;

	case MSG_MOVE_ENTITY_FLOOR:
		{			
			MObjectManager* OM = MObjectManager::GetInstance();
			msgMoveEntityFloor* msg = (msgMoveEntityFloor*)_message;

			CBaseEntity* pEntity = msg->GetEntity();
			int posx = msg->GetPosX();
			int posy = msg->GetPosY();
			
			IDHolder* idholder = new IDHolder();
			idholder->oldID = pEntity->m_nIdentificationNumber;


			pEntity->SetLayerLocation(msg->GetFloor());
			if(posx > -1)
			{
				pEntity->SetPosX((float)posx * TILE_WIDTH);
				pEntity->SetLastPosX(pEntity->GetPosX());
				pEntity->SetIndexPosX(posx);
			}
			if(posy > -1)
			{
				pEntity->SetPosY((float)posy * TILE_HEIGHT);
				pEntity->SetLastPosY(pEntity->GetPosY());
				pEntity->SetIndexPosY(posy);
			}
			pEntity->AddRef();
			OM->RemoveUnit(pEntity->m_nIdentificationNumber);
			pEntity->m_nIdentificationNumber = 0;

			if(pEntity->GetType() == ENT_PLAYER)
			{
				MEventSystem::GetInstance()->SendEvent("spawner.createdplayer", (void*)CGamePlayState::GetInstance()->testVaribale);
				
				idholder->newID = CGamePlayState::GetInstance()->testVaribale = OM->AddUnitIndexed(pEntity, msg->GetFloor());	
				if(((CPlayer*)OM->GetUnit(CGamePlayState::GetInstance()->testVaribale))->GetHeldItem())
					((CPlayer*)OM->GetUnit(CGamePlayState::GetInstance()->testVaribale))->GetHeldItem()->SetLayerLocation(msg->GetFloor());
				
				if(((CPlayer*)OM->GetUnit(CGamePlayState::GetInstance()->testVaribale))->GetEquippedItem())
					((CPlayer*)OM->GetUnit(CGamePlayState::GetInstance()->testVaribale))->GetEquippedItem()->SetLayerLocation(msg->GetFloor());
			}
			else
			{
				idholder->newID = pEntity->m_nIdentificationNumber = OM->AddUnitIndexed(pEntity, msg->GetFloor());
			}

			MEventSystem::GetInstance()->SendEvent("spawner.idchanged", (void*)idholder);

			pEntity->Release();
		}
		break;

		
	case MSG_REMOVE_GOLEM_COMBINED:
		{			
			MObjectManager* OM = MObjectManager::GetInstance();
			msgRemoveGolemCombined* msg = (msgRemoveGolemCombined*)_message;
			
			IDHolder* idholder = new IDHolder();
			idholder->oldID = msg->GetOldID();
			idholder->newID = *msg->GetGolemChanging();


			MMessageSystem::GetInstance()->SendMsg(new msgRemoveUnit(msg->GetOldID()));
			MEventSystem::GetInstance()->SendEvent("spawner.idchanged", (void*)idholder);

			delete msg->GetGolemChanging();
		}
		break;

	case MSG_CHANGE_GOLEM_TYPE:
		{
			MObjectManager* OM = MObjectManager::GetInstance();
			msgChangeGolemType* msg = (msgChangeGolemType*)_message;
			CBaseGolem* pGolem = msg->GetGolem();
			int nType = msg->GetGolemType();

			if(pGolem->GetType() == msg->GetGolemType())
				return;
			
			IDHolder* idholder = new IDHolder();
			idholder->oldID = pGolem->m_nIdentificationNumber;

			//Make a new golem based on type
			CBaseGolem* pNewGolem = NULL;
			switch(nType)
			{
			case EARTH_GOLEM:
				{
					pNewGolem = new CGolem_Earth(pGolem);
				}
				break;

			case FIRE_GOLEM:
				{
					pNewGolem = new CGolem_Fire(pGolem);
				}
				break;

			case WATER_GOLEM:
				{
					pNewGolem = new CGolem_Water(pGolem);
				}
				break;

			case ICE_GOLEM:
				{
					pNewGolem = new CGolem_Ice(pGolem);
				}
				break;

			case AIR_GOLEM:
				{
					pNewGolem = new CGolem_Air(pGolem);
				}
				break;

			case LAVA_GOLEM:
				{
					pNewGolem = new CGolem_Lava(pGolem);
				}
				break;

			case IRON_GOLEM:
				{
					pNewGolem = new CGolem_Iron(pGolem);
				}
				break;

			case SHADOW_GOLEM:
				{
					pNewGolem = new CGolem_Shadow(pGolem);
				}
				break;

			case LIGHT_GOLEM:
				{
					pNewGolem = new CGolem_Light(pGolem);
				}
				break;
			};
			//remove the current golem and add the new one
			//in its place
			int layerlocation = pGolem->GetLayerLocation();
			OM->RemoveUnit(pGolem->m_nIdentificationNumber);
			idholder->newID = OM->AddUnitIndexed(pNewGolem, layerlocation);

			if(msg->GetNewID())
				msg->SetNewID(idholder->newID);

			MEventSystem::GetInstance()->SendEvent("spawner.idchanged", (void*)idholder);
		}
		break;

	case MSG_DELETEME:
		{
			msgDeleteMe* msg = (msgDeleteMe*)_message;
			IUnitInterface* unit = msg->GetPointer();
			delete unit;
			unit = NULL;
			MEventSystem::GetInstance()->SendEvent("spawner.spawn");
		}
		break;

	case DELETE_IDHOLDER:
		{			
			msgDeletIDHolder* msg = (msgDeletIDHolder*)_message;
			IDHolder* idHolder = msg->GetPointer();
			if(idHolder && idHolder->oldID > 0 && idHolder->newID > 0 && idHolder->oldID < 900000 && idHolder->newID < 900000)
			{
				delete idHolder;
				idHolder = NULL;
			}
		}
		break;

	case MSG_DRAW_GENERATORTOP:
		{

			msgDrawGeneratorTop* msg = (msgDrawGeneratorTop*)_message;

			CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

			CSGD_TextureManager::GetInstance()->Draw( msg->GetTop(), msg->GetTileXPos(), msg->GetTileYPos(), 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 255, msg->GetBright(), msg->GetBright(), msg->GetBright() )  );

			break;
		}

	case MSG_END_GAME:
		CGamePlayState::GetInstance()->m_bIsOver = true;
		break;
	}
}
//accessors
bool CGamePlayState::IsPaused() const
{
	return m_bIsPaused;
}
//mutators
void CGamePlayState::SetPaused(const bool bIsPaused)
{
	m_bIsPaused = bIsPaused;
}

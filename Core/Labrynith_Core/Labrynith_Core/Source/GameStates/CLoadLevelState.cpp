
#include "CLoadLevelState.h"
#include "CGamePlayState.h"
#include "CMainMenuState.h"

#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../CGame.h"

#include <iostream>
#include <stdlib.h>

#include <string>
using std::string;

#include "../Object Manager/Units/Objects/CSpawner.h"
#include "../Object Manager/Units/Objects/CAttractor.h"
#include "../Object Manager/Units/Objects/CPowerGloves.h"
#include "../Object Manager/Units/Objects/COilCan.h"
#include "../Object Manager/Units/Objects/COil.h"

#include "../Object Manager/Units/Tiles/CButton.h"
#include "../Object Manager/Units/Tiles/CDoor.h"
#include "../Object Manager/Units/Tiles/CExit.h"
#include "../Object Manager/Units/Tiles/CWaterTile.h"
#include "../Object Manager/Units/Tiles/CPit.h"
#include "../Object Manager/Units/Tiles/CRamp.h"

#include "../TinyXML/tinyxml.h"
#include "../AI Handler/CAI_Handler.h"

// default constructor
CLoadLevelState::CLoadLevelState() 
{
}

// destructor
CLoadLevelState::~CLoadLevelState() { }

CLoadLevelState* CLoadLevelState::GetInstance()
{
	static CLoadLevelState instance;
	return &instance;
}

void CLoadLevelState::Enter(void)
{
	cout << "Deleting current level...\n";	

	MObjectManager::GetInstance()->RemoveAllUnits();
	MMessageSystem::GetInstance()->ShutdownMessageSystem();
	MEventSystem::GetInstance()->ShutdownEventSystem();
	CAI_Handler::GetInstance()->ClearEntityList();
	CGamePlayState::GetInstance()->testVaribale = -1;

	cout << "...current level deleted\n";	

	cout << "Loading Level...\n";
	MMessageSystem::GetInstance()->InitMessageSystem( CGamePlayState::MessageProc );
	LoadLevel(CGamePlayState::GetInstance()->GetCurrentLevel());
	cout << "...Level Loaded\n";
	
	printf("Spawning objects...\n");

	CGame::GetInstance()->PopState();
}

bool CLoadLevelState::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	
	
	if( pDI->KeyPressed( DIK_ESCAPE ) )
		return false;

	if( pDI->KeyPressed( DIK_RETURN ) )
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );

	return true;
}

void CLoadLevelState::Update(float fDT)
{

}

void CLoadLevelState::Render(void)
{
	
}

void CLoadLevelState::Exit(void)
{

}

void CLoadLevelState::EnterCommand(void)
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

bool CLoadLevelState::LoadLevel(int _level)
{		
	char filename[255] = {0};

	sprintf(filename, "resource/Levels/%d.xml", _level);

	TiXmlDocument doc;
	if( doc.LoadFile(filename) == false)
	{
		string tmp = "Could not find the FILE: (";
		tmp += filename;
		tmp += ") to load\n";
		cerr << tmp;
		return false;
	}

	
	TiXmlElement* pRoot = doc.RootElement();
	if(!pRoot)
	{
		cerr << "Could not find ROOT node within file\n";
		return false;
	}
	
	int width, height, depth;
	TiXmlElement* pMapSize = pRoot->FirstChildElement("MapSize");	
	if(pMapSize)
	{
		string swidth, sheight, sdepth;
		const int thingy = 5;

		swidth = pMapSize->FirstChildElement("Width")->GetText();
		sheight = pMapSize->FirstChildElement("Height")->GetText();
		sdepth = pMapSize->FirstChildElement("Depth")->GetText();

		width = atoi(swidth.c_str());
		height = atoi(sheight.c_str());
		depth = atoi(sdepth.c_str());
		
	}

	TiXmlElement* pTiles = pRoot->FirstChildElement("Tiles");	
	if(pTiles)
	{
		TiXmlElement* pArrayofArray = pTiles->FirstChildElement("ArrayOfArrayOfTile");
		int z = 1;
		while(pArrayofArray)
		{
			MObjectManager::GetInstance()->ResizeLayer( z, width, height );
			TiXmlElement* pArray = pArrayofArray->FirstChildElement("ArrayOfTile");
			int x = 0;
			while(pArray)
			{				
				TiXmlElement* pTile = pArray->FirstChildElement("Tile");
				int y = 0;
				while(pTile)
				{								
					string prop = "";

					

					TiXmlElement* pType = pTile->FirstChildElement("Type");
					if(pType)
					{
						TiXmlElement* pProp = pTile->FirstChildElement("Property");
						if(pProp)
						{			
							prop = pProp->GetText();
						}

						string sTypeX, sTypeY;

						sTypeX = pType->FirstChildElement("X")->GetText();
						sTypeY = pType->FirstChildElement("Y")->GetText();

						int TypeX, TypeY, theType;
						TypeX = atoi(sTypeX.c_str());
						TypeY = atoi(sTypeY.c_str());

						theType = TypeX + (TypeY * 5);
						
						switch(theType)
						{
						case -6: // -1, -1 wall
							theType = 0;
							break;

						case 0: // 0, 0 floor
							theType = 1;
							break;

						case 1: // 0, 1 button
							{
							IUnitInterface* temp = new CButton(prop.c_str());
							((CButton*)temp)->SetPosX((float)(x * 32));
							((CButton*)temp)->SetPosY((float)(y * 32));
							((CButton*)temp)->SetIndexPosX(x);
							((CButton*)temp)->SetIndexPosY(y);
							((CBaseObject*)(temp))->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/singleTile.png" ));
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );

							theType = 1;
							break;
							}
							
						case 2: // 0, 2 door
							{
							IUnitInterface* temp = new CDoor(prop.c_str());
							((CDoor*)temp)->SetPosX((float)(x * 32));
							((CDoor*)temp)->SetPosY((float)(y * 32));
							((CDoor*)temp)->SetIndexPosX(x);
							((CDoor*)temp)->SetIndexPosY(y);
							((CBaseObject*)(temp))->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pokeball.png" ));
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );

							theType = 1;
							break;
							}

						case 3: // 0, 3 exit tile
							{
							IUnitInterface* temp = new CExit();
							((CExit*)temp)->SetPosX((float)(x * 32));
							((CExit*)temp)->SetPosY((float)(y * 32));
							((CExit*)temp)->SetIndexPosX(x);
							((CExit*)temp)->SetIndexPosY(y);
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							theType = 1;
							}
							break;
							
						case 4: // 0, 4 Wood tile
							{
							IUnitInterface* temp = new COil();
							((COil*)temp)->SetPosX((float)(x * 32));
							((COil*)temp)->SetPosY((float)(y * 32));
							((COil*)temp)->SetIndexPosX(x);
							((COil*)temp)->SetIndexPosY(y);
							((CBaseObject*)(temp))->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/wood.png" ));
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );

							theType = 1;
							break;
							}
							
						case 5: // water tile
							{
							IUnitInterface* temp = new CWaterTile(false);
							((CWaterTile*)temp)->SetPosX((float)(x * 32));
							((CWaterTile*)temp)->SetPosY((float)(y * 32));
							((CWaterTile*)temp)->SetIndexPosX(x);
							((CWaterTile*)temp)->SetIndexPosY(y);
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							theType = 3;
							}
							break;
						case 6: // ice tile
							{
							IUnitInterface* temp = new CWaterTile(true);
							((CWaterTile*)temp)->SetPosX((float)(x * 32));
							((CWaterTile*)temp)->SetPosY((float)(y * 32));
							((CWaterTile*)temp)->SetIndexPosX(x);
							((CWaterTile*)temp)->SetIndexPosY(y);
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							theType = 3;
							}
							break;

						case 7: // pit tile
							{
							IUnitInterface* temp = new CWaterTile(true);
							((CWaterTile*)temp)->SetPosX((float)(x * 32));
							((CWaterTile*)temp)->SetPosY((float)(y * 32));
							((CWaterTile*)temp)->SetIndexPosX(x);
							((CWaterTile*)temp)->SetIndexPosY(y);
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							theType = 3;
							}
							break;
						
						case 8: // ramp tile
							{
							IUnitInterface* temp = new CWaterTile(true);
							((CWaterTile*)temp)->SetPosX((float)(x * 32));
							((CWaterTile*)temp)->SetPosY((float)(y * 32));
							((CWaterTile*)temp)->SetIndexPosX(x);
							((CWaterTile*)temp)->SetIndexPosY(y);
							MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							theType = 3;
							}
							break;
						}						

						// 0 = pit
						// 1 & 2 = ground
						MObjectManager::GetInstance()->GetLayer( z ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, theType );
					}

						
					++y;
					pTile = pTile->NextSiblingElement("Tile");
				}

				
				++x;
				pArray = pArray->NextSiblingElement("ArrayOfTile");
			}

			++z;
			pArrayofArray = pTiles->NextSiblingElement("ArrayOfArrayOfTile");
		}
	}


	TiXmlElement* pObjects = pRoot->FirstChildElement("Objects");	
	if(pObjects)
	{
		TiXmlElement* pObject = pObjects->FirstChildElement("TileObject");
		while(pObject)
		{			
			int TypeX, TypeY, theType;
			int posX, posY, posZ;
			string prop = "";

			TiXmlElement* pType = pObject->FirstChildElement("Type");
			if(pType)
			{
				string sTypeX, sTypeY;

				sTypeX = pType->FirstChildElement("X")->GetText();
				sTypeY = pType->FirstChildElement("Y")->GetText();

				TypeX = atoi(sTypeX.c_str());
				TypeY = atoi(sTypeY.c_str());

				theType = TypeX + (TypeY * 5); //Type.X + (Type.Y * tileSetSize.Width)
			}
			
			TiXmlElement* pPos = pObject->FirstChildElement("Position");
			if(pPos)
			{			
				string sposX, sposY, sposZ;

				sposX = pPos->FirstChildElement("X")->GetText();
				sposY = pPos->FirstChildElement("Y")->GetText();
				sposZ = pPos->FirstChildElement("Z")->GetText();

				posX = atoi(sposX.c_str());
				posY = atoi(sposY.c_str());
				posZ = atoi(sposZ.c_str()) + 1;
			}

			TiXmlElement* pProp = pObject->FirstChildElement("Property");
			if(pProp)
			{			
				prop = pProp->GetText();
			}
			
			switch(theType)
			{
			case 0:				
				{
					string typeofspawner;
					for(int i = 0; i < prop.length(); ++i)
					{
						if(prop[i] == '.')
							break;

						typeofspawner += prop[i];
					}

					if(typeofspawner == "player")
					{
						// change this code to a spawner of Player type
						IUnitInterface* temp = new CSpawner(SPAWNER_PLAYER);
						((CSpawner*)temp)->SetPosX((float)(posX * 32));
						((CSpawner*)temp)->SetPosY((float)(posY * 32));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );
						break;
					}
					else if(typeofspawner == "lightorb")
					{						
						IUnitInterface* temp = new CSpawner(SPAWNER_LIGHTORB);
						((CSpawner*)temp)->SetPosX((float)(posX * 32));
						((CSpawner*)temp)->SetPosY((float)(posY * 32));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );
					}
					else if(typeofspawner == "attractor")
					{
						string typeofattractor;
						bool beginreading = false;
						for(int i = 0; i < prop.length(); ++i)
						{
							if(beginreading)
								typeofattractor += prop[i];

							if(prop[i] == '.')
								beginreading = true;
						}
						
						IUnitInterface* temp;
						if(typeofattractor == "earth")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_EARTH);
						}
						else if(typeofattractor == "air")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_AIR);
						}
						else if(typeofattractor == "fire")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_FIRE);
						}
						else if(typeofattractor == "water")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_WATER);
						}
						else if(typeofattractor == "ice")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_ICE);
						}
						else if(typeofattractor == "shadow")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_SHADOW);
						}
						else if(typeofattractor == "light")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_LIGHT);
						}
						else if(typeofattractor == "iron")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_IRON);
						}
						else if(typeofattractor == "lava")
						{
							temp = new CSpawner(SPAWNER_ATTRACTOR_LAVA);
						}

						
						((CSpawner*)temp)->SetPosX((float)(posX * 32));
						((CSpawner*)temp)->SetPosY((float)(posY * 32));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );

						break;
					}
					else if(typeofspawner == "golem")
					{
						string typeofgolem;
						bool beginreading = false;
						for(int i = 0; i < prop.length(); ++i)
						{
							if(beginreading)
								typeofgolem += prop[i];

							if(prop[i] == '.')
								beginreading = true;
						}
						
						IUnitInterface* temp;
						if(typeofgolem == "earth")
						{
							temp = new CSpawner(SPAWNER_EARTH);
						}
						else if(typeofgolem == "air")
						{
							temp = new CSpawner(SPAWNER_AIR);
						}
						else if(typeofgolem == "fire")
						{
							temp = new CSpawner(SPAWNER_FIRE);
						}
						else if(typeofgolem == "water")
						{
							temp = new CSpawner(SPAWNER_WATER);
						}
						else if(typeofgolem == "ice")
						{
							temp = new CSpawner(SPAWNER_ICE);
						}
						else if(typeofgolem == "shadow")
						{
							temp = new CSpawner(SPAWNER_SHADOW);
						}
						else if(typeofgolem == "light")
						{
							temp = new CSpawner(SPAWNER_LIGHT);
						}
						else if(typeofgolem == "iron")
						{
							temp = new CSpawner(SPAWNER_IRON);
						}
						else if(typeofgolem == "lava")
						{
							temp = new CSpawner(SPAWNER_LAVA);
						}

						
						((CSpawner*)temp)->SetPosX((float)(posX * 32));
						((CSpawner*)temp)->SetPosY((float)(posY * 32));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );
						break;
					}
				}
				break;

				 // Power Glove
			case 1:
				{
					CPowerGloves* temp = new CPowerGloves();
					temp->SetObjectType(OBJ_POWERGLOVES) ;
					temp->SetPosX((float)(posX * 32));
					temp->SetPosY((float)(posY * 32));
					temp->SetIndexPosX(posX);
					temp->SetIndexPosY(posY);
					temp->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/PowerGlove.png" ));

					MObjectManager::GetInstance()->AddUnitIndexed( temp , posZ ) ;
				}
				break ;

				// Oil Can
			case 2:
				{
					COilCan* temp = new COilCan();
					temp->SetObjectType(OBJ_OILCAN) ;
					temp->SetPosX((float)(posX * 32));
					temp->SetPosY((float)(posY * 32));
					temp->SetIndexPosX(posX);
					temp->SetIndexPosY(posY);

					MObjectManager::GetInstance()->AddUnitIndexed( temp , posZ ) ;
				}
			}
			
			pObject = pObject->NextSiblingElement("TileObject");
		}
	}
	
	
	MEventSystem::GetInstance()->SendEvent("spawner.spawn");
	return true;
}
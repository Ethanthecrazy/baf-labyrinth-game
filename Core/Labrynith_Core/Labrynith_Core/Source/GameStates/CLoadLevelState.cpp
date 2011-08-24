
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
#include "../Object Manager/Units/Tiles/CMetal.h"
#include "../Object Manager/Units/Tiles/CElectricButton.h"
#include "../Object Manager/Units/Tiles/CElectricGenerator.h"
#include "../Object Manager/Units/Tiles/CHelpTile.h"
#include "../Object Manager/Units/Tiles/CLightEffected.h"

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
	cout << "Load Level State\n";
	totalobjects = 0;
	loadingat = 0;
	percentComplete = 0;
	lastPercent = 0;

	m_nBar_IMG = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/LoadingBar.png" );
	m_nGlow_IMG = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/glow.png" );

	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );


	cout << "Deleting current level...\n";	

	MObjectManager::GetInstance()->RemoveAllUnits();
	MMessageSystem::GetInstance()->ShutdownMessageSystem();
	MEventSystem::GetInstance()->ShutdownEventSystem();
	CAI_Handler::GetInstance()->ClearEntityList();
	CGamePlayState::GetInstance()->testVaribale = -1;

	cout << "...current level deleted\n";	

	cout << "Loading Level...\n";
	MMessageSystem::GetInstance()->InitMessageSystem( CGamePlayState::MessageProc );
	//if( !LoadLevel(CGamePlayState::GetInstance()->GetCurrentLevel()) )
	if( !LoadLevel( 4 ) )
	{
		cout << "...level loading failed.\n";
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
		return;
	}

	cout << "...Level Loaded\n";
	
	printf("Spawning objects...\n");

	CGame::GetInstance()->PopState();
}

bool CLoadLevelState::Input(void)
{
	return true;
}

void CLoadLevelState::Update(float fDT)
{

}

void CLoadLevelState::Render(void)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 298;
	rect.bottom = 30;

	RECT rect2;
	rect2.left = 125;
	rect2.top = 450;
	rect2.right = rect2.left + 625;
	rect2.bottom = rect2.top + 75;

	char percent[64] = { };

	
	CSGD_Direct3D::GetInstance()->Clear(0, 0, 0);

	
	CSGD_Direct3D::GetInstance()->SpriteBegin();

	sprintf_s(percent, "Loading the level...%d", percentComplete);
	string test = percent;
	test += "%";

	MetalText.Print((char*)test.c_str(), 315 / 2, 425, .5f);

	CSGD_TextureManager::GetInstance()->Draw( m_nBar_IMG, 214 / 2, 450, 2.0f, 2.0f, &rect );

	

	//CSGD_Direct3D::GetInstance()->DrawRect(rect2, 255, 255, 255);

	rect.top += 30;
	rect.bottom += 30;
	rect.right = (long)( 290 * percentComplete / 100.0f );

	CSGD_TextureManager::GetInstance()->Draw( m_nBar_IMG, 214 / 2, 450, 2.0f, 2.0f, &rect );

	//CSGD_TextureManager::GetInstance()->Draw( m_nGlow_IMG, 214 - 48 + rect.right * 2, 450 - 49 + 32, 1.5f, 1.5f, 0, 32.0f, 32.0f, 0, D3DCOLOR_ARGB( 255, 0, 100, 255 ) );
	//CSGD_TextureManager::GetInstance()->Draw( m_nGlow_IMG, 214 - 48 + rect.right * 2, 450 - 49 + 32, 1.5f, 1.5f, 0, 32.0f, 32.0f, D3DXToRadian( 45 ) );
	//CSGD_Direct3D::GetInstance()->DrawRect(rect, 255-percentComplete, percentComplete-100, 0 );

	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->Present();
}

void CLoadLevelState::Exit(void)
{
	cout << "LoadLevel -> ";
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
	Render();

	char filename[255] = {0};

	sprintf_s(filename, "resource/Levels/%d.xml", _level);

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

		swidth = pMapSize->FirstChildElement("Width")->GetText();
		sheight = pMapSize->FirstChildElement("Height")->GetText();
		sdepth = pMapSize->FirstChildElement("Depth")->GetText();

		width = atoi(swidth.c_str());
		height = atoi(sheight.c_str());
		depth = atoi(sdepth.c_str());
		
	}
	totalobjects = (float)width*height*depth;

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
					std::string prop = "";					

					TiXmlElement* pType = pTile->FirstChildElement("Type");
					if(pType)
					{
						TiXmlElement* pProp = pTile->FirstChildElement("Property");
						if(pProp)
						{	
							if(pProp->GetText())
								prop = pProp->GetText();
						}

						std::string sTypeX = "", sTypeY = "";

						sTypeX = pType->FirstChildElement("X")->GetText();
						sTypeY = pType->FirstChildElement("Y")->GetText();

						int TypeX, TypeY, theType;
						TypeX = atoi(sTypeX.c_str());
						TypeY = atoi(sTypeY.c_str());

						theType = TypeX + (TypeY * 5);
						
						switch(theType)
						{
						case -6: // -1, -1 wall
							{
								MObjectManager::GetInstance()->GetLayer( z ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, 0 );
							}
							break;

						case 0: // 0, 0 floor
							{
								MObjectManager::GetInstance()->GetLayer( z ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, 1 );
							}
							break;

						case 1: // 0, 1 button
							{
								CButton* temp = new CButton(prop.c_str());
								temp->SetLayerLocation(z);
								((CButton*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CButton*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CButton*)temp)->SetIndexPosX(x);
								((CButton*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							break;
							}
							
						case 2: // 0, 2 door
							{
								CDoor* temp = new CDoor(prop.c_str());
								temp->SetLayerLocation(z);
								((CDoor*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CDoor*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CDoor*)temp)->SetIndexPosX(x);
								((CDoor*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							break;
							}

						case 3: // 0, 3 exit tile
							{
								CExit* temp = new CExit();
								temp->SetLayerLocation(z);
								((CExit*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CExit*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CExit*)temp)->SetIndexPosX(x);
								((CExit*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break;
							
						case 4: // 0, 4 Wood tile
							{
								COil* temp = new COil(false);
								temp->SetLayerLocation(z);
								((COil*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((COil*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((COil*)temp)->SetIndexPosX(x);
								((COil*)temp)->SetIndexPosY(y);
								((CBaseObject*)(temp))->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/wood.png" ));
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );

								MObjectManager::GetInstance()->GetLayer( z ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, 1 );
							break;
							}
							
						case 5: // water tile
							{
								CWaterTile* temp = new CWaterTile(false);
								temp->SetLayerLocation(z);
								((CWaterTile*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CWaterTile*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CWaterTile*)temp)->SetIndexPosX(x);
								((CWaterTile*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break;
						case 6: // ice tile
							{
								CWaterTile* temp = new CWaterTile(true);
								temp->SetLayerLocation(z);
								((CWaterTile*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CWaterTile*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CWaterTile*)temp)->SetIndexPosX(x);
								((CWaterTile*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break;

						case 7: // pit tile
							{
								CPit* temp = new CPit();
								temp->SetLayerLocation(z);
								((CPit*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CPit*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CPit*)temp)->SetIndexPosX(x);
								((CPit*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break;
						
						case 8: // ramp tile
							{
								std::string info = "";
								std::string gotolevel = "";
								std::string posx = "", posy = "";
								int realgotolevel;
								int nposx = -1;
								int nposy = -1;
								unsigned int i = 0;
								for(i = 0; i < prop.length(); ++i)
								{
									if(prop[i] == '.')
										break;

									info += prop[i];
								}
								bool hitit = false;
								bool haspos = false;
								for(i; i < prop.length(); ++i)
								{
									if(prop[i] != '.' && !hitit)
										continue;

									hitit = true;
									if(prop[i] == '.')
										continue;
									
									if(prop[i] == ',')
									{
										haspos = true;
										++i;
										break;
									}
									gotolevel += prop[i];
								}
								realgotolevel = atoi(gotolevel.c_str());
								bool nextpos = false;
								for(i; i < prop.length(); ++i)
								{
									if(!haspos)
										break;

									if(nextpos)
									{
										posy += prop[i];
										continue;
									}

									if(prop[i] == ',')
									{
										nextpos = true;
										continue;
									}
									posx += prop[i];
								}
								if(haspos)
								{
									nposx = atoi(posx.c_str());
									nposy = atoi(posy.c_str());
								}

								CRamp* temp = new CRamp(info, realgotolevel);
								temp->SetLayerLocation(z);
								((CRamp*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CRamp*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CRamp*)temp)->SetIndexPosX(x);
								((CRamp*)temp)->SetIndexPosY(y);
								((CRamp*)temp)->SetLinkX(nposx);
								((CRamp*)temp)->SetLinkY(nposy);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break;					
						case 9: //	metal tile
							{
								CMetal* temp = new CMetal();
								temp->SetLayerLocation(z);
								((CMetal*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CMetal*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CMetal*)temp)->SetIndexPosX(x);
								((CMetal*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break ;
						case 10: //	electric button
							{
								CElectricButton* temp = new CElectricButton(prop.c_str());
								temp->SetLayerLocation(z);
								((CElectricButton*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CElectricButton*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CElectricButton*)temp)->SetIndexPosX(x);
								((CElectricButton*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break ;
						case 11: //	electric dynamo
							{
								CElectricGenerator* temp = new CElectricGenerator();
								temp->SetLayerLocation(z);
								((CElectricGenerator*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CElectricGenerator*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CElectricGenerator*)temp)->SetIndexPosX(x);
								((CElectricGenerator*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break ;
						case 12: // wall
							{
								MObjectManager::GetInstance()->GetLayer( z ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, 0 );
							}
							break;							
						case 13: // helper tile
							{	
								CHelpTile* temp = new CHelpTile(prop);
								temp->SetLayerLocation(z);
								((CHelpTile*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CHelpTile*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CHelpTile*)temp)->SetIndexPosX(x);
								((CHelpTile*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break;					
						case 14: // lighting effected tile
							{	
								CLightEffected* temp = new CLightEffected();
								temp->SetLayerLocation(z);
								((CLightEffected*)temp)->SetPosX((float)(x * TILE_WIDTH));
								((CLightEffected*)temp)->SetPosY((float)(y * TILE_HEIGHT));
								((CLightEffected*)temp)->SetIndexPosX(x);
								((CLightEffected*)temp)->SetIndexPosY(y);
								MObjectManager::GetInstance()->AddUnitIndexed( temp, z );
							}
							break;
						}
						
						++loadingat;

						lastPercent = percentComplete;
						percentComplete = (int)((loadingat / totalobjects)*100);

						if(lastPercent != percentComplete)
							Render();
						// 0 = pit
						// 1 & 2 = ground
						//MObjectManager::GetInstance()->GetLayer( z ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, theType );
					}

						
					++y;
					pTile = pTile->NextSiblingElement("Tile");
				}
				
				
				++x;
				pArray = pArray->NextSiblingElement("ArrayOfTile");
			}

			++z;
			pArrayofArray = pArrayofArray->NextSiblingElement("ArrayOfArrayOfTile");
		}

		CGamePlayState::GetInstance()->SetNumLevelFloors(z-1);
	}
	cout << "Tiles loaded\n";


	TiXmlElement* pObjects = pRoot->FirstChildElement("Objects");	
	if(pObjects)
	{
		TiXmlElement* pObject = pObjects->FirstChildElement("TileObject");
		while(pObject)
		{			
			int TypeX, TypeY, theType;
			int posX, posY, posZ;
			std::string prop = "";

			TiXmlElement* pType = pObject->FirstChildElement("Type");
			if(pType)
			{
				std::string sTypeX = "", sTypeY = "";
				
				sTypeX = pType->FirstChildElement("X")->GetText();
				sTypeY = pType->FirstChildElement("Y")->GetText();

				TypeX = atoi(sTypeX.c_str());
				TypeY = atoi(sTypeY.c_str());

				theType = TypeX + (TypeY * 5); //Type.X + (Type.Y * tileSetSize.Width)
			}
			
			TiXmlElement* pPos = pObject->FirstChildElement("Position");
			if(pPos)
			{			
				std::string sposX, sposY, sposZ;

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
				if(pProp->GetText())
					prop = pProp->GetText();
			}
			
			switch(theType)
			{
			case 0:				
				{
					std::string typeofspawner;
					for(unsigned int i = 0; i < prop.length(); ++i)
					{
						if(prop[i] == '.')
							break;

						typeofspawner += prop[i];
					}

					if(typeofspawner == "player")
					{
						// change this code to a spawner of Player type
						CSpawner* temp = new CSpawner(SPAWNER_PLAYER);
						temp->SetLayerLocation(posZ);
						((CSpawner*)temp)->SetPosX((float)(posX * TILE_WIDTH));
						((CSpawner*)temp)->SetPosY((float)(posY * TILE_HEIGHT));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );
						break;
					}
					else if(typeofspawner == "lightorb")
					{						
						CSpawner* temp = new CSpawner(SPAWNER_LIGHTORB);
						temp->SetLayerLocation(posZ);
						((CSpawner*)temp)->SetPosX((float)(posX * TILE_WIDTH));
						((CSpawner*)temp)->SetPosY((float)(posY * TILE_HEIGHT));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );
					}
					else if(typeofspawner == "attractor")
					{
						string typeofattractor;
						bool beginreading = false;
						for(unsigned int i = 0; i < prop.length(); ++i)
						{
							if(beginreading)
								typeofattractor += prop[i];

							if(prop[i] == '.')
								beginreading = true;
						}
						
						CSpawner* temp;
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

						
						temp->SetLayerLocation(posZ);
						((CSpawner*)temp)->SetPosX((float)(posX * TILE_WIDTH));
						((CSpawner*)temp)->SetPosY((float)(posY * TILE_HEIGHT));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );

						break;
					}
					else if(typeofspawner == "golem")
					{
						string typeofgolem;
						bool beginreading = false;
						for(unsigned int i = 0; i < prop.length(); ++i)
						{
							if(beginreading)
								typeofgolem += prop[i];

							if(prop[i] == '.')
								beginreading = true;
						}
						
						CSpawner* temp;

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
						else
						{
							temp = NULL;
						}

						if( temp )
						{
							temp->SetLayerLocation(posZ);
							((CSpawner*)temp)->SetPosX((float)(posX * TILE_WIDTH));
							((CSpawner*)temp)->SetPosY((float)(posY * TILE_HEIGHT));
							((CSpawner*)temp)->SetIndexPosX(posX);
							((CSpawner*)temp)->SetIndexPosY(posY);

							MObjectManager::GetInstance()->AddUnitIndexed( temp, posZ );
						}
						break;
					}
				}
				break;

				 // Power Glove
			case 1:
				{
					CPowerGloves* temp = new CPowerGloves();
					temp->SetLayerLocation(posZ);
					temp->SetObjectType(OBJ_POWERGLOVES) ;
					temp->SetPosX((float)(posX * TILE_WIDTH));
					temp->SetPosY((float)(posY * TILE_HEIGHT));
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
					temp->SetLayerLocation(posZ);
					temp->SetObjectType(OBJ_OILCAN) ;
					temp->SetPosX((float)(posX * TILE_WIDTH));
					temp->SetPosY((float)(posY * TILE_HEIGHT));
					temp->SetIndexPosX(posX);
					temp->SetIndexPosY(posY);

					MObjectManager::GetInstance()->AddUnitIndexed( temp , posZ ) ;
				}
			}
			
			pObject = pObject->NextSiblingElement("TileObject");
		}
	}
	
	cout << "Objects created\n";
	
	MEventSystem::GetInstance()->SendEvent("spawner.spawn");

	cout << "Spawn event sent\n";
	return true;
}
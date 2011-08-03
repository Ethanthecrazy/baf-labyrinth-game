
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

#include "../Object Manager/Units/CBaseGolem.h"

#include "../TinyXML/tinyxml.h"

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
	cout << "Loading Level...\n";

	/*for( int x = 0; x < MObjectManager::GetInstance()->GetLayer( 1 ).GetLayerWidth(); ++x )
	{
		for( int y = 0; y < MObjectManager::GetInstance()->GetLayer( 1 ).GetLayerHeight(); ++y )
		{
			MObjectManager::GetInstance()->GetLayer( 1 ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, rand() % 3 );
		}
	}*/
	LoadLevel(CGamePlayState::GetInstance()->GetCurrentLevel());

	cout << "...Level Loaded\n";

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
	
	TiXmlElement* pMapSize = pRoot->FirstChildElement("MapSize");	
	if(pMapSize)
	{
		string swidth, sheight, sdepth;

		swidth = pMapSize->FirstChildElement("Width")->GetText();
		sheight = pMapSize->FirstChildElement("Height")->GetText();
		sdepth = pMapSize->FirstChildElement("Depth")->GetText();

		int width, height, depth;
		width = atoi(swidth.c_str());
		height = atoi(sheight.c_str());
		depth = atoi(sdepth.c_str());
		
		MObjectManager::GetInstance()->ResizeLayer( 1, width, height );
	}

	TiXmlElement* pTiles = pRoot->FirstChildElement("Tiles");	
	if(pTiles)
	{
		TiXmlElement* pArrayofArray = pTiles->FirstChildElement("ArrayOfArrayOfTile");
		if(pArrayofArray)
		{
			TiXmlElement* pArray = pArrayofArray->FirstChildElement("ArrayOfTile");
			int x = 0;
			while(pArray)
			{				
				TiXmlElement* pTile = pArray->FirstChildElement("Tile");
				int y = 0;
				while(pTile)
				{			
					TiXmlElement* pType = pTile->FirstChildElement("Type");
					if(pType)
					{
						string sTypeX, sTypeY;

						sTypeX = pType->FirstChildElement("X")->GetText();
						sTypeY = pType->FirstChildElement("Y")->GetText();

						int TypeX, TypeY, theType;
						TypeX = atoi(sTypeX.c_str());
						TypeY = atoi(sTypeY.c_str());

						if(TypeX == -1 && TypeY == -1)
							theType = 0;
						else if(TypeX == 0 && TypeY == 0)
							theType = 1;
						else if(TypeX == 1 && TypeY == 0)
							theType = 0;
						else
							theType = -1;

						// 0 = pit
						// 1 & 2 = ground
						MObjectManager::GetInstance()->GetLayer( 1 ).GetFlake( OBJECT_TILE ).SetInfoAtIndex( x, y, theType );
					}

						
					++y;
					pTile = pTile->NextSiblingElement("Tile");
				}

				
				++x;
				pArray = pArray->NextSiblingElement("ArrayOfTile");
			}
		}
	}


	TiXmlElement* pObjects = pRoot->FirstChildElement("Objects");	
	if(pObjects)
	{
		TiXmlElement* pObject = pObjects->FirstChildElement("TileObject");
		while(pObject)
		{			
			int TypeX, TypeY, theType;
			int posX, posY;
			string prop = "";

			TiXmlElement* pType = pObject->FirstChildElement("Type");
			if(pType)
			{
				string sTypeX, sTypeY;

				sTypeX = pType->FirstChildElement("X")->GetText();
				sTypeY = pType->FirstChildElement("Y")->GetText();

				TypeX = atoi(sTypeX.c_str());
				TypeY = atoi(sTypeY.c_str());

				//if(TypeX == -1 && TypeY == -1)
				//	theType = -1;
				//else if(TypeX == 0 && TypeY == 0) // 0 == Golem
				//	theType = 0;
				//else if(TypeX == 1 && TypeY == 0) // 1 == Button
				//	theType = 1;

				theType = TypeX + (TypeY * 5); //Type.X + (Type.Y * tileSetSize.Width)
			}
			
			TiXmlElement* pPos = pObject->FirstChildElement("Position");
			if(pPos)
			{			
				string sposX, sposY;

				sposX = pPos->FirstChildElement("X")->GetText();
				sposY = pPos->FirstChildElement("Y")->GetText();

				posX = atoi(sposX.c_str());
				posY = atoi(sposY.c_str());
		
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
						MMessageSystem::GetInstance()->SendMsg( new msgCreatePlayer( posX, posY ) );
						break;
					}

					if(typeofspawner == "attractor")
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
						
						// change this code to a spawner of Attractor X type

						break;
					}

					if(typeofspawner == "golem")
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

						// change this code to a spawner of Golem X type
						IUnitInterface* temp = new CBaseGolem();

						if(typeofgolem == "earth")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/StoneGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(EARTH_GOLEM);
						}
						else if(typeofgolem == "air")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/AirGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(AIR_GOLEM);
						}
						else if(typeofgolem == "fire")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/FireGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(FIRE_GOLEM);
						}
						else if(typeofgolem == "water")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/WaterGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(WATER_GOLEM);
						}
						else if(typeofgolem == "ice")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IceGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(ICE_GOLEM);
						}
						else if(typeofgolem == "shadow")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/ShadowGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(SHADOW_GOLEM);
						}
						else if(typeofgolem == "light")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/LightGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(LIGHT_GOLEM);
						}
						else if(typeofgolem == "iron")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IronGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(IRON_GOLEM);
						}
						else if(typeofgolem == "lava")
						{
							((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/LavaGolem.png" ));
							((CBaseGolem*)(temp))->SetGolemType(LAVA_GOLEM);
						}

						//Load basic movement animations
						((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
						((CBaseGolem*)(temp))->SetPosX( (float)(posX*32) );
						((CBaseGolem*)(temp))->SetPosY( (float)(posY*32) );
						((CBaseGolem*)(temp))->SetIndexPosX( posX );
						((CBaseGolem*)(temp))->SetIndexPosY( posY );

						MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
						break;
					}
				}
				break;
			}
			
			pObject = pObject->NextSiblingElement("TileObject");
		}
	}

	return true;
}
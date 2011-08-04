
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
#include "../Object Manager/Units/Tiles/CButton.h"
#include "../Object Manager/Units/Tiles/CDoor.h"

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
					string prop = "";

					TiXmlElement* pProp = pTile->FirstChildElement("Property");
					if(pProp)
					{			
						//prop = pProp->GetText();
					}

					TiXmlElement* pType = pTile->FirstChildElement("Type");
					if(pType)
					{
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
							MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );

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
							MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );

							theType = 1;
							break;
							}
						case 3: // exit tile
							{
							theType = 1;
							}
							break;
						}

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
						IUnitInterface* temp = new CSpawner(SPAWNER_PLAYER);
						((CSpawner*)temp)->SetPosX((float)(posX * 32));
						((CSpawner*)temp)->SetPosY((float)(posY * 32));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
						break;
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
							temp = new CSpawner(SPAWNER_ATTRACTOR_EARTH);
						else if(typeofattractor == "air")
							temp = new CSpawner(SPAWNER_ATTRACTOR_AIR);
						else if(typeofattractor == "fire")
							temp = new CSpawner(SPAWNER_ATTRACTOR_FIRE);
						else if(typeofattractor == "water")
							temp = new CSpawner(SPAWNER_ATTRACTOR_WATER);
						else if(typeofattractor == "ice")
							temp = new CSpawner(SPAWNER_ATTRACTOR_ICE);
						else if(typeofattractor == "shadow")
							temp = new CSpawner(SPAWNER_ATTRACTOR_SHADOW);
						else if(typeofattractor == "light")
							temp = new CSpawner(SPAWNER_ATTRACTOR_LIGHT);
						else if(typeofattractor == "iron")
							temp = new CSpawner(SPAWNER_ATTRACTOR_IRON);
						else if(typeofattractor == "lava")
							temp = new CSpawner(SPAWNER_ATTRACTOR_LAVA);

						
						((CSpawner*)temp)->SetPosX((float)(posX * 32));
						((CSpawner*)temp)->SetPosY((float)(posY * 32));
						((CSpawner*)temp)->SetIndexPosX(posX);
						((CSpawner*)temp)->SetIndexPosY(posY);

						MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );

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
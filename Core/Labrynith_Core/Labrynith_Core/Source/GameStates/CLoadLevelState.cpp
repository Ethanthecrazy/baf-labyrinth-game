
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
	return true;
}
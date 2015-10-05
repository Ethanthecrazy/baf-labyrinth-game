#include <iostream>
#include <fstream>
using namespace std;
#include "CBitFont.h"
#include "TinyXML\tinyxml.h"
#include "Wrappers\CSGD_TextureManager.h"


void CBitFont::Initialize( int _Image, char _StartChar, int _CharWidth, int _CharHeight, int _Columns,
	std::string filename)
{
	m_nImageID = _Image;
	m_cStart = _StartChar;

	dimensions.x = _CharWidth;
	dimensions.y = _CharHeight;

	m_nColumns = _Columns;
	LoadCharInfo(filename);
}
void CBitFont::LoadCharInfo(std::string filename)
{
	TiXmlDocument doc;
	if( doc.LoadFile(filename.c_str()) == false)
	{
		cout << "Failed to load CharInfo" << endl;
		return;
	}

	TiXmlElement* pRoot = doc.RootElement();
	if(!pRoot)
		return;

	TiXmlElement* pChar = pRoot->FirstChildElement("Char");
	TiXmlElement* pData;
	char szStartChar = m_cStart;
	while(pChar)
	{
		int nOffset, nWidth;
		//Width
		pData = pChar->FirstChildElement("Width");
		pData->Attribute("width", &nWidth);
		//Offset
		pData = pChar->FirstChildElement("Offset");
		pData->Attribute("offset", &nOffset);

		CharInfo.push_back(tCharInfo(szStartChar, nWidth, nOffset));
		//BUG this can cause problems
		szStartChar += 1;
		pChar = pChar->NextSiblingElement("Char");	
	}
}
void CBitFont::Print( std::string _ToPrint, int _PosX, int _PosY, float _scale )
{
	int OrgX = _PosX; 

	for( unsigned int i  = 0; i < _ToPrint.size(); ++i )
	{
		char proc = _ToPrint[i];
		int index = proc - m_cStart;
		if(index == -22)
		{
			_PosX = OrgX;
			_PosY += (int)(dimensions.y * _scale);
			continue;
		}
			//index = 10;
		//Dont try to print any invalid characters
		if(index >= (signed int)CharInfo.size() || index < 0)
			continue;

			if( proc == ' ' )
			{
				//_PosX += (int)(( dimensions.x - 8 )* _scale);
				_PosX += (int)(( dimensions.x / 2 )* _scale);
				continue;
			}
			else if( (char*)_ToPrint[i] == "" )
			{
				_PosX = OrgX;
				_PosY += (int)(dimensions.y * _scale);
			}
		//Update drawrect based on CharInfo
		RECT rlock = CellAlgorythm(index);
		rlock.left += CharInfo[index].m_nOffset;
		rlock.right = (rlock.left + CharInfo[index].m_nWidth);
		//Draw that section to the screen
		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, _PosX, _PosY, _scale, _scale, &rlock );
		//Update drawposition based on CharInfo
		_PosX += (int)( CharInfo[index].m_nWidth * _scale );
	}
}

RECT CBitFont::CellAlgorythm( int _index )
{
	RECT ToReturn;

	ToReturn.left = (_index % m_nColumns) * dimensions.x;
	ToReturn.top = (_index / m_nColumns) * dimensions.y;
	ToReturn.right = ToReturn.left + dimensions.x;
	ToReturn.bottom = ToReturn.top + dimensions.y;

	return ToReturn;
}
#include <iostream>
#include <fstream>
using namespace std;
#include "CBitFont.h"
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
	//Text - 1st line
	const int size = 50;
	char text[size];
	int nOffset, nWidth;
	//Name
	char letter[1];

	ifstream in; 
	in.open(filename); 
	if (!in.is_open())
	{
		return; 
	}
	
	//Get the Text - 1st line
	in.getline(text, size, '\n');

	//Continue reading until there's no more to read
	while (!in.eof())
	{
		if (in.good())
		{		
			//If we reach the end of file leave
			if (in.eof())
				break;

			//Get the Char
			in.getline(letter, 1, '\t');

			//Get the height & offset
			in >> nWidth >> nOffset; 
			in.ignore(INT_MAX, '\n');

			CharInfo.push_back(tCharInfo(letter[0], nWidth, nOffset));
		}
		else
			break;
	}
	in.close();
}
void CBitFont::Print( char* _ToPrint, int _PosX, int _PosY, float _scale )
{
	int OrgX = _PosX; 

	for( unsigned int i  = 0; i < strlen( _ToPrint ); ++i )
	{
		char proc = _ToPrint[i];

			if( proc == ' ' )
			{
				_PosX += (int)(( dimensions.x - 8 )* _scale);
				continue;
			}
			else if( proc == '\n' )
			{
				_PosX = OrgX;
				_PosY += (int)(dimensions.y * _scale);
			}

		int id = proc - m_cStart;

		RECT rlock = CellAlgorythm( id );

		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, _PosX, _PosY, _scale, _scale, &rlock );

		_PosX += (int)( ( dimensions.x - 8 ) * _scale );
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
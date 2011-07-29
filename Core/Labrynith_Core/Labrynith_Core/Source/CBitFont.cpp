#include "CBitFont.h"
#include "Wrappers\CSGD_TextureManager.h"


void CBitFont::Initialize( int _Image, char _StartChar, int _CharWidth, int _CharHeight, int _Columns )
{
	m_nImageID = _Image;
	m_cStart = _StartChar;

	dimensions.x = _CharWidth;
	dimensions.y = _CharHeight;

	m_nColumns = _Columns;
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
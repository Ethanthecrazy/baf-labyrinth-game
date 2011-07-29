#ifndef CBITFONT_H_
#define CBITFONT_H_

#include <Windows.h>

class CBitFont
{
	int m_nImageID;
	char m_cStart;

	POINT dimensions;
	int m_nColumns;


public:

	void Initialize( int _Image, char _StartChar, int _CharWidth, int _CharHeight, int _Columns );

	void Print( char* _ToPrint, int _PosX, int _PosY, float _scale = 1.0f );

	RECT CellAlgorythm( int _index );
};

#endif
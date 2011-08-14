#ifndef CBITFONT_H_
#define CBITFONT_H_

#include <Windows.h>
#include <vector>
#include <string>

class CBitFont
{
	struct tCharInfo
	{
		char m_szChar;
		int m_nWidth;
		int m_nOffset;
		tCharInfo()
		{
			m_szChar = ' ';
			m_nWidth = 0;
			m_nOffset = 0;
		}
		tCharInfo(char szChar, int nWidth, int nOffset)
		{
			m_szChar = szChar;
			m_nWidth = nWidth;
			m_nOffset = nOffset;
		}
	};

	std::vector<tCharInfo> CharInfo;
	int m_nImageID;
	char m_cStart;

	POINT dimensions;
	int m_nColumns;

	void LoadCharInfo(std::string filename);
public:

	void Initialize( int _Image, char _StartChar, int _CharWidth, int _CharHeight, int _Columns,
	std::string filename);

	void Print( char* _ToPrint, int _PosX, int _PosY, float _scale = 1.0f );

	RECT CellAlgorythm( int _index );
};

#endif
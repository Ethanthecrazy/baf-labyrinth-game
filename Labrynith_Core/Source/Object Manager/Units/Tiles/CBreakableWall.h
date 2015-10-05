
#ifndef CBREAKABLEWALL_H
#define CBREAKABLEWALL_H

#include "../CBaseObject.h"

class CBreakableWall : public CBaseObject
{
private:
	bool m_bBroken ;
	int m_nUnBrokenImageID ;
	int m_nBrokenImageID ;
public:
	CBreakableWall() ;
	~CBreakableWall() ;
	
	bool GetBroken( void ) { return m_bBroken ; } ;

	void SetBroken( bool broken ) { m_bBroken ; } ;
} ;


#endif
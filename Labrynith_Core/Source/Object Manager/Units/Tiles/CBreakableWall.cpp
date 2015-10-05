
#include "CBreakableWall.h"

CBreakableWall::CBreakableWall() 
{
	CBaseObject::CBaseObject() ;
	m_bBroken = false ;
	m_nType = OBJ_BREAKABLEWALL ;
	m_nUnBrokenImageID  = -1 ;
	m_nBrokenImageID = -1 ;
}

CBreakableWall::~CBreakableWall()
{
	CBaseObject::~CBaseObject() ;
}
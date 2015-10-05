#include "CAttractor.h"

CAttractor::CAttractor(int elemType)
{
	CBaseObject::CBaseObject();
	m_nType = OBJ_ATTRACTOR ;
	m_nElemType = elemType ;
}

CAttractor::CAttractor(void)
{
	CBaseObject::CBaseObject() ;
	m_nType = OBJ_ATTRACTOR ;
}

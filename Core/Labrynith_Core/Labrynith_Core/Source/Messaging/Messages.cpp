#include "Messages.h"

msgCreateEntity::msgCreateEntity( int _PosX, int _PosY ) : CBaseMessage( MSG_CREATE_ENTITY )
{
	m_nPosX = _PosX;
	m_nPosY = _PosY;
}
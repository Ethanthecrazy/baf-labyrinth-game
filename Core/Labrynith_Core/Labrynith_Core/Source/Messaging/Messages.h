#ifndef MESSAGES_H
#define MESSAGES_H

enum eMsgTypes { MSG_NULL = 0, MSG_CREATE_ENTITY, MSG_MAX };

class CBaseMessage
{
	int m_msgID;

public:

	CBaseMessage( int msgID )
	{
		m_msgID = msgID;
	}

	virtual ~CBaseMessage( void ) {}

	int GetMsgID( void ) { return m_msgID; }

};


class msgCreateEntity : public CBaseMessage
{
	int m_nPosX;
	int m_nPosY;

public:

	msgCreateEntity( int _PosX, int _PosY );
	int GetX( void ) { return m_nPosX; }
	int GetY( void ) { return m_nPosY; }

};


#endif
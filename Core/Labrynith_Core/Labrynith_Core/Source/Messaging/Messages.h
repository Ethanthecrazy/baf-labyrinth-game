#ifndef MESSAGES_H
#define MESSAGES_H

enum eMsgTypes { MSG_NULL = 0, MSG_CREATE_ENTITY, MSG_CREATE_PLAYER,
	             MSG_REMOVE_UNIT, MSG_TRANSFER_LIGHT, MSG_PLACE_OBJECT,
				 MSG_PICKUP_OBJECT, MSG_CHANGE_GOLEM_TYPE, MSG_MAX };

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

class msgCreatePlayer : public CBaseMessage
{
	int m_nPosX;
	int m_nPosY;

public:

	msgCreatePlayer( int _PosX, int _PosY );
	int GetX( void ) { return m_nPosX; }
	int GetY( void ) { return m_nPosY; }

};

class msgRemoveUnit : public CBaseMessage
{
	int m_pToRemove;

public:

	msgRemoveUnit( int _toRemove );
	int GetTarget( void ) { return m_pToRemove; }

};

class msgTransferLight : public CBaseMessage
{
	int OneX;
	int	OneY;
	int TwoX;
	int TwoY;
	int TransferValue;
	void* Flake;

public:

	msgTransferLight( int _OneX, int _OneY, int _TwoX, int _TwoY, int _TransferValue, void* _Flake );
	int GetOneX( void ) { return OneX; }
	int GetOneY( void ) { return OneY; }
	int GetTwoX( void ) { return TwoX; }
	int GetTwoY( void ) { return TwoY; }
	int GetTransferValue(void) { return TransferValue; }
	void* GetFlake( void ) { return Flake; } 

};


class CBaseObject;
class msgPlaceObject : public CBaseMessage
{
	int m_nTileXPos ;
	int m_nTileYPos ;
public:
	msgPlaceObject( int tileXPos , int tileYPos ) ;
	int GetTileXPos(void) { return m_nTileXPos ; } ;
	int GetTileYPos(void) { return m_nTileYPos ; } ;
};

class msgPickUpObject : public CBaseMessage
{
	CBaseObject* m_pObject;

public:
	msgPickUpObject( CBaseObject* object ) ;
	CBaseObject* GetMsgObject(void) { return m_pObject ; } ;
} ;

class CBaseGolem;
class msgChangeGolemType : public CBaseMessage
{
	CBaseGolem* m_pGolem;
	int m_nGolemType;
public:
	msgChangeGolemType(CBaseGolem* pGolem, int nGolemType);
	CBaseGolem* GetGolem() { return m_pGolem; };
	int GetGolemType() { return m_nGolemType; };
};

#endif
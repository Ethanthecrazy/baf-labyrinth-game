#ifndef MESSAGES_H
#define MESSAGES_H

enum eMsgTypes { MSG_NULL = 0, MSG_CREATE_ENTITY, MSG_CREATE_PLAYER,
	             MSG_REMOVE_UNIT, MSG_TRANSFER_LIGHT, MSG_DRAIN_LIGHT, MSG_PLACE_OBJECT,
				 MSG_PICKUP_OBJECT, MSG_CHANGE_GOLEM_TYPE, MSG_MOVE_ENTITY_FLOOR,
				 MSG_DELETEME, MSG_REMOVE_GOLEM_COMBINED, DELETE_IDHOLDER, MSG_DRAW_GENERATORTOP, MSG_END_GAME, MSG_MAX };

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
	int m_nPosZ;

public:

	msgCreatePlayer( int _PosX, int _PosY, int _PosZ );
	int GetX( void ) { return m_nPosX; }
	int GetY( void ) { return m_nPosY; }
	int GetZ( void ) { return m_nPosZ; }
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

class msgDrainLight : public CBaseMessage
{
	int OneX;
	int	OneY;
	int TransferValue;
	void* Flake;

public:

	msgDrainLight( int _OneX, int _OneY, int _DrainValue, void* _Flake );
	int GetOneX( void ) { return OneX; }
	int GetOneY( void ) { return OneY; }
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

class CBaseEntity;
class msgMoveEntityFloor : public CBaseMessage
{
	CBaseEntity* m_pEntity;
	int m_nFloor;
	int m_nPosX;
	int m_nPosY;
public:
	msgMoveEntityFloor( CBaseEntity* pEntity, int nFloor, int nPosX, int nPosY  );
	CBaseEntity* GetEntity() { return m_pEntity; }
	int GetFloor() { return m_nFloor; }
	int GetPosX() { return m_nPosX; }
	int GetPosY() { return m_nPosY; }
};

class CBaseGolem;
class msgChangeGolemType : public CBaseMessage
{
	CBaseGolem* m_pGolem;
	int m_nGolemType;
	int* m_nNewID;
public:
	msgChangeGolemType(CBaseGolem* pGolem, int nGolemType, int* nNewID = 0);
	CBaseGolem* GetGolem() { return m_pGolem; };
	int GetGolemType() { return m_nGolemType; };
	int* GetNewID() { return m_nNewID; };
	void SetNewID(int newNewID) { *m_nNewID = newNewID; };
};

class msgRemoveGolemCombined : public CBaseMessage
{
	int m_nOldID;
	int* m_pGolemChanging;
public:
	msgRemoveGolemCombined(int nOldID, int* pGolemChanging);
	
	int GetOldID() { return m_nOldID; };
	int* GetGolemChanging() { return m_pGolemChanging; };
};

class IUnitInterface;
class msgDeleteMe : public CBaseMessage
{
	IUnitInterface* m_ptoDelete;
public:
	msgDeleteMe(IUnitInterface* ptoDelete);
	IUnitInterface* GetPointer() { return m_ptoDelete; };
};


struct IDHolder;
class msgDeletIDHolder : public CBaseMessage
{
	IDHolder* m_ptoDelete;
public:
	msgDeletIDHolder(IDHolder* ptoDelete);
	IDHolder* GetPointer() { return m_ptoDelete; };
};


class msgDrawGeneratorTop : public CBaseMessage
{
	int m_nTileXPos;
	int m_nTileYPos;
	int m_nIMG_Top;
	int brightness;

public:

	msgDrawGeneratorTop( int tileXPos , int tileYPos, int m_nImageID, int newBright );
	int GetTileXPos(void) { return m_nTileXPos ; }
	int GetTileYPos(void) { return m_nTileYPos ; }
	int GetTop(void) { return m_nIMG_Top; }
	int GetBright(void) { return brightness; } 
};


class msgEndGame : public CBaseMessage
{

public:

	msgEndGame();
};

#endif
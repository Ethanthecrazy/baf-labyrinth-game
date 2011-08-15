#include "Messages.h"
#include "../Object Manager/Units/CBaseObject.h"
#include "../Object Manager/Units/CBaseEntity.h"

msgCreateEntity::msgCreateEntity( int _PosX, int _PosY ) : CBaseMessage( MSG_CREATE_ENTITY )
{
	m_nPosX = _PosX;
	m_nPosY = _PosY;
}

msgCreatePlayer::msgCreatePlayer( int _PosX, int _PosY, int _PosZ ) : CBaseMessage( MSG_CREATE_PLAYER )
{
	m_nPosX = _PosX;
	m_nPosY = _PosY;
	m_nPosZ = _PosZ;
}

msgRemoveUnit::msgRemoveUnit( int _toRemove ) : CBaseMessage( MSG_REMOVE_UNIT )
{
	m_pToRemove = _toRemove;
}


msgTransferLight::msgTransferLight( int _OneX, int _OneY, int _TwoX, int _TwoY, int _TransferValue, void* _Flake ) : CBaseMessage( MSG_TRANSFER_LIGHT )
{
	OneX = _OneX;
	OneY = _OneY;
	TwoX = _TwoX;
	TwoY = _TwoY;
	TransferValue = _TransferValue;
	Flake = _Flake;
}

msgDrainLight::msgDrainLight( int _OneX, int _OneY, int _DrainValue, void* _Flake ) : CBaseMessage( MSG_DRAIN_LIGHT )
{
	OneX = _OneX;
	OneY = _OneY;
	TransferValue = _DrainValue;
	Flake = _Flake;
}

msgPlaceObject::msgPlaceObject( int tileXPos , int tileYPos ) : CBaseMessage( MSG_PLACE_OBJECT )
{
	m_nTileXPos = tileXPos ;
	m_nTileYPos = tileYPos ;
}

msgPickUpObject::msgPickUpObject( CBaseObject* object ) : CBaseMessage( MSG_PICKUP_OBJECT )
{
	m_pObject = object ;
}

msgMoveEntityFloor::msgMoveEntityFloor( CBaseEntity* pEntity, int nFloor ) : CBaseMessage( MSG_MOVE_ENTITY_FLOOR )
{
	m_pEntity = pEntity;
	m_nFloor = nFloor;
}

msgChangeGolemType::msgChangeGolemType(CBaseGolem* pGolem, int nGolemType)
	: CBaseMessage( MSG_CHANGE_GOLEM_TYPE )
{
	m_pGolem = pGolem;
	m_nGolemType = nGolemType;
}

msgDeleteMe::msgDeleteMe(IUnitInterface* ptoDelete)
	: CBaseMessage( MSG_DELETEME )
{
	m_ptoDelete = ptoDelete;
}

#include "Messages.h"
#include "../Object Manager/Units/CBaseObject.h"

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

msgPlaceObject::msgPlaceObject( int tileXPos , int tileYPos ) : CBaseMessage( MSG_PLACE_OBJECT )
{
	m_nTileXPos = tileXPos ;
	m_nTileYPos = tileYPos ;
}

msgPickUpObject::msgPickUpObject( CBaseObject* object ) : CBaseMessage( MSG_PICKUP_OBJECT )
{
	m_pObject = object ;
}

msgChangeGolemType::msgChangeGolemType(CBaseGolem* pGolem, int nGolemType)
	: CBaseMessage( MSG_CHANGE_GOLEM_TYPE )
{
	m_pGolem = pGolem;
	m_nGolemType = nGolemType;
}

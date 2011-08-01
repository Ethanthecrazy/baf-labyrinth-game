#include "Messages.h"

msgCreateEntity::msgCreateEntity( int _PosX, int _PosY ) : CBaseMessage( MSG_CREATE_ENTITY )
{
	m_nPosX = _PosX;
	m_nPosY = _PosY;
}

msgCreatePlayer::msgCreatePlayer( int _PosX, int _PosY ) : CBaseMessage( MSG_CREATE_PLAYER )
{
	m_nPosX = _PosX;
	m_nPosY = _PosY;
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
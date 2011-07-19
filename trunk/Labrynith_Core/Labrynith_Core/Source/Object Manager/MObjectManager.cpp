#include "MObjectManager.h"

MObjectManager* MObjectManager::GetInstance( void )
{
	static MObjectManager* om = new MObjectManager();
	return om;
}

void MObjectManager::DeleteInstance(void)
{
	delete this;
}

int MObjectManager::AddUnit( IFlakeInterface* _toAdd, int _layer )
{
	_toAdd->m_nIdentificationNumber += _layer * 100000;
	int tempnumber = m_vLayers[_layer].AddUnit( _toAdd );
	LayerIndex.InsertEntry( tempnumber, _layer );
	return tempnumber;
}

bool MObjectManager::RemoveUnit( int _Ident )
{
	m_vLayers[ LayerIndex.ConvertTrueValue( _Ident ) ].RemoveUnit( _Ident );
}

void MObjectManager::RemoveAllUnits( void )
{

}

void MObjectManager::Update( float fDT )
{

}

void MObjectManager::Render( int CameraX, int CameraY )
{

}

void MObjectManager::CheckCollisions( void )
{

}

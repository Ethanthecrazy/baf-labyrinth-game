#include "MObjectManager.h"
#include <iostream>


MObjectManager::MObjectManager() : m_nSize( 0 )
{
	MEventSystem::GetInstance()->RegisterClient( "Add.Object", this );
	for( int i = 0; i < 9; ++i )
		m_vLayers.push_back( MLayer( i ) );
}

MObjectManager::~MObjectManager()
{

}

MObjectManager* MObjectManager::GetInstance( void )
{
	static MObjectManager* om = new MObjectManager();
	return om;
}

void MObjectManager::DeleteInstance(void)
{
	delete this;
}

int MObjectManager::AddUnit( IUnitInterface* _toAdd, int _layer )
{
	m_nSize += 1;
	_toAdd->m_nIdentificationNumber = _layer * 100000;
	int tempnumber = m_vLayers[_layer].AddUnit( _toAdd );
	LayerIndex.InsertEntry( tempnumber, _layer );
	cout << "\tObjectManager: Adding Unit - Id " << tempnumber << "\n";
	return tempnumber;
}

int MObjectManager::AddUnitIndexed( IUnitInterface* _toAdd, int _layer )
{
	m_nSize += 1;
	_toAdd->m_nIdentificationNumber = _layer * 100000;
	int tempnumber = m_vLayers[_layer].AddUnitIndexed( _toAdd );
	LayerIndex.InsertEntry( tempnumber, _layer );
	cout << "\tObjectManager: Adding Unit - Id " << tempnumber << "\n";
	return tempnumber;
}

bool MObjectManager::RemoveUnit( int _Ident )
{
	if( LayerIndex.ConvertTrueValue( _Ident ) < 0 )
		return false;

	m_nSize -= 1;
	m_vLayers[ LayerIndex.ConvertTrueValue( _Ident ) ].RemoveUnit( _Ident );
	LayerIndex.findAndRemove( _Ident );
	return true;
}

void MObjectManager::RemoveAllUnits( void )
{
	m_nSize = 0;

	for( unsigned int i = 0; i < m_vLayers.size(); ++i )
	{
		m_vLayers[i].RemoveAllUnits();
	}

	LayerIndex.clear();

	cout << "\tObjectManager: Removed All Objects\n";
}

void MObjectManager::ResizeLayer( int _layer, int newWidth, int newHeight )
{
	if( (unsigned int)_layer < m_vLayers.size() - 1 && _layer > -1 )
	{
		m_vLayers[_layer].Resize( newWidth, newHeight );
	}
	else
	{
		cout << "\tObjectManager: Error - Layer " << _layer << " does not exist\n";
	}
}

IUnitInterface* MObjectManager::GetUnit( int _Ident )
{
	if( LayerIndex.ConvertTrueValue( _Ident ) == -1 )
		return NULL;
	return m_vLayers[ LayerIndex.ConvertTrueValue( _Ident ) ].GetUnit( _Ident );
}

void MObjectManager::Update( float fDT )
{
	for( unsigned int i = 0; i < m_vLayers.size(); ++i )
	{
		m_vLayers[i].Update( fDT );
	}
}

void MObjectManager::Render( int CameraX, int CameraY )
{
	for( unsigned int i = 0; i < m_vLayers.size(); ++i )
	{
		m_vLayers[i].Render( CameraX, CameraY );
	}
}

void MObjectManager::CheckCollisions( void )
{
	for( unsigned int i = 0; i < m_vLayers.size(); ++i )
	{
		m_vLayers[i].CheckCollisions();
	}
}

bool MObjectManager::MoveEntUp( int _Ident )
{
	IUnitInterface* toCheck = GetUnit( _Ident );

	int otherEntity;

	if( !FindLayer( _Ident ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY() - 1 ) )
		return false;

	if( otherEntity = FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY() - 1 ) > 0 )
	{
		cout << "Collided With unit " << otherEntity << "\n";
		return toCheck->CheckCollision( GetUnit( otherEntity ) );
	}
	else if( FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY() - 1 ) == 0 )
	{
		FindFlake( _Ident ).SetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY() - 1, _Ident );

		toCheck->SetIndexPosY( toCheck->GetIndexPosY() - 1 );
		toCheck->SetFlag_MovementState( FLAG_MOVESTATE_MOVING );
		toCheck->SetFlag_DirectionToMove( FLAG_MOVE_UP );
		toCheck->SetDistanceLeft( 32.0f );
	}

	return true;
}

bool MObjectManager::MoveEntDown( int _Ident )
{
	IUnitInterface* toCheck = GetUnit( _Ident );

	int otherEntity;

	if( !FindLayer( _Ident ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY() + 1 ) )
		return false;

	if( otherEntity = FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY() + 1 ) > 0 )
	{
		cout << "Collided With unit " << otherEntity << "\n";
		
		return toCheck->CheckCollision( GetUnit( otherEntity ) );
	}
	else if( FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY() + 1 ) == 0 )
	{
		FindFlake( _Ident ).SetInfoAtIndex( toCheck->GetIndexPosX(), toCheck->GetIndexPosY(), _Ident );

		toCheck->SetIndexPosY( toCheck->GetIndexPosY() + 1 );
		toCheck->SetFlag_MovementState( FLAG_MOVESTATE_MOVING );
		toCheck->SetFlag_DirectionToMove( FLAG_MOVE_DOWN );
		toCheck->SetDistanceLeft( 32.0f );
	}

	return true;
}

bool MObjectManager::MoveEntLeft( int _Ident )
{
	IUnitInterface* toCheck = GetUnit( _Ident );

	int otherEntity;


	if( !FindLayer( _Ident ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( toCheck->GetIndexPosX() - 1, toCheck->GetIndexPosY()  ) )
		return false;

	if( toCheck->GetIndexPosX() - 1 < 0 )
	{
		cout << "Movement Denied in MoveEntRight\n";
		return false;
	}

	if( otherEntity = FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX() - 1, toCheck->GetIndexPosY() ) > 0 )
	{
		cout << "Collided With unit " << otherEntity << "\n";
		return toCheck->CheckCollision( GetUnit( otherEntity ) );
	}
	else if( FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX() - 1, toCheck->GetIndexPosY() ) == 0 )
	{
		FindFlake( _Ident ).SetInfoAtIndex( toCheck->GetIndexPosX() - 1, toCheck->GetIndexPosY(), _Ident );

		toCheck->SetIndexPosX( toCheck->GetIndexPosX() - 1 );
		toCheck->SetFlag_MovementState( FLAG_MOVESTATE_MOVING );
		toCheck->SetFlag_DirectionToMove( FLAG_MOVE_LEFT );
		toCheck->SetDistanceLeft( 32.0f );
	}

	return true;
}

bool MObjectManager::MoveEntRight( int _Ident )
{
	IUnitInterface* toCheck = GetUnit( _Ident );

	if( toCheck->GetIndexPosX() + 1 > FindLayer( _Ident).GetLayerWidth() - 1 )
	{
		cout << "Movement Denied in MoveEntRight\n";
		return false;
	}

	int otherEntity;

	if( !FindLayer( _Ident ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( toCheck->GetIndexPosX() + 1, toCheck->GetIndexPosY() ) )
		return false;

	if( otherEntity = FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX() + 1, toCheck->GetIndexPosY() ) > 0 )
	{
		cout << "Collided With unit " << otherEntity << "\n";
		return toCheck->CheckCollision( GetUnit( otherEntity ) );
	}
	else if( FindFlake( _Ident ).GetInfoAtIndex( toCheck->GetIndexPosX() + 1, toCheck->GetIndexPosY() ) == 0 )
	{
		FindFlake( _Ident ).SetInfoAtIndex( toCheck->GetIndexPosX() + 1, toCheck->GetIndexPosY(), _Ident );

		toCheck->SetIndexPosX( toCheck->GetIndexPosX() + 1 );
		toCheck->SetFlag_MovementState( FLAG_MOVESTATE_MOVING );
		toCheck->SetFlag_DirectionToMove( FLAG_MOVE_RIGHT );
		toCheck->SetDistanceLeft( 32.0f );
	}

	return true;
}

MLayer& MObjectManager::FindLayer( int _Ident )
{
	return m_vLayers[ LayerIndex.ConvertTrueValue( _Ident ) ];
}

MFlake& MObjectManager::FindFlake( int _Ident )
{
	if( LayerIndex.ConvertTrueValue( _Ident ) > -1 )
		return m_vLayers[ LayerIndex.ConvertTrueValue( _Ident ) ].FindFlake( _Ident );
	else
		return m_vLayers[ 0 ].GetFlake( 0 );
}

int MObjectManager::FindValueInFlakeInLayerAtIndex( int _layer, int _flake, int _x, int _y )
{
	if( _layer > -1 && _layer < 9 )
		return m_vLayers[_layer].GetValueInFlakeAtIndex( _flake, _x, _y );
	else
		return -1;
}

void MObjectManager::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "Add.Object" )
	{
		if( _toHandle->GetParam() )
			MObjectManager::AddUnitIndexed( (IUnitInterface*)_toHandle->GetParam(), 1 );
	}
}
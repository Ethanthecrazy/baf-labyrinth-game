#include "MLayer.h"
#include <iostream>

MLayer::MLayer( int _layerID ) : LayerID( _layerID )
{
	LayerWidth = 0;
	LayerHeight = 0;

	m_nSize = 0;

	OffSetFromCenterX = 0;
	OffSetFromCenterY = 0;

	for( int i = 0; i < 9; ++i )
	{
		m_vFlakes.push_back( MFlake( LayerWidth, LayerHeight, OffSetFromCenterX, OffSetFromCenterY, LayerID ) );
		m_vFlakes[ m_vFlakes.size() - 1].SetFlakeType( i );
	}

	for( int y = 0; y < LayerHeight; ++y )
	{
		for( int x = 0; x < LayerWidth; ++x )
		{
			m_vFlakes[ OBJECT_TILE ].SetInfoAtIndex( x, y, rand() % 2 + 1 );
		}
	}
}

int MLayer::AddUnit( IUnitInterface* _toAdd )
{
	m_nSize += 1;

	// Loop through the array of flakes.
	for( unsigned int i = 0; i < m_vFlakes.size(); ++i )
	{
		//Try and find one that already contains objects of that type.
		if( _toAdd->m_nUnitType == m_vFlakes[i].GetFlakeType() )
		{
			//Modify the unit's ID number.
			_toAdd->m_nIdentificationNumber += i * 10000;

			// Tell the flake to add the new unit and store it's return value.
			int tempnum = m_vFlakes[i].AddUnit( _toAdd );

			// Add the value to the conversion tree.
			FlakeIndex.InsertEntry( tempnum, i );

			// Return the value so that the ObjectManager can add it, then return it.
			return tempnum;
		}
	}

	return -1;
}


int MLayer::AddUnitIndexed( IUnitInterface* _toAdd )
{
	m_nSize += 1;

	// Loop through the array of flakes.
	for( unsigned int i = 0; i < m_vFlakes.size(); ++i )
	{
		//Try and find one that already contains objects of that type.
		if( _toAdd->m_nUnitType == m_vFlakes[i].GetFlakeType() )
		{
			//Modify the unit's ID number.
			_toAdd->m_nIdentificationNumber += i * 10000;

			// Tell the flake to add the new unit and store it's return value.
			int tempnum = m_vFlakes[i].AddUnitIndexed( _toAdd );

			// Add the value to the conversion tree.
			FlakeIndex.InsertEntry( tempnum, i );

			// Return the value so that the ObjectManager can add it, then return it.
			return tempnum;
		}
	}

	return -1;
}
bool MLayer::RemoveUnit( int _Ident )
{
	m_nSize -= 1;
	m_vFlakes[ FlakeIndex.ConvertTrueValue( _Ident ) ].RemoveUnit( _Ident );
	FlakeIndex.findAndRemove( _Ident );
	return true;
}
void MLayer::RemoveAllUnits( void )
{
	m_nSize = 0;

	for( unsigned int i = 0; i < m_vFlakes.size(); ++i )
	{
		m_vFlakes[i].RemoveAllUnits();
	}

	FlakeIndex.clear();
}


void MLayer::Resize( int newWidth, int newHeight )
{
	if( !m_nSize )
	{
		LayerWidth = newWidth;
		LayerHeight = newHeight;

		for( unsigned int i = 0; i < m_vFlakes.size() - 1; ++i )
		{
			m_vFlakes[i].Resize( newWidth, newHeight );
		}
		cout << "\tLayer: Resized( " << LayerWidth <<", " << LayerHeight << " )\n";
	}
	else
		cout << "\tLayer: Resized Failed - The layer contained objects.\n";
}

IUnitInterface* MLayer::GetUnit( int _Ident )
{
	if( FlakeIndex.ConvertTrueValue( _Ident ) == -1 )
		return NULL;
	return m_vFlakes[ FlakeIndex.ConvertTrueValue( _Ident ) ].GetUnit( _Ident );
}

void MLayer::Update( float fDT )
{
	for( unsigned int i = 0; i < m_vFlakes.size(); ++i )
	{
		m_vFlakes[i].Update( fDT );
	}
}
void MLayer::Render( int CameraX, int CameraY )
{
	for( unsigned int i = 0; i < m_vFlakes.size() - 4; ++i )
	{
		m_vFlakes[i].Render( CameraX, CameraY );
	}
}
void MLayer::CheckCollisions( void )
{
	for( unsigned int i = 0; i < m_vFlakes.size(); ++i )
	{
		if( m_vFlakes[i].GetFlakeType() == OBJECT_ENTITY ||  m_vFlakes[i].GetFlakeType() == OBJECT_OBJECT ) 
			m_vFlakes[i].CheckCollisions();
	}
}

MFlake& MLayer::FindFlake( int _Ident )
{
	return m_vFlakes[ FlakeIndex.ConvertTrueValue( _Ident ) ];
}

int MLayer::GetValueInFlakeAtIndex( int _flake, int _x, int _y )
{
	if( _flake < 9 && _flake > -1 )
		return m_vFlakes[ _flake ].GetInfoAtIndex( _x, _y );
	else
		return -1;
}

void MLayer::SetValueInFlakeAtIndex( int _value, int _flake, int _x, int _y )
{
	m_vFlakes[ _flake ].SetInfoAtIndex( _x, _y, _value );
}
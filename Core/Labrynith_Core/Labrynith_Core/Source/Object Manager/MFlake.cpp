#include "MFlake.h"
#include <iostream>

#include "../Wrappers/CSGD_TextureManager.h"
#include "../Wrappers/CSGD_Direct3D.h"

#include "../Messaging/MMessageSystem.h"

#include "MObjectManager.h"

MFlake::MFlake( int _LayerWidth, int _LayerHeight, int _OffSetFromCenterX, int _OffSetFromCenterY, int _parentLayer ) : LayerWidth( _LayerWidth ), 
	LayerHeight( _LayerHeight ), 
	OffSetFromCenterX( _OffSetFromCenterX ), 
	OffSetFromCenterY( OffSetFromCenterY ),
	parentLayer( _parentLayer )
{

	ArrayIndex.clear();

	InformationArray = new int[ _LayerWidth * _LayerHeight ];

	for( int i = 0; i < LayerHeight * LayerWidth; ++i )
	{
		InformationArray[i] = 0;
	}

	m_nSize = 0 ;
}


MFlake::~MFlake()
{
	if( ( LayerWidth + LayerHeight ) )
	{
		delete [] InformationArray;
	}

}

int MFlake::AddUnit( IUnitInterface* _toAdd )
{
	if( m_nSize + 1 > 3499 )
		return -2;

	m_nSize += 1;
	_toAdd->m_nIdentificationNumber += m_nSize;
	ArrayIndex.InsertEntry( _toAdd->m_nIdentificationNumber, m_vObjects.size() );
	m_vObjects.push_back( _toAdd );
	cout << "\tFlake " << m_nFlakeType << ": Added " << _toAdd->m_nIdentificationNumber << "\n"; 
	return _toAdd->m_nIdentificationNumber;
}

int MFlake::AddUnitIndexed( IUnitInterface* _toAdd )
{
	if( m_nSize + 1 > 3499 )
		return -2;

	
	if( _toAdd->m_nIdentificationNumber == 0 )
	{
		// generate a new number
		m_nSize += 1;
		_toAdd->m_nIdentificationNumber = this->parentLayer * 100000 + this->GetFlakeType() * 10000 + m_nSize ;
	}
	else
	{
		while( GetUnit( _toAdd->m_nIdentificationNumber ) )
			_toAdd->m_nIdentificationNumber++ ;
	}



	//_toAdd->m_nIdentificationNumber += m_nSize;
	ArrayIndex.InsertEntry( _toAdd->m_nIdentificationNumber, m_vObjects.size() );
	m_vObjects.push_back( _toAdd );

	int indexX = _toAdd->GetIndexPosX();
	int indexY = _toAdd->GetIndexPosY();

	InformationArray[ indexX + indexY * LayerWidth ] = _toAdd->m_nIdentificationNumber;

	return _toAdd->m_nIdentificationNumber;
}

bool MFlake::RemoveUnit( int _Ident )
{
	cout << "\tFlake " << m_nFlakeType << ": Removing " << _Ident << " "; 
	//m_nSize -= 1;
	if( ArrayIndex.ConvertTrueValue( _Ident ) > -1 )
	{
		IUnitInterface* toDelete = m_vObjects[ ArrayIndex.ConvertTrueValue( _Ident ) ];
		InformationArray[ toDelete->GetIndexPosX() + toDelete->GetIndexPosY() * LayerWidth ] = 0;
		m_vObjects.erase( m_vObjects.begin() + ArrayIndex.ConvertTrueValue( _Ident ) );
		ArrayIndex.DecrementFromNode( ArrayIndex.ConvertTrueValue( _Ident ) );
		ArrayIndex.findAndRemove( _Ident );

		cout << "Successful\n";
		return true;
	}
	else
	{
		cout << "False\n";
		return false;
	}
}

void MFlake::RemoveAllUnits( void )
{
	m_nSize = 0;

	m_vObjects.clear();

	ArrayIndex.clear();
}

IUnitInterface* MFlake::GetUnit( int _Ident )
{
	if( ArrayIndex.ConvertTrueValue( _Ident ) == -1 )
		return NULL;
	return m_vObjects[ ArrayIndex.ConvertTrueValue( _Ident ) ];
}

void MFlake::Update( float fDT )
{
	if( m_nFlakeType == OBJECT_LIGHT )
	{
		int decision = rand() % 4;
		switch( decision )
		{
		case 0:
			for( int y = 0; y < LayerHeight; ++y )
			{
				for( int x = 0; x < LayerWidth; ++x )
				{
					LightingProcess( x, y );
				}
			}
			break;
		case 1:
			for( int x = 0; x < LayerWidth; ++x )
			{
				for( int y = 0; y < LayerHeight; ++y )
				{
					LightingProcess( x, y );
				}
			}
			break;
		case 2:
			for( int y = LayerHeight - 1; y  > -1; --y )
			{
				for( int x = LayerWidth - 1; x > - 1; --x )
				{
					LightingProcess( x, y );
				}
			}
			break;
		case 3:
			for( int x = LayerWidth - 1; x > - 1; --x )
			{
				for( int y = LayerHeight - 1; y  > -1; --y )
				{
					LightingProcess( x, y );
				}
			}
			break;
		}
	}

	for( unsigned int i = 0; i < m_vObjects.size(); ++i )
	{
		m_vObjects[i]->Update( fDT );
	}
}

void MFlake::LightingProcess( int x, int y )
{
	bool bCanTransfer = false;

	if( GetInfoAtIndex( x, y ) > 254 )
		bCanTransfer = true;

		SetInfoAtIndex( x, y, GetInfoAtIndex( x, y ) - 10 );

	if( GetInfoAtIndex( x, y ) < 10 )
	{
		SetInfoAtIndex( x, y, 0 );
		return;
	}

	if( GetInfoAtIndex( x, y ) > GetInfoAtIndex( x + 1, y ) )
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x + 1, y, GetInfoAtIndex( x, y ) - 5 , this ) );
	if( GetInfoAtIndex( x, y ) > GetInfoAtIndex( x - 1, y ) )
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x - 1, y, GetInfoAtIndex( x, y ) - 5 , this ) );
	if( GetInfoAtIndex( x, y ) > GetInfoAtIndex( x, y + 1 ) )
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x, y + 1, GetInfoAtIndex( x, y ) - 5, this ) );
	if( GetInfoAtIndex( x, y ) > GetInfoAtIndex( x, y - 1 ) )
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x, y - 1, GetInfoAtIndex( x, y ) - 5, this ) );
}

void MFlake::Render( int CameraX, int CameraY )
{

	switch ( m_nFlakeType )
	{
	case OBJECT_TILE:
		{
			//char temp[64];

			for( int y = 0; y < LayerHeight; ++y )
			{
				for( int x = 0; x < LayerWidth; ++x )
				{
					//sprintf( temp, "%i", MObjectManager::GetInstance()->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y ) ); 

					if( InformationArray[ x + y * LayerWidth] >= 0 )
					{
						switch( InformationArray[ x + y * LayerWidth] )
						{

						case 0:
							{
								if( GetInfoAtIndex( x, y - 1 ) > 0 )
								{
									CSGD_TextureManager::GetInstance()->Draw( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pitUpper.png" ),
										x * 32 - CameraX,
										y * 32 - CameraY,
										1.0f,
										1.0f,
										0,
										0.0f,
										0.0f,
										0.0f,
										D3DCOLOR_ARGB( MObjectManager::GetInstance()->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y ), 255, 255, 255) );	


								}
								/*		if( GetInfoAtIndex( x, y + 1 ) > 0 )
								{
								CSGD_TextureManager::GetInstance()->Draw( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pitLower.png" ),
								x * 32 - CameraX,
								y * 32 - CameraY );	

								}

								*/
								if( GetInfoAtIndex( x - 1, y ) > 0 )
								{
									CSGD_TextureManager::GetInstance()->Draw( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pitLeft.png" ),
										x * 32 - CameraX,
										y * 32 - CameraY,
										1.0f,
										1.0f,
										0,
										0.0f,
										0.0f,
										0.0f,
										D3DCOLOR_ARGB( MObjectManager::GetInstance()->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y ), 255, 255, 255) );	


								}
								if( GetInfoAtIndex( x + 1, y ) > 0 )
								{
									CSGD_TextureManager::GetInstance()->Draw( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pitRight.png" ),
										x * 32 - CameraX,
										y * 32 - CameraY,
										1.0f,
										1.0f,
										0,
										0.0f,
										0.0f,
										0.0f,
										D3DCOLOR_ARGB( MObjectManager::GetInstance()->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y ), 255, 255, 255) );	


								}

								break;
							}

						case 1:
							{
								//int toUnload;
								//CSGD_TextureManager::GetInstance()->Draw( toUnload = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/singleTile.png" ),
								//	x * 32 - CameraX,
								//	y * 32 - CameraY );	

								////CSGD_TextureManager::GetInstance()->UnloadTexture( toUnload );
								//break;
							}

						case 2:
							{	
								CSGD_TextureManager::GetInstance()->Draw( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/stoneTile.png" ),
									x * 32 - CameraX,
									y * 32 - CameraY,
									1.0f,
									1.0f,
									0,
									0.0f,
									0.0f,
									0.0f,
									D3DCOLOR_ARGB( MObjectManager::GetInstance()->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y ), 255, 255, 255) );	

								//CSGD_TextureManager::GetInstance()->UnloadTexture( toUnload );
								break;
							}
						}
					}	
				}
			}

			CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

			for( unsigned int i = 0; i < m_vObjects.size(); ++i )
			{
				m_vObjects[i]->Render( CameraX, CameraY );
			}
		}
		break;

	case OBJECT_OBJECT:

		for( unsigned int i = 0; i < m_vObjects.size(); ++i )
		{
			m_vObjects[i]->Render( CameraX, CameraY );
		}

		char temp[64];

		for( int y = 0; y < LayerHeight; ++y )
		{
			for( int x = 0; x < LayerWidth; ++x )
			{
				if(InformationArray[ x + y * LayerWidth ] == 0)
					continue;

				sprintf( temp, "%i", InformationArray[ x + y * LayerWidth ] ); 

				CSGD_Direct3D::GetInstance()->DrawTextA( temp, x * 32  - CameraX, y * 32 - CameraY );
			}
		}

		break;

	case OBJECT_ENTITY:

		if( m_nFlakeType == OBJECT_ENTITY )
		{
			CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

			for( unsigned int i = 0; i < m_vObjects.size(); ++i )
			{
				m_vObjects[i]->Render( CameraX, CameraY );
			}

			/*char temp[64];

			for( int y = 0; y < LayerHeight; ++y )
			{
				for( int x = 0; x < LayerWidth; ++x )
				{
					if(InformationArray[ x + y * LayerWidth ] == 0)
						continue;

					sprintf( temp, "%i", InformationArray[ x + y * LayerWidth ] ); 

					CSGD_Direct3D::GetInstance()->DrawTextA( temp, x * 32  - CameraX, y * 32 - CameraY );
				}
			}*/
		}

		break;

	case OBJECT_LIGHT:
		{
			//char temp[64];

			for( int y = 0; y < LayerHeight; ++y )
			{
				for( int x = 0; x < LayerWidth; ++x )
				{
					CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

					//sprintf( temp, "%i", InformationArray[ x + y * LayerWidth ] / 100 ); 

					//CSGD_Direct3D::GetInstance()->DrawTextA( temp, x * 32  - CameraX, y * 32 - CameraY );
				}
			}
		}
		break;
	}
}

void MFlake::CheckCollisions()
{

}

int MFlake::GetInfoAtIndex( int _x, int _y )
{
	int trueIndex = _x +_y * LayerWidth;

	if( _y < 0 )
	{
		return -1;
	}

	if( trueIndex < LayerHeight * LayerWidth )
	{
		return InformationArray[trueIndex];
	}
	else
	{
		return -1;
	}
}

void MFlake::SetInfoAtIndex( int _x, int _y, int _value )
{
	if( _x < LayerWidth && _x >= 0 )
	{
		int trueIndex = _x + _y * LayerHeight;

		if( trueIndex < LayerHeight * LayerWidth )
		{
			InformationArray[trueIndex] = _value;
		}
	}
}

void MFlake::Resize( int newWidth, int newHeight )
{ 
	LayerWidth = newWidth; 
	LayerHeight = newHeight; 

	//delete[] InformationArray;

	InformationArray = new int[ LayerWidth * LayerHeight ];

	for( int i = 0; i < LayerHeight * LayerWidth; ++i )
	{
		InformationArray[i] = 0;
	}
}

void MFlake::FinishMovingEnt( IUnitInterface* toFinish )
{
	switch( toFinish->GetFlag_DirectionToMove() )
	{
	case FLAG_MOVE_UP:
		SetInfoAtIndex( toFinish->GetIndexPosX(), toFinish->GetIndexPosY() + 1, 0 );
		break;
	case FLAG_MOVE_DOWN:
		SetInfoAtIndex( toFinish->GetIndexPosX(), toFinish->GetIndexPosY() - 1, 0 );
		break;
	case FLAG_MOVE_RIGHT:
		SetInfoAtIndex( toFinish->GetIndexPosX() - 1, toFinish->GetIndexPosY(), 0 );
		break;
	case FLAG_MOVE_LEFT:
		SetInfoAtIndex( toFinish->GetIndexPosX() + 1, toFinish->GetIndexPosY(), 0 );
		break;
	}

	cout << toFinish->GetIndexPosX() << " ," << toFinish->GetIndexPosY() << "\n";
}
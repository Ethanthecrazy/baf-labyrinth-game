#include "MFlake.h"
#include <iostream>

#include "../Wrappers/CSGD_TextureManager.h"
#include "../Wrappers/CSGD_Direct3D.h"

#include "../Messaging/MMessageSystem.h"

#include "MObjectManager.h"
#include "../GameStates/CGamePlayState.h"

#include "../Object Manager/Units/CBaseObject.h"
#include "../Object Manager/Units/Tiles/CDoor.h"

#define OFFSCREENAMOUNT 5

MFlake::MFlake( int _LayerWidth, int _LayerHeight, int _OffSetFromCenterX, int _OffSetFromCenterY, int _parentLayer ) : LayerWidth( _LayerWidth ), 
	LayerHeight( _LayerHeight ), 
	OffSetFromCenterX( _OffSetFromCenterX ), 
	OffSetFromCenterY( OffSetFromCenterY ),
	parentLayer( _parentLayer )
{
	ArrayIndex.clear();	
	m_nSize = 0;

	//if( _LayerHeight < 1 )
	//	_LayerHeight = 30;
	//if( _LayerWidth < 1 )
	//	_LayerWidth = 30;

	Resize(_LayerWidth, _LayerHeight);	
}


MFlake::~MFlake()
{	
	RemoveAllUnits();
	Resize( 0, 0 );
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

	_toAdd->AddRef();	
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

		toDelete->Release();
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
	for( unsigned int i = 0; i < m_vObjects.size(); ++i )
	{
		delete m_vObjects[i];
	}
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
	int index = GetInfoAtIndex( x, y );

	if( index > 254 )
		bCanTransfer = true;

		SetInfoAtIndex( x, y, index - 12 );

	if( index < 5 )
	{
		SetInfoAtIndex( x, y, 0 );
		return;
	}

	if( MObjectManager::GetInstance()->GetLayer( parentLayer ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( x, y ) == 0 )
	{
		return;
	}

	IUnitInterface* doorCheck = MObjectManager::GetInstance()->GetUnit( MObjectManager::GetInstance()->GetLayer( parentLayer ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( x, y ) );

	if( doorCheck )
	{
		if( doorCheck->m_nUnitType == OBJECT_TILE )
		{
			if( (((CBaseObject*)(doorCheck)))->GetType() == OBJ_DOOR )
			{
				if( !(((CDoor*)(doorCheck)))->GetIsOpen() )
				{
					return;
				}
			}
		}
	}

	if( index >= GetInfoAtIndex( x + 1, y ) )
	{
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x + 1, y, (int)(index / 1.1f), this ) );
	}
	if( index >= GetInfoAtIndex( x - 1, y ) )
	{
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x - 1, y, (int)(index / 1.1f), this ) );
	}
	if( index >= GetInfoAtIndex( x, y + 1 ) )
	{
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x, y + 1, (int)(index / 1.1f), this ) );
	}
	if( index >= GetInfoAtIndex( x, y - 1 ) )
	{
		MMessageSystem::GetInstance()->SendMsg( new msgTransferLight( x, y, x, y - 1, (int)(index / 1.1f), this ) );
	}
}

void MFlake::RenderObjects( int CameraX, int CameraY )
{
	unsigned int size = m_vObjects.size();
	for( unsigned int i = 0; i < size; ++i )
	{
		//Dont Render things off-screen(exception: lights by a certain amount(OFFSCREENAMOUNT))
		//Top Check
		if(m_vObjects[i]->GetPosY() + (OFFSCREENAMOUNT * TILE_WIDTH) < CameraY)
			continue;
		//Bottom Check
		if(m_vObjects[i]->GetPosY() > CameraY + 600 + (OFFSCREENAMOUNT * TILE_HEIGHT))
			continue;
		//Left Check
		if(m_vObjects[i]->GetPosX() + (OFFSCREENAMOUNT * TILE_WIDTH) < CameraX)
			continue;
		//Right Check
		if(m_vObjects[i]->GetPosX() > CameraX + 800 + (OFFSCREENAMOUNT * TILE_WIDTH))
			continue;

		m_vObjects[i]->Render( CameraX, CameraY );
	}
}
void MFlake::RenderDebug( int CameraX, int CameraY )
{
	char temp[64];
	for( int y = 0; y < LayerHeight; ++y )
	{
		for( int x = 0; x < LayerWidth; ++x )
		{
			if(InformationArray[ x + y * LayerWidth ] == 0)
				continue;

			sprintf_s( temp, "%i", InformationArray[ x + y * LayerWidth ] ); 

			CSGD_Direct3D::GetInstance()->DrawTextA( temp, x * TILE_WIDTH  - CameraX, y * TILE_HEIGHT - CameraY );
		}
	}
}
void MFlake::Render( int CameraX, int CameraY )
{
	switch ( m_nFlakeType )
	{
	case OBJECT_TILE:
		{
			//char temp[64];
			CSGD_TextureManager* TM = CSGD_TextureManager::GetInstance();
			MObjectManager* OM = MObjectManager::GetInstance();
			bool renderCulling = CGamePlayState::GetInstance()->GetRenderCulling();
			int stoneTileID, wallTileID;
			stoneTileID = TM->LoadTexture( "resource/stoneTile.png" );
			wallTileID = TM->LoadTexture( "resource/wall.png" );

			int startY = CameraY/TILE_HEIGHT;
			int startX = CameraX/TILE_WIDTH;
			int camBottom = (CameraY + 600)/TILE_HEIGHT;
			int camRight = (CameraX + 800)/TILE_WIDTH;
			for(int y = startY; y < camBottom; ++y )
			{
				for(int x = startX; x < camRight; ++x )
				{
					//sprintf( temp, "%i", OM->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y ) ); 

					if( InformationArray[ x + y * LayerWidth] >= 0 )
					{	
						int nBrightness = OM->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y );
						switch( InformationArray[ x + y * LayerWidth] )
						{
						case 0:
							{		
								int lightamount = OM->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y );
								if(lightamount == 0 && renderCulling)
									break;

								TM->Draw( wallTileID,
										x * TILE_WIDTH - CameraX,
										y * TILE_HEIGHT - CameraY,
										1.0f,
										1.0f,
										0,
										0.0f,
										0.0f,
										0.0f,
										D3DCOLOR_ARGB( 255, nBrightness, nBrightness, nBrightness) );			
							}
							break;

						case 1:
							{
								int lightamount = OM->GetLayer( parentLayer ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( x, y );
								if(lightamount == 0 && renderCulling)
									break;

								TM->Draw( stoneTileID,
									x * TILE_WIDTH - CameraX,
									y * TILE_HEIGHT - CameraY,
									1.0f,
									1.0f,
									0,
									0.0f,
									0.0f,
									0.0f,
									D3DCOLOR_ARGB( 255, nBrightness, nBrightness, nBrightness) );	

								//TM->UnloadTexture( toUnload );								
							}
							break;
						}
					}	
				}
			}
			RenderObjects( CameraX, CameraY );
			//RenderDebug( CameraX, CameraY );
		}
		break;

	case OBJECT_OBJECT:
		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		RenderObjects( CameraX, CameraY );
		//RenderDebug( CameraX, CameraY );
		break;

	case OBJECT_ENTITY:
		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		RenderObjects( CameraX, CameraY );
		//RenderDebug( CameraX, CameraY );
		break;

	case OBJECT_LIGHT:
		//RenderDebug( CameraX, CameraY );
		break;
	}
}

void MFlake::CheckCollisions()
{

}

int MFlake::GetInfoAtIndex( int _x, int _y )
{
	int trueIndex = _x +_y * LayerWidth;

	if(_x < 0 )
		return -1;

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
	// 11 or 12 && 3
	if( _x < LayerWidth && _x >= 0 )
	{
		if(_y >= 0)
		{
		int trueIndex = _x + _y * LayerHeight;

		if( trueIndex < LayerHeight * LayerWidth )
		{
			InformationArray[trueIndex] = _value;
		}
		}
	}
}

void MFlake::Resize( int newWidth, int newHeight )
{ 
	LayerWidth = newWidth; 
	LayerHeight = newHeight; 

	InformationArray.clear();
	InformationArray.resize(LayerHeight * LayerWidth);

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

	//cout << toFinish->GetIndexPosX() << " ," << toFinish->GetIndexPosY() << "\n";
}

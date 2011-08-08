#include "CPit.h"
#include "../../../Wrappers/CSGD_TextureManager.h"

CPit::CPit()
{
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Exit.png" ));
	this->m_nType = OBJ_PIT;
}

bool CPit::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	return false;
}

void CPit::Render( int CameraPosX, int CameraPosY )
{
	/*if( GetInfoAtIndex( x, y - 1 ) > 0 )
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
	}*/
}
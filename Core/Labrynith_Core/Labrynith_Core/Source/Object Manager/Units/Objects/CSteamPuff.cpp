#include "CSteamPuff.h"

#include "../../../Animation Manager/CAnimationManager.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include <d3dx9.h>
#include "../../../Messaging/MMessageSystem.h"

CSteamPuff::CSteamPuff()
{
	CBaseObject();

	m_nUnitType = OBJECT_OBJECT;

	ID_ANI = CAnimationManager::GetInstance()->GetID( "Puff" );

	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/steam.png" );

	CAnimationManager::GetInstance()->SetAnimTexture( ID_ANI, m_nImageID );

	CAnimationManager::GetInstance()->PlayAnimation( ID_ANI );

	scale = 0.5f;
}

void CSteamPuff::Update( float fDT )
{
	CAnimationManager::GetInstance()->UpdateAnimation( fDT, ID_ANI );
	
	if( !CAnimationManager::GetInstance()->IsAnimationPlaying( ID_ANI ) )
	{
		MMessageSystem::GetInstance()->SendMsg( new msgRemoveUnit( m_nIdentificationNumber ) );
	}
}

void CSteamPuff::Render( int CameraPosX, int CameraPosY )
{
	
	CAnimationManager::GetInstance()->Draw( ID_ANI, (int)GetPosX(), (int)GetPosY(), scale, scale, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 255, 255) );

}

void CSteamPuff::MakeIce()
{
	CAnimationManager::GetInstance()->StopAnimation( ID_ANI );

	ID_ANI = CAnimationManager::GetInstance()->GetID( "IcePuff" );

	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/icePuff.png" );

	CAnimationManager::GetInstance()->SetAnimTexture( ID_ANI, m_nImageID );

	CAnimationManager::GetInstance()->PlayAnimation( ID_ANI );

	scale = 1.0f;
}
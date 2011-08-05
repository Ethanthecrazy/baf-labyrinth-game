#include "COilCan.h"
#include "../../../Wrappers/CSGD_TextureManager.h"

COilCan::COilCan( void )
{
	CBaseObject::CBaseObject() ;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/OilBarrel.png") ;
	SetCharges( 5 ) ;
	SetLifeDuration( 30.0f ) ;
	SetOnFire( false ) ;
}

void COilCan::UseObject( CBaseObject* user )
{

}

void COilCan::Update(float fDT)
{
	CBaseObject::Update(fDT) ;

	if( GetOnFire() )
	{
		// set other objects on fire



		SetLifeDuration( GetLifeDuration() - fDT ) ;
		if( GetLifeDuration() <= 0 )
		{
			// delete oil, and fire on this tile
		}
	}

}

void COilCan::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render( CameraPosX , CameraPosY ) ;
}

#include "CMetal.h"
#include "../../MObjectManager.h"
#include "../CPlayer.h"
#include "../Golems/CGolem_Water.h"
#include "../Golems/CGolem_Iron.h"
#include "CElectricButton.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Animation Manager/CAnimationManager.h"
#include "CWaterTile.h"

CMetal::CMetal() 
{
	CBaseObject::CBaseObject() ;
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_METAL ;
	SetPowered( false ) ;
	SetElectricUpdateTimer( .5f ) ;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metalTile.png" ) ;
	m_nAnimID = CAnimationManager::GetInstance()->GetID( "Electricity" ) ;
	m_nAnimImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/electricity.png") ;
	CAnimationManager::GetInstance()->SetAnimTexture( m_nAnimID , m_nAnimImageID ) ;
	CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
	MEventSystem::GetInstance()->RegisterClient("CIRCUTBROKEN" , this ) ;
}

CMetal::~CMetal() 
{
	CBaseObject::~CBaseObject() ;
	m_nUnitType = OBJECT_TILE;
	MEventSystem::GetInstance()->UnregisterClient("CIRCUTBROKEN" , this ) ;
}

void CMetal::Update( float fDT )
{
	CBaseObject::Update(fDT) ;
	
	if( GetIsElectrified() )
	{
		CAnimationManager::GetInstance()->UpdateAnimation( fDT , m_nAnimID ) ;
		SetElectricUpdateTimer( GetElectricUpdateTimer() - fDT ) ;
	}
}

void CMetal::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render( CameraPosX , CameraPosY ) ;
	if( GetIsElectrified() )
		CAnimationManager::GetInstance()->Draw(m_nAnimID , (int)(GetPosX() - CameraPosX) + 36 , (int)(GetPosY() - CameraPosY) + 32,
		1.0f, 1.0f, 0, 0, 0, 0xffffffff);
}

void CMetal::SetPowered( bool powered ) 
{
	if( GetIsElectrified() == false && powered == true )
	{
		CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
	else if( GetIsElectrified() == true && powered == false )
	{
		CAnimationManager::GetInstance()->StopAnimation( m_nAnimID ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
	m_bPowered = powered ;
}

void CMetal::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "CIRCUTBROKEN" )
	{
		SetPowered( false ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
}
#include "CElectricButton.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../MObjectManager.h"
#include "../CPlayer.h"
#include "../Golems/CGolem_Water.h"
#include "../Golems/CGolem_Iron.h"
#include "CElectricButton.h"
#include "CMetal.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Animation Manager/CAnimationManager.h"
#include "CWaterTile.h"

CElectricButton::CElectricButton(string nLink)
{
	m_nUnitType = OBJECT_TILE;
	SetPressed( false ) ;
	SetLink( nLink ) ;
	MEventSystem::GetInstance()->RegisterClient("Button.Pressed", this);
	MEventSystem::GetInstance()->RegisterClient("Button.Unpress", this);
	m_nType = OBJ_ELECTRICBUTTON ;
	SetPowered( false ) ;
	SetElectricUpdateTimer( 0.0f ) ;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/singleTile.png" , 0xffffffff ) ;
	m_nAnimID = CAnimationManager::GetInstance()->GetID( "Electricity" ) ;
	m_nAnimImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/electricity.png") ;
	CAnimationManager::GetInstance()->SetAnimTexture( m_nAnimID , m_nAnimImageID ) ;
	m_nSoundID = CSGD_FModManager::GetInstance()->LoadSound( "resource/Sounds/Electricity.mp3" ) ;
	MEventSystem::GetInstance()->RegisterClient("CIRCUTBROKEN" , this ) ;
}
CElectricButton::~CElectricButton(void)
{
	CButton::~CButton() ;
	MEventSystem::GetInstance()->UnregisterClient("CIRCUTBROKEN" , this ) ;
}

void CElectricButton::Update( float fDT )
{
	CBaseObject::Update(fDT) ;
	
	if( GetElectricUpdateTimer() > 0 && GetIsElectrified() == false )
			SetElectricUpdateTimer( GetElectricUpdateTimer() - fDT ) ;

	if( GetIsElectrified() )
	{
		CAnimationManager::GetInstance()->UpdateAnimation( fDT , m_nAnimID ) ;
	}
}

bool CElectricButton::CheckCollision(IUnitInterface* pBase)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;
	
	if( GetIsElectrified() == false )
		return false;

	SetPressed(!GetIsPressed()) ;

	if(GetIsPressed())
	{
		MEventSystem::GetInstance()->SendEvent("Button.Pressed", (void*)GetLink());
		CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/pressed.mp3"));
	}
	else
	{
		MEventSystem::GetInstance()->SendEvent("Button.Unpress", (void*)GetLink());
		CSGD_FModManager::GetInstance()->PlaySoundA(CSGD_FModManager::GetInstance()->LoadSound("resource/Sounds/un-pressed.mp3"));
	}

	return false;
}

void CElectricButton::Render( int CameraPosX, int CameraPosY )
{
	CButton::Render( CameraPosX , CameraPosY ) ;
	if( GetIsElectrified() )
		CAnimationManager::GetInstance()->Draw(m_nAnimID , GetPosX() - CameraPosX , GetPosY() - CameraPosY , .2 , .2 , 0 , 0 , 0 , 0xffffffff ) ;
}

void CElectricButton::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "Button.Pressed" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  GetLink())
			SetPressed( true ) ;
	}
	else if( _toHandle->GetEventID() == "Button.Unpress" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  GetLink())
			SetPressed( false ) ;
	}
	else if( _toHandle->GetEventID() == "CIRCUTBROKEN" )
	{
		SetPowered( false ) ;
	}
}

void CElectricButton::SetPowered( bool powered )
{ 
	if( GetIsElectrified() == false && powered == true )
	{
		m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/singleTile.png" , 0x88888888 ) ;
		if( GetElectricUpdateTimer() <= 0.0f )
			CSGD_FModManager::GetInstance()->PlaySoundA( m_nSoundID ) ;
		CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
		SetElectricUpdateTimer( 10.0f ) ;
	}
	else if( GetIsElectrified() == true && powered == false )
	{
		m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/singleTile.png" , 0xffffffff ) ;
		//CSGD_FModManager::GetInstance()->StopSound( m_nSoundID ) ;
	}
	m_bPowered = powered ; 
}
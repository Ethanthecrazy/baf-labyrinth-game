#include "CWaterTile.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../CPlayer.h"
#include "../Golems/CGolem_Water.h"
#include "../Golems/CGolem_Iron.h"
#include "CElectricButton.h"
#include "../../../Animation Manager/CAnimationManager.h"
#include "../../MObjectManager.h"
#include "CMetal.h"

CWaterTile::CWaterTile()
{
	CBaseObject::CBaseObject();
	this->m_nIceImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/iceTile.png" ));
	this->m_nWaterImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/waterTile.png" ));
	this->m_nType = OBJ_WATER;
	SetIsFrozen(false);
	m_bPowered = false ;
	m_nAnimID = CAnimationManager::GetInstance()->GetID( "Electricity" ) ;
	m_nAnimImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/electricity.png") ;
	CAnimationManager::GetInstance()->SetAnimTexture( m_nAnimID , m_nAnimImageID ) ;
	CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
	MEventSystem::GetInstance()->RegisterClient("CIRCUTBROKEN" , this ) ;
}
CWaterTile::CWaterTile(bool bIsFrozen)
{
	CBaseObject::CBaseObject();
	this->m_nIceImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/iceTile.png" ));
	this->m_nWaterImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/waterTile.png" ));
	this->m_nType = OBJ_WATER;
	SetIsFrozen(bIsFrozen);
	m_bPowered = false ;
	m_nAnimID = CAnimationManager::GetInstance()->GetID( "Electricity" ) ;
	m_nAnimImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/electricity.png") ;
	CAnimationManager::GetInstance()->SetAnimTexture( m_nAnimID , m_nAnimImageID ) ;
	CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
	MEventSystem::GetInstance()->RegisterClient("CIRCUTBROKEN" , this ) ;
}
CWaterTile::~CWaterTile()
{
	MEventSystem::GetInstance()->UnregisterClient("CIRCUTBROKEN" , this ) ;
}

void CWaterTile::Update(float fDT)
{
	if( GetIsElectrified() )
	{
		CAnimationManager::GetInstance()->UpdateAnimation( fDT , m_nAnimID ) ;
		SetElectricUpdateTimer( GetElectricUpdateTimer() - fDT ) ;
	}
}
void CWaterTile::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render(CameraPosX, CameraPosY);
	if( GetIsElectrified() )
		CAnimationManager::GetInstance()->Draw(m_nAnimID , GetPosX() - CameraPosX , GetPosY() - CameraPosY , .2 , .2 , 0 , 0 , 0 , 0xffffffff ) ;
}
bool CWaterTile::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	return false;
}
//accessors
bool CWaterTile::IsFrozen()
{
	return m_bIsFrozen;
}
//mutators
void CWaterTile::SetIsFrozen(const bool bIsFrozen)
{
	m_bIsFrozen = bIsFrozen;
	if(IsFrozen())
	{
		SetPowered( false ) ;
		this->m_nImageID = m_nIceImageID;
	}
	else
		this->m_nImageID = m_nWaterImageID;
}

void CWaterTile::SetPowered( bool powered ) 
{
	if( m_bIsFrozen == true && powered == true )
	{
		return ;
	}
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

void CWaterTile::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "CIRCUTBROKEN" )
	{
		SetPowered( false ) ;
		SetElectricUpdateTimer( .5f ) ;
	}
}

#include "CElectricGenerator.h"
#include "../../MObjectManager.h"
#include "CMetal.h"
#include "../CBaseGolem.h"
#include "../Golems/CGolem_Water.h"
#include "../Golems/CGolem_Iron.h"
#include "CElectricButton.h"
#include "../CPlayer.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Animation Manager/CAnimationManager.h"
#include "CWaterTile.h"

CElectricGenerator::CElectricGenerator() 
{
	CBaseObject::CBaseObject() ;
	m_nUnitType = OBJECT_TILE ;
	m_nType = OBJ_ELECTRICGENERATOR ; 
	m_nElectricUpdateTimer = .5f ;
	m_bGolemConnected = false ;
	m_bEvaluateConnections = true ;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/GeneratorTile.png" ) ;
	m_nAnimID = CAnimationManager::GetInstance()->GetID( "Electricity" ) ;
	m_nAnimImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/electricity.png") ;
	CAnimationManager::GetInstance()->SetAnimTexture( m_nAnimID , m_nAnimImageID ) ;
	CAnimationManager::GetInstance()->PlayAnimation( m_nAnimID ) ;
	MEventSystem::GetInstance()->RegisterClient("CIRCUTBROKEN" , this ) ;
}

CElectricGenerator::~CElectricGenerator() 
{
	CBaseObject::~CBaseObject() ;
	MEventSystem::GetInstance()->UnregisterClient("CIRCUTBROKEN" , this ) ;
}

void CElectricGenerator::Update(float fDT )
{
	CBaseObject::Update(fDT) ;
	
	CAnimationManager::GetInstance()->UpdateAnimation( fDT , m_nAnimID ) ;
	if( m_bEvaluateConnections == true )
	{
		m_bEvaluateConnections = false ;
		MakeConnections( this ) ;
		m_EvaluatedConnections.clear() ;
	}
}

void CElectricGenerator::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render( CameraPosX , CameraPosY ) ;
	CAnimationManager::GetInstance()->Draw(m_nAnimID , GetPosX() - CameraPosX , GetPosY() - CameraPosY , .2 , .2 , 0 , 0 , 0 , 0xffffffff ) ;
}

void CElectricGenerator::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "CIRCUTBROKEN" )
	{
		m_bEvaluateConnections = true ;
	}
}

bool CElectricGenerator::MakeConnections( IUnitInterface* obj )
{
	for( int i = -1 ; i <= 1 ; ++i )
	{
		for( int u = -1 ; u <= 1 ; ++u )
		{
			if( ( i == -1 && u != 0 ) || ( i == 1 && u != 0 ) || ( u == -1 && i != 0 ) || ( u == 1 && i != 0 ) )
					continue ;
			
			int item = MObjectManager::GetInstance()->FindLayer( obj->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( obj->GetIndexPosX() + i , obj->GetIndexPosY() + u ) ;
			int buttonID = MObjectManager::GetInstance()->FindLayer( obj->m_nIdentificationNumber ).GetFlake( OBJECT_TILE ).GetInfoAtIndex( obj->GetIndexPosX() + i , obj->GetIndexPosY() + u ) ;
			int entityID = MObjectManager::GetInstance()->FindLayer( obj->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( obj->GetIndexPosX() + i , obj->GetIndexPosY() + u ) ;
			IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
			IUnitInterface* button = (MObjectManager::GetInstance()->GetUnit(buttonID)) ;
			IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
			if( object)
			{
				if( FindConnection(object) == false )
				{
					if( object->GetType() == OBJ_METAL )
					{
						((CMetal*)object)->SetPowered(true) ;
						m_EvaluatedConnections.push_back( object ) ;
						MakeConnections( object ) ;
					}
					else if( object->GetType() == OBJ_WATER )
					{
						((CWaterTile*)object)->SetPowered(true) ;
						m_EvaluatedConnections.push_back( object ) ;
						MakeConnections( object ) ;
					}
				}
			}
			if( button )
			{
				if( FindConnection(button) == false )
				{
					if( button->GetType() == OBJ_ELECTRICBUTTON )
					{
						((CElectricButton*)button)->SetPowered(true) ;
						m_EvaluatedConnections.push_back( button ) ;
						MakeConnections( button ) ;
					}
				}
			}
			if( entity && FindConnection(entity) == false )
			{
				if( FindConnection(object) == false )
				{
					if( entity->GetType() == ENT_GOLEM )
					{
						if( ((CBaseGolem*)entity)->GetGolemType() == WATER_GOLEM )
						{
							((CGolem_Water*)entity)->SetPowered(true) ;
							m_EvaluatedConnections.push_back( object ) ;
							MakeConnections( entity ) ;
							m_bGolemConnected = true ;
						}
						else if( ((CBaseGolem*)entity)->GetGolemType() == IRON_GOLEM )
						{
							((CGolem_Iron*)entity)->SetPowered(true) ;
							m_EvaluatedConnections.push_back( object ) ;
							MakeConnections( entity ) ;
							m_bGolemConnected = true ;
						}
					}
				}
			}
		}
	}
	return true ;
}

bool CElectricGenerator::FindConnection( IUnitInterface* obj )
{
	for( int i = 0 ; i < m_EvaluatedConnections.size() ; ++i )
	{
		if( obj == m_EvaluatedConnections[i] )
			return true ;
	}
	return false ;
}
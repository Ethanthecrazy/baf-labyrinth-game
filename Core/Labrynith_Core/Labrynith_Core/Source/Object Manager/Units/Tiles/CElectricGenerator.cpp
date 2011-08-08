
#include "CElectricGenerator.h"
#include "../../MObjectManager.h"
#include "CMetal.h"
#include "../CBaseGolem.h"
#include "../Golems/CGolem_Water.h"
#include "../Golems/CGolem_Iron.h"
#include "CElectricButton.h"
#include "../CPlayer.h"

CElectricGenerator::CElectricGenerator() 
{
	CBaseObject::CBaseObject() ;
	m_nUnitType = OBJECT_TILE ;
	m_nType = OBJ_ELECTRICGENERATOR ; 
	m_nElectricUpdateTimer = 10.0f ;
}

CElectricGenerator::~CElectricGenerator() 
{
	CBaseObject::~CBaseObject() ;
}

void CElectricGenerator::Update(float fDT )
{
	CBaseObject::Update(fDT) ;
	
	SetElectricUpdateTimer( GetElectricUpdateTimer() - fDT ) ;
	if( GetElectricUpdateTimer() <= 0 )
	{
		// check surrounding objects to see if they can catch on fire
		for( int i = -1 ; i <= 1 ; ++i )
		{
			for( int u = -1 ; u <= 1 ; ++u )
			{
				if( ( i == -1 && u != 0 ) || ( i == 1 && u != 0 ) || ( u == -1 && i != 0 ) || ( u == 1 && i != 0 ) )
					continue ;
				int item = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_OBJECT ).GetInfoAtIndex( this->GetIndexPosX() + i , this->GetIndexPosY() + u ) ;
				IUnitInterface* object = (MObjectManager::GetInstance()->GetUnit(item)) ;
				if( object )
				{
					if( object->GetType() == OBJ_METAL )
					{
						((CMetal*)object)->SetPowered(true) ;
					}
					else if( object->GetType() == OBJ_ELECTRICBUTTON )
					{
						((CElectricButton*)object)->SetPowered(true) ;
					}
					else if( object->GetType() == ENT_GOLEM )
					{
						if( ((CBaseGolem*)object)->GetGolemType() == WATER_GOLEM )
						{
							((CGolem_Water*)object)->SetPowered(true) ;
						}
						else if( ((CBaseGolem*)object)->GetGolemType() == IRON_GOLEM )
						{
							((CGolem_Iron*)object)->SetPowered(true) ;
						}
					}
				}
			}
		}
		int entityID = MObjectManager::GetInstance()->FindLayer( this->m_nIdentificationNumber ).GetFlake( OBJECT_ENTITY ).GetInfoAtIndex( this->GetIndexPosX(), this->GetIndexPosY() ) ;
		IUnitInterface* entity = (MObjectManager::GetInstance()->GetUnit(entityID)) ;
		if( entity )
		{
			if( entity->GetType() == ENT_PLAYER )
			{
				((CPlayer*)entity)->SetLives( ((CPlayer*)entity)->GetLives() - 1 ) ;
			}
		}
	}
}
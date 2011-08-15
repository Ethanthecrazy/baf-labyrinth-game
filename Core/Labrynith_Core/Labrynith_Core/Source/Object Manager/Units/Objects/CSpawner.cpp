#include "CSpawner.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../MObjectManager.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../MObjectManager.h"
#include "../../../GameStates/CGamePlayState.h"
#include "CAttractor.h"
#include "../CPlayer.h"
#include "../CBaseGolem.h"
#include "../Golems/CGolem_Earth.h"
#include "../Golems/CGolem_Fire.h"
#include "../Golems/CGolem_Water.h"
#include "../Golems/CGolem_Ice.h"
#include "../Golems/CGolem_Air.h"
#include "../Golems/CGolem_Lava.h"
#include "../Golems/CGolem_Iron.h"
#include "../Golems/CGolem_Light.h"
#include "../Golems/CGolem_Shadow.h"
#include "CLightOrb.h"

void CSpawner::Update(float fDT)
{
}

void CSpawner::Render(int CameraPosX, int CameraPosY)
{
	CBaseObject::Render(CameraPosX, CameraPosY);
}


bool CSpawner::CheckCollision(IUnitInterface* pBase)
{
	return false;
}



CSpawner::CSpawner(int nSpawnerType) : m_nSpawnerType(nSpawnerType)
{	
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_SPAWNER;
	m_nSpawnedID = 0;
	MEventSystem::GetInstance()->RegisterClient("spawner.spawn", this);
	m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/stoneTile.png" ));
}

CSpawner::~CSpawner(void)
{

}

void CSpawner::HandleEvent( Event* _toHandle )
{
	if(_toHandle->GetEventID() == "spawner.spawn")
	{
		// if it cant find it in the object manager and its not held by the player
		if(!MObjectManager::GetInstance()->GetUnit(m_nSpawnedID))
	{
		// put this in to prevent copy of attractors
		IUnitInterface* player = MObjectManager::GetInstance()->GetUnit(CGamePlayState::GetInstance()->testVaribale);

		if( player )
		{
			if(this->m_nSpawnerType == SPAWNER_PLAYER)
				return;

			if( ((CPlayer*)player)->GetHeldItem() )
			{
				if(((CPlayer*)player)->GetHeldItem()->m_nIdentificationNumber == m_nSpawnedID)
					return;
			}
		}

		switch(m_nSpawnerType)
		{
		case SPAWNER_LIGHTORB:
			{				
				CLightOrb* temp = new CLightOrb();
				((CLightOrb*)(temp))->SetPosX( GetPosX() );
				((CLightOrb*)(temp))->SetPosY( GetPosY() );
				((CLightOrb*)(temp))->SetIndexPosX( GetIndexPosX() );
				((CLightOrb*)(temp))->SetIndexPosY( GetIndexPosY() );

				m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );	
				
				break;
			}

		case SPAWNER_EARTH:
			{
			CGolem_Earth* temp = new CGolem_Earth();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );	
			
			break;
			}
			
		case SPAWNER_FIRE:
			{
			CGolem_Fire* temp = new CGolem_Fire();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );	
			
			break;
			}

		case SPAWNER_WATER:
			{
			CGolem_Water* temp = new CGolem_Water();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;			
			}

		case SPAWNER_ICE:
			{
			CGolem_Ice* temp = new CGolem_Ice();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_AIR:
			{
			CGolem_Air* temp = new CGolem_Air();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_LAVA:
			{
			CGolem_Lava* temp = new CGolem_Lava();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_IRON:
			{
			CGolem_Iron* temp = new CGolem_Iron();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_LIGHT:
			{
			CGolem_Light* temp = new CGolem_Light();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_SHADOW:
			{
			CGolem_Shadow* temp = new CGolem_Shadow();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}



		case SPAWNER_ATTRACTOR_EARTH:
			{
			CAttractor* temp = new CAttractor();			
			((CAttractor*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/EarthAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(EARTH_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );	
			
			break;
			}
			
		case SPAWNER_ATTRACTOR_FIRE:
			{
			CAttractor* temp = new CAttractor();			
			((CAttractor*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/FireAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(FIRE_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );	
			
			break;
			}

		case SPAWNER_ATTRACTOR_WATER:
			{
			CAttractor* temp = new CAttractor();			
			((CBaseObject*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/WaterAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(WATER_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;			
			}

		case SPAWNER_ATTRACTOR_ICE:
			{
			CAttractor* temp = new CAttractor();			
			((CBaseObject*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/IceAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(ICE_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_ATTRACTOR_AIR:
			{
			CAttractor* temp = new CAttractor();			
			((CBaseObject*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/AirAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(AIR_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_ATTRACTOR_LAVA:
			{
			CAttractor* temp = new CAttractor();			
			temp->m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/heart.png" );
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(LAVA_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_ATTRACTOR_IRON:
			{
			CAttractor* temp = new CAttractor();			
			((CBaseObject*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/IronAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(IRON_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_ATTRACTOR_LIGHT:
			{
			CAttractor* temp = new CAttractor();			
			((CBaseObject*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/LightAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(LIGHT_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			
			break;
			}

		case SPAWNER_ATTRACTOR_SHADOW:
			{
			CAttractor* temp = new CAttractor();			
			((CBaseObject*)temp)->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/ShadowAttractor.png" ));
			((CAttractor*)(temp))->SetPosX( GetPosX() );
			((CAttractor*)(temp))->SetPosY( GetPosY() );
			((CAttractor*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CAttractor*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CAttractor*)(temp))->SetElemType(SHADOW_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, this->GetLayerLocation() );
			break;
			}


		case SPAWNER_PLAYER:
			MMessageSystem::GetInstance()->SendMsg( new msgCreatePlayer( GetIndexPosX(), GetIndexPosY(), this->GetLayerLocation() ) );
			m_nSpawnedID = CGamePlayState::GetInstance()->testVaribale;
			break;
		}
	}
	}
}
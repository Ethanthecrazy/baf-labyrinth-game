#include "CSpawner.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../MObjectManager.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../CBaseGolem.h"
#include "../../MObjectManager.h"
#include "../../../GameStates/CGamePlayState.h"

void CSpawner::Update(float fDT)
{
	if(!MObjectManager::GetInstance()->GetUnit(m_nSpawnedID))
	{
		switch(m_nSpawnerType)
		{
		case SPAWNER_EARTH:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/StoneGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(EARTH_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );			
			break;
			}
			
		case SPAWNER_FIRE:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/FireGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(FIRE_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );			
			break;
			}

		case SPAWNER_WATER:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/WaterGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(WATER_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
			break;			
			}

		case SPAWNER_ICE:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IceGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(ICE_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
			break;
			}

		case SPAWNER_AIR:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/AirGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(AIR_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
			break;
			}

		case SPAWNER_LAVA:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/LavaGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(LAVA_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
			break;
			}

		case SPAWNER_IRON:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IronGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(IRON_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
			break;
			}

		case SPAWNER_LIGHT:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/LightGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(LIGHT_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
			break;
			}

		case SPAWNER_SHADOW:
			{
			IUnitInterface* temp = new CBaseGolem();
			((CBaseGolem*)(temp))->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/ShadowGolem.png" ));
			((CBaseGolem*)(temp))->LoadEntMoveAnimIDs();
			((CBaseGolem*)(temp))->SetPosX( GetPosX() );
			((CBaseGolem*)(temp))->SetPosY( GetPosY() );
			((CBaseGolem*)(temp))->SetIndexPosX( GetIndexPosX() );
			((CBaseGolem*)(temp))->SetIndexPosY( GetIndexPosY() );
			((CBaseGolem*)(temp))->SetGolemType(SHADOW_GOLEM);

			m_nSpawnedID = MObjectManager::GetInstance()->AddUnitIndexed( temp, 1 );
			break;
			}

		case SPAWNER_PLAYER:
			MMessageSystem::GetInstance()->SendMsg( new msgCreatePlayer( GetIndexPosX(), GetIndexPosY() ) );
			m_nSpawnedID = CGamePlayState::GetInstance()->testVaribale;
			break;
		}
	}


}

void CSpawner::Render(int CameraPosX, int CameraPosY)
{

}


CSpawner::CSpawner(int nSpawnerType) : m_nSpawnerType(nSpawnerType)
{
	m_nSpawnedID = 0;
}

CSpawner::~CSpawner(void)
{

}
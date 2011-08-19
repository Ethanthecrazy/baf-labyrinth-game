#include "CLightEffected.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../GameStates/CGamePlayState.h"
#include "CPit.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/COptionsState.h"
#include "../../../AI Handler/CAI_Handler.h"

CLightEffected::CLightEffected()
{
	CBaseObject::CBaseObject();	
	m_nUnitType = OBJECT_TILE;
	this->m_nImageID = this->floorID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/stoneTile.png" ));
	this->pitIDBad = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pit.png" ));
	this->pitIDGood = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/pitbelow.png" ));
	this->m_nType = OBJ_LIGHTEFFECTED;
	
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	HurtSoundID = FM->LoadSound("resource/Sounds/hurt.mp3");
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(HurtSoundID, true);
	pitTimer = 0.0f;
}
CLightEffected::~CLightEffected()
{
	CBaseObject::~CBaseObject();
}

void CLightEffected::Update(float fDT)
{	
	int lightamount = MObjectManager::GetInstance()->GetLayer( this->GetLayerLocation() ).GetFlake( OBJECT_LIGHT ).GetInfoAtIndex( GetIndexPosX(), GetIndexPosY() );


	if(lightamount < 100)
	{
		pitTimer += fDT;
		if(pitTimer > .15f)
		{
			//Check to see if we are colliding with an entity
			int EntityID = MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetFlake(OBJECT_ENTITY)
				.GetInfoAtIndex(this->GetIndexPosX(), this->GetIndexPosY());

			//we cannot collide with ourselves
			CBaseEntity* pTemp = ((CBaseEntity*)MObjectManager::GetInstance()->GetUnit(EntityID));
			if( EntityID > 0 && pTemp )
			{
				this->CheckCollision( (IUnitInterface*)pTemp, true);
			}
			m_bIsFloor = false;
		}
	}
	else
	{
		m_bIsFloor = true;
		pitTimer = 0.0f;
	}
}

void CLightEffected::Render( int CameraPosX, int CameraPosY )
{
	if(m_bIsFloor)
		m_nImageID = floorID;
	else
	{		
		//if( this->GetLayerLocation() != CGamePlayState::GetInstance()->GetNumLevelFloors() )		
		//	this->m_nImageID = pitIDGood;
		//else
			this->m_nImageID = pitIDBad;
	}
	CBaseObject::Render(CameraPosX, CameraPosY);
}
bool CLightEffected::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{	
	if(!pBase || pBase == this || !nCanHandleCollision || this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	if(!m_bIsFloor)
	{
		/*if( this->GetLayerLocation() != CGamePlayState::GetInstance()->GetNumLevelFloors() )
		{		
			CSGD_FModManager::GetInstance()->PlaySound2D(HurtSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);

			MMessageSystem::GetInstance()->SendMsg( new msgMoveEntityFloor((CBaseEntity*)pBase, 
				this->GetLayerLocation() + 1) );
			MMessageSystem::GetInstance()->ProcessMessages();
			CAI_Handler::GetInstance()->CheckCollisions((CBaseEntity*)pBase, pBase->GetIndexPosX(), pBase->GetIndexPosY(), true);
		}
		else
		{*/
			// kill the entity
			if(pBase->GetType() == ENT_PLAYER)
				CGamePlayState::GetInstance()->KillPlayer();
			else
				MMessageSystem::GetInstance()->SendMsg( new msgRemoveUnit(pBase->m_nIdentificationNumber) );
		//}
	}

	return false;
}
#include "CDoor.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Wrappers/CSGD_FModManager.h"
#include "../../../GameStates/COptionsState.h"
#include "../../../GameStates/CGamePlayState.h"
#include "../../../AI Handler/CAI_Handler.h"
#include "../CBaseEntity.h"
#include "../../MObjectManager.h"

CDoor::CDoor(string nLink)
{
	m_nUnitType = OBJECT_TILE;
	m_nType = OBJ_DOOR;
	m_bIsOpen = false;
	m_nLink = nLink;
	MEventSystem::GetInstance()->RegisterClient("Button.Pressed", this);
	MEventSystem::GetInstance()->RegisterClient("Button.Unpress", this);
	this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Closed.png" ));
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	OpenSoundID = FM->LoadSound("resource/Sounds/Door_Slide.wav");
	//adjust the sounds to match configurations
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(OpenSoundID, true);
}

CDoor::~CDoor(void)
{

}

void CDoor::HandleEvent( Event* _toHandle )
{
	if( _toHandle->GetEventID() == "Button.Pressed" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
		{
			m_bIsOpen = true;
			this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Open.png" ));
			CSGD_FModManager::GetInstance()->PlaySound2D(OpenSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);
		}
	}

	if( _toHandle->GetEventID() == "Button.Unpress" )
	{
		string tmp = (const char*)_toHandle->GetParam();
		if( tmp ==  m_nLink)
		{
			m_bIsOpen = false;
			this->m_nImageID = (CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Door_Closed.png" ));
			CSGD_FModManager::GetInstance()->PlaySound2D(OpenSoundID, CGamePlayState::GetInstance()->testVaribale, this->m_nIdentificationNumber);


			////Check to see if we are colliding with an entity
			//int EntityID = MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetFlake(OBJECT_ENTITY)
			//	.GetInfoAtIndex(this->GetIndexPosX(), this->GetIndexPosY());

			////we cannot collide with ourselves
			//CBaseEntity* pTemp = ((CBaseEntity*)MObjectManager::GetInstance()->GetUnit(EntityID));
			//if( EntityID > 0 && pTemp )
			//{
			//	MObjectManager::GetInstance()->FindLayer(this->m_nIdentificationNumber).GetFlake(OBJECT_ENTITY).FinishMovingEnt(pTemp);
			//	//pTemp->SetFlag_MovementState( FLAG_MOVESTATE_ATDESTINATION );
			//	
			//	switch(pTemp->GetFlag_DirectionToMove())
			//	{
			//	case FLAG_MOVE_RIGHT:
			//		{
			//			CAI_Handler::GetInstance()->CardinalMove(pTemp, FLAG_MOVE_LEFT);
			//		}
			//		break;

			//	case FLAG_MOVE_LEFT:
			//		{
			//			CAI_Handler::GetInstance()->CardinalMove(pTemp, FLAG_MOVE_RIGHT);
			//		}
			//		break;

			//	case FLAG_MOVE_UP:
			//		{
			//			CAI_Handler::GetInstance()->CardinalMove(pTemp, FLAG_MOVE_DOWN);
			//		}
			//		break;

			//	case FLAG_MOVE_DOWN:
			//		{
			//			CAI_Handler::GetInstance()->CardinalMove(pTemp, FLAG_MOVE_UP);
			//		}
			//		break;
			//	}
			//}
		}
	}
}

bool CDoor::CheckCollision(IUnitInterface* pBase)
{
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	if(m_bIsOpen)
	{
		return false;
	}
	else
	{
		if(pBase->GetIndexPosX() == this->GetIndexPosX() && pBase->GetIndexPosY() == this->GetIndexPosY())
		{
			CAI_Handler::GetInstance()->CardinalMove((CBaseEntity*)pBase, ((CBaseEntity*)pBase)->GetFlag_DirectionToMove());
		}
		return true;
	}
}


void CDoor::Render( int CameraPosX, int CameraPosY )
{
	CBaseObject::Render(CameraPosX, CameraPosY);	
}
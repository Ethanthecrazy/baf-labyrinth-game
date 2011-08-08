#include "CGolem_Water.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging/MMessageSystem.h"
#include "../Tiles/CWaterTile.h"
#include "../../MObjectManager.h"
#include "../Tiles/CMetal.h"
#include "../Tiles/CElectricButton.h"
#include "CGolem_Iron.h"


void CGolem_Water::WaterGolemSetup()
{
	SetGolemType(WATER_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/WaterGolem.png" ));
	SetElectricUpdateTimer( 10.0f ) ;
	SetPowered( false ) ;
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Water::CGolem_Water(void)
{
	CBaseGolem::CBaseGolem();
	//basic Water golem setup
	WaterGolemSetup();
}
CGolem_Water::CGolem_Water(CBaseGolem* pGolem)
{
	CBaseGolem::CBaseGolem();
	//basic Water golem setup
	WaterGolemSetup();
	//copy its positions
	this->SetIndexPosX(pGolem->GetIndexPosX());
	this->SetIndexPosY(pGolem->GetIndexPosY());
	this->SetPosX(pGolem->GetPosX());
	this->SetPosY(pGolem->GetPosY());
	this->SetLastPosX(pGolem->GetLastPosX());
	this->SetLastPosY(pGolem->GetLastPosY());
	//copy its states
	this->SetFlag_DirectionToMove(pGolem->GetFlag_DirectionToMove());
	this->SetFlag_MovementState(pGolem->GetFlag_MovementState());
}
CGolem_Water::~CGolem_Water(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
}

void CGolem_Water::Update(float fDT)
{
	CBaseGolem::Update(fDT);
	UpdateAI();

	if( GetFlag_MovementState() == FLAG_MOVESTATE_MOVING )
		SetPowered( false ) ;

	if( GetIsElectrified() )
	{
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
		}
	}
}
void CGolem_Water::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
}
bool CGolem_Water::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Water Golem specific Collisions
	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			if( temp->GetType() == OBJ_WATER )
			{
				if(((CWaterTile*)temp)->IsFrozen())
				{
					//turn me into an ice golem
					MMessageSystem::GetInstance()->SendMsg(new msgChangeGolemType(this, ICE_GOLEM));
				}
				return false;
			}
		}
		break;
	};
	return false;
}
bool CGolem_Water::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Water Golem specific Collisions
	return false;
}
void CGolem_Water::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Water::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Water Golem responds to
}

#include "CGolem_Iron.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../MObjectManager.h"
#include "../Tiles/CMetal.h"
#include "../Tiles/CElectricButton.h"
#include "CGolem_Water.h"

CGolem_Iron::CGolem_Iron(void)
{
	//basic Iron golem setup
	CBaseGolem::CBaseGolem();
	SetGolemType(IRON_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/IronGolem.png" ));
	SetElectricUpdateTimer( 10.0f ) ;
	SetPowered( false ) ;
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Iron::CGolem_Iron(CBaseGolem* pGolem)
{
	//basic Iron golem setup
	CGolem_Iron::CGolem_Iron();
}
CGolem_Iron::~CGolem_Iron(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
}

void CGolem_Iron::Update(float fDT)
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
void CGolem_Iron::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
}
bool CGolem_Iron::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{
	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Iron Golem specific Collisions
	return false;
}
bool CGolem_Iron::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Iron Golem specific Collisions
	return false;
}
void CGolem_Iron::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Iron::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Iron Golem responds to
}
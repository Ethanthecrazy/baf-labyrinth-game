#include "CGolem_Shadow.h"
#include "../../../Wrappers/CSGD_TextureManager.h"
#include "../../../Messaging/MEventSystem.h"
#include "../../../Messaging//MMessageSystem.h"
#include "../CBaseObject.h"
#include "../../MObjectManager.h"

CGolem_Shadow::CGolem_Shadow(void)
{
	//basic Shadow golem setup
	CBaseGolem::CBaseGolem();
	SetGolemType(SHADOW_GOLEM);
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture( "resource/Sprites/Golems/ShadowGolem.png" ));
	//basic golem events
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->RegisterClient("ATTRACTORREMOVED", this);
}
CGolem_Shadow::CGolem_Shadow(CBaseGolem* pGolem)
{
	//basic Shadow golem setup
	CGolem_Shadow::CGolem_Shadow();
}
CGolem_Shadow::~CGolem_Shadow(void)
{
	CBaseGolem::~CBaseGolem();
	//basic golem events
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORPLACED", this);
	MEventSystem::GetInstance()->UnregisterClient("ATTRACTORREMOVED", this);
}

void CGolem_Shadow::Update(float fDT)
{
	DrainLight( 5 );
	CBaseGolem::Update(fDT);
	UpdateAI();
}
void CGolem_Shadow::Render( int CameraPosX, int CameraPosY )
{
	CBaseGolem::Render(CameraPosX, CameraPosY);
}
bool CGolem_Shadow::CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision)
{	
	if(!pBase || pBase == this ||  this->GetLayerLocation() != pBase->GetLayerLocation())
		return false;

	//If the base collides with an object or entity leave
	bool Collided = CBaseGolem::CheckCollision(pBase, nCanHandleCollision);
	if(Collided)
		return Collided;

	//Do Shadow Golem specific Collisions
	switch(pBase->m_nUnitType)
	{
	case OBJECT_OBJECT:
		{
			CBaseObject* temp = (CBaseObject*)pBase;
			
		}
		break;

	case OBJECT_ENTITY:
		{
			//Entities cannot walk-thro other entities
			if(!nCanHandleCollision)
				return true;

			CBaseEntity* temp = (CBaseEntity*)pBase;
			if(temp->GetType() == ENT_GOLEM)
			{
				CBaseGolem* temp = (CBaseGolem*)pBase;
			}
			return true;
		}
		break;
	};
	return false;
}
bool CGolem_Shadow::CheckTileCollision(int TileID)
{
	//If the base collides with a tile leave
	bool Collided = CBaseGolem::CheckTileCollision(TileID);
	if(Collided)
		return Collided;

	//Do Shadow Golem specific Collisions
	return false;
}
void CGolem_Shadow::UpdateAI()
{
	CBaseGolem::UpdateAI();
}
void CGolem_Shadow::HandleEvent( Event* _toHandle )
{
	CBaseGolem::HandleEvent(_toHandle);
	//Events only the Shadow Golem responds to
}

void CGolem_Shadow::DrainLight( int _range )
{
	int top = GetIndexPosY() - _range;
	if( top < 0 )
		top = 0;

	int bottom = GetIndexPosY() + _range;
	if( bottom > MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetLayerHeight() )
		bottom = MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetLayerHeight();

	int left = GetIndexPosX() - _range;
	if( left < 0 )
		left = 0;

	int right = GetIndexPosX() + _range;
	if( right > MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetLayerWidth() )
		right = MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetLayerWidth();

	//_range /= 2;

	for( int y = top; y <= bottom; ++y )
	{
		for( int x = left; x <= right; ++x )
		{
			float distance = 0;

			int xDistance = ( GetIndexPosX() - x );
			int yDistance = ( GetIndexPosY() - y );
			distance = sqrtf( float( xDistance * xDistance ) + float( yDistance * yDistance ) );
			distance = distance / (float)_range;
			if( distance > 1.0f )
				distance = 1.0f;

			float newValue = 255 - ( distance * 255 );
			
			MFlake* FlakeScope = &MObjectManager::GetInstance()->FindLayer( m_nIdentificationNumber ).GetFlake( OBJECT_LIGHT ); 

			FlakeScope->SetInfoAtIndex( x, y, 
				FlakeScope->GetInfoAtIndex( x, y ) - newValue );
				//newValue );

			if( FlakeScope->GetInfoAtIndex( x, y ) < 0 )
				FlakeScope->SetInfoAtIndex( x, y, 0 );



		} 
	}


}
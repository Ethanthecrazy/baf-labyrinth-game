#ifndef IUNITINTERFACE_H
#define IUNITINTERFACE_H

//the type of object
enum UNIT_TYPES { OBJECT_TILE, OBJECT_OBJECT, OBJECT_ENTITY, OBJECT_LIGHT };
enum { FLAG_MOVE_NONE, FLAG_MOVE_UP, FLAG_MOVE_DOWN, FLAG_MOVE_LEFT, FLAG_MOVE_RIGHT };
enum { FLAG_MOVESTATE_MOVING, FLAG_MOVESTATE_ATDESTINATION };
//huge list of types
enum OBJECT_TYPES {ENT_PLAYER, ENT_GOLEM, OBJ_EXIT , OBJ_SPAWNER,
	               OBJ_DOOR , OBJ_BUTTON , OBJ_ATTRACTOR,
				   OBJ_POWERGLOVES , OBJ_OILCAN , OBJ_OIL,
				   OBJ_WATER, OBJ_LIGHTORB, OBJ_PIT, OBJ_RAMP,
					OBJ_METAL , OBJ_ELECTRICGENERATOR , OBJ_ELECTRICBUTTON,
					OBJ_BREAKABLEWALL};
//all of our golems
enum GOLEM_TYPES { EARTH_GOLEM, FIRE_GOLEM, WATER_GOLEM, 
	 ICE_GOLEM, AIR_GOLEM, LAVA_GOLEM, IRON_GOLEM, 
	 SHADOW_GOLEM, LIGHT_GOLEM };

class IUnitInterface
{

private:
		// position
	float m_fPosX;
	float m_fPosY;

	float m_fLastPosX;
	float m_fLastPosY;

	int m_nIndexLocationX;
	int m_nIndexLocationY;

	int m_nLayerLocation;

	float m_fVelX;
	float m_fVelY;

	float m_fDistanceLeft;

	int FLAG_DirectionToMove;
	int	FLAG_MovementState;
	int	FLAG_prev_DirectionToMove;
	int FLAG_prev_MovementState;

public:

	// The type of Unit
	int m_nUnitType;

	// The identification number used to reference the object in the Manager.
	int m_nIdentificationNumber; 

	virtual ~IUnitInterface(void) { }
	IUnitInterface() : FLAG_DirectionToMove( 0 ), FLAG_MovementState( 0 ), FLAG_prev_DirectionToMove( 0 ), FLAG_prev_MovementState( 0 ) {}

	virtual void Update(float fDT) = 0;
	virtual void Render( int CameraPosX, int CameraPosY ) = 0;

	virtual void AddRef(void) = 0;
	virtual void Release(void) = 0;

	virtual int GetType(void) = 0;

	virtual bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision) = 0;
	virtual void ExitCollision(IUnitInterface* pBase, bool nCanHandleCollision) = 0;
	virtual bool CanInteract(IUnitInterface* pBase) = 0;

		// accessors
	float GetPosX(void) { return m_fPosX; }
	float GetPosY(void) { return m_fPosY; }
	float GetLastPosX(void) { return m_fLastPosX; }
	float GetLastPosY(void) { return m_fLastPosY; }
	float GetVelX(void) { return m_fVelX; }
	float GetVelY(void) { return m_fVelY; }
	int GetIndexPosX(void) { return m_nIndexLocationX; }
	int GetIndexPosY(void) { return m_nIndexLocationY; }
	int GetLayerLocation(void) { return m_nLayerLocation; }
	float GetDistanceLeft(void){ return m_fDistanceLeft; }
	
	// modifiers
	void SetPosX(float nPosX) { m_fPosX = nPosX; }
	void SetPosY(float nPosY) { m_fPosY = nPosY; }
	void SetLastPosX(float nPosX) { m_fLastPosX = nPosX; }
	void SetLastPosY(float nPosY) { m_fLastPosY = nPosY; }
	void SetVelX(float nVelX) { m_fVelX = nVelX; }
	void SetVelY(float nVelY) { m_fVelY = nVelY; }
	void SetIndexPosX(int nPosX) { m_nIndexLocationX = nPosX; }
	void SetIndexPosY(int nPosY) { m_nIndexLocationY = nPosY; }
	void SetLayerLocation( int nLayer ) { m_nLayerLocation = nLayer; }
	void SetDistanceLeft( float _distance ) { m_fDistanceLeft = _distance; }

	int GetFlag_DirectionToMove( void ){ return FLAG_DirectionToMove; }
	int GetFlag_MovementState( void ){ return FLAG_MovementState; }
	int GetFlag_prev_DirectionToMove( void ){ return FLAG_prev_DirectionToMove; }
	int GetFlag_prev_MovementState( void ){ return FLAG_MovementState; }

	virtual void SetFlag_DirectionToMove( int newFlag ){ FLAG_prev_DirectionToMove = FLAG_DirectionToMove; FLAG_DirectionToMove = newFlag; }
	virtual void SetFlag_MovementState( int newFlag ){ FLAG_prev_MovementState = FLAG_MovementState; FLAG_MovementState = newFlag; }
};

#endif
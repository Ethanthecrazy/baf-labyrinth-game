#ifndef CAI_HANDLER
#define CAI_HANDLER

class CBaseEntity;

class CAI_Handler
{

	//singleton
	CAI_Handler();
	CAI_Handler(const CAI_Handler&) { }
	~CAI_Handler();
	//helpers
	bool HorizontalMove(const CBaseEntity* pEntity, const int nTargetX);
	bool VerticalMove(const CBaseEntity* pEntity, const int nTargetY);
    void GetNewTarget(const CBaseEntity* pEntity, const int nDirection);
public:

	static CAI_Handler* GetInstance();
	//Checks to see if an object is in that position
	//bool- determines weather to allow the Entity to handle
	// its own collisions
	bool CheckCollisions(const CBaseEntity* pEntity, const int nX, 
		const int nY, bool nCanHandleCollision);
	bool CardinalMove(const CBaseEntity* pEntity, const int nDirection);
	void MoveToPos(const CBaseEntity* pEntity);
	void RandomMove(const CBaseEntity* pEntity);
};
#endif
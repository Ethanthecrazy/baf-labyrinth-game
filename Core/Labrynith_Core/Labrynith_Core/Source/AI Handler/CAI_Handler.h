#ifndef CAI_HANDLER
#define CAI_HANDLER

class CBaseGolem;

class CAI_Handler
{

	//singleton
	CAI_Handler();
	CAI_Handler(const CAI_Handler&) { }
	~CAI_Handler();
	//helpers
	bool HorizontalMove(const CBaseGolem* pGolem, const int nTargetX);
	bool VerticalMove(const CBaseGolem* pGolem, const int nTargetY);
public:

	static CAI_Handler* GetInstance();
	//Checks to see if an object is in that position
	//bool- determines weather to allow the Golem to handle
	// its own collisions
	bool CheckCollisions(const CBaseGolem* pGolem, const int nX, 
		const int nY, bool nCanHandleCollision);
	bool CardinalMove(const CBaseGolem* pGolem, const int nDirection);
	void MoveToPos(const CBaseGolem* pGolem);
	void RandomMove(const CBaseGolem* pGolem);
	void GetNewTarget(const CBaseGolem* pGolem, const int nDirection);
};
#endif
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
	
public:

	static CAI_Handler* GetInstance();
	void MoveToPos(CBaseGolem* pGolem);
	void RandomMove(CBaseGolem* pGolem);
	//excutes the ai process
	void ExecuteAI(CBaseGolem* pGolem);
};
#endif
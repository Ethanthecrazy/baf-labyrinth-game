#ifndef CEXIT
#define CEXIT

#include "../CBaseObject.h"

class CExit : public CBaseObject
{

public:
	CExit();
	~CExit();

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
};
#endif
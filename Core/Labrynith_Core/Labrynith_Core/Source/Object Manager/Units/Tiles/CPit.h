#ifndef CPIT_H_
#define CPIT_H_

#include "../CBaseObject.h"

class CPit : public CBaseObject
{
	int HurtSoundID;
public:
	CPit(void);

	void Render( int CameraPosX, int CameraPosY );

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
};
#endif
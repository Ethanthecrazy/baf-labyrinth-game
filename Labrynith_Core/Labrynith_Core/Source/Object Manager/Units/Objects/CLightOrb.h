#ifndef CLIGHTORB_H_
#define CLIGHTORB_H_

#include "../CBaseObject.h"

class CLightOrb : public CBaseObject
{
private:

public:
	CLightOrb(void);

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
};

#endif
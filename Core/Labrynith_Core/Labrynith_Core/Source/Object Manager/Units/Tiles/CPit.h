#ifndef CPIT_H_
#define CPIT_H_

#include "../CBaseObject.h"

class CPit : public CBaseObject
{

public:
	CPit(void);

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
};
#endif
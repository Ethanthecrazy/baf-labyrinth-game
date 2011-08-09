#ifndef CRAMP_H_
#define CRAMP_H_

#include "../CBaseObject.h"

#include <string>
using std::string;

class CRamp : public CBaseObject
{
	bool Up;
public:
	CRamp(string direction = "");

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
};
#endif
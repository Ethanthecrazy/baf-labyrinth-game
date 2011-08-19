#ifndef CHELPTILE_H_
#define CHELPTILE_H_

#include "../CBaseObject.h"
#include <string>
using std::string;

class CHelpTile : public CBaseObject
{
	string m_sHelpText;

public:
	CHelpTile(string sHelpText);

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);

	string GetHelpText() { return m_sHelpText; }
};
#endif
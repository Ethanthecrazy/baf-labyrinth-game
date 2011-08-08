#ifndef CRAMP_H_
#define CRAMP_H_

#include "../CBaseObject.h"
#include "../../../Messaging/IListener.h"

#include <string>
using std::string;

class CRamp : public CBaseObject, public IListener
{
	string m_nLink;
public:
	CRamp(string nLink = "");

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	void HandleEvent( Event* _toHandle );

	string GetLink(void) { return m_nLink; }
};
#endif
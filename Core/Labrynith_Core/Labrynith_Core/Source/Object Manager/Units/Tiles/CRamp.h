#ifndef CRAMP_H_
#define CRAMP_H_

#include "../CBaseObject.h"
#include "../../../Messaging/IListener.h"

class CRamp : public CBaseObject, public IListener
{
	int m_nLink;
public:
	CRamp(int nLink = -1);

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	void HandleEvent( Event* _toHandle );

	int GetLink(void) { return m_nLink; }
};
#endif
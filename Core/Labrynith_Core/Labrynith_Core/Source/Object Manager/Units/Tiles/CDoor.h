#ifndef CDOOR_H_
#define CDOOR_H_

#include "../../../Messaging/IListener.h"
#include "../CBaseEntity.h"

#include <string>
using std::string;

class CDoor : public CBaseEntity, public IListener
{
private:
	bool m_bIsOpen;
	string m_nLink;

public:
	CDoor(string nLink = "-1");
	~CDoor(void);

	void HandleEvent( Event* _toHandle );
	
	bool CheckCollision(IUnitInterface* pBase);
	
	void Render( int CameraPosX, int CameraPosY );
};



#endif
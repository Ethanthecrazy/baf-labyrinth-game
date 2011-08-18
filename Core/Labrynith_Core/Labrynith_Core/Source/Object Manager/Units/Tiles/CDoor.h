#ifndef CDOOR_H_
#define CDOOR_H_

#include "../../../Messaging/IListener.h"
#include "../CBaseObject.h"

#include <string>
using std::string;

class CDoor : public CBaseObject, public IListener
{
private:
	int OpenSoundID;
	string m_nLink;
	bool m_bIsOpen;

public:

	CDoor(string nLink = "-1");
	~CDoor(void);

	void HandleEvent( Event* _toHandle );
	
	bool CheckCollision(IUnitInterface* pBase);
	
	void Render( int CameraPosX, int CameraPosY );
	void Update(float fDT) {}

	bool GetIsOpen(void) {return m_bIsOpen;}
};



#endif
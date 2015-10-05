#ifndef MMESSAGESYSTEM_H
#define MMESSAGESYSTEM_H

#include "Messages.h"
#include <queue>
using namespace std;

typedef void (*MESSAGEPROC)(CBaseMessage*);

class MMessageSystem
{

	queue<CBaseMessage*> m_MsgQueue;
	MESSAGEPROC m_pfnMsgProc;

	MMessageSystem() { m_pfnMsgProc = NULL;	}
	MMessageSystem(const MMessageSystem&);
	MMessageSystem& operator=(const MMessageSystem&);

	~MMessageSystem() {}

public:

	static MMessageSystem* GetInstance( void );

	void InitMessageSystem( MESSAGEPROC pfnMsgProc );

	void SendMsg( CBaseMessage* pMsg );

	void ProcessMessages( void );

	void ClearMessages( void );

	void ShutdownMessageSystem( void );

};

#endif
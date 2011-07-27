//	MEventSystem.h
//	Written by : Ethan Pendergraft
//	Based on code by David Brown

#ifndef MEVENTSYSTEM_H
#define MEVENTSYSTEM_H

#include <list>
#include <map>
using namespace std;

#include "IListener.h"

class MEventSystem
{
	multimap< string, IListener*> m_mmClients;

	list< Event > m_lEvents;

	void DispatchEvent( Event* _toDispatch );
	bool AlreadyRegistered( string eventID, IListener* pClient );

	MEventSystem() {}
	MEventSystem(const MEventSystem&);
	MEventSystem& operator=(const MEventSystem&);
	~MEventSystem() {}

public:

	static MEventSystem* GetInstance( void )
	{
		static MEventSystem instance;
		return &instance;
	}

	void RegisterClient(string eventID, IListener* pClient);

	void UnregisterClient(string eventID, IListener* pClient);

	void UnregisterAllClients(IListener* pClient);

	void SendEvent(string eventID, void* pData = NULL);

	void ProcessEvents(void);

	void ClearEvents(void);

	void ShutdownEventSystem(void);

};

#endif
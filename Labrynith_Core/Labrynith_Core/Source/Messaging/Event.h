#ifndef EVENT_H_
#define EVENT_H_

#include <string>
using namespace std;

class Event
{
	string m_sEventID;
	void* m_pParameter;

public:

	Event( string _eventID, void* _param = NULL )
	{
		m_sEventID = _eventID;
		m_pParameter = _param;
	}

	~Event() 
	{
	}

	string GetEventID( void ) { return m_sEventID; }
	void* GetParam( void ) { return m_pParameter; }
};

#endif
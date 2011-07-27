#include "MEventSystem.h"

void MEventSystem::RegisterClient(string eventID, IListener* pClient)
{
	if( !pClient || AlreadyRegistered( eventID, pClient ) )
		return;

	m_mmClients.insert( make_pair( eventID, pClient ) );
}

void MEventSystem::UnregisterClient(string eventID, IListener* pClient)
{
	pair< multimap< string, IListener* >::iterator,
		multimap< string, IListener* >::iterator > range;

	range = m_mmClients.equal_range( eventID );


	for( multimap< string, IListener* >::iterator mmIter = range.first; mmIter != range.second; mmIter++ )
	{
		if( (*mmIter).second == pClient )
		{
			mmIter = m_mmClients.erase( mmIter );
			break;
		}
	}
}

void MEventSystem::UnregisterAllClients(IListener* pClient)
{
	multimap< string, IListener* >::iterator mmIter = m_mmClients.begin();

	while( mmIter != m_mmClients.end() )
	{
		if( (*mmIter).second == pClient )
		{
			mmIter = m_mmClients.erase( mmIter );
		}
		else
			mmIter++;
	}
}

void MEventSystem::SendEvent(string eventID, void* pData )
{
	Event newEvent( eventID, pData );	
	m_lEvents.push_back( newEvent );
}

void MEventSystem::ProcessEvents(void)
{
	while( m_lEvents.size() )
	{
		DispatchEvent( &m_lEvents.front() );
		m_lEvents.pop_front();
	}

}

void MEventSystem::ClearEvents(void)
{
	m_lEvents.clear();
}

void MEventSystem::ShutdownEventSystem(void)
{
	m_mmClients.clear();
}

void MEventSystem::DispatchEvent( Event* _toDispatch )
{
	pair< multimap< string, IListener* >::iterator,
		multimap< string, IListener* >::iterator > range;

	range = m_mmClients.equal_range( _toDispatch->GetEventID() );

	for( multimap< string, IListener* >::iterator mmIter = range.first; mmIter != range.second; mmIter++ )
	{
		(*mmIter).second->HandleEvent( _toDispatch );
	}
}

bool MEventSystem::AlreadyRegistered( string eventID, IListener* pClient )
{
	bool bIsAlreadyRegistered = false;

	pair<multimap<string, IListener*>::iterator,
		multimap<string, IListener*>::iterator> range;

	range = m_mmClients.equal_range( eventID );

	for( multimap<string, IListener*>::iterator mmIter = range.first; mmIter != range.second; mmIter++ )
	{
		if( (*mmIter).second == pClient )
		{
			bIsAlreadyRegistered = true;
			break;
		}
	}

	return bIsAlreadyRegistered;
}
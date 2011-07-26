#include "MSyncManager.h"

MSyncManager* MSyncManager::GetInstance( void )
{
	static MSyncManager* instance = new MSyncManager();
	return instance;
}

void MSyncManager::InitSyncSystem()
{
	InitializeCriticalSection( &ObjectProtection );
	EnterCriticalSection( &ObjectProtection );
	Objects.clear();
	LeaveCriticalSection( &ObjectProtection );
}

int MSyncManager::CreateLockObject( void )
{
	EnterCriticalSection( &ObjectProtection );

	SyncObject temp;
	InitializeCriticalSection( &temp.mutex );
	temp.ObjectType = false;
	Objects.push_back( temp );

	int returnValue = Objects.size() - 1;

	LeaveCriticalSection( &ObjectProtection );

	return returnValue;
}

int MSyncManager::CreateChangeObject( void )
{
	EnterCriticalSection( &ObjectProtection );

	SyncObject temp;
	InitializeCriticalSection( &temp.mutex );
	InitializeConditionVariable( &temp.changeVariable );
	temp.ObjectType = false;
	Objects.push_back( temp );

	int returnValue = Objects.size() - 1;

	LeaveCriticalSection( &ObjectProtection );
	return returnValue;
}

void MSyncManager::LockIt( int _toLock )
{
	EnterCriticalSection( &Objects[_toLock].mutex );
}
void MSyncManager::UnlockIt( int _toUnLock )
{
	LeaveCriticalSection( &Objects[_toUnLock].mutex );
}

void MSyncManager::WaitChange( int _toWait )
{
	SleepConditionVariableCS( &Objects[_toWait].changeVariable, &Objects[_toWait].mutex, INFINITE );
}
void MSyncManager::WaitChangeTimed( int _toWait, int _waitTime )
{
	SleepConditionVariableCS( &Objects[_toWait].changeVariable, &Objects[_toWait].mutex, _waitTime );
}

void MSyncManager::SendChangeWakeup( int _toSend )
{
	WakeConditionVariable( &Objects[_toSend].changeVariable );
}

void MSyncManager::BroadcastChangeWakeup( int _toBroadcast )
{
	WakeAllConditionVariable( &Objects[_toBroadcast].changeVariable );
}

void* MSyncManager::StartThread( pfThread_t threadFunc, void *arg )
{
	unsigned int loveME;  
	HANDLE toReturn = (HANDLE)_beginthreadex( NULL, NULL, threadFunc, arg, 0, &loveME );
	return toReturn;
}
void* MSyncManager::LeaveThread( void *rtn_value )
{
	_endthreadex( (unsigned int)rtn_value );

	return rtn_value;
}
void MSyncManager::FreeThread( void **SelectedThread )
{
	CloseHandle( *SelectedThread );
	*SelectedThread = NULL;
}

void MSyncManager::CloseSyncSystem()
{
	EnterCriticalSection( &ObjectProtection );

	for( int i = 0; i < Objects.size(); ++i )
	{
		DeleteCriticalSection( &Objects[i].mutex );
	}

	Objects.clear();

	LeaveCriticalSection( &ObjectProtection );

	DeleteCriticalSection( &ObjectProtection );
}

void* MSyncManager::GetMyThreadID( void )
{
	return (void*)GetCurrentThreadId(); 
}



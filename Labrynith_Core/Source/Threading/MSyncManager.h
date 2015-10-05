
#include <Windows.h>
#include <process.h>
#include <vector>
using namespace std;


typedef unsigned int ( __stdcall *pfThread_t )( void* );

class  MSyncManager
{
	struct SyncObject
	{
		bool ObjectType;

		CRITICAL_SECTION mutex;
		CONDITION_VARIABLE changeVariable;
	};

	CRITICAL_SECTION ObjectProtection;
	vector< SyncObject > Objects;

	MSyncManager() {}
	MSyncManager(const MSyncManager&);
	MSyncManager& operator=(const MSyncManager&);
	~MSyncManager();

public:

	static MSyncManager* GetInstance( void );
	
	void InitSyncSystem( void );
	int CreateLockObject( void );
	int CreateChangeObject( void );

	void LockIt( int _toLock );
	void UnlockIt( int _toUnLock );

	void WaitChange( int _toWait ); 
	void WaitChangeTimed( int _toWait, int _waitTime );

	void SendChangeWakeup( int _toSend );
	void BroadcastChangeWakeup( int _toBroadcast );

	void* StartThread( pfThread_t threadFunc, void *arg );
	void* LeaveThread( void *rtn_value );
	void FreeThread( void **SelectedThread );

	void CloseSyncSystem();

	void* GetMyThreadID( void );

};
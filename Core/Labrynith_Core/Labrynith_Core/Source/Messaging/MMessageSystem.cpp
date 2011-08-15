#include "MMessageSystem.h"

MMessageSystem* MMessageSystem::GetInstance( void )
{
	static MMessageSystem instance;
	return &instance;
}

void MMessageSystem::InitMessageSystem( MESSAGEPROC pfnMsgProc )
{
	if( !pfnMsgProc )
		return;

	m_pfnMsgProc = pfnMsgProc;
}

void MMessageSystem::SendMsg( CBaseMessage* pMsg )
{
	if( !pMsg )
		return;

	m_MsgQueue.push(pMsg);
}

void MMessageSystem::ProcessMessages( void )
{
	if( !m_pfnMsgProc )
		return;

	while( !m_MsgQueue.empty() )
	{		
		m_pfnMsgProc( m_MsgQueue.front() );		
		delete m_MsgQueue.front();
		m_MsgQueue.pop();
	}
}

void MMessageSystem::ClearMessages( void )
{
	while( !m_MsgQueue.empty() )
	{
		delete m_MsgQueue.front();
		m_MsgQueue.pop();
	}
}

void MMessageSystem::ShutdownMessageSystem( void )
{
	ClearMessages();

	m_pfnMsgProc = NULL;
}
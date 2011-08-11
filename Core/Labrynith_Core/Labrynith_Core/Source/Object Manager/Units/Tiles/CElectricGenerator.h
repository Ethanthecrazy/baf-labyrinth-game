#ifndef CELECTRICGENERATOR_H
#define CELECTRICGENERATOR_H

#include "../CBaseObject.h"
#include "../../../Messaging/MEventSystem.h"
#include <vector>

class CElectricGenerator: public CBaseObject , public IListener
{
private:
	bool m_bGolemConnected ;
	bool m_bEvaluateConnections ;
	vector<IUnitInterface*> m_EvaluatedConnections ;
	int m_nAnimID ;
	int m_nAnimImageID ;
	float m_nElectricUpdateTimer ;
public:
	CElectricGenerator() ;
	~CElectricGenerator() ;

	void Update(float fDT ) ;
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	bool CanInteract(IUnitInterface* pBase);

	void HandleEvent( Event* _toHandle );

	bool MakeConnections( IUnitInterface* obj ) ;

	bool FindConnection( IUnitInterface* obj ) ;

	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
} ;


#endif
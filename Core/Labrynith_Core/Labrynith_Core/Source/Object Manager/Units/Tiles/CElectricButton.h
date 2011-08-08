
#ifndef CELECTRICBUTTON_H
#define CELECTRICBUTTON_H

#include "CButton.h"

class CElectricButton : public CButton
{
private:
	bool m_bPowered ;
	float m_nElectricUpdateTimer ;
public:
	CElectricButton(string nLink = "-5234");
	~CElectricButton(void);

	void Update(float fDT) ;

	bool CheckCollision(IUnitInterface* pBase);

	bool CheckConnection( void ) ;

	void HandleEvent( Event* _toHandle );

	bool GetPowered( void ) { return m_bPowered ; } ;
	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetPowered( bool powered ) { m_bPowered = powered ; } ;
	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
} ;

#endif

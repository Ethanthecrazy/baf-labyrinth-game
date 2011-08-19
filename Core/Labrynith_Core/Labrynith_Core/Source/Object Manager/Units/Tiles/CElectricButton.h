
#ifndef CELECTRICBUTTON_H
#define CELECTRICBUTTON_H

#include "CButton.h"

class CElectricButton : public CButton
{
private:
	int m_nAnimID ;
	int m_nAnimImageID ;
	int m_nSoundID ;
	bool m_bPowered ;
	bool m_bGolemPowered ;
	float m_nElectricUpdateTimer ;

	int m_nIMG_Powered;

public:
	CElectricButton(string nLink = "-5234");
	~CElectricButton(void);

	void Update(float fDT) ;
	void Render( int CameraPosX, int CameraPosY );

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);

	bool CheckConnection( void ) ;

	void HandleEvent( Event* _toHandle );

	bool GetIsElectrified( void ) { return m_bPowered ; } ;
	bool GetIsGolemPowered( void ) { return m_bGolemPowered ; } ;
	float GetElectricUpdateTimer( void ) { return m_nElectricUpdateTimer ; } ;

	void SetElectricUpdateTimer( float time ) { m_nElectricUpdateTimer = time ; } ;
	void SetPowered( bool powered ) ;
	void SetGolemPowered( bool powered ) { m_bGolemPowered = powered ; } ;
} ;

#endif

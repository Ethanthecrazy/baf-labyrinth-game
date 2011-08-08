#ifndef CBUTTON_H_
#define CBUTTON_H_

#include "../CBaseObject.h"
#include "../../../Messaging/IListener.h"

#include <string>
using std::string;

class CButton : public CBaseObject, public IListener
{
private:
	bool m_bIsElectrified ;
	bool m_bIsPressed;
	string m_nLink;

public:
	CButton(string nLink = "-5234");
	~CButton(void);
	
	bool CheckCollision(IUnitInterface* pBase);
	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );

	void HandleEvent( Event* _toHandle );

	//	accessors
	bool GetIsElectrified( void ) { return m_bIsElectrified ; } ;
	bool GetIsPressed( void ) { return m_bIsPressed ; } ;
	string GetLink( void ) { return m_nLink ; } ;

	//	mutators
	void SetIsElectrified( bool electrified ) { m_bIsElectrified = electrified ; } ;
	void SetPressed( bool pressed ) { m_bIsPressed = pressed ; } ;
	void SetLink( string link ) { m_nLink = link ; } ;
};



#endif
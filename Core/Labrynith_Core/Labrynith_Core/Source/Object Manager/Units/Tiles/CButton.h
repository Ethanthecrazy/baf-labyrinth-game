#ifndef CBUTTON_H_
#define CBUTTON_H_

#include "../CBaseObject.h"
#include "../../../Messaging/IListener.h"

#include <string>
using std::string;

class CButton : public CBaseObject, public IListener
{
private:
	
	string m_nLink;
	int OpenSoundID, CloseSoundID;

protected:

	bool m_bIsPressed;
	int m_nIMG_Up;
	int m_nIMG_Down;

public:
	CButton(string nLink = "-5234");
	~CButton(void);
	
	virtual bool CheckCollision(IUnitInterface* pBase);
	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );

	virtual void HandleEvent( Event* _toHandle );

	//	accessors
	bool GetIsPressed( void ) { return m_bIsPressed ; } ;
	const char* GetLink( void ) { return m_nLink.c_str() ; } ;

	//	mutators
	void SetPressed( bool pressed ) { m_bIsPressed = pressed ; } ;
	void SetLink( string link ) { m_nLink = link ; } ;
};



#endif
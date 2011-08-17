#ifndef CSTEAMPUFF_H_
#define CSTEAMPUFF_H_

#include "../CBaseObject.h"

class CSteamPuff : public CBaseObject
{

private:

	int ID_ANI;

public:
	
	CSteamPuff();

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );

};

#endif
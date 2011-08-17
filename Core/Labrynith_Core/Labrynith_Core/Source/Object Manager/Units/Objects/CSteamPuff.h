#ifndef CSTEAMPUFF_H_
#define CSTEAMPUFF_H_

#include "../CBaseObject.h"

class CSteamPuff : public CBaseObject
{

private:

	float scale;

public:
	
	int ID_ANI;

	CSteamPuff();

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );

	void MakeIce();

};

#endif
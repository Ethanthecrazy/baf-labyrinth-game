#ifndef CLIGHTEFFECTED_H_
#define CLIGHTEFFECTED_H_

#include "../CBaseObject.h"

class CLightEffected : public CBaseObject
{
private:
	bool m_bIsFloor;
	int floorID;
	int pitIDGood;
	int pitIDBad;
	int HurtSoundID;

	float pitTimer;
public:

	CLightEffected(void);
	~CLightEffected(void);
	
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	
	void Render( int CameraPosX, int CameraPosY );
	void Update(float fDT);

	bool GetIsFloor(void) {return m_bIsFloor;}
};



#endif
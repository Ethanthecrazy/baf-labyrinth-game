#ifndef CBASEOBJECT_H
#define CBASEOBJECT_H_

#include "../IUnitInterface.h"

enum OBJECT_TYPE{ attractor } ;

class CBaseObject : public IUnitInterface
{
	unsigned int m_uiRefCount;

protected:
	
	int m_nType;

public:

	int m_nImageID;

	CBaseObject(void);
	virtual ~CBaseObject(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );

	int GetType(void) { return m_nType; }

	virtual bool CheckCollision(IUnitInterface* pBase);

	void AddRef(void);
	void Release(void);

};
#endif

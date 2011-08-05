#ifndef CBASEOBJECT_H
#define CBASEOBJECT_H

#include "../IUnitInterface.h"


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
	virtual void UseObject( CBaseObject* user ) ;

	int GetType(void) { return m_nType; }
	void SetObjectType(int type) { m_nType = type ; } ;

	virtual bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);

	void AddRef(void);
	void Release(void);

};
#endif

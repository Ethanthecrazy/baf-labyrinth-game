#ifndef CBaseEntity_h_
#define CBaseEntity_h_

#include "../IUnitInterface.h"

class CBaseEntity : public IUnitInterface
{


	unsigned int m_uiRefCount;

protected:
	int m_nType;

public:

	int m_nImageID;

	CBaseEntity(void);
	virtual ~CBaseEntity(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );

	int GetType(void) { return m_nType; }

	virtual bool CheckCollision(IUnitInterface* pBase);

	void AddRef(void);
	void Release(void);

};

#endif // CBase_h__
#ifndef CBASEGOLEM_H_
#define CBASEGOLEM_H_

#include "CBaseEntity.h"

//enum GOLEMTYPES{ }
class CBaseGolem : public CBaseEntity
{

	int m_nGolemType;
public:
	CBaseGolem(void);
	virtual ~CBaseGolem(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );
	virtual void UpdateAI();

	int GetGolemType() const;
	void SetGolemType(const int nGolemType);
};
#endif
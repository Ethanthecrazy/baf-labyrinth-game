#ifndef CBaseEntity_h_
#define CBaseEntity_h_

#include "../IUnitInterface.h"
#include <vector>

class CBaseEntity : public IUnitInterface
{


	unsigned int m_uiRefCount;
	int m_nCurrAnimID;
	std::vector<int> m_vMovementAnimIDs;
	bool PlayAnimWhileStill;
protected:
	int m_nType;
	
public:

	int m_nImageID;

	CBaseEntity(void);
	virtual ~CBaseEntity(void);

	virtual void Update(float fDT);
	virtual void Render( int CameraPosX, int CameraPosY );

	//loads the animations for basic movement
	void LoadEntMoveAnimIDs();

	int GetType(void) { return m_nType; }
	int GetCurrentAnimID(void) const { return m_nCurrAnimID; }

	void SetAnimID(const int nAnimID);
	void SetImageID(const int nImageID);
	void SetPlayAnimWhileStill(const bool bCanAnim)
	{PlayAnimWhileStill = bCanAnim;}

	virtual bool CheckCollision(IUnitInterface* pBase);

	void AddRef(void);
	void Release(void);

};

#endif // CBase_h__
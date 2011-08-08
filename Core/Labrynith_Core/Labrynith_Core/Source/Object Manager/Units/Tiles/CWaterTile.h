#ifndef CWATERTILE
#define CWATERTILE

#include "../CBaseObject.h"

class CWaterTile : public CBaseObject
{
	int m_nWaterImageID;
	int m_nIceImageID;
	bool m_bIsFrozen;
public:
	CWaterTile();
	CWaterTile(bool bIsFrozen);
	~CWaterTile();

	void Update(float fDT);
	void Render( int CameraPosX, int CameraPosY );
	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	//accessors
	bool IsFrozen();
	//mutators
	void SetIsFrozen(const bool bIsFrozen);
};
#endif
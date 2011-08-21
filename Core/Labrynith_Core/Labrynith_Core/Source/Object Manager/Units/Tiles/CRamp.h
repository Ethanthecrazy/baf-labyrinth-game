#ifndef CRAMP_H_
#define CRAMP_H_

#include "../CBaseObject.h"

#include <string>
using std::string;

class CRamp : public CBaseObject
{
	bool Up;
	int floorlevel;
	int m_nLinkPosX, m_nLinkPosY;
	int m_nMoveFloorSoundID;
public:
	CRamp(std::string direction = "", int nFloorlevel = -1);

	bool CheckCollision(IUnitInterface* pBase, bool nCanHandleCollision);
	void SetLinkX(const int nPosX);
	void SetLinkY(const int nPosY);
	int GetLinkX() const;
	int GetLinkY() const;
};
#endif
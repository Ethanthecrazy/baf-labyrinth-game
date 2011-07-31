#ifndef CFRAME_H_
#define CFRAME_H_

#include "precompiled_header.h"

class CFrame
{
	float m_fDuration;
	std::string m_szTriggerID;
	RECT m_rDrawRect, m_rCollisionRect;
	int m_nAnchorX, m_nAnchorY;
public:
	CFrame();
	CFrame(const CFrame* frame);
	CFrame(float fDuration, std::string szTriggerID, 
		   int nAnchorX, int nAnchorY, RECT rDrawRect,
		   RECT rCollisionRect);
	CFrame(float fDuration, std::string szTriggerID, 
		   int nAnchorX, int nAnchorY,
		   int nDTop, int nDLeft, int nDBottom, int nDRight,
		   int nCTop, int nCLeft, int nCBottom, int nCRight);
	~CFrame();

	friend ostream& operator<<(ostream& os, const CFrame& frame);

	//accessors
	float GetDuration() const;
	std::string GetTriggerID() const;
	RECT GetDrawRect() const;
	RECT GetCollisionRect() const;
	int GetAnchorX() const;
	int GetAnchorY() const;
	//mutators
	void SetDuration(const float fDuration);
	void SetTrigger(const std::string szTriggerID);
	void SetAnchorX(const int nAnchorX);
	void SetAnchorY(const int nAnchorY);
	void SetAnchor(const int nAnchorX, const int nAnchorY);
	void SetDrawRectTop(const int nTop);
	void SetDrawRectBottom(const int nBottom);
	void SetDrawRectLeft(const int nLeft);
	void SetDrawRectRight(const int nRight);
	void SetDrawRect(const RECT rDrawRect);
	void SetDrawRect(const int nTop, const int nLeft,
		const int nBottom, const int nRight);	
	void SetCollisionRectTop(const int nTop);
	void SetCollisionRectBottom(const int nBottom);
	void SetCollisionRectLeft(const int nLeft);
	void SetCollisionRectRight(const int nRight);
	void SetCollisionRect(const RECT rCollisionRect);
	void SetCollisionRect(const int nTop, const int nLeft,
		const int nBottom, const int nRight);
	
};

#endif
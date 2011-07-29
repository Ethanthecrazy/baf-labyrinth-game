#ifndef CFRAME_H_
#define CFRAME_H_

#include "precompiled_header.h"

class CFrame
{
	float m_fDuration;
	std::string m_szTriggerID;
	RECT m_rDrawRect;
public:
	CFrame();
	CFrame(const CFrame* frame);
	CFrame(float fDuration, std::string szTriggerID, RECT rDrawRect);
	CFrame(float fDuration, std::string szTriggerID, 
		   int nTop, int nLeft, int nBottom, int nRight);
	~CFrame();

	friend ostream& operator<<(ostream& os, const CFrame& frame);

	//accessors
	float GetDuration() const;
	std::string GetTriggerID() const;
	RECT GetDrawRect() const;
	//mutators
	void SetDuration(const float fDuration);
	void SetTrigger(const std::string szTriggerID);
	void SetDrawRect(const RECT rDrawRect);
	void SetDrawRect(const int nTop, const int nLeft,
		const int nBottom, const int nRight);
	void SetDrawRectTop(const int nTop);
	void SetDrawRectBottom(const int nBottom);
	void SetDrawRectLeft(const int nLeft);
	void SetDrawRectRight(const int nRight);
};

#endif
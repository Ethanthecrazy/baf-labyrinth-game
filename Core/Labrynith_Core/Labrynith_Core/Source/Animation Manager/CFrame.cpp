#include "CFrame.h"

CFrame::CFrame()
{
	SetDuration(0.0f);
	SetTrigger("");
	SetDrawRect(0,0,0,0);
}
CFrame::CFrame(const CFrame* frame)
{
	SetDuration(frame->GetDuration());
	SetTrigger(frame->GetTriggerID());
	SetDrawRect(frame->GetDrawRect());
}
CFrame::CFrame(float fDuration, std::string szTriggerID, RECT rDrawRect)
{
	SetDuration(fDuration);
	SetTrigger(szTriggerID);
	SetDrawRect(rDrawRect);
}
CFrame::CFrame(float fDuration, std::string szTriggerID, 
	int nTop, int nLeft, int nBottom, int nRight)
{
	SetDuration(fDuration);
	SetTrigger(szTriggerID);
	SetDrawRect(nTop, nLeft, nBottom, nRight);
}
CFrame::~CFrame()
{

}

ostream& operator<<(ostream& os, const CFrame& frame)
{
	os << "Duration: " << frame.GetDuration() << endl;
	os << "TriggerID: " << frame.GetTriggerID() << endl;
	os << "DrawRect: " << endl;
	RECT rect = frame.GetDrawRect();
	os << "Top: " << rect.top << ", ";
	os << "Left: " << rect.left << ", ";
	os << "Bottom: " << rect.bottom << ", ";
	os << "Right: " << rect.right << endl;
	return os;
}

//accessors
float CFrame::GetDuration() const
{
	return m_fDuration;
}
std::string CFrame::GetTriggerID() const
{
	return m_szTriggerID;
}
RECT CFrame::GetDrawRect() const
{
	return m_rDrawRect;
}
//mutators
void CFrame::SetDuration(const float fDuration)
{
	if(fDuration >= 0.0f)
		m_fDuration = fDuration;
}
void CFrame::SetTrigger(const std::string szTriggerID)
{
	m_szTriggerID = szTriggerID;
}
void CFrame::SetDrawRectTop(const int nTop)
{
	if(nTop >= 0)
		m_rDrawRect.top = nTop;
}
void CFrame::SetDrawRectBottom(const int nBottom)
{
	if(nBottom >= 0)
		m_rDrawRect.bottom = nBottom;
}
void CFrame::SetDrawRectLeft(const int nLeft)
{
	if(nLeft >= 0)
		m_rDrawRect.left = nLeft;
}
void CFrame::SetDrawRectRight(const int nRight)
{
	if(nRight >= 0)
		m_rDrawRect.right = nRight;
}
void CFrame::SetDrawRect(const RECT rDrawRect)
{
	SetDrawRectTop(rDrawRect.top);
	SetDrawRectBottom(rDrawRect.bottom);
	SetDrawRectLeft(rDrawRect.left);
	SetDrawRectRight(rDrawRect.right);
}
void CFrame::SetDrawRect(const int nTop, const int nLeft,
	                     const int nBottom, const int nRight)
{
	SetDrawRectTop(nTop);
	SetDrawRectBottom(nBottom);
	SetDrawRectLeft(nLeft);
	SetDrawRectRight(nRight);
}
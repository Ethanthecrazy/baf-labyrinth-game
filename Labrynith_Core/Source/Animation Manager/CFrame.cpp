#include "CFrame.h"

CFrame::CFrame()
{
	SetDuration(0.0f);
	SetTrigger("");
	SetDrawRect(0,0,0,0);
	SetCollisionRect(0,0,0,0);
	SetAnchorX(0);
	SetAnchorY(0);
}
CFrame::CFrame(const CFrame* frame)
{
	SetDuration(frame->GetDuration());
	SetTrigger(frame->GetTriggerID());
	SetDrawRect(frame->GetDrawRect());
	SetCollisionRect(frame->GetCollisionRect());
	SetAnchorX(frame->GetAnchorX());
	SetAnchorY(frame->GetAnchorY());
}
CFrame::CFrame(float fDuration, std::string szTriggerID, 
		   int nAnchorX, int nAnchorY, RECT rDrawRect,
		   RECT rCollisionRect)
{
	SetDuration(fDuration);
	SetTrigger(szTriggerID);
	SetDrawRect(rDrawRect);
	SetCollisionRect(rCollisionRect);
	SetAnchorX(nAnchorX);
	SetAnchorY(nAnchorY);
}
CFrame::CFrame(float fDuration, std::string szTriggerID, 
		   int nAnchorX, int nAnchorY,
		   int nDTop, int nDLeft, int nDBottom, int nDRight,
		   int nCTop, int nCLeft, int nCBottom, int nCRight)
{
	SetDuration(fDuration);
	SetTrigger(szTriggerID);
	SetDrawRect(nDTop, nDLeft, nDBottom, nDRight);
	SetCollisionRect(nCTop, nCLeft, nCBottom, nCRight);
	SetAnchorX(nAnchorX);
	SetAnchorY(nAnchorY);
}
CFrame::~CFrame()
{

}

ostream& operator<<(ostream& os, const CFrame& frame)
{
	os << "Duration: " << frame.GetDuration() << endl;
	os << "TriggerID: " << frame.GetTriggerID() << endl;
	os << "Anchor X: " << frame.GetAnchorX() << endl;
	os << "Anchor Y:" << frame.GetAnchorY() << endl;
	RECT rect = frame.GetDrawRect();
	os << "DrawRect: " << endl;
	os << "Top: " << rect.top << ", ";
	os << "Left: " << rect.left << ", ";
	os << "Bottom: " << rect.bottom << ", ";
	os << "Right: " << rect.right << endl;
	rect = frame.GetCollisionRect();
	os << "CollisionRect: " << endl;
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
RECT CFrame::GetCollisionRect() const
{
	return m_rCollisionRect;
}
int CFrame::GetAnchorX() const
{
	return m_nAnchorX;
}
int CFrame::GetAnchorY() const
{
	return m_nAnchorY;
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
void CFrame::SetAnchorX(const int nAnchorX)
{
	m_nAnchorX = nAnchorX;
}
void CFrame::SetAnchorY(const int nAnchorY)
{
	m_nAnchorY = nAnchorY;
}
void CFrame::SetAnchor(const int nAnchorX, const int nAnchorY)
{
	SetAnchorX(nAnchorX);
	SetAnchorY(nAnchorY);
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
void CFrame::SetCollisionRectTop(const int nTop)
{
	if(nTop >= 0)
		m_rCollisionRect.top = nTop;
}
void CFrame::SetCollisionRectBottom(const int nBottom)
{
	if(nBottom >= 0)
		m_rCollisionRect.bottom = nBottom;
}
void CFrame::SetCollisionRectLeft(const int nLeft)
{
	if(nLeft >= 0)
		m_rCollisionRect.left = nLeft;
}
void CFrame::SetCollisionRectRight(const int nRight)
{
	if(nRight >= 0)
		m_rCollisionRect.right = nRight;
}
void CFrame::SetCollisionRect(const RECT rCollisionRect)
{
	SetCollisionRectTop(rCollisionRect.top);
	SetCollisionRectBottom(rCollisionRect.bottom);
	SetCollisionRectLeft(rCollisionRect.left);
	SetCollisionRectRight(rCollisionRect.right);
}
void CFrame::SetCollisionRect(const int nTop, const int nLeft,
	const int nBottom, const int nRight)
{
	SetCollisionRectTop(nTop);
	SetCollisionRectBottom(nBottom);
	SetCollisionRectLeft(nLeft);
	SetCollisionRectRight(nRight);
}


#include "precompiled_header.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "CAnimation.h"
#include "CFrame.h"

CAnimation::CAnimation()
{
	SetImageID(-1);
	SetCurFrame(0);
	SetSpeed(1.0f);
	m_fTimeWaited = 0.0f;
	SetLooping(false);
	SetReversed(false);
	Stop();
}
CAnimation::CAnimation(const CAnimation* anim)
{
	for(int i = 0; i < anim->GetFrameCount(); i++)
	{
		m_vframes.push_back(new CFrame(anim->GetFrame(i)));
	}
	SetImageID(anim->GetImageID());
	SetCurFrame(0);
	SetSpeed(anim->GetSpeed());
	m_fTimeWaited = 0.0f;
	SetLooping(anim->IsLooping());
	SetReversed(anim->IsReversed());
	Stop();
}
CAnimation::CAnimation(int nImageID, float fSpeed, bool bIsLooping, 
	bool bIsReversed)
{
	SetImageID(nImageID);
	SetCurFrame(0);
	SetSpeed(fSpeed);
	m_fTimeWaited = 0.0f;
	SetLooping(bIsLooping);
	SetReversed(bIsReversed);
	Stop();
}
CAnimation::~CAnimation()
{
	for(unsigned int i = 0; i < m_vframes.size(); i++)
	{
		delete m_vframes[i];
	}
	m_vframes.clear();
}

void CAnimation::Update(float fDelta)
{
	if (!IsPlaying())
        return;

    //increased waited time by delta - affected by speed
    m_fTimeWaited += (fDelta * GetSpeed());
	if (m_fTimeWaited > m_vframes[m_nCurFrame]->GetDuration()) 
    {
        //Reset based of current frames duration
        m_fTimeWaited -= m_vframes[m_nCurFrame]->GetDuration();
		//Go to the next frame
        if(IsReversed())
			SetCurFrame(m_nCurFrame - 1);
		else
			SetCurFrame(m_nCurFrame + 1);

		//Launch this frames event

        if (AtLastFrame()) 
        {
            if (IsLooping())
                Reset();
            else 
            {
                Stop();
                Reset();
            }
        }
    }
}
void CAnimation::Draw(int nPosX, int nPosY, 
	float fScaleX, float fScaleY, float fRotCenterX,
	float fRotCenterY, float fRotation, DWORD dwColor)
{
	if (GetImageID() == -1)
		return;

	CSGD_TextureManager::GetInstance()->Draw(GetImageID(), 
		(nPosX + GetCurFrame()->GetAnchorX()), 
		(nPosY + GetCurFrame()->GetAnchorY()), 
		fScaleX, fScaleY, &GetCurFrame()->GetDrawRect(), fRotCenterX,
		fRotCenterY, fRotation, dwColor);
}

void CAnimation::Play()
{
	Reset();
	Resume();
}
void CAnimation::Stop()
{
	m_bIsPlaying = false;
}
void CAnimation::StopAtFrame(int nIndex)
{
	Stop();
	SetCurFrame(nIndex);
}
void CAnimation::Resume()
{
	m_bIsPlaying = true;
}
void CAnimation::Reset()
{
	m_fTimeWaited = 0.0f;
	SetCurFrame(0);
}

bool CAnimation::AtLastFrame()
{
	if (IsReversed())
    {
        if (m_nCurFrame <= 0)
            return true;
        else
            return false;
    }
    else 
    {
        if(m_nCurFrame >= (int)m_vframes.size())
            return true;
        else
            return false;
    }
}
void CAnimation::AddFrame(CFrame* frame)
{
	m_vframes.push_back(frame);
}
ostream& operator<<(ostream& os, const CAnimation& anim)
{
	//Animation
	os << "Name: " << anim.GetName() << endl;

	os << "Speed: " << anim.GetSpeed() << endl;

	if(anim.IsLooping())
		os << "Looping?: true" << endl;
	else
		os << "Looping?: false" << endl;

	if(anim.IsReversed())
		os << "Reversed?: true" << endl;
	else
		os << "Reversed?: false" << endl;

	//Frames
	for(int i = 0; i < anim.GetFrameCount(); i++)
	{
		CFrame* temp = anim.GetFrame(i);
		if(temp == NULL)
			break;

		os << "Frame " << i << ":" <<endl;
		os << *temp;
	}
	return os;
}

//accessors
CFrame* CAnimation::GetFrame(int nIndex) const
{
	if(nIndex < 0 || nIndex >= (int)m_vframes.size())
		return NULL;

	return m_vframes[nIndex];

}
CFrame* CAnimation::GetCurFrame() const
{
	return m_vframes[m_nCurFrame];
}
int CAnimation::GetFrameCount() const
{
	return m_vframes.size();
}
std::string CAnimation::GetName() const
{
	return m_szName;
}
int CAnimation::GetImageID() const
{
	return m_nImageID;
}
float CAnimation::GetSpeed() const
{
	return m_fSpeed;
}
bool CAnimation::IsLooping() const
{
	return m_bIsLooping;
}
bool CAnimation::IsReversed() const
{
	return m_bIsReversed;
}
bool CAnimation::IsPlaying() const
{
	return m_bIsPlaying;
}
//mutators
void CAnimation::SetName(const std::string szName)
{
	m_szName = szName;
}
void CAnimation::SetImageID(const int nImageID)
{
	m_nImageID = nImageID;
}
void CAnimation::SetCurFrame(int nCurFrame)
{
	if(nCurFrame>= (int)m_vframes.size() 
	   && nCurFrame < 0)
		return;

	m_nCurFrame = nCurFrame;
}
void CAnimation::SetSpeed(const float fSpeed)
{
	if(fSpeed >= 0.25f)
		m_fSpeed = fSpeed;
}
void CAnimation::SetLooping(const bool bIsLooping)
{
	m_bIsLooping = bIsLooping;
}
void CAnimation::SetReversed(const bool bIsReversed)
{
	m_bIsReversed = bIsReversed;
}

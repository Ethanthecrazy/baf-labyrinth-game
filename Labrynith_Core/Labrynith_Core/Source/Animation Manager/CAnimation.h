#ifndef CANIMATION_H_
#define CANIMATION_H_

#include "precompiled_header.h"
class CFrame;

class CAnimation
{
	std::vector<CFrame*> m_vframes;
	std::string m_szName;
	int m_nImageID;
	int m_nCurFrame;
	float m_fSpeed;
	float m_fTimeWaited;
	bool m_bIsPlaying;
	bool m_bIsLooping;
	bool m_bIsReversed;

	void Reset();
	void SetCurFrame(int nCurFrame);
	bool AtLastFrame();
public:
	CAnimation();
	CAnimation(const CAnimation* anim);
	CAnimation(int nImageID, float fSpeed, bool bIsLooping, 
		       bool bIsReversed);
	~CAnimation();

	friend ostream& operator<<(ostream& os, const CAnimation& anim);

	void Update(float fDelta);
	void Draw(int nPosX, int nPosY, float fScaleX, 
		     float fScaleY, float fRotCenterX,
	         float fRotCenterY, float fRotation, DWORD dwColor);

	void Play();
	void Stop();
	void StopAtFrame(int nIndex);
	void Resume();

	void AddFrame(CFrame* frame);

	//accessors
	int GetFrameCount() const;
	//returns a certain frame based on the index sent in
	CFrame* GetFrame(int nIndex) const;
	//returns the current frame the animation is on
	CFrame* GetCurFrame() const;
	std::string GetName() const;
	int GetImageID() const;
	float GetSpeed() const;
	bool IsLooping() const;
	bool IsReversed() const;
	bool IsPlaying() const;
	//mutators
	void SetName(const std::string szName);
	void SetImageID(const int nImageID);
	void SetSpeed(const float fSpeed);
	void SetLooping(const bool bIsLooping);
	void SetReversed(const bool bIsReversed);
};

#endif
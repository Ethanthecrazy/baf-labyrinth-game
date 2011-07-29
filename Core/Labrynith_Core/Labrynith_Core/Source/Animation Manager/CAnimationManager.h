#ifndef CANIMMANAGER
#define CANIMMANAGER

#include "precompiled_header.h"
class CAnimation;

class CAnimationManager
{
	//A list of each different animation
	std::vector<CAnimation*> m_vAnims;
	//All animations that different objects reference to
	std::vector<CAnimation*> m_vAnimPool;

	//Singleton
	static CAnimationManager* instance;
	CAnimationManager();
	CAnimationManager(const CAnimationManager* c);
protected:
	~CAnimationManager();
public:
	friend ostream& operator<<(ostream& os, const CAnimationManager& manager);

	bool LoadBinary(std::string szFilename);
	bool LoadXML(std::string szFilename);
	bool LoadAnimation(std::string szFilename, bool bIsXML);
	void AddAnimation(const CAnimation* anim);
	void AddPoolAnimation(const CAnimation* anim);

	static CAnimationManager* GetInstance();
	static void DeleteInstance();

	bool UpdateAnimation(float fDelta, int nAnimID);
	bool Draw(int nIndex, int nPosX, int nPosY);

	bool PlayAnimation(int nAnimID);
	bool StopAnimation(int nAnimID);
	bool StopAnimationAtFrame(int nAnimID, int nFrameIndex);
	bool ResumeAnimation(int nAnimID);

	int GetListSize() const;
	int GetPoolSize() const;
	int GetID(std::string szName) const;
    bool IsAnimationPlaying(int nAnimID) const;
	bool SetAnimTexture(const int nAnimID, const int nImageID); 
	CAnimation* GetAnimation(int nAnimID) const;
	CAnimation* GetListAnimation(int nAnimID) const;
	CAnimation* GetAnimation(std::string szName) const;
}; 

#endif
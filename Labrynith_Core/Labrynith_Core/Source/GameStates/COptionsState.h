#ifndef COPTIONSSTATE_H_
#define COPTIONSSTATE_H_

#include "IGameState.h"
#include "../CBitFont.h"

class COptionsState : public IGameState
{
private:

	//Data Members
	CBitFont MetalText;
	int m_nIndex;
	int m_nMusicVolume, m_nSFXVolume;
	bool m_bIsFullscreen;
	//Asset ids:
	int		m_nImageID;
	int		m_nSoundID;
	//Enums
	enum MENUOPTIONS{SFXVOLUME , MUSICV , FULLSCREEN , BACK , NUMOPTIONS};


	COptionsState(void);
	COptionsState (const COptionsState &);
	COptionsState & operator=(const COptionsState &);
	~COptionsState (void);
public:
	static COptionsState* GetInstance(void);
	virtual void Enter(void);
	virtual bool Input(void);
	virtual void Update(float fElapsedTime);
	virtual void Render(void);
	virtual void Exit(void);
	void EnterCommand(void);
	void LoadOptions();
	void AdjustSound(const int nSoundID, const bool bIsSFX);

	//accessors
	int GetMusicVolume() const;
	int GetSFXVolume() const;
	bool GetFullScreen() const;
	//mutators
	void SetMusicVolume(const int nMusicVol);
	void SetSFXVolume(const int nSFXVol);
	void SetFullScreen(const bool bIsFullScreen);
};

#endif
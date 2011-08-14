#include <fstream>
#include "COptionsState.h"
#include "..\CGame.h"
#include "CMainMenuState.h"
#include "..\Wrappers\CSGD_FModManager.h"
#include "..\Wrappers\CSGD_DirectInput.h"
#include "..\Wrappers\CSGD_TextureManager.h"
#include "..\Wrappers\CSGD_Direct3D.h"


COptionsState::COptionsState(void)
{
	m_nMusicVolume = 100;
	m_nSFXVolume = 100;
	m_bIsFullscreen = false;
	m_nPANL = 50;
	m_nPANR = 50;
	m_nIndex = 0;
	m_nImageID = -1;
	m_nSoundID= -1;
	CGame* pGame = CGame::GetInstance();
}

COptionsState::~COptionsState(void)
{

}

void COptionsState::Enter(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CSGD_FModManager* pFM = CSGD_FModManager::GetInstance();
	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.txt" );
	LoadOptions();
}
bool COptionsState::Input(void)
{
	//Get access to singletons
	CSGD_FModManager* pFM = CSGD_FModManager::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CGame* pGame = CGame::GetInstance();

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP))
	{
		if(m_nIndex != 0)
			m_nIndex -= 1;
		else
			m_nIndex = NUMOPTIONS - 1;
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN))
	{
		if(m_nIndex != (NUMOPTIONS - 1))
			m_nIndex += 1;
		else
			m_nIndex = 0;
	}

	static float fCollectedTime = 0.0f;
	fCollectedTime += pGame->GetElapsedTime();
	//	right
	if(pDI->KeyPressed(DIK_RIGHT))
	{
		switch( m_nIndex )
		{
		case FULLSCREEN:
			{
				SetFullScreen(!GetFullScreen());
			}
			break;
		};
	}
	if( pDI->KeyDown(DIK_RIGHT))
	{
		if(fCollectedTime < 0.5f)
			return true;

		switch( m_nIndex )
		{
		case SFXVOLUME:
			if( m_nSFXVolume < 100 )
			{
				m_nSFXVolume += 5;
				/*pFM->SetVolume(m_nSoundIDHonk,
					m_fSFXVolume); 
				pFM->PlaySoundA(m_nSoundIDHonk) ;*/
				fCollectedTime = 0.0f;
			}
			break ;
		case MUSICV:
			if( m_nMusicVolume < 100 )
			{
				m_nMusicVolume += 5;
				/*pFM->SetVolume( m_nSoundIDBackground, 
					m_fMusicVolume );*/
				fCollectedTime = 0.0f;
			}
			break ;
		case PANLR:
			if(m_nPANL < 100)
			{
				m_nPANL += 5;
				m_nPANR -= 5;
				/*pFM->SetPan(m_nSoundIDBackground, (float)(m_nPANR
					- m_nPANL)/100);
				pFM->SetPan(m_nSoundIDHonk, (float)(m_nPANR
					- m_nPANL)/100);*/
				fCollectedTime = 0.0f;
			}
			break ;
		} ;
	}

	//	left
	if(pDI->KeyPressed(DIK_LEFT))
	{
		switch( m_nIndex )
		{
		case FULLSCREEN:
			{
				SetFullScreen(!GetFullScreen());
			}
			break;
		};
	}
	if( pDI->KeyDown(DIK_LEFT))
	{
		if(fCollectedTime < 0.5f)
			return true;

		switch( m_nIndex )
		{
		case SFXVOLUME:
			if( m_nSFXVolume > 0 )
			{
				m_nSFXVolume -= 5;
				/*pFM->SetVolume( m_nSoundIDHonk,
					m_fSFXVolume);
				pFM->PlaySoundA(m_nSoundIDHonk);*/
				fCollectedTime = 0.0f;
			}
			break ;
		case MUSICV:
			if( m_nMusicVolume > 0 )
			{
				m_nMusicVolume -= 5;
				/*pFM->SetVolume( m_nSoundIDBackground, 
					m_fMusicVolume );*/
				fCollectedTime = 0.0f;
			}
			break ;
		case PANLR:
			if(m_nPANR < 100)
			{
				m_nPANR += 5;
				m_nPANL -= 5;
				/*pFM->SetPan(m_nSoundIDBackground, (float)(m_nPANR 
					- m_nPANL)/100);
				pFM->SetPan(m_nSoundIDHonk, (float)(m_nPANR
					- m_nPANL)/100);*/
				fCollectedTime = 0.0f;
			}
			break ;
		} ;
	}

	//Enter
	if(pDI->KeyPressed(DIK_RETURN))
	{
		if( m_nIndex == BACK ) 
		{
			pGame->ChangeState( CMainMenuState::GetInstance() );
		}
	}

	return true;
}
void COptionsState::Update(float fElapsedTime)
{

}
void COptionsState::Render(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D* pDev = CSGD_Direct3D::GetInstance();

	MetalText.Print("Options Menu", 140, 50, 0.8f);

	char buffer[200];
	float i = 30;
	//Sfx Vol
	MetalText.Print("Sfx Vol.", 150, 265, 0.6f);
	sprintf_s(buffer, "<%i>", m_nSFXVolume);
	MetalText.Print(buffer, 450, 265, 0.6f);
	//Music Vol
	MetalText.Print("Music Vol.", 150, 300, 0.6f);
	sprintf_s(buffer, "<%i>", m_nMusicVolume);
	MetalText.Print(buffer, 470, 300, 0.6f);
	//Pan
	MetalText.Print("Pan LR", 150, 335, 0.6f);
	sprintf_s(buffer, "<%i>", m_nPANL);
	MetalText.Print(buffer, 350, 340, 0.6f);
	sprintf_s(buffer, "<%i>", m_nPANR);
	MetalText.Print(buffer, 500, 340, 0.6f);
	//FullScreen
	MetalText.Print("Fullscreen: ", 150, 370, 0.5f);
	if(GetFullScreen())
		MetalText.Print("On", 460, 370, 0.5f);
	else
		MetalText.Print("Off", 460, 370, 0.5f);
	//Back
	MetalText.Print("Back", 150, 400, 0.6f);
	//Cursor
	MetalText.Print("->", 80, 265  + (m_nIndex * 35), 0.6f);
}
COptionsState* COptionsState::GetInstance(void)
{
	static COptionsState state;
	return &state;
}
void COptionsState::Exit(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
 
	//save settings
	ofstream out;
	out.open("resource/Game Saves/soundoptions.dat", ios_base::out | ios_base::trunc | ios_base::binary);
	if (out.is_open())
	{
		if (out.good())
		{
			out.write((const char *)&m_nMusicVolume, sizeof m_nMusicVolume); 
			out.write((const char *)&m_nSFXVolume, sizeof m_nSFXVolume); 
			out.write((const char *)&m_nPANL, sizeof m_nPANL); 
			out.write((const char *)&m_nPANR, sizeof m_nPANR); 
			out.write((const char *)&m_bIsFullscreen, sizeof m_bIsFullscreen); 
		}
		out.close();
	}


}
void COptionsState::EnterCommand(void)
{

}
void COptionsState::LoadOptions()
{
	//load options
	ifstream in;
	int nMusicVol, nSfxVol;
	int nPanR, nPanL;
	bool bIsFullScreen;
	in.open("resource/Game Saves/soundoptions.dat", ios_base::in | ios_base::binary);
	if(!in.is_open())
	{
		return;
	}

	if (in.good())
	{
		in.read((char *)&nMusicVol, sizeof(int)); 
		SetMusicVolume(nMusicVol);
		in.read((char *)&nSfxVol, sizeof(int)); 
		SetSFXVolume(nSfxVol);
		in.read((char *)&nPanL, sizeof(int)); 
		in.read((char *)&nPanR, sizeof(int)); 
		in.read((char *)&bIsFullScreen, sizeof(bool));
		SetFullScreen(bIsFullScreen);
	}
	in.close();
}
void COptionsState::AdjustSound(const int nSoundID, const bool bIsSFX)
{
	if(nSoundID < 0)
		return;

	CSGD_FModManager* pFM = CSGD_FModManager::GetInstance();
	if(bIsSFX)
		pFM->SetVolume(nSoundID, (float)GetSFXVolume()/100);
	else
		pFM->SetVolume(nSoundID, (float)GetMusicVolume()/100);
}

//accessors
int COptionsState::GetMusicVolume() const
{
	return m_nMusicVolume;
}
int COptionsState::GetSFXVolume() const
{
	return m_nSFXVolume;
}
int COptionsState::GetPanLeft() const
{
	return m_nPANL;
}
int COptionsState::GetPanRight() const
{
	return m_nPANR;
}
bool COptionsState::GetFullScreen() const
{
	return m_bIsFullscreen;
}
//mutators
void COptionsState::SetMusicVolume(const int nMusicVol)
{
	if(nMusicVol > 100 || nMusicVol < 0)
		return;

	m_nMusicVolume = nMusicVol;
}
void COptionsState::SetSFXVolume(const int nSFXVol)
{
	if(nSFXVol > 100 || nSFXVol < 0)
		return;

	m_nSFXVolume = nSFXVol;
}
void COptionsState::SetPanLeft(const int nPanL)
{
	m_nPANL = nPanL;
}
void COptionsState::SetPanRight(const int nPanR)
{
	m_nPANR = nPanR;
}
void COptionsState::SetFullScreen(const bool bIsFullScreen)
{
	CGame* pGame = CGame::GetInstance();
	m_bIsFullscreen = bIsFullScreen;
	pGame->SetFullScreen(GetFullScreen());
}

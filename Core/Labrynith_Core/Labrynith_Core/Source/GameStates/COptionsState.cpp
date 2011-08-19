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
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	CGame* pGame = CGame::GetInstance();
	
	m_nMusicVolume = 100;
	m_nSFXVolume = 100;
	m_bIsFullscreen = false;
	m_nIndex = 0;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/labyrinth.png");
	m_nSoundID = FM->LoadSound("resource/Sounds/Clic1.wav");	
	AdjustSound(m_nSoundID, true);
}

COptionsState::~COptionsState(void)
{

}

void COptionsState::Enter(void)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CSGD_FModManager* pFM = CSGD_FModManager::GetInstance();
	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
	LoadOptions();
}
bool COptionsState::Input(void)
{
	//Get access to singletons
	CSGD_FModManager* pFM = CSGD_FModManager::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	CGame* pGame = CGame::GetInstance();

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP) ||
		pDI->JoystickGetLStickDirPressed(DIR_UP, 0))
	{
		FM->PlaySoundA(m_nSoundID);
		AdjustSound(m_nSoundID, true);
		if(m_nIndex != 0)
			m_nIndex -= 1;
		else
			m_nIndex = NUMOPTIONS - 1;
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN) ||
		pDI->JoystickGetLStickDirPressed(DIR_DOWN, 0))
	{
		FM->PlaySoundA(m_nSoundID);
		AdjustSound(m_nSoundID, true);
		if(m_nIndex != (NUMOPTIONS - 1))
			m_nIndex += 1;
		else
			m_nIndex = 0;
	}

	static float fCollectedTime = 0.0f;
	fCollectedTime += pGame->GetElapsedTime();
	//	right
	if(pDI->KeyPressed(DIK_RIGHT) ||
		pDI->JoystickGetLStickDirPressed(DIR_RIGHT, 0))
	{
		switch( m_nIndex )
		{
		case FULLSCREEN:
			{
				FM->PlaySoundA(m_nSoundID);
				AdjustSound(m_nSoundID, true);
				SetFullScreen(!GetFullScreen());
			}
			break;
		};
	}
	if( pDI->KeyDown(DIK_RIGHT) ||
		pDI->JoystickGetLStickDirDown(DIR_RIGHT, 0))
	{
		if(fCollectedTime < 0.2f)
			return true;

		switch( m_nIndex )
		{
		case SFXVOLUME:
			if( m_nSFXVolume < 100 )
			{
				m_nSFXVolume += 5;
				FM->PlaySoundA(m_nSoundID);
				AdjustSound(m_nSoundID, true);
				fCollectedTime = 0.0f;
			}
			break ;
		case MUSICV:
			if( m_nMusicVolume < 100 )
			{
				m_nMusicVolume += 5;
				AdjustSound(CMainMenuState::GetInstance()->GetSongID(), false);
				fCollectedTime = 0.0f;
			}
			break ;
		} ;
	}

	//	left
	if(pDI->KeyPressed(DIK_LEFT) ||
		pDI->JoystickGetLStickDirPressed(DIR_LEFT, 0))
	{
		switch( m_nIndex )
		{
		case FULLSCREEN:
			{
				FM->PlaySoundA(m_nSoundID);
				AdjustSound(m_nSoundID, true);
				SetFullScreen(!GetFullScreen());
			}
			break;
		};
	}
	if( pDI->KeyDown(DIK_LEFT) ||
		pDI->JoystickGetLStickDirDown(DIR_LEFT, 0))
	{
		if(fCollectedTime < 0.2f)
			return true;

		switch( m_nIndex )
		{
		case SFXVOLUME:
			if( m_nSFXVolume > 0 )
			{
				m_nSFXVolume -= 5;
				FM->PlaySoundA(m_nSoundID);
				AdjustSound(m_nSoundID, true);
				fCollectedTime = 0.0f;
			}
			break ;
		case MUSICV:
			if( m_nMusicVolume > 0 )
			{
				m_nMusicVolume -= 5;
				AdjustSound(CMainMenuState::GetInstance()->GetSongID(), false);
				fCollectedTime = 0.0f;
			}
			break ;
		} ;
	}

	//Enter
	if(pDI->KeyPressed(DIK_RETURN) ||
		pDI->JoystickButtonPressed(0))
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

	pTM->Draw(m_nImageID, 0 , 0);
	MetalText.Print("Options Menu", 240, 50, 0.8f);

	char buffer[200];
	float i = 30;
	//Sfx Vol
	MetalText.Print("Sfx Vol.", 250, 265, 0.6f);
	sprintf_s(buffer, "<%i>", m_nSFXVolume);
	MetalText.Print(buffer, 550, 265, 0.6f);
	//Music Vol
	MetalText.Print("Music Vol.", 250, 300, 0.6f);
	sprintf_s(buffer, "<%i>", m_nMusicVolume);
	MetalText.Print(buffer, 570, 300, 0.6f);
	//FullScreen
	MetalText.Print("Fullscreen: ", 250, 335, 0.6f);
	if(GetFullScreen())
		MetalText.Print("On", 560, 335, 0.6f);
	else
		MetalText.Print("Off", 560, 335, 0.6f);
	//Back
	MetalText.Print("Back", 250, 370, 0.6f);
	//Cursor
	MetalText.Print("->", 180, 265  + (m_nIndex * 35), 0.6f);
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
void COptionsState::SetFullScreen(const bool bIsFullScreen)
{
	CGame* pGame = CGame::GetInstance();
	m_bIsFullscreen = bIsFullScreen;
	pGame->SetFullScreen(GetFullScreen());
}

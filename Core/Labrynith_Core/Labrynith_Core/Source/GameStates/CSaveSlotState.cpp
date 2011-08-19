#include "CSaveSlotState.h"
#include <fstream>
#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "COptionsState.h"
#include "../CGame.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../Wrappers/CSGD_Direct3D.h"
#include "../Wrappers/CSGD_FModManager.h"

CSaveSlotState::CSaveSlotState()
{
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	m_nCurrSaveSlot = 0;
	m_nIndex = 0;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/darkCave.png");
	m_nSoundID = FM->LoadSound("resource/Sounds/Clic1.wav");
	for(unsigned int i = 0; i < MAXSLOTS; i++)
	{
		m_nLoadedLevels[i] = 1;
	}
}

// destructor
CSaveSlotState::~CSaveSlotState() 
{

}

CSaveSlotState* CSaveSlotState::GetInstance()
{
	static CSaveSlotState instance;
	return &instance;
}

void CSaveSlotState::Enter(void)
{
	MetalText.Initialize( CSGD_TextureManager::GetInstance()->LoadTexture( "resource/metal.png" ),
		' ', 64, 64, 10, "resource/Game Saves/metalpng.xml" );
	//load all the current levels
	for(unsigned int i = 0; i < MAXSLOTS; i++)
	{
		Load(i);
	}
	SetSaveSlot(0);
	COptionsState* Opt = COptionsState::GetInstance();
	Opt->AdjustSound(m_nSoundID, true);
}

bool CSaveSlotState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CGamePlayState* pGamePlay = CGamePlayState::GetInstance();
	CSGD_FModManager* FM = CSGD_FModManager::GetInstance();
	//Enter
	if(pDI->KeyPressed(DIK_RETURN) ||
		pDI->JoystickButtonPressed(0))
	{
		switch(m_nIndex)
		{
		case SELECT:
			{
				//Change to GamePlayState
				FM->StopSound( CMainMenuState::GetInstance()->GetSongID() );
				pGame->ChangeState(pGamePlay);
				break;
			}
		case SLOTDELETE:
			{
				Delete();
				break;
			}
		case BACK:
			{
				pGame->ChangeState(CMainMenuState::GetInstance());
				break;
			}
		};
	}

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP) ||
	   pDI->JoystickGetLStickDirPressed(DIR_UP, 0))
	{
		FM->PlaySoundA(m_nSoundID);
		if(m_nIndex != 0)
			m_nIndex -= 1;
		else
			m_nIndex = NUMSLOTOPTIONS - 1;
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN) ||
		pDI->JoystickGetLStickDirPressed(DIR_DOWN, 0))
	{
		FM->PlaySoundA(m_nSoundID);
		if(m_nIndex != (NUMSLOTOPTIONS - 1))
			m_nIndex += 1;
		else
			m_nIndex = 0;
	}
	//Left
	if(pDI->KeyPressed(DIK_LEFT) ||
		pDI->JoystickGetLStickDirPressed(DIR_LEFT, 0))
	{
		SetSaveSlot(--m_nCurrSaveSlot);
		FM->PlaySoundA(m_nSoundID);
	}
	//Right
	if(pDI->KeyPressed(DIK_RIGHT) ||
		pDI->JoystickGetLStickDirPressed(DIR_RIGHT, 0))
	{
		SetSaveSlot(++m_nCurrSaveSlot);
		FM->PlaySoundA(m_nSoundID);
	}

	return true;
}

void CSaveSlotState::Update(float fDT)
{

}

void CSaveSlotState::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	CSGD_TextureManager::GetInstance()->Draw(m_nImageID, 0 , 0);
	char buffer[64] = { };
	MetalText.Print( "---", 100 + (m_nCurrSaveSlot * 300), 80,  0.8f );
	//Slot1	
	MetalText.Print( "Slot 1", 100, 100, 0.6f );
	sprintf_s(buffer,"Level: %i", m_nLoadedLevels[0]);
	MetalText.Print( buffer, 100, 140, 0.5f );
	//Slot2
	MetalText.Print( "Slot 2", 400, 100, 0.6f );
	sprintf_s(buffer,"Level: %i", m_nLoadedLevels[1]);
	MetalText.Print( buffer, 400, 140, 0.5f );
	//Slot3
	MetalText.Print( "Slot 3", 700, 100, 0.6f );
	sprintf_s(buffer,"Level: %i", m_nLoadedLevels[2]);
	MetalText.Print( buffer, 700, 140, 0.5f );
	//Options
	MetalText.Print( "->", 250, 450 + (m_nIndex * 35), 0.8f );
	MetalText.Print( "Select", 350, 450, 0.8f );
	MetalText.Print( "Delete", 350, 485, 0.8f );
	MetalText.Print( "Back", 350, 520, 0.8f );
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
}

void CSaveSlotState::Exit(void)
{

}

void CSaveSlotState::EnterCommand(void)
{

}

void CSaveSlotState::Save()
{
	CGamePlayState* pGamePlay = CGamePlayState::GetInstance();
	SetCurrLevel(pGamePlay->GetCurrentLevel());

	//save settings
	ofstream out;
	char filename[255] = {0};
	int nLevel = GetCurrLevel();
	sprintf_s(filename, "resource/Game Saves/Slot%d.dat", m_nCurrSaveSlot);
	out.open(filename, ios_base::out | ios_base::trunc | ios_base::binary);
	if (out.is_open())
	{
		if (out.good())
			out.write(((const char *)&nLevel), 4); 
		out.close();
	}
}
void CSaveSlotState::Load(int nSaveSlot)
{
	SetSaveSlot(nSaveSlot);
	CGamePlayState* pGamePlay = CGamePlayState::GetInstance();

	//load settings
	ifstream in;
	char filename[255] = {0};
	int nLevel;
	sprintf_s(filename, "resource/Game Saves/Slot%d.dat", m_nCurrSaveSlot);
	in.open(filename, ios_base::in | ios_base::binary);
	if(!in.is_open())
	{
		//if we couldnt find the file start form lv1
		m_nLoadedLevels[m_nCurrSaveSlot] = 1;
		return;
	}

	if (in.good())
	{
		in.read((char *)&nLevel, sizeof(int));
		m_nLoadedLevels[m_nCurrSaveSlot] = nLevel;
	}
	in.close();

}
void CSaveSlotState::Delete()
{
	CGamePlayState* pGamePlay = CGamePlayState::GetInstance();
	
	//delete settings - restart from the first level 
	ofstream out;
	char filename[255] = {0};
	int nLevel = 1;
	sprintf_s(filename, "resource/Game Saves/Slot%d.dat", m_nCurrSaveSlot);
	out.open(filename, ios_base::out | ios_base::trunc | ios_base::binary);
	if (out.is_open())
	{
		if (out.good())
		{
			out.write(((const char *)&nLevel), 4); 
		}
		out.close();
	}
	Load(m_nCurrSaveSlot);
	SetCurrLevel(nLevel);
}

//accessors
int CSaveSlotState::GetCurrLevel() const
{
	return m_nCurrLoadedLevel;
}
	//mutators
void CSaveSlotState::SetCurrLevel(const int nLevel)
{
	CGamePlayState* pGamePlay = CGamePlayState::GetInstance();
	m_nCurrLoadedLevel = nLevel;
	pGamePlay->SetCurrentLevel(GetCurrLevel());
}
void CSaveSlotState::SetSaveSlot(const int nSlot)
{
	//if index is too low
	if(nSlot < 0 )
	{
		m_nCurrSaveSlot = (MAXSLOTS - 1);
		SetCurrLevel(m_nLoadedLevels[m_nCurrSaveSlot]);
		return;
	}

	//if index is too high
	if(nSlot >= MAXSLOTS)
	{
		m_nCurrSaveSlot = 0;
		SetCurrLevel(m_nLoadedLevels[m_nCurrSaveSlot]);
		return;
	}

	m_nCurrSaveSlot = nSlot;
	SetCurrLevel(m_nLoadedLevels[m_nCurrSaveSlot]);
}

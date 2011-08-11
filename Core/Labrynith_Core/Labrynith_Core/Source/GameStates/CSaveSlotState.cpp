#include "CSaveSlotState.h"
#include <fstream>
#include "CGamePlayState.h"
#include "../CGame.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"
#include "../Wrappers/CSGD_Direct3D.h"

CSaveSlotState::CSaveSlotState()
{
	m_nCurrSaveSlot = 0;
	m_nIndex = 0;
	SetCurrLevel(1);
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
		' ', 64, 64, 10 );
}

bool CSaveSlotState::Input(void)
{
	CGame* pGame = CGame::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CGamePlayState* pGamePlay = CGamePlayState::GetInstance();
	//Enter
	if(pDI->KeyPressed(DIK_RETURN))
	{
		switch(m_nIndex)
		{
		case SELECT:
			{
				//Change to GamePlayState
				Load();
				pGame->ChangeState(pGamePlay);
				break;
			}
		case SLOTDELETE:
			{
				Delete();
				break;
			}
		};
	}

	//Directional
	//Up
	if(pDI->KeyPressed(DIK_UP))
	{
		if(m_nIndex != 0)
			m_nIndex -= 1;
		else
			m_nIndex = NUMSLOTOPTIONS - 1;
	}
	//Down
	if(pDI->KeyPressed(DIK_DOWN))
	{
		if(m_nIndex != (NUMSLOTOPTIONS - 1))
			m_nIndex += 1;
		else
			m_nIndex = 0;
	}
	//Left
	if(pDI->KeyPressed(DIK_LEFT))
	{
		SetSaveSlot(--m_nCurrSaveSlot);
		//Play a sound
	}
	//Right
	if(pDI->KeyPressed(DIK_RIGHT))
	{
		SetSaveSlot(++m_nCurrSaveSlot);
		//Play a sound
	}
	return true;
}

void CSaveSlotState::Update(float fDT)
{

}

void CSaveSlotState::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	MetalText.Print( "---", 0 + (m_nCurrSaveSlot * 250), 80,  0.8f );
	MetalText.Print( "Slot 1", 0, 100, 0.6f );
	MetalText.Print( "Slot 2", 250, 100, 0.6f );
	MetalText.Print( "Slot 3", 500, 100, 0.6f );

	MetalText.Print( "->", 100, 450 + (m_nIndex * 35), 0.8f );
	MetalText.Print( "Select", 200, 450, 0.8f );
	MetalText.Print( "Delete", 200, 485, 0.8f );
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
void CSaveSlotState::Load()
{
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
		SetCurrLevel(1);
		pGamePlay->SetCurrentLevel(GetCurrLevel());
		return;
	}

	if (in.good())
	{
		in.read((char *)&nLevel, sizeof(int));
		SetCurrLevel(nLevel);
		pGamePlay->SetCurrentLevel(GetCurrLevel());
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
			SetCurrLevel(nLevel);
		}
		out.close();
	}
}

//accessors
int CSaveSlotState::GetCurrLevel() const
{
	return m_nCurrLoadedLevel;
}
	//mutators
void CSaveSlotState::SetCurrLevel(const int nLevel)
{
	m_nCurrLoadedLevel = nLevel;
}
void CSaveSlotState::SetSaveSlot(const int nSlot)
{
	//if index is too low
	if(nSlot < 0 )
	{
		m_nCurrSaveSlot = (MAXSLOTS - 1);
		return;
	}

	//if index is too high
	if(nSlot >= MAXSLOTS)
	{
		m_nCurrSaveSlot = 0;
		return;
	}

	m_nCurrSaveSlot = nSlot;
}

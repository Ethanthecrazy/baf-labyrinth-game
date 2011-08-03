#include "CSaveSlotState.h"
#include "../Wrappers/CSGD_DirectInput.h"
#include "../Wrappers/CSGD_TextureManager.h"

CSaveSlotState::CSaveSlotState()
{

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
	
}

bool CSaveSlotState::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();	

	return true;
}

void CSaveSlotState::Update(float fDT)
{

}

void CSaveSlotState::Render(void)
{

}

void CSaveSlotState::Exit(void)
{

}

void CSaveSlotState::EnterCommand(void)
{

}

void CSaveSlotState::Save()
{

}

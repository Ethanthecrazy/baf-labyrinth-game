#ifndef CHUD_H_
#define CHUD_H_

//this is the main HUD in the game
class CPlayer;

class CHUD
{
	//the players stuff to render
	CPlayer* pPlayer;
	//id to render heart image
	int HeartImageID;
	int InvSlotImageID;
	//singleton
	CHUD();
	CHUD(CHUD& hud);
	void SetPlayer(const int nID);
protected:
	~CHUD();
public:
	void Render();
	static CHUD* GetInstance();
	
	void SetPlayer(CPlayer* player);
};
#endif
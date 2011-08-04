#ifndef CSPAWNER_H_
#define CSPAWNER_H_

#include "../CBaseObject.h"
//#include "../CBaseEntity.h"
#include "../../../Messaging/IListener.h"

enum SPAWNERTYPE{ 
				  SPAWNER_EARTH, SPAWNER_FIRE, SPAWNER_WATER, SPAWNER_ICE, SPAWNER_AIR,
				  SPAWNER_LAVA, SPAWNER_IRON, SPAWNER_LIGHT, SPAWNER_SHADOW,

				  SPAWNER_ATTRACTOR_EARTH, SPAWNER_ATTRACTOR_FIRE, SPAWNER_ATTRACTOR_WATER, SPAWNER_ATTRACTOR_ICE, SPAWNER_ATTRACTOR_AIR,
				  SPAWNER_ATTRACTOR_LAVA, SPAWNER_ATTRACTOR_IRON, SPAWNER_ATTRACTOR_LIGHT, SPAWNER_ATTRACTOR_SHADOW,

				  SPAWNER_PLAYER 
				};

class CSpawner : public CBaseObject, public IListener
{
private:
	int m_nSpawnerType;
	int m_nSpawnedID;

public:

	CSpawner(int nSpawnerType);

	virtual ~CSpawner(void);

	virtual void Update(float fDT);
	virtual void Render(int CameraPosX, int CameraPosY);

	int GetSpawnerType(void) {return m_nSpawnerType;}

	void HandleEvent( Event* _toHandle );
	bool CheckCollision(IUnitInterface* pBase);
};


#endif
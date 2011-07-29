#ifndef CGOLEMSPAWNER_H_
#define CGOLEMSPAWNER_H_

#include "../CBaseObject.h"

enum SPAWNERTYPE{ SPAWNER_EARTH, SPAWNER_FIRE, SPAWNER_WATER, SPAWNER_AIR,
				  SPAWNER_LAVA, SPAWNER_IRON, SPAWNER_LIGHT, SPAWNER_DARK,
				  SPAWNER_GLASS, 

				  SPAWNER_ATTRACTOR_EARTH, SPAWNER_ATTRACTOR_FIRE, SPAWNER_ATTRACTOR_WATER, SPAWNER_ATTRACTOR_AIR,
				  SPAWNER_ATTRACTOR_LAVA, SPAWNER_ATTRACTOR_IRON, SPAWNER_ATTRACTOR_LIGHT, SPAWNER_ATTRACTOR_DARK,
				  SPAWNER_ATTRACTOR_GLASS,

				  SPAWNER_PLAYER};

class CSpawner : public CBaseObject
{
private:
	int m_nSpawnerType;
	int m_nSpawnedID;

public:

	CSpawner(int nSpawnerType = SPAWNER_EARTH);

	~CSpawner(void);

	void Update(float fDT);
	void Render(int CameraPosX, int CameraPosY);

	int GetSpawnerType(void) {return m_nSpawnerType;}
};


#endif
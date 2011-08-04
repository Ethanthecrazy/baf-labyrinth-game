#pragma once

#include "MLayer.h"
#include "../Messaging/MEventSystem.h"
#include <vector>
using namespace std;

#include "../BST.h"

class MObjectManager : public IListener
{

private:

	BST<int> LayerIndex;
	vector<MLayer> m_vLayers;
	int m_nSize;

	// default constructor
	MObjectManager();
	// copy constructor
	MObjectManager(const MObjectManager&);
	// assignment operator
	MObjectManager& operator=(const MObjectManager&);
	// destructor
	~MObjectManager();

public:

	static MObjectManager* GetInstance( void );
	void DeleteInstance(void);

	// Adds a unit to the object manager, but does not store it in the flake's data array.
	int AddUnit( IUnitInterface* _toAdd, int _layer );
	// Adds a unit to the game and updates the flake's data array.
	int AddUnitIndexed( IUnitInterface* _toAdd, int _layer );

	bool RemoveUnit( int _Ident );
	void RemoveAllUnits( void );

	void ResizeLayer( int _layer, int newWidth, int newHeight ); 

	IUnitInterface* GetUnit( int _Ident );

	void Update( float fDT );
	void Render( int CameraX, int CameraY );
	void CheckCollisions( void );

	bool MoveEntUp( int _Ident );
	bool MoveEntDown( int _Ident );
	bool MoveEntLeft( int _Ident );
	bool MoveEntRight( int _Ident );
	bool CheckStandingOn( int _Ident );

	MLayer& FindLayer( int _Ident );
	MLayer& GetLayer( int _index ) { return m_vLayers[ _index ]; }

	MFlake& FindFlake( int _Ident );

	int FindValueInFlakeInLayerAtIndex( int _layer, int _flake, int _x, int _y ); 

	void MObjectManager::HandleEvent( Event* _toHandle );

};
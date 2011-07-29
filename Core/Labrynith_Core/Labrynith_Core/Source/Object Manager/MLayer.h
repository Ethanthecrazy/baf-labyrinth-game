#pragma once

#include "MFlake.h"
#include <vector>
using namespace std;

#include "../BST.h"

class MLayer
{
private:

	int LayerID;

	BST<int> FlakeIndex;
	vector<MFlake> m_vFlakes;
	int m_nSize;

	int LayerWidth;
	int LayerHeight;

	int OffSetFromCenterX;
	int OffSetFromCenterY;

public:

	MLayer( int _layerID );

	int AddUnit( IUnitInterface* _toAdd );
	int AddUnitIndexed( IUnitInterface* _toAdd );
	bool RemoveUnit( int _Ident );
	void RemoveAllUnits( void );

	void Resize( int newWidth, int newHeight );

	IUnitInterface* GetUnit( int _Ident );

	void Update( float fDT );
	void Render( int CameraX, int CameraY );
	void CheckCollisions( void );

	MFlake& FindFlake( int _Ident );

	MFlake& GetFlake( int _Index ) { return m_vFlakes[_Index]; }

	int GetValueInFlakeAtIndex( int _flake, int _x, int _y );
	void SetValueInFlakeAtIndex( int _value, int _flake, int _x, int _y );

	int GetLayerWidth(void) { return LayerWidth; }
	int GetLayerHeight(void) { return LayerHeight; }

	int GetLayerID(void) { return LayerID; }

};
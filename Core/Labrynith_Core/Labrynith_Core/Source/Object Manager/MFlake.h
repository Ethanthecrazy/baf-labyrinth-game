#pragma once

#include <vector>
using namespace std;

#include "../BST.h"
#include "IUnitInterface.h"

class MLayer;

class MFlake 
{
private:

	int parentLayer;

	BST<int> ArrayIndex;
	//int* InformationArray;
	vector<int> InformationArray;
	vector<IUnitInterface*> m_vObjects;

	int m_nFlakeType;
	int m_nSize;

	int LayerWidth;
	int LayerHeight;

	int OffSetFromCenterX;
	int OffSetFromCenterY;

	void LightingProcess( int _X, int _Y );

public:

	MFlake( int _LayerWidth, int _LayerHeight, int _OffSetFromCenterX, int _OffSetFromCenterY, int _parentLayer );
	~MFlake();

	int AddUnit( IUnitInterface* _toAdd );
	int AddUnitIndexed( IUnitInterface* _toAdd );
	bool RemoveUnit( int _Ident );
	void RemoveAllUnits( void );

	IUnitInterface* GetUnit( int _Ident );

	void Update( float fDT );
	void Render( int CameraX, int CameraY );
	void CheckCollisions();
	int GetInfoAtIndex( int _x, int _y );
	void SetInfoAtIndex( int _x, int _y, int _value );

	int GetFlakeType(void) { return m_nFlakeType; }
	void SetFlakeType( int _type ) { m_nFlakeType = _type; }

	void FinishMovingEnt( IUnitInterface* toFinish );

	void Resize( int newWidth, int newHeight );

};
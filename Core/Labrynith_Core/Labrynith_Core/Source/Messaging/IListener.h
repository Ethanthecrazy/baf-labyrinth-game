#ifndef ILISTENER_H
#define ILISTENER_H

#include "Event.h"

class IListener
{

public:
	IListener( void ) {}
	virtual ~IListener( void ) {}

	virtual void HandleEvent( Event* _toHandle ) = 0;

};

#endif
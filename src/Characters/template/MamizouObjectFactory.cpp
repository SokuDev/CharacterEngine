//
// Created by PinkySmile on 08/05/24.
//

#include "MamizouObjectFactory.hpp"
#include "BasicObject.hpp"
#include "AnglableObject.hpp"
#include "RotatingBlockingObject.hpp"

#ifndef _DEBUG
#define printf(...)
#endif

SokuLib::v2::GameObject *MamizouObjectFactory::construct(SokuLib::v2::Player *owner, short action)
{
	printf("MamizouObjectFactory::construct(%i)\n", action);

	switch (action) {
	case 998:
		return new RotatingBlockingObject(120, 74, 40);
	default:
		return new BasicObject();
	}
}

//
// Created by PinkySmile on 08/05/24.
//

#include "DefaultFactory.hpp"
#include "RotatingBlockingObject.hpp"

SokuLib::v2::GameObject *DefaultFactory::construct(SokuLib::v2::Player *owner, short action)
{
	switch (action) {
	case 998:
		return new BLOCK_OBJECT();
	default:
		return nullptr;
	}
}

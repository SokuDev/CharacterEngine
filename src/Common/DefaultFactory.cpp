//
// Created by PinkySmile on 08/05/24.
//

#include "DefaultFactory.hpp"
#include "BlockingObject.hpp"

SokuLib::v2::GameObject *DefaultFactory::construct(SokuLib::v2::Player *owner, short action)
{
	switch (action) {
	case 998:
		return new BlockingObject();
	default:
		return nullptr;
	}
}

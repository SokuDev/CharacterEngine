//
// Created by PinkySmile on 08/05/24.
//

#include "TewiObjectFactory.hpp"
#include "TewiObject.hpp"
#include "DefaultFactory.hpp"
#include "Hammer.hpp"

SokuLib::v2::GameObject *TewiObjectFactory::construct(SokuLib::v2::Player *owner, short action)
{
	auto obj = DefaultFactory::construct(owner, action);

	if (obj)
		return obj;
	switch (action) {
	case 800:
		return new Hammer();
	default:
		return new TewiObject();
	}
}
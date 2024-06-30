//
// Created by PinkySmile on 08/05/24.
//

#include "TewiObjectFactory.hpp"
#include "BasicObject.hpp"
#include "DefaultFactory.hpp"
#include "Hammer.hpp"
#include "AnglableObject.hpp"
#include "RotatingBlockingObject.hpp"

SokuLib::v2::GameObject *TewiObjectFactory::construct(SokuLib::v2::Player *owner, short action)
{
	printf("TewiObjectFactory::construct(%i)\n", action);
	auto obj = DefaultFactory<RotatingBlockingObject>::construct(owner, action);

	if (obj)
		return obj;
	switch (action) {
	case 800:
		return new Hammer();
	case 801:
		return new AnglableObject();
	default:
		return new BasicObject();
	}
}
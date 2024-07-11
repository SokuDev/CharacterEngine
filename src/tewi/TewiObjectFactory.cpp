//
// Created by PinkySmile on 08/05/24.
//

#include "TewiObjectFactory.hpp"
#include "BasicObject.hpp"
#include "Hammer.hpp"
#include "AnglableObject.hpp"
#include "RotatingBlockingObject.hpp"
#include "SpinningSmoke.hpp"
#include "Rabbit.hpp"

SokuLib::v2::GameObject *TewiObjectFactory::construct(SokuLib::v2::Player *owner, short action)
{
	printf("TewiObjectFactory::construct(%i)\n", action);

	switch (action) {
	case 800:
		return new Hammer();
	case 801:
		return new AnglableObject({0, 5});
	case 802:
		return new Rabbit();
	case 849:
		return new SpinningSmoke();
	case 998:
		return new RotatingBlockingObject(120, 74, 40);
	default:
		return new BasicObject();
	}
}
//
// Created by PinkySmile on 08/05/24.
//

#include "TewiObjectFactory.hpp"
#include "BasicObject.hpp"
#include "AnglableObject.hpp"
#include "RotatingBlockingObject.hpp"
#include "objects/Hammer.hpp"
#include "objects/SpinningSmoke.hpp"
#include "objects/Rabbit.hpp"
#include "objects/TrapHole.hpp"
#include "objects/TaCRabbit.hpp"
#include "objects/UnexpectedAssistanceObject/Reimu.hpp"
#include "objects/UnexpectedAssistanceObject/Marisa.hpp"
#include "objects/UnexpectedAssistanceObject/Sakuya.hpp"
#include "objects/UnexpectedAssistanceObject/Alice.hpp"
#include "objects/UnexpectedAssistanceObject/Patchouli.hpp"
#include "objects/Bottle.hpp"
#include "objects/SpellTrapHole.hpp"
#include "objects/RabbitOrb.hpp"

#ifndef _DEBUG
#define printf(...)
#endif

SokuLib::v2::GameObject *TewiObjectFactory::construct(SokuLib::v2::Player *owner, short action)
{
	printf("TewiObjectFactory::construct(%i)\n", action);

	switch (action) {
	case 800:
		return new Hammer();
	case 801:
	case 806:
		return new AnglableObject({0, 5});
	case 802:
	case 805:
		return new Rabbit();
	case 803:
		return new TrapHole();
	case 804:
		return new TaCRabbit();
	case 807:
		return new Bottle();
	case 808:
		return new SpellTrapHole();
	case 809:
		return new RabbitOrb();
	case 810:
		return new UnexpectedAssistance::Reimu();
	case 811:
		return new UnexpectedAssistance::Marisa();
	case 812:
		return new UnexpectedAssistance::Sakuya();
	case 813:
		return new UnexpectedAssistance::Alice();
	case 814:
		return new UnexpectedAssistance::Patchouli();
	case 849:
		return new SpinningSmoke();
	case 998:
		return new RotatingBlockingObject(120, 74, 40);
	default:
		return new BasicObject();
	}
}
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
#include "TrapHole.hpp"
#include "TaCRabbit.hpp"
#include "UnexpectedAssistanceObject/Reimu.hpp"
#include "UnexpectedAssistanceObject/Marisa.hpp"
#include "UnexpectedAssistanceObject/Sakuya.hpp"
#include "UnexpectedAssistanceObject/Alice.hpp"
#include "UnexpectedAssistanceObject/Patchouli.hpp"

SokuLib::v2::GameObject *TewiObjectFactory::construct(SokuLib::v2::Player *owner, short action)
{
	printf("TewiObjectFactory::construct(%i)\n", action);

	switch (action) {
	case 800:
		return new Hammer();
	case 801:
		return new AnglableObject({0, 5});
	case 802:
	case 805:
		return new Rabbit();
	case 803:
		return new TrapHole();
	case 804:
		return new TaCRabbit();
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
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
#include "objects/Bottle.hpp"
#include "objects/SpellTrapHole.hpp"
#include "objects/RabbitOrb.hpp"
#include "objects/KillingRabbit.hpp"
#include "objects/RabbitSolidarity.hpp"
#include "objects/SaSOrb.hpp"
#include "objects/UBRabbit.hpp"
#include "objects/IllusionRabbit.hpp"
#include "objects/SubterraneanAnimism.hpp"
#include "objects/Bamboo.hpp"
#include "objects/UnexpectedAssistanceObject/Reimu.hpp"
#include "objects/UnexpectedAssistanceObject/Marisa.hpp"
#include "objects/UnexpectedAssistanceObject/Sakuya.hpp"
#include "objects/UnexpectedAssistanceObject/Alice.hpp"
#include "objects/UnexpectedAssistanceObject/Patchouli.hpp"
#include "objects/UnexpectedAssistanceObject/Youmu.hpp"
#include "objects/UnexpectedAssistanceObject/Remilia.hpp"
#include "objects/UnexpectedAssistanceObject/Yuyuko.hpp"
#include "objects/UnexpectedAssistanceObject/Yukari.hpp"
#include "objects/UnexpectedAssistanceObject/Suika.hpp"
#include "objects/UnexpectedAssistanceObject/Reisen.hpp"
#include "objects/UnexpectedAssistanceObject/Aya.hpp"
#include "objects/UnexpectedAssistanceObject/Komachi.hpp"
#include "objects/UnexpectedAssistanceObject/Iku.hpp"
#include "objects/UnexpectedAssistanceObject/Tenshi.hpp"
#include "objects/UnexpectedAssistanceObject/Sanae.hpp"

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
		return new AnglableObject({0, 5}, 1, 1);
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
	case 815:
		return new UnexpectedAssistance::Youmu();
	case 816:
		return new UnexpectedAssistance::Remilia();
	case 817:
		return new UnexpectedAssistance::Yuyuko();
	case 818:
		return new UnexpectedAssistance::Yukari();
	case 819:
		return new UnexpectedAssistance::Suika();
	case 820:
		return new UnexpectedAssistance::Reisen();
	case 821:
		return new UnexpectedAssistance::Aya();
	case 822:
		return new UnexpectedAssistance::Komachi();
	case 823:
		return new UnexpectedAssistance::Iku();
	case 824:
		return new UnexpectedAssistance::Tenshi();
	case 825:
		return new UnexpectedAssistance::Sanae();
	case 849:
		return new SpinningSmoke();
	case 850:
		return new KillingRabbit();
	case 851:
	case 852:
		return new RabbitSolidarity();
	case 853:
		return new SaSOrb();
	case 854:
	case 855:
		return new UBRabbit();
	case 856:
		return new IllusionRabbit();
	case 857:
	case 858:
		return new SubterraneanAnimism();
	case 859:
	case 860:
		return new Bamboo();
	case 998:
		return new RotatingBlockingObject(120, 74, 40);
	default:
		return new BasicObject();
	}
}
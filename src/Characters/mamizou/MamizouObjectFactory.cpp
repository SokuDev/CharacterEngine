//
// Created by PinkySmile on 08/05/24.
//

#include "MamizouObjectFactory.hpp"
#include "BasicObject.hpp"
#include "AnglableObject.hpp"
#include "RotatingBlockingObject.hpp"
#include "objects/FrogShadow.hpp"
#include "objects/Smoke.hpp"
#include "objects/YoukaiShadow.hpp"
#include "objects/YoukaiShadow2.hpp"
#include "objects/Gui/TimerGui.hpp"
#include "objects/Gui/StackGui.hpp"
#include "objects/LeafSpawner.hpp"
#include "objects/SmokeBullet.hpp"
#include "objects/Umbrella2C.hpp"
#include "objects/BirdShadow.hpp"
#include "objects/BucketYoukai.hpp"
#include "objects/Umbrella5C.hpp"
#include "objects/CrabYoukai.hpp"
#include "objects/LanternYoukai.hpp"
#include "objects/RegularSmoke.hpp"
#include "objects/HotPot.hpp"

#ifndef _DEBUG
#define printf(...)
#endif

SokuLib::v2::GameObject *MamizouObjectFactory::construct(SokuLib::v2::Player *owner, short action)
{
	printf("MamizouObjectFactory::construct(%i)\n", action);

	switch (action) {
	case 800:
		return new LeafSpawner();
	case 801:
		return new BucketYoukai();
	case 802:
		return new Smoke();
	case 803:
		return new YoukaiShadow();
	case 804:
		return new FrogShadow();
	case 805:
		return new YoukaiShadow2();
	case 806:
		return new SmokeBullet();
	case 807:
		return new Umbrella2C();
	case 808:
		return new Umbrella5C();
	case 809:
		return new BirdShadow();
	case 810:
		return new RegularSmoke();
	case 820:
		return new CrabYoukai();
	case 825:
		return new LanternYoukai();
	case 850:
		return new HotPot();
	case 996:
		return new TimerGui();
	case 997:
		return new StackGui();
	case 998:
		return new RotatingBlockingObject(150, 100, 40);
	default:
		return new BasicObject();
	}
}

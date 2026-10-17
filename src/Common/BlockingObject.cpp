//
// Created by PinkySmile on 08/05/24.
//

#include "BlockingObject.hpp"

#define offsetY(t) (*reinterpret_cast<std::tuple<unsigned char, unsigned char, unsigned char> *>(&t->data[0]))

BlockingObject::BlockingObject(unsigned char offsetYAir, unsigned char offsetYHigh, unsigned char offsetYLow)
{
	offsetY(this) = { offsetYHigh, offsetYAir, offsetYLow };
}

void BlockingObject::update()
{
	this->renderInfos.color.a -= 15;
	this->parentPlayer->blockObjectSpawned = true;
	if (SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN <= this->parentPlayer->frameState.actionId && this->parentPlayer->frameState.actionId <= SokuLib::ACTION_AIR_GUARD) {
		float yOffset;

		if (this->parentPlayer->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_HIGH_HUGE_BLOCKSTUN)
			yOffset = std::get<0>(offsetY(this));
		else if (this->parentPlayer->frameState.actionId == SokuLib::ACTION_AIR_GUARD)
			yOffset = std::get<1>(offsetY(this));
		else //if (SokuLib::ACTION_RIGHTBLOCK_LOW_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_LOW_HUGE_BLOCKSTUN)
			yOffset = std::get<2>(offsetY(this));
		this->direction = this->parentPlayer->direction;
		this->position.x = this->parentPlayer->position.x + (float)(this->direction * 57);
		this->position.y = this->parentPlayer->position.y + yOffset;
		this->renderInfos.color.a = 255;
	} else if (this->renderInfos.color.a == 0) {
		this->parentPlayer->blockObjectSpawned = false;
		this->lifetime = 0;
	}
}

void BlockingObject::initializeAction()
{
	this->renderInfos.yRotation = 60;
}

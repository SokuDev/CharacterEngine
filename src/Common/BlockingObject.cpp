//
// Created by PinkySmile on 08/05/24.
//

#include "BlockingObject.hpp"

BlockingObject::BlockingObject(unsigned char offsetYAir, unsigned char offsetYHigh, unsigned char offsetYLow)
{
	this->offsetY = { offsetYHigh, offsetYAir, offsetYLow };
}

void BlockingObject::update()
{
	this->renderInfos.color.a -= 15;
	this->parentPlayer->blockObjectSpawned = true;
	if (SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN <= this->parentPlayer->frameState.actionId && this->parentPlayer->frameState.actionId <= SokuLib::ACTION_AIR_GUARD) {
		float yOffset;

		if (this->parentPlayer->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_HIGH_HUGE_BLOCKSTUN)
			yOffset = std::get<0>(this->offsetY);
		else if (this->parentPlayer->frameState.actionId == SokuLib::ACTION_AIR_GUARD)
			yOffset = std::get<1>(this->offsetY);
		else //if (SokuLib::ACTION_RIGHTBLOCK_LOW_SMALL_BLOCKSTUN <= this->frameState.actionId && this->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_LOW_HUGE_BLOCKSTUN)
			yOffset = std::get<2>(this->offsetY);
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

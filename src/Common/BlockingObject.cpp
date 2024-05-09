//
// Created by PinkySmile on 08/05/24.
//

#include "BlockingObject.hpp"

void BlockingObject::update()
{
	this->renderInfos.zRotation += 4;
	this->renderInfos.color.a -= 15;
	this->parentPlayer->blockObjectSpawned = true;
	if (SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN <= this->parentPlayer->frameState.actionId && this->parentPlayer->frameState.actionId <= SokuLib::ACTION_AIR_GUARD) {
		float yOffset;

		if (this->parentPlayer->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_HIGH_HUGE_BLOCKSTUN)
			yOffset = 94;
		else if (this->frameState.actionId == SokuLib::ACTION_AIR_GUARD)
			yOffset = 100;
		else //if (SokuLib::ACTION_RIGHTBLOCK_LOW_SMALL_BLOCKSTUN <= this->parentPlayer->frameState.actionId && this->parentPlayer->frameState.actionId <= SokuLib::ACTION_RIGHTBLOCK_LOW_HUGE_BLOCKSTUN)
			yOffset = 60;
		this->direction = this->parentPlayer->direction;
		this->position.x = this->parentPlayer->position.x + (float)(this->direction * 57);
		this->position.y = this->parentPlayer->position.y + yOffset;
		this->renderInfos.color.a = 255;
	} else if (this->renderInfos.color.a == 0) {
		this->parentPlayer->blockObjectSpawned = false;
		this->lifetime = 0;
	}
}

bool BlockingObject::initializeAction()
{
	this->renderInfos.yRotation = 60;
	return false;
}

SokuLib::v2::GameObject *BlockingObject::createObject(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize)
{
	return nullptr;
}

SokuLib::v2::GameObject *BlockingObject::createChild(short action, float x, float y, SokuLib::Direction direction, char layer, float *extraData, unsigned int extraDataSize)
{
	return nullptr;
}

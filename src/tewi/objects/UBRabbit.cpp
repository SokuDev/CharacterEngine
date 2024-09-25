//
// Created by PinkySmile on 21/09/24.
//

#include "UBRabbit.hpp"

void UBRabbit::update()
{
	if (this->frameState.actionId == 854) {
		this->position.x += this->direction;
		if (this->advanceFrame())
			this->lifetime = 0;
		return;
	}
	this->advanceFrame();
	if (this->frameState.currentFrame > 35)
		this->lifetime = 0;
	else if (this->frameState.currentFrame > 20)
		this->renderInfos.color.a = (35 - this->frameState.currentFrame) * 255 / 15;
	this->position.x += this->speed.x;
	if (this->frameState.poseId == this->frameState.sequenceSize - 1) {
		this->position.y += this->speed.y;
		this->speed.y -= 0.75;
	}
}

void UBRabbit::initializeAction()
{
	if (this->frameState.actionId == 854)
		return;

	this->speed.y = min(22.5, 7.5 + (this->gameData.opponent->position.y - this->position.y) / 25);
	this->speed.x = 6 * this->direction;
	this->collisionLimit = 1;
}

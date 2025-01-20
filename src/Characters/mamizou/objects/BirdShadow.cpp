//
// Created by PinkySmile on 19/01/25.
//

#include "BirdShadow.hpp"

void BirdShadow::update()
{
	if (
		this->parentPlayerB->frameState.actionId != SokuLib::ACTION_j8A ||
		this->parentPlayerB->frameState.sequenceId == 2
	) {
		if (this->frameState.sequenceId == 2) {
			this->nextSequence();
			this->speed = {0, 0};
		} else if (this->frameState.sequenceId != 3)
			this->lifetime = 0;
	}
	if (this->parentPlayerB->timeStop)
		return;
	if (this->parentPlayerB->hitStop && this->frameState.sequenceId != 3)
		return;
	if (this->frameState.sequenceId == 1 && this->frameState.currentFrame == 0) {
		float params[1] = {2};

		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	}
	this->advanceFrame();
	if (this->frameState.sequenceId != 3) {
		this->position.x = this->parentPlayerB->position.x + this->parentPlayerB->speed.x * this->parentPlayerB->direction;
		this->position.y = this->parentPlayerB->position.y + this->parentPlayerB->speed.y;
	} else {
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		this->speed.x += 0.1;
		this->speed.y += 1;
		if (this->position.y >= 1000)
			this->lifetime = 0;
	}
}

void BirdShadow::initializeAction()
{
	if (this->customData != nullptr)
		this->setSequence(this->customData[0]);
}

//
// Created by PinkySmile on 06/11/24.
//

#include "Namazu.hpp"

void UnexpectedAssistance::Namazu::update()
{
	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	switch (this->frameState.sequenceId) {
	case 0:
		this->speed.y -= 0.25;
		this->position.y += this->speed.y;
		if (this->isOnGround()) {
			this->parentPlayerB->playSFX(53);
			this->position.y = this->getGroundHeight();
			this->nextSequence();
		}
		break;
	case 2:
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0) {
			this->gpShort[0]++;
			this->collisionLimit = 1;
			this->collisionType = COLLISION_TYPE_NONE;
		}
		if (this->gpShort[0] > 6)
			this->nextSequence();
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 1)
			this->parentPlayerB->playSFX(52);
		break;
	case 4:
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0)
			this->parentPlayerB->playSFX(54);
	}
	if (this->advanceFrame())
		this->lifetime = 0;
}

void UnexpectedAssistance::Namazu::initializeAction()
{
	this->layer = 1;
	this->direction = -this->gameData.opponent->direction;
	this->collisionLimit = 1;
	this->position.x = this->gameData.opponent->position.x;
	this->position.y = 1200;
	this->parentPlayerB->playSFX(51);
}

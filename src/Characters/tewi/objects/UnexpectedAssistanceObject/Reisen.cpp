//
// Created by PinkySmile on 26/10/24.
//

#include "Reisen.hpp"

void UnexpectedAssistance::Reisen::update()
{
	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	if (this->collisionType == 5) {
		this->lifetime = 0;
		return;
	}
	this->checkGrazed(0);
	if (this->frameState.sequenceId < 2) {
		if (this->collisionType != COLLISION_TYPE_NONE) {
			if (this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_HIGH_DENSITY)
				this->collisionLimit = 0;
			this->gpShort[0]++;
			if (this->gpShort[0] >= 4) {
				this->gpShort[0] = 0;
				this->collisionType = COLLISION_TYPE_NONE;
			}
		}
		if (this->frameState.currentFrame > 60 || this->collisionLimit == 0) {
			this->nextSequence();
			return;
		}
		this->renderInfos.zRotation -= 0.2;
	}
	if (this->advanceFrame())
		this->lifetime = 0;
}

void UnexpectedAssistance::Reisen::initializeAction()
{
	this->position.x = (SokuLib::rand(1000) < 500) * 1480 - 100;
	this->parentPlayerB->playSFX(33);
	this->direction = this->position.x < 0 ? SokuLib::RIGHT : SokuLib::LEFT;
	this->position.y = SokuLib::rand(1000);
	this->gpShort[1] = 2;
	this->collisionLimit = 20;

	auto target = fmod(atan2(this->gameData.opponent->position.y + 75 - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI + 360, 360);

	if (this->direction == SokuLib::LEFT) {
		this->renderInfos.zRotation = 180 + target + 5;
	} else
		this->renderInfos.zRotation = 360 - target + 5;
	this->renderInfos.scale.x = 4.5;
	this->renderInfos.scale.y = 3.0;
}
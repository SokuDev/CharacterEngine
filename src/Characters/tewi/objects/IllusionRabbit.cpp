//
// Created by PinkySmile on 25/09/24.
//

#include "IllusionRabbit.hpp"

void IllusionRabbit::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->frameState.sequenceId > 1) {
		if (this->renderInfos.color.a <= 5) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.zRotation += this->direction * 2;
		this->renderInfos.color.a -= 5;
		this->position.x += this->customData[0];
		this->position.y += this->customData[1];
		this->customData[1] -= 0.5;
		return;
	}
	if (this->frameState.sequenceId == 1) {
		if (this->advanceFrame())
			this->lifetime = 0;
		return;
	}
	this->advanceFrame();
	this->position.x += this->speed.x * this->direction;
	this->position.y += this->speed.y;
	this->speed.y -= this->gravity.y;
	if (this->renderInfos.color.a != 255) {
		if (this->renderInfos.color.a <= 11)
			this->lifetime = 0;
		this->renderInfos.color.a -= 11;
		return;
	}
	if (this->collisionType == COLLISION_TYPE_HIT) {
		this->nextPose();
		this->collisionType = COLLISION_TYPE_NONE;
		this->position = this->gameData.opponent->position;
		this->speed = {0, 0};
		this->gravity.y = 0;
		return;
	} else if (this->collisionType) {
		this->renderInfos.color.a -= 11;
		this->parentPlayerB->nextPose();
	}
	if (this->gravity.y && this->isOnGround())
		this->renderInfos.color.a -= 11;
	if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
		this->parentPlayerB->playSFX(3);
		this->speed = {10, 12};
		this->gravity.y = 0.5;
	}
	if (this->frameState.poseId == 4 && this->isOnGround())
		this->lifetime = 0;
	if (this->frameState.poseId == 5)
		this->position = this->gameData.opponent->position;
	if (this->frameState.poseId == 6 && this->frameState.poseFrame == 0) {
		this->speed = {0, 20};
		this->parentPlayerB->playSFX(3);
		this->gravity.y = 0.5;
		this->renderInfos.color.a -= 11;
	}
}

void IllusionRabbit::initializeAction()
{
	if (!this->customData) {
		float f[1];

		this->collisionLimit = 1;
		this->createObject(856, this->position.x, this->position.y, this->direction, 1, f);
		return;
	}
	this->setSequence(1);
}

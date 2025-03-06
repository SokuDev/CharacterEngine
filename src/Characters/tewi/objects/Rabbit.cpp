//
// Created by PinkySmile on 09/07/24.
//

#include "Rabbit.hpp"
#include "../Tewi.hpp"

void Rabbit::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->customData[2] != 0 && this->checkTurnIntoCrystals(false, 2, 2)) {
		this->lifetime = 0;
		return;
	}
	this->advanceFrame();
	this->checkGrazed(this->customData[2]);
	this->checkProjectileHit(this->customData[2]);
	if (this->customData[2] > 0 && this->collisionType != COLLISION_TYPE_NONE) {
		this->renderInfos.color.a -= 15;
		if (!this->renderInfos.color.a)
			this->lifetime = 0;
	} else if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0) {
		this->speed.x = this->customData[0];
		this->speed.y = this->customData[1];
		this->parentPlayer->playSFX(3);
	}
	if (this->frameState.poseId == 4 && this->isOnGround()) {
		this->resetForces();
		this->nextPose();
	}
	this->position.x += this->speed.x * this->direction;
	this->position.y += this->speed.y;
	if (this->isOnGround())
		this->position.y = this->getGroundHeight();
	this->speed.y -= 0.5;
	if (this->position.x < 0 || this->position.x > 1300)
		this->lifetime = 0;
}

void Rabbit::initializeAction()
{
	this->collisionLimit = 1;
}

//
// Created by PinkySmile on 15/01/25.
//

#include <cmath>
#include "SmokeBullet.hpp"

void SmokeBullet::update()
{
	this->updateSpeedAngled(this->customData[0], this->customData[1]);
	this->position += this->speed;
	this->customData[1] *= 0.90;
	this->renderInfos.zRotation += this->gpFloat[0];
	if (this->checkTurnIntoCrystals(false, 0, 4)) {
		this->lifetime = 0;
		return;
	}
	if (this->checkGrazed(5)) {
		this->lifetime = 0;
		return;
	}
	if (this->checkProjectileHit(1)) {
		this->lifetime = 0;
		return;
	}
	if (this->frameState.currentFrame >= 30) {
		if (this->renderInfos.color.a < 15) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 15;
	}
	if (this->advanceFrame())
		this->lifetime = 0;
}

void SmokeBullet::initializeAction()
{
	this->collisionType = COLLISION_TYPE_NONE;
	this->collisionLimit = 1;
	this->setSequence(this->customData[2]);
	this->gpFloat[0] = std::copysign(SokuLib::rand(100) / 10.f, SokuLib::rand(100) - 50);
}

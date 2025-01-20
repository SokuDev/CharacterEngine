//
// Created by PinkySmile on 19/01/25.
//

#include "Umbrella2C.hpp"

void Umbrella2C::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
	if (this->frameState.poseId == 0 && !this->isOnGround())
		this->nextPose();
	if (this->frameState.poseId == 3 && this->frameState.poseFrame > 40 || (
		this->collisionLimit == 0 && (
			this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_SAME_DENSITY ||
			this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_HIGH_DENSITY
		)
	)) {
		if (this->renderInfos.color.a <= 15) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 15;
	}
	if (this->checkTurnIntoCrystals(false, 2, 5)) {
		this->lifetime = 0;
		return;
	}
	if (this->checkGrazed(0)) {
		this->lifetime = 0;
		return;
	}
	this->checkProjectileHit(5);
	this->position.y += this->speed.y;
	this->speed.y -= this->gravity.y;
	if (this->speed.y < -2)
		this->speed.y = -2;
}

void Umbrella2C::initializeAction()
{
	this->collisionType = COLLISION_TYPE_NONE;
	this->collisionLimit = 1;
	this->speed.y = 20;
	this->gravity.y = 0.5;
}

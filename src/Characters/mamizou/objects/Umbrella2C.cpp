//
// Created by PinkySmile on 19/01/25.
//

#include "Umbrella2C.hpp"

void Umbrella2C::update()
{
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
	if (this->frameState.sequenceId == 1) {
		if (this->parentB == nullptr)
			return;
		this->lifetime = this->parentB->lifetime;
		this->position = this->parentB->position;
		this->speed = this->parentB->speed;
		if (this->gpShort[0] == 1)
			this->renderInfos.color.a = this->parentB->renderInfos.color.a;
		else if (this->renderInfos.color.a < 220)
			this->renderInfos.color.a += 35;
		else {
			this->gpShort[0] = 1;
			this->renderInfos.color.a = 255;
		}
		return;
	}
	if (this->frameState.poseId == 0 && !this->isOnGround())
		this->nextPose();
	if (this->frameState.poseId == 3 && this->frameState.poseFrame == 0)
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, this->layer);
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
	if (this->parentObject) {
		this->setSequence(1);
		this->renderInfos.color.a = 0;
	} else {
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
		this->speed.y = 20;
		this->gravity.y = 0.5;
	}
}

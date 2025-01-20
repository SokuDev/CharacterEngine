//
// Created by PinkySmile on 19/01/25.
//

#include "BucketYoukai.hpp"

#define angularVelocity gpFloat[0]
#define angularAcceleration gpFloat[1]

void BucketYoukai::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	this->advanceFrame();
	this->renderInfos.zRotation -= this->angularVelocity;
	this->angularAcceleration *= 0.99;
	if (this->renderInfos.zRotation > -90)
		this->angularVelocity += this->angularAcceleration;
	else
		this->angularVelocity -= this->angularAcceleration;
	this->checkGrazed(0);
	this->checkProjectileHit(2);
	if (
		this->parentPlayer->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
		this->parentPlayer->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN
	)
		this->collisionLimit = 0;
	if (this->renderInfos.zRotation < -180)
		this->lifetime = 0;
}

void BucketYoukai::initializeAction()
{
	this->collisionLimit = 1;
	this->collisionType = COLLISION_TYPE_NONE;
	this->angularVelocity = 3;
	this->angularAcceleration = 0.1;
}

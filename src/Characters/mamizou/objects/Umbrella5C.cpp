//
// Created by PinkySmile on 19/01/25.
//

#include <cmath>
#include "Umbrella5C.hpp"

#define maxDrift gpFloat[0]
#define angle gpFloat[1]
#define velocity gpShort[0]

void Umbrella5C::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->advanceFrame())
		this->lifetime = 0;
	if (this->frameState.sequenceId == 2 && this->frameState.poseId == 3) {
		if (this->renderInfos.color.a <= 15)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 15;
		return;
	}
	if (this->frameState.sequenceId != 1)
		return;

	float expected = -std::atan2((this->gameData.opponent->position.y + 100) - this->position.y, (this->gameData.opponent->position.x - this->position.x) * this->direction) * 180 / M_PI;

	if (expected - this->angle > 180)
		expected -= 360;
	else if (expected - this->angle < -180)
		expected += 360;
	if (std::abs(expected - this->angle) < this->maxDrift)
		this->angle = expected;
	else if (expected > this->angle)
		this->angle += this->maxDrift;
	else
		this->angle -= this->maxDrift;
	this->angle = fmod(this->angle, 360);
	this->updateSpeedAngled(this->angle, this->velocity);
	this->renderInfos.zRotation = this->angle;
	this->checkGrazed(0);
	this->checkProjectileHit(1);
	if (this->checkTurnIntoCrystals(false, 2, 5)) {
		this->lifetime = 0;
		return;
	}
	if (this->collisionType != COLLISION_TYPE_NONE)
		this->collisionType = COLLISION_TYPE_NONE;
	this->position.x += this->speed.x * this->direction;
	this->position.y += this->speed.y;
	if (this->collisionLimit == 0)
		this->nextSequence();
	if (this->position.x < -100 || this->position.x > 1380)
		this->lifetime = 0;
}

void Umbrella5C::initializeAction()
{
	this->angle = this->customData[0];
	this->velocity = this->customData[1];
	this->maxDrift = this->customData[2];
	this->renderInfos.zRotation = this->angle;
	this->collisionLimit = 4;
	this->collisionType = COLLISION_TYPE_NONE;
}

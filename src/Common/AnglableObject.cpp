//
// Created by PinkySmile on 22/06/24.
//

#include <cmath>
#include "AnglableObject.hpp"

AnglableObject::AnglableObject(std::pair<char, char> crystalsSpawn, unsigned char grazeResist, unsigned char density)
{
	this->crystalsSpawn = crystalsSpawn;
	this->grazeResist = grazeResist;
	this->density = density;
}

void AnglableObject::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	this->position += this->speed;
	this->advanceFrame();
	if (this->frameState.sequenceId == 0) {
		if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->checkTurnIntoCrystals(false, this->crystalsSpawn.first, this->crystalsSpawn.second)) {
			this->lifetime = 0;
			return;
		}
		if (this->checkGrazed(this->grazeResist)) {
			this->nextSequence();
			return;
		}
		if (this->checkProjectileHit(this->density)) {
			this->nextSequence();
			return;
		}
	}
	if (this->frameState.sequenceId == 1) {
		this->renderInfos.color.a -= 255 / this->frameState.poseDuration;
		if (this->checkTurnIntoCrystals(false, 0, 0)) {
			this->lifetime = 0;
			return;
		}
	}
	if (this->customData[1] > this->customData[2]) {
		this->customData[1] = this->customData[1] - this->customData[2];
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1] * this->direction;
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
	} else {
		this->customData[1] = 0;
		this->speed = {0, 0};
	}
	if (this->frameState.sequenceId != 0)
		return;
	if (this->collisionLimit == 0) {
		this->nextSequence();
		return;
	}
}

void AnglableObject::initializeAction()
{
	this->renderInfos.zRotation = -this->customData[0];
	this->speed = {
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1] * this->direction,
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1]
	};
}

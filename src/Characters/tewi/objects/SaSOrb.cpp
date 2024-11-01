//
// Created by PinkySmile on 18/09/24.
//

#include <cmath>
#include "../Tewi.hpp"
#include "SaSOrb.hpp"

#define nbCollision gpShort[0]
#define rotationSpeed gpFloat[0]

void SaSOrb::_checkCollisionWithOwner()
{
	if (!this->parentPlayerB->hasMoveBeenReset)
		return;
	for (int i = 0; i < this->parentPlayerB->boxData.hitBoxCount; i++) {
		auto hit = this->parentPlayerB->boxData.hitBoxes[i];

		for (int j = 0; j < this->boxData.hurtBoxCount; j++) {
			auto hurt = this->boxData.hurtBoxes[j];

			if (hurt.top > hit.bottom)
				continue;
			if (hurt.bottom < hit.top)
				continue;
			if (hurt.left > hit.right)
				continue;
			if (hurt.right < hit.left)
				continue;
			this->parentPlayerB->hasMoveBeenReset = false;
			SokuLib::playSEWaveBuffer(this->parentPlayerB->boxData.frameData->onHitSFX);
			this->createEffect(
				max(1, this->parentPlayerB->boxData.frameData->onHitFX - 1),
				hit.left + (hit.right - hit.left) / 2,
				-hit.bottom + (-hit.top - -hit.bottom) / 2,
				this->direction, 1
			);
			this->speed = this->parentPlayerB->boxData.frameData->onHitSpeed;
			this->speed.x *= this->direction;

			float target = (this->parentPlayerB->boxData.frameData->attackType * 5 + 15) * this->direction;

			this->rotationSpeed += (target - this->rotationSpeed) / 2;
			return;
		}
	}
}

void SaSOrb::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	this->advanceFrame();
	if (this->frameState.sequenceId == 1) {
		if (this->checkTurnIntoCrystals(false, 0, 0) || this->renderInfos.color.a <= 10) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 10;
		return;
	}
	if (this->checkTurnIntoCrystals(false, 5, 5)) {
		this->lifetime = 0;
		return;
	}
	if ((
		this->parentPlayerB->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206 ||
		this->parentPlayerB->frameState.actionId == Tewi::ACTION_USING_SC_ID_206_HAMMER
	) && this->parentPlayerB->frameState.sequenceId != 0) {
		this->nextSequence();
		return;
	}
	if (this->checkGrazed(10)) {
		this->nextSequence();
		return;
	}
	if (this->checkProjectileHit(1)) {
		this->nextSequence();
		return;
	}
	this->_checkCollisionWithOwner();
	if (this->collisionType) {
		if (this->nbCollision == 0)
			this->nbCollision = 4;
		else
			this->nbCollision--;
		if (this->nbCollision == 0)
			this->collisionType = COLLISION_TYPE_NONE;
	}
	if (this->frameState.currentFrame % 4 == 0)
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1)->renderInfos.zRotation = this->renderInfos.zRotation;
	this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + this->rotationSpeed, 360);
	this->position.x += this->speed.x;
	this->position.y += this->speed.y;
	this->speed.y -= this->gravity.y;
	if (this->isOnGround()) {
		this->position.y = this->getGroundHeight();
		this->speed.x *= 0.8f;
		this->speed.y *= -0.8f;
		if (this->customData[2] == 0)
			this->lifetime = 0;
		this->customData[2]--;
	}
	if (
		(this->position.x <= 40 && this->speed.x < 0) ||
		(this->position.x >= 1240 && this->speed.x > 0)
	) {
		if (this->position.x <= 40)
			this->position.x = 40;
		else
			this->position.x = 1240;
		this->speed.x *= -0.8f;
		this->speed.y *= 0.8f;
	}
	if (this->collisionLimit == 0) {
		this->renderInfos.color.g = 100;
		this->renderInfos.color.b = 100;
	}
}

void SaSOrb::initializeAction()
{
	if (!this->customData) {
		this->renderInfos.color = this->parentObject->renderInfos.color;
		this->renderInfos.color.a = 0xA0;
		this->setSequence(1);
		return;
	}
	this->gravity.y = 0.5;
	this->collisionLimit = 5;
	this->speed.x = std::cos(this->customData[0] * M_PI / 180) * this->customData[1];
	this->speed.y = std::sin(this->customData[0] * M_PI / 180) * this->customData[1];
	this->renderInfos.zRotation = SokuLib::rand(360);
	this->rotationSpeed = (SokuLib::rand(5) + 5) * this->direction;
}
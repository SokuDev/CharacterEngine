//
// Created by PinkySmile on 06/08/24.
//

#include <cmath>
#include "RabbitOrb.hpp"
#include "../Tewi.hpp"

#define nbCollision gpShort[0]

void RabbitOrb::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	this->advanceFrame();
	if (this->frameState.sequenceId == 0) {
		if ((
			this->parentPlayerB->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206 ||
			this->parentPlayerB->frameState.actionId == Tewi::ACTION_USING_SC_ID_206_HAMMER
		) && this->parentPlayerB->frameState.sequenceId != 0) {
			this->nextSequence();
			return;
		}
		if (this->checkTurnIntoCrystals(false, 5, 5)) {
			this->lifetime = 0;
			return;
		}
		if (this->checkGrazed(15)) {
			this->nextSequence();
			return;
		}
		if (this->checkProjectileHit(1)) {
			this->nextSequence();
			return;
		}
		if (this->collisionType) {
			if (this->nbCollision == 0)
				this->nbCollision = 4;
			else
				this->nbCollision--;
			if (this->nbCollision == 0)
				this->collisionType = COLLISION_TYPE_NONE;
		}
		if (this->frameState.currentFrame % 2 == 0)
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1)->renderInfos.zRotation = this->renderInfos.zRotation;
		this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
		this->position += this->speed;
		this->speed.y -= this->gravity.y;
		if (this->isOnGround()) {
			this->position.y = this->getGroundHeight();
			this->speed.x *= 0.8f;
			this->speed.y *= -0.8f;
			if (this->customData[2] == 0)
				this->lifetime = 0;
			this->customData[2]--;
		}
		if (this->position.x <= 40 || this->position.x >= 1240) {
			if (this->position.x <= 40)
				this->position.x = 40;
			else
				this->position.x = 1240;
			this->speed.x *= -0.8f;
			this->speed.y *= 0.8f;
		}
		return;
	}
	if (this->checkTurnIntoCrystals(false, 0, 0) || this->renderInfos.color.a <= 10) {
		this->lifetime = 0;
		return;
	}
	this->renderInfos.color.a -= 10;
}

void RabbitOrb::initializeAction()
{
	if (!this->customData) {
		this->renderInfos.color.a = 0xA0;
		this->setSequence(1);
		return;
	}
	this->gravity.y = 0.5;
	this->collisionLimit = 5;
	this->speed.x = std::cos(this->customData[0] * M_PI / 180) * this->customData[1];
	this->speed.y = std::sin(this->customData[0] * M_PI / 180) * this->customData[1];
	this->renderInfos.zRotation = SokuLib::rand(360);
}

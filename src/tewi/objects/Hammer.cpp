//
// Created by PinkySmile on 08/05/24.
//

#include <cmath>
#include "Hammer.hpp"
#include "../Tewi.hpp"

void Hammer::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->frameState.sequenceId == 10) {
		this->renderInfos.zRotation = this->customData[1];
		if (this->renderInfos.color.a < 25)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 25;
	} else if (this->frameState.sequenceId == 9) {
		if (
			(this->position.x <= SokuLib::camera.leftEdge && this->speed.x <= 0) ||
			(this->position.x >= SokuLib::camera.rightEdge && this->speed.x >= 0)
		) {
			this->speed.x = -this->speed.x;
			this->collisionLimit = 1;
			this->collisionType = COLLISION_TYPE_NONE;
			this->parentPlayerB->playSFX(1);
		}
		if (this->position.y <= this->getGroundHeight() && this->speed.y < 0) {
			this->speed.y = -this->speed.y;
			this->parentPlayerB->playSFX(1);
		}
		if (this->position.y >= SokuLib::camera.topEdge + 150 && this->speed.y >= 0) {
			this->collisionLimit = 0;
			this->customData[3] = 0;
			this->customData[2] = 0;
			this->speed.x = 0;
			this->speed.y = 0;
			this->position.x = this->parentPlayerB->position.x;
			this->gravity.y = 0.25;
			this->setSequence(7);
		}

		float params[2] = {370, this->renderInfos.zRotation};

		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
		this->position += this->speed;
		this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
		this->parentPlayerB->spellBgTimer = 10;
	} else if (this->frameState.sequenceId == 8) {
		if (this->checkTurnIntoCrystals(false, 1, 5)) {
			this->setSequence(0);
			this->gravity.y = 1;
			this->customData[3] = 0;
			this->customData[2] = 1;
			this->skillIndex = -1;
			this->collisionLimit = 0;
			return;
		}
		if (this->customData[3] == 0) {
			this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
			this->position += this->speed;
		}
		this->speed.x = std::cos(this->customData[0] * M_PI / 180) * this->customData[1];
		this->speed.y = std::sin(this->customData[0] * M_PI / 180) * this->customData[1];
		if (this->customData[4] == 0 && std::pow(this->parentPlayerB->position.x - this->position.x, 2) + std::pow(this->parentPlayerB->position.y + 75 - this->position.y, 2) < 10000) {
			short action;

			if (!this->parentPlayerB->isGrounded())
				action = Tewi::ACTION_AIR_PICKUP_HAMMER_FROM_AIR;
			else
				action = Tewi::ACTION_STAND_PICKUP_HAMMER_FROM_AIR;
			this->parentPlayerB->setAction(action);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SLAP_HIT);
			return;
		}
		this->customData[0] = fmod(atan2(this->parentPlayerB->position.y + 75 - this->position.y, this->parentPlayerB->position.x - this->position.x) * 180 / M_PI + 360, 360);
	} else if (this->frameState.sequenceId == 6) {
		this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
		this->position += this->speed;
		this->speed.x += (this->parentPlayer->position.x - this->position.x) / 1000;
		this->speed.y += (this->parentPlayer->position.y + 75 - this->position.y) / 1000;
		if (this->customData[4] == 0 && std::pow(this->parentPlayerB->position.x - this->position.x, 2) + std::pow(this->parentPlayerB->position.y + 75 - this->position.y, 2) < 10000) {
			short action;

			if (!this->parentPlayerB->isGrounded())
				action = Tewi::ACTION_AIR_PICKUP_HAMMER_FROM_AIR;
			else
				action = Tewi::ACTION_STAND_PICKUP_HAMMER_FROM_AIR;
			this->parentPlayerB->setAction(action);
			SokuLib::playSEWaveBuffer(SokuLib::SFX_SLAP_HIT);
		}
	} else if (this->frameState.sequenceId == 0 || this->frameState.sequenceId == 7) {
		if (this->collisionLimit) {
			if (this->HP <= 0) {
				this->collisionLimit = 0;
				this->customData[3] = 0;
				this->customData[2] = 0;
				this->speed.y = 7;
				this->speed.x = std::copysign(2, -this->speed.x);
				this->gravity.y = 0.25;
				this->setSequence(7);
			} else if (this->checkTurnIntoCrystals(false, 1, 5)) {
				this->collisionLimit = 0;
				this->customData[2] = 0;
			}
		}
		this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
		this->position += this->speed;
		this->speed.y -= this->gravity.y;
	} else if (this->position.x == 1280)
		this->renderInfos.zRotation = this->direction * -90;
	else if (this->position.x == 0)
		this->renderInfos.zRotation = this->direction * 90;
	else {
		this->position.y = this->getGroundHeight();
		this->renderInfos.zRotation = 0;
	}

	this->advanceFrame();
	if (this->frameState.sequenceId != 0 && this->frameState.sequenceId != 7)
		return;
	if (this->position.x <= 40 && this->speed.x <= 0) {
		if (this->customData[3] == 0 && this->speed.x != 0) {
			this->position.x = 40;
			this->speed.x *= -0.8f;
			this->speed.y *= 0.8f;
		} else if (!this->unknown360) {
			this->position.x = 0;
			this->speed.x = 0;
			this->speed.y = 0;
			this->setSequence(1);
			this->renderInfos.zRotation = this->direction * 90;
		} else
			this->position.x = 40;
	}
	if (this->position.x >= 1240 && this->speed.x >= 0) {
		if (this->customData[3] == 0 && this->speed.x != 0) {
			this->position.x = 1240;
			this->speed.x = -this->speed.x * 0.8f;
			this->speed.y = this->speed.y * 0.8f;
		} else if (!this->unknown360) {
			this->position.x = 1280;
			this->speed.x = 0;
			this->speed.y = 0;
			this->setSequence(1);
			this->renderInfos.zRotation = this->direction * -90;
		} else
			this->position.x = 1240;
	}
	if (this->position.y <= this->getGroundHeight() && this->speed.y < 0) {
		if (this->customData[2] != 0) {
			this->customData[2]--;
			this->position.y = this->getGroundHeight();
			this->speed.x = this->speed.x * 0.8f;
			this->speed.y = -this->speed.y * 0.8f;
		} else if (!this->unknown360) {
			this->setSequence(1);
			this->position.y = this->getGroundHeight();
			this->speed.x = 0;
			this->speed.y = 0;
		} else
			this->position.y = this->getGroundHeight();
	}
	if (this->position.y >= 800 && this->speed.y > 0) {
		this->position.y = 800;
		this->speed.x = this->speed.x * 0.8f;
		this->speed.y = -this->speed.y * 0.8f;
	}
	if (this->unknown360)
		this->unknown360--;
}

void Hammer::initializeAction()
{
	if (this->customData[0] >= 360) {
		this->setSequence(this->customData[0] - 360);
		return;
	}
	this->HP = 500;
	this->gravity.y = 1;
	this->collisionLimit = 1;
	this->unknown360 = 4;
	this->speed.x = std::cos(this->customData[0] * M_PI / 180) * this->customData[1];
	this->speed.y = std::sin(this->customData[0] * M_PI / 180) * this->customData[1];
}

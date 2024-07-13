//
// Created by PinkySmile on 08/05/24.
//

#include <cmath>
#include "Hammer.hpp"
#include "Tewi.hpp"

void Hammer::update()
{
	if (this->parentPlayerB->timeStop)
		return;
	if (this->frameState.sequenceId == 8) {
		if (this->checkTurnIntoCrystals(false, 1, 5)) {
			this->setSequence(0);
			this->customData[3] = 0;
			this->customData[2] = 1;
			this->skillIndex = -1;
			return;
		}
		if (this->customData[3] == 0) {
			this->renderInfos.zRotation = fmod(this->renderInfos.zRotation + 30, 360);
			this->position += this->speed;
		}
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
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
			this->speed.x = -this->speed.x * 0.8f;
			this->speed.y = this->speed.y * 0.8f;
		} else {
			this->position.x = 0;
			this->speed.x = 0;
			this->speed.y = 0;
			this->nextSequence();
		}
	}
	if (this->position.x >= 1240 && this->speed.x >= 0) {
		if (this->customData[3] == 0 && this->speed.x != 0) {
			this->position.x = 1240;
			this->speed.x = -this->speed.x * 0.8f;
			this->speed.y = this->speed.y * 0.8f;
		} else {
			this->position.x = 1280;
			this->speed.x = 0;
			this->speed.y = 0;
			this->nextSequence();
		}
	}
	if (this->position.y <= this->getGroundHeight() && this->speed.y < 0) {
		if (this->customData[2] == 0) {
			this->nextSequence();
			this->speed.x = 0;
			this->speed.y = 0;
			this->position.y = 0;
		} else {
			this->customData[2]--;
			this->position.y = this->getGroundHeight();
			this->speed.x = this->speed.x * 0.8f;
			this->speed.y = -this->speed.y * 0.8f;
		}
	}
	if (this->position.y >= 800 && this->speed.y > 0) {
		this->position.y = 800;
		this->speed.x = this->speed.x * 0.8f;
		this->speed.y = -this->speed.y * 0.8f;
	}
}

bool Hammer::initializeAction()
{
	this->HP = 500;
	this->gravity.y = 1;
	this->collisionLimit = 1;
	this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
	this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
	return true;
}

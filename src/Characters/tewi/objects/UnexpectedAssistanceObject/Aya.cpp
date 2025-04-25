//
// Created by PinkySmile on 27/10/24.
//

#include "Aya.hpp"

void UnexpectedAssistance::Aya::update()
{
	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	if (this->collisionType == 5) {
		this->lifetime = 0;
		return;
	}
	if (this->frameState.sequenceId == 3) {
		if (this->gpShort[1] % 3 == 0 && this->gpShort[1] < 181) {
			float params[3] = { 45, static_cast<float>(SokuLib::rand(8) + 40), 1 };

			this->createObject(
				this->frameState.actionId,
				this->direction * (SokuLib::rand(400) - 1000.0) + this->gameData.opponent->position.x,
				SokuLib::rand(100) + 900,
				this->direction, 1, params
			);
		}
		if ((int)this->gpShort[1] % 0x28 == 0) {
			this->parentPlayerB->playSFX(34);
		}
		if (0xb3 < this->gpShort[1]) {
			this->lifetime = 0;
			return;
		}
		this->gpShort[1]++;
		return;
	}
	if (this->frameState.sequenceId == 0) {
		this->speed.x = ((this->customData[0] - this->position.x) / 1280.f) * 40.f * this->direction;
		this->speed.y = ((this->customData[1] - this->position.y) / 1280.f) * 40.f;
		if (this->hp < 0) {
			this->setSequence(2);
			this->speed.x = 0;
			this->speed.y = 0;
			return;
		}
		if (this->frameState.currentFrame >= 120) {
			this->nextSequence();
			this->speed.x = 30.0;
			this->speed.y = -30.0;
		}
	} else if (this->frameState.sequenceId == 1) {
		if (this->hp < 0) {
			this->setSequence(2);
			this->speed.x *= 0.1;
			this->speed.y *= 0.1;
			return;
		}
		if (this->position.y <= this->getGroundHeight() || this->collisionLimit <= 0) {
			this->setSequence(2);
			this->speed.x *= 0.1;
			this->speed.y *= 0.1;
			return;
		}
	} else if (this->frameState.sequenceId == 2) {
		this->speed.y += 0.5;
		this->speed.x += 0.25;
		if (this->position.x >= 1380 || this->position.x <= -100 || this->position.y >= 1000 || this->position.y <= -200) {
			this->lifetime = 0;
			return;
		}
	}
	this->position.x += this->direction * this->speed.x;
	this->position.y += this->speed.y;
	this->advanceFrame();
}

void UnexpectedAssistance::Aya::initializeAction()
{
	if (!this->customData) {
		this->direction = this->parentPlayerB->direction;
		this->setSequence(3);
		return;
	}
	this->collisionLimit = 1;
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 1)
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
}

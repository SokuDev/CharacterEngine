//
// Created by PinkySmile on 29/10/24.
//

#include "Iku.hpp"

void UnexpectedAssistance::Iku::update()
{
	float params[3];
	auto FUN_004096a0 = (float (*)(int))0x4096A0;

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
	switch (this->frameState.sequenceId) {
	case 0:
		this->renderInfos.scale.x += 0.005f;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->gpShort[0] != 0) {
			this->renderInfos.scale.x *= 0.9;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.scale.x <= 0.5) {
				if (this->renderInfos.color.a < 10) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 10;
				this->renderInfos.color.r -= 10;
				this->renderInfos.color.g -= 10;
			}
		}
		if (this->parentB == nullptr) {
			this->gpShort[0] = 1;
			this->collisionLimit = 0;
		} else {
			if (this->frameState.currentFrame % 0x3c == 0) {
				params[0] = 0;
				params[1] = 0;
				params[2] = 1;
				this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
			if (this->parentB->gpShort[0] == 1) {
				this->gpShort[0] = 1;
				this->collisionLimit = 0;
				break;
			}
			this->position.x = SokuLib::cos(-this->customData[0]) * this->customData[1] + this->parentB->position.x * this->direction;
			this->position.y = SokuLib::sin(-this->customData[0]) * this->customData[1] + this->parentB->position.y;
			this->customData[0] += 2.0;
			this->customData[1] += this->gpFloat[0];
			this->gpFloat[0] *= 0.97;
			if (this->gpFloat[0] <= 2.0)
				this->gpFloat[0] = 2.0;
			if (this->frameState.currentFrame % 5 == 0) {
				params[0] = SokuLib::rand(360);
				params[1] = SokuLib::rand(7) + 4;
				params[2] = 3;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
		}
		break;
	case 1:
		if (this->parentB == nullptr) {
			this->lifetime = 0;
			return;
		}
		this->position.x = this->parentB->position.x;
		this->position.y = this->parentB->position.y;
		this->renderInfos.scale.x += 0.02;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (0xe < this->frameState.currentFrame) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
			this->renderInfos.color.r -= 10;
			this->renderInfos.color.g -= 10;
		}
		break;
	case 2:
		if (this->gpShort[0] != 0) {
			this->renderInfos.scale.y *= 0.9;
			if (this->renderInfos.scale.y <= 0.5) {
				if (this->renderInfos.color.a < 10) {
					this->lifetime = 0;
					return;
				}
				this->renderInfos.color.a -= 10;
				this->renderInfos.color.r -= 10;
				this->renderInfos.color.g -= 10;
			}
		}
		if (this->parentB == nullptr)
			this->gpShort[0] = 1;
		else {
			if (this->parentB->gpShort[0] != 0) {
				this->gpShort[0] = 1;
				this->collisionLimit = 0;
				return;
			}
			this->renderInfos.scale.y += 0.05;
			if (this->renderInfos.scale.y > 2)
				this->renderInfos.scale.y = 2.0;
			this->position.x = this->parentB->position.x;
			this->position.y = this->parentB->position.y;
			this->renderInfos.scale.x = FUN_004096a0(72) * (this->parentB->customData[1] / 100.0f) * 2;
			this->customData[0] = this->parentB->customData[0] + 162.0;
			this->renderInfos.zRotation = this->customData[0];
			this->setHitBoxData(
				0, this->renderInfos.scale.y * 10.0,
				this->renderInfos.scale.x * 100, this->renderInfos.scale.y * -10.0,
				this->renderInfos.zRotation, 0, 0
			);
		}
		if (this->collisionType != 0) {
			if (this->gpShort[1] < 0xb) {
				this->gpShort[1]++;
				return;
			}
			if (this->collisionLimit > 0) {
				this->gpShort[1]++;
				if (0x13 < this->gpShort[1]) {
					this->gpShort[1] = 0;
					this->collisionType = COLLISION_TYPE_NONE;
				}
			}
		}
		break;
	case 3:
		this->customData[1] *= 0.9;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->renderInfos.scale.x *= 0.9;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (0x18 < this->frameState.currentFrame) {
			if (this->renderInfos.color.a < 5) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 5;
			this->renderInfos.color.r -= 5;
			this->renderInfos.color.g -= 5;
		}
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
		break;
	case 4:
		if (this->gpShort[0] != 0) {
			this->lifetime = 0;
			return;
		}
		this->trackOpponent(0, 1, 100);
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->customData[1] += 0.01;
		if (this->frameState.currentFrame > 0x1EF)
			this->gpShort[0] = 1;
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
	}
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
}

void UnexpectedAssistance::Iku::initializeAction()
{
	float params[3];

	if (!this->customData) {
		this->customData = new float[3];
		this->position.x = SokuLib::rand(2) == 0 ? 1440 : -160;
		this->position.y = SokuLib::rand(900);
		this->customData[0] = this->position.x > 1240 ? 180 : 0;
		this->customData[1] = 3;
		this->customData[2] = 4;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		this->parentPlayerB->playSFX(37);
	}

	this->renderInfos.zRotation = this->customData[0];
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 0) {
		params[0] = this->customData[0] + 162.0;
		params[1] = 0.0;
		params[2] = 2.0;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		this->gpFloat[0] = 8.0;
	}
	if (this->frameState.sequenceId == 2) {
		this->renderInfos.scale.x = 0.0;
		this->collisionLimit = 99;
	}
	if (this->frameState.sequenceId == 3) {
		this->renderInfos.scale.x = SokuLib::rand(5) * 0.1 + 0.75;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
	}
	if (this->frameState.sequenceId == 4) {
		for (int i = 0; i < 5; i++) {
			params[0] = i * 72.0 - 90.0;
			params[1] = 0.0;
			params[2] = 0.0;
			this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		}
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
	}
}

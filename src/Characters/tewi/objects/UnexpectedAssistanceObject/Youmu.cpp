//
// Created by PinkySmile on 12/10/24.
//

#include <cmath>
#include "Youmu.hpp"

void UnexpectedAssistance::Youmu::update()
{
	double angle;
	float params[3];

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	switch(this->frameState.sequenceId) {
	case 0:
		if (this->gpShort[1] == 0) {
			if (this->direction == SokuLib::RIGHT)
				this->speed.x = (this->parentObject->position.x - this->position.x) * 0.1;
			if (this->direction == SokuLib::LEFT)
				this->speed.x = (this->parentObject->position.x - this->position.x) * -0.1;
			this->speed.y = (this->parentObject->position.y - this->position.y) * 0.1;
		} else {
			this->speed.y += 0.02;
			this->speed.x += 0.1;
			if (this->position.x < 0 || this->position.x > 1280 || this->position.y > 1300) {
				this->lifetime = 0;
				this->parentObject->lifetime = 0;
				return;
			}
			this->parentObject->position = this->position;
		}
		this->gpShort[0] = this->gpShort[0] + 2;
		if (this->gpShort[0] >= 360)
			this->gpShort[0] -= 360;

		angle = std::sin(this->gpShort[0] * M_PI / 180);
		angle = angle * 0.2 + 1.0;
		this->renderInfos.scale.x = angle;
		this->renderInfos.scale.y = angle;
		if (this->gpShort[0] % 10 == 0) {
			params[0] = 0;
			params[1] = 0;
			params[2] = 1;
			this->createObject(
				this->frameState.actionId,
				(SokuLib::rand(50) - 25.0) + this->position.x,
				(SokuLib::rand(50) - 25.0) + this->position.y,
				this->direction, -1, params
			);
		}
		// Use the move
		if (
			this->gpShort[1] == 0 &&
			std::pow(this->position.x - this->parentObject->position.x, 2) + std::pow(this->position.y - this->parentObject->position.y, 2) < 2000 &&
			this->parentObject->isOnGround()
		) {
			this->setSequence(6);
			this->gpShort[0] = 0;
			this->parentPlayerB->playSFX(22);
		}
		break;
	case 1:
		if (this->renderInfos.color.a < 0xd) {
			this->lifetime = 0;
			break;
		}
		this->renderInfos.color.a -= 0xd;
		this->renderInfos.scale.x *= 0.94;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->speed.y -= 0.1;
		break;
	case 2:
		this->setHitBoxData(
			this->renderInfos.scale.x * -80, this->renderInfos.scale.x * 80,
			this->renderInfos.scale.x * 80, this->renderInfos.scale.x * -80,
			0, 0, 0
		);
		if (this->renderInfos.color.a < 8) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 8;
		if (this->renderInfos.color.a < 0x41)
			this->collisionLimit = 0;
		this->renderInfos.scale.x = this->customData[1] + this->renderInfos.scale.x;
		this->customData[1] -= 0.15;
		if (this->customData[1] < 0.02)
			this->customData[1] = 0.02;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		break;
	case 3:
		if (this->renderInfos.color.a < 10) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 10;
		this->renderInfos.zRotation += this->gpFloat[0];
		this->gpFloat[0] *= 0.98;
		this->renderInfos.scale.x = this->renderInfos.scale.x + 0.1;
		this->renderInfos.scale.y *= 0.85;
		break;
	case 4:
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1] * this->direction;
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
		this->customData[1] *= 0.9;
		this->renderInfos.scale.x *= 0.98;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->renderInfos.color.a < 10) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 10;
		break;
	case 5:
		if (this->frameState.currentFrame == 30) {
			params[0] = 0;
			params[1] = 0;
			params[2] = 0;
			this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);

			auto target = fmod(atan2(this->gameData.opponent->position.y + 75 - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI + 360, 360);

			this->speed.x = cos(target * M_PI / 180) * 35 * this->direction;
			this->speed.y = sin(target * M_PI / 180) * 35;
			if (this->direction == SokuLib::LEFT) {
				this->renderInfos.zRotation = 180 + target;
			} else
				this->renderInfos.zRotation = 360 - target;
			if (this->renderInfos.zRotation > 360)
				this->renderInfos.zRotation -= 360;
			if (this->renderInfos.zRotation < 0)
				this->renderInfos.zRotation += 360;
		}
		if (this->isOnGround()) {
			this->position.y = this->getGroundHeight();
			this->speed = {0, 0};
			if (this->renderInfos.zRotation > 150)
				this->renderInfos.zRotation -= 180;
			if (this->renderInfos.zRotation > 150)
				this->renderInfos.zRotation = 150;
			if (this->renderInfos.zRotation < 30)
				this->renderInfos.zRotation = 30;
			this->gpShort[0] = 1;
		} else if (this->gpShort[0] == 1) {
			this->renderInfos.zRotation += 2;
			if (this->renderInfos.zRotation > 360)
				this->renderInfos.zRotation -= 360;
		}
		break;
	case 6:
		this->speed.x *= 0.5;
		this->speed.y *= 0.5;
		if (this->frameState.currentFrame > 9) {
			this->speed.x = 0.0;
			this->speed.y = 0.0;
			if (this->gpShort[0] < 5) {
				if ((int)this->frameState.currentFrame % 7 == 0) {
					params[0] = this->gpShort[0] * 20;
					params[1] = 10 * 0.05 + 0.4;
					params[2] = 2;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
					this->gpShort[0]++;
				}
				if (this->frameState.currentFrame % 10 == 0) {
					params[0] = SokuLib::rand(360);
					params[1] = 0;
					params[2] = 3;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				}
				if (this->frameState.currentFrame % 2 == 0) {
					params[0] = SokuLib::rand(360);
					params[1] = SokuLib::rand(60) * 0.1 + 10;
					params[2] = 4;
					this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
				}
			}
		}
		if (this->frameState.currentFrame > 0x57) {
			this->setSequence(0);
			this->gpShort[0] = 0;
			this->gpShort[1] = 1;
			return;
		}
		break;
	}
	this->position.x = this->direction * this->speed.x + this->position.x;
	this->position.y = this->speed.y + this->position.y;
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
}

void UnexpectedAssistance::Youmu::initializeAction()
{
	if (!this->customData) {
		this->direction = SokuLib::rand(2) == 0 ? SokuLib::LEFT : SokuLib::RIGHT;
		this->collisionLimit = 1;
		this->position.y = 1200;
		this->position.x = SokuLib::rand(1200) + 40;
		this->parentPlayerB->playSFX(23);
		this->setSequence(5);
		return;
	}
	this->renderInfos.zRotation = this->customData[0];
	this->setSequence(this->customData[2]);
	this->unknown1A1 = 1;
	if (this->frameState.sequenceId == 1)
		this->speed.y = 4.0;
	if (this->frameState.sequenceId >= 2 && this->frameState.sequenceId <= 4) {
		if (this->frameState.sequenceId == 2) {
			this->renderInfos.scale.x = 0.5;
			this->renderInfos.scale.y = 0.5;
			this->collisionLimit = 1;
		}
		if (this->frameState.sequenceId == 3) {
			this->renderInfos.zRotation = SokuLib::rand(360);
			this->gpFloat[1] = 6.0 - SokuLib::rand(12);
			this->renderInfos.scale.x = 0.5;
			this->renderInfos.scale.y = 0.5;
		}
		if (this->frameState.sequenceId != 4)
			return;
		this->renderInfos.scale.x = SokuLib::rand(10) * 0.1 + 0.5;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->renderInfos.zRotation = SokuLib::rand(360);
	}
}

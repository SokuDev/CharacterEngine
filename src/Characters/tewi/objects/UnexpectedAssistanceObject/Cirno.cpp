//
// Created by PinkySmile on 02/11/24.
//

#include "Cirno.hpp"

void UnexpectedAssistance::Cirno::update()
{
	float params[3];

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	switch (this->frameState.sequenceId) {
	case 0:
		if (this->gpShort[0] != 0) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
			break;
		}
		this->renderInfos.scale.x *= 0.95;
		if (this->renderInfos.scale.x < 1)
			this->renderInfos.scale.x = 1;
		this->renderInfos.scale.y = 1 / this->renderInfos.scale.x;
		if (this->frameState.poseId == 0) {
			this->position.y += this->speed.y;
			this->speed.y -= 1;
			if (!this->isOnGround())
				break;
			this->position.y = this->getGroundHeight();
			this->speed.y = 0;
			this->nextPose();
			for (int i = 0; i < 16; i++) {
				params[0] = -SokuLib::rand(160) - 10.0;
				params[1] = SokuLib::rand(4) + 10;
				params[2] = (50 < SokuLib::rand(100)) + 1.0;
				this->createObject(this->frameState.actionId, this->direction * (SokuLib::cos(-params[0]) * 135.0 + 200.0) + this->position.x, this->position.y, this->direction, 1, params);
			}
			params[2] = 4.0;
			for (int i = 0; i < 8; i++) {
				params[0] = i * 45.0f + 22.5f;
				params[1] = SokuLib::rand(4) + 16.f;
				this->createObject(
					this->frameState.actionId,
					this->direction * (SokuLib::cos(-params[0]) * 80.0 + 200.0) + this->position.x,
					SokuLib::sin(-params[0]) * 15.0 + this->position.y,
					this->direction,
					(0.0 >= SokuLib::sin(-params[0])) * -2 + 1,
					params
				);
			}
			params[2] = 8.0;
			for (int i = 0; i < 15; i++) {
				params[0] = SokuLib::rand(50) / 50.f + 0.5f;
				params[1] = SokuLib::rand(50) + 10;
				this->createObject(this->frameState.actionId, this->position.x + (SokuLib::rand(2000) / 50.f - 100.f), this->position.y, this->direction, 1, params);
			}
			params[2] = 7.0;
			for (int i = 0; i < 45; i++) {
				params[0] = SokuLib::rand(50) / 50.f + 0.5f;
				params[1] = SokuLib::rand(50) + 10;
				this->createObject(this->frameState.actionId, this->position.x + (SokuLib::rand(2000) / 50.f - 100.f), this->position.y, this->direction, 1, params);
			}
			params[2] = 5.0;
			this->parentPlayerB->playSFX(43);
			this->createObject(this->frameState.actionId, (this->direction * 200) + this->position.x, this->position.y, this->direction, -1, params);
			this->renderInfos.scale.x = 1.5;
			this->renderInfos.scale.y = 1 / this->renderInfos.scale.x;
		}
		if (this->frameState.poseId == 2 && this->frameState.poseFrame >= 60)
			this->gpShort[0] = 1;
		break;
	case 1:
	case 2:
		this->renderInfos.scale.x -= 0.001;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->renderInfos.zRotation += this->gpFloat[0];
		this->speed.y -= 0.5;
		if (this->renderInfos.color.a < 4 || this->position.x >= 1380.0 || this->position.x <= -100.0) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 4;
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
		break;
	case 3:
		this->renderInfos.scale.x *= 0.995;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->renderInfos.zRotation += this->gpFloat[0];
		this->gpFloat[0] *= 0.99;
		this->gpFloat[1] -= 0.1;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->customData[1] *= 0.95;
		if (this->customData[1] < 1.0)
			this->customData[1] = 1.0;
		if (this->customData[1] == 1.0) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		this->speed.y += this->gpFloat[1];
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
		break;
	case 4:
		this->renderInfos.scale.x += 0.001;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->renderInfos.zRotation = this->gpFloat[0] + this->renderInfos.zRotation;
		this->gpFloat[0] *= 0.95;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->customData[1] *= 0.95;
		if (this->customData[1] < 1.0)
			this->customData[1] = 1.0;
		if (this->customData[1] == 1.0) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y * 0.15 + this->position.y;
		break;
	case 5:
		this->renderInfos.scale.x += 0.3;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->renderInfos.color.a < 20) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 20;
		break;
	case 6:
		this->renderInfos.scale.x *= 0.9;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->renderInfos.color.a < 30)  {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 30;
		break;
	case 7:
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y += this->speed.y;
		this->speed.y -= this->customData[0];
		this->renderInfos.zRotation += this->customData[1];
		if (this->isOnGround() && this->speed.y < 0) {
			this->lifetime = 0;
			return;
		}
		break;
	case 8:
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y += this->speed.y;
		this->speed.y -= this->customData[0];
		this->renderInfos.zRotation += this->customData[1];
		if (this->isOnGround() && this->speed.y < 0) {
			this->lifetime = 0;
			params[0] = 0;
			params[1] = 0;
			params[2] = 1;
			this->createObject(1016, this->position.x, this->position.y, this->direction, 1, params);
			this->parentPlayerB->playSFX(45);
			return;
		}
		break;
	}
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
}

void UnexpectedAssistance::Cirno::initializeAction()
{
	if (this->customData)
		this->setSequence(this->customData[2]);
	else
		this->parentPlayerB->playSFX(44);
	if (this->frameState.sequenceId == 0) {
		this->collisionLimit = 1;
		this->position.x = this->gameData.opponent->position.x;
		this->position.y = 1200;
		this->speed.y = 15;
	} else if (this->frameState.sequenceId == 1 || this->frameState.sequenceId == 2) {
		this->gpFloat[0] = SokuLib::rand(11) - 5.0;
		this->renderInfos.zRotation = SokuLib::rand(360);
		this->renderInfos.scale.x = SokuLib::rand(6) * 0.1 + 0.8;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		updateSpeedAngled(this->customData[0], this->customData[1]);
	} else if (this->frameState.sequenceId == 3) {
		this->gpFloat[0] = SokuLib::rand(5) - 2.0;
		this->renderInfos.zRotation = SokuLib::rand(360);
		this->renderInfos.scale.x = SokuLib::rand(0xf) * 0.1 + 0.5;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
	} else if (this->frameState.sequenceId == 4) {
		this->gpFloat[0] = SokuLib::rand(11) - 5.0;
		this->renderInfos.zRotation = SokuLib::rand(360);
		this->renderInfos.scale.x = SokuLib::rand(3) * 0.1 + 0.9;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
	} else if (this->frameState.sequenceId == 7 || this->frameState.sequenceId == 8) {
		this->speed.x = SokuLib::rand(5000) / 100.f - 25;
		this->speed.y = SokuLib::rand(1000) / 100.f + 15;
	}
}

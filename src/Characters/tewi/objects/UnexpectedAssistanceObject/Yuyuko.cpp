//
// Created by PinkySmile on 15/10/24.
//

#include "Yuyuko.hpp"

void UnexpectedAssistance::Yuyuko::update()
{
	float params[3];

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
	case 1:
	case 2:
	case 3:
		if (this->collisionType != COLLISION_TYPE_NONE || this->frameState.currentFrame > 180) {
			this->lifetime = this->lifetime + -1;
			for (int i = 0; i < 4; i++) {
				params[0] = SokuLib::rand(360);
				params[1] = SokuLib::rand(50) * 0.1 + 5.0;
				params[2] = this->frameState.sequenceId + 4;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
			return;
		}
		if (1.0 <= this->renderInfos.scale.x)
			this->renderInfos.scale.x += 0.01;
		else {
			this->renderInfos.scale.x += 0.02;
			if (this->renderInfos.scale.x > 1.0)
				this->renderInfos.scale.x = 1.0;
		}
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->customData[1] += 0.2;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->renderInfos.zRotation = this->customData[0];
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		this->renderInfos.zRotation += this->gpFloat[0];
		this->speed.x *= 0.97;
		this->speed.y -= 0.15;
		this->renderInfos.scale.x -= 0.01;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (0xe < this->frameState.currentFrame) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		this->speed.x = 0.0;
		this->speed.y = 0.0;
		if (!this->parentB) {
			this->lifetime = 0;
			return;
		}
		this->position.x = this->direction * this->parentB->speed.x + this->parentB->position.x;
		this->position.y = this->parentB->speed.y + this->parentB->position.y;
		this->renderInfos.scale.x = SokuLib::sin(this->frameState.currentFrame * 6.0) * 0.5 + 1.0;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		if (this->collisionType != COLLISION_TYPE_NONE || this->frameState.currentFrame > 180) {
			this->lifetime = this->lifetime + -1;
			for (int i = 0; i < 3; i++) {
				params[0] = SokuLib::rand(360);
				params[1] = SokuLib::rand(50) * 0.1 + 5.0;
				params[2] = this->frameState.sequenceId + 4;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
			return;
		}
		this->customData[1] *= 0.92;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->renderInfos.zRotation = this->customData[0];
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		this->renderInfos.scale.x *= 0.98;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (0xe < this->frameState.currentFrame) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		break;
	case 20:
		if (this->frameState.poseId == 0)
			break;
		if (this->frameState.currentFrame % 10 == 0)
			this->parentPlayerB->playSFX(26);
		for (int i = 0; i < 4; i++) {
			SokuLib::Vector2f pos;
			float angle;

			switch (SokuLib::rand(3)) {
			case 0:
				pos.x = 0;
				pos.y = SokuLib::rand(1000);
				angle = (-pos.y * 90 / 1000) + SokuLib::rand(90);
				break;
			case 1:
				pos.x = 1280;
				pos.y = SokuLib::rand(1000);
				angle = (90 - pos.y * 90 / 1000) + SokuLib::rand(90) + 90;
				break;
			default:
				pos.x = SokuLib::rand(1280);
				pos.y = 1000;
				angle = pos.y * 90 / 1280 + SokuLib::rand(90);
				break;
			}
			params[0] = angle;
			params[1] = SokuLib::rand(50) * 0.1 + 5.0;
			params[2] = SokuLib::rand(4);
			if (params[2] >= 16)
				params[2] = 15;
			this->createObject(this->frameState.actionId, pos.x, pos.y, this->direction, 1, params);
		}
		break;
	}
	if (this->position.x >= 1380.0 || this->position.x <= -100.0 || this->position.y >= 1060.0 || this->position.y <= -100.0) {
		this->lifetime = 0;
		return;
	}
	this->position.x = this->direction * this->speed.x + this->position.x;
	this->position.y = this->speed.y + this->position.y;
	if (!this->advanceFrame())
		return;
	this->lifetime = 0;
}

void UnexpectedAssistance::Yuyuko::initializeAction()
{
	float params[3];

	if (!this->customData) {
		this->setSequence(20);
		this->parentPlayerB->playSFX(27);
		return;
	}
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->collisionLimit = 1;
	this->renderInfos.zRotation = this->customData[0];
	this->setSequence(this->customData[2]);
	if (0 <= this->frameState.sequenceId && this->frameState.sequenceId < 4) {
		this->renderInfos.scale.x = 0.5;
		this->renderInfos.scale.y = 0.5;
		params[0] = 0.0;
		params[1] = 0.0;
		params[2] = this->frameState.sequenceId + 8;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
	}
	if (4 <= this->frameState.sequenceId && this->frameState.sequenceId < 8) {
		this->gpFloat[0] = SokuLib::rand(100) * 0.1 - 5.0;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->renderInfos.scale.x = SokuLib::rand(10) * 0.1 + 0.5;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
	}
	if (12 <= this->frameState.sequenceId && this->frameState.sequenceId < 16) {
		params[0] = 0.0;
		params[1] = 0.0;
		params[2] = this->frameState.sequenceId - 4;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
	}
}

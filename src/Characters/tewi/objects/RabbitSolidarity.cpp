//
// Created by PinkySmile on 07/09/24.
//

#include "RabbitSolidarity.hpp"
#include "../Tewi.hpp"

void RabbitSolidarity::_hammerUpdate()
{
	float params[3];

	if (this->frameState.currentFrame == 90) { // R f.5A
		params[2] = 1;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, 0, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 115) { // T j.5A
		params[2] = 8;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 250 * this->direction, this->gameData.opponent->position.y - 40, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 135) { // R j.2B
		params[2] = 15;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, this->gameData.opponent->position.y + 20, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 170) { // T j.8A
		params[2] = 14;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 100 * this->direction, this->gameData.opponent->position.y + 20, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 220) { // R j.5A
		params[2] = 7;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 100 * this->direction, this->gameData.opponent->position.y - 20, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 235) { // T j.2A
		params[2] = 9;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 200 * this->direction, this->gameData.opponent->position.y + 20, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 265) { // R j.6A
		params[2] = 16;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, 50, this->direction, 2, params);
	}
	if (this->frameState.currentFrame >= 270) { // Object death
		if (this->renderInfos.color.a <= 20)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 20;
	} else if (this->renderInfos.scale.x < 6) {
		this->renderInfos.scale.x += 0.2;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
	}
}

void RabbitSolidarity::_hammerlessUpdate()
{
	float params[3];

	if (this->frameState.currentFrame == 90) { // R f.5A
		params[2] = 1;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, 0, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 115) { // T j.5A
		params[2] = 8;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, this->gameData.opponent->position.y, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 135) { // R j.2B
		params[2] = 15;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, this->gameData.opponent->position.y + 20, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 170) { // T j.6A
		params[2] = 14;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 300 * this->direction, this->gameData.opponent->position.y + 80, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 182) { // R j.5A
		params[2] = 7;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 100 * this->direction, this->gameData.opponent->position.y - 20, this->direction, 2, params);
	}
	/*
	if (this->frameState.currentFrame == 200) { // T j.5A
		params[2] = 8;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, this->gameData.opponent->position.y + 20, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 225) { // R j.5A
		params[2] = 7;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 100 * this->direction, this->gameData.opponent->position.y - 20, this->direction, 2, params);
	}
	*/
	if (this->frameState.currentFrame == 200) { // T Dive Kick
		params[2] = 9;
		this->parentPlayerB->direction = -this->parentPlayerB->direction;
		this->direction = this->parentPlayerB->direction;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 250 * this->direction, this->gameData.opponent->position.y + 160, this->direction, 2, params);
	}
	if (this->frameState.currentFrame == 230) { // R j.6A
		params[2] = 16;
		this->createObject(this->frameState.actionId, this->gameData.opponent->position.x - 150 * this->direction, 50, this->direction, 2, params);
	}
	if (this->frameState.currentFrame >= 250) { // Object death
		if (this->renderInfos.color.a <= 20)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 20;
	} else if (this->renderInfos.scale.x < 6) {
		this->renderInfos.scale.x += 0.2;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
	}
}

void RabbitSolidarity::update()
{
	float params[3];

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0: // Reisen j.2A
		if (this->frameState.poseId >= 12) {
			if (this->renderInfos.color.a <= 10)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 10;
		} else {
			if (this->renderInfos.color.a < 243)
				this->renderInfos.color.a += 12;
			else
				this->renderInfos.color.a = 255;
		}
		if (this->collisionType == COLLISION_TYPE_HIT) {
			params[2] = 6;
			((Tewi *)this->parentPlayerB)->setRabbitAnimation();
			this->collisionType = COLLISION_TYPE_NONE;
			this->parentPlayerB->playSFX(19);
			this->createObject(this->frameState.actionId, min(1140, max(140, this->position.x)), 225, this->direction, 2, params);
		}
		if (this->frameState.poseId == 10 && this->frameState.poseFrame == 0)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		this->speed -= this->gravity;
		if (this->position.y <= this->getGroundHeight() + 100 && this->frameState.poseId < 12) {
			this->position.y = this->getGroundHeight();
			this->speed.y = 0;
		}
		if (this->position.x >= 1240)
			this->position.x = 1240;
		if (this->position.x <= 40)
			this->position.x = 40;
		if (
			this->gameData.opponent->frameState.actionId >= SokuLib::ACTION_AIR_HIT_SMALL_HITSTUN &&
			this->gameData.opponent->frameState.actionId <= SokuLib::ACTION_AIR_HIT_HUGE_HITSTUN &&
			this->gameData.opponent->speed.y > 0
		)
			this->speed -= this->gravity;
		break;
	case 6: // Reisen d623 effect
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->parentPlayerB->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206) {
			if (this->frameState.currentFrame >= 220)
				SokuLib::camera.forcedYCenter = 50;
			else if (this->frameState.currentFrame >= 183)
				SokuLib::camera.forcedYCenter = 212;
			else if (this->frameState.currentFrame >= 150)
				SokuLib::camera.forcedYCenter = 150.f + (this->frameState.currentFrame - 150.f) * 4;
			else if (this->frameState.currentFrame >= 100)
				SokuLib::camera.forcedYCenter = this->frameState.currentFrame;
			else {
				if (this->gameData.opponent->position.x < 240)
					this->gameData.opponent->position.x += 2;
				if (this->gameData.opponent->position.x > 1040)
					this->gameData.opponent->position.x -= 2;
			}
			this->_hammerlessUpdate();
		} else {
			if (this->frameState.currentFrame >= 250)
				SokuLib::camera.forcedYCenter = 50;
			else if (this->frameState.currentFrame >= 183)
				SokuLib::camera.forcedYCenter = 212;
			else if (this->frameState.currentFrame >= 150)
				SokuLib::camera.forcedYCenter = 150.f + (this->frameState.currentFrame - 150.f) * 4;
			else if (this->frameState.currentFrame >= 100)
				SokuLib::camera.forcedYCenter = this->frameState.currentFrame;
			else {
				if (this->gameData.opponent->position.x < 240)
					this->gameData.opponent->position.x += 2;
				if (this->gameData.opponent->position.x > 1040)
					this->gameData.opponent->position.x -= 2;
			}
			this->_hammerUpdate();
		}
		break;
	case 1: // Reisen f.5A
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.poseId >= 8) {
			if (this->renderInfos.color.a <= 26)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 26;
		} else {
			if (this->renderInfos.color.a < 215)
				this->renderInfos.color.a += 40;
			else
				this->renderInfos.color.a = 255;
		}
		if (this->speed.x > 0)
			this->speed.x -= 0.5;
		if (this->frameState.poseId == 4 && this->frameState.poseFrame == 0) {
			params[2] = 2;
			this->createChild(this->frameState.actionId, this->gameData.opponent->position.x, 0, this->direction, 2, params);
		}
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0)
			this->parentPlayerB->playSFX(17);
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		this->speed.y -= this->gravity.y;
		break;
	case 7: // Reisen j.5A
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.poseId >= 7) {
			if (this->renderInfos.color.a <= 22)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 22;
		} else {
			if (this->renderInfos.color.a < 215)
				this->renderInfos.color.a += 40;
			else
				this->renderInfos.color.a = 255;
		}
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		break;
	case 8: // Tewi j.5A
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.poseId >= 4) {
			if (this->renderInfos.color.a <= 12)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 12;
		} else {
			if (this->renderInfos.color.a < 215)
				this->renderInfos.color.a += 40;
			else
				this->renderInfos.color.a = 255;
		}
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
			SokuLib::playSEWaveBuffer(SokuLib::SFX_MEDIUM_ATTACK);
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		if (this->parentPlayerB->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206)
			this->speed.y -= 0.6;
		else
			this->speed.y -= 1;
		break;
	case 9: // Tewi j.2A / Dive Kick
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->parentPlayerB->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206) {
			if (this->frameState.poseId == 2 && this->frameState.poseFrame == 0) {
				this->speed.x = 5;
				this->speed.y = -20;
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			}
			this->position.x += this->speed.x * this->direction;
			this->position.y += this->speed.y;
			if (this->frameState.poseId >= 6) {
				if (this->renderInfos.color.a <= 15)
					this->lifetime = 0;
				else
					this->renderInfos.color.a -= 15;
			} else {
				if (this->renderInfos.color.a < 215)
					this->renderInfos.color.a += 40;
				else
					this->renderInfos.color.a = 255;
			}
		} else {
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
			if (this->frameState.poseId >= 5) {
				if (this->renderInfos.color.a <= 15)
					this->lifetime = 0;
				else
					this->renderInfos.color.a -= 15;
			} else {
				if (this->renderInfos.color.a < 215)
					this->renderInfos.color.a += 40;
				else
					this->renderInfos.color.a = 255;
			}
		}
		break;
	case 14: // Tewi j.8A/j.6A
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->parentPlayerB->frameState.actionId == SokuLib::ACTION_USING_SC_ID_206) {
			if (this->frameState.poseId >= 4) {
				if (this->renderInfos.color.a <= 20)
					this->lifetime = 0;
				else
					this->renderInfos.color.a -= 20;
			} else {
				if (this->renderInfos.color.a < 215)
					this->renderInfos.color.a += 40;
				else
					this->renderInfos.color.a = 255;
			}
			if (this->frameState.poseId < 3)
				this->speed = {12, 0};
			else if (this->frameState.poseId >= 4 && this->speed.x > 0)
				this->speed.x -= 0.5;
			else if (this->frameState.poseFrame == 0 && this->frameState.poseId == 3)
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
		} else {
			if (this->frameState.poseId >= 7) {
				if (this->renderInfos.color.a <= 22)
					this->lifetime = 0;
				else
					this->renderInfos.color.a -= 22;
			} else {
				if (this->renderInfos.color.a < 215)
					this->renderInfos.color.a += 40;
				else
					this->renderInfos.color.a = 255;
			}
			if (this->frameState.poseFrame == 0 && this->frameState.poseId == 4) {
				SokuLib::playSEWaveBuffer(SokuLib::SFX_HEAVY_ATTACK);
				this->speed.y = 15;
			}
		}
		this->position.x += this->speed.x * this->direction;
		this->position.y += this->speed.y;
		this->speed.y -= this->gravity.y;
		break;
	case 15: // Reisen j.2B
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.poseId >= 12) {
			if (this->renderInfos.color.a <= 17)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 17;
		} else {
			if (this->renderInfos.color.a < 215)
				this->renderInfos.color.a += 40;
			else
				this->renderInfos.color.a = 255;
		}

		if (this->frameState.poseFrame == 0) {
			if (this->frameState.poseId != 4 && this->frameState.poseId != 7 && this->frameState.poseId != 10)
				break;

			float angle = atan2(this->gameData.opponent->position.y - this->position.y, this->direction * (this->gameData.opponent->position.x - this->position.x)) * 180 / M_PI;

			params[0] = (SokuLib::rand(9) - 4.0) + angle;
			params[1] = 25;
			params[2] = 10;
			this->createObject(this->frameState.actionId, (this->direction * 0x38) + this->position.x, this->position.y + 100.0, this->direction, 1, params);
			params[2] = 11;
			this->createObject(this->frameState.actionId, (this->direction * 0x38) + this->position.x, this->position.y + 100.0, this->direction, 1, params);
		}
		break;
	case 16: // Reisen j.6A
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.poseId >= 8) {
			if (this->renderInfos.color.a <= 21)
				this->lifetime = 0;
			else
				this->renderInfos.color.a -= 21;
		} else {
			if (this->renderInfos.color.a < 215)
				this->renderInfos.color.a += 40;
			else
				this->renderInfos.color.a = 255;
		}
		if (this->frameState.poseId == 5 && this->frameState.poseFrame == 0) {
			params[2] = 4;
			this->createChild(this->frameState.actionId, this->gameData.opponent->position.x, 0, this->direction, 2, params);
		}
		if (this->frameState.poseId == 6 && this->frameState.poseFrame == 0) {
			this->parentPlayerB->playSFX(17);
			this->speed.x = -7;
			this->speed.y = 6;
			this->gravity.y = 0.5;
		}
		break;
	case 10:
		this->customData[1] -= 0.5;
		if (this->customData[1] < 3.0)
			this->customData[1] = 3.0;
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
		if (this->position.y < -100) {
			this->lifetime = 0;
			return;
		}
		if (this->frameState.currentFrame > 120 || this->collisionType) {
			for (int i = 0; i < 5; i++) {
				params[0] = SokuLib::rand(360);
				params[1] = SokuLib::rand(15) + 10;
				params[2] = 12;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
			this->lifetime = 0;
			return;
		}
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
		break;
	case 11:
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.scale.x = this->renderInfos.scale.x - 0.1;
		this->renderInfos.scale.y = this->renderInfos.scale.y - 0.1;
		break;
	case 12:
		this->renderInfos.scale.x = this->renderInfos.scale.x - 0.01;
		this->renderInfos.scale.y = this->renderInfos.scale.y - 0.01;
		if (this->renderInfos.scale.x < 0.5 || 10 < this->frameState.currentFrame) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		if (this->renderInfos.scale.x <= 0.0 || this->renderInfos.color.a == 0) {
			this->lifetime = 0;
			return;
		}
		this->customData[1] -= 1.25;
		if (this->customData[1] < 1.0)
			this->customData[1] = 1.0;
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1];
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
		if (this->position.y < -100) {
			this->lifetime = 0;
			return;
		}
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
		break;
	case 13:
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		if (this->parentObject == nullptr || this->parentObject->frameState.sequenceId != 10)  {
			this->lifetime = 0;
			return;
		}
		this->lifetime = this->parentObject->lifetime;
		this->direction = this->parentObject->direction;
		this->position.x = this->parentObject->direction * this->parentObject->speed.x + this->parentObject->position.x;
		this->position.y = this->parentObject->speed.y + this->parentObject->position.y;
		this->renderInfos.zRotation = this->parentObject->renderInfos.zRotation;
		this->renderInfos.color.a = this->parentObject->renderInfos.color.a;
		break;
	case 2: // Reisen f.5A effect
	case 4: // Reisen j.6A effect
		if (this->frameState.currentFrame == 0) {
			this->lifetime = 0;
			return;
		}
		this->position = this->parentObject->position;
		if (this->renderInfos.color.a < 210)
			this->renderInfos.color.a += 45;
		else
			this->renderInfos.color.a = 255;
		break;
	case 3: // Reisen f.5A effect (loop)
	case 5: // Reisen j.6A effect (loop)
		this->position = this->parentObject->position;
		if (this->frameState.currentFrame <= 10)
			return;
		if (this->renderInfos.color.a <= 12)
			this->lifetime = 0;
		else
			this->renderInfos.color.a -= 12;
		this->renderInfos.color.b = this->renderInfos.color.a;
		this->renderInfos.color.g = this->renderInfos.color.a;
		break;
	}
}

void RabbitSolidarity::initializeAction()
{
	this->collisionLimit = 1;
	if (!this->customData) {
		this->renderInfos.color.a = 0;
		this->position = this->gameData.opponent->position;
		this->position.y += 100;
		this->gravity = this->gameData.opponent->gravity;
		this->speed = this->gameData.opponent->speed;
		if (this->gameData.opponent->direction != this->direction)
			this->speed.x *= -1;
		if (this->gameData.opponent->isGrounded()) {
			this->gravity.y = 0;
			this->speed.y = 0;
		}
		return;
	}
	this->renderInfos.color.a = 0;
	this->setSequence(this->customData[2]);
	switch (this->frameState.sequenceId) {
	case 6:
		this->renderInfos.color.a = 255;
		break;
	case 7:
		this->speed.x = -1.5;
		this->speed.y = 2;
		break;
	case 8:
		this->speed.x = 2.5;
		this->speed.y = 15;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		this->renderInfos.color.a = 255;
		this->renderInfos.zRotation = -this->customData[0];
		if (this->frameState.sequenceId == 10) {
			float params[3] = { this->renderInfos.zRotation, 0, 13 };

			this->collisionLimit = 1;
			this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
			this->setTail(991, 10.0, 5, 1, 2);
		}
		if (this->frameState.sequenceId == 11) {
			this->parentPlayerB->playSFX(18);
			this->renderInfos.zRotation = SokuLib::rand(360);

			auto scale = SokuLib::rand(20);

			scale = scale * 0.1 + 0.8;
			this->renderInfos.scale.x = scale;
			this->renderInfos.scale.y = scale;
			for (int i = 0; i < 5; i++) {
				float params[3] = { static_cast<float>(SokuLib::rand(360)), static_cast<float>(SokuLib::rand(15) + 10), 12 };

				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
		}
		if (this->frameState.sequenceId == 2) {
			auto scale = SokuLib::rand(40);

			scale = scale * 0.01 + 0.5;
			this->renderInfos.scale.x = scale;
			this->renderInfos.scale.y = scale;
		}
		if (this->frameState.sequenceId == 13) {
			this->renderInfos.scale.x = 2.0;
			return;
		}
		break;
	case 14:
		this->speed.y = -4;
		this->gravity.y = 0.6;
		break;
	}
}

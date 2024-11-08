//
// Created by PinkySmile on 03/11/24.
//

#include "Utsuho.hpp"

void UnexpectedAssistance::Utsuho::update()
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
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		this->checkGrazed(0);
		if (this->frameState.sequenceId != 2 && this->frameState.sequenceId != 5) {
			if (this->frameState.currentFrame % 2 == 0) {
				params[0] = SokuLib::rand(360);
				params[1] = SokuLib::rand(3) + 15;
				params[2] = 7;
				this->createObject(this->frameState.actionId, this->position.x, this->getGroundHeight(), this->direction, (0.0 <= SokuLib::sin(params[0])) * -2 + 1, params);
			}
		}
		if (this->frameState.sequenceId == 1 || this->frameState.sequenceId == 4) {
			if (this->frameState.currentFrame >= 20 || this->collisionLimit == 0) {
				this->nextSequence();
				return;
			}
		}
		break;
	case 6:
		if (this->gpShort[0] != 0) {
			this->renderInfos.scale.x += 0.05;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 15) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 15;
			this->renderInfos.color.r -= 15;
			this->renderInfos.color.g -= 15;
			break;
		}
		if (this->frameState.currentFrame > 120)
			this->gpShort[0] = 1;
		break;
	case 7:
		this->renderInfos.zRotation = this->renderInfos.zRotation + this->gpFloat[0];
		this->gpFloat[0] *= 0.9;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->customData[1] *= 0.94;
		if (this->customData[1] < 2.0)
			this->customData[1] = 2.0;
		this->gpFloat[0] += 1;
		this->speed.y += this->gpFloat[0];
		if (this->renderInfos.color.a < 8) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 8;
		this->renderInfos.color.b -= 8;
		this->renderInfos.color.g -= 8;
		this->position.x = this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y * 0.2 + this->position.y;
		break;
	case 8:
		if (this->gpShort[0] != 0) {
			this->renderInfos.scale.x += 0.05;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 15) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 15;
			this->renderInfos.color.r -= 15;
			this->renderInfos.color.g -= 15;
			break;
		}
		if (this->parentB != nullptr) {
			this->position.x = this->parentB->position.x;
			this->position.y = this->parentB->position.y - 106;
			if (this->parentB->frameState.sequenceId != 2 && this->parentB->frameState.sequenceId != 5)
				break;
		}
		this->gpShort[0] = 1;
		break;
	case 9:
		this->renderInfos.scale.x += 0.15;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->renderInfos.color.a < 15) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 15;
		this->renderInfos.color.r -= 15;
		this->renderInfos.color.g -= 15;
		break;
	case 10:
		if (this->frameState.currentFrame < 240) {
			if (this->frameState.currentFrame % 60 == 0)
				this->parentPlayerB->playSFX(47);
			if (this->gpShort[0] == 0) {
				if (this->renderInfos.color.a < 250)
					this->renderInfos.color.a += 5;
				else {
					this->renderInfos.color.a = 255;
					this->gpShort[0] = 1;
				}
			} else {
				this->gpShort[1]++;
				if (this->gpShort[1] > SokuLib::rand(25) + 5) {
					this->gpShort[1] = 0;
					params[0] = 0;
					params[1] = 0;
					params[2] = 0;
					this->parentPlayerB->playSFX(48);
					this->createChild(this->frameState.actionId, SokuLib::rand(1280), 106, this->direction, 1, params);
					this->createChild(this->frameState.actionId, SokuLib::rand(1280), 106, this->direction, 1, params);
				}
			}
		} else if (this->renderInfos.color.a > 8)
			this->renderInfos.color.a -= 8;
		else {
			this->lifetime = 0;
			return;
		}
	}
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
	if (this->frameState.currentFrame != 0)
		return;
	if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 6)
		this->lifetime = 0;
	if (this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 3)
		this->lifetime = 0;
	this->hitStop = 3;
}

void UnexpectedAssistance::Utsuho::initializeAction()
{
	float params[3];

	if (!this->customData) {
		this->setSequence(10);
		this->renderInfos.color.a = 0;
		return;
	}
	this->renderInfos.zRotation = this->customData[0];
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 0 || this->frameState.sequenceId == 3)
		this->collisionLimit = 1;
	if (this->frameState.sequenceId == 7) {
		this->renderInfos.zRotation = SokuLib::rand(360);
		this->gpFloat[0] = SokuLib::rand(12) - 6.0;
	}
}

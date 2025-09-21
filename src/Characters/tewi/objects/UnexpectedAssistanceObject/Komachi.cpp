//
// Created by PinkySmile on 27/10/24.
//

#include "Komachi.hpp"

void UnexpectedAssistance::Komachi::update()
{
	float params[3];
	const auto FUN_00439020 = (void (__thiscall *)(void *, float, float))0x439020;

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;

	if (this->collisionType == COLLISION_TYPE_BULLET_ABSORBED) {
		this->lifetime = 0;
		return;
	}
	switch (this->frameState.sequenceId) {
	case 0:
		this->renderInfos.scale.x *= 0.9;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (0xe < this->frameState.currentFrame) {
			if (this->frameState.currentFrame == 0xf) {
				params[0] = 0;
				params[1] = 0;
				params[2] = 1;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
		}
		break;
	case 1:
		this->checkGrazed(0);
		if (0x1d < (int)this->frameState.currentFrame)
			this->collisionLimit = 0;
		if (this->collisionType != COLLISION_TYPE_NONE) {
			this->gpShort[0]++;
			if (this->gpShort[0] > 2) {
				this->gpShort[0] = 0;
				this->collisionType = COLLISION_TYPE_NONE;
			}
		}
		if (this->gpFloat[0] == 0.0) {
			if (this->renderInfos.color.a < 10) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 10;
			return;
		}
		this->renderInfos.scale.x = 1.0f - this->gpFloat[0];
		this->gpFloat[0] *= 0.75;
		if (this->gpFloat[0] <= 0.01) {
			this->gpFloat[0] = 0;
			params[0] = 0.0;
			params[1] = 0;
			params[2] = 2;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			params[2] = 3;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		}
		break;
	case 2:
		this->gpFloat[0] += 2;
		this->gpFloat[0] += (1.0 - this->renderInfos.scale.x) * 10.0;
		FUN_00439020(this, 0, this->gpFloat[0]);
		if (0x167 < this->frameState.currentFrame || this->gameData.opponent->hp < 1)
			this->gpShort[0] = 1;
		if (this->gpShort[0] == 0) {
			if (std::abs(this->position.x - this->gameData.opponent->position.x) <= 100.0)
				this->gameData.opponent->speedXMultiplier = 0.1;
		} else {
			if (this->renderInfos.color.a < 5) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 5;
			this->renderInfos.scale.x = this->renderInfos.scale.x * 0.98;
		}
		break;
	case 3:
		this->gpFloat[0] += 8;
		this->gpFloat[0] += (1.0 - this->renderInfos.scale.x) * 30.0;
		FUN_00439020(this, 0, this->gpFloat[0]);
		if (0xb3 < this->frameState.currentFrame) {
			if (this->renderInfos.color.a < 5) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 5;
			this->renderInfos.scale.x = this->renderInfos.scale.x * 0.98;
			return;
		};
		break;
	case 4:
		this->renderInfos.scale.x += this->gpFloat[0];
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->gpFloat[0] *= 0.85;
		if (this->renderInfos.color.a < 6) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 5;
		break;
	case 6:
		this->position += this->speed;
		this->speed.y -= 0.5;
		this->renderInfos.zRotation += 35;
		if (this->isOnGround()) {
			this->position.y = this->getGroundHeight();
			this->nextSequence();
			this->renderInfos.zRotation = 0;
		}
		break;
	case 7:
		if (this->frameState.poseFrame == 0 && this->frameState.poseId == 1) {
			params[0] = 0;
			params[1] = 10;
			params[2] = 0;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction,  1, params);
			params[2] = 4;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, -1, params);
			this->parentPlayerB->playSFX(36);
		}
		break;
	}
	if (this->advanceFrame())
		this->lifetime = 0;
}

void UnexpectedAssistance::Komachi::initializeAction()
{
	if (!this->customData) {
		this->setSequence(6);
		this->position.x = SokuLib::rand(1200) + 40;
		this->position.y = 1200;
		this->speed.x = (SokuLib::rand(2400) - this->position.x * 2) / 200.f;
		this->speed.y = 0.0;
		this->parentPlayerB->playSFX(35);
		return;
	}
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->gpFloat[0] = 0.0;
	this->gpFloat[1] = 0.0;
	this->collisionLimit = 0;
	this->gpShort[0] = 0;
	this->gpShort[1] = 0;
	this->gpShort[2] = 0;
	this->gpShort[3] = 0;
	this->gpShort[4] = 0;
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 1) {
		this->collisionLimit = 4;
		this->gpFloat[0] = 1.0;
	}
	if (this->frameState.sequenceId == 4) {
		this->gpFloat[0] = 0.5;
		return;
	}
}

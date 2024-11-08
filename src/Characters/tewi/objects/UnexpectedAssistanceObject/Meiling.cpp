//
// Created by PinkySmile on 03/11/24.
//

#include "Meiling.hpp"

void UnexpectedAssistance::Meiling::update()
{
	auto FUN_0048c750 = (void (__thiscall *)(void *, int))0x48c750;

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	switch (this->frameState.sequenceId) {
	case 0:
		this->customData[0] += 1.0;
		this->customData[1] += 1.5;
		this->position.x = this->direction * SokuLib::cos(-this->customData[0]) * this->customData[1] + this->gpFloat[0];
		this->position.y = SokuLib::sin(-this->customData[0]) * this->customData[1] * 0.05 + this->gpFloat[1];
		if (SokuLib::sin(-this->customData[0]) < 0.0)
			this->layer = 1;
		else
			this->layer = -1;
		FUN_0048c750(this, this->customData[0] + this->frameState.currentFrame);
		if (this->gpShort[0] == 0) {
			this->renderInfos.scale.x += 0.05;
			if (this->renderInfos.scale.x >= 1.0)
				this->renderInfos.scale.x = 1.0;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->customData[1] >= 1500.0) {
				this->gpShort[0] = 1;
				this->collisionLimit = 0;
			}
			if (std::abs(SokuLib::sin(this->customData[0])) > 0.05f) {
				this->collisionType = COLLISION_TYPE_NONE;
				this->collisionLimit = 0;
			} else {
				this->collisionLimit = 1;
				this->checkGrazed(0);
			}
		} else {
			this->collisionLimit = 0;
			this->renderInfos.scale.x *= 0.96;
			this->renderInfos.scale.y *= 0.95;
			if (this->renderInfos.color.a < 5) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 5;
		}
		break;
	case 2:
		this->renderInfos.scale.x += this->gpFloat[1];
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->gpFloat[1] -= 0.05;
		if (this->gpFloat[1] <= 0.01)
			this->gpFloat[1] = 0.01;
		this->renderInfos.zRotation = this->gpFloat[0] + this->renderInfos.zRotation;
		this->gpFloat[0] *= 0.85;
		if (this->renderInfos.color.a < 3) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 3;
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		this->renderInfos.zRotation += this->gpFloat[0];
		this->renderInfos.scale.x *= 0.99;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->speed.y < 0)
			this->speed.y -= 0.05;
		else
			this->speed.y -= 0.75;
		this->speed.x *= 0.98;
		if (5 < (int)this->frameState.currentFrame) {
			this->renderInfos.scale.x *= 0.99;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (this->renderInfos.color.a < 2) {
				this->lifetime = 0;
				return;
			}
			this->renderInfos.color.a -= 2;
		}
		this->position.x = (float)(int)this->direction * this->speed.x + this->position.x;
		this->position.y = this->speed.y + this->position.y;
	}
	if (this->advanceFrame()) {
		this->lifetime = 0;
		return;
	}
}

void UnexpectedAssistance::Meiling::initializeAction()
{
	float params[3];

	if (!this->customData) {
		if (this->parentPlayerB->position.x < 640)
			this->position.x = -60;
		else
			this->position.x = 1340;
		this->parentPlayerB->playSFX(46);
		SokuLib::camera.shake = 10;
		this->customData = new float[3];
		this->customData[0] = 0;
		this->customData[1] = 100;
		this->customData[2] = 0;
		for (int i = 1; i < 60; i++) {
			params[0] = i * 6.0;
			params[1] = 100;
			params[2] = 0;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		}
		params[2] = 2;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	}
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 0) {
		this->gpFloat[0] = this->position.x;
		this->gpFloat[1] = this->position.y;
		this->renderInfos.scale.x = 0.05;
		this->renderInfos.scale.y = 0.05;
		this->position.x = this->direction * SokuLib::cos(-this->customData[0]) * this->customData[1] + this->gpFloat[0];
		this->position.y = SokuLib::sin(-this->customData[0]) * this->customData[1] + this->gpFloat[1];
	}
	if (this->frameState.sequenceId == 2) {
		this->gpFloat[0] = 20.0;
		this->gpFloat[1] = 0.5;
		this->renderInfos.scale.x = 0.25;
		this->renderInfos.scale.y = 0.25;
		this->renderInfos.xRotation = 84.0;
		for (int i = 0; i < 26; i++) {
			params[0] = -30.0 - SokuLib::rand(120);
			params[1] = SokuLib::rand(20) + 10.0;
			params[2] = SokuLib::rand(7) + 3.0;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		}
	}
	if (3 < this->frameState.sequenceId && this->frameState.sequenceId <= 9) {
		this->renderInfos.zRotation = SokuLib::rand(360);
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
		this->renderInfos.scale.x = SokuLib::rand(20) * 0.1 + 1.0;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->gpFloat[0] = 10.0 - SokuLib::rand(20);
	}
}

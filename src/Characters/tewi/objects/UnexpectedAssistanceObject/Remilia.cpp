//
// Created by PinkySmile on 13/10/24.
//

#include "Remilia.hpp"

void UnexpectedAssistance::Remilia::update()
{
	float params[3];

	if (this->hitStop) {
		this->hitStop--;
		return;
	}
	if (this->parentPlayerB->timeStop)
		return;
	switch(this->frameState.sequenceId) {
	case 0:
		break;
	case 1:
		if (this->frameState.poseId == 1 && this->frameState.poseFrame == 0)
			this->parentPlayerB->playSFX(25);
		break;
	case 2:
		if (this->frameState.currentFrame == 0) {
			this->customData[0] = fmod(atan2(this->gameData.opponent->position.y + 75 - this->position.y, this->gameData.opponent->position.x - this->position.x) * 180 / M_PI + 360, 360);
		}
		this->checkGrazed(0);
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1] * this->direction;
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
		if (this->collisionType != COLLISION_TYPE_NONE && this->collisionLimit > 0)
			this->collisionType = COLLISION_TYPE_NONE;
		this->renderInfos.zRotation = -this->customData[0];
		if (1880.0 < this->position.x || this->position.x < -600.0 || this->position.y >= 1880 || this->position.y < -600) {
			this->lifetime = 0;
			return;
		}
		params[0] = this->customData[0];
		params[1] = 5.0;
		params[2] = 3;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		if (
			this->frameState.currentFrame % 2 == 0 &&
			this->position.y >= this->getGroundHeight() &&
			this->position.y - this->getGroundHeight() <= 300.0
		) {
			params[0] = 0;
			params[1] = 0;
			params[2] = 7;
			this->createObject(this->frameState.actionId, this->position.x, this->getGroundHeight(), this->direction, 1, params);
		}
		break;
	case 3:
		if (this->renderInfos.color.a < 5) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 5;
		this->renderInfos.color.g -= 5;
		this->renderInfos.color.b -= 5;
		this->renderInfos.scale.y *= 0.95;
		this->renderInfos.scale.x += 0.01;
		this->renderInfos.zRotation = -this->customData[0];
		this->customData[1] = this->customData[1] * 0.95;
		this->speed.x = cos(this->customData[0] * M_PI / 180) * this->customData[1] * this->direction;
		this->speed.y = sin(this->customData[0] * M_PI / 180) * this->customData[1];
		break;
	case 4:
		if (this->renderInfos.color.a < 5) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.scale.x += this->gpFloat[0];
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->renderInfos.color.g -= 5;
		this->renderInfos.color.b -= 5;
		this->renderInfos.color.a -= 5;
		this->gpFloat[0] *= 0.9;
		break;
	case 5:
		if (this->renderInfos.color.a < 5) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.g -= 5;
		this->renderInfos.color.b -= 5;
		this->renderInfos.color.a -= 5;
		this->renderInfos.scale.x += this->gpFloat[0];
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		this->renderInfos.zRotation -= this->gpFloat[1];
		this->gpFloat[1] *= 0.9;
		this->gpFloat[0] *= 0.9;
		break;
	case 6:
		if (this->parentB) {
			this->lifetime = 0;
			return;
		}
		this->position.x = this->parentB->position.x;
		this->position.y = this->parentB->position.y;
		break;
	case 7:
		if (this->renderInfos.color.a < 5) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.scale.x += this->gpFloat[0];
		this->renderInfos.scale.y *= 0.95;
		this->renderInfos.color.g -= 5;
		this->renderInfos.color.b -= 5;
		this->renderInfos.color.a -= 5;
		this->gpFloat[0] *= 0.9;
	}
	this->position.x += this->speed.x;
	this->position.y += this->speed.y;
	if (this->advanceFrame())
		this->lifetime = 0;
	if (this->frameState.currentFrame == 0 && this->frameState.poseFrame == 0 && this->frameState.poseId == 0 && this->frameState.sequenceId == 2) {
		this->lifetime = 0;
		params[0] = 0;
		params[1] = 100;
		params[2] = 2;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		params[2] = 4;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		params[2] = 5;
		this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	}
}

void UnexpectedAssistance::Remilia::initializeAction()
{
	float params[3];

	if (!this->customData) {
		this->parentPlayerB->playSFX(24);
		this->customData = new float[3];
		this->customData[0] = 0;
		this->customData[1] = 0;
		this->customData[2] = 0;
		switch (SokuLib::rand(2)) {
		case 1:
			this->position.x = -400;
			this->position.y = SokuLib::rand(1280);
			break;
		default:
			this->position.x = 1680;
			this->position.y = SokuLib::rand(1280);
			break;
		}
	}
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 0 || this->frameState.sequenceId == 1) {
		this->unknown360 = 1;
	}
	if (this->frameState.sequenceId == 2) {
		this->renderInfos.zRotation = -this->customData[0];
		this->collisionLimit = 5;
		params[0] = this->customData[0];
		params[1] = 0;
		params[2] = 6;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	}
	if (this->frameState.sequenceId == 3) {
		this->renderInfos.zRotation = -this->customData[0];
	}
	if (this->frameState.sequenceId == 4) {
		this->gpFloat[0] = 0.25;
	}
	if (this->frameState.sequenceId == 5) {
		this->renderInfos.yRotation = 70.0;
		this->gpFloat[0] = 0.4;
		this->gpFloat[1] = 12.0;
	}
	if (this->frameState.sequenceId == 6) {
		this->renderInfos.zRotation = -this->customData[0];
	}
	if (this->frameState.sequenceId == 7) {
		this->gpFloat[0] = 0.4;
	}
}

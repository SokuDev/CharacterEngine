//
// Created by PinkySmile on 05/11/24.
//

#include "Suwako.hpp"

void UnexpectedAssistance::Suwako::update()
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
		if (this->collisionType == COLLISION_TYPE_BULLET_ABSORBED) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.zRotation += 33;
		if (this->frameState.currentFrame % 2 == 0) {
			params[0] = this->renderInfos.zRotation;
			params[1] = 0;
			params[2] = 1;
			this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		}
		if (this->position.x >= 1480 || this->position.x <= -400 || this->position.y >= 1100 || this->position.y <= -300) {
			this->lifetime = 0;
			return;
		}
		this->checkGrazed(10);
		this->checkProjectileHit(2);
		if (this->collisionType != COLLISION_TYPE_NONE) {
			if (
				this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_HIGH_DENSITY ||
				this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_SAME_DENSITY ||
				this->collisionType == COLLISION_TYPE_GRAZED ||
				this->collisionLimit == 0
			) {
				this->setSequence(3);
				this->speed.x *= -0.5;
				this->speed.y = 7.5;
				return;
			}
			this->gpShort[2]++;
			if (3 < this->gpShort[2]) {
				this->collisionType = COLLISION_TYPE_NONE;
				this->gpShort[2] = 0;
			}
		}
		if (this->customData[3] > 0) {
			if (this->position.y - 63.0 <= this->getGroundHeight() && this->speed.y < 0) {
				this->parentPlayerB->playSFX(49);
				this->speed.y *= -1;
				this->customData[3] -= 1;
				params[0] = 0;
				params[1] = 0;
				params[2] = 2;
				this->createObject(this->frameState.actionId, this->position.x, this->getGroundHeight(), this->direction, -1, params);
			}
			if (this->position.y + 63.0 >= SokuLib::camera.topEdge && this->speed.y > 0) {
				this->parentPlayerB->playSFX(49);
				this->speed.y *= -1;
				this->customData[3] -= 1;
			}
			if (
				(this->direction * this->speed.x > 0 && this->position.x + 63 >= SokuLib::camera.rightEdge) ||
				(this->direction * this->speed.x < 0 && this->position.x - 63 <= SokuLib::camera.leftEdge)
			) {
				this->parentPlayerB->playSFX(49);
				this->speed.x *= -1.0;
				this->customData[3] -= 1;
				params[0] = 90.0;
				params[1] = 0;
				params[2] = 2;
				if (this->speed.x < 0.0)
					this->createObject(this->frameState.actionId, this->position.x - (this->direction * 63), this->position.y, this->direction, -1, params);
				else
					this->createObject(this->frameState.actionId, this->position.x + (this->direction * 63), this->position.y, this->direction, -1, params);
			}
		}
		this->position.x += this->direction * this->speed.x;
		this->position.y += this->speed.y;
		break;
	case 1:
		this->renderInfos.zRotation += 10.0;
		this->renderInfos.scale.x += 0.04;
		this->renderInfos.scale.y = this->renderInfos.scale.x;
		if (this->renderInfos.color.a < 12) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 12;
		this->renderInfos.color.g -= 12;
		this->renderInfos.color.b -= 12;
		break;
	case 2:
		this->renderInfos.scale.x = this->renderInfos.scale.x + 0.05;
		this->renderInfos.scale.y = this->renderInfos.scale.y + 0.05;
		if (this->renderInfos.color.a < 11) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 10;
		this->renderInfos.color.g -= 10;
		this->renderInfos.color.b -= 10;
		break;
	case 3:
		this->speed.y -= 0.5;
		this->renderInfos.zRotation += 5;
		this->renderInfos.yRotation += 5;
		this->renderInfos.xRotation += 5;
		if (this->renderInfos.color.a < 11) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.color.a -= 10;
		this->position.x += this->direction * this->speed.x;
		this->position.y += this->speed.y;
		break;
	}
	if (this->advanceFrame())
		this->lifetime = 0;
}

void UnexpectedAssistance::Suwako::initializeAction()
{
	if (!this->customData) {
		this->parentPlayerB->playSFX(50);
		this->customData = new float[4];
		this->customData[0] = SokuLib::rand(360);
		this->customData[1] = 12.5;
		this->customData[2] = 0;
		this->customData[3] = 6;
		this->position.x = SokuLib::rand(1280);
		this->position.y = 1000;
		for (int i = 0; i < 9; i++) {
			this->createObject(this->frameState.actionId, SokuLib::rand(1280), 1000, this->direction, 1, this->customData, 4);
			this->customData[0] = SokuLib::rand(360);
		}
	}
	this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 0) {
		this->collisionLimit = 4;
		this->updateSpeedAngled(this->customData[0], this->customData[1]);
	}
	if (this->frameState.sequenceId == 1 || this->frameState.sequenceId == 2)
		this->renderInfos.zRotation = this->customData[0];
}

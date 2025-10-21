//
// Created by PinkySmile on 18/10/2025.
//

#include "DemonGate.hpp"

#define disappearing gpShort[0]
#define appearTimer gpShort[1]

void DemonGate::update()
{
	float params[3] = {0, 0, 0};

	switch (this->frameState.sequenceId) {
	case 0:
		this->advanceFrame();
		this->position.x = this->parentPlayerB->position.x;
		if (this->disappearing == 0) {
			if (this->renderInfos.scale.x < 1) {
				this->renderInfos.scale.x += 0.035;
				this->renderInfos.scale.y += 0.035;
				this->parentPlayerB->position.y = this->position.y + 425 * this->renderInfos.scale.y;
				if (this->frameState.currentFrame % 4 == 0)
					this->createObject(810, this->position.x + 75 - SokuLib::rand(150), this->position.y, this->direction, 1, params);
			} else if (this->parentPlayerB->frameState.sequenceId == 1) {
				this->parentPlayerB->nextSequence();
				this->renderInfos.scale.x = 1;
				this->renderInfos.scale.y = 1;
			} else if (this->parentPlayerB->frameState.sequenceId >= 3) {
				this->appearTimer++;
			}
			if (this->appearTimer % 9 == 1) {
				params[0] = SokuLib::rand(17) + 1;
				if (params[0] >= 19)
					params[0] = 18;
				this->createObject(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
			}
			if (this->appearTimer == 180) {
				this->parentPlayerB->nextSequence();
				this->disappearing = 1;
			}
		} else {
			this->renderInfos.scale.x -= 0.05;
			this->renderInfos.scale.y -= 0.05;
			if (this->frameState.currentFrame % 4 == 0)
				this->createObject(810, this->position.x + 75 - SokuLib::rand(150), this->position.y, this->direction, 1, params);
			if (this->renderInfos.scale.x <= 0)
				this->lifetime = 0;
		}
		break;
	case 19:
		this->advanceFrame();
		this->position.x = this->parentPlayerB->position.x;
		if (!this->parentObject || this->parentObject->disappearing) {
			this->lifetime = 0;
			return;
		}
		this->renderInfos.scale = this->parentObject->renderInfos.scale;
		break;
	default:
		this->advanceFrame();
		if (this->position.x < -160 || this->position.x > 1480) {
			this->lifetime = 0;
			break;
		}
		if (
			this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_HIGH_DENSITY ||
			this->collisionType == COLLISION_TYPE_GRAZED ||
			this->collisionType == COLLISION_TYPE_ARMORED ||
			this->collisionType == COLLISION_TYPE_BULLET_COLLIDE_SAME_DENSITY
		) {
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		}
		if (this->renderInfos.scale.x < 1) {
			this->renderInfos.scale.x += 0.025;
			this->renderInfos.scale.y += 0.025;
		} else {
			this->renderInfos.scale.x = 1;
			this->renderInfos.scale.y = 1;
		}
		this->position.x += 20 * this->direction;
		if (this->frameState.currentFrame % 4 == 0)
			this->createObject(810, this->position.x + 50 - SokuLib::rand(100), this->position.y, this->direction, 1, params);
		break;
	}
}

void DemonGate::initializeAction()
{
	float params[1] = { 19 };

	this->setSequence(this->customData[0]);
	switch (this->frameState.sequenceId) {
	case 0:
		this->disappearing = 0;
		this->appearTimer = 0;
		this->renderInfos.scale.x = 0;
		this->renderInfos.scale.y = 0;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
		break;
	case 19:
		break;
	default:
		this->renderInfos.scale.x = 0.5;
		this->renderInfos.scale.y = 0.5;
		this->collisionLimit = 1;
		break;
	}
}

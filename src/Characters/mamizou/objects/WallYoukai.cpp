//
// Created by PinkySmile on 13/06/2026.
//

#include "WallYoukai.hpp"

void WallYoukai::update()
{
	float params[3] = {0, 0, 5};

	switch (this->frameState.sequenceId) {
	case 0:
		this->position.x = this->parentPlayerB->position.x - 40 * this->parentPlayerB->direction;
		this->position.y = this->parentPlayerB->position.y + 180;
		if (this->renderInfos.scale.x < 1) {
			this->renderInfos.scale.x += 0.1;
			this->renderInfos.scale.y = this->renderInfos.scale.x;
			if (
				this->parentPlayer->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
				this->parentPlayer->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN
			) {
				this->lifetime = 0;
				return;
			}
			if (this->renderInfos.scale.x >= 1) {
				this->collisionType = COLLISION_TYPE_NONE;
				this->collisionLimit = 1;
			}
		}
		if (
			this->parentPlayer->frameState.actionId >= SokuLib::ACTION_STAND_GROUND_HIT_SMALL_HITSTUN &&
			this->parentPlayer->frameState.actionId < SokuLib::ACTION_RIGHTBLOCK_HIGH_SMALL_BLOCKSTUN
		) {
			this->setSequence(3);
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		} else if (
			this->parentPlayerB->frameState.sequenceId == 2 &&
			this->parentPlayerB->frameState.poseId == 1
		) {
			this->speed.x = 8;
			this->speed.y = 12;
			this->nextSequence();
		}
		break;
	case 1:
		this->speed.y -= this->gravity.y;
		if (this->speed.y <= 0)
			this->nextSequence();
		if (this->collisionType != COLLISION_TYPE_NONE)
			this->gpShort[0] = 1;
		break;
	case 2:
	case 3:
		this->speed.y -= this->gravity.y;
		if (this->speed.y + this->gravity.y >= -5 && this->speed.y < -5 && this->gpShort[0] == 1) {
			this->collisionType = COLLISION_TYPE_NONE;
			this->collisionLimit = 1;
		}
		if (this->isOnGround()) {
			this->position.y = this->getGroundHeight();
			if (this->parentPlayerB->effectiveSkillLevel[7] >= 3) {
				this->createObject(this->frameState.actionId, 0, this->position.y, SokuLib::RIGHT, 1, params);
				SokuLib::playSEWaveBuffer(SokuLib::SFX_CATFISH_LAND);
				SokuLib::camera.shake = this->parentPlayerB->effectiveSkillLevel[7] * 5;
			} else
				this->parentPlayerB->playSFX(2);
			this->setSequence(4);
			this->resetForces();
		}
		break;
	case 4:
		this->advanceFrame();
		if (this->frameState.currentFrame > 20) {
			this->renderInfos.color.a -= 5;
			if (this->renderInfos.color.a == 0)
				this->lifetime = 0;
		}
		break;
	case 5:
		if (this->advanceFrame())
			this->lifetime = 0;
		break;
	case 6:
		if (!this->parentObject || this->parentObject->frameState.sequenceId != 0) {
			this->lifetime = 0;
			return;
		}
		this->position = this->parentObject->position;
		this->renderInfos.scale = this->parentObject->renderInfos.scale;
	}
	this->position.x += this->speed.x * this->direction;
	this->position.y += this->speed.y;
}

void WallYoukai::initializeAction()
{
	if (!this->customData) {
		float params[] = {0, 0, 6};

		this->renderInfos.scale.x = 0;
		this->renderInfos.scale.y = 0;
		this->createChild(this->frameState.actionId, this->position.x, this->position.y, this->direction, 1, params);
	} else
		this->setSequence(this->customData[2]);
	if (this->frameState.sequenceId == 5) {
		this->collisionType = COLLISION_TYPE_NONE;
		this->collisionLimit = 1;
	}
	this->skillIndex = 7;
	this->gravity.y = 1;
}
